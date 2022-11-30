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

template <typename ExPolicy>
void test_for_loop_reduction_plus(ExPolicy&& policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using iterator = std::vector<std::size_t>::iterator;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t sum = 0;
    auto f = pika::ranges::for_loop(std::forward<ExPolicy>(policy), c,
        pika::reduction_plus(sum),
        [](iterator it, std::size_t& sum) { sum += *it; });
    f.wait();

    // verify values
    std::size_t sum2 =
        std::accumulate(std::begin(c), std::end(c), std::size_t(0));
    PIKA_TEST_EQ(sum, sum2);
}

template <typename ExPolicy>
void test_for_loop_reduction_multiplies(ExPolicy&& policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using iterator = std::vector<std::size_t>::iterator;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t prod = 0;
    auto f = pika::ranges::for_loop(std::forward<ExPolicy>(policy), c,
        pika::reduction_multiplies(prod),
        [](iterator it, std::size_t& prod) { prod *= *it; });
    f.wait();

    // verify values
    std::size_t prod2 = std::accumulate(std::begin(c), std::end(c),
        std::size_t(1), std::multiplies<std::size_t>());
    PIKA_TEST_EQ(prod, prod2);
}

template <typename ExPolicy>
void test_for_loop_reduction_min(ExPolicy&& policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using iterator = std::vector<std::size_t>::iterator;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t minval = c[0];

    auto f = pika::ranges::for_loop(std::forward<ExPolicy>(policy), c,
        pika::reduction_min(minval), [](iterator it, std::size_t& minval) {
            minval = (std::min)(minval, *it);
        });
    f.wait();

    // verify values
    std::size_t minval2 = std::accumulate(std::begin(c), std::end(c), c[0],
        pika::parallel::detail::min_of<std::size_t>());
    PIKA_TEST_EQ(minval, minval2);
}

///////////////////////////////////////////////////////////////////////////////
void test_for_loop_reduction()
{
    test_for_loop_reduction_plus(pika::execution::seq(pika::execution::task));
    test_for_loop_reduction_plus(pika::execution::par(pika::execution::task));

    test_for_loop_reduction_multiplies(
        pika::execution::seq(pika::execution::task));
    test_for_loop_reduction_multiplies(
        pika::execution::par(pika::execution::task));

    test_for_loop_reduction_min(pika::execution::seq(pika::execution::task));
    test_for_loop_reduction_min(pika::execution::par(pika::execution::task));
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    test_for_loop_reduction();

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
