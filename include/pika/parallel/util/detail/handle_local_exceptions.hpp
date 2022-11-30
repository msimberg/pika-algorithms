//  Copyright (c) 2007-2016 Hartmut Kaiser
//  Copyright (c)      2017 Taeguk Kwon
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/config.hpp>
#include <pika/assert.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/modules/async.hpp>
#include <pika/modules/errors.hpp>

#include <exception>
#include <list>
#include <utility>
#include <vector>

namespace pika::parallel::detail {
    template <typename ExPolicy>
    struct handle_local_exceptions
    {
        ///////////////////////////////////////////////////////////////////////
        // std::bad_alloc has to be handled separately
#if defined(PIKA_COMPUTE_DEVICE_CODE)
        static void call(std::exception_ptr const&)
        {
            PIKA_ASSERT(false);
        }
#else
        [[noreturn]] static void call(std::exception_ptr const& e)
        {
            try
            {
                std::rethrow_exception(e);
            }
            catch (std::bad_alloc const& ba)
            {
                throw ba;
            }
            catch (...)
            {
                throw exception_list(e);
            }
        }
#endif

        static void call(
            std::exception_ptr const& e, std::list<std::exception_ptr>& errors)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(e);
            PIKA_UNUSED(errors);
            PIKA_ASSERT(false);
#else
            try
            {
                std::rethrow_exception(e);
            }
            catch (std::bad_alloc const& ba)
            {
                throw ba;
            }
            catch (...)
            {
                errors.push_back(e);
            }
#endif
        }

        template <typename T>
        static void call(std::vector<pika::future<T>> const& workitems,
            std::list<std::exception_ptr>& errors, bool throw_errors = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_UNUSED(errors);
            PIKA_UNUSED(throw_errors);
            PIKA_ASSERT(false);
#else
            if (workitems.empty() && errors.empty())
            {
                return;
            }

            // first extract exception from all futures
            std::vector<std::exception_ptr> exceptions;
            exceptions.reserve(workitems.size());
            for (auto const& f : workitems)
            {
                if (f.has_exception())
                {
                    exceptions.reserve(workitems.size());
                    exceptions.push_back(f.get_exception_ptr());
                }
            }

            // now handle exceptions as needed
            for (auto& e : exceptions)
            {
                call(e, errors);
            }

            if (throw_errors && !errors.empty())
            {
                throw exception_list(PIKA_MOVE(errors));
            }
#endif
        }

        ///////////////////////////////////////////////////////////////////////
        template <typename T>
        static void call(std::vector<pika::shared_future<T>> const& workitems,
            std::list<std::exception_ptr>& errors, bool throw_errors = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_UNUSED(errors);
            PIKA_UNUSED(throw_errors);
            PIKA_ASSERT(false);
#else
            if (workitems.empty() && errors.empty())
            {
                return;
            }

            // first extract exception from all futures
            std::vector<std::exception_ptr> exceptions;
            exceptions.reserve(workitems.size());
            for (auto const& f : workitems)
            {
                if (f.has_exception())
                {
                    exceptions.reserve(workitems.size());
                    exceptions.push_back(f.get_exception_ptr());
                }
            }

            // now handle exceptions as needed
            for (auto& e : exceptions)
            {
                call(e, errors);
            }

            if (throw_errors && !errors.empty())
            {
                throw exception_list(PIKA_MOVE(errors));
            }
#endif
        }

        template <typename T, typename Cleanup>
        static void call_with_cleanup(std::vector<pika::future<T>>& workitems,
            std::list<std::exception_ptr>& errors, Cleanup&& cleanup,
            bool throw_errors = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_UNUSED(errors);
            PIKA_UNUSED(cleanup);
            PIKA_UNUSED(throw_errors);
            PIKA_ASSERT(false);
#else
            if (workitems.empty() && errors.empty())
            {
                return;
            }

            bool has_exception = false;
            std::exception_ptr bad_alloc_exception;
            for (auto& f : workitems)
            {
                if (f.has_exception())
                {
                    std::exception_ptr e = f.get_exception_ptr();
                    try
                    {
                        std::rethrow_exception(e);
                    }
                    catch (std::bad_alloc const&)
                    {
                        bad_alloc_exception = e;
                    }
                    catch (...)
                    {
                        errors.push_back(e);
                    }
                    has_exception = true;
                }
            }

            // If at least one partition failed with an exception, call
            // the cleanup function for all others (the failed partitioned
            // are assumed to have already run the cleanup).
            if (has_exception)
            {
                for (auto& f : workitems)
                {
                    if (!f.has_exception())
                    {
                        cleanup(f.get());
                    }
                }
            }

            if (bad_alloc_exception)
            {
                std::rethrow_exception(bad_alloc_exception);
            }

            if (throw_errors && !errors.empty())
            {
                throw exception_list(PIKA_MOVE(errors));
            }
#endif
        }
    };

    template <>
    struct handle_local_exceptions<pika::execution::parallel_unsequenced_policy>
    {
        ///////////////////////////////////////////////////////////////////////
#if defined(PIKA_COMPUTE_DEVICE_CODE)
        static void call(std::exception_ptr const&)
        {
            PIKA_ASSERT(false);
        }
#else
        [[noreturn]] static void call(std::exception_ptr const&)
        {
            std::terminate();
        }
#endif

#if defined(PIKA_COMPUTE_DEVICE_CODE)
        static void call(
            std::exception_ptr const&, std::list<std::exception_ptr>&)
        {
            PIKA_ASSERT(false);
        }
#else
        [[noreturn]] static void call(
            std::exception_ptr const&, std::list<std::exception_ptr>&)
        {
            std::terminate();
        }
#endif

        template <typename T>
        static void call(std::vector<pika::future<T>> const& workitems,
            std::list<std::exception_ptr>&, bool = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_ASSERT(false);
#else
            for (auto const& f : workitems)
            {
                if (f.has_exception())
                {
                    std::terminate();
                }
            }
#endif
        }

        template <typename T>
        static void call(std::vector<pika::shared_future<T>> const& workitems,
            std::list<std::exception_ptr>&, bool = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_ASSERT(false);
#else
            for (auto const& f : workitems)
            {
                if (f.has_exception())
                {
                    std::terminate();
                }
            }
#endif
        }

        template <typename T, typename Cleanup>
        static void
        call_with_cleanup(std::vector<pika::future<T>> const& workitems,
            std::list<std::exception_ptr>&, Cleanup&&, bool = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_ASSERT(false);
#else
            for (auto const& f : workitems)
            {
                if (f.has_exception())
                {
                    std::terminate();
                }
            }
#endif
        }
    };

    template <>
    struct handle_local_exceptions<pika::execution::unsequenced_policy>
    {
        ///////////////////////////////////////////////////////////////////////
#if defined(PIKA_COMPUTE_DEVICE_CODE)
        static void call(std::exception_ptr const&)
        {
            PIKA_ASSERT(false);
        }
#else
        [[noreturn]] static void call(std::exception_ptr const&)
        {
            std::terminate();
        }
#endif

#if defined(PIKA_COMPUTE_DEVICE_CODE)
        static void call(
            std::exception_ptr const&, std::list<std::exception_ptr>&)
        {
            PIKA_ASSERT(false);
        }
#else
        [[noreturn]] static void call(
            std::exception_ptr const&, std::list<std::exception_ptr>&)
        {
            std::terminate();
        }
#endif

        template <typename T>
        static void call(std::vector<pika::future<T>> const& workitems,
            std::list<std::exception_ptr>&, bool = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_ASSERT(false);
#else
            for (auto const& f : workitems)
            {
                if (f.has_exception())
                {
                    std::terminate();
                }
            }
#endif
        }

        template <typename T>
        static void call(std::vector<pika::shared_future<T>> const& workitems,
            std::list<std::exception_ptr>&, bool = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_ASSERT(false);
#else
            for (auto const& f : workitems)
            {
                if (f.has_exception())
                {
                    std::terminate();
                }
            }
#endif
        }

        template <typename T, typename Cleanup>
        static void
        call_with_cleanup(std::vector<pika::future<T>> const& workitems,
            std::list<std::exception_ptr>&, Cleanup&&, bool = true)
        {
#if defined(PIKA_COMPUTE_DEVICE_CODE)
            PIKA_UNUSED(workitems);
            PIKA_ASSERT(false);
#else
            for (auto const& f : workitems)
            {
                if (f.has_exception())
                {
                    std::terminate();
                }
            }
#endif
        }
    };
}    // namespace pika::parallel::detail
