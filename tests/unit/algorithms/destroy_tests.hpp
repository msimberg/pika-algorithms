//  Copyright (c) 2014-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/execution.hpp>
#include <pika/parallel/algorithms/destroy.hpp>
#include <pika/testing.hpp>

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <utility>

#include "test_utils.hpp"

std::atomic<std::size_t> destruct_count(0);
unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);

struct destructable
{
    destructable()
      : value_(0)
    {
    }

    ~destructable()
    {
        ++destruct_count;
    }

    std::uint32_t value_;
};

std::size_t const data_size = 10007;

////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_destroy(IteratorTag)
{
    using base_iterator = destructable*;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    destructable* p =
        (destructable*) std::malloc(data_size * sizeof(destructable));

    // value-initialize data in array
    std::for_each(p, p + data_size, [](destructable& d) {
        ::new (static_cast<void*>(std::addressof(d))) destructable;
    });

    destruct_count.store(0);

    pika::destroy(iterator(p), iterator(p + data_size));

    PIKA_TEST_EQ(destruct_count.load(), data_size);

    std::free(p);
}

template <typename ExPolicy, typename IteratorTag>
void test_destroy(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = destructable*;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    destructable* p =
        (destructable*) std::malloc(data_size * sizeof(destructable));

    // value-initialize data in array
    std::for_each(p, p + data_size, [](destructable& d) {
        ::new (static_cast<void*>(std::addressof(d))) destructable;
    });

    destruct_count.store(0);

    pika::destroy(
        std::forward<ExPolicy>(policy), iterator(p), iterator(p + data_size));

    PIKA_TEST_EQ(destruct_count.load(), data_size);

    std::free(p);
}

template <typename ExPolicy, typename IteratorTag>
void test_destroy_async(ExPolicy&& policy, IteratorTag)
{
    using base_iterator = destructable*;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    destructable* p =
        (destructable*) std::malloc(data_size * sizeof(destructable));

    // value-initialize data in array
    std::for_each(p, p + data_size, [](destructable& d) {
        ::new (static_cast<void*>(std::addressof(d))) destructable;
    });

    destruct_count.store(0);

    auto f = pika::destroy(
        std::forward<ExPolicy>(policy), iterator(p), iterator(p + data_size));
    f.wait();

    PIKA_TEST_EQ(destruct_count.load(), data_size);

    std::free(p);
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_destroy_exception(IteratorTag)
{
    using data_type = test::count_instances_v<destructable>;
    using base_iterator = data_type*;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    data_type* p = (data_type*) std::malloc(data_size * sizeof(data_type));

    data_type::instance_count.store(0);
    data_type::max_instance_count.store(0);

    // value-initialize data in array
    std::for_each(p, p + data_size, [](data_type& d) {
        ::new (static_cast<void*>(std::addressof(d))) data_type;
    });

    PIKA_TEST_EQ(data_type::instance_count.load(), data_size);

    std::uniform_int_distribution<> dis(0, data_size - 1);
    std::atomic<std::size_t> throw_after(dis(gen));    //-V104
    std::int64_t throw_after_ = throw_after.load();

    bool caught_exception = false;
    try
    {
        pika::destroy(decorated_iterator(p,
                          [&throw_after]() {
                              if (throw_after-- == 0)
                                  throw std::runtime_error("test");
                          }),
            decorated_iterator(p + data_size));
        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<pika::execution::sequenced_policy,
            IteratorTag>::call(pika::execution::seq, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
    PIKA_TEST_LTE(data_type::instance_count.load(),
        std::size_t(data_size - throw_after_));

    std::free(p);
}

template <typename ExPolicy, typename IteratorTag>
void test_destroy_exception(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using data_type = test::count_instances_v<destructable>;
    using base_iterator = data_type*;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    data_type* p = (data_type*) std::malloc(data_size * sizeof(data_type));

    data_type::instance_count.store(0);
    data_type::max_instance_count.store(0);

    // value-initialize data in array
    std::for_each(p, p + data_size, [](data_type& d) {
        ::new (static_cast<void*>(std::addressof(d))) data_type;
    });

    PIKA_TEST_EQ(data_type::instance_count.load(), data_size);

    std::uniform_int_distribution<> dis(0, data_size - 1);
    std::atomic<std::size_t> throw_after(dis(gen));    //-V104
    std::int64_t throw_after_ = throw_after.load();

    bool caught_exception = false;
    try
    {
        pika::destroy(policy,
            decorated_iterator(p,
                [&throw_after]() {
                    if (throw_after-- == 0)
                        throw std::runtime_error("test");
                }),
            decorated_iterator(p + data_size));
        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
    PIKA_TEST_LTE(data_type::instance_count.load(),
        std::size_t(data_size - throw_after_));

    std::free(p);
}

template <typename ExPolicy, typename IteratorTag>
void test_destroy_exception_async(ExPolicy&& policy, IteratorTag)
{
    using data_type = test::count_instances_v<destructable>;
    using base_iterator = data_type*;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    data_type* p = (data_type*) std::malloc(data_size * sizeof(data_type));

    data_type::instance_count.store(0);
    data_type::max_instance_count.store(0);

    // value-initialize data in array
    std::for_each(p, p + data_size, [](data_type& d) {
        ::new (static_cast<void*>(std::addressof(d))) data_type;
    });

    PIKA_TEST_EQ(data_type::instance_count.load(), data_size);

    std::uniform_int_distribution<> dis(0, data_size - 1);
    std::atomic<std::size_t> throw_after(dis(gen));    //-V104
    std::int64_t throw_after_ = throw_after.load();

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::destroy(policy,
            decorated_iterator(p,
                [&throw_after]() {
                    if (throw_after-- == 0)
                        throw std::runtime_error("test");
                }),
            decorated_iterator(p + data_size));

        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
    PIKA_TEST(returned_from_algorithm);
    PIKA_TEST_LTE(data_type::instance_count.load(),
        std::size_t(data_size - throw_after_));

    std::free(p);
}

//////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_destroy_bad_alloc(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using data_type = test::count_instances_v<destructable>;
    using base_iterator = data_type*;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    data_type* p = (data_type*) std::malloc(data_size * sizeof(data_type));

    data_type::instance_count.store(0);
    data_type::max_instance_count.store(0);

    // value-initialize data in array
    std::for_each(p, p + data_size, [](data_type& d) {
        ::new (static_cast<void*>(std::addressof(d))) data_type;
    });

    PIKA_TEST_EQ(data_type::instance_count.load(), data_size);

    std::uniform_int_distribution<> dis(0, data_size - 1);
    std::atomic<std::size_t> throw_after(dis(gen));    //-V104
    std::int64_t throw_after_ = throw_after.load();

    bool caught_bad_alloc = false;
    try
    {
        pika::destroy(policy,
            decorated_iterator(p,
                [&throw_after]() {
                    if (throw_after-- == 0)
                        throw std::bad_alloc();
                }),
            decorated_iterator(p + data_size));

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_bad_alloc);
    PIKA_TEST_LTE(data_type::instance_count.load(),
        std::size_t(data_size - throw_after_));

    std::free(p);
}

template <typename ExPolicy, typename IteratorTag>
void test_destroy_bad_alloc_async(ExPolicy&& policy, IteratorTag)
{
    using data_type = test::count_instances_v<destructable>;
    using base_iterator = data_type*;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    data_type* p = (data_type*) std::malloc(data_size * sizeof(data_type));

    data_type::instance_count.store(0);
    data_type::max_instance_count.store(0);

    // value-initialize data in array
    std::for_each(p, p + data_size, [](data_type& d) {
        ::new (static_cast<void*>(std::addressof(d))) data_type;
    });

    PIKA_TEST_EQ(data_type::instance_count.load(), data_size);

    std::uniform_int_distribution<> dis(0, data_size - 1);
    std::atomic<std::size_t> throw_after(dis(gen));    //-V104
    std::int64_t throw_after_ = throw_after.load();

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::destroy(policy,
            decorated_iterator(p,
                [&throw_after]() {
                    if (throw_after-- == 0)
                        throw std::bad_alloc();
                }),
            decorated_iterator(p + data_size));

        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_bad_alloc);
    PIKA_TEST(returned_from_algorithm);
    PIKA_TEST_LTE(data_type::instance_count.load(),
        std::size_t(data_size - throw_after_));

    std::free(p);
}
