//  Copyright (c) 2014 Grant Mercer
//  Copyright (c) 2015 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/iterator_support/iterator_range.hpp>
#include <pika/parallel/container_algorithms/uninitialized_move.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include <pika/algorithms/tests/iter_sent.hpp>
#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_uninitialized_move_sent(IteratorTag)
{
    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::copy(std::begin(c), std::end(c), std::rbegin(d));
    std::size_t sent_len = (std::rand() % 10007) + 1;
    pika::ranges::uninitialized_move(std::begin(c),
        sentinel<std::size_t>{*(std::begin(c) + sent_len)}, std::begin(d),
        sentinel<std::size_t>{*(std::begin(d) + sent_len)});

    std::size_t count = 0;
    // loop till for sent_len since either the sentinel for the input or output iterator
    // will be reached by then
    PIKA_TEST(std::equal(std::begin(c), std::begin(c) + sent_len, std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));

    PIKA_TEST_EQ(count, sent_len);
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_move_sent(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::copy(std::begin(c), std::end(c), std::rbegin(d));
    std::size_t sent_len = (std::rand() % 10007) + 1;
    pika::ranges::uninitialized_move(policy, std::begin(c),
        sentinel<std::size_t>{*(std::begin(c) + sent_len)}, std::begin(d),
        sentinel<std::size_t>{*(std::begin(d) + sent_len)});

    std::size_t count = 0;
    // loop till for sent_len since either the sentinel for the input or output iterator
    // will be reached by then
    PIKA_TEST(std::equal(std::begin(c), std::begin(c) + sent_len, std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));

    PIKA_TEST_EQ(count, sent_len);
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_move_sent_async(ExPolicy&& p, IteratorTag)
{
    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    std::copy(std::begin(c), std::end(c), std::rbegin(d));
    std::size_t sent_len = (std::rand() % 10007) + 1;
    auto f = pika::ranges::uninitialized_move(p, std::begin(c),
        sentinel<std::size_t>{*(std::begin(c) + sent_len)}, std::begin(d),
        sentinel<std::size_t>{*(std::begin(d) + sent_len)});
    f.wait();

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::begin(c) + sent_len, std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, sent_len);
}

template <typename IteratorTag>
void test_uninitialized_move_sent()
{
    using namespace pika::execution;

    test_uninitialized_move_sent(IteratorTag());

    test_uninitialized_move_sent(seq, IteratorTag());
    test_uninitialized_move_sent(par, IteratorTag());
    test_uninitialized_move_sent(par_unseq, IteratorTag());

    test_uninitialized_move_sent_async(seq(task), IteratorTag());
    test_uninitialized_move_sent_async(par(task), IteratorTag());
}

void uninitialized_move_sent_test()
{
    test_uninitialized_move_sent<std::random_access_iterator_tag>();
    test_uninitialized_move_sent<std::forward_iterator_tag>();
}

////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_uninitialized_move(IteratorTag)
{
    using test_vector =
        typename test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    test_vector d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    pika::ranges::uninitialized_move(c, d);

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::end(c), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_move(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using test_vector =
        typename test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    test_vector d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());
    pika::ranges::uninitialized_move(policy, c, d);

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::end(c), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_move_async(ExPolicy&& p, IteratorTag)
{
    using test_vector =
        typename test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    test_vector d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    auto f = pika::ranges::uninitialized_move(p, c, d);
    f.wait();

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c), std::end(c), std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename IteratorTag>
void test_uninitialized_move()
{
    using namespace pika::execution;

    test_uninitialized_move(IteratorTag());

    test_uninitialized_move(seq, IteratorTag());
    test_uninitialized_move(par, IteratorTag());
    test_uninitialized_move(par_unseq, IteratorTag());

    test_uninitialized_move_async(seq(task), IteratorTag());
    test_uninitialized_move_async(par(task), IteratorTag());
}

void uninitialized_move_test()
{
    test_uninitialized_move<std::random_access_iterator_tag>();
    test_uninitialized_move<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_uninitialized_move_exception(IteratorTag)
{
    using base_iterator = typename std::vector<std::size_t>::iterator;
    using decorated_iterator =
        typename test::decorated_iterator<base_iterator, IteratorTag>;
    using test_vector =
        typename test::test_container<std::vector<std::size_t>, IteratorTag>;

    std::vector<std::size_t> c(10007);
    test_vector d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_exception = false;
    try
    {
        pika::ranges::uninitialized_move(
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))),
            d);
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
void test_uninitialized_move_exception(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = typename std::vector<std::size_t>::iterator;
    using decorated_iterator =
        typename test::decorated_iterator<base_iterator, IteratorTag>;
    using test_vector =
        typename test::test_container<std::vector<std::size_t>, IteratorTag>;

    std::vector<std::size_t> c(10007);
    test_vector d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_exception = false;
    try
    {
        pika::ranges::uninitialized_move(policy,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))),
            d);
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
void test_uninitialized_move_exception_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = typename std::vector<std::size_t>::iterator;
    using decorated_iterator =
        typename test::decorated_iterator<base_iterator, IteratorTag>;
    using test_vector =
        typename test::test_container<std::vector<std::size_t>, IteratorTag>;

    std::vector<std::size_t> c(10007);
    test_vector d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::ranges::uninitialized_move(p,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))),
            d);
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
void test_uninitialized_move_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_uninitialized_move_exception(seq, IteratorTag());
    test_uninitialized_move_exception(par, IteratorTag());

    test_uninitialized_move_exception_async(seq(task), IteratorTag());
    test_uninitialized_move_exception_async(par(task), IteratorTag());
}

void uninitialized_move_exception_test()
{
    test_uninitialized_move_exception<std::random_access_iterator_tag>();
    test_uninitialized_move_exception<std::forward_iterator_tag>();
}

//////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_uninitialized_move_bad_alloc(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = typename std::vector<std::size_t>::iterator;
    using decorated_iterator =
        typename test::decorated_iterator<base_iterator, IteratorTag>;
    using test_vector =
        typename test::test_container<std::vector<std::size_t>, IteratorTag>;

    std::vector<std::size_t> c(10007);
    test_vector d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_bad_alloc = false;
    try
    {
        pika::ranges::uninitialized_move(policy,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::bad_alloc(); }),
                decorated_iterator(std::end(c))),
            d);
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
void test_uninitialized_move_bad_alloc_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = typename std::vector<std::size_t>::iterator;
    using decorated_iterator =
        typename test::decorated_iterator<base_iterator, IteratorTag>;
    using test_vector =
        typename test::test_container<std::vector<std::size_t>, IteratorTag>;

    std::vector<std::size_t> c(10007);
    test_vector d(c.size());
    std::iota(std::begin(c), std::end(c), std::rand());

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::ranges::uninitialized_move(p,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::bad_alloc(); }),
                decorated_iterator(std::end(c))),
            d);
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
void test_uninitialized_move_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_uninitialized_move_bad_alloc(seq, IteratorTag());
    test_uninitialized_move_bad_alloc(par, IteratorTag());

    test_uninitialized_move_bad_alloc_async(seq(task), IteratorTag());
    test_uninitialized_move_bad_alloc_async(par(task), IteratorTag());
}

void uninitialized_move_bad_alloc_test()
{
    test_uninitialized_move_bad_alloc<std::random_access_iterator_tag>();
    test_uninitialized_move_bad_alloc<std::forward_iterator_tag>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    uninitialized_move_test();
    uninitialized_move_sent_test();
    uninitialized_move_exception_test();
    uninitialized_move_bad_alloc_test();
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
