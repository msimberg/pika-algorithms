///////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 2017 Taeguk Kwon
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////////////////////

#include <pika/init.hpp>
#include <pika/modules/program_options.hpp>
#include <pika/modules/timing.hpp>
#include <pika/parallel/algorithms/copy.hpp>
#include <pika/parallel/algorithms/generate.hpp>
#include <pika/parallel/algorithms/merge.hpp>
#include <pika/parallel/algorithms/sort.hpp>

#include <fmt/ostream.h>
#include <fmt/printf.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "utils.hpp"

///////////////////////////////////////////////////////////////////////////////
unsigned int seed = (unsigned int) std::random_device{}();
std::mt19937 rng(seed);
///////////////////////////////////////////////////////////////////////////////

struct random_fill
{
    random_fill(std::size_t random_range)
      : gen(rng())
      , dist(0, random_range - 1)
    {
    }

    int operator()()
    {
        return dist(gen);
    }

    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
};

///////////////////////////////////////////////////////////////////////////////
template <typename OrgIter, typename BidirIter>
double run_inplace_merge_benchmark_std(int test_count, OrgIter org_first,
    OrgIter org_last, BidirIter first, BidirIter middle, BidirIter last)
{
    using namespace std::chrono;
    auto dur = duration<double>(0);

    for (int i = 0; i < test_count; ++i)
    {
        // Restore [first, last) with original data.
        pika::copy(pika::execution::par, org_first, org_last, first);

        auto start = high_resolution_clock::now();
        std::inplace_merge(first, middle, last);
        dur += high_resolution_clock::now() - start;
    }

    return dur.count() / test_count;
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename OrgIter, typename BidirIter>
double run_inplace_merge_benchmark_pika(int test_count, ExPolicy policy,
    OrgIter org_first, OrgIter org_last, BidirIter first, BidirIter middle,
    BidirIter last)
{
    using namespace std::chrono;
    auto dur = duration<double>(0);

    for (int i = 0; i < test_count; ++i)
    {
        // Restore [first, last) with original data.
        pika::copy(pika::execution::par, org_first, org_last, first);

        auto start = high_resolution_clock::now();
        pika::inplace_merge(policy, first, middle, last);
        dur += high_resolution_clock::now() - start;
    }

    return dur.count() / test_count;
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void run_benchmark(std::size_t vector_left_size, std::size_t vector_right_size,
    int test_count, std::size_t random_range, IteratorTag)
{
    std::cout << "* Preparing Benchmark..." << std::endl;

    using test_container = test_container<IteratorTag>;
    using container = typename test_container::type;

    container c =
        test_container::get_container(vector_left_size + vector_right_size);
    container org_c;

    auto first = std::begin(c);
    auto middle = first + vector_left_size;
    auto last = std::end(c);

    // initialize data
    using namespace pika::execution;
    pika::generate(par, first, middle, random_fill(random_range));
    pika::generate(par, middle, last, random_fill(random_range));
    pika::sort(par, first, middle);
    pika::sort(par, middle, last);
    org_c = c;

    auto org_first = std::begin(org_c);
    auto org_last = std::end(org_c);

    std::cout << "* Running Benchmark..." << std::endl;
    std::cout << "--- run_inplace_merge_benchmark_std ---" << std::endl;
    double time_std = run_inplace_merge_benchmark_std(
        test_count, org_first, org_last, first, middle, last);

    std::cout << "--- run_inplace_merge_benchmark_seq ---" << std::endl;
    double time_seq = run_inplace_merge_benchmark_pika(
        test_count, seq, org_first, org_last, first, middle, last);

    std::cout << "--- run_inplace_merge_benchmark_par ---" << std::endl;
    double time_par = run_inplace_merge_benchmark_pika(
        test_count, par, org_first, org_last, first, middle, last);

    std::cout << "--- run_inplace_merge_benchmark_par_unseq ---" << std::endl;
    double time_par_unseq = run_inplace_merge_benchmark_pika(
        test_count, par_unseq, org_first, org_last, first, middle, last);

    std::cout << "\n-------------- Benchmark Result --------------"
              << std::endl;
    constexpr auto fmt = "inplace_merge ({}) : {}(sec)\n";
    fmt::print(std::cout, fmt, "std", time_std);
    fmt::print(std::cout, fmt, "seq", time_seq);
    fmt::print(std::cout, fmt, "par", time_par);
    fmt::print(std::cout, fmt, "par_unseq", time_par_unseq);
    std::cout << "----------------------------------------------" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
std::string correct_iterator_tag_str(std::string iterator_tag)
{
    if (iterator_tag != "random"/* &&
        iterator_tag != "bidirectional"*/)
        return "random";
    else
        return iterator_tag;
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    if (vm.count("seed"))
    {
        seed = vm["seed"].as<unsigned int>();
        rng.seed(seed);
    }

    // pull values from cmd
    std::size_t vector_size = vm["vector_size"].as<std::size_t>();
    double vector_ratio = vm["vector_ratio"].as<double>();
    std::size_t random_range = vm["random_range"].as<std::size_t>();
    int test_count = vm["test_count"].as<int>();
    std::string iterator_tag_str =
        correct_iterator_tag_str(vm["iterator_tag"].as<std::string>());

    std::size_t const os_threads = pika::get_os_thread_count();

    if (random_range < 1)
        random_range = 1;

    std::size_t vector_left_size = std::size_t(vector_size * vector_ratio);
    std::size_t vector_right_size = vector_size - vector_left_size;

    std::cout << "-------------- Benchmark Config --------------" << std::endl;
    std::cout << "seed              : " << seed << std::endl;
    std::cout << "vector_left_size  : " << vector_left_size << std::endl;
    std::cout << "vector_right_size : " << vector_right_size << std::endl;
    std::cout << "random_range      : " << random_range << std::endl;
    std::cout << "iterator_tag      : " << iterator_tag_str << std::endl;
    std::cout << "test_count        : " << test_count << std::endl;
    std::cout << "os threads        : " << os_threads << std::endl;
    std::cout << "----------------------------------------------\n"
              << std::endl;

    if (iterator_tag_str == "random")
        run_benchmark(vector_left_size, vector_right_size, test_count,
            random_range, std::random_access_iterator_tag());
    //else // bidirectional
    //    run_benchmark(vector_left_size, vector_right_size,
    //        test_count, random_range,
    //        std::bidirectional_iterator_tag());

    return pika::finalize();
}

int main(int argc, char* argv[])
{
    using namespace pika::program_options;
    options_description desc_commandline(
        "usage: " PIKA_APPLICATION_STRING " [options]");

    desc_commandline.add_options()("vector_size",
        pika::program_options::value<std::size_t>()->default_value(1000000),
        "sum of sizes of two vectors (default: 1000000)")("vector_ratio",
        pika::program_options::value<double>()->default_value(0.7),
        "ratio of two vector sizes (default: 0.7)")("random_range",
        pika::program_options::value<std::size_t>()->default_value(6),
        "range of random numbers [0, x) (default: 6)")("iterator_tag",
        pika::program_options::value<std::string>()->default_value("random"),
        "the kind of iterator tag (random/bidirectional/forward)")("test_count",
        pika::program_options::value<int>()->default_value(10),
        "number of tests to be averaged (default: 10)")("seed,s",
        pika::program_options::value<unsigned int>(),
        "the random number generator seed to use for this run");

    // initialize program
    std::vector<std::string> const cfg = {"pika.os_threads=all"};

    // Initialize and run pika
    pika::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    return pika::init(pika_main, argc, argv, init_args);
}
