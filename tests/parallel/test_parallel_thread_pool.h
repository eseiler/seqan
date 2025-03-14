// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2024, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Rene Rahn <rene.rahn@fu-berlin.de>
// ==========================================================================

#include <seqan/parallel.h>

#if defined(__SANITIZE_THREAD__)
#    define SEQAN_IS_NOT_TSAN 0
#elif defined(__has_feature)
#    if __has_feature(thread_sanitizer)
#        define SEQAN_IS_NOT_TSAN 0
#    endif
#else
#    define SEQAN_IS_NOT_TSAN 1
#endif

SEQAN_DEFINE_TEST(test_parallel_thread_pool_construct)
{
    using namespace seqan2;
    // We need to be able to construct a thread pool.
    SEQAN_ASSERT(std::is_default_constructible<ThreadPool>::value);
    SEQAN_ASSERT(!std::is_copy_constructible<ThreadPool>::value);
    SEQAN_ASSERT(!std::is_move_constructible<ThreadPool>::value);
    SEQAN_ASSERT(!std::is_copy_assignable<ThreadPool>::value);
    SEQAN_ASSERT(!std::is_move_assignable<ThreadPool>::value);
}

SEQAN_DEFINE_TEST(test_parallel_thread_pool_spawn)
{
    using namespace seqan2;
    // We need to be able to construct a thread pool
    ThreadPool pool;
    bool res = false;
    auto masterId = std::this_thread::get_id();
    spawn(pool, [=, &res]()
    {
        auto id = std::this_thread::get_id();
        res = id != masterId;
    });

    join(pool);
    SEQAN_ASSERT(res);
}

SEQAN_DEFINE_TEST(test_parallel_thread_pool_join)
{
    using namespace seqan2;

    ThreadPool pool;
    std::vector<int> res(2);
    for (unsigned i = 0; i < res.size(); ++i)
    {
        auto f = [=, &res]
        {
            std::this_thread::sleep_for(std::chrono::seconds(i));
            res[i] = i + 1;
        };
        spawn(pool, f);
    }

#if SEQAN_IS_NOT_TSAN
    SEQAN_ASSERT_NOT(std::accumulate(std::begin(res), std::end(res), 0, std::plus<int>()) == 3);
#endif

    join(pool);
    SEQAN_ASSERT(std::accumulate(std::begin(res), std::end(res), 0, std::plus<int>()) == 3);
}

SEQAN_DEFINE_TEST(test_parallel_thread_pool_destruct)
{
    using namespace seqan2;

    { // Destructor.
        void* buffer = malloc(sizeof(ThreadPool));
        ThreadPool* poolPtr = new(buffer) ThreadPool();

        bool res = false;
        auto f = [&res]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            res = true;
        };
        spawn(*poolPtr, f);

#if SEQAN_IS_NOT_TSAN
        SEQAN_ASSERT_NOT(res);
#endif

        try
        {
            poolPtr->~ThreadPool();
        }
        catch(...)
        {
            SEQAN_ASSERT_FAIL("Could not savely destruct thread pool!");
        }
        SEQAN_ASSERT(res);

        free(buffer);
    }

    {  // Destructor after join
        void* buffer = malloc(sizeof(ThreadPool));
        ThreadPool* poolPtr = new(buffer) ThreadPool();

        bool res = false;
        auto f = [&res]()
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            res = true;
        };
        spawn(*poolPtr, f);

#if SEQAN_IS_NOT_TSAN
        SEQAN_ASSERT_NOT(res);
#endif

        join(*poolPtr);
        SEQAN_ASSERT(res);

        try
        {
            poolPtr->~ThreadPool();
        }
        catch(...)
        {
            SEQAN_ASSERT_FAIL("Could not savely destruct thread pool!");
        }
        SEQAN_ASSERT(res);

        free(buffer);
    }
}
