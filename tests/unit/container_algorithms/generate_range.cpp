//  copyright (c) 2020 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/algorithms/tests/iter_sent.hpp>
#include <pika/init.hpp>
#include <pika/iterator_support/iterator_range.hpp>
#include <pika/parallel/container_algorithms/generate.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "test_utils.hpp"

// FIXME: Intel 15 currently can not compile this code. This needs to be fixed. See #1408
#if !(defined(PIKA_INTEL_VERSION) && PIKA_INTEL_VERSION == 1500)

////////////////////////////////////////////////////////////////////////////
void test_generate_sent()
{
    std::vector<std::size_t> c(200);
    std::iota(std::begin(c), std::end(c), std::rand());

    auto gen = []() { return std::size_t(10); };

    pika::ranges::generate(
        std::begin(c), sentinel<std::size_t>{*(std::begin(c) + 100)}, gen);

    // verify values
    std::size_t count = 0;
    std::for_each(
        std::begin(c), std::begin(c) + 100, [&count](std::size_t v) -> void {
            PIKA_TEST_EQ(v, std::size_t(10));
            ++count;
        });
    PIKA_TEST_EQ(count, (size_t) 100);
}

template <typename ExPolicy>
void test_generate_sent(ExPolicy policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    std::vector<std::size_t> c(200);
    std::iota(std::begin(c), std::end(c), std::rand());

    auto gen = []() { return std::size_t(10); };

    pika::ranges::generate(policy, std::begin(c),
        sentinel<std::size_t>{*(std::begin(c) + 100)}, gen);

    // verify values
    std::size_t count = 0;
    std::for_each(
        std::begin(c), std::begin(c) + 100, [&count](std::size_t v) -> void {
            PIKA_TEST_EQ(v, std::size_t(10));
            ++count;
        });
    PIKA_TEST_EQ(count, (size_t) 100);
}

template <typename IteratorTag>
void test_generate(IteratorTag)
{
    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);

    auto gen = []() { return std::size_t(10); };

    pika::ranges::generate(c, gen);

    // verify values
    std::size_t count = 0;
    std::for_each(std::begin(c.base()), std::end(c.base()),
        [&count](std::size_t v) -> void {
            PIKA_TEST_EQ(v, std::size_t(10));
            ++count;
        });
    PIKA_TEST_EQ(count, c.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_generate(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);

    auto gen = []() { return std::size_t(10); };

    pika::ranges::generate(policy, c, gen);

    // verify values
    std::size_t count = 0;
    std::for_each(std::begin(c.base()), std::end(c.base()),
        [&count](std::size_t v) -> void {
            PIKA_TEST_EQ(v, std::size_t(10));
            ++count;
        });
    PIKA_TEST_EQ(count, c.size());
}

template <typename ExPolicy, typename IteratorTag>
void test_generate_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c(10007);

    auto gen = []() { return std::size_t(10); };

    pika::future<iterator> f = pika::ranges::generate(p, c, gen);
    f.wait();

    std::size_t count = 0;
    std::for_each(std::begin(c.base()), std::end(c.base()),
        [&count](std::size_t v) -> void {
            PIKA_TEST_EQ(v, std::size_t(10));
            ++count;
        });
    PIKA_TEST_EQ(count, c.size());
}

template <typename IteratorTag>
void test_generate()
{
    using namespace pika::execution;

    test_generate(IteratorTag());

    test_generate(seq, IteratorTag());
    test_generate(par, IteratorTag());
    test_generate(par_unseq, IteratorTag());

    test_generate_async(seq(task), IteratorTag());
    test_generate_async(par(task), IteratorTag());

    test_generate_sent();
    test_generate_sent(seq);
    test_generate_sent(par);
    test_generate_sent(par_unseq);
}

void generate_test()
{
    test_generate<std::random_access_iterator_tag>();
    test_generate<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_generate_exception(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<std::size_t> c(10007);

    auto gen = []() { return std::size_t(10); };

    bool caught_exception = false;
    try
    {
        pika::ranges::generate(
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))),
            gen);
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
void test_generate_exception(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<std::size_t> c(10007);

    auto gen = []() { return std::size_t(10); };

    bool caught_exception = false;
    try
    {
        pika::ranges::generate(policy,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))),
            gen);
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
void test_generate_exception_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);

    auto gen = []() { return std::size_t(10); };

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<void> f = pika::ranges::generate(p,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))),
            gen);
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
void test_generate_exception()
{
    using namespace pika::execution;

    test_generate_exception(IteratorTag());

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_generate_exception(seq, IteratorTag());
    test_generate_exception(par, IteratorTag());

    test_generate_exception_async(seq(task), IteratorTag());
    test_generate_exception_async(par(task), IteratorTag());
}

void generate_exception_test()
{
    test_generate_exception<std::random_access_iterator_tag>();
    test_generate_exception<std::forward_iterator_tag>();
}

//////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_generate_bad_alloc(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(100007);

    auto gen = []() { return 10; };

    bool caught_bad_alloc = false;
    try
    {
        pika::ranges::generate(policy,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::bad_alloc(); }),
                decorated_iterator(std::end(c))),
            gen);
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
void test_generate_bad_alloc_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);

    auto gen = []() { return std::size_t(10); };

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<void> f = pika::ranges::generate(p,
            pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::bad_alloc(); }),
                decorated_iterator(std::end(c))),
            gen);
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
void test_generate_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_generate_bad_alloc(seq, IteratorTag());
    test_generate_bad_alloc(par, IteratorTag());

    test_generate_bad_alloc_async(seq(task), IteratorTag());
    test_generate_bad_alloc_async(par(task), IteratorTag());
}

void generate_bad_alloc_test()
{
    test_generate_bad_alloc<std::random_access_iterator_tag>();
    test_generate_bad_alloc<std::forward_iterator_tag>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    generate_test();
    generate_exception_test();
    generate_bad_alloc_test();
    return pika::finalize();
}
#else
int pika_main(pika::program_options::variables_map& vm)
{
    return pika::finalize();
}
#endif

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
