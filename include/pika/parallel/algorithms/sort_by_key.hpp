//  Copyright (c) 2016 Hartmut Kaiser
//  Copyright (c) 2016 John Biddiscombe
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/parallel/algorithms/detail/predicates.hpp>
#include <pika/parallel/algorithms/sort.hpp>
#include <pika/parallel/util/zip_iterator.hpp>

#include <algorithm>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <utility>

namespace pika {
    template <typename KeyIter, typename ValueIter>
    using sort_by_key_result = std::pair<KeyIter, ValueIter>;

    ///////////////////////////////////////////////////////////////////////////
    // sort
    namespace parallel::detail {
        /// \cond NOINTERNAL
        struct extract_key
        {
            template <typename Tuple>
            auto operator()(Tuple&& t) const
                -> decltype(std::get<0>(PIKA_FORWARD(Tuple, t)))
            {
                return std::get<0>(PIKA_FORWARD(Tuple, t));
            }
        };
        /// \endcond
    }    // namespace parallel::detail

    //-----------------------------------------------------------------------------
    /// Sorts one range of data using keys supplied in another range.
    /// The key elements in the range [key_first, key_last) are sorted in
    /// ascending order with the corresponding elements in the value range
    /// moved to follow the sorted order.
    /// The algorithm is not stable, the order of equal elements is not guaranteed
    /// to be preserved.
    /// The function uses the given comparison function object comp (defaults
    /// to using operator<()).
    ///
    /// \note   Complexity: O(Nlog(N)), where N = std::distance(first, last)
    ///                     comparisons.
    ///
    /// A sequence is sorted with respect to a comparator \a comp and a
    /// projection \a proj if for every iterator i pointing to the sequence and
    /// every non-negative integer n such that i + n is a valid iterator
    /// pointing to an element of the sequence, and
    /// INVOKE(comp, INVOKE(proj, *(i + n)), INVOKE(proj, *i)) == false.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it applies user-provided function objects.
    /// \tparam KeyIter     The type of the key iterators used (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     random access iterator.
    /// \tparam ValueIter   The type of the value iterators used (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     random access iterator.
    /// \tparam Comp        The type of the function/function object to use
    ///                     (deduced).
    ///
    /// \param policy       The execution policy to use for the scheduling of
    ///                     the iterations.
    /// \param key_first    Refers to the beginning of the sequence of key
    ///                     elements the algorithm will be applied to.
    /// \param key_last     Refers to the end of the sequence of key elements the
    ///                     algorithm will be applied to.
    /// \param value_first  Refers to the beginning of the sequence of value
    ///                     elements the algorithm will be applied to, the range
    ///                     of elements must match [key_first, key_last)
    /// \param comp         comp is a callable object. The return value of the
    ///                     INVOKE operation applied to an object of type Comp,
    ///                     when contextually converted to bool, yields true if
    ///                     the first argument of the call is less than the
    ///                     second, and false otherwise. It is assumed that comp
    ///                     will not apply any non-constant function through the
    ///                     dereferenced iterator.
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
    /// \returns  The \a sort_by-key algorithm returns a
    /// \a pika::future<sort_by_key_result<KeyIter, ValueIter>>
    ///           if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and returns \a
    ///           \a sort_by_key_result<KeyIter, ValueIter>
    ///           otherwise.
    ///           The algorithm returns a pair holding an iterator pointing to
    ///           the first element after the last element in the input key
    ///           sequence and an iterator pointing to the first element after
    ///           the last element in the input value sequence.
    //-----------------------------------------------------------------------------

    template <typename ExPolicy, typename KeyIter, typename ValueIter,
        typename Compare = parallel::detail::less>
    parallel::detail::algorithm_result_t<ExPolicy,
        sort_by_key_result<KeyIter, ValueIter>>
    sort_by_key([[maybe_unused]] ExPolicy&& policy,
        [[maybe_unused]] KeyIter key_first, [[maybe_unused]] KeyIter key_last,
        [[maybe_unused]] ValueIter value_first,
        [[maybe_unused]] Compare&& comp = Compare())
    {
#if !defined(PIKA_HAVE_TUPLE_RVALUE_SWAP)
        static_assert(sizeof(KeyIter) == 0,    // always false
            "sort_by_key is not supported unless PIKA_HAVE_TUPLE_RVALUE_SWAP "
            "is defined");
#else
        static_assert((pika::traits::is_random_access_iterator_v<KeyIter>),
            "Requires a random access iterator.");
        static_assert((pika::traits::is_random_access_iterator_v<ValueIter>),
            "Requires a random access iterator.");

        ValueIter value_last = value_first;
        std::advance(value_last, std::distance(key_first, key_last));

        using iterator_type = pika::util::zip_iterator<KeyIter, ValueIter>;

        return detail::get_iter_pair<iterator_type>(
            detail::sort<iterator_type>().call(PIKA_FORWARD(ExPolicy, policy),
                pika::util::make_zip_iterator(key_first, value_first),
                pika::util::make_zip_iterator(key_last, value_last),
                PIKA_FORWARD(Compare, comp), parallel::detail::extract_key()));
#endif
    }
}    // namespace pika
