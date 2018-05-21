/**
 * \file unlock_guard.hpp
 *
 * \brief This file defines a guard type that inverts the normal
 *        std::lock_guard, which allows unlocking on entry and locking
 *        on exit.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 */
#ifndef BIT_CONCURRENCY_UTILITIES_UNLOCK_GUARD_HPP
#define BIT_CONCURRENCY_UTILITIES_UNLOCK_GUARD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <mutex> // std::lock_guard

namespace bit {
  namespace concurrency {
    namespace detail {

      ////////////////////////////////////////////////////////////////////////
      /// \brief A private type that inverts the \c lock and \c unlock
      ///        for an underlying \p Mutex type
      ///
      /// \tparam Mutex the mutex type
      ////////////////////////////////////////////////////////////////////////
      template<typename Mutex>
      class unlockable
      {
        //--------------------------------------------------------------------
        // Constructor
        //--------------------------------------------------------------------
      public:

        unlockable( Mutex& mutex );

        //--------------------------------------------------------------------
        // Locking
        //--------------------------------------------------------------------
      public:

        /// \brief Calls 'unlock' on the underlying mutex
        void lock();

        /// \brief Calls 'lock' on the underlying mutex
        void unlock();

        //--------------------------------------------------------------------
        // Private Members
        //--------------------------------------------------------------------
      private:

        Mutex& m_mutex;
      };

    } // namespace detail

    /// \brief A simple type-wrapper that inverts a std::lock_guard to
    ///        unlock a mutex on entry and lock on exit
    template<typename Mutex>
    using unlock_guard = std::lock_guard<detail::unlockable<Mutex>>;

  } // namespace concurrency
} // namespace bit

#include "detail/unlock_guard.inl"

#endif /* BIT_CONCURRENCY_UTILITIES_UNLOCK_GUARD_HPP */
