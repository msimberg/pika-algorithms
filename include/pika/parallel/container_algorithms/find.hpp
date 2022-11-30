//  Copyright (c) 2018 Bruno Pitrus
//  Copyright (c) 2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/container_algorithms/find.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika { namespace ranges {
    // clang-format off

    /// Returns the first element in the range [first, last) that is equal
    /// to value
    ///
    /// \note   Complexity: At most last - first
    ///         applications of the operator==().
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the begin source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent        The type of the end source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter.
    /// \tparam T           The type of the value to find (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     of the first range the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements of
    ///                     the first range the algorithm will be applied to.
    /// \param val          the value to compare the elements to
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The comparison operations in the parallel \a find algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a find algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a find algorithm returns a \a pika::future<FwdIter> if the
    ///           execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter otherwise.
    ///           The \a find algorithm returns the first element in the range
    ///           [first,last) that is equal to \a val.
    ///           If no such element in the range of [first,last) is equal to
    ///           \a val, then the algorithm returns \a last.
    ///
    template <typename ExPolicy, typename Iter, typename Sent, typename T,
        typename Proj = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, Iter>::type
    find(ExPolicy&& policy, Iter first, Sent last, T const& val,
        Proj&& proj = Proj());

    /// Returns the first element in the range [first, last) that is equal
    /// to value
    ///
    /// \note   Complexity: At most last - first
    ///         applications of the operator==().
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an input iterator.
    /// \tparam T           The type of the value to find (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param val          the value to compare the elements to
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The comparison operations in the parallel \a find algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a find algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a find algorithm returns a \a pika::future<FwdIter> if the
    ///           execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter otherwise.
    ///           The \a find algorithm returns the first element in the range
    ///           [first,last) that is equal to \a val.
    ///           If no such element in the range of [first,last) is equal to
    ///           \a val, then the algorithm returns \a last.
    ///
    template <typename ExPolicy, typename Rng, typename T,
        typename Proj = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, Iter>::type
    find(ExPolicy&& policy, Rng&& rng, T const& val, Proj&& proj = Proj());

    /// Returns the last subsequence of elements \a [first2, last2) found in
    /// the range \a [first1, last1) using the given predicate \a f to
    /// compare elements.
    ///
    /// \note   Complexity: at most S*(N-S+1) comparisons where
    ///         \a S = distance(first2, last2) and
    ///         \a N = distance(first1, last1).
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter1       The type of the begin source iterators for the first
    ///                     sequence used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent1       The type of the end source iterators for the first
    ///                     sequence used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter1.
    /// \tparam Iter2       The type of the begin source iterators for the second
    ///                     sequence used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent2       The type of the end source iterators for the second
    ///                     sequence used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter2.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a replace requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::equal_to<>
    /// \tparam Proj1       The type of an optional projection function applied
    ///                     to the first sequence. This
    ///                     defaults to \a parallel::detail::projection_identity
    /// \tparam Proj2       The type of an optional projection function applied
    ///                     to the second sequence. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first1       Refers to the beginning of the first sequence of
    ///                     elements the algorithm will be applied to.
    /// \param last1        Refers to the end of the first sequence of elements
    ///                     the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the second sequence of
    ///                     elements the algorithm will be applied to.
    /// \param last2        Refers to the end of the second sequence of elements
    ///                     the algorithm will be applied to.
    /// \param op           The binary predicate which returns \a true
    ///                     if the elements should be treated as equal. The signature
    ///                     should be equivalent to the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be such
    ///                     that objects of types \a iterator_t<Rng> and \a iterator_t<Rng2>
    ///                     can be dereferenced and then implicitly converted
    ///                     to \a Type1 and \a Type2 respectively.
    /// \param proj1        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the first
    ///                     range of type dereferenced \a iterator_t<Rng1>
    ///                     as a projection operation before the function \a op
    ///                     is invoked.
    /// \param proj2        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the second
    ///                     range of type dereferenced \a iterator_t<Rng2>
    ///                     as a projection operation before the function \a op
    ///                     is invoked.
    ///
    /// The comparison operations in the parallel \a find_end algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a find_end algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a find_end algorithm returns a \a pika::future<iterator_t<Rng> >
    ///           if the execution policy is of type \a sequenced_task_policy
    ///           or \a parallel_task_policy and returns \a iterator_t<Rng> otherwise.
    ///           The \a find_end algorithm returns an iterator to the beginning of
    ///           the last subsequence \a rng2 in range \a rng.
    ///           If the length of the subsequence \a rng2 is greater
    ///           than the length of the range \a rng, \a end(rng) is returned.
    ///           Additionally if the size of the subsequence is empty or no subsequence
    ///           is found, \a end(rng) is also returned.
    ///
    /// This overload of \a find_end is available if the user decides to provide the
    /// algorithm their own predicate \a op.
    ///
    template <typename ExPolicy, typename Iter1, typename Sent1, typename Iter2,
        typename Sent2, typename Pred = ranges::equal_to,
        typename Proj1 = parallel::detail::projection_identity,
        typename Proj2 = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename pika::traits::range_iterator<Rng1>::type>::type
    find_end(ExPolicy&& policy, Iter1 first1, Sent1 last1, Iter2 first2,
        Sent2 last2, Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
        Proj2&& proj2 = Proj2());

    /// Returns the last subsequence of elements \a rng2 found in the range
    /// \a rng using the given predicate \a f to compare elements.
    ///
    /// \note   Complexity: at most S*(N-S+1) comparisons where
    ///         \a S = distance(begin(rng2), end(rng2)) and
    ///         \a N = distance(begin(rng), end(rng)).
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng1        The type of the first source range (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Rng2        The type of the second source range (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a replace requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::equal_to<>
    /// \tparam Proj1       The type of an optional projection function applied
    ///                     to the first sequence. This
    ///                     defaults to \a parallel::detail::projection_identity
    /// \tparam Proj2       The type of an optional projection function applied
    ///                     to the second sequence. This
    ///                     defaults to \a parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the first sequence of elements
    ///                     the algorithm will be applied to.
    /// \param rng2         Refers to the second sequence of elements
    ///                     the algorithm will be applied to.
    /// \param op           The binary predicate which returns \a true
    ///                     if the elements should be treated as equal. The signature
    ///                     should be equivalent to the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be such
    ///                     that objects of types \a iterator_t<Rng> and \a iterator_t<Rng2>
    ///                     can be dereferenced and then implicitly converted
    ///                     to \a Type1 and \a Type2 respectively.
    /// \param proj1        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the first
    ///                     range of type dereferenced \a iterator_t<Rng1>
    ///                     as a projection operation before the function \a op
    ///                     is invoked.
    /// \param proj2        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of the second
    ///                     range of type dereferenced \a iterator_t<Rng2>
    ///                     as a projection operation before the function \a op
    ///                     is invoked.
    ///
    /// The comparison operations in the parallel \a find_end algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a find_end algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a find_end algorithm returns a \a pika::future<iterator_t<Rng> >
    ///           if the execution policy is of type \a sequenced_task_policy
    ///           or \a parallel_task_policy and returns \a iterator_t<Rng> otherwise.
    ///           The \a find_end algorithm returns an iterator to the beginning of
    ///           the last subsequence \a rng2 in range \a rng.
    ///           If the length of the subsequence \a rng2 is greater
    ///           than the length of the range \a rng, \a end(rng) is returned.
    ///           Additionally if the size of the subsequence is empty or no subsequence
    ///           is found, \a end(rng) is also returned.
    ///
    /// This overload of \a find_end is available if the user decides to provide the
    /// algorithm their own predicate \a op.
    ///
    template <typename ExPolicy, typename Rng1, typename Rng2,
        typename Pred = ranges::equal_to,
        typename Proj1 = parallel::detail::projection_identity,
        typename Proj2 = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename pika::traits::range_iterator<Rng1>::type>::type
    find_end(ExPolicy&& policy, Rng1&& rng, Rng2&& rng2, Pred&& op = Pred(),
        Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2());

    /// Searches the range \a [first1, last1) for any elements in the
    /// range \a [first2, last2).
    /// Uses binary predicate \a p to compare elements
    ///
    /// \note   Complexity: at most (S*N) comparisons where
    ///         \a S = distance(first2, last2) and
    ///         \a N = distance(first1, last1).
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter1       The type of the begin source iterators for the first
    ///                     sequence used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent1       The type of the end source iterators for the first
    ///                     sequence used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter1.
    /// \tparam Iter2       The type of the begin source iterators for the second
    ///                     sequence used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Sent2       The type of the end source iterators for the second
    ///                     sequence used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     sentinel for Iter2.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a replace requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::equal_to<>
    /// \tparam Proj1       The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity and is
    ///                     applied to the elements in \a rng1.
    /// \tparam Proj2       The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity and is
    ///                     applied to the elements in \a rng2.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first1       Refers to the beginning of the first sequence of
    ///                     elements the algorithm will be applied to.
    /// \param last1        Refers to the end of the first sequence of elements
    ///                     the algorithm will be applied to.
    /// \param first2       Refers to the beginning of the second sequence of
    ///                     elements the algorithm will be applied to.
    /// \param last2        Refers to the end of the second sequence of elements
    ///                     the algorithm will be applied to.
    /// \param op           The binary predicate which returns \a true
    ///                     if the elements should be treated as equal. The signature
    ///                     should be equivalent to the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be such
    ///                     that objects of types \a iterator_t<Rng1>
    ///                     and \a iterator_t<Rng2>
    ///                     can be dereferenced and then implicitly converted
    ///                     to \a Type1 and \a Type2 respectively.
    /// \param proj1        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of type
    ///                     dereferenced \a iterator_t<Rng1> before the function
    ///                     \a op is invoked.
    /// \param proj2        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of type
    ///                     dereferenced \a iterator_t<Rng2> before the function
    ///                     \a op is invoked.
    ///
    /// The comparison operations in the parallel \a find_first_of algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a find_first_of algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a find_end algorithm returns a \a pika::future<iterator_t<Rng1> >
    ///           if the execution policy is of type \a sequenced_task_policy
    ///           or \a parallel_task_policy and returns \a iterator_t<Rng1> otherwise.
    ///           The \a find_first_of algorithm returns an iterator to the first element
    ///           in the range \a rng1 that is equal to an element from the range
    ///           \a rng2.
    ///           If the length of the subsequence \a rng2 is
    ///           greater than the length of the range \a rng1,
    ///           \a end(rng1) is returned.
    ///           Additionally if the size of the subsequence is empty or no subsequence
    ///           is found, \a end(rng1) is also returned.
    ///
    /// This overload of \a find_first_of is available if the user decides to provide the
    /// algorithm their own predicate \a op.
    ///
    template <typename ExPolicy, typename Iter1, typename Sent1, typename Iter2,
        typename Sent2, typename Pred = ranges::equal_to,
        typename Proj1 = parallel::detail::projection_identity,
        typename Proj2 = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename pika::traits::range_iterator<Rng1>::type>::type
    find_first_of(ExPolicy&& policy, Iter1 first1, Sent1 last1, Iter2 first2,
        Sent2 last2, Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
        Proj2&& proj2 = Proj2());

    /// Searches the range \a rng1 for any elements in the range \a rng2.
    /// Uses binary predicate \a p to compare elements
    ///
    /// \note   Complexity: at most (S*N) comparisons where
    ///         \a S = distance(begin(rng2), end(rng2)) and
    ///         \a N = distance(begin(rng1), end(rng1)).
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng1        The type of the first source range (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Rng2        The type of the second source range (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Pred        The type of an optional function/function object to use.
    ///                     Unlike its sequential form, the parallel
    ///                     overload of \a replace requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible. This defaults
    ///                     to std::equal_to<>
    /// \tparam Proj1       The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity and is
    ///                     applied to the elements in \a rng1.
    /// \tparam Proj2       The type of an optional projection function. This
    ///                     defaults to \a parallel::detail::projection_identity and is
    ///                     applied to the elements in \a rng2.
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng1         Refers to the first sequence of elements
    ///                     the algorithm will be applied to.
    /// \param rng2         Refers to the second sequence of elements
    ///                     the algorithm will be applied to.
    /// \param op           The binary predicate which returns \a true
    ///                     if the elements should be treated as equal. The signature
    ///                     should be equivalent to the following:
    ///                     \code
    ///                     bool pred(const Type1 &a, const Type2 &b);
    ///                     \endcode \n
    ///                     The signature does not need to have const &, but
    ///                     the function must not modify the objects passed to
    ///                     it. The types \a Type1 and \a Type2 must be such
    ///                     that objects of types \a iterator_t<Rng1>
    ///                     and \a iterator_t<Rng2>
    ///                     can be dereferenced and then implicitly converted
    ///                     to \a Type1 and \a Type2 respectively.
    /// \param proj1        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of type
    ///                     dereferenced \a iterator_t<Rng1> before the function
    ///                     \a op is invoked.
    /// \param proj2        Specifies the function (or function object) which
    ///                     will be invoked for each of the elements of type
    ///                     dereferenced \a iterator_t<Rng2> before the function
    ///                     \a op is invoked.
    ///
    /// The comparison operations in the parallel \a find_first_of algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The comparison operations in the parallel \a find_first_of algorithm invoked
    /// with an execution policy object of type \a parallel_policy
    /// or \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a find_end algorithm returns a \a pika::future<iterator_t<Rng1> >
    ///           if the execution policy is of type \a sequenced_task_policy
    ///           or \a parallel_task_policy and returns \a iterator_t<Rng1> otherwise.
    ///           The \a find_first_of algorithm returns an iterator to the first element
    ///           in the range \a rng1 that is equal to an element from the range
    ///           \a rng2.
    ///           If the length of the subsequence \a rng2 is
    ///           greater than the length of the range \a rng1,
    ///           \a end(rng1) is returned.
    ///           Additionally if the size of the subsequence is empty or no subsequence
    ///           is found, \a end(rng1) is also returned.
    ///
    /// This overload of \a find_first_of is available if the user decides to provide the
    /// algorithm their own predicate \a op.
    ///
    template <typename ExPolicy, typename Rng1, typename Rng2,
        typename Pred = ranges::equal_to,
        typename Proj1 = parallel::detail::projection_identity,
        typename Proj2 = parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename pika::traits::range_iterator<Rng1>::type>::type
    find_first_of(ExPolicy&& policy, Rng1&& rng1, Rng2&& rng2,
        Pred&& op = Pred(), Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2());

    // clang-format on
}}    // namespace pika::ranges

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/execution/traits/is_execution_policy.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_range.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/algorithms/traits/projected_range.hpp>
#include <pika/parallel/algorithms/find.hpp>
#include <pika/parallel/util/projection_identity.hpp>

#include <type_traits>
#include <utility>

namespace pika::ranges {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::find
    inline constexpr struct find_t final
      : pika::detail::tag_parallel_algorithm<find_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T = typename pika::parallel::detail::projected<Iter,
                Proj>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(find_t, ExPolicy&& policy, Iter first, Sent last,
            T const& val, Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_forward_iterator<Iter>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::find<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last, val,
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T = typename pika::parallel::detail::projected<
                pika::traits::range_iterator_t<Rng>, Proj>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng>::type>::type
        tag_fallback_invoke(find_t, ExPolicy&& policy, Rng&& rng, T const& val,
            Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(
                pika::traits::is_forward_iterator<iterator_type>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::find<iterator_type>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                pika::util::end(rng), val, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Iter, typename Sent,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T = typename pika::parallel::detail::projected<Iter,
                Proj>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value
            )>
        // clang-format on
        friend Iter tag_fallback_invoke(
            find_t, Iter first, Sent last, T const& val, Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_input_iterator<Iter>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::find<Iter>().call(
                pika::execution::seq, first, last, val,
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T = typename pika::parallel::detail::projected<
                pika::traits::range_iterator_t<Rng>, Proj>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value
            )>
        // clang-format on
        friend typename pika::traits::range_iterator<Rng>::type
        tag_fallback_invoke(
            find_t, Rng&& rng, T const& val, Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(pika::traits::is_input_iterator<iterator_type>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::find<iterator_type>().call(
                pika::execution::seq, pika::util::begin(rng),
                pika::util::end(rng), val, PIKA_FORWARD(Proj, proj));
        }
    } find{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::find_if
    inline constexpr struct find_if_t final
      : pika::detail::tag_parallel_algorithm<find_if_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<Iter>::value_type
                >
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(find_if_t, ExPolicy&& policy, Iter first, Sent last,
            Pred&& pred, Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_forward_iterator<Iter>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::find_if<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last,
                PIKA_FORWARD(Pred, pred), PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<
                        typename pika::traits::range_iterator<Rng>::type
                    >::value_type
                >
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng>::type>::type
        tag_fallback_invoke(find_if_t, ExPolicy&& policy, Rng&& rng,
            Pred&& pred, Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(
                pika::traits::is_forward_iterator<iterator_type>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::find_if<iterator_type>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                pika::util::end(rng), PIKA_FORWARD(Pred, pred),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Iter, typename Sent, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<Iter>::value_type
                >
            )>
        // clang-format on
        friend Iter tag_fallback_invoke(
            find_if_t, Iter first, Sent last, Pred&& pred, Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_input_iterator<Iter>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::find_if<Iter>().call(
                pika::execution::seq, first, last, PIKA_FORWARD(Pred, pred),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<
                        typename pika::traits::range_iterator<Rng>::type
                    >::value_type
                >
            )>
        // clang-format on
        friend typename pika::traits::range_iterator<Rng>::type
        tag_fallback_invoke(
            find_if_t, Rng&& rng, Pred&& pred, Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(pika::traits::is_input_iterator<iterator_type>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::find_if<iterator_type>().call(
                pika::execution::seq, pika::util::begin(rng),
                pika::util::end(rng), PIKA_FORWARD(Pred, pred),
                PIKA_FORWARD(Proj, proj));
        }
    } find_if{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::find_if_not
    inline constexpr struct find_if_not_t final
      : pika::detail::tag_parallel_algorithm<find_if_not_t>
    {
    private:
        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<Iter>::value_type
                >
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(find_if_not_t, ExPolicy&& policy, Iter first,
            Sent last, Pred&& pred, Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_forward_iterator<Iter>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::find_if_not<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, last,
                PIKA_FORWARD(Pred, pred), PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<
                        typename pika::traits::range_iterator<Rng>::type
                    >::value_type
                >
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng>::type>::type
        tag_fallback_invoke(find_if_not_t, ExPolicy&& policy, Rng&& rng,
            Pred&& pred, Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(
                pika::traits::is_forward_iterator<iterator_type>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::find_if_not<iterator_type>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                pika::util::end(rng), PIKA_FORWARD(Pred, pred),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Iter, typename Sent, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<Iter>::value_type
                >
            )>
        // clang-format on
        friend Iter tag_fallback_invoke(find_if_not_t, Iter first, Sent last,
            Pred&& pred, Proj&& proj = Proj())
        {
            static_assert(pika::traits::is_input_iterator<Iter>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::find_if_not<Iter>().call(
                pika::execution::seq, first, last, PIKA_FORWARD(Pred, pred),
                PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<
                        typename pika::traits::range_iterator<Rng>::type
                    >::value_type
                >
            )>
        // clang-format on
        friend typename pika::traits::range_iterator<Rng>::type
        tag_fallback_invoke(
            find_if_not_t, Rng&& rng, Pred&& pred, Proj&& proj = Proj())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng>::type;

            static_assert(pika::traits::is_input_iterator<iterator_type>::value,
                "Requires at least input iterator.");

            return pika::parallel::detail::find_if_not<iterator_type>().call(
                pika::execution::seq, pika::util::begin(rng),
                pika::util::end(rng), PIKA_FORWARD(Pred, pred),
                PIKA_FORWARD(Proj, proj));
        }
    } find_if_not{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::find_end
    inline constexpr struct find_end_t final
      : pika::detail::tag_parallel_algorithm<find_end_t>
    {
    private:
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
                    pika::parallel::detail::projected_range<Proj1, Rng1>,
                    pika::parallel::detail::projected_range<Proj2, Rng2>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng1>::type>::type
        tag_fallback_invoke(find_end_t, ExPolicy&& policy, Rng1&& rng1,
            Rng2&& rng2, Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
            Proj2&& proj2 = Proj2())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng1>::type;

            static_assert(
                (pika::traits::is_forward_iterator<iterator_type>::value),
                "Requires at least forward iterator.");
            static_assert(
                (pika::traits::is_forward_iterator<
                    typename pika::traits::range_iterator<Rng2>::type>::value),
                "Requires at least forward iterator.");

            return pika::parallel::detail::find_end<iterator_type>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng1),
                pika::util::end(rng1), pika::util::begin(rng2),
                pika::util::end(rng2), PIKA_FORWARD(Pred, op),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2));
        }

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
            Iter1>::type
        tag_fallback_invoke(find_end_t, ExPolicy&& policy, Iter1 first1,
            Sent1 last1, Iter2 first2, Sent2 last2, Pred&& op = Pred(),
            Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2())
        {
            static_assert((pika::traits::is_forward_iterator<Iter1>::value),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_forward_iterator<Iter2>::value),
                "Requires at least forward iterator.");

            return pika::parallel::detail::find_end<Iter1>().call(
                PIKA_FORWARD(ExPolicy, policy), first1, last1, first2, last2,
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
                    pika::parallel::detail::projected_range<Proj1, Rng1>,
                    pika::parallel::detail::projected_range<Proj2, Rng2>
                >::value
            )>
        // clang-format on
        friend typename pika::traits::range_iterator<Rng1>::type
        tag_fallback_invoke(find_end_t, Rng1&& rng1, Rng2&& rng2,
            Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
            Proj2&& proj2 = Proj2())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng1>::type;

            static_assert(
                (pika::traits::is_forward_iterator<iterator_type>::value),
                "Requires at least forward iterator.");
            static_assert(
                (pika::traits::is_forward_iterator<
                    typename pika::traits::range_iterator<Rng2>::type>::value),
                "Requires at least forward iterator.");

            return pika::parallel::detail::find_end<iterator_type>().call(
                pika::execution::seq, pika::util::begin(rng1),
                pika::util::end(rng1), pika::util::begin(rng2),
                pika::util::end(rng2), PIKA_FORWARD(Pred, op),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2));
        }

        // clang-format off
        template <typename Iter1, typename Sent1,
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
        friend Iter1 tag_fallback_invoke(find_end_t, Iter1 first1, Sent1 last1,
            Iter2 first2, Sent2 last2, Pred&& op = Pred(),
            Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2())
        {
            static_assert((pika::traits::is_forward_iterator<Iter1>::value),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_forward_iterator<Iter2>::value),
                "Requires at least forward iterator.");

            return pika::parallel::detail::find_end<Iter1>().call(
                pika::execution::seq, first1, last1, first2, last2,
                PIKA_FORWARD(Pred, op), PIKA_FORWARD(Proj1, proj1),
                PIKA_FORWARD(Proj2, proj2));
        }
    } find_end{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::find_first_of
    inline constexpr struct find_first_of_t final
      : pika::detail::tag_parallel_algorithm<find_first_of_t>
    {
    private:
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
                    pika::parallel::detail::projected_range<Proj1, Rng1>,
                    pika::parallel::detail::projected_range<Proj2, Rng2>
                >::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng1>::type>::type
        tag_fallback_invoke(find_first_of_t, ExPolicy&& policy, Rng1&& rng1,
            Rng2&& rng2, Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
            Proj2&& proj2 = Proj2())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng1>::type;

            static_assert(
                (pika::traits::is_forward_iterator<iterator_type>::value),
                "Requires at least forward iterator.");
            static_assert(
                (pika::traits::is_forward_iterator<
                    typename pika::traits::range_iterator<Rng2>::type>::value),
                "Subsequence requires at least forward iterator.");

            return pika::parallel::detail::find_first_of<iterator_type>().call(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng1),
                pika::util::end(rng1), pika::util::begin(rng2),
                pika::util::end(rng2), PIKA_FORWARD(Pred, op),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2));
        }

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
            Iter1>::type
        tag_fallback_invoke(find_first_of_t, ExPolicy&& policy, Iter1 first1,
            Sent1 last1, Iter2 first2, Sent2 last2, Pred&& op = Pred(),
            Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2())
        {
            static_assert((pika::traits::is_forward_iterator<Iter1>::value),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_forward_iterator<Iter2>::value),
                "Subsequence requires at least forward iterator.");

            return pika::parallel::detail::find_first_of<Iter1>().call(
                PIKA_FORWARD(ExPolicy, policy), first1, last1, first2, last2,
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
                    pika::parallel::detail::projected_range<Proj1, Rng1>,
                    pika::parallel::detail::projected_range<Proj2, Rng2>
                >::value
            )>
        // clang-format on
        friend typename pika::traits::range_iterator<Rng1>::type
        tag_fallback_invoke(find_first_of_t, Rng1&& rng1, Rng2&& rng2,
            Pred&& op = Pred(), Proj1&& proj1 = Proj1(),
            Proj2&& proj2 = Proj2())
        {
            using iterator_type =
                typename pika::traits::range_iterator<Rng1>::type;

            static_assert(
                (pika::traits::is_forward_iterator<iterator_type>::value),
                "Requires at least forward iterator.");
            static_assert(
                (pika::traits::is_forward_iterator<
                    typename pika::traits::range_iterator<Rng2>::type>::value),
                "Subsequence requires at least forward iterator.");

            return pika::parallel::detail::find_first_of<iterator_type>().call(
                pika::execution::seq, pika::util::begin(rng1),
                pika::util::end(rng1), pika::util::begin(rng2),
                pika::util::end(rng2), PIKA_FORWARD(Pred, op),
                PIKA_FORWARD(Proj1, proj1), PIKA_FORWARD(Proj2, proj2));
        }

        // clang-format off
        template <typename Iter1, typename Sent1, typename Iter2, typename Sent2,
            typename Pred = equal_to,
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
        friend Iter1 tag_fallback_invoke(find_first_of_t, Iter1 first1,
            Sent1 last1, Iter2 first2, Sent2 last2, Pred&& op = Pred(),
            Proj1&& proj1 = Proj1(), Proj2&& proj2 = Proj2())
        {
            static_assert((pika::traits::is_forward_iterator<Iter1>::value),
                "Requires at least forward iterator.");
            static_assert((pika::traits::is_forward_iterator<Iter2>::value),
                "Subsequence requires at least forward iterator.");

            return pika::parallel::detail::find_first_of<Iter1>().call(
                pika::execution::seq, first1, last1, first2, last2,
                PIKA_FORWARD(Pred, op), PIKA_FORWARD(Proj1, proj1),
                PIKA_FORWARD(Proj2, proj2));
        }

    } find_first_of{};
}    // namespace pika::ranges

#endif    // DOXYGEN
