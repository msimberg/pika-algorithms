//  Copyright (c) 2016 John Biddiscombe
//  Copyright (c) 2016 Agustin Berge
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/exclusive_scan.hpp>
#include <pika/parallel/algorithms/inclusive_scan.hpp>
#include <pika/testing.hpp>

#include <iostream>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
void test_scan_non_commutative()
{
    std::vector<std::string> const vs = {"a", "b", "c", "d", "e", "f", "g", "h",
        "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v",
        "w", "x", "y", "z"};

    for (unsigned int i = 0; i < vs.size(); ++i)
    {
        std::vector<std::string> rs(vs.size());
        pika::inclusive_scan(
            pika::execution::par.with(pika::execution::static_chunk_size(i)),
            vs.cbegin(), vs.cend(), rs.begin());
        std::cout << rs.back() << "\n";
        bool is_equal =
            (std::string("abcdefghijklmnopqrstuvwxyz") == rs.back());
        PIKA_TEST(is_equal);
    }
    //
    for (unsigned int i = 0; i < vs.size(); ++i)
    {
        std::vector<std::string> rs(vs.size());
        pika::exclusive_scan(
            pika::execution::par.with(pika::execution::static_chunk_size(i)),
            vs.cbegin(), vs.cend(), rs.begin(), std::string("0"));
        std::cout << rs.back() << "\n";
        bool is_equal =
            (std::string("0abcdefghijklmnopqrstuvwxy") == rs.back());
        PIKA_TEST(is_equal);
    }
}

////////////////////////////////////////////////////////////////////////////////
int pika_main()
{
    test_scan_non_commutative();

    return pika::finalize();
}

int main(int argc, char* argv[])
{
    // add command line option which controls the random number generator seed
    using namespace pika::program_options;
    options_description desc_commandline(
        "Usage: " PIKA_APPLICATION_STRING " [options]");

    // By default this test should run on all available cores
    std::vector<std::string> const cfg = {"pika.os_threads=all"};

    pika::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    PIKA_TEST_EQ_MSG(pika::init(pika_main, argc, argv, init_args), 0,
        "pika main exited with non-zero status");

    return 0;
}
