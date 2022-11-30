//  Copyright (c) 2014-2017 Hartmut Kaiser
//  Copyright (c)      2021 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/replace.hpp

#pragma once

#if defined(DOXYGEN)

namespace pika {

    /// Replaces all elements satisfying specific criteria with \a new_value
    /// in the range [first, last).
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          [first, last) with new_value, when the following corresponding
    ///          conditions hold: *it == old_value
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam InIter      The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam T           The type of the old and new values to replace (deduced).
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    ///
    /// The assignments in the parallel \a replace algorithm
    /// execute in sequential order in the calling thread.
    ///
    ///
    /// \returns  The \a replace algorithm returns a \a void.
    ///
    template <typename Initer, typename T>
    void
    replace(InIter first, InIter last, T const& old_value, T const& new_value);

    /// Replaces all elements satisfying specific criteria with \a new_value
    /// in the range [first, last).
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          [first, last) with new_value, when the following corresponding
    ///          conditions hold: *it == old_value
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter     The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    /// \tparam T          The type of the old and new values to replace (deduced).
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
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
    /// \returns  The \a replace algorithm returns a \a pika::future<void> if
    ///           the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a void otherwise.
    ///
    template <typename ExPolicy, typename FwdIter, typename T>
    typename parallel::detail::algorithm_result<ExPolicy, void>::type
    replace(ExPolicy&& policy, FwdIter first, FwdIter last, T const& old_value,
        T const& new_value);

    /// Replaces all elements satisfying specific criteria (for which predicate
    /// \a f returns true) with \a new_value in the range [first, last).
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          [first, last) with new_value, when the following corresponding
    ///          conditions hold: INVOKE(f, *it) != false
    ///
    /// \note   Complexity: Performs exactly \a last - \a first applications of
    ///         the predicate.
    ///
    /// \tparam Iter        The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
    ///
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
    ///                     type \a InIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    ///
    /// The assignments in the parallel \a replace_if algorithm
    /// execute in sequential order in the calling thread.
    ///
    ///
    /// \returns  The \a replace_if algorithm returns \a void.
    ///
    template <typename Iter, typename Pred, typename T>
    void replace_if(Iter first, Iter last, Pred&& pred, T const& new_value);

    /// Replaces all elements satisfying specific criteria (for which predicate
    /// \a f returns true) with \a new_value in the range [first, last).
    ///
    /// Effects: Substitutes elements referred by the iterator it in the range
    ///          [first, last) with new_value, when the following corresponding
    ///          conditions hold: INVOKE(f, *it) != false
    ///
    /// \note   Complexity: Performs exactly \a last - \a first applications of
    ///         the predicate.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter     The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a Pred to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
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
    /// \returns  The \a replace_if algorithm returns a \a pika::future<void>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy
    ///           and returns \a void otherwise.
    ///
    template <typename ExPolicy, typename FwdIter, typename Pred, typename T>
    typename parallel::detail::algorithm_result<ExPolicy, void>::type
    replace_if(ExPolicy&& policy, FwdIter first, FwdIter last, Pred&& pred,
        T const& new_value);

    /// Copies the all elements from the range [first, last) to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (last - first)) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          *(first + (i - result)) == old_value
    ///
    /// \note   Complexity: Performs exactly \a last - \a first applications of
    ///         the predicate.
    ///

    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam InIter      The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam OutIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     output iterator.
    /// \tparam T          The type of the old and new values (deduced).
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
    ///
    /// The assignments in the parallel \a replace_copy algorithm
    /// execute in sequential order in the calling thread.
    ///
    ///
    /// \returns  The \a replace_copy algorithm returns an
    ///           \a OutIter
    ///           The \a replace_copy algorithm returns the
    ///           Iterator to the element past the last element copied.
    ///
    template <typename InIter, typename OutIter, typename T>
    OutIter replace_copy(InIter first, InIter last, OutIter dest,
        T const& old_value, T const& new_value);

    /// Copies the all elements from the range [first, last) to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (last - first)) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          *(first + (i - result)) == old_value
    ///
    /// \note   Complexity: Performs exactly \a last - \a first applications of
    ///         the predicate.
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
    /// \tparam T           The type of the old and new values (deduced).
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    /// \param old_value    Refers to the old value of the elements to replace.
    /// \param new_value    Refers to the new value to use as the replacement.
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
    ///           \a pika::future<FwdIter2>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter2
    ///           otherwise.
    ///           The \a replace_copy algorithm returns the
    ///           Iterator to the element past the last element copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
        typename T>
    typename parallel::detail::algorithm_result<ExPolicy, FwdIter2>::type
    replace_copy(ExPolicy&& policy, FwdIter1 first, FwdIter1 last,
        FwdIter2 dest, T const& old_value, T const& new_value);

    /// Copies the all elements from the range [first, last) to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (last - first)) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(f, *(first + (i - result))) != false
    ///
    /// \note   Complexity: Performs exactly \a last - \a first applications of
    ///         the predicate.
    ///
    /// \tparam InIter      The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
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
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
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
    ///                     type \a InIter can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
    ///
    /// The assignments in the parallel \a replace_copy_if algorithm
    /// execute in sequential order in the calling thread.
    ///
    ///
    /// \returns  The \a replace_copy_if algorithm returns an
    ///           \a OutIter.
    ///           The \a replace_copy_if algorithm returns
    ///           the output iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename InIter, typename OutIter, typename Pred, typename T>
    OutIter replace_copy_if(InIter first, InIter last, OutIter dest,
        Pred&& pred, T const& new_value);

    /// Copies the all elements from the range [first, last) to another range
    /// beginning at \a dest replacing all elements satisfying a specific
    /// criteria with \a new_value.
    ///
    /// Effects: Assigns to every iterator it in the range
    ///          [result, result + (last - first)) either new_value or
    ///          *(first + (it - result)) depending on whether the following
    ///          corresponding condition holds:
    ///          INVOKE(f, *(first + (i - result))) != false
    ///
    /// \note   Complexity: Performs exactly \a last - \a first applications of
    ///         the predicate.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1    The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    /// \tparam FwdIter2    The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    /// \tparam Pred        The type of the function/function object to use
    ///                     (deduced). Unlike its sequential form, the parallel
    ///                     overload of \a equal requires \a F to meet the
    ///                     requirements of \a CopyConstructible.
    ///                     (deduced).
    /// \tparam T           The type of the new values to replace (deduced).
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
    ///                     elements which need to replaced. The
    ///                     signature of this predicate should be equivalent
    ///                     to:
    ///                     \code
    ///                     bool pred(const Type &a);
    ///                     \endcode \n
    ///                     The signature does not need to have const&, but
    ///                     the function must not modify the objects passed to
    ///                     it. The type \a Type must be such that an object of
    ///                     type \a FwdIter1 can be dereferenced and then
    ///                     implicitly converted to \a Type.
    /// \param new_value    Refers to the new value to use as the replacement.
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
    /// \returns  The \a replace_copy_if algorithm returns a
    ///           \a pika::future<FwdIter2>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy
    ///           and returns \a FwdIter2
    ///           otherwise.
    ///           The \a replace_copy_if algorithm returns the
    ///           iterator to the
    ///           element in the destination range, one past the last element
    ///           copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
        typename Pred, typename T>
    typename parallel::detail::algorithm_result<ExPolicy, FwdIter2>::type
    replace_copy_if(ExPolicy&& policy, FwdIter1 first, FwdIter1 last,
        FwdIter2 dest, Pred&& pred, T const& new_value);

}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>
#include <pika/type_support/unused.hpp>

#include <pika/algorithms/traits/projected.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/for_each.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/projection_identity.hpp>
#include <pika/parallel/util/zip_iterator.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // replace
    /// \cond NOINTERNAL
    // sequential replace
    template <typename InIter, typename T1, typename T2, typename Proj>
    inline InIter sequential_replace(InIter first, InIter last,
        T1 const& old_value, T2 const& new_value, Proj&& proj)
    {
        for (/* */; first != last; ++first)
        {
            if (PIKA_INVOKE(proj, *first) == old_value)
            {
                *first = new_value;
            }
        }
        return first;
    }

    template <typename Iter>
    struct replace : public algorithm<replace<Iter>, Iter>
    {
        replace()
          : replace::algorithm("replace")
        {
        }

        template <typename ExPolicy, typename InIter, typename T1, typename T2,
            typename Proj>
        static InIter sequential(ExPolicy, InIter first, InIter last,
            T1 const& old_value, T2 const& new_value, Proj&& proj)
        {
            return sequential_replace(
                first, last, old_value, new_value, PIKA_FORWARD(Proj, proj));
        }

        template <typename ExPolicy, typename FwdIter, typename T1, typename T2,
            typename Proj>
        static typename algorithm_result<ExPolicy, FwdIter>::type
        parallel(ExPolicy&& policy, FwdIter first, FwdIter last,
            T1 const& old_value, T2 const& new_value, Proj&& proj)
        {
            using type = typename std::iterator_traits<FwdIter>::value_type;

            return for_each_n<FwdIter>().call(
                PIKA_FORWARD(ExPolicy, policy), first,
                std::distance(first, last),
                [old_value, new_value, proj = PIKA_FORWARD(Proj, proj)](
                    type& t) -> void {
                    if (PIKA_INVOKE(proj, t) == old_value)
                    {
                        t = new_value;
                    }
                },
                projection_identity());
        }
    };
    /// \endcond

    ///////////////////////////////////////////////////////////////////////////
    // replace_if
    /// \cond NOINTERNAL

    // sequential replace_if
    template <typename InIter, typename Sent, typename F, typename T,
        typename Proj>
    inline InIter sequential_replace_if(
        InIter first, Sent sent, F&& f, T const& new_value, Proj&& proj)
    {
        for (/* */; first != sent; ++first)
        {
            if (PIKA_INVOKE(f, PIKA_INVOKE(proj, *first)))
            {
                *first = new_value;
            }
        }
        return first;
    }

    template <typename Iter>
    struct replace_if : public algorithm<replace_if<Iter>, Iter>
    {
        replace_if()
          : replace_if::algorithm("replace_if")
        {
        }

        template <typename ExPolicy, typename InIter, typename Sent, typename F,
            typename T, typename Proj>
        static InIter sequential(ExPolicy, InIter first, Sent last, F&& f,
            T const& new_value, Proj&& proj)
        {
            return sequential_replace_if(first, last, PIKA_FORWARD(F, f),
                new_value, PIKA_FORWARD(Proj, proj));
        }

        template <typename ExPolicy, typename FwdIter, typename Sent,
            typename F, typename T, typename Proj>
        static typename algorithm_result<ExPolicy, FwdIter>::type
        parallel(ExPolicy&& policy, FwdIter first, Sent last, F&& f,
            T const& new_value, Proj&& proj)
        {
            using type = typename std::iterator_traits<FwdIter>::value_type;

            return for_each_n<FwdIter>().call(
                PIKA_FORWARD(ExPolicy, policy), first, (distance) (first, last),
                [new_value, f = PIKA_FORWARD(F, f),
                    proj = PIKA_FORWARD(Proj, proj)](type& t) mutable -> void {
                    if (PIKA_INVOKE(f, PIKA_INVOKE(proj, t)))
                    {
                        t = new_value;
                    }
                },
                projection_identity());
        }
    };
    /// \endcond

    ///////////////////////////////////////////////////////////////////////////
    // replace_copy
    /// \cond NOINTERNAL

    // sequential replace_copy
    template <typename InIter, typename Sent, typename OutIter, typename T,
        typename Proj>
    inline in_out_result<InIter, OutIter>
    sequential_replace_copy(InIter first, Sent sent, OutIter dest,
        T const& old_value, T const& new_value, Proj&& proj)
    {
        for (/* */; first != sent; ++first)
        {
            if (PIKA_INVOKE(proj, *first) == old_value)
                *dest++ = new_value;
            else
                *dest++ = *first;
        }
        return in_out_result<InIter, OutIter>(first, dest);
    }

    template <typename IterPair>
    struct replace_copy : public algorithm<replace_copy<IterPair>, IterPair>
    {
        replace_copy()
          : replace_copy::algorithm("replace_copy")
        {
        }

        template <typename ExPolicy, typename InIter, typename Sent,
            typename OutIter, typename T, typename Proj>
        static in_out_result<InIter, OutIter>
        sequential(ExPolicy, InIter first, Sent sent, OutIter dest,
            T const& old_value, T const& new_value, Proj&& proj)
        {
            return sequential_replace_copy(first, sent, dest, old_value,
                new_value, PIKA_FORWARD(Proj, proj));
        }

        template <typename ExPolicy, typename FwdIter1, typename Sent,
            typename FwdIter2, typename T, typename Proj>
        static typename algorithm_result<ExPolicy,
            in_out_result<FwdIter1, FwdIter2>>::type
        parallel(ExPolicy&& policy, FwdIter1 first, Sent sent, FwdIter2 dest,
            T const& old_value, T const& new_value, Proj&& proj)
        {
            using zip_iterator = pika::util::zip_iterator<FwdIter1, FwdIter2>;
            using reference = typename zip_iterator::reference;

            return get_in_out_result(for_each_n<zip_iterator>().call(
                PIKA_FORWARD(ExPolicy, policy),
                pika::util::make_zip_iterator(first, dest),
                (distance) (first, sent),
                [old_value, new_value, proj = PIKA_FORWARD(Proj, proj)](
                    reference t) -> void {
                    using std::get;
                    if (PIKA_INVOKE(proj, get<0>(t)) == old_value)
                        get<1>(t) = new_value;
                    else
                        get<1>(t) = get<0>(t);    //-V573
                },
                projection_identity()));
        }
    };
    /// \endcond

    ///////////////////////////////////////////////////////////////////////////
    // replace_copy_if
    /// \cond NOINTERNAL

    // sequential replace_copy_if
    template <typename InIter, typename Sent, typename OutIter, typename F,
        typename T, typename Proj>
    inline in_out_result<InIter, OutIter>
    sequential_replace_copy_if(InIter first, Sent sent, OutIter dest, F&& f,
        T const& new_value, Proj&& proj)
    {
        for (/* */; first != sent; ++first)
        {
            if (PIKA_INVOKE(f, PIKA_INVOKE(proj, *first)))
            {
                *dest++ = new_value;
            }
            else
            {
                *dest++ = *first;
            }
        }
        return in_out_result<InIter, OutIter>{first, dest};
    }

    template <typename IterPair>
    struct replace_copy_if
      : public algorithm<replace_copy_if<IterPair>, IterPair>
    {
        replace_copy_if()
          : replace_copy_if::algorithm("replace_copy_if")
        {
        }

        template <typename ExPolicy, typename InIter, typename Sent,
            typename OutIter, typename F, typename T, typename Proj>
        static in_out_result<InIter, OutIter> sequential(ExPolicy, InIter first,
            Sent sent, OutIter dest, F&& f, T const& new_value, Proj&& proj)
        {
            return sequential_replace_copy_if(first, sent, dest,
                PIKA_FORWARD(F, f), new_value, PIKA_FORWARD(Proj, proj));
        }

        template <typename ExPolicy, typename FwdIter1, typename Sent,
            typename FwdIter2, typename F, typename T, typename Proj>
        static typename algorithm_result<ExPolicy,
            in_out_result<FwdIter1, FwdIter2>>::type
        parallel(ExPolicy&& policy, FwdIter1 first, Sent sent, FwdIter2 dest,
            F&& f, T const& new_value, Proj&& proj)
        {
            using zip_iterator = pika::util::zip_iterator<FwdIter1, FwdIter2>;
            using reference = typename zip_iterator::reference;

            return get_in_out_result(for_each_n<zip_iterator>().call(
                PIKA_FORWARD(ExPolicy, policy),
                pika::util::make_zip_iterator(first, dest),
                (distance) (first, sent),
                [new_value, f = PIKA_FORWARD(F, f),
                    proj = PIKA_FORWARD(Proj, proj)](
                    reference t) mutable -> void {
                    using std::get;
                    if (PIKA_INVOKE(f, PIKA_INVOKE(proj, get<0>(t))))
                    {
                        get<1>(t) = new_value;
                    }
                    else
                    {
                        get<1>(t) = get<0>(t);    //-V573
                    }
                },
                projection_identity()));
        }
    };
    /// \endcond
}    // namespace pika::parallel::detail

namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::replace_if
    inline constexpr struct replace_if_t final
      : pika::detail::tag_parallel_algorithm<replace_if_t>
    {
    private:
        // clang-format off
        template <typename Iter,
            typename Pred,
            typename T = typename std::iterator_traits<Iter>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<Iter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<Iter>::value_type
                >
            )>
        // clang-format on
        friend void tag_fallback_invoke(pika::replace_if_t, Iter first,
            Iter last, Pred&& pred, T const& new_value)
        {
            static_assert((pika::traits::is_input_iterator<Iter>::value),
                "Required at least input iterator.");

            pika::parallel::detail::replace_if<Iter>().call(
                pika::execution::sequenced_policy{}, first, last,
                PIKA_FORWARD(Pred, pred), new_value,
                pika::parallel::detail::projection_identity());
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter,
            typename Pred,
            typename T = typename std::iterator_traits<FwdIter>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter>::value_type
                >
            )>
        // clang-format on
        friend typename parallel::detail::algorithm_result<ExPolicy, void>::type
        tag_fallback_invoke(pika::replace_if_t, ExPolicy&& policy,
            FwdIter first, FwdIter last, Pred&& pred, T const& new_value)
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Required at least forward iterator.");

            return parallel::detail::algorithm_result<ExPolicy>::get(
                pika::parallel::detail::replace_if<FwdIter>().call(
                    PIKA_FORWARD(ExPolicy, policy), first, last,
                    PIKA_FORWARD(Pred, pred), new_value,
                    pika::parallel::detail::projection_identity()));
        }
    } replace_if{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::replace
    inline constexpr struct replace_t final
      : pika::detail::tag_parallel_algorithm<replace_t>
    {
    private:
        // clang-format off
        template <typename InIter,
            typename T = typename std::iterator_traits<InIter>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value
            )>
        // clang-format on
        friend void tag_fallback_invoke(pika::replace_t, InIter first,
            InIter last, T const& old_value, T const& new_value)
        {
            static_assert((pika::traits::is_input_iterator<InIter>::value),
                "Required at least input iterator.");

            using Type = typename std::iterator_traits<InIter>::value_type;

            return pika::replace_if(
                pika::execution::seq, first, last,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value);
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter,
            typename T = typename std::iterator_traits<FwdIter>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter>::value
            )>
        // clang-format on
        friend typename parallel::detail::algorithm_result<ExPolicy, void>::type
        tag_fallback_invoke(pika::replace_t, ExPolicy&& policy, FwdIter first,
            FwdIter last, T const& old_value, T const& new_value)
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter>::value),
                "Required at least forward iterator.");

            using Type = typename std::iterator_traits<FwdIter>::value_type;

            return pika::replace_if(
                PIKA_FORWARD(ExPolicy, policy), first, last,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value);
        }
    } replace{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::replace_copy_if
    inline constexpr struct replace_copy_if_t final
      : pika::detail::tag_parallel_algorithm<replace_copy_if_t>
    {
    private:
        // clang-format off
        template <typename InIter, typename OutIter,
            typename Pred,
            typename T = typename std::iterator_traits<OutIter>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value &&
                pika::traits::is_iterator<OutIter>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<InIter>::value_type
                >
            )>
        // clang-format on
        friend OutIter
        tag_fallback_invoke(pika::replace_copy_if_t, InIter first, InIter last,
            OutIter dest, Pred&& pred, T const& new_value)
        {
            static_assert((pika::traits::is_input_iterator<InIter>::value),
                "Required at least input iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            return parallel::detail::get_second_element(
                pika::parallel::detail::replace_copy_if<
                    pika::parallel::detail::in_out_result<InIter, OutIter>>()
                    .call(pika::execution::sequenced_policy{}, first, last,
                        dest, PIKA_FORWARD(Pred, pred), new_value,
                        pika::parallel::detail::projection_identity()));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename Pred,
            typename T = typename std::iterator_traits<FwdIter2>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value &&
                pika::detail::is_invocable_v<Pred,
                    typename std::iterator_traits<FwdIter1>::value_type
                >
            )>
        // clang-format on
        friend typename parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::replace_copy_if_t, ExPolicy&& policy,
            FwdIter1 first, FwdIter1 last, FwdIter2 dest, Pred&& pred,
            T const& new_value)
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Required at least forward iterator.");

            static_assert((pika::traits::is_forward_iterator<FwdIter2>::value),
                "Required at least forward iterator.");

            return parallel::detail::get_second_element(
                pika::parallel::detail::replace_copy_if<
                    pika::parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(PIKA_FORWARD(ExPolicy, policy), first, last, dest,
                        PIKA_FORWARD(Pred, pred), new_value,
                        pika::parallel::detail::projection_identity()));
        }
    } replace_copy_if{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::replace_copy
    inline constexpr struct replace_copy_t final
      : pika::detail::tag_parallel_algorithm<replace_copy_t>
    {
    private:
        // clang-format off
        template <typename InIter, typename OutIter,
            typename T = typename std::iterator_traits<OutIter>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value &&
                pika::traits::is_iterator<OutIter>::value
            )>
        // clang-format on
        friend OutIter tag_fallback_invoke(pika::replace_copy_t, InIter first,
            InIter last, OutIter dest, T const& old_value, T const& new_value)
        {
            static_assert((pika::traits::is_input_iterator<InIter>::value),
                "Required at least input iterator.");

            static_assert((pika::traits::is_output_iterator<OutIter>::value),
                "Required at least output iterator.");

            using Type = typename std::iterator_traits<InIter>::value_type;

            return pika::replace_copy_if(
                pika::execution::seq, first, last, dest,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value);
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            typename T = typename std::iterator_traits<FwdIter2>::value_type,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_iterator<FwdIter1>::value &&
                pika::traits::is_iterator<FwdIter2>::value
            )>
        // clang-format on
        friend typename parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::replace_copy_t, ExPolicy&& policy,
            FwdIter1 first, FwdIter1 last, FwdIter2 dest, T const& old_value,
            T const& new_value)
        {
            static_assert((pika::traits::is_forward_iterator<FwdIter1>::value),
                "Required at least forward iterator.");

            static_assert((pika::traits::is_forward_iterator<FwdIter2>::value),
                "Required at least forward iterator.");

            using Type = typename std::iterator_traits<FwdIter1>::value_type;

            return pika::replace_copy_if(
                PIKA_FORWARD(ExPolicy, policy), first, last, dest,
                [old_value](Type const& a) -> bool { return old_value == a; },
                new_value);
        }
    } replace_copy{};
}    // namespace pika

#endif    // DOXYGEN
