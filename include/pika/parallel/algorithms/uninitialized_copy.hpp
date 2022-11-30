//  Copyright (c) 2014-2017 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

/// \file parallel/algorithms/uninitialized_copy.hpp

#pragma once

#if defined(DOXYGEN)
namespace pika {

    /// Copies the elements in the range, defined by [first, last), to an
    /// uninitialized memory area beginning at \a dest. If an exception is
    /// thrown during the copy operation, the function has no effects.
    ///
    /// \note   Complexity: Performs exactly \a last - \a first assignments.
    ///
    /// \tparam InIter      The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam FwdIter     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param last         Refers to the end of the sequence of elements the
    ///                     algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    ///
    /// The assignments in the parallel \a uninitialized_copy algorithm invoked
    /// without an execution policy object will execute in sequential order in
    /// the calling thread.
    ///
    /// \returns  The \a uninitialized_copy algorithm returns \a FwdIter.
    ///           The \a uninitialized_copy algorithm returns the output
    ///           iterator to the element in the destination range, one past
    ///           the last element copied.
    ///
    template <typename InIter, typename FwdIter>
    FwdIter uninitialized_copy(InIter first, InIter last, FwdIter dest);

    /// Copies the elements in the range, defined by [first, last), to an
    /// uninitialized memory area beginning at \a dest. If an exception is
    /// thrown during the copy operation, the function has no effects.
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
    ///                     This iterator type must meet the requirements of a
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
    /// The assignments in the parallel \a uninitialized_copy algorithm invoked
    /// with an execution policy object of type \a sequenced_policy
    /// execute in sequential order in the calling thread.
    ///
    /// The assignments in the parallel \a uninitialized_copy algorithm invoked
    /// with an execution policy object of type \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an
    /// unordered fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a uninitialized_copy algorithm returns a
    ///           \a pika::future<FwdIter2>, if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter2 otherwise.
    ///           The \a uninitialized_copy algorithm returns the output
    ///           iterator to the element in the destination range, one past
    ///           the last element copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename FwdIter2>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter2>::type
    uninitialized_copy(
        ExPolicy&& policy, FwdIter1 first, FwdIter1 last, FwdIter2 dest);

    /// Copies the elements in the range [first, first + count), starting from
    /// first and proceeding to first + count - 1., to another range beginning
    /// at dest. If an exception is thrown during the copy operation, the
    /// function has no effects.
    ///
    /// \note   Complexity: Performs exactly \a count assignments, if
    ///         count > 0, no assignments otherwise.
    ///
    /// \tparam FwdIter1      The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam Size        The type of the argument specifying the number of
    ///                     elements to apply \a f to.
    /// \tparam FwdIter2     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of a
    ///                     forward iterator.
    ///
    /// \param first        Refers to the beginning of the sequence of elements
    ///                     the algorithm will be applied to.
    /// \param count        Refers to the number of elements starting at
    ///                     \a first the algorithm will be applied to.
    /// \param dest         Refers to the beginning of the destination range.
    ///
    /// The assignments in the parallel \a uninitialized_copy_n algorithm
    /// invoked without an execution policy object execute in sequential order
    /// in the calling thread.
    ///
    /// \returns  The \a uninitialized_copy_n algorithm returns a
    ///           returns \a FwdIter2.
    ///           The \a uninitialized_copy_n algorithm returns the output
    ///           iterator to the element in the destination range, one past
    ///           the last element copied.
    ///
    template <typename InIter, typename Size, typename FwdIter>
    FwdIter uninitialized_copy_n(InIter first, Size count, FwdIter dest);

    /// Copies the elements in the range [first, first + count), starting from
    /// first and proceeding to first + count - 1., to another range beginning
    /// at dest. If an exception is thrown during the copy operation, the
    /// function has no effects.
    ///
    /// \note   Complexity: Performs exactly \a count assignments, if
    ///         count > 0, no assignments otherwise.
    ///
    /// \tparam ExPolicy    The type of the execution policy to use (deduced).
    ///                     It describes the manner in which the execution
    ///                     of the algorithm may be parallelized and the manner
    ///                     in which it executes the assignments.
    /// \tparam FwdIter1      The type of the source iterators used (deduced).
    ///                     This iterator type must meet the requirements of an
    ///                     input iterator.
    /// \tparam Size        The type of the argument specifying the number of
    ///                     elements to apply \a f to.
    /// \tparam FwdIter2     The type of the iterator representing the
    ///                     destination range (deduced).
    ///                     This iterator type must meet the requirements of a
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
    /// The assignments in the parallel \a uninitialized_copy_n algorithm
    /// invoked with an execution policy object of type
    /// \a sequenced_policy execute in sequential order in the
    /// calling thread.
    ///
    /// The assignments in the parallel \a uninitialized_copy_n algorithm
    /// invoked with an execution policy object of type
    /// \a parallel_policy or
    /// \a parallel_task_policy are permitted to execute in an
    /// unordered fashion in unspecified threads, and indeterminately sequenced
    /// within each thread.
    ///
    /// \returns  The \a uninitialized_copy_n algorithm returns a
    ///           \a pika::future<FwdIter2> if the execution policy is of type
    ///           \a sequenced_task_policy or
    ///           \a parallel_task_policy and
    ///           returns \a FwdIter2 otherwise.
    ///           The \a uninitialized_copy_n algorithm returns the output
    ///           iterator to the element in the destination range, one past
    ///           the last element copied.
    ///
    template <typename ExPolicy, typename FwdIter1, typename Size,
        typename FwdIter2>
    typename pika::parallel::detail::algorithm_result<ExPolicy, FwdIter2>::type
    uninitialized_copy_n(
        ExPolicy&& policy, FwdIter1 first, Size count, FwdIter2 dest);
}    // namespace pika

#else    // DOXYGEN

#include <pika/config.hpp>
#include <pika/concepts/concepts.hpp>
#include <pika/functional/detail/tag_fallback_invoke.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>

#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/algorithms/detail/dispatch.hpp>
#include <pika/parallel/algorithms/detail/distance.hpp>
#include <pika/parallel/algorithms/detail/is_negative.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/detail/sender_util.hpp>
#include <pika/parallel/util/loop.hpp>
#include <pika/parallel/util/partitioner_with_cleanup.hpp>
#include <pika/parallel/util/result_types.hpp>
#include <pika/parallel/util/zip_iterator.hpp>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace pika::parallel::detail {
    ///////////////////////////////////////////////////////////////////////////
    // uninitialized_copy
    /// \cond NOINTERNAL
    ///////////////////////////////////////////////////////////////////////
    template <typename InIter1, typename FwdIter2, typename Cond>
    in_out_result<InIter1, FwdIter2>
    sequential_uninitialized_copy(InIter1 first, FwdIter2 dest, Cond cond)
    {
        using value_type = typename std::iterator_traits<FwdIter2>::value_type;

        FwdIter2 current = dest;
        try
        {
            for (/* */; cond(first, current); (void) ++first, ++current)
            {
                ::new (std::addressof(*current)) value_type(*first);
            }
            return in_out_result<InIter1, FwdIter2>{first, current};
        }
        catch (...)
        {
            for (/* */; dest != current; ++dest)
            {
                (*dest).~value_type();
            }
            throw;
        }
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename InIter1, typename InIter2>
    in_out_result<InIter1, InIter2>
    sequential_uninitialized_copy_n(InIter1 first, std::size_t count,
        InIter2 dest, util::cancellation_token<util::detail::no_data>& tok)
    {
        using value_type = typename std::iterator_traits<InIter2>::value_type;

        return {std::next(first, count),
            loop_with_cleanup_n_with_token(
                first, count, dest, tok,
                [](InIter1 it, InIter2 dest) -> void {
                    ::new (std::addressof(*dest)) value_type(*it);
                },
                [](InIter2 dest) -> void { (*dest).~value_type(); })};
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename Iter, typename FwdIter2>
    typename algorithm_result<ExPolicy, in_out_result<Iter, FwdIter2>>::type
    parallel_sequential_uninitialized_copy_n(
        ExPolicy&& policy, Iter first, std::size_t count, FwdIter2 dest)
    {
        if (count == 0)
        {
            return algorithm_result<ExPolicy, in_out_result<Iter, FwdIter2>>::
                get(in_out_result<Iter, FwdIter2>{first, dest});
        }

        using zip_iterator = pika::util::zip_iterator<Iter, FwdIter2>;
        using partition_result_type = std::pair<FwdIter2, FwdIter2>;
        using value_type = typename std::iterator_traits<FwdIter2>::value_type;

        util::cancellation_token<util::detail::no_data> tok;

        return partitioner_with_cleanup<ExPolicy, in_out_result<Iter, FwdIter2>,
            partition_result_type>::
            call(
                PIKA_FORWARD(ExPolicy, policy),
                pika::util::make_zip_iterator(first, dest), count,
                [tok](zip_iterator t,
                    std::size_t part_size) mutable -> partition_result_type {
                    using std::get;
                    auto iters = t.get_iterator_tuple();
                    FwdIter2 dest = get<1>(iters);
                    return std::make_pair(dest,
                        get_second_element(sequential_uninitialized_copy_n(
                            get<0>(iters), part_size, dest, tok)));
                },
                // finalize, called once if no error occurred
                [dest, first, count](
                    std::vector<pika::future<partition_result_type>>&&
                        data) mutable -> in_out_result<Iter, FwdIter2> {
                    // make sure iterators embedded in function object that is
                    // attached to futures are invalidated
                    data.clear();

                    std::advance(first, count);
                    std::advance(dest, count);
                    return in_out_result<Iter, FwdIter2>{first, dest};
                },
                // cleanup function, called for each partition which
                // didn't fail, but only if at least one failed
                [](partition_result_type&& r) -> void {
                    while (r.first != r.second)
                    {
                        (*r.first).~value_type();
                        ++r.first;
                    }
                });
    }

    ///////////////////////////////////////////////////////////////////////
    template <typename IterPair>
    struct uninitialized_copy
      : public algorithm<uninitialized_copy<IterPair>, IterPair>
    {
        uninitialized_copy()
          : uninitialized_copy::algorithm("uninitialized_copy")
        {
        }

        template <typename ExPolicy, typename InIter1, typename Sent,
            typename FwdIter2>
        static in_out_result<InIter1, FwdIter2>
        sequential(ExPolicy, InIter1 first, Sent last, FwdIter2 dest)
        {
            return sequential_uninitialized_copy(
                first, dest, [last](InIter1 first, FwdIter2) -> bool {
                    return first != last;
                });
        }

        template <typename ExPolicy, typename Iter, typename Sent,
            typename FwdIter2>
        static typename algorithm_result<ExPolicy,
            in_out_result<Iter, FwdIter2>>::type
        parallel(ExPolicy&& policy, Iter first, Sent last, FwdIter2 dest)
        {
            return parallel_sequential_uninitialized_copy_n(
                PIKA_FORWARD(ExPolicy, policy), first,
                detail::distance(first, last), dest);
        }
    };
    /// \endcond

    /////////////////////////////////////////////////////////////////////////////
    // uninitialized_copy_sent
    /// \cond NOINTERNAL
    template <typename IterPair>
    struct uninitialized_copy_sent
      : public algorithm<uninitialized_copy_sent<IterPair>, IterPair>
    {
        uninitialized_copy_sent()
          : uninitialized_copy_sent::algorithm("uninitialized_copy_sent")
        {
        }

        template <typename ExPolicy, typename InIter1, typename Sent1,
            typename FwdIter2, typename Sent2>
        static in_out_result<InIter1, FwdIter2> sequential(
            ExPolicy, InIter1 first, Sent1 last, FwdIter2 dest, Sent2 last_d)
        {
            return sequential_uninitialized_copy(first, dest,
                [last, last_d](InIter1 first, FwdIter2 current) -> bool {
                    return !(first == last || current == last_d);
                });
        }

        template <typename ExPolicy, typename Iter, typename Sent1,
            typename FwdIter2, typename Sent2>
        static typename algorithm_result<ExPolicy,
            in_out_result<Iter, FwdIter2>>::type
        parallel(ExPolicy&& policy, Iter first, Sent1 last, FwdIter2 dest,
            Sent2 last_d)
        {
            std::size_t dist1 = detail::distance(first, last);
            std::size_t dist2 = detail::distance(dest, last_d);
            std::size_t dist = dist1 <= dist2 ? dist1 : dist2;

            return parallel_sequential_uninitialized_copy_n(
                PIKA_FORWARD(ExPolicy, policy), first, dist, dest);
        }
    };
    /// \endcond

    /////////////////////////////////////////////////////////////////////////////
    // uninitialized_copy_n
    /// \cond NOINTERNAL
    template <typename IterPair>
    struct uninitialized_copy_n
      : public algorithm<uninitialized_copy_n<IterPair>, IterPair>
    {
        uninitialized_copy_n()
          : uninitialized_copy_n::algorithm("uninitialized_copy_n")
        {
        }

        template <typename ExPolicy, typename InIter, typename FwdIter2>
        static in_out_result<InIter, FwdIter2>
        sequential(ExPolicy, InIter first, std::size_t count, FwdIter2 dest)
        {
            using value_type =
                typename std::iterator_traits<FwdIter2>::value_type;

            FwdIter2 current = dest;
            try
            {
                for (/* */; count > 0; ++first, (void) ++current, --count)
                {
                    ::new (std::addressof(*current)) value_type(*first);
                }
                return in_out_result<InIter, FwdIter2>{first, current};
            }
            catch (...)
            {
                for (/* */; dest != current; ++dest)
                {
                    (*dest).~value_type();
                }
                throw;
            }
        }

        template <typename ExPolicy, typename Iter, typename FwdIter2>
        static typename algorithm_result<ExPolicy,
            in_out_result<Iter, FwdIter2>>::type
        parallel(
            ExPolicy&& policy, Iter first, std::size_t count, FwdIter2 dest)
        {
            return parallel_sequential_uninitialized_copy_n(
                PIKA_FORWARD(ExPolicy, policy), first, count, dest);
        }
    };
    /// \endcond
}    // namespace pika::parallel::detail

namespace pika {
    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::uninitialized_copy
    inline constexpr struct uninitialized_copy_t final
      : pika::detail::tag_parallel_algorithm<uninitialized_copy_t>
    {
        // clang-format off
        template <typename InIter, typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value &&
                pika::traits::is_forward_iterator<FwdIter>::value
            )>
        // clang-format on
        friend FwdIter tag_fallback_invoke(
            pika::uninitialized_copy_t, InIter first, InIter last, FwdIter dest)
        {
            static_assert(pika::traits::is_input_iterator<InIter>::value,
                "Required at least input iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::get_second_element(
                pika::parallel::detail::uninitialized_copy<
                    parallel::detail::in_out_result<InIter, FwdIter>>()
                    .call(pika::execution::seq, first, last, dest));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_forward_iterator<FwdIter1>::value &&
                pika::traits::is_forward_iterator<FwdIter2>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::uninitialized_copy_t, ExPolicy&& policy,
            FwdIter1 first, FwdIter1 last, FwdIter2 dest)
        {
            static_assert(pika::traits::is_forward_iterator<FwdIter1>::value,
                "Requires at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter2>::value,
                "Requires at least forward iterator.");

            return pika::parallel::detail::get_second_element(
                pika::parallel::detail::uninitialized_copy<
                    parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(PIKA_FORWARD(ExPolicy, policy), first, last, dest));
        }

    } uninitialized_copy{};

    ///////////////////////////////////////////////////////////////////////////
    // CPO for pika::uninitialized_copy_n
    inline constexpr struct uninitialized_copy_n_t final
      : pika::detail::tag_parallel_algorithm<uninitialized_copy_n_t>
    {
        // clang-format off
        template <typename InIter, typename Size,
            typename FwdIter,
            PIKA_CONCEPT_REQUIRES_(
                pika::traits::is_iterator<InIter>::value &&
                pika::traits::is_forward_iterator<FwdIter>::value
            )>
        // clang-format on
        friend FwdIter tag_fallback_invoke(pika::uninitialized_copy_n_t,
            InIter first, Size count, FwdIter dest)
        {
            static_assert(pika::traits::is_input_iterator<InIter>::value,
                "Required at least input iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter>::value,
                "Requires at least forward iterator.");

            // if count is representing a negative value, we do nothing
            if (pika::parallel::detail::is_negative(count))
            {
                return dest;
            }

            return pika::parallel::detail::get_second_element(
                pika::parallel::detail::uninitialized_copy_n<
                    parallel::detail::in_out_result<InIter, FwdIter>>()
                    .call(
                        pika::execution::seq, first, std::size_t(count), dest));
        }

        // clang-format off
        template <typename ExPolicy, typename FwdIter1, typename Size, typename FwdIter2,
            PIKA_CONCEPT_REQUIRES_(
                pika::is_execution_policy<ExPolicy>::value &&
                pika::traits::is_forward_iterator<FwdIter1>::value &&
                pika::traits::is_forward_iterator<FwdIter2>::value
            )>
        // clang-format on
        friend typename pika::parallel::detail::algorithm_result<ExPolicy,
            FwdIter2>::type
        tag_fallback_invoke(pika::uninitialized_copy_n_t, ExPolicy&& policy,
            FwdIter1 first, Size count, FwdIter2 dest)
        {
            static_assert(pika::traits::is_forward_iterator<FwdIter1>::value,
                "Requires at least forward iterator.");
            static_assert(pika::traits::is_forward_iterator<FwdIter2>::value,
                "Requires at least forward iterator.");

            // if count is representing a negative value, we do nothing
            if (pika::parallel::detail::is_negative(count))
            {
                return pika::parallel::detail::algorithm_result<ExPolicy,
                    FwdIter2>::get(PIKA_MOVE(dest));
            }

            return pika::parallel::detail::get_second_element(
                pika::parallel::detail::uninitialized_copy_n<
                    parallel::detail::in_out_result<FwdIter1, FwdIter2>>()
                    .call(PIKA_FORWARD(ExPolicy, policy), first,
                        std::size_t(count), dest));
        }

    } uninitialized_copy_n{};
}    // namespace pika

#endif    // DOXYGEN
