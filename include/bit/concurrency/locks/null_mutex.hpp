/*****************************************************************************
 * \file
 * \brief This header contains an implementation of a null Lockable
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
#ifndef BIT_CONCURRENCY_LOCKS_NULL_MUTEX_HPP
#define BIT_CONCURRENCY_LOCKS_NULL_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <chrono>

namespace bit {
  namespace concurrency {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A mutex that follows the null-object-pattern.
    ///
    /// This is used for single-threading in a generic API.
    //////////////////////////////////////////////////////////////////////////
    class null_mutex
    {
      template<typename Rep, typename Period>
      using duration = std::chrono::duration<Rep,Period>;

      template<typename Clock, typename Duration>
      using time_point = std::chrono::time_point<Clock,Duration>;

      //----------------------------------------------------------------------
      // Constructors
      //----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a null_mutex
      constexpr null_mutex() = default;

      null_mutex( const null_mutex& ) = delete;
      null_mutex( null_mutex&& ) = delete;

      //----------------------------------------------------------------------
      // Exclusive Locking
      //----------------------------------------------------------------------
    public:

      /// \brief 'Locks' this null_mutex
      constexpr void lock() noexcept;

      /// \brief Attempts to 'lock' this null_mutex
      ///
      /// \return \c true
      constexpr bool try_lock() noexcept;

      /// \brief Attempts to 'lock' this null_mutex for \p duration
      ///
      /// \param duration the timeout duration
      /// \return \c true
      template<typename Rep, typename Period>
      constexpr bool try_lock_for( const duration<Rep,Period>& duration ) noexcept;

      /// \brief Attempts to 'lock' this null_mutex until \p time
      ///
      /// \param time the time point to stop trying
      /// \return \c true
      template<typename Clock, typename Duration>
      constexpr bool try_lock_until( const time_point<Clock,Duration>& time ) noexcept;

      /// \brief 'Unlocks' this null_mutex
      constexpr void unlock() noexcept;

      //----------------------------------------------------------------------
      // Shared Locking
      //----------------------------------------------------------------------
    public:

      /// \copydoc null_mutex::lock()
      constexpr void lock_shared() noexcept;

      /// \copydoc null_mutex::try_lock_shared()
      constexpr bool try_lock_shared() noexcept;

      /// \copydoc null_mutex::try_lock_shared_for( const duration<Rep,Period>& )
      template<typename Rep, typename Period>
      constexpr bool try_lock_shared_for( const duration<Rep,Period>& duration ) noexcept;

      /// \copydoc null_mutex::try_lock_shared_until( const time_point<Clock,Duration>& )
      template<typename Clock, typename Duration>
      constexpr bool try_lock_shared_until( const time_point<Clock,Duration>& time ) noexcept;

      /// \copydoc null_mutex::unlock_shared()
      constexpr void unlock_shared() noexcept;
    };

  } // namespace concurrency
} // namespace bit

#include "detail/null_mutex.inl"

#endif /* BIT_CONCURRENCY_LOCKS_NULL_MUTEX_HPP */
