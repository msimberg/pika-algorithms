//  Copyright (c) 2015 Daniel Bourgeois
//  Copyright (c) 2017-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/transform_reduce.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {
    // clang-format off

    /// Returns GENERALIZED_SUM(red_op, init, conv_op(*first), ...,
    /// conv_op(*(first + (last - first) - 1))).
    ///
    /// \note   Complexity: O(\a last - \a first) applications of the
    ///         predicates \a red_op and \a conv_op.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter      The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam F           The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a copy_if requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    /// \tparam T           The type of the value to be used as initial (and
    ///                     intermediate) values (deduced).
    /// \tparam Reduce      The type of the binary function object used for
    ///                     the reduction operation.
    /// \tparam Convert     The type of the unary function object used to
    ///                     transform the elements of the input sequence before
    ///                     invoking the reduce function.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param conv_op      Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last). This is a
    ///                     unary predicate. The signature of this predicate
    ///                     should be equivalent to:
    ///                     \code
    ///                     R fun(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter can be dereferenced and then
    ///                     implicitly converted to Type.
    ///                     The type \a R must be such that an object of this
    ///                     type can be implicitly converted to \a T.
    /// \param init         The initial value for the generalized sum.
    /// \param red_op       Specifies the function (or function object) which
    ///                     will be invoked for each of the values returned
    ///                     from the invocation of \a conv_op. This is a
    ///                     binary predicate. The signature of this predicate
    ///                     should be equivalent to:
    ///                     \code
    ///                     Ret fun(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it.
    ///                     The types \a Type1, \a Type2, and \a Ret must be
    ///                     such that an object of a type as returned from
    ///                     \a conv_op can be implicitly converted to any
    ///                     of those types.
    ///
    /// The reduce operations in the parallel \a transform_reduce algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The reduce operations in the parallel \a transform_reduce algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a transform_reduce algorithm returns a \a pika::future<T> if the
    ///           execution policy is of type \a parallel_task_policy and
    ///           returns \a T otherwise.
    ///           The \a transform_reduce algorithm returns the result of the
    ///           generalized sum over the values returned from \a conv_op when
    ///           applied to the elements given by the input range
    ///           [first, last).
    ///
    /// \note   GENERALIZED_SUM(op, a1, ..., aN) is defined as follows:
    ///         * a1 when N is 1
    ///         * op(GENERALIZED_SUM(op, b1, ..., bK), GENERALIZED_SUM(op, bM, ..., bN)),
    ///           where:
    ///           * b1, ..., bN may be any permutation of a1, ..., aN and
    ///           * 1 < K+1 = M <= N.
    ///
    /// The difference between \a transform_reduce and \a accumulate is
    /// that the behavior of transform_reduce may be non-deterministic for
    /// non-associative or non-commutative binary predicate.
    ///
    template <typename ExPolicy, typename FwdIter, typename T, typename Reduce,
        typename Convert>
    typename pika::parallel::detail::algorithm_result<ExPolicy, T>::type
    transform_reduce(ExPolicy&& policy, FwdIter first, FwdIter last, T init,
        Reduce&& red_op, Convert&& conv_op);

    ///////////////////////////////////////////////////////////////////////////
    /// Returns the result of accumulating init with the inner products of the
    /// pairs formed by the elements of two ranges starting at first1 and
    /// first2.
    ///
    /// \note   Complexity: O(\a last - \a first) applications of the
    ///         predicate \a op2.
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the first source iterators used
    ///                     (deduced). This iterator type must meet the
    ///                     requirements of an forward iterator.
    /// \tparam FwdIter2    The type of the second source iterators used
    ///                     (deduced). This iterator type must meet the
    ///                     requirements of an forward iterator.
    /// \tparam T           The type of the value to be used as return)
    ///                     values (deduced).
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first1       Refers to the beginning of the first sequence of
    ///                     elements the result will be calculated with.
    /// \param last1        Refers to the end of the first sequence of elements
    ///                     the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the second sequence of
    ///                     elements the result will be calculated with.
    /// \param init         The initial value for the sum.
    ///
    /// The operations in the parallel \a transform_reduce algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The operations in the parallel \a transform_reduce algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a transform_reduce algorithm returns a \a pika::future<T> if
    ///           the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a T otherwise.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2, typename T>
    typename pika::parallel::detail::algorithm_result<ExPolicy, T>::type
    transform_reduce(ExPolicy&& policy, FwdIter1 first1, FwdIter1 last1,
        FwdIter2 first2, T init);

    ///////////////////////////////////////////////////////////////////////////
    /// Returns the result of accumulating init with the inner products of the
    /// pairs formed by the elements of two ranges starting at first1 and
    /// first2.
    ///
    /// \note   Complexity: O(\a last - \a first) applications of the
    ///         predicate \a op2.
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the first source iterators used
    ///                     (deduced). This iterator type must meet the
    ///                     requirements of an forward iterator.
    /// \tparam FwdIter2    The type of the second source iterators used
    ///                     (deduced). This iterator type must meet the
    ///                     requirements of an forward iterator.
    /// \tparam T           The type of the value to be used as return)
    ///                     values (deduced).
    /// \tparam Reduce      The type of the binary function object used for
    ///                     the multiplication operation.
    /// \tparam Convert     The type of the unary function object used to
    ///                     transform the elements of the input sequence before
    ///                     invoking the reduce function.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first1       Refers to the beginning of the first sequence of
    ///                     elements the result will be calculated with.
    /// \param last1        Refers to the end of the first sequence of elements
    ///                     the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the second sequence of
    ///                     elements the result will be calculated with.
    /// \param init         The initial value for the sum.
    /// \param red_op       Specifies the function (or function object) which
    ///                     will be invoked for the initial value and each
    ///                     of the return values of \a op2.
    ///                     This is a binary predicate. The
    ///                     signature of this predicate should be equivalent to
    ///                     should be equivalent to:
    ///                     \code
    ///                     Ret fun(const Type1 &a, const Type1 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it.
    ///                     The type \a Ret must be
    ///                     such that it can be implicitly converted to a type
    ///                     of \a T.
    /// \param conv_op      Specifies the function (or function object) which
    ///                     will be invoked for each of the input values
    ///                     of the sequence. This is a binary predicate. The
    ///                     signature of this predicate should be equivalent to
    ///                     \code
    ///                     Ret fun(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it.
    ///                     The type \a Ret must be such that it can be
    ///                     implicitly converted to an object for the second
    ///                     argument type of \a op1.
    ///
    /// The operations in the parallel \a transform_reduce algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The operations in the parallel \a transform_reduce algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a transform_reduce algorithm returns a \a pika::future<T>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a T otherwise.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
        typename T, typename Reduce, typename Convert>
    typename pika::parallel::detail::algorithm_result<ExPolicy, T>::type
    transform_reduce(ExPolicy&& policy, FwdIter1 first1, FwdIter1 last1,
        FwdIter2 first2, T init, Reduce&& red_op, Convert&& conv_op);

    // clang-format on
}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/functional/invoke_result.hpp>
#include <pika/functional/traits/is_invocable.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/pack_traversal/unwrap.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>

#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/detail/accumulate.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/distance.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/loop.hpp>
#include <pika/parallel/util/partitioner.hpp>
#include <pika/parallel/util/zip_iterator.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // transform_reduce
    ///////////////////////////////////////////////////////////////////////
    template <typename T, typename ExPolicy, typename Reduce, typename Convert>
    struct transform_reduce_iteration
    {
        using execution_policy_type = std::decay_t<ExPolicy>;
        using reduce_type = std::decay_t<Reduce>;
        using convert_type = std::decay_t<Convert>;

        reduce_type reduce_;
        convert_type convert_;

        template <typename Reduce_, typename Convert_>
        PIKA_HOST_DEVICE
        transform_reduce_iteration(Reduce_&& reduce, Convert_&& convert)
          : reduce_(PIKA_FORWARD(Reduce_, reduce))
          , convert_(PIKA_FORWARD(Convert_, convert))
        {
        }

#if !defined(__NVCC__) && !defined(__CUDACC__)
        transform_reduce_iteration(transform_reduce_iteration const&) = default;
        transform_reduce_iteration(transform_reduce_iteration&&) = default;
#else
        PIKA_HOST_DEVICE
        transform_reduce_iteration(transform_reduce_iteration const& rhs)
          : reduce_(rhs.reduce_)
          , convert_(rhs.convert_)
        {
        }

        PIKA_HOST_DEVICE
        transform_reduce_iteration(transform_reduce_iteration&& rhs)
          : reduce_(PIKA_MOVE(rhs.reduce_))
          , convert_(PIKA_MOVE(rhs.convert_))
        {
        }
#endif

        transform_reduce_iteration& operator=(
            transform_reduce_iteration const&) = default;
        transform_reduce_iteration& operator=(
            transform_reduce_iteration&&) = default;

        template <typename Iter>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE T operator()(
            Iter part_begin, std::size_t part_size)
        {
            using reference = typename std::iterator_traits<Iter>::reference;

            T val = PIKA_INVOKE(convert_, *part_begin);
            return accumulate_n(++part_begin, --part_size, PIKA_MOVE(val),
                [PIKA_CXX20_CAPTURE_THIS(=)](
                    T const& res, reference next) mutable -> T {
                    return PIKA_INVOKE(
                        reduce_, res, PIKA_INVOKE(convert_, next));
                });
        }
    };

    template <typename T>
    struct transform_reduce : public algorithm<transform_reduce<T>, T>
    {
        transform_reduce()
          : transform_reduce::algorithm("transform_reduce")
        {
        }

        template <typename ExPolicy, typename Iter, typename Sent, typename T_,
            typename Reduce, typename Convert>
        static T sequential(ExPolicy, Iter first, Sent last, T_&& init,
            Reduce&& r, Convert&& conv)
        {
            using value_type = typename std::iterator_traits<Iter>::value_type;

            return detail::accumulate(first, last, PIKA_FORWARD(T_, init),
                [&r, &conv](T const& res, value_type const& next) -> T {
                    return PIKA_INVOKE(r, res, PIKA_INVOKE(conv, next));
                });
        }

        template <typename ExPolicy, typename Iter, typename Sent, typename T_,
            typename Reduce, typename Convert>
        static typename algorithm_result<ExPolicy, T>::type
        parallel(ExPolicy&& policy, Iter first, Sent last, T_&& init,
            Reduce&& r, Convert&& conv)
        {
            if (first == last)
            {
                T init_ = init;
                return algorithm_result<ExPolicy, T>::get(PIKA_MOVE(init_));
            }

            auto f1 = transform_reduce_iteration<T, ExPolicy, Reduce, Convert>(
                PIKA_FORWARD(Reduce, r), PIKA_FORWARD(Convert, conv));

            return detail::partitioner<ExPolicy, T>::call(
                PIKA_FORWARD(ExPolicy, policy), first,
                detail::distance(first, last), PIKA_MOVE(f1),
                pika::unwrapping([init = PIKA_FORWARD(T_, init),
                                     r = PIKA_FORWARD(Reduce, r)](
                                     std::vector<T>&& results) mutable -> T {
                    return accumulate_n(pika::util::begin(results),
                        pika::util::size(results), init, r);
                }));
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // transform_reduce_binary
    template <typename F>
    struct transform_reduce_binary_indirect
    {
        F f_;

        template <typename Iter1, typename Iter2>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr auto operator()(
            Iter1 it1, Iter2 it2) -> decltype(PIKA_INVOKE(f_, *it1, *it2))
        {
            return PIKA_INVOKE(f_, *it1, *it2);
        }
    };

    template <typename Op1, typename Op2, typename T>
    struct transform_reduce_binary_partition
    {
        using value_type = std::decay_t<T>;

        Op1 op1_;
        Op2 op2_;
        value_type& part_sum_;

        template <typename Iter1, typename Iter2>
        PIKA_HOST_DEVICE PIKA_FORCEINLINE constexpr void
        operator()(Iter1 it1, Iter2 it2)
        {
            part_sum_ =
                PIKA_INVOKE(op1_, part_sum_, PIKA_INVOKE(op2_, *it1, *it2));
        }
    };

    template <typename T>
    struct transform_reduce_binary
      : public algorithm<transform_reduce_binary<T>, T>
    {
        transform_reduce_binary()
          : transform_reduce_binary::algorithm("transform_reduce_binary")
        {
        }

        template <typename ExPolicy, typename Iter, typename Sent,
            typename Iter2, typename T_, typename Op1, typename Op2>
        static T sequential(ExPolicy&& /* policy */, Iter first1, Sent last1,
            Iter2 first2, T_ init, Op1&& op1, Op2&& op2)
        {
            if (first1 == last1)
            {
                return init;
            }

            // check whether we should apply vectorization
            if (!loop_optimization<ExPolicy>(first1, last1))
            {
                loop2<ExPolicy>(std::false_type(), first1, last1, first2,
                    transform_reduce_binary_partition<Op1, Op2, T>{
                        PIKA_FORWARD(Op1, op1), PIKA_FORWARD(Op2, op2), init});
                return init;
            }

            // loop_step properly advances the iterators
            auto part_sum = loop_step<ExPolicy>(std::true_type(),
                transform_reduce_binary_indirect<Op2>{op2}, first1, first2);

            std::pair<Iter, Iter2> p = loop2<ExPolicy>(std::true_type(), first1,
                last1, first2,
                transform_reduce_binary_partition<Op1, Op2, decltype(part_sum)>{
                    op1, op2, part_sum});

            // this is to support vectorization, it will call op1 for each
            // of the elements of a value-pack
            auto result = accumulate_values<ExPolicy>(
                [&op1](T const& sum, T&& val) -> T {
                    return PIKA_INVOKE(op1, sum, val);
                },
                PIKA_MOVE(part_sum), PIKA_MOVE(init));

            // the vectorization might not cover all of the sequences,
            // handle the remainder directly
            if (p.first != last1)
            {
                loop2<ExPolicy>(std::false_type(), p.first, last1, p.second,
                    transform_reduce_binary_partition<Op1, Op2,
                        decltype(result)>{PIKA_FORWARD(Op1, op1),
                        PIKA_FORWARD(Op2, op2), result});
            }

            return extract_value<ExPolicy>(result);
        }

        template <typename ExPolicy, typename Iter, typename Sent,
            typename Iter2, typename T_, typename Op1, typename Op2>
        static typename algorithm_result<ExPolicy, T>::type
        parallel(ExPolicy&& policy, Iter first1, Sent last1, Iter2 first2,
            T_&& init, Op1&& op1, Op2&& op2)
        {
            using result = algorithm_result<ExPolicy, T>;
            using zip_iterator = pika::util::zip_iterator<Iter, Iter2>;
            using difference_type =
                typename std::iterator_traits<Iter>::difference_type;

            if (first1 == last1)
            {
                return result::get(PIKA_FORWARD(T_, init));
            }

            difference_type count = detail::distance(first1, last1);

            auto f1 = [op1, op2 = PIKA_FORWARD(Op2, op2)](
                          zip_iterator part_begin,
                          std::size_t part_size) mutable -> T {
                auto iters = part_begin.get_iterator_tuple();
                Iter it1 = std::get<0>(iters);
                Iter2 it2 = std::get<1>(iters);

                Iter last1 = it1;
                std::advance(last1, part_size);

                if (!loop_optimization<ExPolicy>(it1, last1))
                {
                    // loop_step properly advances the iterators
                    auto result = loop_step<ExPolicy>(std::false_type(),
                        transform_reduce_binary_indirect<Op2>{op2}, it1, it2);

                    loop2<ExPolicy>(std::false_type(), it1, last1, it2,
                        transform_reduce_binary_partition<Op1, Op2,
                            decltype(result)>{PIKA_FORWARD(Op1, op1),
                            PIKA_FORWARD(Op2, op2), result});

                    return extract_value<ExPolicy>(result);
                }

                // loop_step properly advances the iterators
                auto part_sum = loop_step<ExPolicy>(std::true_type(),
                    transform_reduce_binary_indirect<Op2>{op2}, it1, it2);

                std::pair<Iter, Iter2> p =
                    loop2<ExPolicy>(std::true_type(), it1, last1, it2,
                        transform_reduce_binary_partition<Op1, Op2,
                            decltype(part_sum)>{op1, op2, part_sum});

                // this is to support vectorization, it will call op1
                // for each of the elements of a value-pack
                auto result = accumulate_values<ExPolicy>(
                    [&op1](T const& sum, T&& val) -> T {
                        return PIKA_INVOKE(op1, sum, val);
                    },
                    part_sum);

                // the vectorization might not cover all of the sequences,
                // handle the remainder directly
                if (p.first != last1)
                {
                    loop2<ExPolicy>(std::false_type(), p.first, last1, p.second,
                        transform_reduce_binary_partition<Op1, Op2,
                            decltype(result)>{PIKA_FORWARD(Op1, op1),
                            PIKA_FORWARD(Op2, op2), result});
                }

                return extract_value<ExPolicy>(result);
            };

            using pika::util::make_zip_iterator;

            return detail::partitioner<ExPolicy, T>::call(
                PIKA_FORWARD(ExPolicy, policy),
                make_zip_iterator(first1, first2), count, PIKA_MOVE(f1),
                [init = PIKA_FORWARD(T_, init), op1 = PIKA_FORWARD(Op1, op1)](
                    std::vector<pika::future<T>>&& results) mutable -> T {
                    T ret = PIKA_MOVE(init);
                    for (auto&& fut : results)
                    {
                        ret = PIKA_INVOKE(op1, PIKA_MOVE(ret), fut.get());
                    }
                    return ret;
                });
        }
    };
}    // namespace pika::parallel::detail

#if defined(PIKA_HAVE_THREAD_DESCRIPTION)
#include <pika/functional/traits/get_function_address.hpp>
#include <pika/functional/traits/get_function_annotation.hpp>

namespace pika::detail {
    template <typename T, typename ExPolicy, typename Reduce, typename Convert>
    struct get_function_address<parallel::detail::transform_reduce_iteration<T,
        ExPolicy, Reduce, Convert>>
    {
        static constexpr std::size_t call(
            parallel::detail::transform_reduce_iteration<T, ExPolicy, Reduce,
                Convert> const& f) noexcept
        {
            return get_function_address<std::decay_t<Reduce>>::call(f.reduce_);
        }
    };

    template <typename T, typename ExPolicy, typename Reduce, typename Convert>
    struct get_function_annotation<parallel::detail::transform_reduce_iteration<
        T, ExPolicy, Reduce, Convert>>
    {
        static constexpr char const* call(
            parallel::detail::transform_reduce_iteration<T, ExPolicy, Reduce,
                Convert> const& f) noexcept
        {
            char const* reduce_name =
                get_function_annotation<std::decay_t<Reduce>>::call(f.reduce_);

            return reduce_name != nullptr ?
                reduce_name :
                get_function_annotation<std::decay_t<Convert>>::call(
                    f.convert_);
        }
    };

#if PIKA_HAVE_ITTNOTIFY != 0 && !defined(PIKA_HAVE_APEX)
    template <typename T, typename ExPolicy, typename Reduce, typename Convert>
    struct get_function_annotation_itt<parallel::detail::
            transform_reduce_iteration<T, ExPolicy, Reduce, Convert>>
    {
        static util::itt::string_handle call(
            parallel::detail::transform_reduce_iteration<T, ExPolicy, Reduce,
                Convert> const& f) noexcept
        {
            static util::itt::string_handle sh(get_function_annotation<
                parallel::detail::transform_reduce_iteration<T, ExPolicy,
                    Reduce, Convert>>::call(f));

            return sh;
        }
    };
#endif
}    // namespace pika::detail
#endif

namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::transform_reduce
    inline constexpr struct transform_reduce_t final
      : pika::detail::tag_parallel_algorithm<transform_reduce_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter, typename T,
            typename Reduce, typename Convert,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter>::value &&
                pika::detail::is_invocable_v<Convert,
                   typename std::iterator_traits<FwdIter>::value_type> &&
                pika::detail::is_invocable_v<Reduce,
                   typename pika::util::detail::invoke_result<Convert,
                       typename std::iterator_traits<FwdIter>::value_type
                   >::type,
                   typename pika::util::detail::invoke_result<Convert,
                       typename std::iterator_traits<FwdIter>::value_type
                   >::type
                >
            )>
        // clang-format on
        friend
            typename pika::parallel::detail::algorithm_result<ExPolicy, T>::type
            tag_fallback_invoke(transform_reduce_t, ExPolicy&& policy,
                FwdIter first, FwdIter last, T init, Reduce&& red_op,
                Convert&& conv_op)
        {
            static_assert(pika::traits::is_forward_iterator<FwdIter>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::transform_reduce<T>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last,
                PIKA_FORWARD(T, init), PIKA_FORWARD(Reduce, red_op),
                PIKA_FORWARD(Convert, conv_op));
        }

        // clang-format off
        template <typename InIter, typename T, typename Reduce,
            typename Convert,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value &&
                pika::detail::is_invocable_v<Convert,
                   typename std::iterator_traits<InIter>::value_type> &&
                pika::detail::is_invocable_v<Reduce,
                   typename pika::util::detail::invoke_result<Convert,
                       typename std::iterator_traits<InIter>::value_type
                   >::type,
                   typename pika::util::detail::invoke_result<Convert,
                       typename std::iterator_traits<InIter>::value_type
                   >::type
                >
            )>
        // clang-format on
        friend T tag_fallback_invoke(transform_reduce_t, InIter first,
            InIter last, T init, Reduce&& red_op, Convert&& conv_op)
        {
            static_assert(pika::traits::is_input_iterator<InIter>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::transform_reduce<T>().call(
                pika::execution::seq, first, last, PIKA_FORWARD(T, init),
                PIKA_FORWARD(Reduce, red_op), PIKA_FORWARD(Convert, conv_op));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename T,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value
            )>
        // clang-format on
        friend
            typename pika::parallel::detail::algorithm_result<ExPolicy, T>::type
            tag_fallback_invoke(transform_reduce_t, ExPolicy&& policy,
                FwdIter1 first1, FwdIter1 last1, FwdIter2 first2, T init)
        {
            static_assert(pika::traits::is_forward_iterator<FwdIter1>::value,
                "Requires at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter2>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::transform_reduce_binary<T>().call(
                PIKA_FORWARD(ExPolicy, policy), first1, last1, first2,
                PIKA_MOVE(init), pika::parallel::detail::plus(),
                pika::parallel::detail::multiplies());
        }

        // clang-format off
        template <typename InIter1, typename InIter2, typename T,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter1>::value &&
                pika::traits::is_iterator<InIter2>::value
            )>
        // clang-format on
        friend T tag_fallback_invoke(transform_reduce_t, InIter1 first1,
            InIter1 last1, InIter2 first2, T init)
        {
            static_assert(pika::traits::is_input_iterator<InIter1>::value,
                "Requires at least input iterator.");
            static_assert(pika::traits::is_input_iterator<InIter2>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::transform_reduce_binary<T>().call(
                pika::execution::seq, first1, last1, first2, PIKA_MOVE(init),
                pika::parallel::detail::plus(),
                pika::parallel::detail::multiplies());
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename T, typename Reduce, typename Convert,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value &&
                pika::detail::is_invocable_v<Convert,
                    typename std::iterator_traits<FwdIter1>::value_type,
                    typename std::iterator_traits<FwdIter2>::value_type
                > &&
                pika::detail::is_invocable_v<Reduce,
                    typename pika::util::detail::invoke_result<Convert,
                        typename std::iterator_traits<FwdIter1>::value_type,
                        typename std::iterator_traits<FwdIter2>::value_type
                    >::type,
                    typename pika::util::detail::invoke_result<Convert,
                        typename std::iterator_traits<FwdIter1>::value_type,
                        typename std::iterator_traits<FwdIter2>::value_type
                    >::type
                >
            )>
        // clang-format on
        friend
            typename pika::parallel::detail::algorithm_result<ExPolicy, T>::type
            tag_fallback_invoke(transform_reduce_t, ExPolicy&& policy,
                FwdIter1 first1, FwdIter1 last1, FwdIter2 first2, T init,
                Reduce&& red_op, Convert&& conv_op)
        {
            static_assert(pika::traits::is_forward_iterator<FwdIter1>::value,
                "Requires at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter2>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::transform_reduce_binary<T>().call(
                PIKA_FORWARD(ExPolicy, policy), first1, last1, first2,
                PIKA_MOVE(init), PIKA_FORWARD(Reduce, red_op),
                PIKA_FORWARD(Convert, conv_op));
        }

        // clang-format off
        template <typename InIter1, typename InIter2, typename T,
            typename Reduce, typename Convert,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter1>::value &&
                pika::traits::is_iterator<InIter2>::value &&
                pika::detail::is_invocable_v<Convert,
                    typename std::iterator_traits<InIter1>::value_type,
                    typename std::iterator_traits<InIter2>::value_type
                > &&
                pika::detail::is_invocable_v<Reduce,
                    typename pika::util::detail::invoke_result<Convert,
                        typename std::iterator_traits<InIter1>::value_type,
                        typename std::iterator_traits<InIter2>::value_type
                    >::type,
                    typename pika::util::detail::invoke_result<Convert,
                        typename std::iterator_traits<InIter1>::value_type,
                        typename std::iterator_traits<InIter2>::value_type
                    >::type
                >
            )>
        // clang-format on
        friend T
        tag_fallback_invoke(transform_reduce_t, InIter1 first1, InIter1 last1,
            InIter2 first2, T init, Reduce&& red_op, Convert&& conv_op)
        {
            static_assert(pika::traits::is_input_iterator<InIter1>::value,
                "Requires at least input iterator.");
            static_assert(pika::traits::is_input_iterator<InIter2>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::transform_reduce_binary<T>().call(
                pika::execution::seq, first1, last1, first2, PIKA_MOVE(init),
                PIKA_FORWARD(Reduce, red_op), PIKA_FORWARD(Convert, conv_op));
        }
    } transform_reduce{};
}    // namespace pika

#endif    // DOXYGEN
