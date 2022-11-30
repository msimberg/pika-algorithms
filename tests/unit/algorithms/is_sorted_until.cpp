//  Copyright (c) 2015 Daniel Bourgeois
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/is_sorted.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////////
int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(0, 99);

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until1(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    iterator until = pika::is_sorted_until(
        policy, iterator(std::begin(c)), iterator(std::end(c)));

    base_iterator test_index = std::end(c);

    PIKA_TEST(until == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until1_async(ExPolicy p, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    pika::future<iterator> f = pika::is_sorted_until(
        p, iterator(std::begin(c)), iterator(std::end(c)));

    base_iterator test_index = std::end(c);

    f.wait();
    PIKA_TEST(f.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_sorted_until1_seq(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    iterator until =
        pika::is_sorted_until(iterator(std::begin(c)), iterator(std::end(c)));

    base_iterator test_index = std::end(c);

    PIKA_TEST(until == iterator(test_index));
}

template <typename IteratorTag>
void test_sorted_until1()
{
    using namespace pika::execution;

    test_sorted_until1(seq, IteratorTag());
    test_sorted_until1(par, IteratorTag());
    test_sorted_until1(par_unseq, IteratorTag());

    test_sorted_until1_async(seq(task), IteratorTag());
    test_sorted_until1_async(par(task), IteratorTag());

    test_sorted_until1_seq(IteratorTag());
}

void sorted_until_test1()
{
    test_sorted_until1<std::random_access_iterator_tag>();
    test_sorted_until1<std::forward_iterator_tag>();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_until2(ExPolicy policy, IteratorTag)
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

    iterator until = pika::is_sorted_until(
        policy, iterator(std::begin(c)), iterator(std::end(c)), pred);

    base_iterator test_index = std::end(c);

    PIKA_TEST(until == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until2_async(ExPolicy p, IteratorTag)
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

    pika::future<iterator> f = pika::is_sorted_until(
        p, iterator(std::begin(c)), iterator(std::end(c)), pred);

    base_iterator test_index = std::end(c);
    f.wait();
    PIKA_TEST(f.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_sorted_until2_seq(IteratorTag)
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

    iterator until = pika::is_sorted_until(
        iterator(std::begin(c)), iterator(std::end(c)), pred);

    base_iterator test_index = std::end(c);

    PIKA_TEST(until == iterator(test_index));
}

template <typename IteratorTag>
void test_sorted_until2()
{
    using namespace pika::execution;
    test_sorted_until2(seq, IteratorTag());
    test_sorted_until2(par, IteratorTag());
    test_sorted_until2(par_unseq, IteratorTag());

    test_sorted_until2_async(seq(task), IteratorTag());
    test_sorted_until2_async(par(task), IteratorTag());

    test_sorted_until2_seq(IteratorTag());
}

void sorted_until_test2()
{
    test_sorted_until2<std::random_access_iterator_tag>();
    test_sorted_until2<std::forward_iterator_tag>();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_until3(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;
    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<std::size_t> c1(10007);
    std::vector<std::size_t> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);
    c1[0] = 20000;
    c1[c1.size() - 1] = 0;
    c2[c2.size() / 3] = 0;
    c2[2 * c2.size() / 3] = 0;

    iterator until1 = pika::is_sorted_until(
        policy, iterator(std::begin(c1)), iterator(std::end(c1)));
    iterator until2 = pika::is_sorted_until(
        policy, iterator(std::begin(c2)), iterator(std::end(c2)));

    base_iterator test_index1 = std::begin(c1) + 1;
    base_iterator test_index2 = std::begin(c2) + c2.size() / 3;

    PIKA_TEST(until1 == iterator(test_index1));
    PIKA_TEST(until2 == iterator(test_index2));
}

template <typename ExPolicy, typename IteratorTag>
void test_sorted_until3_async(ExPolicy p, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;
    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<std::size_t> c1(10007);
    std::vector<std::size_t> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);
    c1[0] = 20000;
    c1[c1.size() - 1] = 0;
    c2[c2.size() / 3] = 0;
    c2[2 * c2.size() / 3] = 0;

    pika::future<iterator> f1 = pika::is_sorted_until(
        p, iterator(std::begin(c1)), iterator(std::end(c1)));
    pika::future<iterator> f2 = pika::is_sorted_until(
        p, iterator(std::begin(c2)), iterator(std::end(c2)));

    base_iterator test_index1 = std::begin(c1) + 1;
    base_iterator test_index2 = std::begin(c2) + c2.size() / 3;

    f1.wait();
    PIKA_TEST(f1.get() == iterator(test_index1));
    f2.wait();
    PIKA_TEST(f2.get() == iterator(test_index2));
}

template <typename IteratorTag>
void test_sorted_until3_seq(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;
    //test the following:
    // put unsorted elements at each ends
    // put two unsorted elements in the middle

    std::vector<std::size_t> c1(10007);
    std::vector<std::size_t> c2(10007);
    std::iota(std::begin(c1), std::end(c1), 0);
    std::iota(std::begin(c2), std::end(c2), 0);
    c1[0] = 20000;
    c1[c1.size() - 1] = 0;
    c2[c2.size() / 3] = 0;
    c2[2 * c2.size() / 3] = 0;

    iterator until1 =
        pika::is_sorted_until(iterator(std::begin(c1)), iterator(std::end(c1)));
    iterator until2 =
        pika::is_sorted_until(iterator(std::begin(c2)), iterator(std::end(c2)));

    base_iterator test_index1 = std::begin(c1) + 1;
    base_iterator test_index2 = std::begin(c2) + c2.size() / 3;

    PIKA_TEST(until1 == iterator(test_index1));
    PIKA_TEST(until2 == iterator(test_index2));
}

template <typename IteratorTag>
void test_sorted_until3()
{
    using namespace pika::execution;
    test_sorted_until3(seq, IteratorTag());
    test_sorted_until3(par, IteratorTag());
    test_sorted_until3(par_unseq, IteratorTag());

    test_sorted_until3_async(seq(task), IteratorTag());
    test_sorted_until3_async(par(task), IteratorTag());

    test_sorted_until3_seq(IteratorTag());
}

void sorted_until_test3()
{
    test_sorted_until3<std::random_access_iterator_tag>();
    test_sorted_until3<std::forward_iterator_tag>();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_until_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        pika::is_sorted_until(policy,
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
void test_sorted_until_async_exception(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        pika::future<decorated_iterator> f = pika::is_sorted_until(p,
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
void test_sorted_until_seq_exception(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        pika::is_sorted_until(decorated_iterator(std::begin(c),
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

template <typename IteratorTag>
void test_sorted_until_exception()
{
    using namespace pika::execution;
    //If the execution policy object is of type vector_execution_policy,
    //  std::terminate shall be called. Therefore we do not test exceptions
    //  with a vector execution policy
    test_sorted_until_exception(seq, IteratorTag());
    test_sorted_until_exception(par, IteratorTag());

    test_sorted_until_async_exception(seq(task), IteratorTag());
    test_sorted_until_async_exception(par(task), IteratorTag());

    test_sorted_until_seq_exception(IteratorTag());
}

void sorted_until_exception_test()
{
    test_sorted_until_exception<std::random_access_iterator_tag>();
    test_sorted_until_exception<std::forward_iterator_tag>();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_sorted_until_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::fill(std::begin(c), std::begin(c) + c.size() / 2, 2 * (dis(gen)));
    std::fill(std::begin(c) + c.size() / 2, std::end(c), 2 * (dis(gen)) + 1);

    bool caught_bad_alloc = false;
    try
    {
        pika::is_sorted_until(policy,
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
void test_sorted_until_async_bad_alloc(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::fill(std::begin(c), std::begin(c) + c.size() / 2, 2 * (dis(gen)));
    std::fill(std::begin(c) + c.size() / 2, std::end(c), 2 * (dis(gen)) + 1);

    bool caught_bad_alloc = false;
    try
    {
        pika::future<decorated_iterator> f = pika::is_sorted_until(p,
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
void test_sorted_until_seq_bad_alloc(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    //fill first half of array with even numbers and second half
    //with odd numbers
    std::fill(std::begin(c), std::begin(c) + c.size() / 2, 2 * (dis(gen)));
    std::fill(std::begin(c) + c.size() / 2, std::end(c), 2 * (dis(gen)) + 1);

    bool caught_bad_alloc = false;
    try
    {
        pika::is_sorted_until(
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

template <typename IteratorTag>
void test_sorted_until_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_sorted_until_bad_alloc(par, IteratorTag());
    test_sorted_until_bad_alloc(seq, IteratorTag());

    test_sorted_until_async_bad_alloc(seq(task), IteratorTag());
    test_sorted_until_async_bad_alloc(par(task), IteratorTag());

    test_sorted_until_seq_bad_alloc(IteratorTag());
}

void sorted_until_bad_alloc_test()
{
    test_sorted_until_bad_alloc<std::random_access_iterator_tag>();
    test_sorted_until_bad_alloc<std::forward_iterator_tag>();
}

int pika_main()
{
    sorted_until_test1();
    sorted_until_test2();
    sorted_until_test3();
    sorted_until_exception_test();
    sorted_until_bad_alloc_test();
    return pika::finalize();
}

int main(int argc, char* argv[])
{
    using namespace pika::program_options;
    options_description desc_commandline(
        "Usage: " PIKA_APPLICATION_STRING " [options]");

    std::vector<std::string> const cfg = {"pika.os_threads=all"};

    pika::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    PIKA_TEST_EQ_MSG(pika::init(pika_main, argc, argv, init_args), 0,
        "pika main exited with non-zero status");

    return 0;
}
