//  Copyright (c) 2014 Grant Mercer
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/container_algorithms/find.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "test_utils.hpp"

////////////////////////////////////////////////////////////////////////////
unsigned int seed = std::random_device{}();
std::mt19937 gen(seed);
std::uniform_int_distribution<> dis(3, 102);
std::uniform_int_distribution<> dist(7, 106);

template <typename IteratorTag>
void test_find_end1_proj(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    // fill vector with random values above 2
    std::fill(std::begin(c), std::end(c), dis(gen));

    // create subsequence in middle of vector
    c[c.size() / 2] = 1 + 65536;
    c[c.size() / 2 + 1] = 2 + 65536;

    std::size_t h[] = {1, 2};

    auto proj = [](std::size_t x) { return x % 65536; };

    iterator index = pika::ranges::find_end(iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(h), std::end(h),
        std::equal_to<std::size_t>(), proj, proj);

    base_iterator test_index = std::begin(c) + c.size() / 2;

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_find_end1_proj(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    // fill vector with random values above 2
    std::fill(std::begin(c), std::end(c), dis(gen));
    // create subsequence in middle of vector
    c[c.size() / 2] = 1 + 65536;
    c[c.size() / 2 + 1] = 2 + 65536;

    std::size_t h[] = {1, 2};

    auto proj = [](std::size_t x) { return x % 65536; };

    iterator index = pika::ranges::find_end(policy, iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(h), std::end(h),
        std::equal_to<std::size_t>(), proj, proj);

    base_iterator test_index = std::begin(c) + c.size() / 2;

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_find_end1_async_proj(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    // fill vector with random values above 2
    std::fill(std::begin(c), std::end(c), dis(gen));
    // create subsequence in middle of vector
    c[c.size() / 2] = 1 + 65536;
    c[c.size() / 2 + 1] = 2 + 65536;

    std::size_t h[] = {1, 2};

    auto proj = [](std::size_t x) { return x % 65536; };

    pika::future<iterator> f = pika::ranges::find_end(p,
        iterator(std::begin(c)), iterator(std::end(c)), std::begin(h),
        std::end(h), std::equal_to<std::size_t>(), proj, proj);
    f.wait();

    // create iterator at position of value to be found
    base_iterator test_index = std::begin(c) + c.size() / 2;

    PIKA_TEST(f.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_find_end1()
{
    using namespace pika::execution;

    test_find_end1_proj(IteratorTag());

    test_find_end1_proj(seq, IteratorTag());
    test_find_end1_proj(par, IteratorTag());
    test_find_end1_proj(par_unseq, IteratorTag());

    test_find_end1_async_proj(seq(task), IteratorTag());
    test_find_end1_async_proj(par(task), IteratorTag());
}

void find_end_test1()
{
    test_find_end1<std::random_access_iterator_tag>();
    test_find_end1<std::forward_iterator_tag>();
}

template <typename IteratorTag>
void test_find_end2_proj(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    // fill vector with random values about 2
    std::fill(std::begin(c), std::end(c), dis(gen));

    // create subsequence at start and end
    c[0] = 1;
    c[1] = 2;
    c[c.size() - 1] = 2;
    c[c.size() - 2] = 1;

    std::size_t h[] = {1 + 65536, 2 + 65536};

    auto proj = [](std::size_t x) { return x % 65536; };

    iterator index = pika::ranges::find_end(iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(h), std::end(h),
        std::equal_to<std::size_t>(), proj, proj);

    base_iterator test_index = std::begin(c) + c.size() - 2;

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_find_end2_proj(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    // fill vector with random values about 2
    std::fill(std::begin(c), std::end(c), dis(gen));

    // create subsequence at start and end
    c[0] = 1;
    c[1] = 2;
    c[c.size() - 1] = 2;
    c[c.size() - 2] = 1;

    std::size_t h[] = {1 + 65536, 2 + 65536};

    auto proj = [](std::size_t x) { return x % 65536; };

    iterator index = pika::ranges::find_end(policy, iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(h), std::end(h),
        std::equal_to<std::size_t>(), proj, proj);

    base_iterator test_index = std::begin(c) + c.size() - 2;

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_find_end2_async_proj(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    // fill vector with random values above 2
    std::vector<std::size_t> c(10007);
    std::fill(std::begin(c), std::end(c), dis(gen));

    // create subsequence at start and end
    c[0] = 1;
    c[1] = 2;
    c[c.size() - 1] = 2;
    c[c.size() - 2] = 1;

    std::size_t h[] = {1 + 65536, 2 + 65536};

    auto proj = [](std::size_t x) { return x % 65536; };

    pika::future<iterator> f = pika::ranges::find_end(p,
        iterator(std::begin(c)), iterator(std::end(c)), std::begin(h),
        std::end(h), std::equal_to<std::size_t>(), proj, proj);

    f.wait();

    // create iterator at position of value to be found
    base_iterator test_index = std::begin(c) + c.size() - 2;

    PIKA_TEST(f.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_find_end2()
{
    using namespace pika::execution;

    test_find_end2_proj(IteratorTag());

    test_find_end2_proj(seq, IteratorTag());
    test_find_end2_proj(par, IteratorTag());
    test_find_end2_proj(par_unseq, IteratorTag());

    test_find_end2_async_proj(seq(task), IteratorTag());
    test_find_end2_async_proj(par(task), IteratorTag());
}

void find_end_test2()
{
    test_find_end2<std::random_access_iterator_tag>();
    test_find_end2<std::forward_iterator_tag>();
}

template <typename IteratorTag>
void test_find_end3_proj(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    // fill vector with random values above 2
    std::vector<std::size_t> c(10007);
    std::fill(std::begin(c), std::end(c), dis(gen));

    // create subsequence large enough to always be split into multiple partitions
    std::iota(std::begin(c), std::begin(c) + c.size() / 16 + 1, 1);
    std::size_t sub_size = c.size() / 16 + 1;

    std::vector<std::size_t> h(sub_size);
    std::iota(std::begin(h), std::end(h), 1 + 65536);

    auto proj = [](std::size_t x) { return x % 65536; };

    iterator index = pika::ranges::find_end(iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(h), std::end(h),
        std::equal_to<std::size_t>(), proj, proj);

    base_iterator test_index = std::begin(c);

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_find_end3_proj(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    // fill vector with random values above 2
    std::vector<std::size_t> c(10007);
    std::fill(std::begin(c), std::end(c), dis(gen));

    // create subsequence large enough to always be split into multiple partitions
    std::iota(std::begin(c), std::begin(c) + c.size() / 16 + 1, 1);
    std::size_t sub_size = c.size() / 16 + 1;

    std::vector<std::size_t> h(sub_size);
    std::iota(std::begin(h), std::end(h), 1 + 65536);

    auto proj = [](std::size_t x) { return x % 65536; };

    iterator index = pika::ranges::find_end(policy, iterator(std::begin(c)),
        iterator(std::end(c)), std::begin(h), std::end(h),
        std::equal_to<std::size_t>(), proj, proj);

    base_iterator test_index = std::begin(c);

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_find_end3_async_proj(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    // fill vector with random values above 6
    std::vector<std::size_t> c(10007);
    std::fill(std::begin(c), std::end(c), dist(gen));

    // create subsequence large enough to always be split into multiple partitions
    std::iota(std::begin(c), std::begin(c) + c.size() / 16 + 1, 1);
    std::size_t sub_size = c.size() / 16 + 1;

    std::vector<std::size_t> h(sub_size);
    std::iota(std::begin(h), std::end(h), 1 + 65536);

    auto proj = [](std::size_t x) { return x % 65536; };

    // create only two partitions, splitting the desired sub sequence into
    // separate partitions.
    pika::future<iterator> f = pika::ranges::find_end(p,
        iterator(std::begin(c)), iterator(std::end(c)), std::begin(h),
        std::end(h), std::equal_to<std::size_t>(), proj, proj);

    f.wait();

    //create iterator at position of value to be found
    base_iterator test_index = std::begin(c);

    PIKA_TEST(f.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_find_end3()
{
    using namespace pika::execution;

    test_find_end3_proj(IteratorTag());

    test_find_end3_proj(seq, IteratorTag());
    test_find_end3_proj(par, IteratorTag());
    test_find_end3_proj(par_unseq, IteratorTag());

    test_find_end3_async_proj(seq(task), IteratorTag());
    test_find_end3_async_proj(par(task), IteratorTag());
}

void find_end_test3()
{
    test_find_end3<std::random_access_iterator_tag>();
    test_find_end3<std::forward_iterator_tag>();
}

template <typename IteratorTag>
void test_find_end4_proj(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    // fill vector with random values above 2
    std::fill(std::begin(c), std::end(c), dis(gen));

    // create subsequence in middle of vector
    c[c.size() / 2] = 1;
    c[c.size() / 2 + 1] = 2;

    std::size_t h[] = {1 + 65536, 2 + 65536};

    auto proj = [](std::size_t x) { return x % 65536; };

    iterator index = pika::ranges::find_end(
        iterator(std::begin(c)), iterator(std::end(c)), std::begin(h),
        std::end(h), [](std::size_t v1, std::size_t v2) { return !(v1 != v2); },
        proj, proj);

    base_iterator test_index = std::begin(c) + c.size() / 2;

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_find_end4_proj(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    // fill vector with random values above 2
    std::fill(std::begin(c), std::end(c), dis(gen));
    // create subsequence in middle of vector
    c[c.size() / 2] = 1;
    c[c.size() / 2 + 1] = 2;

    std::size_t h[] = {1 + 65536, 2 + 65536};

    auto proj = [](std::size_t x) { return x % 65536; };

    iterator index = pika::ranges::find_end(
        policy, iterator(std::begin(c)), iterator(std::end(c)), std::begin(h),
        std::end(h), [](std::size_t v1, std::size_t v2) { return !(v1 != v2); },
        proj, proj);

    base_iterator test_index = std::begin(c) + c.size() / 2;

    PIKA_TEST(index == iterator(test_index));
}

template <typename ExPolicy, typename IteratorTag>
void test_find_end4_async_proj(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using iterator = test::test_iterator<base_iterator, IteratorTag>;

    // fill vector with random values above 2
    std::vector<std::size_t> c(10007);
    std::fill(std::begin(c), std::end(c), dis(gen));

    // create subsequence in middle of vector
    c[c.size() / 2] = 1;
    c[c.size() / 2 + 1] = 2;

    std::size_t h[] = {1 + 65536, 2 + 65536};

    auto proj = [](std::size_t x) { return x % 65536; };

    pika::future<iterator> f = pika::ranges::find_end(
        p, iterator(std::begin(c)), iterator(std::end(c)), std::begin(h),
        std::end(h), [](std::size_t v1, std::size_t v2) { return !(v1 != v2); },
        proj, proj);

    f.wait();

    //create iterator at position of value to be found
    base_iterator test_index = std::begin(c) + c.size() / 2;

    PIKA_TEST(f.get() == iterator(test_index));
}

template <typename IteratorTag>
void test_find_end4()
{
    using namespace pika::execution;

    test_find_end4_proj(IteratorTag());

    test_find_end4_proj(seq, IteratorTag());
    test_find_end4_proj(par, IteratorTag());
    test_find_end4_proj(par_unseq, IteratorTag());

    test_find_end4_async_proj(seq(task), IteratorTag());
    test_find_end4_async_proj(par(task), IteratorTag());
}

void find_end_test4()
{
    test_find_end4<std::random_access_iterator_tag>();
    test_find_end4<std::forward_iterator_tag>();
}

///////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_find_end_exception(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;
    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);
    c[c.size() / 2] = 1;
    c[c.size() / 2 + 1] = 2;

    std::vector<std::size_t> h;
    h.push_back(1);
    h.push_back(2);

    bool caught_exception = false;
    try
    {
        pika::ranges::find_end(policy,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }),
            std::begin(h), std::end(h));
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

template <typename IteratorTag>
void test_find_end_exception(IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);
    c[c.size() / 2] = 1;
    c[c.size() / 2 + 1] = 2;

    std::vector<std::size_t> h;
    h.push_back(1);
    h.push_back(2);

    bool caught_exception = false;
    try
    {
        pika::ranges::find_end(decorated_iterator(std::begin(c),
                                   []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }),
            std::begin(h), std::end(h));
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
void test_find_end_exception_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);
    c[c.size() / 2] = 1;
    c[c.size() / 2 + 1] = 2;

    std::size_t h[] = {1, 2};

    bool caught_exception = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<decorated_iterator> f = pika::ranges::find_end(p,
            decorated_iterator(
                std::begin(c), []() { throw std::runtime_error("test"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("test"); }),
            std::begin(h), std::end(h));
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
void test_find_end_exception()
{
    using namespace pika::execution;

    test_find_end_exception(IteratorTag());

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_find_end_exception(seq, IteratorTag());
    test_find_end_exception(par, IteratorTag());

    test_find_end_exception_async(seq(task), IteratorTag());
    test_find_end_exception_async(par(task), IteratorTag());
}

void find_end_exception_test()
{
    test_find_end_exception<std::random_access_iterator_tag>();
    test_find_end_exception<std::forward_iterator_tag>();
}

//////////////////////////////////////////////////////////////////////////////
template <typename ExPolicy, typename IteratorTag>
void test_find_end_bad_alloc(ExPolicy&& policy, IteratorTag)
{
    static_assert(pika::is_execution_policy<ExPolicy>::value,
        "pika::is_execution_policy<ExPolicy>::value");

    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(100007);
    std::iota(std::begin(c), std::end(c), gen() + 1);
    c[c.size() / 2] = 0;

    std::size_t h[] = {1, 2};

    bool caught_bad_alloc = false;
    try
    {
        pika::ranges::find_end(policy,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c), []() { throw std::bad_alloc(); }),
            std::begin(h), std::end(h));
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
void test_find_end_bad_alloc_async(ExPolicy&& p, IteratorTag)
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator =
        test::decorated_iterator<base_iterator, IteratorTag>;

    std::vector<std::size_t> c(10007);
    std::iota(std::begin(c), std::end(c), gen() + 1);
    c[c.size() / 2] = 0;

    std::size_t h[] = {1, 2};

    bool caught_bad_alloc = false;
    bool returned_from_algorithm = false;
    try
    {
        pika::future<decorated_iterator> f = pika::ranges::find_end(p,
            decorated_iterator(std::begin(c), []() { throw std::bad_alloc(); }),
            decorated_iterator(std::end(c), []() { throw std::bad_alloc(); }),
            std::begin(h), std::end(h));
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
void test_find_end_bad_alloc()
{
    using namespace pika::execution;

    // If the execution policy object is of type vector_execution_policy,
    // std::terminate shall be called. therefore we do not test exceptions
    // with a vector execution policy
    test_find_end_bad_alloc(seq, IteratorTag());
    test_find_end_bad_alloc(par, IteratorTag());

    test_find_end_bad_alloc_async(seq(task), IteratorTag());
    test_find_end_bad_alloc_async(par(task), IteratorTag());
}

void find_end_bad_alloc_test()
{
    test_find_end_bad_alloc<std::random_access_iterator_tag>();
    test_find_end_bad_alloc<std::forward_iterator_tag>();
}

int pika_main(pika::program_options::variables_map& vm)
{
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    gen.seed(seed);

    find_end_test1();
    find_end_test2();
    find_end_test3();
    find_end_test4();
    find_end_exception_test();
    find_end_bad_alloc_test();
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
