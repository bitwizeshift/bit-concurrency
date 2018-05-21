/*****************************************************************************
 * \file
 * \brief TODO: Add description
 *****************************************************************************/

/*
  Implementation of the semaphore below is an adaptation of Jeff Preshing's
  portable + lightweight semaphore from
  https://github.com/preshing/cpp11-on-multicore/blob/master/common/sema.h

  LICENSE:

  Copyright (c) 2015 Jeff Preshing

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgement in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  ALTERATIONS:
  - Added timed waiting
*/
#ifndef BIT_CONCURRENCY_LOCKS_SPINNING_SEMAPHORE_HPP
#define BIT_CONCURRENCY_LOCKS_SPINNING_SEMAPHORE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "semaphore.hpp"

#include <atomic>
#include <cassert>
#include <chrono>

namespace bit {
  namespace concurrency {

    ///////////////////////////////////////////////////////////////////////////
    /// \brief A lightweight semaphore that uses partial spinning to avoid
    ///        yielding processor time-slices and causing context switches
    ///
    ///////////////////////////////////////////////////////////////////////////
    class spinning_semaphore
    {
      template<typename Rep, typename Period>
      using duration = std::chrono::duration<Rep,Period>;

      template<typename Clock, typename Duration>
      using time_point = std::chrono::time_point<Clock,Duration>;

      //-----------------------------------------------------------------------
      // Constructors / Assignment
      //-----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a spinning_semaphore with count 1
      spinning_semaphore();

      /// \brief Constructs a spinning_semaphore with count \p initial_count
      ///
      /// \param initial_count the initial count for the semaphore
      explicit spinning_semaphore( int initial_count );

      // deleted copy constructor
      spinning_semaphore( const spinning_semaphore& ) = delete;

      // deleted move constructor
      spinning_semaphore( spinning_semaphore&& ) = delete;

      //-----------------------------------------------------------------------

      // deleted copy assignment
      spinning_semaphore& operator=( const spinning_semaphore& ) = delete;

      // deleted move assignment
      spinning_semaphore& operator=( spinning_semaphore&& ) = delete;

      //----------------------------------------------------------------------
      // Locking
      //----------------------------------------------------------------------
    public:

      /// \brief Waits for an available entry in the semaphore
      void wait();

      /// \brief Attempts to wait an available entry in the semaphore
      ///
      /// \return \c true if access was acquired
      bool try_wait();

      /// \brief Attempts to wait for the specified duration, returning the
      ///        success
      ///
      /// \param duration the timeout duration
      /// \return \c true if access was acquired
      template<typename Rep, typename Period>
      bool try_wait_for( const duration<Rep,Period>& duration ) noexcept;

      /// \brief Attempts to wait until the specified time point, returning the
      ///        success
      ///
      /// \param time the time point to stop trying
      /// \return \c true
      template<typename Clock, typename Duration>
      bool try_wait_until( const time_point<Clock,Duration>& time ) noexcept;

      /// \brief Signals that \p count threads may access the semaphore
      ///
      /// \param count
      void signal( int count = 1 );

      //-----------------------------------------------------------------------
      // Private Members
      //-----------------------------------------------------------------------
    private:

      std::atomic<int> m_count;
      semaphore        m_semaphore;

      //-----------------------------------------------------------------------
      // Private Access
      //-----------------------------------------------------------------------
    private:

      void partial_spin_wait();

      template<typename Rep, typename Period>
      bool try_partial_spin_wait( const duration<Rep,Period>& duration );
    };

  } // namespace concurrency
} // namespace bit

#include "detail/spinning_semaphore.inl"

#endif /* BIT_CONCURRENCY_LOCKS_SPINNING_SEMAPHORE_HPP */
