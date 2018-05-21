#ifndef BIT_CONCURRENCY_LOCKS_DETAIL_SPINNING_SEMAPHORE_INL
#define BIT_CONCURRENCY_LOCKS_DETAIL_SPINNING_SEMAPHORE_INL

//=============================================================================
// Inline Definitions : spinning_semaphore
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

inline bit::concurrency::spinning_semaphore::spinning_semaphore()
  : spinning_semaphore(0)
{

}

inline bit::concurrency::spinning_semaphore::spinning_semaphore( int count )
  : m_count(count),
    m_semaphore(0)
{
  assert( count > 0 );
}

//-----------------------------------------------------------------------------
// Locking
//-----------------------------------------------------------------------------

inline void bit::concurrency::spinning_semaphore::wait()
{
  if( !try_wait() ) {
    partial_spin_wait();
  }
}

inline bool bit::concurrency::spinning_semaphore::try_wait()
{
  auto old = m_count.load(std::memory_order_relaxed);

  return (old > 0) &&
         m_count.compare_exchange_strong(old, old-1, std::memory_order_acquire);
}

template<typename Rep, typename Period>
inline bool
  bit::concurrency::spinning_semaphore
  ::try_wait_for( const duration<Rep,Period>& duration )
  noexcept
{
  return try_partial_spin_wait( duration );
}

template<typename Clock, typename Duration>
inline bool
  bit::concurrency::spinning_semaphore
  ::try_wait_until( const time_point<Clock,Duration>& time )
  noexcept
{
  auto diff = (time - std::chrono::high_resolution_clock::now());

  // If 'time' was not in the past, try waiting.
  if( diff.count() > 0 ) {
    return try_wait_for( diff );
  }

  // Otherwise try waiting now
  return try_wait();
}

inline void bit::concurrency::spinning_semaphore::signal( int count )
{
  auto old = m_count.fetch_add(count, std::memory_order_release);

  auto to_release = -old < count ? -old : count;
  if( to_release > 0 ) {
    m_semaphore.signal( to_release );
  }
}

//-----------------------------------------------------------------------------
// Private Access
//-----------------------------------------------------------------------------

inline void bit::concurrency::spinning_semaphore::partial_spin_wait()
{
  auto old  = int();
  auto spin = int(10000);

  while( spin-- ) {
    old = m_count.load(std::memory_order_relaxed);
    if( (old > 0) && m_count.compare_exchange_strong(old, old-1, std::memory_order_acquire) ) {
      return;
    }
    // Prevent the compiler from collapsing the loop.
    std::atomic_signal_fence(std::memory_order_acquire);
  }

  old = m_count.fetch_sub(1, std::memory_order_acquire);

  if(old <= 0) {
    m_semaphore.wait();
  }
}

template<typename Rep, typename Period>
inline bool bit::concurrency::spinning_semaphore
  ::try_partial_spin_wait( const duration<Rep,Period>& duration )
{
  auto start = std::chrono::steady_clock::now();
  auto old   = int();
  auto spin  = int(10000);

  while( spin-- ) {

    // If we wait the entire duration and aren't signaled, return false
    auto diff = (std::chrono::steady_clock::now() - start);
    if( diff >= duration ) {
      return false;
    }

    // If we get signaled, return true
    old = m_count.load(std::memory_order_relaxed);
    if( (old > 0) && m_count.compare_exchange_strong(old, old-1, std::memory_order_acquire) ) {
      return true;
    }
    // Prevent the compiler from collapsing the loop.
    std::atomic_signal_fence(std::memory_order_acquire);
  }

  old = m_count.fetch_sub(1, std::memory_order_acquire);

  if(old <= 0) {
    auto remaining = (std::chrono::steady_clock::now() - start) - duration;
    if( remaining.count() > 0 ) {
      return m_semaphore.try_wait_for( remaining );
    }
    return m_semaphore.try_wait();
  }
  return true;
}

#endif /* BIT_CONCURRENCY_LOCKS_DETAIL_SPINNING_SEMAPHORE_INL */
