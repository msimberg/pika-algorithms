//  Copyright (c) 2015 John Biddiscombe
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/testing.hpp>
#include <pika/testing/performance.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// use smaller array sizes for debug tests
#if defined(PIKA_ALGORITHMS_DEBUG)
#define PIKA_SORT_TEST_SIZE 50000L
#define PIKA_SORT_TEST_SIZE_STRINGS 10000L
#endif

#include "stable_sort_tests.hpp"

////////////////////////////////////////////////////////////////////////////////
// this function times a sort and outputs the time for CDash to plot it
void sort_benchmark()
{
    try
    {
        using namespace pika::execution;
        // Fill vector with random values
        std::vector<double> c(PIKA_SORT_TEST_SIZE << 4);
        rnd_fill<double>(c, (std::numeric_limits<double>::min)(),
            (std::numeric_limits<double>::max)(), double(std::rand()));

        pika::chrono::detail::high_resolution_timer t;
        // sort, blocking when seq, par, par_vec
        pika::stable_sort(par, c.begin(), c.end());
        std::uint64_t elapsed = t.elapsed<std::chrono::nanoseconds>();

        bool is_sorted = (verify_(c, std::less<double>(), elapsed, true) != 0);
        PIKA_TEST(is_sorted);
        if (is_sorted)
        {
            // CDash graph plotting
            pika::util::print_cdash_timing("SortDoublesTime", elapsed);
        }
    }
    catch (...)
    {
        PIKA_TEST(false);
    }
}

////////////////////////////////////////////////////////////////////////////////
void test_stable_sort1()
{
    using namespace pika::execution;

    // default comparison operator (std::less)
    test_stable_sort1(int());
    test_stable_sort1(seq, int());
    test_stable_sort1(par, int());
    test_stable_sort1(par_unseq, int());

    // default comparison operator (std::less)
    test_stable_sort1(seq, double());
    test_stable_sort1(par, double());
    test_stable_sort1(par_unseq, double());

    // default comparison operator (std::less)
    test_stable_sort1(seq, std::string());
    test_stable_sort1(par, std::string());
    test_stable_sort1(par_unseq, std::string());

    // user supplied comparison operator (std::less)
    test_stable_sort1_comp(seq, int(), std::less<std::size_t>());
    test_stable_sort1_comp(par, int(), std::less<std::size_t>());
    test_stable_sort1_comp(par_unseq, int(), std::less<std::size_t>());

    // user supplied comparison operator (std::greater)
    test_stable_sort1_comp(seq, double(), std::greater<double>());
    test_stable_sort1_comp(par, double(), std::greater<double>());
    test_stable_sort1_comp(par_unseq, double(), std::greater<double>());

    // default comparison operator (std::less)
    test_stable_sort1_comp(seq, std::string(), std::greater<std::string>());
    test_stable_sort1_comp(par, std::string(), std::greater<std::string>());
    test_stable_sort1_comp(
        par_unseq, std::string(), std::greater<std::string>());

    // Async execution, default comparison operator
    test_stable_sort1_async(seq(task), int());
    test_stable_sort1_async(par(task), char());
    test_stable_sort1_async(seq(task), double());
    test_stable_sort1_async(par(task), float());
    test_stable_sort1_async_str(seq(task));
    test_stable_sort1_async_str(par(task));

    // Async execution, user comparison operator
    test_stable_sort1_async(seq(task), int(), std::less<unsigned int>());
    test_stable_sort1_async(par(task), char(), std::less<char>());
    //
    test_stable_sort1_async(seq(task), double(), std::greater<double>());
    test_stable_sort1_async(par(task), float(), std::greater<float>());
    //
    test_stable_sort1_async_str(seq(task), std::greater<std::string>());
    test_stable_sort1_async_str(par(task), std::greater<std::string>());
}

void test_stable_sort2()
{
    using namespace pika::execution;
    // default comparison operator (std::less)
    test_stable_sort2(int());
    test_stable_sort2(seq, int());
    test_stable_sort2(par, int());
    test_stable_sort2(par_unseq, int());

    // default comparison operator (std::less)
    test_stable_sort2(seq, double());
    test_stable_sort2(par, double());
    test_stable_sort2(par_unseq, double());

    // user supplied comparison operator (std::less)
    test_stable_sort2_comp(seq, int(), std::less<std::size_t>());
    test_stable_sort2_comp(par, int(), std::less<std::size_t>());
    test_stable_sort2_comp(par_unseq, int(), std::less<std::size_t>());

    // user supplied comparison operator (std::greater)
    test_stable_sort2_comp(seq, double(), std::greater<double>());
    test_stable_sort2_comp(par, double(), std::greater<double>());
    test_stable_sort2_comp(par_unseq, double(), std::greater<double>());

    // Async execution, default comparison operator
    test_stable_sort2_async(seq(task), int());
    test_stable_sort2_async(par(task), char());
    test_stable_sort2_async(seq(task), double());
    test_stable_sort2_async(par(task), float());

    // Async execution, user comparison operator
    test_stable_sort2_async(seq(task), int(), std::less<unsigned int>());
    test_stable_sort2_async(par(task), char(), std::less<char>());
    //
    test_stable_sort2_async(seq(task), double(), std::greater<double>());
    test_stable_sort2_async(par(task), float(), std::greater<float>());
}

////////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    test_stable_sort1();
    test_stable_sort2();
    sort_benchmark();

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

    pika::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    PIKA_TEST_EQ_MSG(pika::init(pika_main, argc, argv, init_args), 0,
        "pika main exited with non-zero status");

    return 0;
}
