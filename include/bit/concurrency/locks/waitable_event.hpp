/*****************************************************************************
 * \file
 * \brief TODO: Add description
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
#ifndef BIT_CONCURRENCY_LOCKS_WAITABLE_EVENT_HPP
#define BIT_CONCURRENCY_LOCKS_WAITABLE_EVENT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <chrono>             // std::chrono::duration, std::chrono::time_point
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

namespace bit {
  namespace concurrency {

    //////////////////////////////////////////////////////////////////////////
    /// \brief A simple waitable event that helps manage waiting for a valid
    ///        signal condition.
    ///
    /// A waitable_event converts a std::mutex's API into a signaling
    /// semaphore API.
    //////////////////////////////////////////////////////////////////////////
    class waitable_event
    {
      //----------------------------------------------------------------------
      // Constructors / Assignment
      //----------------------------------------------------------------------
    public:

      /// \brief Default constructs a waitable_event that is not yet signaled
      waitable_event();

      // Deleted move constructor
      waitable_event( waitable_event&& other ) = delete;

      // Deleted copy constructor
      waitable_event( const waitable_event& other ) = delete;

      //----------------------------------------------------------------------

      // Deleted move assignment
      waitable_event& operator=( waitable_event&& other ) = delete;

      // Deleted copy assignment
      waitable_event& operator=( const waitable_event& other ) = delete;

      //----------------------------------------------------------------------
      // Waiting
      //----------------------------------------------------------------------
    public:

      /// \brief Blocks the current thread until it is signaled
      void wait();

      /// \brief Blocks the current thread until it is signaled, or until the
      ///        specified duration has been waited for
      ///
      /// \param duration the amount of time to wait for
      /// \return \c true if the event was woken up because it was signaled
      template<typename Rep, typename Period>
      bool wait_for( const std::chrono::duration<Rep,Period>& duration );

      /// \brief Blocks the current thread until it is signaled, or until the
      ///        specified \p time_out has been reached
      ///
      /// \param time_out the time to wait until
      /// \return \c true if the event was woken up because it was signaled
      template<typename Clock, typename Duration>
      bool wait_until( const std::chrono::time_point<Clock,Duration>& time_point );

      //----------------------------------------------------------------------
      // Signaling
      //----------------------------------------------------------------------
    public:

      /// \brief Signals for this waitable_event to stop waiting
      void signal();

      //----------------------------------------------------------------------
      // Private Members
      //----------------------------------------------------------------------
    private:

      std::mutex              m_mutex;
      std::condition_variable m_cv;
      bool                    m_signal;
    };

  } // namespace concurrency
} // namespace bit

#include "detail/waitable_event.inl"

#endif /* BIT_CONCURRENCY_LOCKS_WAITABLE_EVENT_HPP */
