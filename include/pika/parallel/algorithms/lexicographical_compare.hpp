//  Copyright (c) 2014 Grant Mercer
//  Copyright (c) 2017 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/lexicographical_compare.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {

    /// Checks if the first range [first1, last1) is lexicographically less than
    /// the second range [first2, last2). uses a provided predicate to compare
    /// elements.
    ///
    /// \note   Complexity: At most 2 * min(N1, N2) applications of the comparison
    ///         operation, where N1 = std::distance(first1, last)
    ///         and N2 = std::distance(first2, last2).
    ///
    /// \tparam InIter1    The type of the source iterators used for the
    ///                     first range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam InIter2    The type of the source iterators used for the
    ///                     second range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a lexicographical_compare requires \a Pred to
    ///                     meet the requirements of \a CopyConstructible. This defaults
    ///                     to std::less<>
    ///
    /// \param first1       Refers to the beginning of the sequence of elements
    ///                     of the first range the algorithm will be applied to.
    /// \param last1        Refers to the end of the sequence of elements of
    ///                     the first range the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the sequence of elements
    ///                     of the second range the algorithm will be applied to.
    /// \param last2        Refers to the end of the sequence of elements of
    ///                     the second range the algorithm will be applied to.
    /// \param pred         Refers to the comparison function that the first
    ///                     and second ranges will be applied to
    ///
    /// The comparison operations in the parallel \a lexicographical_compare
    /// algorithm invoked without an execution policy object execute in sequential
    /// order in the calling thread.
    ///
    /// \note     Lexicographical comparison is an operation with the
    ///           following properties
    ///             - Two ranges are compared element by element
    ///             - The first mismatching element defines which range
    ///               is lexicographically
    ///               \a less or \a greater than the other
    ///             - If one range is a prefix of another, the shorter range is
    ///               lexicographically \a less than the other
    ///             - If two ranges have equivalent elements and are of the same length,
    ///               then the ranges are lexicographically \a equal
    ///             - An empty range is lexicographically \a less than any non-empty
    ///               range
    ///             - Two empty ranges are lexicographically \a equal
    ///
    /// \returns  The \a lexicographically_compare algorithm returns a
    ///           returns \a bool if the execution policy object is not passed in.
    ///           The \a lexicographically_compare algorithm returns true
    ///           if the first range is lexicographically less, otherwise
    ///           it returns false.
    ///           range [first2, last2), it returns false.
    template <typename InIter1, typename InIter2, typename Pred>
    bool lexicographical_compare(InIter1 first1, InIter1 last1, InIter2 first2,
        InIter2 last2, Pred&& pred);

    /// Checks if the first range [first1, last1) is lexicographically less than
    /// the second range [first2, last2). uses a provided predicate to compare
    /// elements.
    ///
    /// \note   Complexity: At most 2 * min(N1, N2) applications of the comparison
    ///         operation, where N1 = std::distance(first1, last)
    ///         and N2 = std::distance(first2, last2).
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the source iterators used for the
    ///                     first range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the source iterators used for the
    ///                     second range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a lexicographical_compare requires \a Pred to
    ///                     meet the requirements of \a CopyConstructible. This defaults
    ///                     to std::less<>
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
    /// \param pred         Refers to the comparison function that the first
    ///                     and second ranges will be applied to
    ///
    /// The comparison operations in the parallel \a lexicographical_compare
    /// algorithm invoked with an execution policy object of type
    /// \a sequenced_policy execute in sequential order in the
    /// calling thread.
    ///
    /// The comparison operations in the parallel \a lexicographical_compare
    /// algorithm invoked with an execution policy object of type
    /// \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \note     Lexicographical comparison is an operation with the
    ///           following properties
    ///             - Two ranges are compared element by element
    ///             - The first mismatching element defines which range
    ///               is lexicographically
    ///               \a less or \a greater than the other
    ///             - If one range is a prefix of another, the shorter range is
    ///               lexicographically \a less than the other
    ///             - If two ranges have equivalent elements and are of the same length,
    ///               then the ranges are lexicographically \a equal
    ///             - An empty range is lexicographically \a less than any non-empty
    ///               range
    ///             - Two empty ranges are lexicographically \a equal
    ///
    /// \returns  The \a lexicographically_compare algorithm returns a
    ///           \a pika::future<bool> if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a bool otherwise.
    ///           The \a lexicographically_compare algorithm returns true
    ///           if the first range is lexicographically less, otherwise
    ///           it returns false.
    ///           range [first2, last2), it returns false.
    template <typename FwdIter1, typename FwdIter2, typename Pred>
    typename pika::parallel::detail::algorithm_result<ExPolicy, bool>::type
    lexicographical_compare(ExPolicy&& policy, FwdIter1 first1, FwdIter1 last1,
        FwdIter2 first2, FwdIter2 last2, Pred&& pred);

}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>

#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>
#include <pika/parallel/algorithms/for_each.hpp>
#include <pika/parallel/algorithms/mismatch.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/loop.hpp>
#include <pika/parallel/util/partitioner.hpp>
#include <pika/parallel/util/zip_iterator.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // lexicographical_compare
    /// \cond NOINTERNAL
    struct lexicographical_compare
      : public algorithm<lexicographical_compare, bool>
    {
        lexicographical_compare()
          : lexicographical_compare::algorithm("lexicographical_compare")
        {
        }

        template <typename ExPolicy, typename InIter1, typename Sent1,
            typename InIter2, typename Sent2, typename Pred, typename Proj1,
            typename Proj2>
        static bool
        sequential(ExPolicy, InIter1 first1, Sent1 last1, InIter2 first2,
            Sent2 last2, Pred&& pred, Proj1&& proj1, Proj2&& proj2)
        {
            for (; (first1 != last1) && (first2 != last2);
                 ++first1, (void) ++first2)
            {
                if (PIKA_INVOKE(pred, PIKA_INVOKE(proj1, *first1),
                        PIKA_INVOKE(proj2, *first2)))
                    return true;
                if (PIKA_INVOKE(pred, PIKA_INVOKE(proj2, *first2),
                        PIKA_INVOKE(proj1, *first1)))
                    return false;
            }
            return (first1 == last1) && (first2 != last2);
        }

        template <typename ExPolicy, typename FwdIter1, typename Sent1,
            typename FwdIter2, typename Sent2, typename Pred, typename Proj1,
            typename Proj2>
        static typename algorithm_result<ExPolicy, bool>::type parallel(
            ExPolicy&& policy, FwdIter1 first1, Sent1 last1, FwdIter2 first2,
            Sent2 last2, Pred&& pred, Proj1&& proj1, Proj2&& proj2)
        {
            using zip_iterator = pika::util::zip_iterator<FwdIter1, FwdIter2>;
            using reference = typename zip_iterator::reference;

            std::size_t count1 = detail::distance(first1, last1);
            std::size_t count2 = detail::distance(first2, last2);

            // An empty range is lexicographically less than any non-empty
            // range
            if (count1 == 0 && count2 != 0)
            {
                return algorithm_result<ExPolicy, bool>::get(true);
            }

            if (count2 == 0 && count1 != 0)
            {
                return algorithm_result<ExPolicy, bool>::get(false);
            }

            std::size_t count = (std::min)(count1, count2);
            util::cancellation_token<std::size_t> tok(count);

            auto f1 = [tok, pred, proj1, proj2](zip_iterator it,
                          std::size_t part_count,
                          std::size_t base_idx) mutable -> void {
                loop_idx_n<std::decay_t<ExPolicy>>(base_idx, it, part_count,
                    tok,
                    [&pred, &tok, &proj1, &proj2](
                        reference t, std::size_t i) mutable -> void {
                        using std::get;
                        if (PIKA_INVOKE(pred, PIKA_INVOKE(proj1, get<0>(t)),
                                PIKA_INVOKE(proj2, get<1>(t))) ||
                            PIKA_INVOKE(pred, PIKA_INVOKE(proj2, get<1>(t)),
                                PIKA_INVOKE(proj1, get<0>(t))))
                        {
                            tok.cancel(i);
                        }
                    });
            };

            auto f2 =
                [tok, first1, first2, last1, last2, pred, proj1, proj2](
                    std::vector<pika::future<void>>&& data) mutable -> bool {
                // make sure iterators embedded in function object that is
                // attached to futures are invalidated
                data.clear();

                std::size_t mismatched = tok.get_data();

                std::advance(first1, mismatched);
                std::advance(first2, mismatched);

                if (first1 != last1 && first2 != last2)
                    return PIKA_INVOKE(pred, PIKA_INVOKE(proj1, *first1),
                        PIKA_INVOKE(proj2, *first2));

                return first2 != last2;
            };

            using pika::util::make_zip_iterator;
            return detail::partitioner<ExPolicy, bool, void>::call_with_index(
                PIKA_FORWARD(ExPolicy, policy),
                make_zip_iterator(first1, first2), count, 1, PIKA_MOVE(f1),
                PIKA_MOVE(f2));
        }
    };
    /// \endcond
}    // namespace pika::parallel::detail

namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::lexicographical_compare
    inline constexpr struct lexicographical_compare_t final
      : pika::detail::tag_parallel_algorithm<lexicographical_compare_t>
    {
        // clang-format off
        template <typename InIter1, typename InIter2,
            typename Pred = pika::parallel::detail::less,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter1>::value &&
                pika::traits::is_iterator<InIter2>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<InIter1>::value_type,
                    typename std::iterator_traits<InIter2>::value_type
                >
            )>
        // clang-format on
        friend bool
        tag_fallback_invoke(pika::lexicographical_compare_t, InIter1 first1,
            InIter1 last1, InIter2 first2, InIter2 last2, Pred&& pred = Pred())
        {
            static_assert(pika::traits::is_input_iterator<InIter1>::value,
                "Requires at least input iterator.");
            static_assert(pika::traits::is_input_iterator<InIter2>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::lexicographical_compare().call(
                pika::execution::seq, first1, last1, first2, last2,
                PIKA_FORWARD(Pred, pred),
                pika::parallel::detail::projection_identity{},
                pika::parallel::detail::projection_identity{});
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename Pred = pika::parallel::detail::less,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter1>::value_type,
                    typename std::iterator_traits<FwdIter2>::value_type
                >
            )>
        // clang-format on
        friend typename parallel::detail::algorithm_result<ExPolicy, bool>::type
        tag_fallback_invoke(pika::lexicographical_compare_t, ExPolicy&& policy,
            FwdIter1 first1, FwdIter1 last1, FwdIter2 first2, FwdIter2 last2,
            Pred&& pred = Pred())
        {
            static_assert(pika::traits::is_forward_iterator<FwdIter1>::value,
                "Requires at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter2>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::lexicographical_compare().call(
                PIKA_FORWARD(ExPolicy, policy), first1, last1, first2, last2,
                PIKA_FORWARD(Pred, pred),
                pika::parallel::detail::projection_identity{},
                pika::parallel::detail::projection_identity{});
        }

    } lexicographical_compare{};
}    // namespace pika

#endif    // DOXYGEN
