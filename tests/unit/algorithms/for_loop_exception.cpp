//  Copyright (c) 2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithm.hpp>
#include <pika/init.hpp>
#include <pika/testing.hpp>

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
struct throw_always
{
    throw_always(std::size_t throw_after)
    {
        throw_after_ = throw_after;
    }

    template <typename T>
    void operator()(T)
    {
        if (--throw_after_ == 0)
            throw std::runtime_error("test");
    }

    static std::atomic<std::size_t> throw_after_;
};

std::atomic<std::size_t> throw_always::throw_after_(0);

struct throw_bad_alloc
{
    throw_bad_alloc(std::size_t throw_after)
    {
        throw_after_ = throw_after;
    }

    template <typename T>
    void operator()(T)
    {
        if (--throw_after_ == 0)
            throw std::bad_alloc();
    }

    static std::atomic<std::size_t> throw_after_;
};

std::atomic<std::size_t> throw_bad_alloc::throw_after_(0);

///////////////////////////////////////////////////////////////////////////////
unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);

template <typename ExPolicy, typename IteratorTag>
void test_for_loop_exception(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::uniform_int_distribution<std::size_t> dis(1, c.size());

    bool caught_exception = false;
    try
    {
        pika::for_loop(std::forward<ExPolicy>(policy), iterator(std::begin(c)),
            iterator(std::end(c)), throw_always(dis(gen)));

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
void test_for_loop_exception_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::uniform_int_distribution<std::size_t> dis(1, c.size());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f =
            pika::for_loop(std::forward<ExPolicy>(p), iterator(std::begin(c)),
                iterator(std::end(c)), throw_always(dis(gen)));
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

////////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_for_loop_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::uniform_int_distribution<std::size_t> dis(1, c.size());

    bool caught_exception = false;
    try
    {
        pika::for_loop(std::forward<ExPolicy>(policy), iterator(std::begin(c)),
            iterator(std::end(c)), throw_bad_alloc(dis(gen)));

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_exception = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_for_loop_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::uniform_int_distribution<std::size_t> dis(1, c.size());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f =
            pika::for_loop(std::forward<ExPolicy>(p), iterator(std::begin(c)),
                iterator(std::end(c)), throw_bad_alloc(dis(gen)));
        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_exception = true;
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
void test_for_loop_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_for_loop_exception(seq, IteratorTag());
    test_for_loop_exception(par, IteratorTag());

    test_for_loop_bad_alloc(seq, IteratorTag());
    test_for_loop_bad_alloc(par, IteratorTag());

    test_for_loop_exception_async(seq(task), IteratorTag());
    test_for_loop_exception_async(par(task), IteratorTag());

    test_for_loop_bad_alloc_async(seq(task), IteratorTag());
    test_for_loop_bad_alloc_async(par(task), IteratorTag());
}

void for_loop_exception_test()
{
    test_for_loop_exception<std::random_access_iterator_tag>();
    test_for_loop_exception<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_for_loop_idx_exception(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::uniform_int_distribution<std::size_t> dis(1, c.size());

    bool caught_exception = false;
    try
    {
        pika::for_loop(std::forward<ExPolicy>(policy), 0, c.size(),
            throw_always(dis(gen)));

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
void test_for_loop_idx_exception_async(ExPolicy&& p, IteratorTag)
{
    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::uniform_int_distribution<std::size_t> dis(1, c.size());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::for_loop(
            std::forward<ExPolicy>(p), 0, c.size(), throw_always(dis(gen)));
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
template <typename ExPolicy>
void test_for_loop_idx_bad_alloc(ExPolicy&& policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::uniform_int_distribution<std::size_t> dis(1, c.size());

    bool caught_exception = false;
    try
    {
        pika::for_loop(std::forward<ExPolicy>(policy), 0, c.size(),
            throw_bad_alloc(dis(gen)));

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const& e)
    {
        caught_exception = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
}

template <typename ExPolicy>
void test_for_loop_idx_bad_alloc_async(ExPolicy&& p)
{
    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::uniform_int_distribution<std::size_t> dis(1, c.size());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::for_loop(
            std::forward<ExPolicy>(p), 0, c.size(), throw_bad_alloc(dis(gen)));
        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const& e)
    {
        caught_exception = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
    PIKA_TEST(returned_from_algorithm);
}

template <typename IteratorTag>
void test_for_loop_exception_idx()
{
    using namespace pika::execution;

    test_for_loop_idx_exception(seq, IteratorTag());
    test_for_loop_idx_exception(par, IteratorTag());

    test_for_loop_idx_bad_alloc(seq);
    test_for_loop_idx_bad_alloc(par);

    test_for_loop_idx_exception_async(seq(task), IteratorTag());
    test_for_loop_idx_exception_async(par(task), IteratorTag());

    test_for_loop_idx_bad_alloc_async(seq(task));
    test_for_loop_idx_bad_alloc_async(par(task));
}

void for_loop_exception_test_idx()
{
    test_for_loop_exception_idx<std::random_access_iterator_tag>();
    test_for_loop_exception_idx<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    for_loop_exception_test();
    for_loop_exception_test_idx();

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
