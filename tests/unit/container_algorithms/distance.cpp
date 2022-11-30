//  Copyright (c) 2020 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithms/tests/iter_sent.hpp>
#include <pika/parallel/algorithms/detail/distance.hpp>
#include <pika/testing.hpp>

#include <cstdint>

int main()
{
    PIKA_TEST_EQ(pika::parallel::detail::distance(
                     iterator<std::int64_t>{0}, sentinel<int64_t>{100}),
        100);

    return 0;
}
