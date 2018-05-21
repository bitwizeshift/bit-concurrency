#ifndef BIT_CONCURRENCY_LOCKS_DETAIL_SEMAPHORE_INL
#define BIT_CONCURRENCY_LOCKS_DETAIL_SEMAPHORE_INL

//-----------------------------------------------------------------------------
// Locking
//-----------------------------------------------------------------------------

template<typename Rep, typename Period>
inline bool
  bit::concurrency::semaphore::try_wait_for( const duration<Rep,Period>& duration )
  noexcept
{
  using usec_type = std::chrono::duration<std::uint64_t,std::micro>;

  auto usecs = std::chrono::duration_cast<usec_type>(duration);

  return try_wait( usecs.count() );
}

template<typename Clock, typename Duration>
inline bool
  bit::concurrency::semaphore::try_wait_until( const time_point<Clock,Duration>& time )
  noexcept
{
  auto diff = (time - std::chrono::steady_clock::now());

  // If 'time' was not in the past, try waiting.
  if( diff.count() > 0 ) {
    return try_wait_for( diff );
  }

  // Otherwise try waiting now
  return try_wait();
}

//-----------------------------------------------------------------------------
// Native Handle
//-----------------------------------------------------------------------------

inline bit::concurrency::semaphore::native_handle_type
  bit::concurrency::semaphore::native_handle()
{
  return m_semaphore;
}

#endif /* BIT_CONCURRENCY_LOCKS_DETAIL_SEMAPHORE_INL */
