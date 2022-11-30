//  Copyright (c) 2017 Antoine Tran Tan
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <pika/execution/traits/is_execution_policy.hpp>
#include <pika/executors/execution_policy.hpp>
#include <pika/functional/first_argument.hpp>
#include <pika/functional/invoke.hpp>
#include <pika/futures/future.hpp>
#include <pika/iterator_support/irange.hpp>
#include <pika/iterator_support/traits/is_iterator.hpp>
#include <pika/synchronization/barrier.hpp>
#include <pika/synchronization/mutex.hpp>
#include <pika/type_support/pack.hpp>

#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace pika {
    /// The class spmd_block defines an interface for launching
    /// multiple images while giving handles to each image to interact with
    /// the remaining images. The \a define_spmd_block function templates create
    /// multiple images of a user-defined function (or lambda) and launches them
    /// in a possibly separate thread. A temporary spmd block object is created
    /// and diffused to each image. The constraint for the function (or lambda)
    /// given to the define_spmd_block function is to accept a spmd_block as
    /// first parameter.
    struct spmd_block
    {
    private:
        using barrier_type = pika::barrier<>;
        using table_type =
            std::map<std::set<std::size_t>, std::shared_ptr<barrier_type>>;
        using mutex_type = pika::mutex;

    public:
        explicit spmd_block(std::size_t num_images, std::size_t image_id,
            barrier_type& barrier, table_type& barriers, mutex_type& mtx)
          : num_images_(num_images)
          , image_id_(image_id)
          , barrier_(barrier)
          , barriers_(barriers)
          , mtx_(mtx)
        {
        }

        // Note: spmd_block class is movable/move-assignable
        // but not copyable/copy-assignable

        spmd_block(spmd_block&&) = default;
        spmd_block(spmd_block const&) = delete;

        spmd_block& operator=(spmd_block&&) = default;
        spmd_block& operator=(spmd_block const&) = delete;

        std::size_t get_num_images() const
        {
            return num_images_;
        }

        std::size_t this_image() const
        {
            return image_id_;
        }

        void sync_all() const
        {
            barrier_.get().arrive_and_wait();
        }

        void sync_images(std::set<std::size_t> const& images) const
        {
            using lock_type = std::lock_guard<mutex_type>;

            table_type& brs = barriers_.get();
            typename table_type::iterator it;

            // Critical section
            {
                lock_type lk(mtx_);
                it = brs.find(images);

                if (it == brs.end())
                {
                    it = brs
                             .insert({images,
                                 std::make_shared<barrier_type>(images.size())})
                             .first;
                }
            }

            if (images.find(image_id_) != images.end())
            {
                it->second->arrive_and_wait();
            }
        }

        void sync_images(std::vector<std::size_t> const& input_images) const
        {
            std::set<std::size_t> images(
                input_images.begin(), input_images.end());
            sync_images(images);
        }

        template <typename Iterator>
        typename std::enable_if<
            traits::is_input_iterator<Iterator>::value>::type
        sync_images(Iterator begin, Iterator end) const
        {
            std::set<std::size_t> images(begin, end);
            sync_images(images);
        }

        template <typename... I>
        typename std::enable_if<
            util::detail::all_of_v<typename std::is_integral<I>::type...>>::type
        sync_images(I... i) const
        {
            std::set<std::size_t> images = {(std::size_t) i...};
            sync_images(images);
        }

    private:
        std::size_t num_images_;
        std::size_t image_id_;
        mutable std::reference_wrapper<barrier_type> barrier_;
        mutable std::reference_wrapper<table_type> barriers_;
        mutable std::reference_wrapper<mutex_type> mtx_;
    };

    namespace detail {
        template <typename F>
        struct spmd_block_helper
        {
        private:
            using barrier_type = pika::barrier<>;
            using table_type =
                std::map<std::set<std::size_t>, std::shared_ptr<barrier_type>>;
            using mutex_type = pika::mutex;

        public:
            std::shared_ptr<barrier_type> barrier_;
            std::shared_ptr<table_type> barriers_;
            std::shared_ptr<mutex_type> mtx_;
            std::decay_t<F> f_;
            std::size_t num_images_;

            template <typename... Ts>
            void operator()(std::size_t image_id, Ts&&... ts) const
            {
                spmd_block block(
                    num_images_, image_id, *barrier_, *barriers_, *mtx_);
                PIKA_INVOKE(f_, PIKA_MOVE(block), PIKA_FORWARD(Ts, ts)...);
            }
        };

        // Asynchronous version
        template <typename ExPolicy, typename F, typename... Args,
            typename = typename std::enable_if<
                pika::is_async_execution_policy<ExPolicy>::value>::type>
        std::vector<pika::future<void>> define_spmd_block(
            ExPolicy&& policy, std::size_t num_images, F&& f, Args&&... args)
        {
            static_assert(pika::is_async_execution_policy<ExPolicy>::value,
                "pika::is_async_execution_policy<ExPolicy>::value");

            using ftype = std::decay_t<F>;
            using first_type =
                typename pika::util::detail::first_argument<ftype>::type;

            using barrier_type = pika::barrier<>;
            using table_type =
                std::map<std::set<std::size_t>, std::shared_ptr<barrier_type>>;
            using mutex_type = pika::mutex;

            static_assert(std::is_same<spmd_block, first_type>::value,
                "define_spmd_block() needs a function or lambda that "
                "has at least a local spmd_block as 1st argument");

            std::shared_ptr<barrier_type> barrier =
                std::make_shared<barrier_type>(num_images);
            std::shared_ptr<table_type> barriers =
                std::make_shared<table_type>();
            std::shared_ptr<mutex_type> mtx = std::make_shared<mutex_type>();

            return pika::parallel::execution::bulk_async_execute(
                policy.executor(),
                detail::spmd_block_helper<F>{
                    barrier, barriers, mtx, PIKA_FORWARD(F, f), num_images},
                pika::detail::irange(std::size_t(0), num_images),
                PIKA_FORWARD(Args, args)...);
        }

        // Synchronous version
        template <typename ExPolicy, typename F, typename... Args,
            typename = typename std::enable_if<
                !pika::is_async_execution_policy<ExPolicy>::value>::type>
        void define_spmd_block(
            ExPolicy&& policy, std::size_t num_images, F&& f, Args&&... args)
        {
            static_assert(pika::is_execution_policy<ExPolicy>::value,
                "pika::is_execution_policy<ExPolicy>::value");

            using ftype = std::decay_t<F>;
            using first_type =
                typename pika::util::detail::first_argument<ftype>::type;

            using barrier_type = pika::barrier<>;
            using table_type =
                std::map<std::set<std::size_t>, std::shared_ptr<barrier_type>>;
            using mutex_type = pika::mutex;

            static_assert(std::is_same<spmd_block, first_type>::value,
                "define_spmd_block() needs a lambda that "
                "has at least a spmd_block as 1st argument");

            std::shared_ptr<barrier_type> barrier =
                std::make_shared<barrier_type>(num_images);
            std::shared_ptr<table_type> barriers =
                std::make_shared<table_type>();
            std::shared_ptr<mutex_type> mtx = std::make_shared<mutex_type>();

            pika::parallel::execution::bulk_sync_execute(policy.executor(),
                detail::spmd_block_helper<F>{
                    barrier, barriers, mtx, PIKA_FORWARD(F, f), num_images},
                pika::detail::irange(std::size_t(0), num_images),
                PIKA_FORWARD(Args, args)...);
        }
    }    // namespace detail

    // Asynchronous version
    template <typename ExPolicy, typename F, typename... Args,
        typename = typename std::enable_if<
            pika::is_async_execution_policy<ExPolicy>::value>::type>
    std::vector<pika::future<void>> define_spmd_block(
        ExPolicy&& policy, std::size_t num_images, F&& f, Args&&... args)
    {
        return detail::define_spmd_block(PIKA_FORWARD(ExPolicy, policy),
            num_images, PIKA_FORWARD(F, f), PIKA_FORWARD(Args, args)...);
    }

    // Synchronous version
    template <typename ExPolicy, typename F, typename... Args,
        typename = typename std::enable_if<
            !pika::is_async_execution_policy<ExPolicy>::value>::type>
    void define_spmd_block(
        ExPolicy&& policy, std::size_t num_images, F&& f, Args&&... args)
    {
        detail::define_spmd_block(PIKA_FORWARD(ExPolicy, policy), num_images,
            PIKA_FORWARD(F, f), PIKA_FORWARD(Args, args)...);
    }

    template <typename F, typename... Args>
    void define_spmd_block(std::size_t num_images, F&& f, Args&&... args)
    {
        detail::define_spmd_block(pika::execution::par, num_images,
            PIKA_FORWARD(F, f), PIKA_FORWARD(Args, args)...);
    }
}    // namespace pika
