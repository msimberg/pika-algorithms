//  Copyright (c) 2015 Hartmut Kaiser
//  Copyright (c) 2021 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/container_algorithms/replace.hpp

#pragma once

#if defined(DOXYGEN)

namespace pika { namespace ranges {

    ///////////////////////////////////////////////////////////////////////////
    /// Replaces all elements satisfying specific criteria with \a new_value
    /// in the range [first, last).
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          [first,last) with new_value, when the following corresponding
    ///          conditions hold: INVOKE(proj, *i) == old_value
    ///
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of an input iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    /// \tparam T1          The type of the old value to replace (deduced).
    /// \tparam T2          The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a replace algorithm returns an \a Iter.
    ///
    template <typename Iter, typename Sent, typename T1, typename T2,
        typename Proj = pika::parallel::detail::projection_identity>
    Iter replace(Iter first, Sent sent, T1 const& old_value,
        T2 const& new_value, Proj&& proj = Proj());

    /// Replaces all elements satisfying specific criteria with \a new_value
    /// in the range \a rng.
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         assignments.
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          rng with new_value, when the following corresponding
    ///          conditions hold: INVOKE(proj, *i) == old_value
    ///
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam T1          The type of the old value to replace (deduced).
    /// \tparam T2          The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a replace algorithm returns an
    ///           \a pika::traits::range_iterator<Rng>::type.
    ///
    template <typename Rng, typename T1, typename T2,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::traits::range_iterator<Rng>::type replace(Rng&& rng,
        T1 const& old_value, T2 const& new_value, Proj&& proj = Proj());

    /// Replaces all elements satisfying specific criteria with \a new_value
    /// in the range [first, last).
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          [first,last) with new_value, when the following corresponding
    ///          conditions hold: INVOKE(proj, *i) == old_value
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    /// \tparam T1          The type of the old value to replace (deduced).
    /// \tparam T2          The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace algorithm invoked with an
    /// execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace algorithm returns a \a pika::future<Iter> if
    ///           the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a Iter otherwise.
    ///
    template <typename ExPolicy, typename Iter, typename Sent, typename T1,
        typename T2,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, Iter>::type
    replace(ExPolicy&& policy, Iter first, Sent sent, T1 const& old_value,
        T2 const& new_value, Proj&& proj = Proj());

    /// Replaces all elements satisfying specific criteria with \a new_value
    /// in the range \a rng.
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         assignments.
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          rng with new_value, when the following corresponding
    ///          conditions hold: INVOKE(proj, *i) == old_value
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam T1          The type of the old value to replace (deduced).
    /// \tparam T2          The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    /// The assignments in the parallel \a replace algorithm invoked with an
    /// execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace algorithm returns an
    ///           \a pika::future<pika::traits::range_iterator<Rng>::type> if
    ///           the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a pika::traits::range_iterator<Rng>::type otherwise.
    ///
    template <typename ExPolicy, typename Rng, typename T1, typename T2,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename pika::traits::range_iterator<Rng>::type>::type
    replace(ExPolicy&& policy, Rng&& rng, T1 const& old_value,
        T2 const& new_value, Proj&& proj = Proj());

    ///////////////////////////////////////////////////////////////////////////
    /// Replaces all elements satisfying specific criteria (for which predicate
    /// \a f returns true) with \a new_value in the range [first, sent).
    ///
    /// \note   Complexity: Performs exactly \a sent - \a first applications of
    ///         the predicate.
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          [first, sent) with new_value, when the following corresponding
    ///          conditions hold: INVOKE(f, INVOKE(proj, *it)) != false
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate which returns \a true for the
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a Iter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_if algorithm
    /// execute in sequential order in the calling thread.
    ///
    ///
    /// \returns  The \a replace_if algorithm returns an \a Iter
    ///           It returns \a last.
    ///
    template <typename Iter, typename Sent, typename Pred, typename T,
        typename Proj = pika::parallel::detail::projection_identity>
    Iter replace_if(Iter first, Sent sent, Pred&& pred, T const& new_value,
        Proj&& proj = Proj());

    /// Replaces all elements satisfying specific criteria (for which predicate
    /// \a pred returns true) with \a new_value in the range rng.
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         applications of the predicate.
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          rng with new_value, when the following corresponding
    ///          conditions hold: INVOKE(f, INVOKE(proj, *it)) != false
    ///
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by rng.This is an
    ///                     unary predicate which returns \a true for the
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace algorithm invoked with an
    /// execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace_if algorithm returns an \a pika::traits::range_iterator<Rng>::type
    ///           It returns \a last.
    ///
    template <typename Rng, typename Pred, typename T,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::traits::range_iterator<Rng>::type replace_if(
        Rng&& rng, Pred&& pred, T const& new_value, Proj&& proj = Proj());

    /// Replaces all elements satisfying specific criteria (for which predicate
    /// \a pred returns true) with \a new_value in the range rng.
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         applications of the predicate.
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          rng with new_value, when the following corresponding
    ///          conditions hold: INVOKE(f, INVOKE(proj, *it)) != false
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate which returns \a true for the
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_if algorithm invoked with an
    /// execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace_if algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace_if algorithm returns a \a pika::future<Iter>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy.
    ///           It returns \a last.
    ///
    template <typename ExPolicy, typename Iter, typename Sent, typename Pred,
        typename T, typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy, Iter>::type t
    replace_if(ExPolicy&& policy, Iter first, Sent sent, Pred&& pred,
        T const& new_value, Proj&& proj = Proj());

    /// Replaces all elements satisfying specific criteria (for which predicate
    /// \a pred returns true) with \a new_value in the range rng.
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         applications of the predicate.
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          rng with new_value, when the following corresponding
    ///          conditions hold: INVOKE(f, INVOKE(proj, *it)) != false
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by rng.This is an
    ///                     unary predicate which returns \a true for the
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace algorithm invoked with an
    /// execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace algorithm invoked with
    /// an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace_if algorithm returns a \a
    ///           pika::future<typename pika::traits::range_iterator<Rng>::type>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy.
    ///           It returns \a last.
    ///
    template <typename ExPolicy, typename Rng, typename Pred, typename T,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        typename pika::traits::range_iterator<Rng>::type>::type
    replace_if(ExPolicy&& policy, Rng&& rng, Pred&& pred, T const& new_value,
        Proj&& proj = Proj());

    ///////////////////////////////////////////////////////////////////////////
    /// Copies the all elements from the range [first, sent) to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (sent - first)) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(proj, *(first + (i - result))) == old_value
    ///
    /// \note   Complexity: Performs exactly \a sent - \a first applications of
    ///         the predicate.
    ///
    /// \tparam Iter        The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of an input iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam T1          The type of the old value to replace (deduced).
    /// \tparam T2          The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param sent         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_copy algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a replace_copy algorithm returns an
    ///           \a in_out_result<InIter, OutIter>.
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           \a last and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename Initer, typename Sent, typename OutIter, typename T1,
        typename T2,
        typename Proj = pika::parallel::detail::projection_identity>
    replace_copy_result<InIter, OutIter>
    replace_copy(InIter first, Sent sent, OutIter dest, T1 const& old_value,
        T2 const& new_value, Proj&& proj = Proj());

    /// Copies the all elements from the range rbg to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (util::end(rng) - util::begin(rng))) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(proj, *(first + (i - result))) == old_value
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         applications of the predicate.
    ///
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an input iterator.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam T1          The type of the old value to replace (deduced).
    /// \tparam T2          The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_copy algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a replace_copy algorithm returns an
    ///           \a in_out_result<typename pika::traits::range_iterator<
    ///             Rng>::type, OutIter>.
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           \a last and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename Rng, typename OutIter, typename T1, typename T2,
        typename Proj = pika::parallel::detail::projection_identity>
    replace_copy_result<typename pika::traits::range_iterator<Rng>::type,
        OutIter>
    replace_copy(Rng&& rng, OutIter dest, T1 const& old_value,
        T2 const& new_value, Proj&& proj = Proj());

    /// Copies the all elements from the range [first, sent) to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (sent - first)) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(proj, *(first + (i - result))) == old_value
    ///
    /// \note   Complexity: Performs exactly \a sent - \a first applications of
    ///         the predicate.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of an forward iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for Iter.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam T1          The type of the old value to replace (deduced).
    /// \tparam T2          The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param sent         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_copy algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace_copy algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace_copy algorithm returns a
    ///           \a pika::future<in_out_result<FwdIter1, FwdIter2>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a in_out_result<FwdIter1, FwdIter2>
    ///           otherwise.
    ///           The \a copy algorithm returns the pair of the forward iterator
    ///           \a last and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename Sent,
        typename FwdIter2, typename T1, typename T2,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        replace_copy_result<FwdIter1, FwdIter2>>::type
    replace_copy(ExPolicy&& policy, FwdIter1 first, Sent sent, FwdIter2 dest,
        T1 const& old_value, T2 const& new_value, Proj&& proj = Proj());

    /// Copies the all elements from the range rbg to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (util::end(rng) - util::begin(rng))) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(proj, *(first + (i - result))) == old_value
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         applications of the predicate.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an input iterator.
    /// \tparam FwdIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam T1          The type of the old value to replace (deduced).
    /// \tparam T2          The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_copy algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace_copy algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace_copy algorithm returns a
    ///           \a pika::future<in_out_result<
    ///            typename pika::traits::range_iterator<Rng>::type, FwdIter>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a in_out_result<
    ///            typename pika::traits::range_iterator<Rng>::type, FwdIter>>
    ///           The \a copy algorithm returns the pair of the input iterator
    ///           \a last and the forward iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename Rng, typename FwdIter, typename T1,
        typename T2,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        replace_copy_result<typename pika::traits::range_iterator<Rng>::type,
            FwdIter>>::type
    replace_copy(ExPolicy&& policy, Rng&& rng, FwdIter dest,
        T1 const& old_value, T2 const& new_value, Proj&& proj = Proj());

    ///////////////////////////////////////////////////////////////////////////
    /// Copies the all elements from the range [first, sent) to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (sent - first)) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(f, INVOKE(proj, *(first + (i - result)))) != false
    ///
    /// \note   Complexity: Performs exactly \a sent - \a first applications of
    ///         the predicate.
    ///
    /// \tparam InIter      The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of an input iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for InIter.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param sent         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate which returns \a true for the
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_copy_if algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a replace_copy_if algorithm returns a
    ///           \a in_out_result<InIter, OutIter>.
    ///           The \a replace_copy_if algorithm returns the input iterator
    ///           \a last and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename InIter, typename Sent, typename OutIter, typename Pred,
        typename T, typename Proj = pika::parallel::detail::projection_identity>
    replace_copy_if_result<InIter, OutIter>
    replace_copy_if(InIter first, Sent sent, OutIter dest, Pred&& pred,
        T const& new_value, Proj&& proj = Proj());

    /// Copies the all elements from the range rng to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (util::end(rng) - util::begin(rng))) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(f, INVOKE(proj, *(first + (i - result)))) != false
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         applications of the predicate.
    ///
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an input iterator.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate which returns \a true for the
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_copy_if algorithm
    /// execute in sequential order in the calling thread.
    ///
    /// \returns  The \a replace_copy_if algorithm returns an
    ///           \a in_out_result<typename pika::traits::range_iterator<Rng>::type,
    ///             OutIter>.
    ///           The \a replace_copy_if algorithm returns the input iterator
    ///           \a last and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename Rng, typename OutIter, typename Pred, typename T,
        typename Proj = pika::parallel::detail::projection_identity>
    replace_copy_if_result<typename pika::traits::range_iterator<Rng>::type,
        OutIter>
    replace_copy_if(Rng&& rng, OutIter dest, Pred&& pred, T const& new_value,
        Proj&& proj = Proj());

    /// Copies the all elements from the range [first, sent) to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (sent - first)) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(f, INVOKE(proj, *(first + (i - result)))) != false
    ///
    /// \note   Complexity: Performs exactly \a sent - \a first applications of
    ///         the predicate.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the source iterator used (deduced).
    ///                     The iterator type must
    ///                     meet the requirements of a forward iterator.
    /// \tparam Sent        The type of the end iterators used (deduced). This
    ///                     sentinel type must be a sentinel for InIter.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     forward iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param sent         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate which returns \a true for the
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_copy_if algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace_copy_if algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace_copy_if algorithm returns an
    ///           \a pika::future<FwdIter1, FwdIter2>.
    ///           The \a replace_copy_if algorithm returns the input iterator
    ///           \a last and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename Sent,
        typename FwdIter2, typename Pred, typename T,
        typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        replace_copy_if_result<FwdIter1, FwdIter2>>::type
    replace_copy_if(ExPolicy&& policy, FwdIter1 first, Sent sent, FwdIter2 dest,
        Pred&& pred, T const& new_value, Proj&& proj = Proj());

    /// Copies the all elements from the range rng to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (util::end(rng) - util::begin(rng))) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(f, INVOKE(proj, *(first + (i - result)))) != false
    ///
    /// \note   Complexity: Performs exactly \a util::end(rng) - \a util::begin(rng)
    ///         applications of the predicate.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam Rng         The type of the source range used (deduced).
    ///                     The iterators extracted from this range type must
    ///                     meet the requirements of an input iterator.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    /// \tparam Proj        The type of an optional projection function. This
    ///                     defaults to \a pika::parallel::detail::projection_identity
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param rng          Refers to the sequence of elements the algorithm
    ///                     will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param pred         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements in the
    ///                     sequence specified by [first, last).This is an
    ///                     unary predicate which returns \a true for the
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    /// \param proj         Specifies the function (or function object) which
    ///                     will be invoked for each of the elements as a
    ///                     projection operation before the actual predicate
    ///                     \a is invoked.
    ///
    /// The assignments in the parallel \a replace_copy_if algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a replace_copy_if algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an unordered
    /// fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a replace_copy_if algorithm returns an
    ///           \a pika::future<in_out_result<typename pika::traits::range_iterator<Rng>::type,
    ///             OutIter>>.
    ///           The \a replace_copy_if algorithm returns the input iterator
    ///           \a last and the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename Rng, typename FwdIter, typename Pred,
        typename T, typename Proj = pika::parallel::detail::projection_identity>
    typename pika::parallel::detail::algorithm_result<ExPolicy,
        replace_copy_if_result<typename pika::traits::range_iterator<Rng>::type,
            FwdIter>>::type
    replace_copy_if(ExPolicy&& policy, Rng&& rng, FwdIter dest, Pred&& pred,
        T const& new_value, Proj&& proj = Proj());

}}    // namespace pika::ranges

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/iterator_support/range.hpp>
#include <pika/iterator_support/traits/is_range.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>

#include <pika/algorithms/traits/projected_range.hpp>
#include <pika/parallel/algorithms/replace.hpp>
#include <pika/parallel/util/projection_identity.hpp>

#include <type_traits>
#include <utility>

namespace pika::ranges {
    /// `replace_copy_if_result` is equivalent to
    /// `pika::parallel::detail::in_out_result`
    template <typename I, typename O>
    using replace_copy_if_result = pika::parallel::detail::in_out_result<I, O>;

    /// `replace_copy_result` is equivalent to
    /// `pika::parallel::detail::in_out_result`
    template <typename I, typename O>
    using replace_copy_result = pika::parallel::detail::in_out_result<I, O>;

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::replace_if
    inline constexpr struct replace_if_t final
      : pika::detail::tag_parallel_algorithm<replace_if_t>
    {
    private:
        // clang-format off
        template <typename Iter, typename Sent, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T = typename pika::parallel::detail::projected<Iter,
                Proj>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<Iter>::value_type
                >
            )>
        // clang-format on
        friend Iter tag_fallback_invoke(pika::ranges::replace_if_t, Iter first,
            Sent sent, Pred&& pred, T const& new_value, Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_input_iterator<Iter>::value),
                "Required at least input iterator.");

            return pika::parallel::detail::replace_if<Iter>().call(
                pika::execution::seq, first, sent, PIKA_FORWARD(Pred, pred),
                new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T = typename pika::parallel::detail::projected<
                pika::traits::range_iterator_t<Rng>, Proj>::value_type,
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
        tag_fallback_invoke(pika::ranges::replace_if_t, Rng&& rng, Pred&& pred,
            T const& new_value, Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_input_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least input iterator.");

            return pika::parallel::detail::replace_if<
                typename pika::traits::range_iterator<Rng>::type>()
                .call(pika::execution::seq, pika::util::begin(rng),
                    pika::util::end(rng), PIKA_FORWARD(Pred, pred), new_value,
                    PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T = typename pika::parallel::detail::projected<Iter,
                Proj>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy,
                    Pred, pika::parallel::detail::projected<Proj, Iter>>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(pika::ranges::replace_if_t, ExPolicy&& policy,
            Iter first, Sent sent, Pred&& pred, T const& new_value,
            Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_forward_iterator<Iter>::value),
                "Required at least forward iterator.");

            return pika::parallel::detail::replace_if<Iter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, sent,
                PIKA_FORWARD(Pred, pred), new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng, typename Pred,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T = typename pika::parallel::detail::projected<
                pika::traits::range_iterator_t<Rng>, Proj>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy,
                    Pred, pika::parallel::detail::projected_range<Proj, Rng>>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng>::type>::type
        tag_fallback_invoke(pika::ranges::replace_if_t, ExPolicy&& policy,
            Rng&& rng, Pred&& pred, T const& new_value, Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_forward_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least forward iterator.");

            return pika::parallel::detail::replace_if<
                typename pika::traits::range_iterator<Rng>::type>()
                .call(PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                    pika::util::end(rng), PIKA_FORWARD(Pred, pred), new_value,
                    PIKA_FORWARD(Proj, proj));
        }
    } replace_if{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::replace
    inline constexpr struct replace_t final
      : pika::detail::tag_parallel_algorithm<replace_t>
    {
    private:
        // clang-format off
        template <typename Iter, typename Sent,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T1 = typename pika::parallel::detail::projected<Iter,
                Proj>::value_type,
            typename T2 = T1,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value
            )>
        // clang-format on
        friend Iter
        tag_fallback_invoke(pika::ranges::replace_t, Iter first, Sent sent,
            T1 const& old_value, T2 const& new_value, Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_input_iterator<Iter>::value),
                "Required at least input iterator.");

            using Type = typename std::iterator_traits<Iter>::value_type;

            return pika::ranges::replace_if(
                first, sent,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T1 = typename pika::parallel::detail::projected<
                pika::traits::range_iterator_t<Rng>, Proj>::value_type,
            typename T2 = T1,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value
            )>
        // clang-format on
        friend typename pika::traits::range_iterator<Rng>::type
        tag_fallback_invoke(pika::ranges::replace_t, Rng&& rng,
            T1 const& old_value, T2 const& new_value, Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_input_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least input iterator.");

            using Type = typename std::iterator_traits<
                typename pika::traits::range_iterator<Rng>::type>::value_type;

            return pika::ranges::replace_if(
                PIKA_FORWARD(Rng, rng),
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Iter, typename Sent,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T1 = typename pika::parallel::detail::projected<Iter,
                Proj>::value_type,
            typename T2 = T1,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<Iter>::value &&
                pika::traits::is_sentinel_for<Sent, Iter>::value &&
                pika::parallel::detail::is_projected<Proj, Iter>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            Iter>::type
        tag_fallback_invoke(pika::ranges::replace_t, ExPolicy&& policy,
            Iter first, Sent sent, T1 const& old_value, T2 const& new_value,
            Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_forward_iterator<Iter>::value),
                "Required at least forward iterator.");

            using Type = typename std::iterator_traits<Iter>::value_type;

            return pika::ranges::replace_if(
                PIKA_FORWARD(ExPolicy, policy), first, sent,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T1 = typename pika::parallel::detail::projected<
                pika::traits::range_iterator_t<Rng>, Proj>::value_type,
            typename T2 = T1,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            typename pika::traits::range_iterator<Rng>::type>::type
        tag_fallback_invoke(pika::ranges::replace_t, ExPolicy&& policy,
            Rng&& rng, T1 const& old_value, T2 const& new_value,
            Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_forward_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least forward iterator.");

            using Type = typename std::iterator_traits<
                typename pika::traits::range_iterator<Rng>::type>::value_type;

            return pika::ranges::replace_if(
                PIKA_FORWARD(ExPolicy, policy), PIKA_FORWARD(Rng, rng),
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value, PIKA_FORWARD(Proj, proj));
        }
    } replace{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::replace_copy_if
    inline constexpr struct replace_copy_if_t final
      : pika::detail::tag_parallel_algorithm<replace_copy_if_t>
    {
    private:
        // clang-format off
        template <typename InIter, typename Sent, typename OutIter,
            typename Pred,
            typename T = typename std::iterator_traits<OutIter>::value_type,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value &&
                pika::traits::is_iterator<OutIter>::value &&
                pika::parallel::detail::is_projected<Proj, InIter>::value &&
                pika::traits::is_sentinel_for<Sent, InIter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<InIter>::value_type
                >
            )>
        // clang-format on
        friend replace_copy_if_result<InIter, OutIter> tag_fallback_invoke(
            pika::ranges::replace_copy_if_t, InIter first, Sent sent,
            OutIter dest, Pred&& pred, T const& new_value, Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_input_iterator<InIter>::value),
                "Required at least input iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            return pika::parallel::detail::replace_copy_if<
                pika::parallel::detail::in_out_result<InIter, OutIter>>()
                .call(pika::execution::seq, first, sent, dest,
                    PIKA_FORWARD(Pred, pred), new_value,
                    PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng, typename OutIter, typename Pred,
            typename T = typename std::iterator_traits<OutIter>::value_type,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::traits::is_iterator<OutIter>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<
                        typename pika::traits::range_iterator<Rng>::type
                    >::value_type
                >
            )>
        // clang-format on
        friend replace_copy_if_result<
            typename pika::traits::range_iterator<Rng>::type, OutIter>
        tag_fallback_invoke(pika::ranges::replace_copy_if_t, Rng&& rng,
            OutIter dest, Pred&& pred, T const& new_value, Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_input_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least input iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            return pika::parallel::detail::replace_copy_if<
                pika::parallel::detail::in_out_result<
                    typename pika::traits::range_iterator<Rng>::type,
                    OutIter>>()
                .call(pika::execution::seq, pika::util::begin(rng),
                    pika::util::end(rng), dest, PIKA_FORWARD(Pred, pred),
                    new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename Sent,
            typename FwdIter2, typename Pred,
            typename T = typename std::iterator_traits<FwdIter2>::value_type,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value &&
                pika::parallel::detail::is_projected<Proj, FwdIter1>::value &&
                pika::traits::is_sentinel_for<Sent, FwdIter1>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy,
                    Pred, pika::parallel::detail::projected<Proj, FwdIter1>>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            replace_copy_if_result<FwdIter1, FwdIter2>>::type
        tag_fallback_invoke(pika::ranges::replace_copy_if_t, ExPolicy&& policy,
            FwdIter1 first, Sent sent, FwdIter2 dest, Pred&& pred,
            T const& new_value, Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Required at least forward iterator.");

            static_assert((pika::traits::is_forward_iterator<FwdIter2>::value),
                "Required at least forward iterator.");

            return pika::parallel::detail::replace_copy_if<
                pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                .call(PIKA_FORWARD(ExPolicy, policy), first, sent, dest,
                    PIKA_FORWARD(Pred, pred), new_value,
                    PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng, typename FwdIter, typename Pred,
            typename T = typename std::iterator_traits<FwdIter>::value_type,
            typename Proj = pika::parallel::detail::projection_identity,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::traits::is_iterator<FwdIter>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value &&
                pika::parallel::detail::is_indirect_callable<ExPolicy,
                    Pred, pika::parallel::detail::projected_range<Proj, Rng>>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            replace_copy_if_result<
                typename pika::traits::range_iterator<Rng>::type,
                FwdIter>>::type
        tag_fallback_invoke(pika::ranges::replace_copy_if_t, ExPolicy&& policy,
            Rng&& rng, FwdIter dest, Pred&& pred, T const& new_value,
            Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_forward_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least forward iterator.");

            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Required at least forward iterator.");

            return pika::parallel::detail::replace_copy_if<
                pika::parallel::detail::in_out_result<
                    typename pika::traits::range_iterator<Rng>::type,
                    FwdIter>>()
                .call(PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                    pika::util::end(rng), dest, PIKA_FORWARD(Pred, pred),
                    new_value, PIKA_FORWARD(Proj, proj));
        }
    } replace_copy_if{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::ranges::replace_copy
    inline constexpr struct replace_copy_t final
      : pika::detail::tag_parallel_algorithm<replace_copy_t>
    {
    private:
        // clang-format off
        template <typename InIter, typename Sent,
            typename OutIter,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T1 = typename pika::parallel::detail::projected<InIter,
                Proj>::value_type, typename T2 = T1,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value &&
                pika::parallel::detail::is_projected<Proj, InIter>::value &&
                pika::traits::is_sentinel_for<Sent, InIter>::value
            )>
        // clang-format on
        friend replace_copy_result<InIter, OutIter> tag_fallback_invoke(
            pika::ranges::replace_copy_t, InIter first, Sent sent, OutIter dest,
            T1 const& old_value, T2 const& new_value, Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_input_iterator<InIter>::value),
                "Required at least input iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            using Type = typename std::iterator_traits<InIter>::value_type;

            return pika::ranges::replace_copy_if(
                pika::execution::seq, first, sent, dest,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename Rng, typename OutIter,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T1 = typename pika::parallel::detail::projected<
                pika::traits::range_iterator_t<Rng>, Proj>::value_type,
            typename T2 = T1,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value
            )>
        // clang-format on
        friend replace_copy_result<
            typename pika::traits::range_iterator<Rng>::type, OutIter>
        tag_fallback_invoke(pika::ranges::replace_copy_t, Rng&& rng,
            OutIter dest, T1 const& old_value, T2 const& new_value,
            Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_input_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least input iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            using Type = typename std::iterator_traits<
                typename pika::traits::range_iterator<Rng>::type>::value_type;

            return pika::ranges::replace_copy_if(
                pika::execution::seq, pika::util::begin(rng),
                pika::util::end(rng), dest,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1,
            typename Sent, typename FwdIter2,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T1 = typename pika::parallel::detail::projected<FwdIter1,
                Proj>::value_type, typename T2 = T1,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value &&
                pika::traits::is_sentinel_for<Sent, FwdIter1>::value &&
                pika::parallel::detail::is_projected<Proj, FwdIter1>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            replace_copy_result<FwdIter1, FwdIter2>>::type
        tag_fallback_invoke(pika::ranges::replace_copy_t, ExPolicy&& policy,
            FwdIter1 first, Sent sent, FwdIter2 dest, T1 const& old_value,
            T2 const& new_value, Proj&& proj = Proj())
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Required at least forward iterator.");

            static_assert((pika::traits::is_forward_iterator<FwdIter2>::value),
                "Required at least forward iterator.");

            using Type = typename std::iterator_traits<FwdIter1>::value_type;

            return pika::ranges::replace_copy_if(
                PIKA_FORWARD(ExPolicy, policy), first, sent, dest,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value, PIKA_FORWARD(Proj, proj));
        }

        // clang-format off
        template <typename ExPolicy, typename Rng, typename FwdIter,
            typename Proj = pika::parallel::detail::projection_identity,
            typename T1 = typename pika::parallel::detail::projected<
                pika::traits::range_iterator_t<Rng>, Proj>::value_type,
            typename T2 = T1,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_range<Rng>::value &&
                pika::parallel::detail::is_projected_range<Proj, Rng>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            replace_copy_result<
                typename pika::traits::range_iterator<Rng>::type,
                FwdIter>>::type
        tag_fallback_invoke(pika::ranges::replace_copy_t, ExPolicy&& policy,
            Rng&& rng, FwdIter dest, T1 const& old_value, T2 const& new_value,
            Proj&& proj = Proj())
        {
            static_assert(
                (pika::traits::is_forward_iterator<
                    typename pika::traits::range_iterator<Rng>::type>::value),
                "Required at least forward iterator.");

            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Required at least forward iterator.");

            using Type = typename std::iterator_traits<
                typename pika::traits::range_iterator<Rng>::type>::value_type;

            return pika::ranges::replace_copy_if(
                PIKA_FORWARD(ExPolicy, policy), pika::util::begin(rng),
                pika::util::end(rng), dest,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value, PIKA_FORWARD(Proj, proj));
        }
    } replace_copy{};
}    // namespace pika::ranges

#endif    // DOXYGEN
