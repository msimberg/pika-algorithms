//  Copyright (c) 2020 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithms/tests/iter_sent.hpp>
#include <pika/init.hpp>
#include <pika/parallel/container_algorithms/for_each.hpp>
#include <pika/testing.hpp>

#include <cstdint>

void myfunction(std::int64_t) {}

void test_invoke_projected()
{
    iterator<std::int64_t> iter = pika::ranges::for_each(pika::execution::seq,
        iterator<std::int64_t>{0}, sentinel<std::int64_t>{100}, myfunction);

    PIKA_TEST_EQ(*iter, std::int64_t(100));

    iter = pika::ranges::for_each(pika::execution::par,
        iterator<std::int64_t>{0}, sentinel<std::int64_t>{100}, myfunction);

    PIKA_TEST_EQ(*iter, std::int64_t(100));
}

void test_begin_end_iterator()
{
    iterator<std::int64_t> iter = pika::ranges::for_each(pika::execution::seq,
        iterator<std::int64_t>{0}, sentinel<std::int64_t>{100}, &myfunction);

    PIKA_TEST_EQ(*iter, std::int64_t(100));

    iter = pika::ranges::for_each(pika::execution::par,
        iterator<std::int64_t>{0}, sentinel<std::int64_t>{100}, &myfunction);

    PIKA_TEST_EQ(*iter, std::int64_t(100));
}

int pika_main()
{
    test_begin_end_iterator();
    test_invoke_projected();

    return pika::finalize();
}

int main(int argc, char* argv[])
{
    PIKA_TEST_EQ(pika::init(pika_main, argc, argv), 0);
    return 0;
}
