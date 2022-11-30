//  Copyright (c) 2017 Jeff Trull
//  Copyright (c) 2017 Taeguk Kwon
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithm.hpp>
#include <pika/execution.hpp>
#include <pika/init.hpp>
#include <pika/testing.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
auto seed = std::random_device{}();
std::mt19937 rng(seed);

struct random_fill
{
    random_fill() = default;
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

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename DataType>
void test_merge_stable(ExPolicy policy, DataType, int rand_base)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using ElemType = typename std::pair<DataType, int>;

    using std::get;

    std::size_t const size1 = 1000007, size2 = 960202;
    std::vector<ElemType> src1(size1), src2(size2), dest(size1 + size2);

    int no = 0;
    auto rf = random_fill(rand_base, 6);
    std::generate(
        std::begin(src1), std::end(src1), [&no, &rf]() -> std::pair<int, int> {
            return {rf(), no++};
        });
    rf = random_fill(rand_base, 8);
    std::generate(
        std::begin(src2), std::end(src2), [&no, &rf]() -> std::pair<int, int> {
            return {rf(), no++};
        });
    std::sort(std::begin(src1), std::end(src1));
    std::sort(std::begin(src2), std::end(src2));

    pika::ranges::merge(
        policy, std::begin(src1), std::end(src1), std::begin(src2),
        std::end(src2), std::begin(dest),
        [](DataType const& a, DataType const& b) -> bool { return a < b; },
        [](ElemType const& elem) -> DataType const& {
            // This is projection.
            return elem.first;
        },
        [](ElemType const& elem) -> DataType const& {
            // This is projection.
            return elem.first;
        });

    bool stable = true;
    int check_count = 0;
    for (auto i = 1u; i < size1 + size2; ++i)
    {
        if (dest[i - 1].first == dest[i].first)
        {
            ++check_count;
            if (dest[i - 1].second > dest[i].second)
                stable = false;
        }
    }

    bool test_is_meaningful = check_count >= 100;

    PIKA_TEST(test_is_meaningful);
    PIKA_TEST(stable);
}

int pika_main()
{
    using ElemType = std::tuple<int, char>;

    // these two vectors are sorted by the first value of each tuple
    std::vector<ElemType> a1{std::make_tuple(1, 'a'), std::make_tuple(2, 'b'),
        std::make_tuple(3, 'a'), std::make_tuple(3, 'b'),
        std::make_tuple(4, 'a'), std::make_tuple(5, 'a'),
        std::make_tuple(5, 'b')};
    std::vector<ElemType> a2{std::make_tuple(0, 'c'), std::make_tuple(3, 'c'),
        std::make_tuple(4, 'c'), std::make_tuple(5, 'c')};

    std::vector<ElemType> result(a1.size() + a2.size());
    std::vector<ElemType> solution(a1.size() + a2.size());

    // I expect a stable merge to order {3, 'a'} and {3, 'b'} before {3, 'c'}
    // because they come from the first sequence
    pika::ranges::merge(pika::execution::par, a1.begin(), a1.end(), a2.begin(),
        a2.end(), result.begin(), [](ElemType const& a, ElemType const& b) {
            return std::get<0>(a) < std::get<0>(b);
        });
    std::merge(a1.begin(), a1.end(), a2.begin(), a2.end(), solution.begin(),
        [](ElemType const& a, ElemType const& b) {
            return std::get<0>(a) < std::get<0>(b);
        });

    PIKA_TEST(result == solution);

    // Expect {3, 'c'}, {3, 'a'}, {3, 'b'} in order.
    pika::ranges::merge(pika::execution::par, a2.begin(), a2.end(), a1.begin(),
        a1.end(), result.begin(), [](ElemType const& a, ElemType const& b) {
            return std::get<0>(a) < std::get<0>(b);
        });
    std::merge(a2.begin(), a2.end(), a1.begin(), a1.end(), solution.begin(),
        [](ElemType const& a, ElemType const& b) {
            return std::get<0>(a) < std::get<0>(b);
        });

    PIKA_TEST(result == solution);

    // Do modulus operation for avoiding overflow in ramdom_fill. (#2954)
    std::uniform_int_distribution<> dis(0, 9999);
    int rand_base = dis(rng);

    using namespace pika::execution;

    test_merge_stable(seq, int(), rand_base);
    test_merge_stable(par, int(), rand_base);
    test_merge_stable(par_unseq, int(), rand_base);

    return pika::finalize();
}

int main(int argc, char* argv[])
{
    // By default this should run on all available cores
    std::vector<std::string> const cfg = {"pika.os_threads=1"};

    // Initialize and run pika
    pika::init_params init_args;
    init_args.cfg = cfg;

    return pika::init(pika_main, argc, argv, init_args);
}
