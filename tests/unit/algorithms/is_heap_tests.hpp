//  Copyright (c) 2017 Taeguk Kwon
//  Copyright (c) 2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/parallel/algorithms/is_heap.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(0, 10006);

struct throw_always
{
    template <typename T1, typename T2>
    bool operator()(T1 const&, T2 const&) const
    {
        throw std::runtime_error("test");
    }
};

struct throw_bad_alloc
{
    template <typename T1, typename T2>
    bool operator()(T1 const&, T2 const&) const
    {
        throw std::bad_alloc();
    }
};

struct user_defined_type
{
    user_defined_type() = default;
    user_defined_type(int rand_no)
      : val(rand_no)
    {
    }

    bool operator<(user_defined_type const& t) const
    {
        if (this->name < t.name)
            return true;
        else if (this->name > t.name)
            return false;
        else
            return this->val < t.val;
    }

    const user_defined_type& operator++()
    {
        static const std::vector<std::string> name_list = {
            "ABB", "ABC", "ACB", "BCA", "CAA", "CAAA", "CAAB"};
        std::uniform_int_distribution<> dist(0, name_list.size() - 1);
        name = name_list[dist(gen)];
        ++val;
        return *this;
    }

    std::string name;
    int val;
};

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag, typename DataType>
void test_is_heap(IteratorTag, DataType, bool test_for_is_heap = true)
{
    using base_iterator = typename std::vector<DataType>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<DataType> c(10007);
    std::iota(std::begin(c), std::end(c), DataType(gen()));

    auto heap_end_iter = std::next(std::begin(c), dis(gen));
    std::make_heap(std::begin(c), heap_end_iter);

    if (test_for_is_heap)
    {
        bool result =
            pika::is_heap(iterator(std::begin(c)), iterator(std::end(c)));
        bool solution = std::is_heap(std::begin(c), std::end(c));

        PIKA_TEST_EQ(result, solution);
    }
    else
    {
        iterator result =
            pika::is_heap_until(iterator(std::begin(c)), iterator(std::end(c)));
        auto solution = std::is_heap_until(std::begin(c), std::end(c));

        PIKA_TEST(result.base() == solution);
    }
}

template <typename ExPolicy, typename IteratorTag, typename DataType>
void test_is_heap(
    ExPolicy&& policy, IteratorTag, DataType, bool test_for_is_heap = true)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = typename std::vector<DataType>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<DataType> c(10007);
    std::iota(std::begin(c), std::end(c), DataType(gen()));

    auto heap_end_iter = std::next(std::begin(c), dis(gen));
    std::make_heap(std::begin(c), heap_end_iter);

    if (test_for_is_heap)
    {
        bool result = pika::is_heap(
            policy, iterator(std::begin(c)), iterator(std::end(c)));
        bool solution = std::is_heap(std::begin(c), std::end(c));

        PIKA_TEST_EQ(result, solution);
    }
    else
    {
        iterator result = pika::is_heap_until(
            policy, iterator(std::begin(c)), iterator(std::end(c)));
        auto solution = std::is_heap_until(std::begin(c), std::end(c));

        PIKA_TEST(result.base() == solution);
    }
}

template <typename IteratorTag, typename DataType, typename Pred>
void test_is_heap_with_pred(
    IteratorTag, DataType, Pred pred, bool test_for_is_heap = true)
{
    using base_iterator = typename std::vector<DataType>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<DataType> c(10007);
    std::iota(std::begin(c), std::end(c), DataType(gen()));

    auto heap_end_iter = std::next(std::begin(c), dis(gen));
    std::make_heap(std::begin(c), heap_end_iter);

    if (test_for_is_heap)
    {
        bool result =
            pika::is_heap(iterator(std::begin(c)), iterator(std::end(c)), pred);
        bool solution = std::is_heap(std::begin(c), std::end(c), pred);

        PIKA_TEST_EQ(result, solution);
    }
    else
    {
        iterator result = pika::is_heap_until(
            iterator(std::begin(c)), iterator(std::end(c)), pred);
        auto solution = std::is_heap_until(std::begin(c), std::end(c), pred);

        PIKA_TEST(result.base() == solution);
    }
}

template <typename ExPolicy, typename IteratorTag, typename DataType,
    typename Pred>
void test_is_heap_with_pred(ExPolicy&& policy, IteratorTag, DataType, Pred pred,
    bool test_for_is_heap = true)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = typename std::vector<DataType>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<DataType> c(10007);
    std::iota(std::begin(c), std::end(c), DataType(gen()));

    auto heap_end_iter = std::next(std::begin(c), dis(gen));
    std::make_heap(std::begin(c), heap_end_iter);

    if (test_for_is_heap)
    {
        bool result = pika::is_heap(
            policy, iterator(std::begin(c)), iterator(std::end(c)), pred);
        bool solution = std::is_heap(std::begin(c), std::end(c), pred);

        PIKA_TEST_EQ(result, solution);
    }
    else
    {
        iterator result = pika::is_heap_until(
            policy, iterator(std::begin(c)), iterator(std::end(c)), pred);
        auto solution = std::is_heap_until(std::begin(c), std::end(c), pred);

        PIKA_TEST(result.base() == solution);
    }
}

template <typename ExPolicy, typename IteratorTag, typename DataType>
void test_is_heap_async(
    ExPolicy&& policy, IteratorTag, DataType, bool test_for_is_heap = true)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = typename std::vector<DataType>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<DataType> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    auto heap_end_iter = std::next(std::begin(c), dis(gen));
    std::make_heap(std::begin(c), heap_end_iter);

    if (test_for_is_heap)
    {
        auto f = pika::is_heap(
            policy, iterator(std::begin(c)), iterator(std::end(c)));
        bool result = f.get();
        bool solution = std::is_heap(std::begin(c), std::end(c));

        PIKA_TEST_EQ(result, solution);
    }
    else
    {
        auto f = pika::is_heap_until(
            policy, iterator(std::begin(c)), iterator(std::end(c)));
        iterator result = f.get();
        auto solution = std::is_heap_until(std::begin(c), std::end(c));

        PIKA_TEST(result.base() == solution);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_is_heap_exception(IteratorTag, bool test_for_is_heap = true)
{
    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());
    std::make_heap(std::begin(c), std::end(c));

    bool caught_exception = false;
    try
    {
        if (test_for_is_heap)
        {
            pika::is_heap(
                iterator(std::begin(c)), iterator(std::end(c)), throw_always());
        }
        else
        {
            pika::is_heap_until(
                iterator(std::begin(c)), iterator(std::end(c)), throw_always());
        }

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
void test_is_heap_exception(
    ExPolicy&& policy, IteratorTag, bool test_for_is_heap = true)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());
    std::make_heap(std::begin(c), std::end(c));

    bool caught_exception = false;
    try
    {
        if (test_for_is_heap)
        {
            pika::is_heap(policy, iterator(std::begin(c)),
                iterator(std::end(c)), throw_always());
        }
        else
        {
            pika::is_heap_until(policy, iterator(std::begin(c)),
                iterator(std::end(c)), throw_always());
        }

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
void test_is_heap_exception_async(
    ExPolicy&& policy, IteratorTag, bool test_for_is_heap = true)
{
    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());
    std::make_heap(std::begin(c), std::end(c));

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        if (test_for_is_heap)
        {
            auto f = pika::is_heap(policy, iterator(std::begin(c)),
                iterator(std::end(c)), throw_always());
            returned_from_algorithm = true;
            f.get();
        }
        else
        {
            auto f = pika::is_heap_until(policy, iterator(std::begin(c)),
                iterator(std::end(c)), throw_always());
            returned_from_algorithm = true;
            f.get();
        }

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
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_is_heap_bad_alloc(IteratorTag, bool test_for_is_heap = true)
{
    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());
    std::make_heap(std::begin(c), std::end(c));

    bool caught_bad_alloc = false;
    try
    {
        if (test_for_is_heap)
        {
            pika::is_heap(iterator(std::begin(c)), iterator(std::end(c)),
                throw_bad_alloc());
        }
        else
        {
            pika::is_heap_until(iterator(std::begin(c)), iterator(std::end(c)),
                throw_bad_alloc());
        }

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
void test_is_heap_bad_alloc(
    ExPolicy&& policy, IteratorTag, bool test_for_is_heap = true)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());
    std::make_heap(std::begin(c), std::end(c));

    bool caught_bad_alloc = false;
    try
    {
        if (test_for_is_heap)
        {
            pika::is_heap(policy, iterator(std::begin(c)),
                iterator(std::end(c)), throw_bad_alloc());
        }
        else
        {
            pika::is_heap_until(policy, iterator(std::begin(c)),
                iterator(std::end(c)), throw_bad_alloc());
        }

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
void test_is_heap_bad_alloc_async(
    ExPolicy&& policy, IteratorTag, bool test_for_is_heap = true)
{
    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());
    std::make_heap(std::begin(c), std::end(c));

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        if (test_for_is_heap)
        {
            auto f = pika::is_heap(policy, iterator(std::begin(c)),
                iterator(std::end(c)), throw_bad_alloc());
            returned_from_algorithm = true;
            f.get();
        }
        else
        {
            auto f = pika::is_heap_until(policy, iterator(std::begin(c)),
                iterator(std::end(c)), throw_bad_alloc());
            returned_from_algorithm = true;
            f.get();
        }

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

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_is_heap(bool test_for_is_heap = true)
{
    using namespace pika::execution;

    test_is_heap(IteratorTag(), int(), test_for_is_heap);
    test_is_heap(seq, IteratorTag(), int(), test_for_is_heap);
    test_is_heap(par, IteratorTag(), int(), test_for_is_heap);
    test_is_heap(par_unseq, IteratorTag(), int(), test_for_is_heap);

    test_is_heap(IteratorTag(), user_defined_type(), test_for_is_heap);
    test_is_heap(seq, IteratorTag(), user_defined_type(), test_for_is_heap);
    test_is_heap(par, IteratorTag(), user_defined_type(), test_for_is_heap);
    test_is_heap(
        par_unseq, IteratorTag(), user_defined_type(), test_for_is_heap);

    test_is_heap_with_pred(
        IteratorTag(), int(), std::greater<int>(), test_for_is_heap);
    test_is_heap_with_pred(
        seq, IteratorTag(), int(), std::greater<int>(), test_for_is_heap);
    test_is_heap_with_pred(
        par, IteratorTag(), int(), std::greater<int>(), test_for_is_heap);
    test_is_heap_with_pred(
        par_unseq, IteratorTag(), int(), std::greater<int>(), test_for_is_heap);

    test_is_heap_async(seq(task), IteratorTag(), int(), test_for_is_heap);
    test_is_heap_async(par(task), IteratorTag(), int(), test_for_is_heap);

    test_is_heap_async(
        seq(task), IteratorTag(), user_defined_type(), test_for_is_heap);
    test_is_heap_async(
        par(task), IteratorTag(), user_defined_type(), test_for_is_heap);
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_is_heap_exception(bool test_for_is_heap = true)
{
    using namespace pika::execution;

    test_is_heap_exception(IteratorTag(), test_for_is_heap);

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_is_heap_exception(seq, IteratorTag(), test_for_is_heap);
    test_is_heap_exception(par, IteratorTag(), test_for_is_heap);

    test_is_heap_exception_async(seq(task), IteratorTag(), test_for_is_heap);
    test_is_heap_exception_async(par(task), IteratorTag(), test_for_is_heap);
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_is_heap_bad_alloc(bool test_for_is_heap = true)
{
    using namespace pika::execution;

    test_is_heap_bad_alloc(IteratorTag(), test_for_is_heap);

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_is_heap_bad_alloc(seq, IteratorTag(), test_for_is_heap);
    test_is_heap_bad_alloc(par, IteratorTag(), test_for_is_heap);

    test_is_heap_bad_alloc_async(seq(task), IteratorTag(), test_for_is_heap);
    test_is_heap_bad_alloc_async(par(task), IteratorTag(), test_for_is_heap);
}
