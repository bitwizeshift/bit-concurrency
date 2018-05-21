#include <bit/concurrency/locks/semaphore.hpp>

#include <semaphore.h>

#include <cerrno>
#include <cassert>

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

bit::concurrency::semaphore::semaphore()
  : semaphore(0)
{
}

bit::concurrency::semaphore::semaphore( int initial_count )
{
  assert( initial_count >= 0 );
  ::sem_init( &m_semaphore, 0, initial_count );
}

//----------------------------------------------------------------------------

bit::concurrency::semaphore::~semaphore()
{
  ::sem_destroy( &m_semaphore );
}

//----------------------------------------------------------------------------
// Locking
//----------------------------------------------------------------------------

void bit::concurrency::semaphore::wait()
{
  int rc;
  do {
      rc = ::sem_wait(&m_semaphore);
  } while (rc == -1 && errno == EINTR);
}

bool bit::concurrency::semaphore::try_wait()
{
  return ::sem_trywait(&m_semaphore) == 0;
}

void bit::concurrency::semaphore::signal( int count )
{
  while( count-- > 0 ) {
    ::sem_post(&m_semaphore);
  }
}

//----------------------------------------------------------------------------
// Private Member Functions
//----------------------------------------------------------------------------

bool bit::concurrency::semaphore::try_wait( std::uint64_t usecs )
{
  constexpr auto usecs_in_1_sec = 1000000u;
  constexpr auto nsecs_in_1_sec = 1000000000u;

  struct timespec ts;

  ::clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += usecs / usecs_in_1_sec;
  ts.tv_nsec += (usecs % usecs_in_1_sec) * 1000;

  if (ts.tv_nsec > nsecs_in_1_sec) {
    ts.tv_nsec -= nsecs_in_1_sec;
    ++ts.tv_sec;
  }

  int rc;
  do {
    rc = ::sem_timedwait(&m_semaphore, &ts);
  } while (rc == -1 && errno == EINTR);
  return !(rc == -1 && errno == ETIMEDOUT);
}
