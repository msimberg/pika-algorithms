//  Copyright (c) 2017-2018 Taeguk Kwon
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/container_algorithms/merge.hpp>
#include <pika/testing.hpp>

#include <algorithm>
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
int seed = std::random_device{}();
std::mt19937 rng(seed);

////////////////////////////////////////////////////////////////////////////
struct user_defined_type
{
    user_defined_type() = default;
    user_defined_type(int rand_no)
      : val(rand_no)
      , name(name_list[rng() % name_list.size()])
    {
    }

    bool operator<(user_defined_type const& t) const
    {
        if (this->name < t.name)
            return true;
        else if (this->name > t.name)
            return false;
        else
            return this->val < t.val;
    }

    bool operator>(user_defined_type const& t) const
    {
        if (this->name > t.name)
            return true;
        else if (this->name < t.name)
            return false;
        else
            return this->val > t.val;
    }

    bool operator==(user_defined_type const& t) const
    {
        return this->name == t.name && this->val == t.val;
    }

    user_defined_type operator+(int val) const
    {
        user_defined_type t(*this);
        t.val += val;
        return t;
    }

    static const std::vector<std::string> name_list;

    int val;
    std::string name;
};

const std::vector<std::string> user_defined_type::name_list{
    "ABB", "ABC", "ACB", "BASE", "CAA", "CAAA", "CAAB"};

struct random_fill
{
    random_fill(int rand_base, int range)
      : gen(rng())
      , dist(rand_base - range / 2, rand_base + range / 2)
    {
    }

    int operator()()
    {
        return dist(gen);
    }

    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
};

////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename DataType>
void test_inplace_merge(ExPolicy policy, DataType)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::size_t const left_size = 300007, right_size = 123456;
    std::vector<DataType> res(left_size + right_size), sol;

    auto res_first = std::begin(res);
    auto res_middle = res_first + left_size;
    auto res_last = std::end(res);

    std::generate(res_first, res_middle, random_fill(0, 6));
    std::generate(res_middle, res_last, random_fill(0, 8));
    std::sort(res_first, res_middle);
    std::sort(res_middle, res_last);

    sol = res;
    auto sol_first = std::begin(sol);
    auto sol_middle = sol_first + left_size;
    auto sol_last = std::end(sol);

    auto result = pika::ranges::inplace_merge(policy, res, res_middle);
    std::inplace_merge(sol_first, sol_middle, sol_last);

    PIKA_TEST(result == res_last);

    bool equality = test::equal(res_first, res_last, sol_first, sol_last);

    PIKA_TEST(equality);
}

template <typename ExPolicy, typename DataType>
void test_inplace_merge_async(ExPolicy policy, DataType)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::size_t const left_size = 300007, right_size = 123456;
    std::vector<DataType> res(left_size + right_size), sol;

    auto res_first = std::begin(res);
    auto res_middle = res_first + left_size;
    auto res_last = std::end(res);

    std::generate(res_first, res_middle, random_fill(0, 6));
    std::generate(res_middle, res_last, random_fill(0, 8));
    std::sort(res_first, res_middle);
    std::sort(res_middle, res_last);

    sol = res;
    auto sol_first = std::begin(sol);
    auto sol_middle = sol_first + left_size;
    auto sol_last = std::end(sol);

    auto f = pika::ranges::inplace_merge(policy, res, res_middle);
    auto result = f.get();
    std::inplace_merge(sol_first, sol_middle, sol_last);

    PIKA_TEST(result == res_last);

    bool equality =
        test::equal(res_first, res_last, std::begin(sol), std::end(sol));

    PIKA_TEST(equality);
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag, typename DataType>
void test_inplace_merge_stable(
    ExPolicy&& policy, IteratorTag, DataType, int rand_base)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using ElemType = typename std::pair<DataType, int>;
    using base_iterator = typename std::vector<ElemType>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::size_t const left_size = 300007, right_size = 123456;
    std::vector<ElemType> res(left_size + right_size);

    base_iterator res_first = std::begin(res);
    base_iterator res_middle = res_first + left_size;
    base_iterator res_last = std::end(res);

    int no = 0;
    auto rf = random_fill(rand_base, 6);
    std::generate(res_first, res_middle, [&no, &rf]() -> std::pair<int, int> {
        return {rf(), no++};
    });
    rf = random_fill(rand_base, 8);
    std::generate(res_middle, res_last, [&no, &rf]() -> std::pair<int, int> {
        return {rf(), no++};
    });
    std::sort(res_first, res_middle);
    std::sort(res_middle, res_last);

    pika::ranges::inplace_merge(
        policy, iterator(res_first), iterator(res_middle), iterator(res_last),
        [](DataType const& a, DataType const& b) -> bool { return a < b; },
        [](ElemType const& elem) -> DataType const& {
            // This is projection.
            return elem.first;
        });

    bool stable = true;
    int check_count = 0;
    for (std::size_t i = 1u; i < left_size + right_size; ++i)
    {
        if (res[i - 1].first == res[i].first)
        {
            ++check_count;
            if (res[i - 1].second > res[i].second)
                stable = false;
        }
    }

    bool test_is_meaningful = check_count >= 100;

    PIKA_TEST(test_is_meaningful);
    PIKA_TEST(stable);
}

///////////////////////////////////////////////////////////////////////////////
template <typename DataType>
void test_inplace_merge()
{
    using namespace pika::execution;

    test_inplace_merge(seq, DataType());
    test_inplace_merge(par, DataType());
    test_inplace_merge(par_unseq, DataType());

    test_inplace_merge_async(seq(task), DataType());
    test_inplace_merge_async(par(task), DataType());
}

void test_inplace_merge()
{
    test_inplace_merge<int>();
    test_inplace_merge<user_defined_type>();
}

template <typename IteratorTag, typename DataType>
void test_inplace_merge_stable()
{
    ////////// Test cases for checking whether the algorithm is stable.
    using namespace pika::execution;

    int rand_base = rng();

    ////////// Test cases for checking whether the algorithm is stable.
    test_inplace_merge_stable(seq, IteratorTag(), int(), rand_base);
    test_inplace_merge_stable(par, IteratorTag(), int(), rand_base);
    test_inplace_merge_stable(par_unseq, IteratorTag(), int(), rand_base);
    test_inplace_merge_stable(
        seq, IteratorTag(), user_defined_type(), rand_base);
    test_inplace_merge_stable(
        par, IteratorTag(), user_defined_type(), rand_base);
    test_inplace_merge_stable(
        par_unseq, IteratorTag(), user_defined_type(), rand_base);
}

void test_inplace_merge_stable()
{
    test_inplace_merge_stable<std::random_access_iterator_tag, int>();
    test_inplace_merge_stable<std::random_access_iterator_tag,
        user_defined_type>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag, typename DataType>
void test_inplace_merge_etc(IteratorTag, DataType, int rand_base)
{
    using base_iterator = typename std::vector<DataType>::iterator;

    std::size_t const left_size = 300007, right_size = 123456;
    std::vector<DataType> res(left_size + right_size), sol, org;

    base_iterator res_first = std::begin(res);
    base_iterator res_middle = res_first + left_size;
    base_iterator res_last = std::end(res);

    std::generate(res_first, res_middle, random_fill(rand_base, 6));
    std::generate(res_middle, res_last, random_fill(rand_base, 8));
    std::sort(res_first, res_middle);
    std::sort(res_middle, res_last);

    org = sol = res;
    base_iterator sol_first = std::begin(sol);
    base_iterator sol_middle = sol_first + left_size;
    base_iterator sol_last = std::end(sol);

    // Test projection.
    {
        using iterator = test::test_iterator<base_iterator, IteratorTag>;

        sol = res = org;

        DataType val;
        pika::ranges::inplace_merge(
            iterator(res_first), iterator(res_middle), iterator(res_last),
            [](DataType const& a, DataType const& b) -> bool { return a < b; },
            [&val](DataType const&) -> DataType& {
                // This is projection.
                return val;
            });

        // The container must not be changed.
        bool equality = test::equal(res_first, res_last, sol_first, sol_last);

        PIKA_TEST(equality);
    }
}

template <typename ExPolicy, typename IteratorTag, typename DataType>
void test_inplace_merge_etc(
    ExPolicy&& policy, IteratorTag, DataType, int rand_base)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = typename std::vector<DataType>::iterator;

    std::size_t const left_size = 300007, right_size = 123456;
    std::vector<DataType> res(left_size + right_size), sol, org;

    base_iterator res_first = std::begin(res);
    base_iterator res_middle = res_first + left_size;
    base_iterator res_last = std::end(res);

    std::generate(res_first, res_middle, random_fill(rand_base, 6));
    std::generate(res_middle, res_last, random_fill(rand_base, 8));
    std::sort(res_first, res_middle);
    std::sort(res_middle, res_last);

    org = sol = res;
    base_iterator sol_first = std::begin(sol);
    base_iterator sol_middle = sol_first + left_size;
    base_iterator sol_last = std::end(sol);

    // Test projection.
    {
        using iterator = test::test_iterator<base_iterator, IteratorTag>;

        sol = res = org;

        DataType val;
        pika::ranges::inplace_merge(
            policy, iterator(res_first), iterator(res_middle),
            iterator(res_last),
            [](DataType const& a, DataType const& b) -> bool { return a < b; },
            [&val](DataType const&) -> DataType& {
                // This is projection.
                return val;
            });

        // The container must not be changed.
        bool equality = test::equal(res_first, res_last, sol_first, sol_last);

        PIKA_TEST(equality);
    }
}

template <typename IteratorTag, typename DataType>
void test_inplace_merge_etc()
{
    using namespace pika::execution;

    int rand_base = rng();

    ////////// Another test cases for justifying the implementation.
    test_inplace_merge_etc(IteratorTag(), DataType(), rand_base);
    test_inplace_merge_etc(seq, IteratorTag(), DataType(), rand_base);
    test_inplace_merge_etc(par, IteratorTag(), DataType(), rand_base);
    test_inplace_merge_etc(par_unseq, IteratorTag(), DataType(), rand_base);
}

void test_inplace_merge_etc()
{
    test_inplace_merge_stable<std::random_access_iterator_tag, int>();
    test_inplace_merge_stable<std::random_access_iterator_tag,
        user_defined_type>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    if (vm.count("seed"))
    {
        seed = vm["seed"].as<unsigned int>();
        rng.seed(seed);
    }
    std::cout << "using seed: " << seed << std::endl;

    test_inplace_merge();
    test_inplace_merge_etc();
    test_inplace_merge_stable();

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
