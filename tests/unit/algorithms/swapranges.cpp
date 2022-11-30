//  Copyright (c) 2014 Grant Mercer
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/swap_ranges.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_swap_ranges(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::fill(std::begin(d), std::end(d), std::rand());

    pika::swap_ranges(
        iterator(std::begin(c)), iterator(std::end(c)), std::begin(d));

    //equal begins at one, therefore counter is started at 1
    std::size_t count = 1;
    PIKA_TEST(std::equal(std::begin(c) + 1, std::end(c), std::begin(c),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, c.size());

    count = 1;
    PIKA_TEST(std::equal(std::begin(d) + 1, std::end(d), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_NEQ(v1, v2);
            ++count;
            return !(v1 == v2);
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_swap_ranges(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::fill(std::begin(d), std::end(d), std::rand());

    pika::swap_ranges(
        policy, iterator(std::begin(c)), iterator(std::end(c)), std::begin(d));

    //equal begins at one, therefore counter is started at 1
    std::size_t count = 1;
    PIKA_TEST(std::equal(std::begin(c) + 1, std::end(c), std::begin(c),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, c.size());

    count = 1;
    PIKA_TEST(std::equal(std::begin(d) + 1, std::end(d), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_NEQ(v1, v2);
            ++count;
            return !(v1 == v2);
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_swap_ranges_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::fill(std::begin(d), std::end(d), std::rand());

    pika::future<base_iterator> f = pika::swap_ranges(
        p, iterator(std::begin(c)), iterator(std::end(c)), std::begin(d));

    f.wait();

    std::size_t count = 1;
    PIKA_TEST(std::equal(std::begin(c) + 1, std::end(c), std::begin(c),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));

    count = 1;
    PIKA_TEST(std::equal(std::begin(d) + 1, std::end(d), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_NEQ(v1, v2);
            ++count;
            return !(v1 == v2);
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename IteratorTag>
void test_swap_ranges()
{
    using namespace pika::execution;
    test_swap_ranges(IteratorTag());
    test_swap_ranges(seq, IteratorTag());
    test_swap_ranges(par, IteratorTag());
    test_swap_ranges(par_unseq, IteratorTag());

    test_swap_ranges_async(seq(task), IteratorTag());
    test_swap_ranges_async(par(task), IteratorTag());
}

void swap_ranges_test()
{
    test_swap_ranges<std::random_access_iterator_tag>();
    test_swap_ranges<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_swap_ranges_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::fill(std::begin(d), std::end(d), std::rand());

    bool caught_exception = false;
    try
    {
        pika::swap_ranges(policy,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::end(c)), std::begin(d));
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
void test_swap_ranges_exception_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::fill(std::begin(d), std::end(d), std::rand());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<base_iterator> f = pika::swap_ranges(p,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::end(c)), std::begin(d));
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

template <typename IteratorTag>
void test_swap_ranges_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_swap_ranges_exception(seq, IteratorTag());
    test_swap_ranges_exception(par, IteratorTag());

    test_swap_ranges_exception_async(seq(task), IteratorTag());
    test_swap_ranges_exception_async(par(task), IteratorTag());
}

void swap_ranges_exception_test()
{
    test_swap_ranges_exception<std::random_access_iterator_tag>();
    test_swap_ranges_exception<std::forward_iterator_tag>();
}

//////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_swap_ranges_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(100007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::fill(std::begin(d), std::end(d), std::rand());

    bool caught_bad_alloc = false;
    try
    {
        pika::swap_ranges(policy,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c)), std::begin(d));
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
void test_swap_ranges_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::fill(std::begin(d), std::end(d), std::rand());

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<base_iterator> f = pika::swap_ranges(p,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c)), std::begin(d));
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
void test_swap_ranges_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_swap_ranges_bad_alloc(seq, IteratorTag());
    test_swap_ranges_bad_alloc(par, IteratorTag());

    test_swap_ranges_bad_alloc_async(seq(task), IteratorTag());
    test_swap_ranges_bad_alloc_async(par(task), IteratorTag());
}

void swap_ranges_bad_alloc_test()
{
    test_swap_ranges_bad_alloc<std::random_access_iterator_tag>();
    test_swap_ranges_bad_alloc<std::forward_iterator_tag>();
}
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    swap_ranges_test();
    swap_ranges_exception_test();
    swap_ranges_bad_alloc_test();
    return pika::finalize();
}

int main(int argc, char* argv[])
{
    // add command line option which controls the random number generator seed
    using namespace pika::program_options;
    options_description desc_commandline(
        "Usage: " PIKA_APPLICATION_STRING " [options]");

    desc_commandline.add_options()("seed,s", value<unsigned int>(),
        "the random number generator seed to use for this run");

    // By default this test should run on all available cores
    std::vector<std::string> const cfg = {"pika.os_threads=all"};

    // Initialize and run pika
    pika::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    PIKA_TEST_EQ_MSG(pika::init(pika_main, argc, argv, init_args), 0,
        "pika main exited with non-zero status");

    return 0;
}
