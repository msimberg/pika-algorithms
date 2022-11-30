//  Copyright (c) 2015 Grant Mercer
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/container_algorithms/make_heap.hpp>
#include <pika/testing.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////
int seed = std::random_device{}();
std::mt19937 gen(seed);

///////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_make_heap1(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(pika::util::begin(c), pika::util::end(c), 0);

    pika::ranges::make_heap(
        iterator(pika::util::begin(c)), iterator(pika::util::end(c)));

    PIKA_TEST_EQ(std::is_heap(pika::util::begin(c), pika::util::end(c)), true);
}

template <typename ExPolicy, typename IteratorTag>
void test_make_heap1(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(pika::util::begin(c), pika::util::end(c), 0);

    pika::ranges::make_heap(
        policy, iterator(pika::util::begin(c)), iterator(pika::util::end(c)));

    PIKA_TEST_EQ(std::is_heap(pika::util::begin(c), pika::util::end(c)), true);
}

template <typename ExPolicy, typename IteratorTag>
void test_make_heap_async1(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(pika::util::begin(c), pika::util::end(c), 0);

    auto test = pika::ranges::make_heap(
        p, iterator(pika::util::begin(c)), iterator(pika::util::end(c)));

    test.get();

    PIKA_TEST_EQ(std::is_heap(pika::util::begin(c), pika::util::end(c)), true);
}

template <typename IteratorTag>
void test_make_heap1()
{
    using namespace pika::execution;

    test_make_heap1(IteratorTag());

    test_make_heap1(seq, IteratorTag());
    test_make_heap1(par, IteratorTag());
    test_make_heap1(par_unseq, IteratorTag());

    test_make_heap_async1(seq(task), IteratorTag());
    test_make_heap_async1(par(task), IteratorTag());
}

void make_heap_test1()
{
    test_make_heap1<std::random_access_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_make_heap2(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(25);
    std::iota(pika::util::begin(c), pika::util::end(c), 0);

    pika::ranges::make_heap(iterator(pika::util::begin(c)),
        iterator(pika::util::end(c)), std::greater<std::size_t>());

    PIKA_TEST_EQ(std::is_heap(pika::util::begin(c), pika::util::end(c),
                     std::greater<std::size_t>()),
        true);
}

template <typename ExPolicy, typename IteratorTag>
void test_make_heap2(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(25);
    std::iota(pika::util::begin(c), pika::util::end(c), 0);

    pika::ranges::make_heap(policy, iterator(pika::util::begin(c)),
        iterator(pika::util::end(c)), std::greater<std::size_t>());

    PIKA_TEST_EQ(std::is_heap(pika::util::begin(c), pika::util::end(c),
                     std::greater<std::size_t>()),
        true);
}

template <typename ExPolicy, typename IteratorTag>
void test_make_heap_async2(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(pika::util::begin(c), pika::util::end(c), 0);

    pika::future<void> test =
        pika::ranges::make_heap(p, iterator(pika::util::begin(c)),
            iterator(pika::util::end(c)), std::greater<std::size_t>());

    test.wait();
    PIKA_TEST_EQ(std::is_heap(pika::util::begin(c), pika::util::end(c),
                     std::greater<std::size_t>()),
        true);
}

template <typename IteratorTag>
void test_make_heap2()
{
    using namespace pika::execution;

    test_make_heap2(IteratorTag());

    test_make_heap2(seq, IteratorTag());
    test_make_heap2(par, IteratorTag());
    test_make_heap2(par_unseq, IteratorTag());

    test_make_heap_async2(seq(task), IteratorTag());
    test_make_heap_async2(par(task), IteratorTag());
}

void make_heap_test2()
{
    test_make_heap2<std::random_access_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_make_heap_exception(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<std::size_t> c(10007);
    std::iota(pika::util::begin(c), pika::util::end(c), gen());

    bool caught_exception = false;
    try
    {
        pika::ranges::make_heap(decorated_iterator(pika::util::begin(c),
                                    []() { throw std::runtime_error("test"); }),
            decorated_iterator(pika::util::end(c)));
        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        //test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_make_heap_exception(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<std::size_t> c(10007);
    std::iota(pika::util::begin(c), pika::util::end(c), gen());

    bool caught_exception = false;
    try
    {
        pika::ranges::make_heap(policy,
            decorated_iterator(pika::util::begin(c),
                []() { throw std::runtime_error("test"); }),
            decorated_iterator(pika::util::end(c)));
        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        //test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_make_heap_exception_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(pika::util::begin(c), pika::util::end(c), gen());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<void> f = pika::ranges::make_heap(p,
            decorated_iterator(pika::util::begin(c),
                []() { throw std::runtime_error("test"); }),
            decorated_iterator(pika::util::end(c)));
        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        //test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }
    PIKA_TEST(caught_exception);
    PIKA_TEST(returned_from_algorithm);
}

template <typename IteratorTag>
void test_make_heap_exception()
{
    using namespace pika::execution;

    test_make_heap_exception(IteratorTag());

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_make_heap_exception(seq, IteratorTag());
    test_make_heap_exception(par, IteratorTag());

    test_make_heap_exception_async(seq(task), IteratorTag());
    test_make_heap_exception_async(par(task), IteratorTag());
}

void make_heap_exception_test()
{
    test_make_heap_exception<std::random_access_iterator_tag>();
}

//////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_make_heap_bad_alloc(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(100007);
    std::iota(pika::util::begin(c), pika::util::end(c), gen());

    bool caught_bad_alloc = false;
    try
    {
        pika::ranges::make_heap(decorated_iterator(pika::util::begin(c),
                                    []() { throw std::bad_alloc(); }),
            decorated_iterator(pika::util::end(c)));
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
void test_make_heap_bad_alloc(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(100007);
    std::iota(pika::util::begin(c), pika::util::end(c), gen());

    bool caught_bad_alloc = false;
    try
    {
        pika::ranges::make_heap(policy,
            decorated_iterator(
                pika::util::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(pika::util::end(c)));
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
void test_make_heap_bad_alloc_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(pika::util::begin(c), pika::util::end(c), gen());

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<void> f = pika::ranges::make_heap(p,
            decorated_iterator(
                pika::util::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(pika::util::end(c)));
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

template <typename IteratorTag>
void test_make_heap_bad_alloc()
{
    using namespace pika::execution;

    test_make_heap_bad_alloc(IteratorTag());

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_make_heap_bad_alloc(seq, IteratorTag());
    test_make_heap_bad_alloc(par, IteratorTag());

    test_make_heap_bad_alloc_async(seq(task), IteratorTag());
    test_make_heap_bad_alloc_async(par(task), IteratorTag());
}

void make_heap_bad_alloc_test()
{
    test_make_heap_bad_alloc<std::random_access_iterator_tag>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    make_heap_test1();
    make_heap_test2();
    make_heap_exception_test();
    make_heap_bad_alloc_test();

    return pika::finalize();
}

int main(int argc, char* argv[])
{
    using namespace pika::program_options;
    options_description desc_commandline(
        "Usage: " PIKA_APPLICATION_STRING " [options]");

    desc_commandline.add_options()("seed,s", value<unsigned int>(),
        "the random number generator seed to use for this run");

    pika::init_params init_args;
    init_args.desc_cmdline = desc_commandline;

    PIKA_TEST_EQ_MSG(pika::init(pika_main, argc, argv, init_args), 0,
        "pika main exited with a non-zero status");

    return 0;
}
