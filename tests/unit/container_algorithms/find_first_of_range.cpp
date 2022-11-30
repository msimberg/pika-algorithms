//  copyright (c) 2014 Grant Mercer
//                2018 Bruno Pitrus
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/container_algorithms/find.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(0, 10006);
std::uniform_int_distribution<> dist(0, 2);

template <typename IteratorTag>
void test_find_first_of(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;

    int find_first_of_pos = dis(gen);
    int random_sub_seq_pos = dist(gen);

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 19);
    std::size_t h[] = {1, 7, 18, 3};
    c[find_first_of_pos] = h[random_sub_seq_pos];    //-V108

    base_iterator index = pika::ranges::find_first_of(c, h);

    base_iterator test_index = std::begin(c) + find_first_of_pos;

    PIKA_TEST(index == test_index);
}

template <typename ExPolicy, typename IteratorTag>
void test_find_first_of(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;

    int find_first_of_pos = dis(gen);
    int random_sub_seq_pos = dist(gen);

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 19);
    std::size_t h[] = {1, 7, 18, 3};
    c[find_first_of_pos] = h[random_sub_seq_pos];    //-V108

    base_iterator index = pika::ranges::find_first_of(policy, c, h);

    base_iterator test_index = std::begin(c) + find_first_of_pos;

    PIKA_TEST(index == test_index);
}

template <typename IteratorTag>
void test_find_first_of_proj(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;

    int find_first_of_pos = dis(gen);
    int random_sub_seq_pos = dist(gen);

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), (gen() % 32768) + 19);
    std::size_t h[] = {1 + 65536, 7 + 65536, 18 + 65536, 3 + 65536};
    c[find_first_of_pos] = h[random_sub_seq_pos];    //-V108

    base_iterator index = pika::ranges::find_first_of(
        c, h, std::equal_to<std::size_t>(),
        [](std::size_t x) { return x % 65536; },
        [](std::size_t x) { return x % 65536; });

    base_iterator test_index = std::begin(c) + find_first_of_pos;

    PIKA_TEST(index == test_index);
}

template <typename ExPolicy, typename IteratorTag>
void test_find_first_of_proj(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;

    int find_first_of_pos = dis(gen);
    int random_sub_seq_pos = dist(gen);

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), (gen() % 32768) + 19);
    std::size_t h[] = {1 + 65536, 7 + 65536, 18 + 65536, 3 + 65536};
    c[find_first_of_pos] = h[random_sub_seq_pos];    //-V108

    base_iterator index = pika::ranges::find_first_of(
        policy, c, h, std::equal_to<std::size_t>(),
        [](std::size_t x) { return x % 65536; },
        [](std::size_t x) { return x % 65536; });

    base_iterator test_index = std::begin(c) + find_first_of_pos;

    PIKA_TEST(index == test_index);
}

template <typename ExPolicy, typename IteratorTag>
void test_find_first_of_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;

    int find_first_of_pos = dis(gen);
    int random_sub_seq_pos = dist(gen);

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 19);
    std::size_t h[] = {1, 7, 18, 3};
    c[find_first_of_pos] = h[random_sub_seq_pos];    //-V108

    pika::future<base_iterator> f = pika::ranges::find_first_of(p, c, h);
    f.wait();

    // create iterator at position of value to be found
    base_iterator test_index = std::begin(c) + find_first_of_pos;

    PIKA_TEST(f.get() == test_index);
}

template <typename ExPolicy, typename IteratorTag>
void test_find_first_of_async_proj(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;

    int find_first_of_pos = dis(gen);
    int random_sub_seq_pos = dist(gen);

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), (gen() % 32768) + 19);
    std::size_t h[] = {1 + 65536, 7 + 65536, 18 + 65536, 3 + 65536};
    c[find_first_of_pos] = h[random_sub_seq_pos];    //-V108

    pika::future<base_iterator> f = pika::ranges::find_first_of(
        p, c, h, std::equal_to<std::size_t>(),
        [](std::size_t x) { return x % 65536; },
        [](std::size_t x) { return x % 65536; });
    f.wait();

    // create iterator at position of value to be found
    base_iterator test_index = std::begin(c) + find_first_of_pos;

    PIKA_TEST(f.get() == test_index);
}

template <typename IteratorTag>
void test_find_first_of()
{
    using namespace pika::execution;

    test_find_first_of(IteratorTag());
    test_find_first_of(seq, IteratorTag());
    test_find_first_of(par, IteratorTag());
    test_find_first_of(par_unseq, IteratorTag());

    test_find_first_of_proj(IteratorTag());
    test_find_first_of_proj(seq, IteratorTag());
    test_find_first_of_proj(par, IteratorTag());
    test_find_first_of_proj(par_unseq, IteratorTag());

    test_find_first_of_async(seq(task), IteratorTag());
    test_find_first_of_async(par(task), IteratorTag());
    test_find_first_of_async_proj(seq(task), IteratorTag());
    test_find_first_of_async_proj(par(task), IteratorTag());
}

void find_first_of_test()
{
    test_find_first_of<std::random_access_iterator_tag>();
    test_find_first_of<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    find_first_of_test();
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
