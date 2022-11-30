//  Copyright (c) 2018 Christopher Ogle
//  Copyright (c) 2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithms/tests/iter_sent.hpp>
#include <pika/init.hpp>
#include <pika/parallel/container_algorithms/lexicographical_compare.hpp>
#include <pika/testing.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
unsigned int seed;
std::mt19937 gen;
std::uniform_int_distribution<> dis(0, 25);

void test_lexicographical_compare_sent()
{
    // ensure all characters are unique
    std::unordered_set<unsigned char> uset;

    std::vector<char> c1(7);
    std::generate(std::begin(c1), std::end(c1), [&uset]() {
        unsigned char c = 'a' + dis(gen);
        while (uset.find(c) != uset.end())
        {
            c = 'a' + dis(gen);
        }
        uset.insert(c);
        return c;
    });

    uset.clear();
    std::vector<char> c2(7);
    std::generate(std::begin(c2), std::end(c2), [&uset]() {
        unsigned char c = 'a' + dis(gen);
        while (uset.find(c) != uset.end())
        {
            c = 'a' + dis(gen);
        }
        uset.insert(c);
        return c;
    });

    bool actual_result1 = std::lexicographical_compare(
        std::begin(c1), std::begin(c1) + 5, std::begin(c2), std::begin(c2) + 5);
    bool result1 = pika::ranges::lexicographical_compare(std::begin(c1),
        sentinel<char>{*(std::begin(c1) + 5)}, std::begin(c2),
        sentinel<char>{*(std::begin(c2) + 5)});

    bool actual_result2 = std::lexicographical_compare(
        std::begin(c2), std::begin(c2) + 5, std::begin(c1), std::begin(c1) + 5);
    bool result2 = pika::ranges::lexicographical_compare(std::begin(c2),
        sentinel<char>{*(std::begin(c2) + 5)}, std::begin(c1),
        sentinel<char>{*(std::begin(c1) + 5)});

    bool actual_result3 = std::lexicographical_compare(
        std::begin(c1), std::begin(c1) + 5, std::begin(c1), std::begin(c1) + 5);
    bool result3 = pika::ranges::lexicographical_compare(std::begin(c1),
        sentinel<char>{*(std::begin(c1) + 5)}, std::begin(c1),
        sentinel<char>{*(std::begin(c1) + 5)});

    PIKA_TEST_EQ(actual_result1, result1);
    PIKA_TEST_EQ(actual_result2, result2);
    PIKA_TEST_EQ(actual_result3, result3);

    // check corner cases
    std::vector<char> c3 = {1, 1, 1, 1, 3, 2, 2, 8};
    std::vector<char> c4 = {1, 1, 1, 1, 3, 5, 5, 8};
    auto result4 = pika::ranges::lexicographical_compare(
        std::begin(c3), sentinel<char>{3}, std::begin(c4), sentinel<char>{3});
    auto result5 = pika::ranges::lexicographical_compare(
        std::begin(c3), sentinel<char>{8}, std::begin(c4), sentinel<char>{8});

    PIKA_TEST_EQ(false, result4);
    PIKA_TEST_EQ(true, result5);
}

template <typename ExPolicy>
void test_lexicographical_compare_sent(ExPolicy policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    // ensure all characters are unique
    std::unordered_set<unsigned char> uset;

    std::vector<char> c1(7);
    std::generate(std::begin(c1), std::end(c1), [&uset]() {
        unsigned char c = 'a' + dis(gen);
        while (uset.find(c) != uset.end())
        {
            c = 'a' + dis(gen);
        }
        uset.insert(c);
        return c;
    });

    uset.clear();
    std::vector<char> c2(7);
    std::generate(std::begin(c2), std::end(c2), [&uset]() {
        unsigned char c = 'a' + dis(gen);
        while (uset.find(c) != uset.end())
        {
            c = 'a' + dis(gen);
        }
        uset.insert(c);
        return c;
    });

    bool actual_result1 = std::lexicographical_compare(
        std::begin(c1), std::begin(c1) + 5, std::begin(c2), std::begin(c2) + 5);
    bool result1 = pika::ranges::lexicographical_compare(policy, std::begin(c1),
        sentinel<char>{*(std::begin(c1) + 5)}, std::begin(c2),
        sentinel<char>{*(std::begin(c2) + 5)});

    bool actual_result2 = std::lexicographical_compare(
        std::begin(c2), std::begin(c2) + 5, std::begin(c1), std::begin(c1) + 5);
    bool result2 = pika::ranges::lexicographical_compare(policy, std::begin(c2),
        sentinel<char>{*(std::begin(c2) + 5)}, std::begin(c1),
        sentinel<char>{*(std::begin(c1) + 5)});

    bool actual_result3 = std::lexicographical_compare(
        std::begin(c1), std::begin(c1) + 5, std::begin(c1), std::begin(c1) + 5);
    bool result3 = pika::ranges::lexicographical_compare(policy, std::begin(c1),
        sentinel<char>{*(std::begin(c1) + 5)}, std::begin(c1),
        sentinel<char>{*(std::begin(c1) + 5)});

    PIKA_TEST_EQ(actual_result1, result1);
    PIKA_TEST_EQ(actual_result2, result2);
    PIKA_TEST_EQ(actual_result3, result3);

    // check corner cases
    std::vector<char> c3 = {1, 1, 1, 1, 3, 2, 2, 8};
    std::vector<char> c4 = {1, 1, 1, 1, 3, 5, 5, 8};
    auto result4 = pika::ranges::lexicographical_compare(policy, std::begin(c3),
        sentinel<char>{3}, std::begin(c4), sentinel<char>{3});
    auto result5 = pika::ranges::lexicographical_compare(policy, std::begin(c3),
        sentinel<char>{8}, std::begin(c4), sentinel<char>{8});

    PIKA_TEST_EQ(false, result4);
    PIKA_TEST_EQ(true, result5);
}

template <typename IteratorTag>
void test_lexicographical_compare(IteratorTag)
{
    std::vector<char> c1(10);
    std::generate(
        std::begin(c1), std::end(c1), []() { return 'a' + dis(gen); });

    std::vector<char> c2(10);
    std::generate(
        std::begin(c2), std::end(c2), []() { return 'a' + dis(gen); });

    bool actual_result1 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    bool result1 = pika::ranges::lexicographical_compare(c1, c2);

    bool actual_result2 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    bool result2 = pika::ranges::lexicographical_compare(c1, c2);

    bool actual_result3 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    bool result3 = pika::ranges::lexicographical_compare(c1, c2);

    PIKA_TEST_EQ(actual_result1, result1);
    PIKA_TEST_EQ(actual_result2, result2);
    PIKA_TEST_EQ(actual_result3, result3);
}

template <typename ExPolicy, typename IteratorTag>
void test_lexicographical_compare(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::vector<char> c1(10);
    std::generate(
        std::begin(c1), std::end(c1), []() { return 'a' + dis(gen); });

    std::vector<char> c2(10);
    std::generate(
        std::begin(c2), std::end(c2), []() { return 'a' + dis(gen); });

    bool actual_result1 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    bool result1 = pika::ranges::lexicographical_compare(policy, c1, c2);

    bool actual_result2 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    bool result2 = pika::ranges::lexicographical_compare(policy, c1, c2);

    bool actual_result3 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    bool result3 = pika::ranges::lexicographical_compare(policy, c1, c2);

    PIKA_TEST_EQ(actual_result1, result1);
    PIKA_TEST_EQ(actual_result2, result2);
    PIKA_TEST_EQ(actual_result3, result3);
}

template <typename ExPolicy, typename IteratorTag>
void test_lexicographical_compare_async(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::vector<char> c1(10);
    std::generate(
        std::begin(c1), std::end(c1), []() { return 'a' + dis(gen); });

    std::vector<char> c2(10);
    std::generate(
        std::begin(c2), std::end(c2), []() { return 'a' + dis(gen); });

    bool actual_result1 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    pika::future<bool> result1 =
        pika::ranges::lexicographical_compare(policy, c1, c2);

    bool actual_result2 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    pika::future<bool> result2 =
        pika::ranges::lexicographical_compare(policy, c1, c2);

    bool actual_result3 = std::lexicographical_compare(
        std::begin(c1), std::end(c1), std::begin(c2), std::end(c2));
    pika::future<bool> result3 =
        pika::ranges::lexicographical_compare(policy, c1, c2);

    result1.wait();
    result2.wait();
    result3.wait();

    PIKA_TEST_EQ(actual_result1, result1.get());
    PIKA_TEST_EQ(actual_result2, result2.get());
    PIKA_TEST_EQ(actual_result3, result3.get());
}

template <typename IteratorTag>
void test_lexicographical_compare()
{
    using namespace pika::execution;

    test_lexicographical_compare(IteratorTag());
    test_lexicographical_compare(seq, IteratorTag());
    test_lexicographical_compare(par, IteratorTag());
    test_lexicographical_compare(par_unseq, IteratorTag());

    test_lexicographical_compare_async(seq(task), IteratorTag());
    test_lexicographical_compare_async(par(task), IteratorTag());

    test_lexicographical_compare_sent();
    test_lexicographical_compare_sent(seq);
    test_lexicographical_compare_sent(par);
    test_lexicographical_compare_sent(par_unseq);
}

void lexicographical_compare_test()
{
    test_lexicographical_compare<std::random_access_iterator_tag>();
    test_lexicographical_compare<std::forward_iterator_tag>();
}

////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed1 = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed1 = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed1 << std::endl;
    std::srand(seed1);

    seed = seed1;
    gen = std::mt19937(seed);

    lexicographical_compare_test();
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
