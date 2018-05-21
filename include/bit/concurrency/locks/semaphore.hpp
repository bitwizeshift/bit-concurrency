/*****************************************************************************
 * \file
 * \brief todo: fill in documentation
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

  - Added 'try_wait'
  - Fixed includes for macos 'semaphore' implementation
  - Split into separate cpp files, rather than inline in a single header. This
    prevents polluting the current API by #include <windows.h> for windows
    targets

 */
#ifndef BIT_CONCURRENCY_LOCKS_SEMAPHORE_HPP
#define BIT_CONCURRENCY_LOCKS_SEMAPHORE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <chrono>
#include <cstdint>

#if defined(__MACH__)
#include <mach/semaphore.h> // ::semaphore_t
#elif defined(__unix__)
#include <semaphore.h> // ::sem_t
#elif defined(_WIN32)
// nothing needed; HANDLE is void*
#else
# error semaphore.hpp: no semaphore implementation available
#endif

namespace bit {
  namespace concurrency {

    //////////////////////////////////////////////////////////////////////////
    /// \brief Implementation of a system semaphore
    ///
    //////////////////////////////////////////////////////////////////////////
    class semaphore
    {
      template<typename Rep, typename Period>
      using duration = std::chrono::duration<Rep,Period>;

      template<typename Clock, typename Duration>
      using time_point = std::chrono::time_point<Clock,Duration>;

      //----------------------------------------------------------------------
      // Public Member Types
      //----------------------------------------------------------------------
    public:

#if defined(__MACH__)
      using native_handle_type = ::semaphore_t; ///< Semaphore type for mac
#elif defined(__unix__) || defined(__unix)
      using native_handle_type = ::sem_t; ///< Semaphore type for *nix
#elif defined(_WIN32) || defined(_WIN64)
      using native_handle_type = void*; ///< Semaphore type for windows
#else
# error Operating system not detected or not properly supported
#endif

      //----------------------------------------------------------------------
      // Constructors / Destructor / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Default-constructs a semaphore with count 0
      semaphore();

      /// \brief Constructs a semaphore with count \p initial_count
      ///
      /// \param initial_count the initial count for the semaphore
      explicit semaphore( int initial_count );

      // deleted copy constructor
      semaphore( const semaphore& ) = delete;

      // deleted move constructor
      semaphore( semaphore&& ) = delete;

      //----------------------------------------------------------------------

      /// \brief Destroys this semaphore
      ~semaphore();

      //----------------------------------------------------------------------

      // deleted copy assignment
      semaphore& operator=( const semaphore& ) = delete;

      // deleted move assignment
      semaphore& operator=( semaphore&& ) = delete;

      //----------------------------------------------------------------------
      // Locking
      //----------------------------------------------------------------------
    public:

      /// \brief Waits for an available entry in the semaphore
      void wait();

      /// \brief Tries to wait on an available entry in the semaphore, returning
      ///        its success
      ///
      /// \return \c true if this succeeds in updating the semaphore
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

      //----------------------------------------------------------------------
      // Native Handle
      //----------------------------------------------------------------------
    public:

      /// \brief Gets the underlying native handle for this semaphore
      ///
      /// \return the underlying handle
      native_handle_type native_handle();

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      native_handle_type m_semaphore; ///< the underlying semaphore

      //----------------------------------------------------------------------
      // Private Member Functions
      //----------------------------------------------------------------------
    private:

      bool try_wait( std::uint64_t usecs );
    };

  } // namespace concurrency
} // namespace bit

#include "detail/semaphore.inl"

#endif /* BIT_CONCURRENCY_LOCKS_SEMAPHORE_HPP */
