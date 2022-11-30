//  Copyright (c) 2014 Grant Mercer
//  Copyright (c) 2015 Daniel Bourgeois
//  Copyright (c) 2016-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/copy.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {
    // clang-format off

    /// Copies the elements in the range, defined by [first, last), to another
    /// range beginning at \a dest.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    ///
    /// The assignments in the parallel \a copy algorithm invoked with an
    /// execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a copy algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a copy algorithm returns a
    ///           \a pika::future<FwdIter2> >
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter2> otherwise.
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           \a last and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter2>::type
    copy(ExPolicy&& policy, FwdIter1 first, FwdIter1 last, FwdIter2 dest);

    /// Copies the elements in the range [first, first + count), starting from
    /// first and proceeding to first + count - 1., to another range beginning
    /// at dest.
    ///
    /// \note   Complexity: Performs exactly \a count assignments, if
    ///         count > 0, no assignments otherwise.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Size        The type of the argument specifying the number of
    ///                     elements to apply \a f to.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param count        Refers to the number of elements starting at
    ///                     \a first the algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    ///
    /// The assignments in the parallel \a copy_n algorithm invoked with
    /// an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a copy_n algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a copy_n algorithm returns a
    ///           \a pika::future<FwdIter2>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter2
    ///           otherwise.
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           forwarded to the first element after the last in the input
    ///           sequence and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename Size, typename FwdIter2>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter2>::type
    copy_n(ExPolicy&& policy, FwdIter1 first, Size count, FwdIter2 dest);

    /// Copies the elements in the range, defined by [first, last), to another
    /// range beginning at \a dest. Copies only the elements for which the
    /// predicate \a f returns true. The order of the elements that are not
    /// removed is preserved.
    ///
    /// \note   Complexity: Performs not more than \a last - \a first
    ///         assignments, exactly \a last - \a first applications of the
    ///         predicate \a f.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a copy_if requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate which returns \a true for the
    ///                     required elements. The signature of this predicate
    ///                     should be equivalent to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter1 can be dereferenced and then
    ///                     implicitly converted to Type.
    ///
    /// The assignments in the parallel \a copy_if algorithm invoked with
    /// an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a copy_if algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a copy_if algorithm returns a
    ///           \a pika::future<FwdIter2> >
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter2 otherwise.
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           forwarded to the first element after the last in the input
    ///           sequence and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
        typename F>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter2> >::type
    copy_if(ExPolicy&& policy, FwdIter1 first, FwdIter1 last, FwdIter2 dest,
        Pred&& pred);

    // clang-format on
}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/assert.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/distance.hpp>
#include <pika/parallel/algorithms/detail/is_negative.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>
#include <pika/parallel/algorithms/detail/transfer.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/foreach_partitioner.hpp>
#include <pika/parallel/util/loop.hpp>
#include <pika/parallel/util/projection_identity.hpp>
#include <pika/parallel/util/result_types.hpp>
#include <pika/parallel/util/scan_partitioner.hpp>
#include <pika/parallel/util/transfer.hpp>
#include <pika/parallel/util/zip_iterator.hpp>
#include <pika/type_support/unused.hpp>

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace pika { namespace parallel {

    ///////////////////////////////////////////////////////////////////////////
    // copy
    namespace detail {

        template <typename ExPolicy>
        struct copy_iteration
        {
            using execution_policy_type = std::decay_t<ExPolicy>;

            template <typename Iter>
            PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr void operator()(
                Iter part_begin, std::size_t part_size, std::size_t) const
            {
                using std::get;
                auto iters = part_begin.get_iterator_tuple();
                copy_n<execution_policy_type>(
                    get<0>(iters), part_size, get<1>(iters));
            }
        };

        template <typename IterPair>
        struct copy_algo : public algorithm<copy_algo<IterPair>, IterPair>
        {
            copy_algo()
              : copy_algo::algorithm("copy")
            {
            }

            template <typename ExPolicy, typename InIter, typename Sent,
                typename OutIter>
            static constexpr std::enable_if_t<
                !pika::traits::is_random_access_iterator_v<InIter>,
                in_out_result<InIter, OutIter>>
            sequential(ExPolicy, InIter first, Sent last, OutIter dest)
            {
                in_out_result<InIter, OutIter> result =
                    (copy) (first, last, dest);
                copy_synchronize(first, dest);
                return result;
            }

            template <typename ExPolicy, typename InIter, typename Sent,
                typename OutIter>
            static constexpr std::enable_if_t<
                pika::traits::is_random_access_iterator_v<InIter>,
                in_out_result<InIter, OutIter>>
            sequential(ExPolicy, InIter first, Sent last, OutIter dest)
            {
                in_out_result<InIter, OutIter> result = copy_n<ExPolicy>(
                    first, detail::distance(first, last), dest);
                copy_synchronize(first, dest);
                return result;
            }

            template <typename ExPolicy, typename FwdIter1, typename Sent1,
                typename FwdIter2>
            static typename algorithm_result<ExPolicy,
                in_out_result<FwdIter1, FwdIter2>>::type
            parallel(
                ExPolicy&& policy, FwdIter1 first, Sent1 last, FwdIter2 dest)
            {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
                PIKA_UNUSED(policy);
                PIKA_UNUSED(first);
                PIKA_UNUSED(last);
                PIKA_UNUSED(dest);
                PIKA_ASSERT(false);
                typename algorithm_result<ExPolicy,
                    in_out_result<FwdIter1, FwdIter2>>::type* dummy = nullptr;
                return PIKA_MOVE(*dummy);
#else
                using zip_iterator =
                    pika::util::zip_iterator<FwdIter1, FwdIter2>;

                return get_in_out_result(foreach_partitioner<ExPolicy>::call(
                    PIKA_FORWARD(ExPolicy, policy),
                    pika::util::make_zip_iterator(first, dest),
                    detail::distance(first, last), copy_iteration<ExPolicy>(),
                    [](zip_iterator&& last) -> zip_iterator {
                        using std::get;
                        auto iters = last.get_iterator_tuple();
                        copy_synchronize(get<0>(iters), get<1>(iters));
                        return PIKA_MOVE(last);
                    }));
#endif
            }
        };

#if defined(PIKA_COMPUTE_DEVICE_CODE)
        template <typename FwdIter1, typename FwdIter2, typename Enable = void>
        struct copy_iter : public copy_algo<in_out_result<FwdIter1, FwdIter2>>
        {
        };
#else
        ///////////////////////////////////////////////////////////////////////
        template <typename FwdIter1, typename FwdIter2>
        struct copy_iter : public copy_algo<in_out_result<FwdIter1, FwdIter2>>
        {
        };
#endif
    }    // namespace detail

    /////////////////////////////////////////////////////////////////////////////
    // copy_n
    namespace detail {

        // sequential copy_n
        template <typename IterPair>
        struct copy_n_algo : public algorithm<copy_n_algo<IterPair>, IterPair>
        {
            copy_n_algo()
              : copy_n_algo::algorithm("copy_n")
            {
            }

            template <typename ExPolicy, typename InIter, typename OutIter>
            static constexpr in_out_result<InIter, OutIter>
            sequential(ExPolicy, InIter first, std::size_t count, OutIter dest)
            {
                in_out_result<InIter, OutIter> result =
                    copy_n<ExPolicy>(first, count, dest);
                copy_synchronize(first, dest);
                return result;
            }

            template <typename ExPolicy, typename FwdIter1, typename FwdIter2>
            static typename algorithm_result<ExPolicy,
                in_out_result<FwdIter1, FwdIter2>>::type
            parallel(ExPolicy&& policy, FwdIter1 first, std::size_t count,
                FwdIter2 dest)
            {
                using zip_iterator =
                    pika::util::zip_iterator<FwdIter1, FwdIter2>;

                return get_in_out_result(foreach_partitioner<ExPolicy>::call(
                    PIKA_FORWARD(ExPolicy, policy),
                    pika::util::make_zip_iterator(first, dest), count,
                    [](zip_iterator part_begin, std::size_t part_size,
                        std::size_t) {
                        using std::get;

                        auto iters = part_begin.get_iterator_tuple();
                        copy_n<ExPolicy>(
                            get<0>(iters), part_size, get<1>(iters));
                    },
                    [](zip_iterator&& last) -> zip_iterator {
                        using std::get;
                        auto iters = last.get_iterator_tuple();
                        copy_synchronize(get<0>(iters), get<1>(iters));
                        return PIKA_MOVE(last);
                    }));
            }
        };
    }    // namespace detail

    /////////////////////////////////////////////////////////////////////////////
    // copy_if
    namespace detail {

        // sequential copy_if with projection function
        template <typename InIter1, typename InIter2, typename OutIter,
            typename Pred, typename Proj>
        inline in_out_result<InIter1, OutIter> sequential_copy_if(
            InIter1 first, InIter2 last, OutIter dest, Pred&& pred, Proj&& proj)
        {
            while (first != last)
            {
                if (PIKA_INVOKE(pred, PIKA_INVOKE(proj, *first)))
                    *dest++ = *first;
                first++;
            }
            return in_out_result<InIter1, OutIter>{
                PIKA_MOVE(first), PIKA_MOVE(dest)};
        }

        template <typename IterPair>
        struct copy_if_algo : public algorithm<copy_if_algo<IterPair>, IterPair>
        {
            copy_if_algo()
              : copy_if_algo::algorithm("copy_if")
            {
            }

            template <typename ExPolicy, typename InIter1, typename InIter2,
                typename OutIter, typename Pred,
                typename Proj = projection_identity>
            static in_out_result<InIter1, OutIter>
            sequential(ExPolicy, InIter1 first, InIter2 last, OutIter dest,
                Pred&& pred, Proj&& proj /* = Proj()*/)
            {
                return sequential_copy_if(first, last, dest,
                    PIKA_FORWARD(Pred, pred), PIKA_FORWARD(Proj, proj));
            }

            template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
                typename FwdIter3, typename Pred,
                typename Proj = projection_identity>
            static typename algorithm_result<ExPolicy,
                in_out_result<FwdIter1, FwdIter3>>::type
            parallel(ExPolicy&& policy, FwdIter1 first, FwdIter2 last,
                FwdIter3 dest, Pred&& pred, Proj&& proj /* = Proj()*/)
            {
                using zip_iterator = pika::util::zip_iterator<FwdIter1, bool*>;
                using result = algorithm_result<ExPolicy,
                    in_out_result<FwdIter1, FwdIter3>>;
                using difference_type =
                    typename std::iterator_traits<FwdIter1>::difference_type;

                if (first == last)
                {
                    return result::get(in_out_result<FwdIter1, FwdIter3>{
                        PIKA_MOVE(first), PIKA_MOVE(dest)});
                }

                difference_type count = detail::distance(first, last);

                std::shared_ptr<bool[]> flags(new bool[count]);
                std::size_t init = 0;

                using pika::util::make_zip_iterator;
                using std::get;
                using scan_partitioner_type = scan_partitioner<ExPolicy,
                    in_out_result<FwdIter1, FwdIter3>, std::size_t>;

                auto f1 = [pred = PIKA_FORWARD(Pred, pred),
                              proj = PIKA_FORWARD(decltype(proj), proj)](
                              zip_iterator part_begin,
                              std::size_t part_size) -> std::size_t {
                    std::size_t curr = 0;

                    // Note: replacing the invoke() with PIKA_INVOKE()
                    // below makes gcc generate errors

                    // MSVC complains if proj is captured by ref below
                    loop_n<std::decay_t<ExPolicy>>(part_begin, part_size,
                        [&pred, proj, &curr](zip_iterator it) mutable -> void {
                            bool f = pika::util::detail::invoke(pred,
                                pika::util::detail::invoke(proj, get<0>(*it)));

                            if ((get<1>(*it) = f))
                                ++curr;
                        });

                    return curr;
                };
                auto f3 = [dest, flags](zip_iterator part_begin,
                              std::size_t part_size, std::size_t val) mutable {
                    PIKA_UNUSED(flags);
                    std::advance(dest, val);
                    loop_n<std::decay_t<ExPolicy>>(part_begin, part_size,
                        [&dest](zip_iterator it) mutable {
                            if (get<1>(*it))
                                *dest++ = get<0>(*it);
                        });
                };

                auto f4 = [first, dest, flags](std::vector<std::size_t>&& items,
                              std::vector<pika::future<void>>&& data) mutable
                    -> in_out_result<FwdIter1, FwdIter3> {
                    PIKA_UNUSED(flags);

                    auto dist = items.back();
                    std::advance(first, dist);
                    std::advance(dest, dist);

                    // make sure iterators embedded in function object that is
                    // attached to futures are invalidated
                    data.clear();

                    return in_out_result<FwdIter1, FwdIter3>{
                        PIKA_MOVE(first), PIKA_MOVE(dest)};
                };

                return scan_partitioner_type::call(
                    PIKA_FORWARD(ExPolicy, policy),
                    make_zip_iterator(first, flags.get()), count, init,
                    // step 1 performs first part of scan algorithm
                    PIKA_MOVE(f1),
                    // step 2 propagates the partition results from left
                    // to right
                    std::plus<std::size_t>(),
                    // step 3 runs final accumulation on each partition
                    PIKA_MOVE(f3),
                    // step 4 use this return value
                    PIKA_MOVE(f4));
            }
        };
    }    // namespace detail
}}       // namespace pika::parallel

namespace pika {

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::copy
    inline constexpr struct copy_t final
      : pika::detail::tag_parallel_algorithm<copy_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy_v<ExPolicy> &&
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2>
            )>
        // clang-format on
        friend typename parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::copy_t, ExPolicy&& policy, FwdIter1 first,
            FwdIter1 last, FwdIter2 dest)
        {
            return parallel::detail::get_second_element(
                parallel::detail::transfer<
                    parallel::detail::copy_iter<FwdIter1, FwdIter2>>(
                    PIKA_FORWARD(ExPolicy, policy), first, last, dest));
        }

        // clang-format off
        template <typename FwdIter1, typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2>
            )>
        // clang-format on
        friend FwdIter2 tag_fallback_invoke(
            pika::copy_t, FwdIter1 first, FwdIter1 last, FwdIter2 dest)
        {
            return parallel::detail::get_second_element(
                parallel::detail::transfer<
                    parallel::detail::copy_iter<FwdIter1, FwdIter2>>(
                    pika::execution::seq, first, last, dest));
        }
    } copy{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::copy_n
    inline constexpr struct copy_n_t final
      : pika::detail::tag_parallel_algorithm<copy_n_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename Size,
            typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy_v<ExPolicy> &&
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2>
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::copy_n_t, ExPolicy&& policy, FwdIter1 first,
            Size count, FwdIter2 dest)
        {
            static_assert((pika::traits::is_forward_iterator_v<FwdIter1>),
                "Required at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator_v<FwdIter2> ||
                    (pika::is_sequenced_execution_policy_v<ExPolicy> &&
                        pika::traits::is_output_iterator_v<FwdIter2>),
                "Requires at least forward iterator or sequential execution.");

            // if count is representing a negative value, we do nothing
            if (pika::parallel::detail::is_negative(count))
            {
                return pika::parallel::detail::algorithm_result<ExPolicy,
                    FwdIter2>::get(PIKA_MOVE(dest));
            }

            return pika::parallel::detail::get_second_element(
                pika::parallel::detail::copy_n_algo<
                    pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(PIKA_FORWARD(ExPolicy, policy), first,
                        std::size_t(count), dest));
        }

        // clang-format off
        template <typename FwdIter1, typename Size, typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2>
            )>
        // clang-format on
        friend FwdIter2 tag_fallback_invoke(
            pika::copy_n_t, FwdIter1 first, Size count, FwdIter2 dest)
        {
            static_assert((pika::traits::is_forward_iterator_v<FwdIter1>),
                "Required at least forward iterator.");
            static_assert((pika::traits::is_output_iterator_v<FwdIter2>),
                "Requires at least output iterator.");

            // if count is representing a negative value, we do nothing
            if (pika::parallel::detail::is_negative(count))
            {
                return pika::parallel::detail::algorithm_result<
                    pika::execution::sequenced_policy,
                    FwdIter2>::get(PIKA_MOVE(dest));
            }

            return pika::parallel::detail::get_second_element(
                pika::parallel::detail::copy_n_algo<
                    pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(
                        pika::execution::seq, first, std::size_t(count), dest));
        }
    } copy_n{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::copy_if
    inline constexpr struct copy_if_t final
      : pika::detail::tag_parallel_algorithm<copy_if_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename Pred,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy_v<ExPolicy> &&
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2> &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter1>::value_type
                >
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::copy_if_t, ExPolicy&& policy, FwdIter1 first,
            FwdIter1 last, FwdIter2 dest, Pred&& pred)
        {
            static_assert((pika::traits::is_forward_iterator_v<FwdIter1>),
                "Required at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator_v<FwdIter2> ||
                    (pika::is_sequenced_execution_policy_v<ExPolicy> &&
                        pika::traits::is_output_iterator_v<FwdIter2>),
                "Requires at least forward iterator or sequential execution.");

            return pika::parallel::detail::get_second_element(
                pika::parallel::detail::copy_if_algo<
                    pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(PIKA_FORWARD(ExPolicy, policy), first, last, dest,
                        PIKA_FORWARD(Pred, pred),
                        pika::parallel::detail::projection_identity{}));
        }

        // clang-format off
        template <typename FwdIter1, typename FwdIter2, typename Pred,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator_v<FwdIter1> &&
                pika::traits::is_iterator_v<FwdIter2> &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter1>::value_type
                >
            )>
        // clang-format on
        friend FwdIter2 tag_fallback_invoke(pika::copy_if_t, FwdIter1 first,
            FwdIter1 last, FwdIter2 dest, Pred&& pred)
        {
            static_assert((pika::traits::is_forward_iterator_v<FwdIter1>),
                "Required at least forward iterator.");
            static_assert((pika::traits::is_output_iterator_v<FwdIter2>),
                "Requires at least output iterator.");

            return pika::parallel::detail::get_second_element(
                pika::parallel::detail::copy_if_algo<
                    pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(pika::execution::seq, first, last, dest,
                        PIKA_FORWARD(Pred, pred),
                        pika::parallel::detail::projection_identity{}));
        }
    } copy_if{};
}    // namespace pika

#endif    // DOXYGEN
