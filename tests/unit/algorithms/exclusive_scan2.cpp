//  Copyright (c) 2014-2015 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/exclusive_scan.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_exclusive_scan2(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    pika::exclusive_scan(
        iterator(std::begin(c)), iterator(std::end(c)), std::begin(d), val);

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_exclusive_scan(std::begin(c),
        std::end(c), std::begin(e), val, std::plus<std::size_t>());

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));
}

template <typename ExPolicy, typename IteratorTag>
void test_exclusive_scan2(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    pika::exclusive_scan(policy, iterator(std::begin(c)), iterator(std::end(c)),
        std::begin(d), val);

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_exclusive_scan(std::begin(c),
        std::end(c), std::begin(e), val, std::plus<std::size_t>());

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));
}

template <typename ExPolicy, typename IteratorTag>
void test_exclusive_scan2_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    pika::future<void> f = pika::exclusive_scan(
        p, iterator(std::begin(c)), iterator(std::end(c)), std::begin(d), val);
    f.wait();

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_exclusive_scan(std::begin(c),
        std::end(c), std::begin(e), val, std::plus<std::size_t>());

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));
}

template <typename IteratorTag>
void test_exclusive_scan2()
{
    using namespace pika::execution;

    test_exclusive_scan2(IteratorTag());
    test_exclusive_scan2(seq, IteratorTag());
    test_exclusive_scan2(par, IteratorTag());
    test_exclusive_scan2(par_unseq, IteratorTag());

    test_exclusive_scan2_async(seq(task), IteratorTag());
    test_exclusive_scan2_async(par(task), IteratorTag());
}

void exclusive_scan_test2()
{
    test_exclusive_scan2<std::random_access_iterator_tag>();
    test_exclusive_scan2<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    exclusive_scan_test2();

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
