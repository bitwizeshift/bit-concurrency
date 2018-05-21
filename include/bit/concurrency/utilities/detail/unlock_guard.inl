#ifndef BIT_CONCURRENCY_UTILITIES_DETAIL_UNLOCK_GUARD_INL
#define BIT_CONCURRENCY_UTILITIES_DETAIL_UNLOCK_GUARD_INL

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------

template<typename Mutex>
inline bit::concurrency::detail::unlockable<Mutex>::unlockable( Mutex& mutex )
  : m_mutex(mutex)
{

}

//----------------------------------------------------------------------------
// Locking
//----------------------------------------------------------------------------

template<typename Mutex>
inline void bit::concurrency::detail::unlockable<Mutex>::lock()
{
  m_mutex.unlock();
}

template<typename Mutex>
inline void bit::concurrency::detail::unlockable<Mutex>::unlock()
{
  m_mutex.lock();
}

#endif /* BIT_CONCURRENCY_UTILITIES_DETAIL_UNLOCK_GUARD_INL */
