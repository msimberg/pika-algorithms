//  Copyright (c) 2017 Taeguk Kwon
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/container_algorithms/merge.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika { namespace ranges {
    // clang-format off

    /// Merges two sorted ranges [first1, last1) and [first2, last2)
    /// into one sorted range beginning at \a dest. The order of
    /// equivalent elements in the each of original two ranges is preserved.
    /// For equivalent elements in the original two ranges, the elements from
    /// the first range precede the elements from the second range.
    /// The destination range cannot overlap with either of the input ranges.
    ///
    /// \note   Complexity: Performs
    ///         O(std::distance(first1, last1) + std::distance(first2, last2))
    ///         applications of the comparison \a comp and the each projection.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter1       The type of the source iterators used (deduced)
    ///                     representing the first sequence.
    ///                     This iterator type must meet the requirements of an
    ///                     random access iterator.
    /// \tparam Sent1       The type of the end source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter1.
    /// \tparam Iter2       The type of the source iterators used (deduced)
    ///                     representing the second sequence.
    ///                     This iterator type must meet the requirements of an
    ///                     random access iterator.
    /// \tparam Sent2       The type of the end source iterators used (deduced)
    ///                     representing the second sequence.
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter2.
    /// \tparam Iter3       The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     random access iterator.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a merge requires \a Comp to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::less<>
    /// \tparam Proj1       The type of an optional projection function to be
    ///                     used for elements of the first range. This defaults
    ///                     to \a parallel::detail::projection_identity
    /// \tparam Proj2       The type of an optional projection function to be
    ///                     used for elements of the second range. This defaults
    ///                     to \a parallel::detail::projection_identity
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
    /// \param comp         \a comp is a callable object which returns true if
    ///                     the first argument is less than the second,
    ///                     and false otherwise. The signature of this
    ///                     comparison should be equivalent to:
    ///                     \code
    ///                     bool comp(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be such that
    ///                     objects of types \a RandIter1 and \a RandIter2 can be
    ///                     dereferenced and then implicitly converted to
    ///                     both \a Type1 and \a Type2
    /// \param proj1        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the
    ///                     first range as a projection operation before the
    ///                     actual comparison \a comp is invoked.
    /// \param proj2        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the
    ///                     second range as a projection operation before the
    ///                     actual comparison \a comp is invoked.
    ///
    /// The assignments in the parallel \a merge algorithm invoked with
    /// an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a merge algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a merge algorithm returns a
    /// \a pika::future<merge_result<Iter1, Iter2, Iter3>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and returns
    ///           \a merge_result<Iter1, Iter2, Iter3> otherwise.
    ///           The \a merge algorithm returns the tuple of
    ///           the source iterator \a last1,
    ///           the source iterator \a last2,
    ///           the destination iterator to the end of the \a dest range.
    ///
    template <typename ExPolicy, typename Iter1, typename Sent, typename Iter2,
        typename Sent2, typename Iter3, typename Comp = pika::ranges::less,
        typename Proj1 = parallel::detail::projection_identity,
        typename Proj2 = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        pika::ranges::merge_result<Iter1, Iter2, Iter3>>::type
    merge(ExPolicy&& policy, Iter1 first1, Sent1 last1, Iter2 first2,
        Sent2 last2, Iter3 dest, Comp&& comp = Comp(), Proj1&& proj1 = Proj1(),
        Proj2&& proj2 = Proj2());

    /// Merges two sorted ranges [first1, last1) and [first2, last2)
    /// into one sorted range beginning at \a dest. The order of
    /// equivalent elements in the each of original two ranges is preserved.
    /// For equivalent elements in the original two ranges, the elements from
    /// the first range precede the elements from the second range.
    /// The destination range cannot overlap with either of the input ranges.
    ///
    /// \note   Complexity: Performs
    ///         O(std::distance(first1, last1) + std::distance(first2, last2))
    ///         applications of the comparison \a comp and the each projection.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng1        The type of the first source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an random access iterator.
    /// \tparam Rng2        The type of the second source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an random access iterator.
    /// \tparam RandIter3   The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     random access iterator.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a merge requires \a Comp to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::less<>
    /// \tparam Proj1       The type of an optional projection function to be
    ///                     used for elements of the first range. This defaults
    ///                     to \a parallel::detail::projection_identity
    /// \tparam Proj2       The type of an optional projection function to be
    ///                     used for elements of the second range. This defaults
    ///                     to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng1         Refers to the first range of elements the algorithm
    ///                     will be applied to.
    /// \param rng2         Refers to the second range of elements the algorithm
    ///                     will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param comp         \a comp is a callable object which returns true if
    ///                     the first argument is less than the second,
    ///                     and false otherwise. The signature of this
    ///                     comparison should be equivalent to:
    ///                     \code
    ///                     bool comp(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be such that
    ///                     objects of types \a RandIter1 and \a RandIter2 can be
    ///                     dereferenced and then implicitly converted to
    ///                     both \a Type1 and \a Type2
    /// \param proj1        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the
    ///                     first range as a projection operation before the
    ///                     actual comparison \a comp is invoked.
    /// \param proj2        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the
    ///                     second range as a projection operation before the
    ///                     actual comparison \a comp is invoked.
    ///
    /// The assignments in the parallel \a merge algorithm invoked with
    /// an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a merge algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a merge algorithm returns a
    /// \a pika::future<merge_result<RandIter1, RandIter2, RandIter3>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and returns
    ///           \a merge_result<RandIter1, RandIter2, RandIter3> otherwise.
    ///           The \a merge algorithm returns the tuple of
    ///           the source iterator \a last1,
    ///           the source iterator \a last2,
    ///           the destination iterator to the end of the \a dest range.
    ///
    template <typename ExPolicy, typename Rng1, typename Rng2,
        typename RandIter3, typename Comp = pika::ranges::less,
        typename Proj1 = parallel::detail::projection_identity,
        typename Proj2 = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        pika::ranges::merge_result<
            typename pika::traits::range_iterator<Rng1>::type,
            typename pika::traits::range_iterator<Rng2>::type,
            RandIter3
        >
    >::type
    merge(ExPolicy&& policy, Rng1&& rng1, Rng2&& rng2, RandIter3 dest,
        Comp&& comp = Comp(), Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2());

    /// Merges two consecutive sorted ranges [first, middle) and
    /// [middle, last) into one sorted range [first, last). The order of
    /// equivalent elements in the each of original two ranges is preserved.
    /// For equivalent elements in the original two ranges, the elements from
    /// the first range precede the elements from the second range.
    ///
    /// \note   Complexity: Performs O(std::distance(first, last))
    ///         applications of the comparison \a comp and the each projection.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     random access iterator.
    /// \tparam Sent       The type of the end source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter1.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a inplace_merge requires \a Comp
    ///                     to meet the requirements of \a CopyConstructible.
    ///                     This defaults to std::less<>
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the first sorted range
    ///                     the algorithm will be applied to.
    /// \param middle       Refers to the end of the first sorted range and
    ///                     the beginning of the second sorted range
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the second sorted range
    ///                     the algorithm will be applied to.
    /// \param comp         \a comp is a callable object which returns true if
    ///                     the first argument is less than the second,
    ///                     and false otherwise. The signature of this
    ///                     comparison should be equivalent to:
    ///                     \code
    ///                     bool comp(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be
    ///                     such that objects of types \a RandIter can be
    ///                     dereferenced and then implicitly converted to both
    ///                     \a Type1 and \a Type2
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a inplace_merge algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a inplace_merge algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a inplace_merge algorithm returns a
    ///           \a pika::future<Iter> if the execution policy is of type
    ///           \a sequenced_task_policy or \a parallel_task_policy
    ///           and returns \a Iter otherwise.
    ///           The \a inplace_merge algorithm returns
    ///           the source iterator \a last
    ///
    template <typename ExPolicy, typename Iter, typename Sent,
        typename Comp = pika::ranges::less,
        typename Proj = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, Iter>::type
    inplace_merge(ExPolicy&& policy, Iter first, Iter middle, Sent last,
        Comp&& comp = Comp(), Proj&& proj = Proj());

    /// Merges two consecutive sorted ranges [first, middle) and
    /// [middle, last) into one sorted range [first, last). The order of
    /// equivalent elements in the each of original two ranges is preserved.
    /// For equivalent elements in the original two ranges, the elements from
    /// the first range precede the elements from the second range.
    ///
    /// \note   Complexity: Performs O(std::distance(first, last))
    ///         applications of the comparison \a comp and the each projection.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an random access iterator.
    /// \tparam RandIter    The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     random access iterator.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a inplace_merge requires \a Comp
    ///                     to meet the requirements of \a CopyConstructible.
    ///                     This defaults to std::less<>
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the range of elements the algorithm
    ///                     will be applied to.
    /// \param middle       Refers to the end of the first sorted range and
    ///                     the beginning of the second sorted range
    ///                     the algorithm will be applied to.
    /// \param comp         \a comp is a callable object which returns true if
    ///                     the first argument is less than the second,
    ///                     and false otherwise. The signature of this
    ///                     comparison should be equivalent to:
    ///                     \code
    ///                     bool comp(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be
    ///                     such that objects of types \a RandIter can be
    ///                     dereferenced and then implicitly converted to both
    ///                     \a Type1 and \a Type2
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a inplace_merge algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a inplace_merge algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a inplace_merge algorithm returns a
    ///           \a pika::future<RandIter> if the execution policy is of type
    ///           \a sequenced_task_policy or \a parallel_task_policy
    ///           and returns \a RandIter otherwise.
    ///           The \a inplace_merge algorithm returns
    ///           the source iterator \a last
    ///
    template <typename ExPolicy, typename Rng, typename RandIter,
        typename Comp = pika::ranges::less,
        typename Proj = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, RandIter>::type
    inplace_merge(ExPolicy&& policy, Rng&& rng, RandIter middle,
        Comp&& comp = Comp(), Proj&& proj = Proj());

    // clang-format on
}}    // namespace pika::ranges

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/iterator_support/traits/is_sentinel_for.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/algorithms/traits/projected_range.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/merge.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>
#include <pika/parallel/util/result_types.hpp>

#include <type_traits>
#include <utility>

namespace pika { namespace ranges {

    template <typename I1, typename I2, typename O>
    using merge_result = pika::parallel::detail::in_in_out_result<I1, I2, O>;

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::merge
    inline constexpr struct merge_t final
      : pika::detail::tag_parallel_algorithm<merge_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename Rng1, typename Rng2,
            typename Iter3, typename Comp = pika::ranges::less,
            typename Proj1 = pika::parallel::detail::projection_identity,
            typename Proj2 = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng1>::value &&
                pika::parallel::detail::is_projected_range<Proj1, Rng1>::value &&
                pika::traits::is_range<Rng2>::value &&
                pika::parallel::detail::is_projected_range<Proj2, Rng2>::value &&
                pika::traits::is_iterator<Iter3>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Comp,
                    pika::parallel::detail::projected_range<Proj1, Rng1>,
                    pika::parallel::detail::projected_range<Proj2, Rng2>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            pika::ranges::merge_result<
                typename pika::traits::range_iterator<Rng1>::type,
                typename pika::traits::range_iterator<Rng2>::type, Iter3>>::type
        tag_fallback_invoke(merge_t, ExPolicy&& policy, Rng1&& rng1,
            Rng2&& rng2, Iter3 dest, Comp&& comp = Comp(),
            Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2())
        {
            using iterator_type1 =
                typename pika::traits::range_iterator<Rng1>::type;
            using iterator_type2 =
                typename pika::traits::range_iterator<Rng2>::type;

            static_assert(
                pika::traits::is_random_access_iterator<iterator_type1>::value,
                "Required at least random access iterator.");
            static_assert(
                pika::traits::is_random_access_iterator<iterator_type2>::value,
                "Requires at least random access iterator.");
            static_assert(pika::traits::is_random_access_iterator<Iter3>::value,
                "Requires at least random access iterator.");

            using result_type = pika::ranges::merge_result<iterator_type1,
                iterator_type2, Iter3>;

            return pika::parallel::detail::merge<result_type>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng1),
                pika::util::end(rng1), pika::util::begin(rng2),
                pika::util::end(rng2), dest, PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2));
        }

        // clang-format off
        template <typename ExPolicy, typename Iter1, typename Sent1,
            typename Iter2, typename Sent2, typename Iter3,
            typename Comp = pika::ranges::less,
            typename Proj1 = pika::parallel::detail::projection_identity,
            typename Proj2 = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_sentinel_for<Sent1, Iter1>::value &&
                pika::parallel::detail::is_projected<Proj1, Iter1>::value &&
                pika::traits::is_sentinel_for<Sent2, Iter2>::value &&
                pika::parallel::detail::is_projected<Proj2, Iter2>::value &&
                pika::traits::is_iterator<Iter3>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Comp,
                    pika::parallel::detail::projected<Proj1, Iter1>,
                    pika::parallel::detail::projected<Proj2, Iter2>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            pika::ranges::merge_result<Iter1, Iter2, Iter3>>::type
        tag_fallback_invoke(merge_t, ExPolicy&& policy, Iter1 first1,
            Sent1 last1, Iter2 first2, Sent2 last2, Iter3 dest,
            Comp&& comp = Comp(), Proj1&& proj1 = Proj1(),
            Proj2&& proj2 = Proj2())
        {
            static_assert(pika::traits::is_random_access_iterator<Iter1>::value,
                "Required at least random access iterator.");
            static_assert(pika::traits::is_random_access_iterator<Iter2>::value,
                "Requires at least random access iterator.");
            static_assert(pika::traits::is_random_access_iterator<Iter3>::value,
                "Requires at least random access iterator.");

            using result_type = pika::ranges::merge_result<Iter1, Iter2, Iter3>;

            return pika::parallel::detail::merge<result_type>().call(
                PIKA_FORWARD(ExPolicy, policy), first1, last1, first2, last2,
                dest, PIKA_FORWARD(Comp, comp), PIKA_FORWARD(Proj1, proj1),
                PIKA_FORWARD(Proj2, proj2));
        }

        // clang-format off
        template <typename Rng1, typename Rng2,
            typename Iter3, typename Comp = pika::ranges::less,
            typename Proj1 = pika::parallel::detail::projection_identity,
            typename Proj2 = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng1>::value &&
                pika::parallel::detail::is_projected_range<Proj1, Rng1>::value &&
                pika::traits::is_range<Rng2>::value &&
                pika::parallel::detail::is_projected_range<Proj2, Rng2>::value &&
                pika::traits::is_iterator<Iter3>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Comp,
                    pika::parallel::detail::projected_range<Proj1, Rng1>,
                    pika::parallel::detail::projected_range<Proj2, Rng2>
                >::value
            )>
        // clang-format on
        friend pika::ranges::merge_result<
            typename pika::traits::range_iterator<Rng1>::type,
            typename pika::traits::range_iterator<Rng2>::type, Iter3>
        tag_fallback_invoke(merge_t, Rng1&& rng1, Rng2&& rng2, Iter3 dest,
            Comp&& comp = Comp(), Proj1&& proj1 = Proj1(),
            Proj2&& proj2 = Proj2())
        {
            using iterator_type1 =
                typename pika::traits::range_iterator<Rng1>::type;
            using iterator_type2 =
                typename pika::traits::range_iterator<Rng2>::type;

            static_assert(
                pika::traits::is_random_access_iterator<iterator_type1>::value,
                "Required at least random access iterator.");
            static_assert(
                pika::traits::is_random_access_iterator<iterator_type2>::value,
                "Requires at least random access iterator.");
            static_assert(pika::traits::is_random_access_iterator<Iter3>::value,
                "Requires at least random access iterator.");

            using result_type = pika::ranges::merge_result<iterator_type1,
                iterator_type2, Iter3>;

            return pika::parallel::detail::merge<result_type>().call(
                pika::execution::seq, pika::util::begin(rng1),
                pika::util::end(rng1), pika::util::begin(rng2),
                pika::util::end(rng2), dest, PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2));
        }

        // clang-format off
        template <typename Iter1, typename Sent1,
            typename Iter2, typename Sent2, typename Iter3,
            typename Comp = pika::ranges::less,
            typename Proj1 = pika::parallel::detail::projection_identity,
            typename Proj2 = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_sentinel_for<Sent1, Iter1>::value &&
                pika::parallel::detail::is_projected<Proj1, Iter1>::value &&
                pika::traits::is_sentinel_for<Sent2, Iter2>::value &&
                pika::parallel::detail::is_projected<Proj2, Iter2>::value &&
                pika::traits::is_iterator<Iter3>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Comp,
                    pika::parallel::detail::projected<Proj1, Iter1>,
                    pika::parallel::detail::projected<Proj2, Iter2>
                >::value
            )>
        // clang-format on
        friend pika::ranges::merge_result<Iter1, Iter2, Iter3>
        tag_fallback_invoke(merge_t, Iter1 first1, Sent1 last1, Iter2 first2,
            Sent2 last2, Iter3 dest, Comp&& comp = Comp(),
            Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2())
        {
            static_assert(pika::traits::is_random_access_iterator<Iter1>::value,
                "Required at least random access iterator.");
            static_assert(pika::traits::is_random_access_iterator<Iter2>::value,
                "Requires at least random access iterator.");
            static_assert(pika::traits::is_random_access_iterator<Iter3>::value,
                "Requires at least random access iterator.");

            using result_type = pika::ranges::merge_result<Iter1, Iter2, Iter3>;

            return pika::parallel::detail::merge<result_type>().call(
                pika::execution::seq, first1, last1, first2, last2, dest,
                PIKA_FORWARD(Comp, comp), PIKA_FORWARD(Proj1, proj1),
                PIKA_FORWARD(Proj2, proj2));
        }
    } merge{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::inplace_merge
    inline constexpr struct inplace_merge_t final
      : pika::detail::tag_parallel_algorithm<inplace_merge_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename Rng, typename Iter,
            typename Comp = pika::ranges::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::traits::is_iterator<Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Comp,
                    pika::parallel::detail::projected_range<Proj, Rng>,
                    pika::parallel::detail::projected_range<Proj, Rng>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(inplace_merge_t, ExPolicy&& policy, Rng&& rng,
            Iter middle, Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(
                pika::traits::is_random_access_iterator<iterator_type>::value,
                "Required at least random access iterator.");
            static_assert(pika::traits::is_random_access_iterator<Iter>::value,
                "Required at least random access iterator.");

            return pika::parallel::detail::inplace_merge<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng), middle,
                pika::util::end(rng), PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent,
            typename Comp = pika::ranges::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Comp,
                    pika::parallel::detail::projected<Proj, Iter>,
                    pika::parallel::detail::projected<Proj, Iter>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(inplace_merge_t, ExPolicy&& policy, Iter first,
            Iter middle, Sent last, Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_random_access_iterator<Iter>::value,
                "Required at least random access iterator.");

            return pika::parallel::detail::inplace_merge<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, middle, last,
                PIKA_FORWARD(Comp, comp), PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng, typename Iter,
            typename Comp = pika::ranges::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::traits::is_iterator<Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Comp,
                    pika::parallel::detail::projected_range<Proj, Rng>,
                    pika::parallel::detail::projected_range<Proj, Rng>
                >::value
            )>
        // clang-format on
        friend Iter tag_fallback_invoke(inplace_merge_t, Rng&& rng, Iter middle,
            Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(
                pika::traits::is_random_access_iterator<iterator_type>::value,
                "Required at least random access iterator.");
            static_assert(pika::traits::is_random_access_iterator<Iter>::value,
                "Required at least random access iterator.");

            return pika::parallel::detail::inplace_merge<Iter>().call(
                pika::execution::seq, pika::util::begin(rng), middle,
                pika::util::end(rng), PIKA_FORWARD(Comp, comp),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Iter, typename Sent,
            typename Comp = pika::ranges::less,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Comp,
                    pika::parallel::detail::projected<Proj, Iter>,
                    pika::parallel::detail::projected<Proj, Iter>
                >::value
            )>
        // clang-format on
        friend Iter tag_fallback_invoke(inplace_merge_t, Iter first,
            Iter middle, Sent last, Comp&& comp = Comp(), Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_random_access_iterator<Iter>::value,
                "Required at least random access iterator.");

            return pika::parallel::detail::inplace_merge<Iter>().call(
                pika::execution::seq, first, middle, last,
                PIKA_FORWARD(Comp, comp), PIKA_FORWARD(Proj, proj));
        }
    } inplace_merge{};

}}    // namespace pika::ranges
#endif    //DOXYGEN
