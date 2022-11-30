//  Copyright (c) 2014-2016 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>

#include <iostream>
#include <string>
#include <vector>

#include "stable_partition_tests.hpp"

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_stable_partition()
{
    using namespace pika::execution;

    test_stable_partition(IteratorTag());
    test_stable_partition(seq, IteratorTag());
    test_stable_partition(par, IteratorTag());
    test_stable_partition(par_unseq, IteratorTag());

    test_stable_partition_async(seq(task), IteratorTag());
    test_stable_partition_async(par(task), IteratorTag());
}

void stable_partition_test()
{
    test_stable_partition<std::random_access_iterator_tag>();
    test_stable_partition<std::bidirectional_iterator_tag>();
}

template <typename IteratorTag>
void test_stable_partition_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_stable_partition_exception(seq, IteratorTag());
    test_stable_partition_exception(par, IteratorTag());

    test_stable_partition_exception_async(seq(task), IteratorTag());
    test_stable_partition_exception_async(par(task), IteratorTag());
}

void stable_partition_exception_test()
{
    test_stable_partition_exception<std::random_access_iterator_tag>();
    test_stable_partition_exception<std::bidirectional_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_stable_partition_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_stable_partition_bad_alloc(seq, IteratorTag());
    test_stable_partition_bad_alloc(par, IteratorTag());

    test_stable_partition_bad_alloc_async(seq(task), IteratorTag());
    test_stable_partition_bad_alloc_async(par(task), IteratorTag());
}

void stable_partition_bad_alloc_test()
{
    test_stable_partition_bad_alloc<std::random_access_iterator_tag>();
    test_stable_partition_bad_alloc<std::bidirectional_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    stable_partition_test();
    stable_partition_exception_test();
    stable_partition_bad_alloc_test();

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
