#include <bit/concurrency/locks/spin_lock.hpp>

#include <thread>

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

bit::concurrency::spin_lock::spin_lock()
  noexcept
  : m_lock()
{
  m_lock.clear();
}

//----------------------------------------------------------------------------
// Locking
//----------------------------------------------------------------------------

void bit::concurrency::spin_lock::lock()
  noexcept
{
  // if test_and_set returns 'false', then the loop exits and the lock is
  // acquired
  while( m_lock.test_and_set(std::memory_order_acquire) ) {
    std::this_thread::yield();
  }
}

bool bit::concurrency::spin_lock::try_lock()
  noexcept
{
  // if test_and_set returns 'false', then lock has been acquired
  return !m_lock.test_and_set(std::memory_order_acquire);
}

void bit::concurrency::spin_lock::unlock()
  noexcept
{
  m_lock.clear(std::memory_order_release);
}
