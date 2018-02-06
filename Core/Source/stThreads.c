// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include "stThreads.h"

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
#include <limits.h>
#endif


void stAtomicS32_Initialize(stAtomicS32* atomic)
{
  ST_NULL_CHECK(atomic);
  stAtomicS32_Store(atomic, 0);
}

void stAtomicS32_Shutdown(stAtomicS32* atomic)
{
  ST_NULL_CHECK(atomic);
  stAtomicS32_Store(atomic, 0);
}

void stAtomicS32_Store(stAtomicS32* atomic, s32 newValue)
{
  ST_NULL_CHECK(atomic);

  #if ST_PLATFORM == ST_PLATFORM_WINDOWS
    InterlockedExchange(&atomic->value, newValue);
  #elif ST_PLATFORM == ST_PLATFORM_MACH
    ST_UNUSED(atomic);
    ST_UNUSED(newValue);
    ST_TODO("Atomic Store for Mach");
  #else
    ST_UNUSED(atomic);
    ST_UNUSED(newValue);
    ST_NOOP;
  #endif
}

s32 stAtomicS32_Fetch(stAtomicS32* atomic)
{
  s32 value;

  ST_NULL_CHECK(atomic);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  value = InterlockedExchangeAdd(&atomic->value, 0);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(value);
  ST_TODO("Atomic Store for Mach")
#else
  ST_UNUSED(value);
  ST_NOOP;
#endif

  return value;
}

void stSpinLock_Initialize(stSpinLock* lock)
{
  ST_NULL_CHECK(lock);

  lock->lock = 0;
}

void stSpinLock_Shutdown(stSpinLock* lock)
{
  ST_NULL_CHECK(lock);

  lock->lock = 0;
}

void stSpinLock_Lock(stSpinLock* lock)
{
  ST_NULL_CHECK(lock);

  stSleeper sleeper = ST_SLEEPER_DEFAULT;
  while(stSpinLock_MaybeLock(lock) == false)
    stSleeper_Sleep(&sleeper);
}

stBool stSpinLock_MaybeLock(stSpinLock* lock)
{
  ST_NULL_CHECK(lock);

  #if ST_PLATFORM == ST_PLATFORM_WINDOWS
    return _interlockedbittestandset((volatile long*) &lock->lock, 0) == 1;
  #elif ST_PLATFORM == ST_PLATFORM_MACH
    ST_UNUSED(lock);
    ST_TODO("Spinlock for Mach");
    ST_NOOP;
    return true;
  #else
    ST_UNUSED(lock);
    ST_NOOP;
    return true;
  #endif
}

void stSpinLock_Unlock(stSpinLock* lock)
{
  ST_NULL_CHECK(lock);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  _interlockedbittestandreset((volatile long*) &lock->lock, 0);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(lock);
  ST_TODO("Spinlock for Mach");
#else
  ST_UNUSED(lock);
  ST_NOOP;
#endif
}

void stMutex_Initialize(stMutex* mutex)
{
  ST_NULL_CHECK(mutex);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  InitializeCriticalSection(&mutex->criticalSection);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(mutex);
  ST_TODO("Create Mutex for Mach");
#else
  ST_UNUSED(mutex);
  ST_NOOP;
#endif
}

void stMutex_Shutdown(stMutex* mutex)
{
  ST_NULL_CHECK(mutex);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  DeleteCriticalSection(&mutex->criticalSection);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(mutex);
  ST_TODO("Destroy Mutex for Mach");
#else
  ST_UNUSED(mutex);
  ST_NOOP;
#endif
}

void stMutex_Lock(stMutex* mutex)
{
  ST_NULL_CHECK(mutex);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  EnterCriticalSection(&mutex->criticalSection);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(mutex);
  ST_TODO("Lock Mutex for Mach");
#else
  ST_UNUSED(mutex);
  ST_NOOP;
#endif
}

void stMutex_Unlock(stMutex* mutex)
{
  ST_NULL_CHECK(mutex);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  LeaveCriticalSection(&mutex->criticalSection);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(mutex);
  ST_TODO("Unlock Mutex for Mach");
#else
  ST_UNUSED(mutex);
  ST_NOOP;
#endif
}

void stSemaphore_Initialize(stSemaphore* semaphore)
{
  ST_NULL_CHECK(semaphore);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  semaphore->handle = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
  ST_ASSERT(semaphore->handle != NULL, "Invalid Semaphore");
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(semaphore);
  ST_TODO("Create Semaphore for Mach");
#else
  ST_UNUSED(semaphore);
  ST_NOOP;
#endif
}

void stSemaphore_Shutdown(stSemaphore* semaphore)
{
  ST_NULL_CHECK(semaphore);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  CloseHandle(semaphore->handle);
  semaphore->handle = NULL;
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(semaphore);
  ST_TODO("Destroy Semaphore for Mach");
#else
  ST_UNUSED(semaphore);
  ST_NOOP;
#endif
}

void stSemaphore_Post(stSemaphore* semaphore, u32 count)
{
  ST_NULL_CHECK(semaphore);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  ReleaseSemaphore(semaphore->handle, count, NULL);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(semaphore);
  ST_TODO("Post Semaphore for Mach");
#else
  ST_UNUSED(semaphore);
  ST_NOOP;
#endif

}

void stSemaphore_Wait(stSemaphore* semaphore)
{
  ST_NULL_CHECK(semaphore);

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  WaitForSingleObject(semaphore->handle, INFINITE);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(semaphore);
  ST_TODO("Post Semaphore for Mach");
#else
  ST_UNUSED(semaphore);
  ST_NOOP;
#endif
}

void stThread_Initialize(stThread* thread)
{
  ST_NULL_CHECK(thread);


#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  thread->handle    = NULL;
  thread->id        = 0;
  thread->semaphore.handle = 0 ; //ST_SEMAPHORE_DEFAULT;
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(thread);
  ST_TODO("Initialize Thread for Mach");
#else
  ST_UNUSED(thread);
  ST_NOOP;
#endif

  thread->function  = NULL;
  thread->userData  = NULL;
  thread->isRunning = false;
}

void stThread_Shutdown(stThread* thread)
{
  ST_UNUSED(thread);
  ST_NOOP;
}

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
static DWORD WINAPI runThread(void* threadObject)
#else
static s32 runThread(void* threadObject)
#endif
{
  ST_TODO("Semaphore post here??");

  stThread* thread = (stThread*) threadObject;
  s32 returnValue = thread->function(thread->userData);

  return returnValue;
}

void stThread_Start(stThread* thread, stThreadFn function, void* userData, u32 stackSize)
{
  ST_NULL_CHECK(thread);

  ST_ASSERT(thread->isRunning == false, "Expected Thread not to be running");
  ST_ASSERT(function != NULL, "Expected Thread Entry function");

  thread->function  = function;
  thread->userData  = userData;
  thread->isRunning = true;

  ST_PRINT_CHATTERF("Starting Thread %p", thread->function);

  #if ST_PLATFORM == ST_PLATFORM_WINDOWS
    thread->handle  = CreateThread(NULL, stackSize, runThread, &thread, 0, NULL);
  #elif ST_PLATFORM == ST_PLATFORM_MACH
    ST_TODO("Launch thread on Mach")
  #else
    ST_NOOP;
  #endif

    ST_PRINT_CHATTERF("Started Thread %p", function);
}

void stThread_Stop(stThread* thread)
{
  ST_NULL_CHECK(thread);

  ST_ASSERT(thread->isRunning == true, "Expected Thread to be running");
  
  ST_PRINT_CHATTERF("Stopping Thread %p", thread->function);

  #if ST_PLATFORM == ST_PLATFORM_WINDOWS
    WaitForSingleObject(thread->handle, INFINITE);
    CloseHandle(thread->handle);
  #elif ST_PLATFORM == ST_PLATFORM_MACH

  #else

  #endif

  ST_PRINT_CHATTERF("Stopped thread %p", thread->function);

  stThread_Initialize(thread); // Reset thread object.
}

void stSleep(u32 milliseconds)
{
#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  Sleep(milliseconds);
#elif ST_PLATFORM == ST_PLATFORM_MACH
  ST_UNUSED(milliseconds);
  ST_TODO("Sleep for Mach");
#else
  ST_UNUSED(millisecondds);
  ST_NOOP;
#endif
}

void stSleeper_Sleep(stSleeper* sleeper)
{
  if (sleeper->spinCount < 4000)
  {
    ++sleeper->spinCount;

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
    _mm_pause();
#elif ST_PLATFORM == ST_PLATFORM_MACH
    ST_TODO("__mm_pause for Mach");
#else
    ST_NOOP;
#endif
  }
  else
  {
    stSleep(1);
  }
}
