//  Copyright (c) 2014 Grant Mercer
//  Copyright (c) 2020 Francisco Jose Tapia (fjtapia@gmail.com )
//  Copyright (c) 2021 Akhil J Nair
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/nth_element.hpp>
#include <pika/testing.hpp>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
int seed = std::random_device{}();
std::mt19937 gen(seed);

#define SIZE 10007

template <typename IteratorTag>
void test_nth_element(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;

        auto rand_index = std::rand() % SIZE;

        pika::nth_element(iterator(std::begin(c)),
            iterator(std::begin(c) + rand_index), iterator(std::end(c)));

        std::nth_element(
            std::begin(d), std::begin(d) + rand_index, std::end(d));

        PIKA_TEST_EQ(
            *(std::begin(c) + rand_index), *(std::begin(d) + rand_index));

        for (int k = 0; k < rand_index; k++)
        {
            PIKA_TEST_LTE(c[k], c[rand_index]);
        }

        for (int k = rand_index + 1; k < SIZE; k++)
        {
            PIKA_TEST_LTE(c[rand_index], c[k]);
        }
    }

    // Explicitly test nth == std::begin(c): the smallest element should be
    // first
    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;

        pika::nth_element(iterator(std::begin(c)), iterator(std::begin(c)),
            iterator(std::end(c)));

        std::nth_element(std::begin(d), std::begin(d), std::end(d));

        PIKA_TEST_EQ(*std::begin(c), *std::begin(d));

        for (int k = 1; k < SIZE; k++)
        {
            PIKA_TEST_LTE(c[0], c[k]);
        }
    }

    // Explicitly test nth == std::end(c): nothing should be done to the input
    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;
        std::vector<std::size_t> orig = c;

        pika::nth_element(iterator(std::begin(c)), iterator(std::end(c)),
            iterator(std::end(c)));

        std::nth_element(std::begin(d), std::end(d), std::end(d));

        for (int k = 0; k < SIZE; k++)
        {
            PIKA_TEST_EQ(c[k], d[k]);
            PIKA_TEST_EQ(c[k], orig[k]);
        }
    }
}

template <typename ExPolicy, typename IteratorTag>
void test_nth_element(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;

        auto rand_index = std::rand() % SIZE;

        pika::nth_element(policy, iterator(std::begin(c)),
            iterator(std::begin(c) + rand_index), iterator(std::end(c)));

        std::nth_element(
            std::begin(d), std::begin(d) + rand_index, std::end(d));

        PIKA_TEST_EQ(
            *(std::begin(c) + rand_index), *(std::begin(d) + rand_index));

        for (int k = 0; k < rand_index; k++)
        {
            PIKA_TEST_LTE(c[k], c[rand_index]);
        }

        for (int k = rand_index + 1; k < SIZE; k++)
        {
            PIKA_TEST_LTE(c[rand_index], c[k]);
        }
    }

    // Explicitly test nth == std::begin(c): the smallest element should be
    // first
    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;

        pika::nth_element(policy, iterator(std::begin(c)),
            iterator(std::begin(c)), iterator(std::end(c)));

        std::nth_element(std::begin(d), std::begin(d), std::end(d));

        PIKA_TEST_EQ(*std::begin(c), *std::begin(d));

        for (int k = 1; k < SIZE; k++)
        {
            PIKA_TEST_LTE(c[0], c[k]);
        }
    }

    // Explicitly test nth == std::end(c): nothing should be done to the input
    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;
        std::vector<std::size_t> orig = c;

        pika::nth_element(policy, iterator(std::begin(c)),
            iterator(std::end(c)), iterator(std::end(c)));

        std::nth_element(std::begin(d), std::end(d), std::end(d));

        for (int k = 0; k < SIZE; k++)
        {
            PIKA_TEST_EQ(c[k], d[k]);
            PIKA_TEST_EQ(c[k], orig[k]);
        }
    }
}

template <typename ExPolicy, typename IteratorTag>
void test_nth_element_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;

        auto rand_index = std::rand() % SIZE;

        auto actual = pika::nth_element(p, iterator(std::begin(c)),
            iterator(std::begin(c) + rand_index), iterator(std::end(c)));

        std::nth_element(
            std::begin(d), std::begin(d) + rand_index, std::end(d));

        actual.wait();
        PIKA_TEST_EQ(
            *(std::begin(c) + rand_index), *(std::begin(d) + rand_index));

        for (int k = 0; k < rand_index; k++)
        {
            PIKA_TEST_LTE(c[k], c[rand_index]);
        }

        for (int k = rand_index + 1; k < SIZE; k++)
        {
            PIKA_TEST_LTE(c[rand_index], c[k]);
        }
    }

    // Explicitly test nth == std::begin(c): the smallest element should be
    // first
    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;

        auto actual = pika::nth_element(p, iterator(std::begin(c)),
            iterator(std::begin(c)), iterator(std::end(c)));

        std::nth_element(std::begin(d), std::begin(d), std::end(d));

        actual.wait();

        PIKA_TEST_EQ(*std::begin(c), *std::begin(d));

        for (int k = 1; k < SIZE; k++)
        {
            PIKA_TEST_LTE(c[0], c[k]);
        }
    }

    // Explicitly test nth == std::end(c): nothing should be done to the input
    {
        std::vector<std::size_t> c(SIZE);
        std::generate(
            std::begin(c), std::end(c), []() { return std::rand() % SIZE; });
        std::vector<std::size_t> d = c;
        std::vector<std::size_t> orig = c;

        auto actual = pika::nth_element(p, iterator(std::begin(c)),
            iterator(std::end(c)), iterator(std::end(c)));

        std::nth_element(std::begin(d), std::end(d), std::end(d));

        actual.wait();

        for (int k = 0; k < SIZE; k++)
        {
            PIKA_TEST_EQ(c[k], d[k]);
            PIKA_TEST_EQ(c[k], orig[k]);
        }
    }
}

template <typename IteratorTag>
void test_nth_element()
{
    using namespace pika::execution;
    test_nth_element(IteratorTag());
    test_nth_element(seq, IteratorTag());
    test_nth_element(par, IteratorTag());
    test_nth_element(par_unseq, IteratorTag());

    test_nth_element_async(seq(task), IteratorTag());
    test_nth_element_async(par(task), IteratorTag());
}

void nth_element_test()
{
    test_nth_element<std::random_access_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_nth_element_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(SIZE);
    std::generate(
        std::begin(c), std::end(c), []() { return std::rand() % SIZE; });

    auto rand_index = std::rand() % SIZE;

    bool caught_exception = false;
    try
    {
        pika::nth_element(policy,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::begin(c) + rand_index,
                []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
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
void test_nth_element_async_exception(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(SIZE);
    std::generate(
        std::begin(c), std::end(c), []() { return std::rand() % SIZE; });

    auto rand_index = std::rand() % SIZE;

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::nth_element(p,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(std::begin(c) + rand_index,
                []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }));
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
void test_nth_element_exception()
{
    using namespace pika::execution;
    //If the execution policy object is of type vector_execution_policy,
    //  std::terminate shall be called. therefore we do not test exceptions
    //  with a vector execution policy
    test_nth_element_exception(seq, IteratorTag());
    test_nth_element_exception(par, IteratorTag());

    test_nth_element_async_exception(seq(task), IteratorTag());
    test_nth_element_async_exception(par(task), IteratorTag());
}

void nth_element_exception_test()
{
    test_nth_element_exception<std::random_access_iterator_tag>();
}

//////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_nth_element_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(SIZE);
    std::generate(
        std::begin(c), std::end(c), []() { return std::rand() % SIZE; });

    auto rand_index = std::rand() % SIZE;

    bool caught_bad_alloc = false;
    try
    {
        pika::nth_element(policy,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(
                std::begin(c) + rand_index, []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c), []() { throw std::bad_alloc(); }));
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
void test_nth_element_async_bad_alloc(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(SIZE);
    std::generate(
        std::begin(c), std::end(c), []() { return std::rand() % SIZE; });

    auto rand_index = std::rand() % SIZE;

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        auto f = pika::nth_element(p,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(
                std::begin(c) + rand_index, []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c), []() { throw std::bad_alloc(); }));
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
void test_nth_element_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_nth_element_bad_alloc(par, IteratorTag());
    test_nth_element_bad_alloc(seq, IteratorTag());

    test_nth_element_async_bad_alloc(seq(task), IteratorTag());
    test_nth_element_async_bad_alloc(par(task), IteratorTag());
}

void nth_element_bad_alloc_test()
{
    test_nth_element_bad_alloc<std::random_access_iterator_tag>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    nth_element_test();
    nth_element_exception_test();
    nth_element_bad_alloc_test();
    return pika::finalize();
}

int main(int argc, char* argv[])
{
    using namespace pika::program_options;
    options_description desc_commandline(
        "Usage: " PIKA_APPLICATION_STRING " [options]");

    desc_commandline.add_options()("seed,s", value<unsigned int>(),
        "the random number generator seed to use for this run");

    std::vector<std::string> const cfg = {"pika.os_threads=all"};

    pika::init_params init_args;
    init_args.desc_cmdline = desc_commandline;
    init_args.cfg = cfg;

    PIKA_TEST_EQ_MSG(pika::init(pika_main, argc, argv, init_args), 0,
        "pika main exited with non-zero status");

    return 0;
}
