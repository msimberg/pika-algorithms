//  Copyright (c) 2017 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithm.hpp>
#include <pika/execution.hpp>
#include <pika/init.hpp>
#include <pika/testing.hpp>

#include <algorithm>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
int pika_main()
{
    std::vector<int> c(10007);
    std::iota(std::begin(c), std::end(c), std::random_device{}());

    pika::ranges::for_each(pika::execution::par, c,
        pika::annotated_function(
            [](int) -> void {
                pika::detail::thread_description desc(
                    pika::threads::detail::get_thread_description(
                        pika::threads::detail::get_self_id()));
#if defined(PIKA_HAVE_THREAD_DESCRIPTION)
                PIKA_TEST_EQ(
                    std::string(desc.get_description()), "annotated_function");
#else
                PIKA_TEST_EQ(std::string(desc.get_description()), "<unknown>");
#endif
            },
            "annotated_function"));

    return pika::finalize();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> const cfg = {"pika.os_threads=4"};

    pika::init_params init_args;
    init_args.cfg = cfg;

    PIKA_TEST_EQ_MSG(pika::init(pika_main, argc, argv, init_args), 0,
        "pika main exited with non-zero status");

    return 0;
}
