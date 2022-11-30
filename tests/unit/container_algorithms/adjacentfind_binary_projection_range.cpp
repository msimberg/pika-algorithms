//  Copyright (c) 2014 Grant Mercer
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithm.hpp>
#include <pika/init.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(2, 101);
std::uniform_int_distribution<> dist(2, 10005);

struct negate
{
    template <typename T>
    T operator()(T x)
    {
        return -x;
    }
};

template <typename ExPolicy, typename Proj, typename IteratorTag>
void test_adjacent_find(ExPolicy policy, Proj&& proj, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<int> c(10007);
    //fill vector with random values about 1
    std::iota(std::begin(c), std::end(c), dis(gen));

    std::size_t random_pos = dist(gen);    //-V101

    c[random_pos] = 100000;
    c[random_pos + 1] = 1;

    iterator index =
        pika::ranges::adjacent_find(policy, iterator(std::begin(c)),
            iterator(std::end(c)), std::less<int>(), std::forward<Proj>(proj));

    base_iterator test_index = std::begin(c) + random_pos;

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename Proj, typename IteratorTag>
void test_adjacent_find_async(ExPolicy p, Proj&& proj, IteratorTag)
{
    using base_iterator = std::vector<int>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    // fill vector with random values above 1
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), dis(gen));

    std::size_t random_pos = dist(gen);    //-V101

    c[random_pos] = 100000;
    c[random_pos + 1] = 1;

    pika::future<iterator> f =
        pika::ranges::adjacent_find(p, iterator(std::begin(c)),
            iterator(std::end(c)), std::less<int>(), std::forward<Proj>(proj));
    f.wait();

    // create iterator at position of value to be found
    base_iterator test_index = std::begin(c) + random_pos;
    PIKA_TEST(f.get() == iterator(test_index));
}

template <typename ExPolicy, typename Proj>
void test_adjacent_find(ExPolicy policy, Proj&& proj)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using iterator = std::vector<int>::iterator;

    std::vector<int> c(10007);
    //fill vector with random values about 1
    std::iota(std::begin(c), std::end(c), dis(gen));

    std::size_t random_pos = dist(gen);    //-V101

    c[random_pos] = 100000;
    c[random_pos + 1] = 1;

    iterator index = pika::ranges::adjacent_find(
        policy, c, std::less<int>(), std::forward<Proj>(proj));

    iterator test_index = std::begin(c) + random_pos;

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename Proj>
void test_adjacent_find_async(ExPolicy p, Proj&& proj)
{
    using iterator = std::vector<int>::iterator;

    // fill vector with random values above 1
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), dis(gen));

    std::size_t random_pos = dist(gen);    //-V101

    c[random_pos] = 100000;
    c[random_pos + 1] = 1;

    pika::future<iterator> f = pika::ranges::adjacent_find(
        p, c, std::less<int>(), std::forward<Proj>(proj));
    f.wait();

    // create iterator at position of value to be found
    iterator test_index = std::begin(c) + random_pos;
    PIKA_TEST(f.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_adjacent_find()
{
    using namespace pika::execution;
    test_adjacent_find(seq, negate(), IteratorTag());
    test_adjacent_find(par, negate(), IteratorTag());
    test_adjacent_find(par_unseq, negate(), IteratorTag());

    test_adjacent_find_async(seq(task), negate(), IteratorTag());
    test_adjacent_find_async(par(task), negate(), IteratorTag());
}

void adjacent_find_test()
{
    using namespace pika::execution;
    test_adjacent_find<std::random_access_iterator_tag>();
    test_adjacent_find<std::forward_iterator_tag>();

    test_adjacent_find(seq, negate());
    test_adjacent_find(par, negate());
    test_adjacent_find(par_unseq, negate());

    test_adjacent_find_async(seq(task), negate());
    test_adjacent_find_async(par(task), negate());
}

int pika_main(pika::program_options::variables_map& vm)
{
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    adjacent_find_test();
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
