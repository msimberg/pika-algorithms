//  Copyright (c) 2014-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/transform_inclusive_scan.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_transform_inclusive_scan1(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };
    auto conv = [](std::size_t val) { return 2 * val; };

    pika::transform_inclusive_scan(iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(d), op, conv, val);

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_transform_inclusive_scan(
        std::begin(c), std::end(c), std::begin(e), conv, val, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_inclusive_scan1(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };
    auto conv = [](std::size_t val) { return 2 * val; };

    pika::transform_inclusive_scan(policy, iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(d), op, conv, val);

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_transform_inclusive_scan(
        std::begin(c), std::end(c), std::begin(e), conv, val, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));

#if defined(PIKA_ALGORITHMS_HAVE_CXX17_STD_TRANSFORM_SCAN_ALGORITHMS)
    std::vector<std::size_t> f(c.size());
    std::transform_inclusive_scan(
        std::begin(c), std::end(c), std::begin(f), op, conv, val);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(f)));
#endif
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_inclusive_scan1_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    std::size_t const val(0);
    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };
    auto conv = [](std::size_t val) { return 2 * val; };

    pika::future<void> fut =
        pika::transform_inclusive_scan(p, iterator(std::begin(c)),
            iterator(std::end(c)), std::begin(d), op, conv, val);
    fut.wait();

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_transform_inclusive_scan(
        std::begin(c), std::end(c), std::begin(e), conv, val, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));

#if defined(PIKA_ALGORITHMS_HAVE_CXX17_STD_TRANSFORM_SCAN_ALGORITHMS)
    std::vector<std::size_t> f(c.size());
    std::transform_inclusive_scan(
        std::begin(c), std::end(c), std::begin(f), op, conv, val);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(f)));
#endif
}

template <typename IteratorTag>
void test_transform_inclusive_scan1()
{
    using namespace pika::execution;

    test_transform_inclusive_scan1(IteratorTag());
    test_transform_inclusive_scan1(seq, IteratorTag());
    test_transform_inclusive_scan1(par, IteratorTag());
    test_transform_inclusive_scan1(par_unseq, IteratorTag());

    test_transform_inclusive_scan1_async(seq(task), IteratorTag());
    test_transform_inclusive_scan1_async(par(task), IteratorTag());
}

void transform_inclusive_scan_test1()
{
    test_transform_inclusive_scan1<std::random_access_iterator_tag>();
    test_transform_inclusive_scan1<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_transform_inclusive_scan2(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };
    auto conv = [](std::size_t val) { return 2 * val; };

    pika::transform_inclusive_scan(iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(d), op, conv);

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_transform_inclusive_scan_noinit(
        std::begin(c), std::end(c), std::begin(e), conv, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_inclusive_scan2(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };
    auto conv = [](std::size_t val) { return 2 * val; };

    pika::transform_inclusive_scan(policy, iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(d), op, conv);

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_transform_inclusive_scan_noinit(
        std::begin(c), std::end(c), std::begin(e), conv, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));

#if defined(PIKA_ALGORITHMS_HAVE_CXX17_STD_TRANSFORM_SCAN_ALGORITHMS)
    std::vector<std::size_t> f(c.size());
    std::transform_inclusive_scan(
        std::begin(c), std::end(c), std::begin(f), op, conv);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(f)));
#endif
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_inclusive_scan2_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    auto op = [](std::size_t v1, std::size_t v2) { return v1 + v2; };
    auto conv = [](std::size_t val) { return 2 * val; };

    pika::future<void> fut =
        pika::transform_inclusive_scan(p, iterator(std::begin(c)),
            iterator(std::end(c)), std::begin(d), op, conv);
    fut.wait();

    // verify values
    std::vector<std::size_t> e(c.size());
    pika::parallel::detail::sequential_transform_inclusive_scan_noinit(
        std::begin(c), std::end(c), std::begin(e), conv, op);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(e)));

#if defined(PIKA_ALGORITHMS_HAVE_CXX17_STD_TRANSFORM_SCAN_ALGORITHMS)
    std::vector<std::size_t> f(c.size());
    std::transform_inclusive_scan(
        std::begin(c), std::end(c), std::begin(f), op, conv);

    PIKA_TEST(std::equal(std::begin(d), std::end(d), std::begin(f)));
#endif
}

template <typename IteratorTag>
void test_transform_inclusive_scan2()
{
    using namespace pika::execution;

    test_transform_inclusive_scan2(IteratorTag());
    test_transform_inclusive_scan2(seq, IteratorTag());
    test_transform_inclusive_scan2(par, IteratorTag());
    test_transform_inclusive_scan2(par_unseq, IteratorTag());

    test_transform_inclusive_scan2_async(seq(task), IteratorTag());
    test_transform_inclusive_scan2_async(par(task), IteratorTag());
}

void transform_inclusive_scan_test2()
{
    test_transform_inclusive_scan2<std::random_access_iterator_tag>();
    test_transform_inclusive_scan2<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_transform_inclusive_scan_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    bool caught_exception = false;
    try
    {
        pika::transform_inclusive_scan(
            policy, iterator(std::begin(c)), iterator(std::end(c)),
            std::begin(d),
            [](std::size_t v1, std::size_t v2) {
                return throw std::runtime_error("test"), v1 + v2;
            },
            [](std::size_t val) { return val; }, std::size_t(0));

        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(policy, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_inclusive_scan_exception_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<void> f = pika::transform_inclusive_scan(
            p, iterator(std::begin(c)), iterator(std::end(c)), std::begin(d),
            [](std::size_t v1, std::size_t v2) {
                return throw std::runtime_error("test"), v1 + v2;
            },
            [](std::size_t val) { return val; }, std::size_t(0));

        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<ExPolicy, IteratorTag>::call(p, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
    PIKA_TEST(returned_from_algorithm);
}

template <typename IteratorTag>
void test_transform_inclusive_scan_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_transform_inclusive_scan_exception(seq, IteratorTag());
    test_transform_inclusive_scan_exception(par, IteratorTag());

    test_transform_inclusive_scan_exception_async(seq(task), IteratorTag());
    test_transform_inclusive_scan_exception_async(par(task), IteratorTag());
}

void transform_inclusive_scan_exception_test()
{
    test_transform_inclusive_scan_exception<std::random_access_iterator_tag>();
    test_transform_inclusive_scan_exception<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_transform_inclusive_scan_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    bool caught_exception = false;
    try
    {
        pika::transform_inclusive_scan(
            policy, iterator(std::begin(c)), iterator(std::end(c)),
            std::begin(d),
            [](std::size_t v1, std::size_t v2) {
                return throw std::bad_alloc(), v1 + v2;
            },
            [](std::size_t val) { return val; }, std::size_t(0));

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_exception = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_inclusive_scan_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::fill(std::begin(c), std::end(c), std::size_t(1));

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<void> f = pika::transform_inclusive_scan(
            p, iterator(std::begin(c)), iterator(std::end(c)), std::begin(d),
            [](std::size_t v1, std::size_t v2) {
                return throw std::bad_alloc(), v1 + v2;
            },
            [](std::size_t val) { return val; }, std::size_t(0));

        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_exception = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
    PIKA_TEST(returned_from_algorithm);
}

template <typename IteratorTag>
void test_transform_inclusive_scan_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_transform_inclusive_scan_bad_alloc(seq, IteratorTag());
    test_transform_inclusive_scan_bad_alloc(par, IteratorTag());

    test_transform_inclusive_scan_bad_alloc_async(seq(task), IteratorTag());
    test_transform_inclusive_scan_bad_alloc_async(par(task), IteratorTag());
}

void transform_inclusive_scan_bad_alloc_test()
{
    test_transform_inclusive_scan_bad_alloc<std::random_access_iterator_tag>();
    test_transform_inclusive_scan_bad_alloc<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    transform_inclusive_scan_test1();
    transform_inclusive_scan_test2();

    transform_inclusive_scan_exception_test();
    transform_inclusive_scan_bad_alloc_test();

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
