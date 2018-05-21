/*****************************************************************************
 * \file
 * \brief This header adds the missing shared_mutex implementation
 *        that was omitted from the c++14 standard. If c++17 mode
 *        is enabled, it uses std::shared_mutex; otherwise it
 *        limits std::shared_timed_mutex to a strictly lockable API
 *****************************************************************************/

/*
  The MIT License (MIT)

  Copyright (c) 2018 Matthew Rodusek

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef BIT_CONCURRENCY_LOCKS_SHARED_MUTEX_HPP
#define BIT_CONCURRENCY_LOCKS_SHARED_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <shared_mutex>

namespace bit {
  namespace concurrency {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A mutex that supports both unique and shared locking
    ///
    /// This was an omission from the c++14 standard that has been rectified
    /// in c++17. As such, with c++17 mode enabled, it defaults to
    /// std::shared_mutex.
    //////////////////////////////////////////////////////////////////////////
#if __cplusplus >= 201703L
    using shared_mutex = std::shared_mutex;
#else
    class shared_mutex : std::shared_timed_mutex
    {
      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------
    public:

      using std::shared_timed_mutex::shared_timed_mutex;

      //-----------------------------------------------------------------------
      // Locking
      //-----------------------------------------------------------------------
    public:

      using std::shared_timed_mutex::lock;
      using std::shared_timed_mutex::try_lock;
      using std::shared_timed_mutex::unlock;

      //-----------------------------------------------------------------------
      // Shared Locking
      //-----------------------------------------------------------------------
    public:

      using std::shared_timed_mutex::lock_shared;
      using std::shared_timed_mutex::try_lock_shared;
      using std::shared_timed_mutex::unlock_shared;
    };
#endif

  } // namespace platform
} // namespace bit

#endif /* BIT_CONCURRENCY_LOCKS_SHARED_MUTEX_HPP */
