//  Copyright (c) 2015 Daniel Bourgeois
//  Copyright (c) 2021 Giannis Gonidelis
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithms/tests/iter_sent.hpp>
#include <pika/init.hpp>
#include <pika/iterator_support/iterator_range.hpp>
#include <pika/parallel/container_algorithms/remove_copy.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////////
void test_remove_copy_sent()
{
    using std::get;

    std::size_t const size = 100;
    std::vector<std::int16_t> c(size), d(size, -1);
    std::iota(std::begin(c), std::end(c), 1);
    c[99] = 42;    //both c[99] and c[42] are equal to 42

    int value = 42;

    auto pred = [](const int& n) -> bool { return n > 0; };

    pika::ranges::remove_copy(
        std::begin(c), sentinel<std::int16_t>{50}, std::begin(d), value);
    auto result = std::count_if(std::begin(d), std::end(d), pred);

    PIKA_TEST(result == 48);
}

template <typename ExPolicy>
void test_remove_copy_sent(ExPolicy policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using std::get;

    std::size_t const size = 100;
    std::vector<std::int16_t> c(size), d(size, -1);
    std::iota(std::begin(c), std::end(c), 1);

    c[99] = 42;    //both c[99] and c[42] are equal to 42

    int value = 42;

    auto pred = [](const int& n) -> bool { return n > 0; };

    pika::ranges::remove_copy(policy, std::begin(c), sentinel<std::int16_t>{50},
        std::begin(d), value);
    auto result = std::count_if(std::begin(d), std::end(d), pred);

    PIKA_TEST(result == 48);
}

////////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_remove_copy(IteratorTag)
{
    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    std::vector<std::size_t> d(c.size() / 2);
    std::size_t middle_idx = std::rand() % (c.size() / 2);
    auto middle =
        pika::parallel::detail::next(std::begin(c.base()), middle_idx);
    std::fill(std::begin(c.base()), middle, 1);
    std::fill(middle, std::end(c.base()), 2);

    pika::ranges::remove_copy(c, std::begin(d), std::size_t(2));

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c.base()), middle, std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, middle_idx);
}

template <typename ExPolicy, typename IteratorTag>
void test_remove_copy(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    std::vector<std::size_t> d(c.size() / 2);
    std::size_t middle_idx = std::rand() % (c.size() / 2);
    auto middle =
        pika::parallel::detail::next(std::begin(c.base()), middle_idx);
    std::fill(std::begin(c.base()), middle, 1);
    std::fill(middle, std::end(c.base()), 2);

    pika::ranges::remove_copy(policy, c, std::begin(d), std::size_t(2));

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c.base()), middle, std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, middle_idx);
}

template <typename ExPolicy, typename IteratorTag>
void test_remove_copy_async(ExPolicy p, IteratorTag)
{
    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    std::vector<std::size_t> d(c.size() / 2);
    std::size_t middle_idx = std::rand() % (c.size() / 2);
    auto middle =
        pika::parallel::detail::next(std::begin(c.base()), middle_idx);
    std::fill(std::begin(c.base()), middle, 1);
    std::fill(middle, std::end(c.base()), 2);

    auto f = pika::ranges::remove_copy(p, c, std::begin(d), std::size_t(2));
    f.wait();

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c.base()), middle, std::begin(d),
        [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, middle_idx);
}

template <typename ExPolicy, typename IteratorTag>
void test_remove_copy_outiter(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    std::vector<std::size_t> d(0);
    std::iota(std::begin(c), std::end(c), 0);

    pika::ranges::remove_copy(
        policy, c, std::back_inserter(d), std::size_t(3000));

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c.base()), std::begin(c.base()) + 3000,
        std::begin(d), [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST(std::equal(std::begin(c.base()) + 3001, std::end(c.base()),
        std::begin(d) + 3000, [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_remove_copy_outiter_async(ExPolicy p, IteratorTag)
{
    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);
    std::vector<std::size_t> d(0);
    std::iota(std::begin(c), std::end(c), 0);

    auto f = pika::ranges::remove_copy(
        p, c, std::back_inserter(d), std::size_t(3000));
    f.wait();

    std::size_t count = 0;
    PIKA_TEST(std::equal(std::begin(c.base()), std::begin(c.base()) + 3000,
        std::begin(d), [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST(std::equal(std::begin(c.base()) + 3001, std::end(c.base()),
        std::begin(d) + 3000, [&count](std::size_t v1, std::size_t v2) -> bool {
            PIKA_TEST_EQ(v1, v2);
            ++count;
            return v1 == v2;
        }));
    PIKA_TEST_EQ(count, d.size());
}

template <typename IteratorTag>
void test_remove_copy()
{
    using namespace pika::execution;
    test_remove_copy(IteratorTag());
    test_remove_copy(seq, IteratorTag());
    test_remove_copy(par, IteratorTag());
    test_remove_copy(par_unseq, IteratorTag());

    test_remove_copy_async(seq(task), IteratorTag());
    test_remove_copy_async(par(task), IteratorTag());

    test_remove_copy_sent();
    test_remove_copy_sent(seq);
    test_remove_copy_sent(par);
    test_remove_copy_sent(par_unseq);
}

void remove_copy_test()
{
    test_remove_copy<std::random_access_iterator_tag>();
    test_remove_copy<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_remove_copy_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    try
    {
        pika::ranges::remove_copy(policy,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))),
            std::begin(d), std::size_t(3000));
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
void test_remove_copy_exception_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::ranges::remove_copy(p,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))),
            std::begin(d), std::size_t(3000));
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
void test_remove_copy_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_remove_copy_exception(seq, IteratorTag());
    test_remove_copy_exception(par, IteratorTag());

    test_remove_copy_exception_async(seq(task), IteratorTag());
    test_remove_copy_exception_async(par(task), IteratorTag());
}

void remove_copy_exception_test()
{
    test_remove_copy_exception<std::random_access_iterator_tag>();
    test_remove_copy_exception<std::forward_iterator_tag>();
}

//////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_remove_copy_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    try
    {
        pika::ranges::remove_copy(policy,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::bad_alloc(); }),
                decorated_iterator(std::end(c))),
            std::begin(d), std::size_t(3000));
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
void test_remove_copy_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::vector<std::size_t> d(c.size());
    std::iota(std::begin(c), std::end(c), 0);

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::ranges::remove_copy(p,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::bad_alloc(); }),
                decorated_iterator(std::end(c))),
            std::begin(d), std::size_t(3000));
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
void test_remove_copy_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_remove_copy_bad_alloc(seq, IteratorTag());
    test_remove_copy_bad_alloc(par, IteratorTag());

    test_remove_copy_bad_alloc_async(seq(task), IteratorTag());
    test_remove_copy_bad_alloc_async(par(task), IteratorTag());
}

void remove_copy_bad_alloc_test()
{
    test_remove_copy_bad_alloc<std::random_access_iterator_tag>();
    test_remove_copy_bad_alloc<std::forward_iterator_tag>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    remove_copy_test();
    remove_copy_exception_test();
    remove_copy_bad_alloc_test();
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
