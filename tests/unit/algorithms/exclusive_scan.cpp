//  Copyright (c) 2014-2015 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/exclusive_scan.hpp>
#include <pika/testing.hpp>
#include <pika/testing/performance.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
void exclusive_scan_benchmark()
{
    try
    {
#if defined(PIKA_ALGORITHMS_DEBUG)
        std::vector<double> c(1000000);
#else
        std::vector<double> c(100000000);
#endif
        std::vector<double> d(c.size());
        std::fill(std::begin(c), std::end(c), 1.0);

        double const val(0);
        auto op = [](double v1, double v2) { return v1 + v2; };

        pika::chrono::detail::high_resolution_timer t;
        pika::exclusive_scan(pika::execution::par, std::begin(c), std::end(c),
            std::begin(d), val, op);
        double elapsed = t.elapsed();

        // verify values
        std::vector<double> e(c.size());
        pika::parallel::detail::sequential_exclusive_scan(
            std::begin(c), std::end(c), std::begin(e), val, op);

        bool ok = std::equal(std::begin(d), std::end(d), std::begin(e));
        PIKA_TEST(ok);
        if (ok)
        {
            // CDash graph plotting
            pika::util::print_cdash_timing("ExclusiveScanTime", elapsed);
        }
    }
    catch (...)
    {
        PIKA_TEST(false);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_exclusive_scan1(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };

    pika::exclusive_scan(
        iterator(std::begin(c)), iterator(std::end(c)), std::begin(d), val, op);

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_exclusive_scan(
        std::begin(c), std::end(c), std::begin(e), val, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));

#if defined(PIKA_ALGORITHMS_HAVE_CXX17_STD_SCAN_ALGORITHMS)
    std::vector<std::size_t> f(c.size());
    std::exclusive_scan(std::begin(c), std::end(c), std::begin(f), val, op);
    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(f)));
#endif
}

template <typename ExPolicy, typename IteratorTag>
void test_exclusive_scan1(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };

    pika::exclusive_scan(policy, iterator(std::begin(c)), iterator(std::end(c)),
        std::begin(d), val, op);

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_exclusive_scan(
        std::begin(c), std::end(c), std::begin(e), val, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));

#if defined(PIKA_ALGORITHMS_HAVE_CXX17_STD_SCAN_ALGORITHMS)
    std::vector<std::size_t> f(c.size());
    std::exclusive_scan(std::begin(c), std::end(c), std::begin(f), val, op);
    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(f)));
#endif
}

template <typename ExPolicy, typename IteratorTag>
void test_exclusive_scan1_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };

    pika::future<void> fut = pika::exclusive_scan(p, iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(d), val, op);
    fut.wait();

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_exclusive_scan(
        std::begin(c), std::end(c), std::begin(e), val, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));

#if defined(PIKA_ALGORITHMS_HAVE_CXX17_STD_SCAN_ALGORITHMS)
    std::vector<std::size_t> f(c.size());
    std::exclusive_scan(std::begin(c), std::end(c), std::begin(f), val, op);
    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(f)));
#endif
}

template <typename IteratorTag>
void test_exclusive_scan1()
{
    using namespace pika::execution;

    test_exclusive_scan1(IteratorTag());
    test_exclusive_scan1(seq, IteratorTag());
    test_exclusive_scan1(par, IteratorTag());
    test_exclusive_scan1(par_unseq, IteratorTag());

    test_exclusive_scan1_async(seq(task), IteratorTag());
    test_exclusive_scan1_async(par(task), IteratorTag());
}

void exclusive_scan_test1()
{
    test_exclusive_scan1<std::random_access_iterator_tag>();
    test_exclusive_scan1<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    exclusive_scan_test1();
    exclusive_scan_benchmark();

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
