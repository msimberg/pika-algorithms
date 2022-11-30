//  Copyright (c) 2007-2018 Hartmut Kaiser
//  Copyright (c)      2015 Daniel Bourgeois
//  Copyright (c)      2017 Taeguk Kwon
//  Copyright (c)      2021 Akhil J Nair
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/assert.hpp>
#include <pika/async_combinators/wait_all.hpp>
#include <pika/modules/errors.hpp>
#if !defined(PIKA_COMPUTE_DEVICE_CODE)
#include <pika/async/dataflow.hpp>
#endif

#include <pika/execution/executors/execution.hpp>
#include <pika/execution/executors/execution_parameters.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/parallel/util/detail/algorithm_result.hpp>
#include <pika/parallel/util/detail/chunk_size.hpp>
#include <pika/parallel/util/detail/handle_local_exceptions.hpp>
#include <pika/parallel/util/detail/scoped_executor_parameters.hpp>
#include <pika/parallel/util/detail/select_partitioner.hpp>

#include <algorithm>
#include <cstddef>
#include <exception>
#include <list>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace pika::parallel::detail {
    struct scan_partitioner_normal_tag
    {
    };
    struct scan_partitioner_sequential_f3_tag
    {
    };

    ///////////////////////////////////////////////////////////////////////
    // The static partitioner simply spawns one chunk of iterations for
    // each available core.
    template <typename ExPolicy, typename ScanPartTag, typename R,
        typename Result1, typename Result2>
    struct scan_static_partitioner
    {
        using parameters_type = typename ExPolicy::executor_parameters_type;
        using executor_type = typename ExPolicy::executor_type;

        using scoped_parameters =
            scoped_executor_parameters_ref<parameters_type, executor_type>;

        using handle_exceptions = handle_local_exceptions<ExPolicy>;

        template <typename ExPolicy_, typename FwdIter, typename T, typename F1,
            typename F2, typename F3, typename F4>
        static R
        call(scan_partitioner_normal_tag, ExPolicy_ policy, FwdIter first,
            std::size_t count, T&& init, F1&& f1, F2&& f2, F3&& f3, F4&& f4)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(policy);
            PIKA_UNUSED(first);
            PIKA_UNUSED(count);
            PIKA_UNUSED(init);
            PIKA_UNUSED(f1);
            PIKA_UNUSED(f2);
            PIKA_UNUSED(f3);
            PIKA_UNUSED(f4);
            PIKA_ASSERT(false);
            return R();
#else
            // inform parameter traits
            scoped_parameters scoped_params(
                policy.parameters(), policy.executor());

            std::vector<pika::shared_future<Result1>> workitems;
            std::vector<pika::future<Result2>> finalitems;
            std::vector<Result1> f2results;
            std::list<std::exception_ptr> errors;
            try
            {
                // pre-initialize first intermediate result
                workitems.push_back(make_ready_future(PIKA_FORWARD(T, init)));

                PIKA_ASSERT(count > 0);
                FwdIter first_ = first;
                std::size_t count_ = count;

                // estimate a chunk size based on number of cores used
                using has_variable_chunk_size =
                    typename execution::extract_has_variable_chunk_size<
                        parameters_type>::type;

                auto shape = get_bulk_iteration_shape(has_variable_chunk_size(),
                    policy, workitems, f1, first, count, 1);

                // schedule every chunk on a separate thread
                std::size_t size = pika::util::size(shape);

                // If the size of count was enough to warrant testing for a
                // chunk, pre-initialize second intermediate result and
                // start f3.
                if (workitems.size() == 2)
                {
                    PIKA_ASSERT(count_ > count);

                    workitems.reserve(size + 2);
                    finalitems.reserve(size + 1);

                    finalitems.push_back(
                        execution::async_execute(policy.executor(), f3, first_,
                            count_ - count, workitems[0].get()));

                    workitems[1] = make_ready_future(PIKA_INVOKE(
                        f2, workitems[0].get(), workitems[1].get()));
                }
                else
                {
                    workitems.reserve(size + 1);
                    finalitems.reserve(size);
                }

                // Schedule first step of scan algorithm, step 2 is
                // performed when all f1 tasks are done
                for (auto const& elem : shape)
                {
                    FwdIter it = std::get<0>(elem);
                    std::size_t size = std::get<1>(elem);

                    auto curr = execution::async_execute(
                        policy.executor(), f1, it, size)
                                    .share();

                    workitems.push_back(curr);
                }

                // Wait for all f1 tasks to finish
                pika::wait_all_nothrow(workitems);

                // perform f2 sequentially in one go
                f2results.resize(workitems.size());
                auto result = workitems[0].get();
                f2results[0] = result;
                for (std::size_t i = 1; i < workitems.size(); i++)
                {
                    result = PIKA_INVOKE(f2, result, workitems[i].get());
                    f2results[i] = result;
                }

                // start all f3 tasks
                std::size_t i = 0;
                for (auto const& elem : shape)
                {
                    finalitems.push_back(execution::async_execute(
                        policy.executor(), f3, std::get<0>(elem),
                        std::get<1>(elem), f2results[i]));
                    i++;
                }

                scoped_params.mark_end_of_scheduling();
            }
            catch (...)
            {
                handle_exceptions::call(std::current_exception(), errors);
            }
            return reduce(PIKA_MOVE(f2results), PIKA_MOVE(finalitems),
                PIKA_MOVE(errors), PIKA_FORWARD(F4, f4));
#endif
        }

        template <typename ExPolicy_, typename FwdIter, typename T, typename F1,
            typename F2, typename F3, typename F4>
        static R call(scan_partitioner_sequential_f3_tag, ExPolicy_ policy,
            FwdIter first, std::size_t count, T&& init, F1&& f1, F2&& f2,
            F3&& f3, F4&& f4)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(policy);
            PIKA_UNUSED(first);
            PIKA_UNUSED(count);
            PIKA_UNUSED(init);
            PIKA_UNUSED(f1);
            PIKA_UNUSED(f2);
            PIKA_UNUSED(f3);
            PIKA_UNUSED(f4);
            PIKA_ASSERT(false);
            return R();
#else
            // inform parameter traits
            scoped_parameters scoped_params(
                policy.parameters(), policy.executor());

            std::vector<pika::shared_future<Result1>> workitems;
            std::vector<pika::future<Result2>> finalitems;
            std::list<std::exception_ptr> errors;
            try
            {
                // pre-initialize first intermediate result
                workitems.push_back(make_ready_future(PIKA_FORWARD(T, init)));

                PIKA_ASSERT(count > 0);
                FwdIter first_ = first;
                std::size_t count_ = count;
                bool tested = false;

                // estimate a chunk size based on number of cores used
                using has_variable_chunk_size =
                    typename execution::extract_has_variable_chunk_size<
                        parameters_type>::type;

                auto shape = get_bulk_iteration_shape(has_variable_chunk_size(),
                    policy, workitems, f1, first, count, 1);

                // schedule every chunk on a separate thread
                std::size_t size = pika::util::size(shape);

                // If the size of count was enough to warrant testing for a
                // chunk, pre-initialize second intermediate result.
                if (workitems.size() == 2)
                {
                    workitems.reserve(size + 2);
                    finalitems.reserve(size + 1);

                    pika::shared_future<Result1> curr = workitems[1];
                    workitems[1] =
                        dataflow(pika::launch::sync, f2, workitems[0], curr);
                    tested = true;
                }
                else
                {
                    workitems.reserve(size + 1);
                    finalitems.reserve(size);
                }

                // Schedule first step of scan algorithm, step 2 is
                // performed as soon as the current partition and the
                // partition to the left is ready.
                for (auto const& elem : shape)
                {
                    FwdIter it = std::get<0>(elem);
                    std::size_t size = std::get<1>(elem);

                    pika::shared_future<Result1> prev = workitems.back();
                    auto curr = execution::async_execute(
                        policy.executor(), f1, it, size)
                                    .share();

                    workitems.push_back(
                        dataflow(pika::launch::sync, f2, prev, curr));
                }

                // In the code below, performs step 3 sequentially.
                auto shape_iter = std::begin(shape);

                // First, perform f3 of the first partition.
                if (tested)
                {
                    PIKA_ASSERT(count_ > count);

                    finalitems.push_back(dataflow(pika::launch::sync, f3,
                        first_, count_ - count, workitems[0], workitems[1]));
                }
                else
                {
                    auto elem = *shape_iter++;
                    FwdIter it = std::get<0>(elem);
                    std::size_t size = std::get<1>(elem);

                    finalitems.push_back(dataflow(pika::launch::sync, f3, it,
                        size, workitems[0], workitems[1]));
                }

                PIKA_ASSERT(finalitems.size() >= 1);

                // Perform f3 sequentially from the second to the end
                // of partitions.
                for (std::size_t widx = 1ul; shape_iter != std::end(shape);
                     ++shape_iter, ++widx)
                {
                    auto elem = *shape_iter;
                    FwdIter it = std::get<0>(elem);
                    std::size_t size = std::get<1>(elem);

                    // Wait the completion of f3 on previous partition.
                    finalitems.back().wait();

                    finalitems.push_back(dataflow(pika::launch::sync, f3, it,
                        size, workitems[widx], workitems[widx + 1]));
                }

                scoped_params.mark_end_of_scheduling();
            }
            catch (...)
            {
                handle_exceptions::call(std::current_exception(), errors);
            }
            return reduce(PIKA_MOVE(workitems), PIKA_MOVE(finalitems),
                PIKA_MOVE(errors), PIKA_FORWARD(F4, f4));
#endif
        }

        template <typename ExPolicy_, typename FwdIter, typename T, typename F1,
            typename F2, typename F3, typename F4>
        static R call(ExPolicy_&& policy, FwdIter first, std::size_t count,
            T&& init, F1&& f1, F2&& f2, F3&& f3, F4&& f4)
        {
            return call(ScanPartTag{}, PIKA_FORWARD(ExPolicy_, policy), first,
                count, PIKA_FORWARD(T, init), PIKA_FORWARD(F1, f1),
                PIKA_FORWARD(F2, f2), PIKA_FORWARD(F3, f3),
                PIKA_FORWARD(F4, f4));
        }

    private:
        template <typename F>
        static R reduce(std::vector<pika::shared_future<Result1>>&& workitems,
            std::vector<pika::future<Result2>>&& finalitems,
            std::list<std::exception_ptr>&& errors, F&& f)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_UNUSED(finalitems);
            PIKA_UNUSED(errors);
            PIKA_UNUSED(f);
            PIKA_ASSERT(false);
            return R();
#else
            // wait for all tasks to finish
            pika::wait_all_nothrow(workitems, finalitems);

            // always rethrow if 'errors' is not empty or 'workitems' or
            // 'finalitems' have an exceptional future
            handle_exceptions::call(workitems, errors);
            handle_exceptions::call(finalitems, errors);

            try
            {
                return f(PIKA_MOVE(workitems), PIKA_MOVE(finalitems));
            }
            catch (...)
            {
                // rethrow either bad_alloc or exception_list
                handle_exceptions::call(std::current_exception());
            }
#endif
        }

        template <typename F>
        static R reduce(std::vector<Result1>&& workitems,
            std::vector<pika::future<Result2>>&& finalitems,
            std::list<std::exception_ptr>&& errors, F&& f)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_UNUSED(finalitems);
            PIKA_UNUSED(errors);
            PIKA_UNUSED(f);
            PIKA_ASSERT(false);
            return R();
#else
            // wait for all tasks to finish
            pika::wait_all_nothrow(finalitems);

            // always rethrow if 'errors' is not empty or
            // 'finalitems' have an exceptional future
            handle_exceptions::call(finalitems, errors);

            try
            {
                return f(PIKA_MOVE(workitems), PIKA_MOVE(finalitems));
            }
            catch (...)
            {
                // rethrow either bad_alloc or exception_list
                handle_exceptions::call(std::current_exception());
            }
#endif
        }
    };

    ///////////////////////////////////////////////////////////////////////
    template <typename ExPolicy, typename ScanPartTag, typename R,
        typename Result1, typename Result2>
    struct scan_task_static_partitioner
    {
        template <typename ExPolicy_, typename FwdIter, typename T, typename F1,
            typename F2, typename F3, typename F4>
        static pika::future<R> call(ExPolicy_&& policy, FwdIter first,
            std::size_t count, T&& init, F1&& f1, F2&& f2, F3&& f3, F4&& f4)
        {
            return execution::async_execute(policy.executor(),
                [first, count, policy = PIKA_FORWARD(ExPolicy_, policy),
                    init = PIKA_FORWARD(T, init), f1 = PIKA_FORWARD(F1, f1),
                    f2 = PIKA_FORWARD(F2, f2), f3 = PIKA_FORWARD(F3, f3),
                    f4 = PIKA_FORWARD(F4, f4)]() mutable -> R {
                    using partitioner_type = scan_static_partitioner<ExPolicy,
                        ScanPartTag, R, Result1, Result2>;
                    return partitioner_type::call(ScanPartTag{},
                        PIKA_FORWARD(ExPolicy_, policy), first, count,
                        PIKA_MOVE(init), f1, f2, f3, f4);
                });
        }
    };

    ///////////////////////////////////////////////////////////////////////////
    // ExPolicy:    execution policy
    // R:           overall result type
    // Result1:     intermediate result type of first and second step
    // Result2:     intermediate result of the third step
    // ScanPartTag: select appropriate policy of scan partitioner
    template <typename ExPolicy, typename R = void, typename Result1 = R,
        typename Result2 = void,
        typename ScanPartTag = scan_partitioner_normal_tag>
    struct scan_partitioner
      : select_partitioner<std::decay_t<ExPolicy>, scan_static_partitioner,
            scan_task_static_partitioner>::template apply<ScanPartTag, R,
            Result1, Result2>
    {
    };
}    // namespace pika::parallel::detail
