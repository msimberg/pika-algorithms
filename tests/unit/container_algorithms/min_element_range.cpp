//  Copyright (c) 2014-2016 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/container_algorithms/minmax.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <ctime>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <pika/algorithms/tests/iter_sent.hpp>
#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////
void test_min_element_sent()
{
    using std::get;

    auto c = test::random_iota(100);
    auto ref = std::min_element(std::begin(c), std::begin(c) + 50);
    auto r = pika::ranges::min_element(
        std::begin(c), sentinel<size_t>{*(std::begin(c) + 50)});

    PIKA_TEST(*r == *ref);

    auto c1 = std::vector<size_t>{5, 7, 8};
    ref = std::min_element(
        std::begin(c1), std::begin(c1) + 2, std::greater<std::size_t>());
    r = pika::ranges::min_element(
        std::begin(c1), sentinel<size_t>{8}, std::greater<std::size_t>());

    PIKA_TEST(*r == *ref);

    auto c2 = std::vector<size_t>{2, 2, 2};
    r = pika::ranges::min_element(std::begin(c2), sentinel<size_t>{2});
    PIKA_TEST(r == std::begin(c2));

    auto c3 = std::vector<size_t>{2, 3, 3, 4};
    r = pika::ranges::min_element(std::begin(c3), sentinel<size_t>{3});
    PIKA_TEST(*r == 2);
}

template <typename ExPolicy>
void test_min_element_sent(ExPolicy policy)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using std::get;

    auto c = test::random_iota(100);
    auto ref = std::min_element(std::begin(c), std::begin(c) + 50);
    auto r = pika::ranges::min_element(
        policy, std::begin(c), sentinel<size_t>{*(std::begin(c) + 50)});

    PIKA_TEST(*r == *ref);

    auto c1 = std::vector<size_t>{5, 7, 8};
    ref = std::min_element(
        std::begin(c1), std::begin(c1) + 2, std::greater<std::size_t>());
    r = pika::ranges::min_element(policy, std::begin(c1), sentinel<size_t>{8},
        std::greater<std::size_t>());

    PIKA_TEST(*r == *ref);

    auto c2 = std::vector<size_t>{2, 2, 2};
    r = pika::ranges::min_element(policy, std::begin(c2), sentinel<size_t>{2});
    PIKA_TEST(r == std::begin(c2));

    auto c3 = std::vector<size_t>{4, 3, 3, 4};
    r = pika::ranges::min_element(policy, std::begin(c3), sentinel<size_t>{3});
    PIKA_TEST(*r == 4);
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_min_element(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c = test::random_iota(10007);

    base_iterator ref_end(std::end(c.base()));

    auto r = pika::ranges::min_element(c, std::less<std::size_t>());
    PIKA_TEST(r != std::end(c));

    base_iterator ref = std::min_element(
        std::begin(c.base()), std::end(c.base()), std::less<std::size_t>());
    PIKA_TEST(ref != ref_end);
    PIKA_TEST_EQ(*ref, *r);

    r = pika::ranges::min_element(c);
    PIKA_TEST(r != std::end(c));

    ref = std::min_element(std::begin(c.base()), std::end(c.base()));
    PIKA_TEST(ref != ref_end);
    PIKA_TEST_EQ(*ref, *r);
}

template <typename ExPolicy, typename IteratorTag>
void test_min_element(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c = test::random_iota(10007);

    base_iterator ref_end(std::end(c.base()));

    auto r = pika::ranges::min_element(policy, c, std::less<std::size_t>());
    PIKA_TEST(r != std::end(c));

    base_iterator ref = std::min_element(
        std::begin(c.base()), std::end(c.base()), std::less<std::size_t>());
    PIKA_TEST(ref != ref_end);
    PIKA_TEST_EQ(*ref, *r);

    r = pika::ranges::min_element(policy, c);
    PIKA_TEST(r != std::end(c));

    ref = std::min_element(std::begin(c.base()), std::end(c.base()));
    PIKA_TEST(ref != ref_end);
    PIKA_TEST_EQ(*ref, *r);
}

template <typename ExPolicy, typename IteratorTag>
void test_min_element_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    using test_vector =
        test::test_container<std::vector<std::size_t>, IteratorTag>;

    test_vector c = test::random_iota(10007);

    base_iterator ref_end(std::end(c.base()));

    auto r = pika::ranges::min_element(p, c, std::less<std::size_t>());
    iterator rit = r.get();
    PIKA_TEST(rit != std::end(c));

    base_iterator ref = std::min_element(
        std::begin(c.base()), std::end(c.base()), std::less<std::size_t>());
    PIKA_TEST(ref != ref_end);
    PIKA_TEST_EQ(*ref, *rit);

    r = pika::ranges::min_element(p, c);
    rit = r.get();
    PIKA_TEST(rit != std::end(c));

    ref = std::min_element(std::begin(c.base()), std::end(c.base()));
    PIKA_TEST(ref != ref_end);
    PIKA_TEST_EQ(*ref, *rit);
}

template <typename IteratorTag>
void test_min_element()
{
    using namespace pika::execution;

    test_min_element(IteratorTag());
    test_min_element(seq, IteratorTag());
    test_min_element(par, IteratorTag());
    test_min_element(par_unseq, IteratorTag());

    test_min_element_async(seq(task), IteratorTag());
    test_min_element_async(par(task), IteratorTag());

    test_min_element_sent();
    test_min_element_sent(seq);
    test_min_element_sent(par);
    test_min_element_sent(par_unseq);
}

void min_element_test()
{
    test_min_element<std::random_access_iterator_tag>();
    test_min_element<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_min_element_exception(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c = test::random_iota(10007);

    {
        bool caught_exception = false;
        try
        {
            pika::ranges::min_element(
                pika::util::make_iterator_range(
                    decorated_iterator(std::begin(c),
                        []() { throw std::runtime_error("test"); }),
                    decorated_iterator(std::end(c))),
                std::less<std::size_t>());

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

    {
        bool caught_exception = false;
        try
        {
            pika::ranges::min_element(pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::runtime_error("test"); }),
                decorated_iterator(std::end(c))));

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
}

template <typename ExPolicy, typename IteratorTag>
void test_min_element_exception(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c = test::random_iota(10007);

    {
        bool caught_exception = false;
        try
        {
            pika::ranges::min_element(policy,
                pika::util::make_iterator_range(
                    decorated_iterator(std::begin(c),
                        []() { throw std::runtime_error("test"); }),
                    decorated_iterator(std::end(c))),
                std::less<std::size_t>());

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

    {
        bool caught_exception = false;
        try
        {
            pika::ranges::min_element(policy,
                pika::util::make_iterator_range(
                    decorated_iterator(std::begin(c),
                        []() { throw std::runtime_error("test"); }),
                    decorated_iterator(std::end(c))));

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
}

template <typename ExPolicy, typename IteratorTag>
void test_min_element_exception_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c = test::random_iota(10007);

    {
        bool returned_from_algorithm = false;
        bool caught_exception = false;

        try
        {
            pika::future<decorated_iterator> f = pika::ranges::min_element(p,
                pika::util::make_iterator_range(
                    decorated_iterator(std::begin(c),
                        []() { throw std::runtime_error("test"); }),
                    decorated_iterator(std::end(c))),
                std::less<std::size_t>());

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

    {
        bool caught_exception = false;
        bool returned_from_algorithm = false;

        try
        {
            pika::future<decorated_iterator> f = pika::ranges::min_element(p,
                pika::util::make_iterator_range(
                    decorated_iterator(std::begin(c),
                        []() { throw std::runtime_error("test"); }),
                    decorated_iterator(std::end(c))));

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
}

template <typename IteratorTag>
void test_min_element_exception()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_min_element_exception(IteratorTag());
    test_min_element_exception(seq, IteratorTag());
    test_min_element_exception(par, IteratorTag());

    test_min_element_exception_async(seq(task), IteratorTag());
    test_min_element_exception_async(par(task), IteratorTag());
}

void min_element_exception_test()
{
    test_min_element_exception<std::random_access_iterator_tag>();
    test_min_element_exception<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename IteratorTag>
void test_min_element_bad_alloc(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c = test::random_iota(10007);

    {
        bool caught_exception = false;
        try
        {
            pika::ranges::min_element(pika::util::make_iterator_range(
                                          decorated_iterator(std::begin(c),
                                              []() { throw std::bad_alloc(); }),
                                          decorated_iterator(std::end(c))),
                std::less<std::size_t>());

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

    {
        bool caught_exception = false;
        try
        {
            pika::ranges::min_element(pika::util::make_iterator_range(
                decorated_iterator(
                    std::begin(c), []() { throw std::bad_alloc(); }),
                decorated_iterator(std::end(c))));

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
}

template <typename ExPolicy, typename IteratorTag>
void test_min_element_bad_alloc(ExPolicy policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c = test::random_iota(10007);

    {
        bool caught_exception = false;
        try
        {
            pika::ranges::min_element(policy,
                pika::util::make_iterator_range(
                    decorated_iterator(
                        std::begin(c), []() { throw std::bad_alloc(); }),
                    decorated_iterator(std::end(c))),
                std::less<std::size_t>());

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

    {
        bool caught_exception = false;
        try
        {
            pika::ranges::min_element(policy,
                pika::util::make_iterator_range(
                    decorated_iterator(
                        std::begin(c), []() { throw std::bad_alloc(); }),
                    decorated_iterator(std::end(c))));

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
}

template <typename ExPolicy, typename IteratorTag>
void test_min_element_bad_alloc_async(ExPolicy p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c = test::random_iota(10007);

    {
        bool returned_from_algorithm = false;
        bool caught_exception = false;

        try
        {
            pika::future<decorated_iterator> f = pika::ranges::min_element(p,
                pika::util::make_iterator_range(
                    decorated_iterator(
                        std::begin(c), []() { throw std::bad_alloc(); }),
                    decorated_iterator(std::end(c))),
                std::less<std::size_t>());

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

    {
        bool caught_exception = false;
        bool returned_from_algorithm = false;

        try
        {
            pika::future<decorated_iterator> f = pika::ranges::min_element(p,
                pika::util::make_iterator_range(
                    decorated_iterator(
                        std::begin(c), []() { throw std::bad_alloc(); }),
                    decorated_iterator(std::end(c))));

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
}

template <typename IteratorTag>
void test_min_element_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_min_element_bad_alloc(IteratorTag());
    test_min_element_bad_alloc(seq, IteratorTag());
    test_min_element_bad_alloc(par, IteratorTag());

    test_min_element_bad_alloc_async(seq(task), IteratorTag());
    test_min_element_bad_alloc_async(par(task), IteratorTag());
}

void min_element_bad_alloc_test()
{
    test_min_element_bad_alloc<std::random_access_iterator_tag>();
    test_min_element_bad_alloc<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
int pika_main(pika::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int) std::time(nullptr);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    min_element_test();
    min_element_exception_test();
    min_element_bad_alloc_test();

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
