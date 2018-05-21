#include <bit/concurrency/locks/semaphore.hpp>

#ifndef NOMINMAX
# define NOMINMAX 1
#endif
#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>

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
  static constexpr auto max_long = 0x7fffffffu;

  m_semaphore = ::CreateSemaphoreW( nullptr, initial_count, max_long, nullptr );
}

//----------------------------------------------------------------------------

bit::concurrency::semaphore::~semaphore()
{
  ::CloseHandle( m_semaphore );
}

//----------------------------------------------------------------------------
// Locking
//----------------------------------------------------------------------------

void bit::concurrency::semaphore::wait()
{
  static constexpr auto infinite = 0xffffffffu;

  ::WaitForSingleObject( m_semaphore, infinite );
}

bool bit::concurrency::semaphore::try_wait()
{
  return ::WaitForSingleObject( m_semaphore, 0 ) == WAIT_OBJECT_0;
}

void bit::concurrency::semaphore::signal( int count )
{
  ::ReleaseSemaphore( m_semaphore, count, nullptr );
}

//----------------------------------------------------------------------------
// Private Member Functions
//----------------------------------------------------------------------------

bool bit::concurrency::semaphore::try_wait( std::uint64_t usecs )
{
  const auto milliseconds = static_cast<::DWORD>(usecs / 1000u);

  return ::WaitForSingleObject( m_semaphore, milliseconds ) != WAIT_TIMEOUT;
}
