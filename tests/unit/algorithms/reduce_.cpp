//  Copyright (c) 2014-2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/reduce.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
int seed = std::random_device{}();
std::mt19937 gen(seed);

template <typename IteratorTag>
void test_reduce1(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t val(42);
    auto op = [val](std::size_t v1, std::size_t v2) { return v1 + v2 + val; };

    std::size_t r1 =
        pika::reduce(iterator(std::begin(c)), iterator(std::end(c)), val, op);

    // verify values
    std::size_t r2 = std::accumulate(std::begin(c), std::end(c), val, op);
    PIKA_TEST_EQ(r1, r2);
}

template <typename ExPolicy, typename IteratorTag>
void test_reduce1(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t val(42);
    auto op = [val](std::size_t v1, std::size_t v2) { return v1 + v2 + val; };

    std::size_t r1 = pika::reduce(
        policy, iterator(std::begin(c)), iterator(std::end(c)), val, op);

    // verify values
    std::size_t r2 = std::accumulate(std::begin(c), std::end(c), val, op);
    PIKA_TEST_EQ(r1, r2);
}

template <typename ExPolicy, typename IteratorTag>
void test_reduce1_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t val(42);
    auto op = [val](std::size_t v1, std::size_t v2) { return v1 + v2 + val; };

    pika::future<std::size_t> f = pika::reduce(
        p, iterator(std::begin(c)), iterator(std::end(c)), val, op);
    f.wait();

    // verify values
    std::size_t r2 = std::accumulate(std::begin(c), std::end(c), val, op);
    PIKA_TEST_EQ(f.get(), r2);
}

template <typename IteratorTag>
void test_reduce1()
{
    using namespace pika::execution;

    test_reduce1(IteratorTag());
    test_reduce1(seq, IteratorTag());
    test_reduce1(par, IteratorTag());
    test_reduce1(par_unseq, IteratorTag());

    test_reduce1_async(seq(task), IteratorTag());
    test_reduce1_async(par(task), IteratorTag());
}

void reduce_test1()
{
    test_reduce1<std::random_access_iterator_tag>();
    test_reduce1<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_reduce2(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t const val(42);
    std::size_t r1 =
        pika::reduce(iterator(std::begin(c)), iterator(std::end(c)), val);

    // verify values
    std::size_t r2 = std::accumulate(std::begin(c), std::end(c), val);
    PIKA_TEST_EQ(r1, r2);
}

template <typename ExPolicy, typename IteratorTag>
void test_reduce2(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t const val(42);
    std::size_t r1 = pika::reduce(
        policy, iterator(std::begin(c)), iterator(std::end(c)), val);

    // verify values
    std::size_t r2 = std::accumulate(std::begin(c), std::end(c), val);
    PIKA_TEST_EQ(r1, r2);
}

template <typename ExPolicy, typename IteratorTag>
void test_reduce2_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t const val(42);
    pika::future<std::size_t> f =
        pika::reduce(p, iterator(std::begin(c)), iterator(std::end(c)), val);
    f.wait();

    // verify values
    std::size_t r2 = std::accumulate(std::begin(c), std::end(c), val);
    PIKA_TEST_EQ(f.get(), r2);
}

template <typename IteratorTag>
void test_reduce2()
{
    using namespace pika::execution;

    test_reduce2(IteratorTag());
    test_reduce2(seq, IteratorTag());
    test_reduce2(par, IteratorTag());
    test_reduce2(par_unseq, IteratorTag());

    test_reduce2_async(seq(task), IteratorTag());
    test_reduce2_async(par(task), IteratorTag());
}

void reduce_test2()
{
    test_reduce2<std::random_access_iterator_tag>();
    test_reduce2<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_reduce3(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t r1 =
        pika::reduce(iterator(std::begin(c)), iterator(std::end(c)));

    // verify values
    std::size_t r2 =
        std::accumulate(std::begin(c), std::end(c), std::size_t(0));
    PIKA_TEST_EQ(r1, r2);
}

template <typename ExPolicy, typename IteratorTag>
void test_reduce3(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    std::size_t r1 =
        pika::reduce(policy, iterator(std::begin(c)), iterator(std::end(c)));

    // verify values
    std::size_t r2 =
        std::accumulate(std::begin(c), std::end(c), std::size_t(0));
    PIKA_TEST_EQ(r1, r2);
}

template <typename ExPolicy, typename IteratorTag>
void test_reduce3_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    pika::future<std::size_t> f =
        pika::reduce(p, iterator(std::begin(c)), iterator(std::end(c)));
    f.wait();

    // verify values
    std::size_t r2 =
        std::accumulate(std::begin(c), std::end(c), std::size_t(0));
    PIKA_TEST_EQ(f.get(), r2);
}

template <typename IteratorTag>
void test_reduce3()
{
    using namespace pika::execution;

    test_reduce3(IteratorTag());
    test_reduce3(seq, IteratorTag());
    test_reduce3(par, IteratorTag());
    test_reduce3(par_unseq, IteratorTag());

    test_reduce3_async(seq(task), IteratorTag());
    test_reduce3_async(par(task), IteratorTag());
}

void reduce_test3()
{
    test_reduce3<std::random_access_iterator_tag>();
    test_reduce3<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_reduce_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    try
    {
        pika::reduce(policy, iterator(std::begin(c)), iterator(std::end(c)),
            std::size_t(42), [](std::size_t v1, std::size_t v2) {
                return throw std::runtime_error("test"), v1 + v2;
            });

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
void test_reduce_exception_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<void> f =
            pika::reduce(p, iterator(std::begin(c)), iterator(std::end(c)),
                std::size_t(42), [](std::size_t v1, std::size_t v2) {
                    return throw std::runtime_error("test"), v1 + v2;
                });
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
void test_reduce_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_reduce_exception(seq, IteratorTag());
    test_reduce_exception(par, IteratorTag());

    test_reduce_exception_async(seq(task), IteratorTag());
    test_reduce_exception_async(par(task), IteratorTag());
}

void reduce_exception_test()
{
    test_reduce_exception<std::random_access_iterator_tag>();
    test_reduce_exception<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_reduce_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    try
    {
        pika::reduce(policy, iterator(std::begin(c)), iterator(std::end(c)),
            std::size_t(42), [](std::size_t v1, std::size_t v2) {
                return throw std::bad_alloc(), v1 + v2;
            });

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
void test_reduce_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<void> f =
            pika::reduce(p, iterator(std::begin(c)), iterator(std::end(c)),
                std::size_t(42), [](std::size_t v1, std::size_t v2) {
                    return throw std::bad_alloc(), v1 + v2;
                });
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
void test_reduce_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_reduce_bad_alloc(seq, IteratorTag());
    test_reduce_bad_alloc(par, IteratorTag());

    test_reduce_bad_alloc_async(seq(task), IteratorTag());
    test_reduce_bad_alloc_async(par(task), IteratorTag());
}

void reduce_bad_alloc_test()
{
    test_reduce_bad_alloc<std::random_access_iterator_tag>();
    test_reduce_bad_alloc<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    reduce_test1();
    reduce_test2();
    reduce_test3();

    reduce_exception_test();
    reduce_bad_alloc_test();
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
