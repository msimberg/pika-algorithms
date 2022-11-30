//  Copyright (c) 2007-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/equal.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika { namespace ranges {
    // clang-format off

    /// Returns true if the range [first1, last1) is equal to the range
    /// [first2, last2), and false otherwise.
    ///
    /// \note   Complexity: At most min(last1 - first1, last2 - first2)
    ///         applications of the predicate \a f.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter1       The type of the source iterators used for the
    ///                     first range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent1       The type of the source iterators used for the end of
    ///                     the first range (deduced).
    /// \tparam Iter2       The type of the source iterators used for the
    ///                     second range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent2       The type of the source iterators used for the end of
    ///                     the second range (deduced).
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::equal_to<>
    /// \tparam Proj1       The type of an optional projection function applied
    ///                     to the first range. This
    ///                     defaults to \a parallel::detail::projection_identity
    /// \tparam Proj2       The type of an optional projection function applied
    ///                     to the second range. This
    ///                     defaults to \a parallel::detail::projection_identity
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
    /// \param op           The binary predicate which returns true if the
    ///                     elements should be treated as equal. The signature
    ///                     of the predicate function should be equivalent to
    ///                     the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be such
    ///                     that objects of types \a FwdIter1 and \a FwdIter2 can
    ///                     be dereferenced and then implicitly converted to
    ///                     \a Type1 and \a Type2 respectively
    /// \param proj1        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the
    ///                     first range as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    /// \param proj2        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the
    ///                     second range as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The comparison operations in the parallel \a equal algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a equal algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \note     The two ranges are considered equal if, for every iterator
    ///           i in the range [first1,last1), *i equals *(first2 + (i - first1)).
    ///           This overload of equal uses operator== to determine if two
    ///           elements are equal.
    ///
    /// \returns  The \a equal algorithm returns a \a pika::future<bool> if the
    ///           execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a bool otherwise.
    ///           The \a equal algorithm returns true if the elements in the
    ///           two ranges are equal, otherwise it returns false.
    ///           If the length of the range [first1, last1) does not equal
    ///           the length of the range [first2, last2), it returns false.
    ///
    template <typename ExPolicy, typename Iter1, typename Sent1, typename Iter2,
        typename Sent2, typename Pred = ranges::equal_to,
        typename Proj1 = parallel::detail::projection_identity,
        typename Proj2 = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, bool>::type
    equal(ExPolicy&& policy, Iter1 first1, Sent1 last1, Iter2 first2,
        Sent2 last2, Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
        Proj2&& proj2 = Proj2());

    /// Returns true if the range [first1, last1) is equal to the range
    /// starting at first2, and false otherwise.
    ///
    /// \note   Complexity: At most \a last1 - \a first1 applications of the
    ///         predicate \a f.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng1        The type of the first source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an forward iterator.
    /// \tparam Rng2        The type of the second source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an forward iterator.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::equal_to<>
    /// \tparam Proj1       The type of an optional projection function applied
    ///                     to the first range. This
    ///                     defaults to \a parallel::detail::projection_identity
    /// \tparam Proj2       The type of an optional projection function applied
    ///                     to the second range. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng1         Refers to the first sequence of elements the
    ///                     algorithm will be applied to.
    /// \param rng2         Refers to the second sequence of elements the
    ///                     algorithm will be applied to.
    /// \param op           The binary predicate which returns true if the
    ///                     elements should be treated as equal. The signature
    ///                     of the predicate function should be equivalent to
    ///                     the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be such
    ///                     that objects of types \a FwdIter1 and \a FwdIter2 can
    ///                     be dereferenced and then implicitly converted to
    ///                     \a Type1 and \a Type2 respectively
    /// \param proj1        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the
    ///                     first range as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    /// \param proj2        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the
    ///                     second range as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The comparison operations in the parallel \a equal algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a equal algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \note     The two ranges are considered equal if, for every iterator
    ///           i in the range [first1,last1), *i equals *(first2 + (i - first1)).
    ///           This overload of equal uses operator== to determine if two
    ///           elements are equal.
    ///
    /// \returns  The \a equal algorithm returns a \a pika::future<bool> if the
    ///           execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a bool otherwise.
    ///           The \a equal algorithm returns true if the elements in the
    ///           two ranges are equal, otherwise it returns false.
    ///
    template <typename ExPolicy, typename Rng1, typename Rng2,
        typename Pred = ranges::equal_to,
        typename Proj1 = parallel::detail::projection_identity,
        typename Proj2 = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, bool>::type
    equal(ExPolicy&& policy, Rng1&& rng1, Rng2&& rng2, Pred&& op = Pred(),
        Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2());

    // clang-format on
}}    // namespace pika::ranges

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_range.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/algorithms/traits/projected_range.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>
#include <pika/parallel/algorithms/equal.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>
#include <pika/parallel/util/invoke_projected.hpp>
#include <pika/parallel/util/projection_identity.hpp>

#include <type_traits>
#include <utility>

namespace pika::ranges {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::equal
    inline constexpr struct equal_t final
      : pika::detail::tag_parallel_algorithm<equal_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename Iter1, typename Sent1,
            typename Iter2, typename Sent2, typename Pred = equal_to,
            typename Proj1 = pika::parallel::detail::projection_identity,
            typename Proj2 = pika::parallel::detail::projection_identity,
        PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_sentinel_for<Sent1, Iter1>::value &&
                pika::traits::is_sentinel_for<Sent2, Iter2>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Pred,
                    pika::parallel::detail::projected<Proj1, Iter1>,
                    pika::parallel::detail::projected<Proj2, Iter2>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            bool>::type
        tag_fallback_invoke(equal_t, ExPolicy&& policy, Iter1 first1,
            Sent1 last1, Iter2 first2, Sent2 last2, Pred&& op = Pred(),
            Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2())
        {
            static_assert((pika::traits::is_forward_iterator<Iter1>::value),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_forward_iterator<Iter2>::value),
                "Requires at least forward iterator.");

            return pika::parallel::detail::equal_binary().call(
                PIKA_FORWARD(ExPolicy, policy), first1, last1, first2, last2,
                PIKA_FORWARD(Pred, op), PIKA_FORWARD(Proj1, proj1),
                PIKA_FORWARD(Proj2, proj2));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng1, typename Rng2,
            typename Pred = equal_to,
            typename Proj1 = pika::parallel::detail::projection_identity,
            typename Proj2 = pika::parallel::detail::projection_identity,
        PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::parallel::detail::is_projected_range<Proj1, Rng1>::value &&
                pika::parallel::detail::is_projected_range<Proj2, Rng2>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy, Pred,
                    pika::parallel::detail::projected<Proj1,
                        typename pika::traits::range_traits<Rng1>::iterator_type>,
                    pika::parallel::detail::projected<Proj2,
                        typename pika::traits::range_traits<Rng2>::iterator_type>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            bool>::type
        tag_fallback_invoke(equal_t, ExPolicy&& policy, Rng1&& rng1,
            Rng2&& rng2, Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
            Proj2&& proj2 = Proj2())
        {
            static_assert(
                (pika::traits::is_forward_iterator<typename pika::traits::
                        range_traits<Rng1>::iterator_type>::value),
                "Requires at least forward iterator.");
            static_assert(
                (pika::traits::is_forward_iterator<typename pika::traits::
                        range_traits<Rng2>::iterator_type>::value),
                "Requires at least forward iterator.");

            return pika::parallel::detail::equal_binary().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng1),
                pika::util::end(rng1), pika::util::begin(rng2),
                pika::util::end(rng2), PIKA_FORWARD(Pred, op),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2));
        }

        // clang-format off
        template < typename Iter1, typename Sent1,
            typename Iter2, typename Sent2, typename Pred = equal_to,
            typename Proj1 = pika::parallel::detail::projection_identity,
            typename Proj2 = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_sentinel_for<Sent1, Iter1>::value &&
                pika::traits::is_sentinel_for<Sent2, Iter2>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Pred,
                    pika::parallel::detail::projected<Proj1, Iter1>,
                    pika::parallel::detail::projected<Proj2, Iter2>
                >::value
            )>
        // clang-format on
        friend bool tag_fallback_invoke(equal_t, Iter1 first1, Sent1 last1,
            Iter2 first2, Sent2 last2, Pred&& op = Pred(),
            Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2())
        {
            static_assert((pika::traits::is_forward_iterator<Iter1>::value),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_forward_iterator<Iter2>::value),
                "Requires at least forward iterator.");

            return pika::parallel::detail::equal_binary().call(
                pika::execution::seq, first1, last1, first2, last2,
                PIKA_FORWARD(Pred, op), PIKA_FORWARD(Proj1, proj1),
                PIKA_FORWARD(Proj2, proj2));
        }

        // clang-format off
        template <typename Rng1, typename Rng2, typename Pred = equal_to,
            typename Proj1 = pika::parallel::detail::projection_identity,
            typename Proj2 = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::parallel::detail::is_projected_range<Proj1, Rng1>::value &&
                pika::parallel::detail::is_projected_range<Proj2, Rng2>::value &&
                pika::parallel::detail::is_indirect_callable<
                    pika::execution::sequenced_policy, Pred,
                    pika::parallel::detail::projected<Proj1,
                        typename pika::traits::range_traits<Rng1>::iterator_type>,
                    pika::parallel::detail::projected<Proj2,
                        typename pika::traits::range_traits<Rng2>::iterator_type>
                >::value
            )>
        // clang-format on
        friend bool tag_fallback_invoke(equal_t, Rng1&& rng1, Rng2&& rng2,
            Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
            Proj2&& proj2 = Proj2())
        {
            static_assert(
                (pika::traits::is_forward_iterator<typename pika::traits::
                        range_traits<Rng1>::iterator_type>::value),
                "Requires at least forward iterator.");
            static_assert(
                (pika::traits::is_forward_iterator<typename pika::traits::
                        range_traits<Rng2>::iterator_type>::value),
                "Requires at least forward iterator.");

            return pika::parallel::detail::equal_binary().call(
                pika::execution::seq, pika::util::begin(rng1),
                pika::util::end(rng1), pika::util::begin(rng2),
                pika::util::end(rng2), PIKA_FORWARD(Pred, op),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2));
        }

    } equal{};
}    // namespace pika::ranges

#endif    // DOXYGEN
