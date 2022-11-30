//  Copyright (c) 2016 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithm.hpp>
#include <pika/init.hpp>
#include <pika/testing.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(1, 10006);

template <typename ExPolicy, typename IteratorTag>
void test_for_loop_strided(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::for_each(std::begin(c), std::end(c), [](std::size_t& v) -> void {
        if (v == 42)
            v = 43;
    });

    int stride = dis(gen);    //-V103

    pika::for_loop_strided(std::forward<ExPolicy>(policy),
        iterator(std::begin(c)), iterator(std::end(c)), stride,
        [](iterator it) { *it = 42; });

    // verify values
    std::size_t count = 0;
    for (std::size_t i = 0; i != c.size(); ++i)
    {
        if (i % stride == 0)    //-V104
        {
            PIKA_TEST_EQ(c[i], std::size_t(42));
        }
        else
        {
            PIKA_TEST_NEQ(c[i], std::size_t(42));
        }
        ++count;
    }
    PIKA_TEST_EQ(count, c.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_for_loop_strided_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::for_each(std::begin(c), std::end(c), [](std::size_t& v) -> void {
        if (v == 42)
            v = 43;
    });

    int stride = dis(gen);    //-V103

    auto f = pika::for_loop_strided(std::forward<ExPolicy>(p),
        iterator(std::begin(c)), iterator(std::end(c)), stride,
        [](iterator it) { *it = 42; });
    f.wait();

    // verify values
    std::size_t count = 0;
    for (std::size_t i = 0; i != c.size(); ++i)
    {
        if (i % stride == 0)    //-V104
        {
            PIKA_TEST_EQ(c[i], std::size_t(42));
        }
        else
        {
            PIKA_TEST_NEQ(c[i], std::size_t(42));
        }
        ++count;
    }
    PIKA_TEST_EQ(count, c.size());
}

template <typename IteratorTag>
void test_for_loop_strided()
{
    using namespace pika::execution;

    test_for_loop_strided(seq, IteratorTag());
    test_for_loop_strided(par, IteratorTag());
    test_for_loop_strided(par_unseq, IteratorTag());

    test_for_loop_strided_async(seq(task), IteratorTag());
    test_for_loop_strided_async(par(task), IteratorTag());
}

void for_loop_strided_test()
{
    test_for_loop_strided<std::random_access_iterator_tag>();
    test_for_loop_strided<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy>
void test_for_loop_strided_idx(ExPolicy&& policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::for_each(std::begin(c), std::end(c), [](std::size_t& v) -> void {
        if (v == 42)
            v = 43;
    });

    int stride = dis(gen);    //-V103

    pika::for_loop_strided(std::forward<ExPolicy>(policy), 0, c.size(), stride,
        [&c](std::size_t i) { c[i] = 42; });

    // verify values
    std::size_t count = 0;
    for (std::size_t i = 0; i != c.size(); ++i)
    {
        if (i % stride == 0)    //-V104
        {
            PIKA_TEST_EQ(c[i], std::size_t(42));
        }
        else
        {
            PIKA_TEST_NEQ(c[i], std::size_t(42));
        }
        ++count;
    }
    PIKA_TEST_EQ(count, c.size());
}

template <typename ExPolicy>
void test_for_loop_strided_idx_async(ExPolicy&& p)
{
    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::for_each(std::begin(c), std::end(c), [](std::size_t& v) -> void {
        if (v == 42)
            v = 43;
    });

    int stride = dis(gen);    //-V103

    auto f = pika::for_loop_strided(std::forward<ExPolicy>(p), 0, c.size(),
        stride, [&c](std::size_t i) { c[i] = 42; });
    f.wait();

    // verify values
    std::size_t count = 0;
    for (std::size_t i = 0; i != c.size(); ++i)
    {
        if (i % stride == 0)    //-V104
        {
            PIKA_TEST_EQ(c[i], std::size_t(42));
        }
        else
        {
            PIKA_TEST_NEQ(c[i], std::size_t(42));
        }
        ++count;
    }
    PIKA_TEST_EQ(count, c.size());
}

void for_loop_strided_test_idx()
{
    using namespace pika::execution;

    test_for_loop_strided_idx(seq);
    test_for_loop_strided_idx(par);
    test_for_loop_strided_idx(par_unseq);

    test_for_loop_strided_idx_async(seq(task));
    test_for_loop_strided_idx_async(par(task));
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    for_loop_strided_test();
    for_loop_strided_test_idx();

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
