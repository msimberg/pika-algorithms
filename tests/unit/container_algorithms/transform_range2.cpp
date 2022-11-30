//  Copyright (c) 2020 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/iterator_support/iterator_range.hpp>
#include <pika/parallel/container_algorithms/transform.hpp>
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
void test_transform(IteratorTag)
{
    using test_vector = test::test_container<std::vector<int>, IteratorTag>;

    test_vector c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    auto result = pika::ranges::transform(std::begin(c), std::end(c),
        std::begin(d), [](std::size_t v) { return v + 1; });

    PIKA_TEST(result.in == std::end(c));
    PIKA_TEST(result.out == std::end(d));

    // verify values
    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::end(c), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1 + 1, v2);
            ++count;
            return v1 + 1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_transform(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using test_vector = test::test_container<std::vector<int>, IteratorTag>;

    test_vector c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    auto result = pika::ranges::transform(policy, std::begin(c), std::end(c),
        std::begin(d), [](std::size_t v) { return v + 1; });

    PIKA_TEST(result.in == std::end(c));
    PIKA_TEST(result.out == std::end(d));

    // verify values
    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::end(c), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1 + 1, v2);
            ++count;
            return v1 + 1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_async(ExPolicy p, IteratorTag)
{
    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    auto f = pika::ranges::transform(p, std::begin(c), std::end(c),
        std::begin(d), [](std::size_t& v) { return v + 1; });
    f.wait();

    auto result = f.get();
    PIKA_TEST(result.in == std::end(c));
    PIKA_TEST(result.out == std::end(d));

    // verify values
    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::end(c), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1 + 1, v2);
            ++count;
            return v1 + 1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename IteratorTag>
void test_transform()
{
    using namespace pika::execution;

    test_transform(IteratorTag());
    test_transform(seq, IteratorTag());
    test_transform(par, IteratorTag());
    test_transform(par_unseq, IteratorTag());

    test_transform_async(seq(task), IteratorTag());
    test_transform_async(par(task), IteratorTag());
}

void transform_test()
{
    test_transform<std::random_access_iterator_tag>();
    test_transform<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_transform_exception(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_exception = false;
    try
    {
        pika::ranges::transform(
            pika::util::make_iterator_range(
                iterator(std::begin(c)), iterator(std::end(c))),
            std::begin(d),
            [](std::size_t v) { return throw std::runtime_error("test"), v; });

        PIKA_TEST(false);
    }
    catch (pika::exception_list const& e)
    {
        caught_exception = true;
        test::test_num_exceptions<pika::execution::sequenced_policy,
            IteratorTag>::call(pika::execution::seq, e);
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_exception = false;
    try
    {
        pika::ranges::transform(policy,
            pika::util::make_iterator_range(
                iterator(std::begin(c)), iterator(std::end(c))),
            std::begin(d),
            [](std::size_t v) { return throw std::runtime_error("test"), v; });

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
void test_transform_exception_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::ranges::transform(p,
            pika::util::make_iterator_range(
                iterator(std::begin(c)), iterator(std::end(c))),
            std::begin(d),
            [](std::size_t v) { return throw std::runtime_error("test"), v; });
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
void test_transform_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_transform_exception(IteratorTag());
    test_transform_exception(seq, IteratorTag());
    test_transform_exception(par, IteratorTag());

    test_transform_exception_async(seq(task), IteratorTag());
    test_transform_exception_async(par(task), IteratorTag());
}

void transform_exception_test()
{
    test_transform_exception<std::random_access_iterator_tag>();
    test_transform_exception<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_transform_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_bad_alloc = false;
    try
    {
        pika::ranges::transform(policy,
            pika::util::make_iterator_range(
                iterator(std::begin(c)), iterator(std::end(c))),
            std::begin(d),
            [](std::size_t v) { return throw std::bad_alloc(), v; });

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_bad_alloc);
}

template <typename ExPolicy, typename IteratorTag>
void test_transform_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::ranges::transform(p,
            pika::util::make_iterator_range(
                iterator(std::begin(c)), iterator(std::end(c))),
            std::begin(d),
            [](std::size_t v) { return throw std::bad_alloc(), v; });
        returned_from_algorithm = true;
        f.get();

        PIKA_TEST(false);
    }
    catch (std::bad_alloc const&)
    {
        caught_bad_alloc = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_bad_alloc);
    PIKA_TEST(returned_from_algorithm);
}

template <typename IteratorTag>
void test_transform_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_transform_bad_alloc(seq, IteratorTag());
    test_transform_bad_alloc(par, IteratorTag());

    test_transform_bad_alloc_async(seq(task), IteratorTag());
    test_transform_bad_alloc_async(par(task), IteratorTag());
}

void transform_bad_alloc_test()
{
    test_transform_bad_alloc<std::random_access_iterator_tag>();
    test_transform_bad_alloc<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    transform_test();
    transform_exception_test();
    transform_bad_alloc_test();
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
