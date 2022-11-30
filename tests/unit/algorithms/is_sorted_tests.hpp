//  Copyright (c) 2015 Daniel Bourgeois
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/execution.hpp>
#include <pika/parallel/algorithms/is_sorted.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iterator>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////////
int seed = std::random_device{}();
std::mt19937 gen(seed);

template <typename ExPolicy, typename IteratorTag>
void test_sorted1(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);

    bool is_ordered = pika::is_sorted(std::forward<ExPolicy>(policy),
        iterator(std::begin(c)), iterator(std::end(c)));

    PIKA_TEST(is_ordered);
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted1_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);

    pika::future<bool> f = pika::is_sorted(std::forward<ExPolicy>(p),
        iterator(std::begin(c)), iterator(std::end(c)));
    f.wait();

    PIKA_TEST(f.get());
}

template <typename IteratorTag>
void test_sorted1_seq(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);

    PIKA_TEST(pika::is_sorted(iterator(std::begin(c)), iterator(std::end(c))));
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted2(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    std::size_t ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](std::size_t ahead, std::size_t behind) {
        return behind > ahead && behind != ignore;
    };

    bool is_ordered = pika::is_sorted(
        policy, iterator(std::begin(c)), iterator(std::end(c)), pred);

    PIKA_TEST(is_ordered);
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted2_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    std::size_t ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](std::size_t ahead, std::size_t behind) {
        return behind > ahead && behind != ignore;
    };

    pika::future<bool> f = pika::is_sorted(
        p, iterator(std::begin(c)), iterator(std::end(c)), pred);
    f.wait();

    PIKA_TEST(f.get());
}

template <typename IteratorTag>
void test_sorted2_seq(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c), std::end(c), 0);
    //Add a certain large value in middle of array to ignore
    std::size_t ignore = 20000;
    c[c.size() / 2] = ignore;
    //Provide custom predicate to ignore the value of ignore
    //pred should return true when it is given something deemed not sorted
    auto pred = [&ignore](std::size_t ahead, std::size_t behind) {
        return behind > ahead && behind != ignore;
    };

    PIKA_TEST(
        pika::is_sorted(iterator(std::begin(c)), iterator(std::end(c)), pred));
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted3(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c_beg(10007);
    std::vector<std::size_t> c_end(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c_beg), std::end(c_beg), 0);
    std::iota(std::begin(c_end), std::end(c_end), 0);
    //add unsorted element to c_beg, c_end at the beginning, end respectively
    c_beg[0] = 20000;
    c_end[c_end.size() - 1] = 0;

    bool is_ordered1 = pika::is_sorted(
        policy, iterator(std::begin(c_beg)), iterator(std::end(c_beg)));
    bool is_ordered2 = pika::is_sorted(
        policy, iterator(std::begin(c_end)), iterator(std::end(c_end)));

    PIKA_TEST(!is_ordered1);
    PIKA_TEST(!is_ordered2);
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted3_async(ExPolicy p, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c_beg(10007);
    std::vector<std::size_t> c_end(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c_beg), std::end(c_beg), 0);
    std::iota(std::begin(c_end), std::end(c_end), 0);
    //add unsorted element to c_beg, c_end at the beginning, end respectively
    c_beg[0] = 20000;
    c_end[c_end.size() - 1] = 0;

    pika::future<bool> f1 = pika::is_sorted(
        p, iterator(std::begin(c_beg)), iterator(std::end(c_beg)));
    pika::future<bool> f2 = pika::is_sorted(
        p, iterator(std::begin(c_end)), iterator(std::end(c_end)));
    f1.wait();
    PIKA_TEST(!f1.get());
    f2.wait();
    PIKA_TEST(!f2.get());
}

template <typename IteratorTag>
void test_sorted3_seq(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c_beg(10007);
    std::vector<std::size_t> c_end(10007);
    //Fill with sorted values from 0 to 10006
    std::iota(std::begin(c_beg), std::end(c_beg), 0);
    std::iota(std::begin(c_end), std::end(c_end), 0);
    //add unsorted element to c_beg, c_end at the beginning, end respectively
    c_beg[0] = 20000;
    c_end[c_end.size() - 1] = 0;

    PIKA_TEST(!pika::is_sorted(
        iterator(std::begin(c_beg)), iterator(std::end(c_beg))));
    PIKA_TEST(!pika::is_sorted(
        iterator(std::begin(c_end)), iterator(std::end(c_end))));
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        pika::is_sorted(policy,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
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
void test_sorted_exception_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);

    bool caught_exception = false;
    try
    {
        pika::future<bool> f = pika::is_sorted(p,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
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
}

template <typename IteratorTag>
void test_sorted_exception_seq(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        pika::is_sorted(decorated_iterator(std::begin(c),
                            []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
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

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(100007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        pika::is_sorted(policy,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c), []() { throw std::bad_alloc(); }));
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
void test_sorted_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        pika::future<bool> f = pika::is_sorted(p,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c), []() { throw std::bad_alloc(); }));

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
}

template <typename IteratorTag>
void test_sorted_bad_alloc_seq(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(100007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        pika::is_sorted(
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c), []() { throw std::bad_alloc(); }));
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
