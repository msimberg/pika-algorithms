//  Copyright (c) 2014 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/uninitialized_copy.hpp>
#include <pika/testing.hpp>

#include <atomic>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_copy_n(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    pika::uninitialized_copy_n(
        policy, iterator(std::begin(c)), c.size(), std::begin(d));

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::end(c), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_copy_n_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    pika::future<base_iterator> f = pika::uninitialized_copy_n(
        p, iterator(std::begin(c)), c.size(), std::begin(d));
    f.wait();

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::end(c), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename IteratorTag>
void test_uninitialized_copy_n()
{
    using namespace pika::execution;

    test_uninitialized_copy_n(seq, IteratorTag());
    test_uninitialized_copy_n(par, IteratorTag());
    test_uninitialized_copy_n(par_unseq, IteratorTag());

    test_uninitialized_copy_n_async(seq(task), IteratorTag());
    test_uninitialized_copy_n_async(par(task), IteratorTag());
}

void uninitialized_copy_n_test()
{
    test_uninitialized_copy_n<std::random_access_iterator_tag>();
    test_uninitialized_copy_n<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_copy_n_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<test::count_instances>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<test::count_instances> c(10007);
    std::vector<test::count_instances> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    std::atomic<std::size_t> throw_after(std::rand() % c.size());    //-V104
    test::count_instances::instance_count.store(0);

    bool caught_exception = false;
    try
    {
        pika::uninitialized_copy_n(policy,
            decorated_iterator(std::begin(c),
                [&throw_after]() {
                    if (throw_after-- == 0)
                        throw std::runtime_error("test");
                }),
            c.size(), std::begin(d));
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
    PIKA_TEST_EQ(test::count_instances::instance_count.load(), std::size_t(0));
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_copy_n_exception_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<test::count_instances>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<test::count_instances> c(10007);
    std::vector<test::count_instances> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    std::atomic<std::size_t> throw_after(std::rand() % c.size());    //-V104
    test::count_instances::instance_count.store(0);

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<base_iterator> f = pika::uninitialized_copy_n(p,
            decorated_iterator(std::begin(c),
                [&throw_after]() {
                    if (throw_after-- == 0)
                        throw std::runtime_error("test");
                }),
            c.size(), std::begin(d));

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
    PIKA_TEST_EQ(test::count_instances::instance_count.load(), std::size_t(0));
}

template <typename IteratorTag>
void test_uninitialized_copy_n_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_uninitialized_copy_n_exception(seq, IteratorTag());
    test_uninitialized_copy_n_exception(par, IteratorTag());

    test_uninitialized_copy_n_exception_async(seq(task), IteratorTag());
    test_uninitialized_copy_n_exception_async(par(task), IteratorTag());
}

void uninitialized_copy_n_exception_test()
{
    test_uninitialized_copy_n_exception<std::random_access_iterator_tag>();
    test_uninitialized_copy_n_exception<std::forward_iterator_tag>();
}

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_copy_n_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<test::count_instances>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<test::count_instances> c(10007);
    std::vector<test::count_instances> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    std::atomic<std::size_t> throw_after(std::rand() % c.size());    //-V104
    test::count_instances::instance_count.store(0);

    bool caught_bad_alloc = false;
    try
    {
        pika::uninitialized_copy_n(policy,
            decorated_iterator(std::begin(c),
                [&throw_after]() {
                    if (throw_after-- == 0)
                        throw std::bad_alloc();
                }),
            c.size(), std::begin(d));

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
    PIKA_TEST_EQ(test::count_instances::instance_count.load(), std::size_t(0));
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_copy_n_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<test::count_instances>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<test::count_instances> c(10007);
    std::vector<test::count_instances> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    std::atomic<std::size_t> throw_after(std::rand() % c.size());    //-V104
    test::count_instances::instance_count.store(0);

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<base_iterator> f = pika::uninitialized_copy_n(p,
            decorated_iterator(std::begin(c),
                [&throw_after]() {
                    if (throw_after-- == 0)
                        throw std::bad_alloc();
                }),
            c.size(), std::begin(d));

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
    PIKA_TEST_EQ(test::count_instances::instance_count.load(), std::size_t(0));
}

template <typename IteratorTag>
void test_uninitialized_copy_n_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_uninitialized_copy_n_bad_alloc(seq, IteratorTag());
    test_uninitialized_copy_n_bad_alloc(par, IteratorTag());

    test_uninitialized_copy_n_bad_alloc_async(seq(task), IteratorTag());
    test_uninitialized_copy_n_bad_alloc_async(par(task), IteratorTag());
}

void uninitialized_copy_n_bad_alloc_test()
{
    test_uninitialized_copy_n_bad_alloc<std::random_access_iterator_tag>();
    test_uninitialized_copy_n_bad_alloc<std::forward_iterator_tag>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    uninitialized_copy_n_test();
    uninitialized_copy_n_exception_test();
    uninitialized_copy_n_bad_alloc_test();
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
