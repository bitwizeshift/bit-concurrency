#include <bit/concurrency/locks/semaphore.hpp>

#include <mach/semaphore.h> // ::semaphore_wait, ::semaphore_signal
#include <mach/task.h>      // ::semaphore_destroy, ::semaphore_create, etc

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

  ::semaphore_create(::mach_task_self(), &m_semaphore, SYNC_POLICY_FIFO, initial_count );
}

//----------------------------------------------------------------------------

bit::concurrency::semaphore::~semaphore()
{
  ::semaphore_destroy(::mach_task_self(), m_semaphore);
}

//----------------------------------------------------------------------------
// Locking
//----------------------------------------------------------------------------

void bit::concurrency::semaphore::wait()
{
  ::semaphore_wait(m_semaphore);
}

bool bit::concurrency::semaphore::try_wait()
{
  return ::semaphore_wait_noblock(m_semaphore) != KERN_SUCCESS;
}

void bit::concurrency::semaphore::signal( int count )
{
  while(count-- > 0) {
    ::semaphore_signal(m_semaphore);
  }
}

//----------------------------------------------------------------------------
// Private Member Functions
//----------------------------------------------------------------------------

bool bit::concurrency::semaphore::try_wait( std::uint64_t usecs )
{
  ::mach_timespec_t ts;
  ts.tv_sec = usecs / 1000000u;
  ts.tv_nsec = (usecs % 1000000u) * 1000u;

  // added in OSX 10.10: https://developer.apple.com/library/prerelease/mac/documentation/General/Reference/APIDiffsMacOSX10_10SeedDiff/modules/Darwin.html
  ::kern_return_t rc = ::semaphore_timedwait(m_semaphore, ts);

  return rc == KERN_SUCCESS;
}
