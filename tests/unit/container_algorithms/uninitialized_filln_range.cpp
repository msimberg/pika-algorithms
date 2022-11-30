//  Copyright (c) 2014 Grant Mercer
//  Copyright (c) 2015 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/iterator_support/iterator_range.hpp>
#include <pika/parallel/container_algorithms/uninitialized_fill.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_uninitialized_fill_n_sent(IteratorTag)
{
    std::vector<std::size_t> c(10007);
    std::size_t sent_len = (std::rand() % 10007) + 1;
    pika::ranges::uninitialized_fill_n(std::begin(c), sent_len, 10);

    std::size_t count = 0;
    std::for_each(std::begin(c), std::begin(c) + sent_len,
        [&count](std::size_t v) -> void {
            PIKA_TEST_EQ(v, std::size_t(10));
            ++count;
        });

    PIKA_TEST_EQ(count, sent_len);
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_fill_n_sent(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::vector<std::size_t> c(10007);
    std::size_t sent_len = (std::rand() % 10007) + 1;
    pika::ranges::uninitialized_fill_n(policy, std::begin(c), sent_len, 10);

    std::size_t count = 0;
    std::for_each(std::begin(c), std::begin(c) + sent_len,
        [&count](std::size_t v) -> void {
            PIKA_TEST_EQ(v, std::size_t(10));
            ++count;
        });

    PIKA_TEST_EQ(count, sent_len);
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_fill_n_sent_async(ExPolicy&& p, IteratorTag)
{
    std::vector<std::size_t> c(10007);
    std::size_t sent_len = (std::rand() % 10007) + 1;
    auto f = pika::ranges::uninitialized_fill_n(p, std::begin(c), sent_len, 10);
    f.wait();

    std::size_t count = 0;
    std::for_each(std::begin(c), std::begin(c) + sent_len,
        [&count](std::size_t v) -> void {
            PIKA_TEST_EQ(v, std::size_t(10));
            ++count;
        });

    PIKA_TEST_EQ(count, sent_len);
}

template <typename IteratorTag>
void test_uninitialized_fill_n_sent()
{
    using namespace pika::execution;

    test_uninitialized_fill_n_sent(IteratorTag());

    test_uninitialized_fill_n_sent(seq, IteratorTag());
    test_uninitialized_fill_n_sent(par, IteratorTag());
    test_uninitialized_fill_n_sent(par_unseq, IteratorTag());

    test_uninitialized_fill_n_sent_async(seq(task), IteratorTag());
    test_uninitialized_fill_n_sent_async(par(task), IteratorTag());
}

void uninitialized_fill_n_sent_test()
{
    test_uninitialized_fill_n_sent<std::random_access_iterator_tag>();
    test_uninitialized_fill_n_sent<std::forward_iterator_tag>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    uninitialized_fill_n_sent_test();
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
