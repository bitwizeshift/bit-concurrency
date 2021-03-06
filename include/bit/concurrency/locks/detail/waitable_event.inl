#ifndef BIT_CONCURRENCY_LOCKS_DETAIL_WAITABLE_EVENT_INL
#define BIT_CONCURRENCY_LOCKS_DETAIL_WAITABLE_EVENT_INL

//----------------------------------------------------------------------------
// Constructor / Assignment
//----------------------------------------------------------------------------

inline bit::concurrency::waitable_event::waitable_event()
  : m_signal(false)
{

}

//----------------------------------------------------------------------------
// Waiting
//----------------------------------------------------------------------------

inline void bit::concurrency::waitable_event::wait()
{
  std::unique_lock<std::mutex> lock(m_mutex);

  m_cv.wait(lock, [this](){ return m_signal; });
  m_signal = false;
}

template<typename Rep, typename Period>
inline bool bit::concurrency::waitable_event
  ::wait_for( const std::chrono::duration<Rep,Period>& duration )
{
  auto success = false;
  { // critical section
    std::unique_lock<std::mutex> lock(m_mutex);

    success = m_cv.wait_for(lock, duration, [this](){ return m_signal; });

    // Only locks if successful (thread-safe access)
    if( success ) m_signal = false;
  }

  return success;
}

template<typename Clock, typename Duration>
inline bool bit::concurrency::waitable_event
  ::wait_until( const std::chrono::time_point<Clock,Duration>& time_point )
{
  auto success = false;
  { // critical section
    std::unique_lock<std::mutex> lock(m_mutex);

    success = m_cv.wait_until(lock, time_point, [this](){ return m_signal; });

    // Only locks if successful (thread-safe access)
    if( success ) m_signal = false;
  }

  return success;
}

//----------------------------------------------------------------------------
// Signaling
//----------------------------------------------------------------------------

inline void bit::concurrency::waitable_event::signal()
{
  { // critical section
    std::unique_lock<std::mutex> lock(m_mutex);

    m_signal = true;
  }
  m_cv.notify_all();
}

#endif /* BIT_CONCURRENCY_LOCKS_DETAIL_WAITABLE_EVENT_INL */
