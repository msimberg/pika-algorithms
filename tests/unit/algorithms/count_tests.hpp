//  Copyright (c) 2014-2016 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/parallel/algorithms/count.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(1, 30);

template <typename IteratorTag>
void test_count(IteratorTag)
{
    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    // assure gen() does not evalulate to zero
    std::iota(std::begin(c), std::end(c), gen() + 1);
    std::size_t find_count = dis(gen);    //-V101
    for (std::size_t i = 0; i != find_count && i != c.size(); ++i)
    {
        c[i] = 0;
    }

    std::int64_t num_items =
        pika::count(iterator(std::begin(c)), iterator(std::end(c)), int(0));

    PIKA_TEST_EQ(num_items, static_cast<std::int64_t>(find_count));
}

template <typename ExPolicy, typename IteratorTag>
void test_count(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    // assure gen() does not evalulate to zero
    std::iota(std::begin(c), std::end(c), gen() + 1);
    std::size_t find_count = dis(gen);    //-V101
    for (std::size_t i = 0; i != find_count && i != c.size(); ++i)
    {
        c[i] = 0;
    }

    std::int64_t num_items = pika::count(
        policy, iterator(std::begin(c)), iterator(std::end(c)), int(0));

    PIKA_TEST_EQ(num_items, static_cast<std::int64_t>(find_count));
}

template <typename ExPolicy, typename IteratorTag>
void test_count_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<int>::iterator;
    using diff_type = std::vector<int>::difference_type;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    // assure gen() does not evaluate to zero
    std::iota(std::begin(c), std::end(c), gen() + 1);

    std::size_t find_count = dis(gen);    //-V101
    for (std::size_t i = 0; i != find_count && i != c.size(); ++i)
    {
        c[i] = 0;
    }

    pika::future<diff_type> f =
        pika::count(p, iterator(std::begin(c)), iterator(std::end(c)), int(0));

    PIKA_TEST_EQ(static_cast<diff_type>(find_count), f.get());
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_count_exception(IteratorTag)
{
    using base_iterator = std::vector<int>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    try
    {
        pika::count(decorated_iterator(std::begin(c),
                        []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::end(c)), int(10));
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
}

template <typename ExPolicy, typename IteratorTag>
void test_count_exception(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<int>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    try
    {
        pika::count(policy,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::end(c)), int(10));
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
}

template <typename ExPolicy, typename IteratorTag>
void test_count_exception_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<int>::iterator;
    using diff_type = std::vector<int>::difference_type;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::fill(std::begin(c), std::end(c), 10);

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<diff_type> f = pika::count(p,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::end(c)), int(10));
        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(p, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
    PIKA_TEST(returned_from_algorithm);
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_count_bad_alloc(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<int>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_bad_alloc = false;
    try
    {
        pika::count(policy,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c)), int(10));
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
}

template <typename ExPolicy, typename IteratorTag>
void test_count_bad_alloc_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<int>::iterator;
    using diff_type = std::vector<int>::difference_type;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<diff_type> f = pika::count(p,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c)), int(10));
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
}
