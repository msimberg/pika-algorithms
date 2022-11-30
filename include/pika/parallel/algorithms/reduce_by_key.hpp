//  Copyright (c) 2015 John Biddiscombe
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/reduce_by_key.hpp

#pragma once
//
#include <pika/config.hpp>
#include <pika/execution/executors/execution.hpp>
//
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/transform_iterator.hpp>
#include <pika/parallel/algorithms/for_each.hpp>
#include <pika/parallel/algorithms/inclusive_scan.hpp>
#include <pika/parallel/algorithms/sort.hpp>
#include <pika/parallel/container_algorithms/copy.hpp>
#include <pika/parallel/util/result_types.hpp>
#include <pika/parallel/util/zip_iterator.hpp>
//
#include <cstdint>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
//
/// \cond NOINTERNAL
#ifdef EXTRA_DEBUG
#include <iostream>
#define debug_reduce_by_key(a) std::cout << a
#else
#define debug_reduce_by_key(a)
#endif
/// \endcond

namespace pika {
    // reduce_by_key
    namespace detail {
        /// \cond NOINTERNAL
        // -------------------------------------------------------------------
        // simple iterator helper object for access to prev/next items
        // -------------------------------------------------------------------
        struct reduce_stencil_transformer
        {
            // declare result type as a template
            template <typename T>
            struct result;

            // specialize result for iterator type
            template <typename This, typename Iterator>
            struct result<This(Iterator)>
            {
                using element_type =
                    typename std::iterator_traits<Iterator>::reference;
                using type =
                    std::tuple<element_type, element_type, element_type>;
            };

            // call operator for stencil transform
            // it will dereference tuple(it-1, it, it+1)
            template <typename Iterator>
            typename result<reduce_stencil_transformer(Iterator)>::type
            operator()(Iterator const& it) const
            {
                using type =
                    typename result<reduce_stencil_transformer(Iterator)>::type;
                return type(*std::prev(it), *it, *std::next(it));
            }
        };

        // -------------------------------------------------------------------
        // transform iterator using reduce_stencil_transformer helper
        // -------------------------------------------------------------------
        template <typename Iterator,
            typename Transformer = detail::reduce_stencil_transformer>
        class reduce_stencil_iterator
          : public pika::util::transform_iterator<Iterator, Transformer>
        {
        private:
            using base_type =
                pika::util::transform_iterator<Iterator, Transformer>;

        public:
            reduce_stencil_iterator() {}

            explicit reduce_stencil_iterator(Iterator const& it)
              : base_type(it, Transformer())
            {
            }

            reduce_stencil_iterator(Iterator const& it, Transformer const& t)
              : base_type(it, t)
            {
            }
        };

        template <typename Iterator, typename Transformer>
        inline reduce_stencil_iterator<Iterator, Transformer>
        make_reduce_stencil_iterator(Iterator const& it, Transformer const& t)
        {
            return reduce_stencil_iterator<Iterator, Transformer>(it, t);
        }

        // -------------------------------------------------------------------
        // state of a reduce by key step
        // -------------------------------------------------------------------
        struct reduce_key_series_states
        {
            bool start;    // START of a segment
            bool end;      // END of a segment
            reduce_key_series_states(bool s = false, bool e = false)
              : start(s)
              , end(e)
            {
            }
        };

        // -------------------------------------------------------------------
        // callable that actually computes the state using the stencil iterator
        // -------------------------------------------------------------------
        template <typename Transformer, typename StencilIterType,
            typename KeyStateIterType, typename Compare>
        struct reduce_stencil_generate
        {
            using element_type =
                typename Transformer::template result<Transformer(
                    StencilIterType)>::element_type;
            using tuple_type =
                typename Transformer::template result<Transformer(
                    StencilIterType)>::type;
            using KeyStateType =
                typename std::iterator_traits<KeyStateIterType>::reference;

            reduce_stencil_generate() {}

            void operator()(const tuple_type& value, KeyStateType& kiter,
                const Compare& comp) const
            {
                // resolves to a tuple of values for *(it-1), *it, *(it+1)

                element_type left = std::get<0>(value);
                element_type mid = std::get<1>(value);
                element_type right = std::get<2>(value);

                // we need to determine which of three states this
                // index is. It can be:
                // 1. Middle of a set of equivalent keys.
                // 2. Start of a set of equivalent keys.
                // 3. End of a set of equivalent keys.
                // 4. Both the start and end of a set of keys

                {
                    const bool leftMatches(comp(left, mid));
                    const bool rightMatches(comp(mid, right));
                    kiter =
                        reduce_key_series_states(!leftMatches, !rightMatches);
                }
            }
        };

        // -------------------------------------------------------------------
        // helper that extracts the final output iterators from copy_if
        // -------------------------------------------------------------------
        // Zip iterator has 3 iterators inside
        // Iter1, key type : Iter2, value type : Iter3, state type
        template <typename ZIter, typename iKey, typename iVal>
        parallel::detail::in_out_result<iKey, iVal>
        make_pair_result(ZIter zipiter, iKey key_start, iVal val_start)
        {
            // the iterator we want is 'second' part of pair type (from copy_if)
            auto t = zipiter.out.get_iterator_tuple();
            iKey key_end = std::get<0>(t);
            return parallel::detail::in_out_result<iKey, iVal>{key_end,
                std::next(val_start, std::distance(key_start, key_end))};
        }

        // async version that returns future<pair> from future<zip_iterator<blah>>
        template <typename ZIter, typename iKey, typename iVal>
        pika::future<parallel::detail::in_out_result<iKey, iVal>>
        make_pair_result(
            pika::future<ZIter>&& ziter, iKey key_start, iVal val_start)
        {
            using result_type = parallel::detail::in_out_result<iKey, iVal>;

            return pika::make_future<result_type>(
                PIKA_MOVE(ziter), [=](ZIter zipiter) {
                    auto t = zipiter.second.get_iterator_tuple();
                    iKey key_end = std::get<0>(t);
                    return result_type{key_end,
                        std::next(
                            val_start, std::distance(key_start, key_end))};
                });
        }

        // -------------------------------------------------------------------
        // The main algorithm is implemented here, it replaces any async
        // execution policy with a non async one so that no waits are
        // necessry on the internal algorithms. Async execution is handled
        // by the wrapper layer that calls this.
        // -------------------------------------------------------------------
        template <typename ExPolicy, typename RanIter, typename RanIter2,
            typename FwdIter1, typename FwdIter2, typename Compare,
            typename Func>
        static parallel::detail::in_out_result<FwdIter1, FwdIter2>
        reduce_by_key_impl(ExPolicy&& policy, RanIter key_first,
            RanIter key_last, RanIter2 values_first, FwdIter1 keys_output,
            FwdIter2 values_output, Compare&& comp, Func&& func)
        {
            using namespace pika::parallel::detail;
            using namespace pika::util;

            // we need to determine based on the keys what is the keystate for
            // each key. The states are start, middle, end of a series and the special
            // state start and end of the sequence
            std::vector<reduce_key_series_states> key_state;
            using keystate_iter_type =
                std::vector<reduce_key_series_states>::iterator;
            using reducebykey_iter =
                reduce_stencil_iterator<RanIter, reduce_stencil_transformer>;
            using element_type =
                typename std::iterator_traits<RanIter>::reference;
            using zip_ref = typename zip_iterator<reducebykey_iter,
                keystate_iter_type>::reference;
            //
            const std::uint64_t number_of_keys =
                std::distance(key_first, key_last);
            //
            key_state.assign(number_of_keys, reduce_key_series_states());
            {
                reduce_stencil_transformer r_s_t;
                reducebykey_iter reduce_begin =
                    make_reduce_stencil_iterator(key_first, r_s_t);
                reducebykey_iter reduce_end =
                    make_reduce_stencil_iterator(key_last, r_s_t);

                // FIXME: handle cases number_of_keys == 0 and
                //        number_of_keys == 1

                if (number_of_keys == 2)
                {
                    // for two entries, one is a start, the other an end,
                    // if they are different, then they are both start/end
                    element_type left = *key_first;
                    element_type right = *std::next(key_first);
                    key_state[0] =
                        reduce_key_series_states(true, !comp(left, right));
                    key_state[1] =
                        reduce_key_series_states(!comp(left, right), true);
                }
                else
                {
                    // do the first and last elements by hand to simplify the iterator
                    // traversal as there is no prev/next for first/last
                    element_type elem0 = *key_first;
                    element_type elem1 = *std::next(key_first);
                    key_state[0] =
                        reduce_key_series_states(true, elem0 != elem1);
                    // middle elements
                    reduce_stencil_generate<reduce_stencil_transformer, RanIter,
                        keystate_iter_type, Compare>
                        kernel;
                    pika::for_each(policy(pika::execution::non_task),
                        make_zip_iterator(
                            reduce_begin + 1, key_state.begin() + 1),
                        make_zip_iterator(reduce_end - 1, key_state.end() - 1),
                        [&kernel, &comp](zip_ref ref) {
                            kernel(std::get<0>(ref), std::get<1>(ref), comp);
                        });
                    // Last element
                    element_type elemN = *std::prev(key_last);
                    element_type elemn = *std::prev(std::prev(key_last));
                    key_state.back() =
                        reduce_key_series_states(elemN != elemn, true);
                }
            }
            {
                using zip_iterator_in = zip_iterator<RanIter2,
                    std::vector<reduce_key_series_states>::iterator>;
                using zip_type_in = typename zip_iterator_in::value_type;

                using zip_iterator_vout = zip_iterator<FwdIter2,
                    std::vector<reduce_key_series_states>::iterator>;

                using value_type =
                    typename std::iterator_traits<RanIter2>::value_type;

                zip_iterator_in states_begin = make_zip_iterator(
                    values_first, pika::util::begin(key_state));
                zip_iterator_in states_end = make_zip_iterator(
                    values_first + number_of_keys, pika::util::end(key_state));
                zip_iterator_vout states_out_begin = make_zip_iterator(
                    values_output, pika::util::begin(key_state));
                //

                zip_type_in initial;
                //
                using lambda_type =
                    std::tuple<value_type, reduce_key_series_states>;
                pika::inclusive_scan(
                    policy(pika::execution::non_task), states_begin, states_end,
                    states_out_begin,
                    // B is the current entry, A is the one passed in from 'previous'
                    [&func](zip_type_in a, zip_type_in b) -> lambda_type {
                        value_type a_val = std::get<0>(a);
                        reduce_key_series_states a_state = std::get<1>(a);
                        value_type b_val = std::get<0>(b);
                        reduce_key_series_states b_state = std::get<1>(b);
                        debug_reduce_by_key("{ " << a_val << "+" << b_val
                                                 << " },\t" << a_state
                                                 << b_state);
                        // if carrying a start flag, then copy - don't add
                        if (b_state.start)
                        {
                            debug_reduce_by_key(" = " << b_val << std::endl);
                            return std::make_tuple(b_val,
                                reduce_key_series_states(
                                    a_state.start || b_state.start,
                                    b_state.end));
                        }
                        // normal add of previous + this
                        else
                        {
                            debug_reduce_by_key(
                                " = " << func(a_val, b_val) << std::endl);
                            value_type temp = func(a_val, b_val);
                            return std::make_tuple(temp,
                                reduce_key_series_states(
                                    a_state.start || b_state.start,
                                    b_state.end));
                        }
                    },
                    initial);

                // now copy the values and keys for each element that
                // is marked by an 'END' state to the final output
                using zip2_ref = typename pika::util::zip_iterator<RanIter,
                    FwdIter2,
                    std::vector<reduce_key_series_states>::iterator>::reference;

                std::vector<value_type> temp(number_of_keys);
                pika::copy(policy(pika::execution::non_task), values_output,
                    values_output + number_of_keys, temp.begin());

                return make_pair_result(
                    pika::ranges::copy_if(policy(pika::execution::non_task),
                        make_zip_iterator(key_first, temp.begin(),
                            pika::util::begin(key_state)),
                        make_zip_iterator(key_last,
                            temp.begin() + number_of_keys,
                            pika::util::end(key_state)),
                        make_zip_iterator(keys_output, values_output,
                            pika::util::begin(key_state)),
                        // copies to dest only when 'end' state is true
                        [](zip2_ref it) { return std::get<2>(it).end; }),
                    keys_output, values_output);
            }
        }

        ///////////////////////////////////////////////////////////////////////
        // reduce_by_key wrapper struct
        template <typename FwdIter1, typename FwdIter2>
        struct reduce_by_key
          : public parallel::detail::algorithm<
                reduce_by_key<FwdIter1, FwdIter2>,
                parallel::detail::in_out_result<FwdIter1, FwdIter2>>
        {
            reduce_by_key()
              : reduce_by_key::algorithm("reduce_by_key")
            {
            }

            template <typename ExPolicy, typename RanIter, typename RanIter2,
                typename Compare, typename Func>
            static parallel::detail::in_out_result<FwdIter1, FwdIter2>
            sequential(ExPolicy&& policy, RanIter key_first, RanIter key_last,
                RanIter2 values_first, FwdIter1 keys_output,
                FwdIter2 values_output, Compare&& comp, Func&& func)
            {
                return reduce_by_key_impl(PIKA_FORWARD(ExPolicy, policy),
                    key_first, key_last, values_first, keys_output,
                    values_output, PIKA_FORWARD(Compare, comp),
                    PIKA_FORWARD(Func, func));
            }

            template <typename ExPolicy, typename RanIter, typename RanIter2,
                typename Compare, typename Func>
            static typename parallel::detail::algorithm_result<ExPolicy,
                parallel::detail::in_out_result<FwdIter1, FwdIter2>>::type
            parallel(ExPolicy&& policy, RanIter key_first, RanIter key_last,
                RanIter2 values_first, FwdIter1 keys_output,
                FwdIter2 values_output, Compare&& comp, Func&& func)
            {
                return parallel::detail::algorithm_result<ExPolicy,
                    parallel::detail::in_out_result<FwdIter1, FwdIter2>>::
                    get(parallel::execution::async_execute(policy.executor(),
                        pika::util::detail::deferred_call(
                            &reduce_by_key_impl<ExPolicy&&, RanIter, RanIter2,
                                FwdIter1, FwdIter2, Compare&&, Func&&>,
                            policy, key_first, key_last, values_first,
                            keys_output, values_output,
                            PIKA_FORWARD(Compare, comp),
                            PIKA_FORWARD(Func, func))));
            }
        };
        /// \endcond
    }    // namespace detail

#ifdef EXTRA_DEBUG
    std::ostream& operator<<(
        std::ostream& os, const detail::reduce_key_series_states& rs)
    {
        os << "{ start=" << rs.start << ",end=" << rs.end << "} ";
        return os;
    }
#endif

    //-----------------------------------------------------------------------------
    /// Reduce by Key performs an inclusive scan reduction operation on elements
    /// supplied in key/value pairs. The algorithm produces a single output
    /// value for each set of equal consecutive keys in [key_first, key_last).
    /// the value being the
    /// GENERALIZED_NONCOMMUTATIVE_SUM(op, init, *first, ..., *(first + (i - result))).
    /// for the run of consecutive matching keys.
    /// The number of keys supplied must match the number of values.
    ///
    /// \note   Complexity: O(\a last - \a first) applications of the
    ///         predicate \a op.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it applies user-provided function objects.
    /// \tparam RanIter     The type of the key iterators used (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     random access iterator.
    /// \tparam RanIter2    The type of the value iterators used (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     random access iterator.
    /// \tparam FwdIter1    The type of the iterator representing the
    ///                     destination key range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination value range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Compare     The type of the optional function/function object to use
    ///                     to compare keys (deduced).
    ///                     Assumed to be std::equal_to otherwise.
    /// \tparam Func        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a copy_if requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param key_first    Refers to the beginning of the sequence of key elements
    ///                     the algorithm will be applied to.
    /// \param key_last     Refers to the end of the sequence of key elements the
    ///                     algorithm will be applied to.
    /// \param values_first Refers to the beginning of the sequence of value elements
    ///                     the algorithm will be applied to.
    /// \param keys_output  Refers to the start output location for the keys
    ///                     produced by the algorithm.
    /// \param values_output Refers to the start output location for the values
    ///                     produced by the algorithm.
    /// \param comp         comp is a callable object. The return value of the
    ///                     INVOKE operation applied to an object of type Comp,
    ///                     when contextually converted to bool, yields true if
    ///                     the first argument of the call is less than the
    ///                     second, and false otherwise. It is assumed that comp
    ///                     will not apply any non-constant function through the
    ///                     dereferenced iterator.
    /// \param func         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last). This is a
    ///                     binary predicate. The signature of this predicate
    ///                     should be equivalent to:
    ///                     \code
    ///                     Ret fun(const Type1 &a, const Type1 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&.
    ///                     The types \a Type1 \a Ret must be
    ///                     such that an object of type \a FwdIter can be
    ///                     dereferenced and then implicitly converted to any
    ///                     of those types.
    ///
    /// \a comp has to induce a strict weak ordering on the values.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a sequenced_policy execute in sequential order in the
    /// calling thread.
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_policy or \a parallel_task_policy are
    /// permitted to execute in an unordered fashion in unspecified
    /// threads, and indeterminately sequenced within each thread.
    ///
    /// \returns  The \a reduce_by_key algorithm returns a
    ///           \a pika::future<pair<Iter1,Iter2>> if the execution policy is of
    ///           type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and returns \a pair<Iter1,Iter2>
    ///           otherwise.
    //-----------------------------------------------------------------------------
    template <typename ExPolicy, typename RanIter, typename RanIter2,
        typename FwdIter1, typename FwdIter2,
        typename Compare =
            std::equal_to<typename std::iterator_traits<RanIter>::value_type>,
        typename Func =
            std::plus<typename std::iterator_traits<RanIter2>::value_type>,
        PIKA_CONCEPT_REQUIRES_(pika::is_execution_policy<ExPolicy>::value&&
                pika::traits::is_iterator<RanIter>::value&&
                    pika::traits::is_iterator<RanIter2>::value&&
                        pika::traits::is_iterator<FwdIter1>::value&&
                            pika::traits::is_iterator<FwdIter2>::value)>
    typename parallel::detail::algorithm_result<ExPolicy,
        parallel::detail::in_out_result<FwdIter1, FwdIter2>>::type
    reduce_by_key(ExPolicy&& policy, RanIter key_first, RanIter key_last,
        RanIter2 values_first, FwdIter1 keys_output, FwdIter2 values_output,
        Compare&& comp = Compare(), Func&& func = Func())
    {
        using result = parallel::detail::algorithm_result<ExPolicy,
            parallel::detail::in_out_result<FwdIter1, FwdIter2>>;

        static_assert(
            (pika::traits::is_random_access_iterator<RanIter>::value) &&
                (pika::traits::is_random_access_iterator<RanIter2>::value) &&
                (pika::traits::is_forward_iterator<FwdIter1>::value) &&
                (pika::traits::is_forward_iterator<FwdIter2>::value),
            "iterators : Random_access for inputs and forward for outputs.");

        const std::uint64_t number_of_keys = std::distance(key_first, key_last);

        if (number_of_keys <= 1)
        {    // we only have a single key/value so that is our output
            *keys_output = *key_first;
            *values_output = *values_first;
            return result::get(
                parallel::detail::in_out_result<FwdIter1, FwdIter2>{
                    keys_output, values_output});
        }

        return detail::reduce_by_key<FwdIter1, FwdIter2>().call(
            PIKA_FORWARD(ExPolicy, policy), key_first, key_last, values_first,
            keys_output, values_output, PIKA_FORWARD(Compare, comp),
            PIKA_FORWARD(Func, func));
    }
}    // namespace pika
