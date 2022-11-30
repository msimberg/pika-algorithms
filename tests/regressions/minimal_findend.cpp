//  Copyright (c) 2014 Grant Mercer
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <pika/init.hpp>
#include <pika/parallel/algorithms/find.hpp>
#include <pika/testing.hpp>

#include <cstddef>
#include <iterator>
#include <string>
#include <vector>

namespace test {
    ///////////////////////////////////////////////////////////////////////////
    template <typename BaseIterator, typename IteratorTag>
    struct decorated_iterator
      : pika::util::iterator_adaptor<
            decorated_iterator<BaseIterator, IteratorTag>, BaseIterator, void,
            IteratorTag>
    {
    private:
        using base_type = pika::util::iterator_adaptor<
            decorated_iterator<BaseIterator, IteratorTag>, BaseIterator, void,
            IteratorTag>;

    public:
        decorated_iterator() {}

        decorated_iterator(BaseIterator base)
          : base_type(base)
        {
        }

        decorated_iterator(BaseIterator base, std::function<void()> f)
          : base_type(base)
          , m_callback(f)
        {
        }

    private:
        friend class pika::util::iterator_core_access;

        typename base_type::reference dereference() const
        {
            if (m_callback)
                m_callback();
            return *(this->base());
        }

    private:
        std::function<void()> m_callback;
    };
}    // namespace test

void find_end_failing_test()
{
    using base_iterator = std::vector<std::size_t>::iterator;
    using decorated_iterator = test::decorated_iterator<base_iterator,
        std::random_access_iterator_tag>;

    std::vector<std::size_t> c(10007, 0);
    std::size_t h[] = {1, 2};

    bool caught_exception = false;
    try
    {
        std::find_end(decorated_iterator(std::begin(c),
                          []() { throw std::runtime_error("error"); }),
            decorated_iterator(
                std::end(c), []() { throw std::runtime_error("error"); }),
            std::begin(h), std::end(h));

        // should never reach this point
        PIKA_TEST(false);
    }
    catch (std::runtime_error const&)
    {
        caught_exception = true;
    }
    catch (...)
    {
        PIKA_TEST(false);
    }

    PIKA_TEST(caught_exception);
}

int pika_main()
{
    find_end_failing_test();
    return pika::finalize();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> const cfg = {"pika.os_threads=all"};

    pika::init_params init_args;
    init_args.cfg = cfg;

    PIKA_TEST_EQ_MSG(pika::init(pika_main, argc, argv, init_args), 0,
        "pika main exited with non-zero status");

    return 0;
}
