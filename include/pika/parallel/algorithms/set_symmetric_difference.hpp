//  Copyright (c) 2007-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/set_symmetric_difference.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {
    // clang-format off

    /// Constructs a sorted range beginning at dest consisting of all elements
    /// present in either of the sorted ranges [first1, last1) and
    /// [first2, last2), but not in both of them are copied to the range
    /// beginning at \a dest. The resulting range is also sorted. This
    /// algorithm expects both input ranges to be sorted with the given binary
    /// predicate \a f.
    ///
    /// \note   Complexity: At most 2*(N1 + N2 - 1) comparisons, where \a N1 is
    ///         the length of the first sequence and \a N2 is the length of the
    ///         second sequence.
    ///
    /// If some element is found \a m times in [first1, last1) and \a n times
    /// in [first2, last2), it will be copied to \a dest exactly std::abs(m-n)
    /// times. If m>n, then the last m-n of those elements are copied from
    /// [first1,last1), otherwise the last n-m elements are copied from
    /// [first2,last2). The resulting range cannot overlap with either of the
    /// input ranges.
    ///
    /// The resulting range cannot overlap with either of the input ranges.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it applies user-provided function objects.
    /// \tparam FwdIter1    The type of the source iterators used (deduced)
    ///                     representing the first sequence.
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the source iterators used (deduced)
    ///                     representing the first sequence.
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter3    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a set_symmetric_difference requires \a Pred
    ///                     to meet the requirements of \a CopyConstructible. This
    ///                     defaults to std::less<>
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first1       Refers to the beginning of the sequence of elements
    ///                     of the first range the algorithm will be applied to.
    /// \param last1        Refers to the end of the sequence of elements of
    ///                     the first range the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the sequence of elements
    ///                     of the second range the algorithm will be applied to.
    /// \param last2        Refers to the end of the sequence of elements of
    ///                     the second range the algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param op           The binary predicate which returns true if the
    ///                     elements should be treated as equal. The signature
    ///                     of the predicate function should be equivalent to
    ///                     the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type1 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type1 must be such
    ///                     that objects of type \a InIter can
    ///                     be dereferenced and then implicitly converted to
    ///                     \a Type1
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with a sequential execution policy object execute in sequential
    /// order in the calling thread (\a sequenced_policy) or in a
    /// single new thread spawned from the current thread
    /// (for \a sequenced_task_policy).
    ///
    /// The application of function objects in parallel algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_policy or \a parallel_task_policy are
    /// permitted to execute in an unordered fashion in unspecified
    /// threads, and indeterminately sequenced within each thread.
    ///
    /// \returns  The \a set_symmetric_difference algorithm returns a
    ///           \a pika::future<FwdIter3>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter3 otherwise.
    ///           The \a set_symmetric_difference algorithm returns the output
    ///           iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
        typename FwdIter3, typename Pred = pika::parallel::detail::less>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter3>::type>::type
    set_symmetric_difference(ExPolicy&& policy, FwdIter1 first1, FwdIter1 last1,
        FwdIter2 first2, FwdIter2 last2, FwdIter3 dest, Pred&& op = Pred());

    // clang-format on
}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>

#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/copy.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>
#include <pika/parallel/algorithms/detail/set_operation.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/projection_identity.hpp>
#include <pika/parallel/util/result_types.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // set_symmetric_difference
    template <typename Iter1, typename Sent1, typename Iter2, typename Sent2,
        typename Iter3, typename Comp, typename Proj1, typename Proj2>
    constexpr in_in_out_result<Iter1, Iter2, Iter3>
    sequential_set_symmetric_difference(Iter1 first1, Sent1 last1, Iter2 first2,
        Sent2 last2, Iter3 dest, Comp&& comp, Proj1&& proj1, Proj2&& proj2)
    {
        while (first1 != last1)
        {
            if (first2 == last2)
            {
                auto result = (copy) (first1, last1, dest);
                return {result.in, first2, result.out};
            }

            auto&& value1 = PIKA_INVOKE(proj1, *first1);
            auto&& value2 = PIKA_INVOKE(proj2, *first2);

            if (PIKA_INVOKE(comp, value1, value2))
            {
                *dest++ = *first1++;
            }
            else
            {
                if (PIKA_INVOKE(comp, value2, value1))
                {
                    *dest++ = *first2;
                }
                else
                {
                    ++first1;
                }
                ++first2;
            }
        }

        auto result = (copy) (first2, last2, dest);
        return {first1, result.in, result.out};
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename Result>
    struct set_symmetric_difference
      : public algorithm<set_symmetric_difference<Result>, Result>
    {
        set_symmetric_difference()
          : set_symmetric_difference::algorithm("set_symmetric_difference")
        {
        }

        template <typename ExPolicy, typename Iter1, typename Sent1,
            typename Iter2, typename Sent2, typename Iter3, typename F,
            typename Proj1, typename Proj2>
        static in_in_out_result<Iter1, Iter2, Iter3>
        sequential(ExPolicy, Iter1 first1, Sent1 last1, Iter2 first2,
            Sent2 last2, Iter3 dest, F&& f, Proj1&& proj1, Proj2&& proj2)
        {
            return sequential_set_symmetric_difference(first1, last1, first2,
                last2, dest, PIKA_FORWARD(F, f), PIKA_FORWARD(Proj1, proj1),
                PIKA_FORWARD(Proj2, proj2));
        }

        template <typename ExPolicy, typename Iter1, typename Sent1,
            typename Iter2, typename Sent2, typename Iter3, typename F,
            typename Proj1, typename Proj2>
        static typename algorithm_result<ExPolicy,
            in_in_out_result<Iter1, Iter2, Iter3>>::type
        parallel(ExPolicy&& policy, Iter1 first1, Sent1 last1, Iter2 first2,
            Sent2 last2, Iter3 dest, F&& f, Proj1&& proj1, Proj2&& proj2)
        {
            using difference_type1 =
                typename std::iterator_traits<Iter1>::difference_type;
            using difference_type2 =
                typename std::iterator_traits<Iter2>::difference_type;

            using result_type = in_in_out_result<Iter1, Iter2, Iter3>;

            if (first1 == last1)
            {
                return convert_to_result(
                    copy_algo<in_out_result<Iter2, Iter3>>().call(
                        PIKA_FORWARD(ExPolicy, policy), first2, last2, dest),
                    [first1](
                        in_out_result<Iter2, Iter3> const& p) -> result_type {
                        return {first1, p.in, p.out};
                    });
            }

            if (first2 == last2)
            {
                return convert_to_result(
                    copy_algo<in_out_result<Iter1, Iter3>>().call(
                        PIKA_FORWARD(ExPolicy, policy), first1, last1, dest),
                    [first2](
                        in_out_result<Iter1, Iter3> const& p) -> result_type {
                        return {p.in, first2, p.out};
                    });
            }

            using buffer_type = typename set_operations_buffer<Iter3>::type;
            using func_type = std::decay_t<F>;

            // calculate approximate destination index
            auto f1 = [](difference_type1 idx1,
                          difference_type2 idx2) -> difference_type1 {
                return idx1 + idx2;
            };

            // perform required set operation for one chunk
            auto f2 = [proj1, proj2](Iter1 part_first1, Sent1 part_last1,
                          Iter2 part_first2, Sent2 part_last2,
                          buffer_type* dest, func_type const& f) {
                return sequential_set_symmetric_difference(part_first1,
                    part_last1, part_first2, part_last2, dest, f, proj1, proj2);
            };

            return set_operation(PIKA_FORWARD(ExPolicy, policy), first1, last1,
                first2, last2, dest, PIKA_FORWARD(F, f),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2),
                PIKA_MOVE(f1), PIKA_MOVE(f2));
        }
    };
}    // namespace pika::parallel::detail

namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::set_symmetric_difference
    inline constexpr struct set_symmetric_difference_t final
      : pika::detail::tag_parallel_algorithm<set_symmetric_difference_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename FwdIter3, typename Pred = pika::parallel::detail::less,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value &&
                pika::traits::is_iterator<FwdIter3>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter1>::value_type,
                    typename std::iterator_traits<FwdIter2>::value_type
                >
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            FwdIter3>::type
        tag_fallback_invoke(set_symmetric_difference_t, ExPolicy&& policy,
            FwdIter1 first1, FwdIter1 last1, FwdIter2 first2, FwdIter2 last2,
            FwdIter3 dest, Pred&& op = Pred())
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_forward_iterator<FwdIter2>::value),
                "Requires at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter3>::value ||
                    (pika::is_sequenced_execution_policy<ExPolicy>::value &&
                        pika::traits::is_output_iterator<FwdIter3>::value),
                "Requires at least forward iterator or sequential execution.");

            using is_seq = std::integral_constant<bool,
                pika::is_sequenced_execution_policy<ExPolicy>::value ||
                    !pika::traits::is_random_access_iterator<FwdIter1>::value ||
                    !pika::traits::is_random_access_iterator<FwdIter2>::value>;

            using result_type =
                pika::parallel::detail::in_in_out_result<FwdIter1, FwdIter2,
                    FwdIter3>;

            return pika::parallel::detail::get_third_element(
                pika::parallel::detail::set_symmetric_difference<result_type>()
                    .call2(PIKA_FORWARD(ExPolicy, policy), is_seq(), first1,
                        last1, first2, last2, dest, PIKA_FORWARD(Pred, op),
                        pika::parallel::detail::projection_identity(),
                        pika::parallel::detail::projection_identity()));
        }

        // clang-format off
        template <typename FwdIter1, typename FwdIter2, typename FwdIter3,
            typename Pred = pika::parallel::detail::less,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value &&
                pika::traits::is_iterator<FwdIter3>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter1>::value_type,
                    typename std::iterator_traits<FwdIter2>::value_type
                >
            )>
        // clang-format on
        friend FwdIter3 tag_fallback_invoke(set_symmetric_difference_t,
            FwdIter1 first1, FwdIter1 last1, FwdIter2 first2, FwdIter2 last2,
            FwdIter3 dest, Pred&& op = Pred())
        {
            static_assert((pika::traits::is_input_iterator<FwdIter1>::value),
                "Requires at least input iterator.");
            static_assert((pika::traits::is_input_iterator<FwdIter2>::value),
                "Requires at least input iterator.");
            static_assert((pika::traits::is_output_iterator<FwdIter3>::value),
                "Requires at least output iterator.");

            using result_type =
                pika::parallel::detail::in_in_out_result<FwdIter1, FwdIter2,
                    FwdIter3>;

            return pika::parallel::detail::get_third_element(
                pika::parallel::detail::set_symmetric_difference<result_type>()
                    .call(pika::execution::seq, first1, last1, first2, last2,
                        dest, PIKA_FORWARD(Pred, op),
                        pika::parallel::detail::projection_identity(),
                        pika::parallel::detail::projection_identity()));
        }
    } set_symmetric_difference{};
}    // namespace pika

#endif    // DOXYGEN
