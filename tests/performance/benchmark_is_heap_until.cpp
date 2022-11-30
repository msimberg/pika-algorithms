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
#include <pika/parallel/algorithms/generate.hpp>
#include <pika/parallel/algorithms/is_heap.hpp>

#include <fmt/ostream.h>
#include <fmt/printf.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
unsigned int seed = (unsigned int) std::random_device{}();
std::mt19937 rng(seed);
///////////////////////////////////////////////////////////////////////////////

struct random_fill
{
    random_fill()
      : gen(rng())
      , dist(0, RAND_MAX)
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
double run_is_heap_until_benchmark_std(
    int test_count, std::vector<int> const& v)
{
    std::cout << "--- run_is_heap_until_benchmark_std ---" << std::endl;
    decltype(std::begin(v)) result;
    using namespace std::chrono;
    auto time = high_resolution_clock::now();

    for (int i = 0; i < test_count; ++i)
    {
        result = std::is_heap_until(std::begin(v), std::end(v));
    }

    duration<double> dur = high_resolution_clock::now() - time;

    auto heap_range = std::distance(std::begin(v), result);
    std::cout << "Heap Range : " << heap_range << std::endl;

    return dur.count() / test_count;
}

///////////////////////////////////////////////////////////////////////////////
double run_is_heap_until_benchmark_seq(
    int test_count, std::vector<int> const& v)
{
    std::cout << "--- run_is_heap_until_benchmark_seq ---" << std::endl;
    decltype(std::begin(v)) result;
    using namespace std::chrono;
    auto time = high_resolution_clock::now();

    for (int i = 0; i < test_count; ++i)
    {
        using namespace pika::execution;
        result = pika::is_heap_until(seq, std::begin(v), std::end(v));
    }

    duration<double> dur = high_resolution_clock::now() - time;

    auto heap_range = std::distance(std::begin(v), result);
    std::cout << "Heap Range : " << heap_range << std::endl;

    return dur.count() / test_count;
}

///////////////////////////////////////////////////////////////////////////////
double run_is_heap_until_benchmark_par(
    int test_count, std::vector<int> const& v)
{
    std::cout << "--- run_is_heap_until_benchmark_par ---" << std::endl;
    decltype(std::begin(v)) result;
    using namespace std::chrono;
    auto time = high_resolution_clock::now();

    for (int i = 0; i < test_count; ++i)
    {
        using namespace pika::execution;
        result = pika::is_heap_until(par, std::begin(v), std::end(v));
    }

    duration<double> dur = high_resolution_clock::now() - time;

    auto heap_range = std::distance(std::begin(v), result);
    std::cout << "Heap Range : " << heap_range << std::endl;

    return dur.count() / test_count;
}

///////////////////////////////////////////////////////////////////////////////
double run_is_heap_until_benchmark_par_unseq(
    int test_count, std::vector<int> const& v)
{
    std::cout << "--- run_is_heap_until_benchmark_par_unseq ---" << std::endl;
    decltype(std::begin(v)) result;
    using namespace std::chrono;
    auto time = high_resolution_clock::now();

    for (int i = 0; i < test_count; ++i)
    {
        using namespace pika::execution;
        result = pika::is_heap_until(par_unseq, std::begin(v), std::end(v));
    }

    duration<double> dur = high_resolution_clock::now() - time;

    auto heap_range = std::distance(std::begin(v), result);
    std::cout << "Heap Range : " << heap_range << std::endl;

    return dur.count() / test_count;
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
    std::size_t break_pos = vm["break_pos"].as<std::size_t>();
    int test_count = vm["test_count"].as<int>();

    std::size_t const os_threads = pika::get_os_thread_count();

    if (break_pos > vector_size)
        break_pos = vector_size;

    std::cout << "-------------- Benchmark Config --------------" << std::endl;
    std::cout << "seed        : " << seed << std::endl;
    std::cout << "vector_size : " << vector_size << std::endl;
    std::cout << "break_pos   : " << break_pos << std::endl;
    std::cout << "test_count  : " << test_count << std::endl;
    std::cout << "os threads  : " << os_threads << std::endl;
    std::cout << "----------------------------------------------\n"
              << std::endl;

    std::cout << "* Preparing Benchmark..." << std::endl;
    std::vector<int> v(vector_size);

    // initialize data
    using namespace pika::execution;
    pika::generate(par, std::begin(v), std::end(v), random_fill());
    std::make_heap(std::begin(v), std::next(std::begin(v), break_pos));
    if (break_pos < vector_size)
        v[break_pos] =
            static_cast<int>((std::numeric_limits<std::size_t>::max)());

    std::cout << "* Running Benchmark..." << std::endl;
    double time_std = run_is_heap_until_benchmark_std(test_count, v);
    double time_seq = run_is_heap_until_benchmark_seq(test_count, v);
    double time_par = run_is_heap_until_benchmark_par(test_count, v);
    double time_par_unseq =
        run_is_heap_until_benchmark_par_unseq(test_count, v);

    std::cout << "\n-------------- Benchmark Result --------------"
              << std::endl;
    constexpr auto fmt = "is_heap_until ({}) : {}(sec)\n";
    fmt::print(std::cout, fmt, "std", time_std);
    fmt::print(std::cout, fmt, "seq", time_seq);
    fmt::print(std::cout, fmt, "par", time_par);
    fmt::print(std::cout, fmt, "par_unseq", time_par_unseq);
    std::cout << "----------------------------------------------" << std::endl;

    return pika::finalize();
}

int main(int argc, char* argv[])
{
    using namespace pika::program_options;
    options_description desc_commandline(
        "usage: " PIKA_APPLICATION_STRING " [options]");

    desc_commandline.add_options()("vector_size",
        pika::program_options::value<std::size_t>()->default_value(1000000),
        "size of vector (default: 1000000)")("break_pos",
        pika::program_options::value<std::size_t>()->default_value(
            (std::numeric_limits<std::size_t>::max)()),
        "a position which breaks max heap (default: vector_size)")("test_count",
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
