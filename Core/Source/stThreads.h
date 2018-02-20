// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stThreads.h
 *
 * Cross-platform functions and classes for threads and synchronization
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_THREADS_H
#define ST_THREADS_H

#include "stPlatform.h"

/**
 * A threadsafe signed 32-bit integer.
 */
ST_PRIVATE_STRUCT(Named="stAtomicS32", Text="Atomic Signed32 Integer", ThreadSafe)
typedef struct stAtomicS32T
{
#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  volatile LONG value;
#elif ST_PLATFORM == ST_PLATFORM_MACH

#else
  u32 unused;
#error "Unknown Platform for stThread"
#endif
} stAtomicS32;

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
#define ST_ATOMIC32_DEFAULT { NULL }
#elif ST_PLATFORM == ST_PLATFORM_MACH
#define ST_ATOMIC32_DEFAULT { 0 }
#else
#define ST_ATOMIC32_DEFAULT { 0 }
#endif

/**
 * Initialise a stAtomicS32 to a default value
 */
ST_INITIALIZER(For=stAtomicS32)
void stAtomicS32_Initialize(stAtomicS32* atomic);

/**
 * Release a stAtomicS32
 */
ST_SHUTDOWN(For=stAtomicS32)
void stAtomicS32_Shutdown(stAtomicS32* atomic);

/**
 * Safely store value in given atomic.
 * @threadsafe
 */
ST_PRIVATE_FUNCTION(Named="stAtomicS32_Store", Text="Safely store value in given atomic.", ThreadSafe)
void stAtomicS32_Store(stAtomicS32* atomic, s32 newValue);

/**
 * Safely fetch most recent value in given atomic.
 * @threadsafe
 */
ST_PRIVATE_FUNCTION(Named="stAtomicS32_Store", Text="Safely fetch most recent value in given atomic.", ThreadSafe)
s32 stAtomicS32_Fetch(stAtomicS32* atomic);


/**
 * Spinlock class for waiting very short periods for another thread to finish some work before
 * another thread can continue.
 */
ST_PRIVATE_STRUCT(Named="stSpinLock", "Spin Lock Object", ThreadSafe)
typedef struct stSpinLockT
{
#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  LONG lock;
#elif ST_PLATFORM == ST_PLATFORM_MACH
  u32 lock; // Unused
#else
  u32 unused;
#error "Unknown Platform for stThread"
#endif
} stSpinLock;

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
#define ST_SPINLOCK_DEFAULT { 0 }
#elif ST_PLATFORM == ST_PLATFORM_MACH
#define ST_SPINLOCK_DEFAULT { 0 }
#else
#define ST_SPINLOCK_DEFAULT { 0 }
#endif

/**
 * Initialise a spinlock
 */
ST_INITIALIZER(For=stSpinLock)
void stSpinLock_Initialize(stSpinLock* lock);

/**
 * Release a spinlock
 */
ST_SHUTDOWN(For=stSpinLock)
void stSpinLock_Shutdown(stSpinLock* lock);

/**
 * Lock a spinlock, and possibly wait if it is already locked to unlock.
 */
ST_PRIVATE_FUNCTION(Named="stSpinLock_Lock", Text="Lock a spinlock", ThreadSafe)
void stSpinLock_Lock(stSpinLock* lock);

/**
 * Lock a spinlock, but if it can't be currently locked then return false
 */
ST_PRIVATE_FUNCTION(Named="stSpinLock_MaybeLock", Text="Maybe lock a spinlock, or return false", MaybeThreadSafe)
stBool stSpinLock_MaybeLock(stSpinLock* lock);

/**
 * Unlock a locked spinlock
 */
ST_PRIVATE_FUNCTION(Named="stAtomicS32_Unlock", Text="Unlock a spinlock", ThreadSafe)
void stSpinLock_Unlock(stSpinLock* lock);

#if 0

#define ST_LOCK(S)    stSpinLock_Lock(S)

#define ST_UNLOCK(S)  stSpinLock_Unlock(S)
#else
#define ST_LOCK(S)    ST_UNUSED(S)
#define ST_UNLOCK(S)  ST_UNUSED(S)
#endif

/**
 * A mutex object for safely locking parts of code or an object between threads
 */
ST_PRIVATE_STRUCT(Named="stMutex", Text="Mutex Object", ThreadSafe)
typedef struct stMutexT
{
#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  CRITICAL_SECTION criticalSection;
#elif ST_PLATFORM == ST_PLATFORM_MACH
  u32 unknown;
#else
  u32 unused;
#error "Unknown Platform for stThread"
#endif
} stMutex;

/**
 * Initalize a mutex object
 */
ST_INITIALIZER(For=stMutex)
void stMutex_Initialize(stMutex* mutex);

/**
 * Release a mutext object
 */
ST_SHUTDOWN(For=stMutex)
void stMutex_Shutdown(stMutex* mutex);

/**
 * Lock (and possibly wait) a mutex
 */
ST_PRIVATE_FUNCTION(Named="stMutex_Lock", Text="Lock a mutex", ThreadSafe)
void stMutex_Lock(stMutex* mutex);

/**
 * Unlock a mutex
 */
ST_PRIVATE_FUNCTION(Named="stAtomicS32_Unlock", Text="Unlock a mutex", ThreadSafe)
void stMutex_Unlock(stMutex* mutex);


/**
 * A portable semaphore object
 */
ST_PRIVATE_STRUCT(Named="stSemaphore", Text="Semaphore Object", ThreadSafe)
typedef struct stSemaphoreT
{
#if ST_PLATFORM == ST_PLATFORM_WINDOWS
  HANDLE handle;
#elif ST_PLATFORM == ST_PLATFORM_MACH
  u32 unknown;
#else
  u32 unused;
#error "Unknown Platform for stThread"
#endif
} stSemaphore;

#if ST_PLATFORM == ST_PLATFORM_WINDOWS
#define ST_SEMAPHORE_DEFAULT { 0 }
#elif ST_PLATFORM == ST_PLATFORM_MACH
#define ST_SEMAPHORE_DEFAULT { 0 }
#else
#define ST_SEMAPHORE_DEFAULT { 0 }
#endif

/**
 * Initialize a semaphore
 */
ST_INITIALIZER(For=stSemaphore)
void stSemaphore_Initialize(stSemaphore* semaphore);

/**
 * Release a semaphore
 */
ST_SHUTDOWN(For=stSemaphore)
void stSemaphore_Shutdown(stSemaphore* semaphore);

/**
 * Post a semaphore
 */
ST_PRIVATE_FUNCTION(Named="stSemaphore_Post", ThreadSafe)
void stSemaphore_Post(stSemaphore* semaphore, u32 count);

/**
 * Wait for semaphore
 */
ST_PRIVATE_FUNCTION(Named="stAtomicS32_Wait",  ThreadSafe)
void stSemaphore_Wait(stSemaphore* semaphore);

typedef s32(*stThreadFn)(void* userData);

/**
 * Portable thread object
 */
ST_PRIVATE_STRUCT(Named="stThread", Text="Thread Object", MaybeThreadSafe)
typedef struct stThreadT
{
  #if ST_PLATFORM == ST_PLATFORM_WINDOWS
    HANDLE      handle;
    DWORD       id;
    stSemaphore semaphore;
  #elif ST_PLATFORM == ST_PLATFORM_MACH
    
  #else
    u32 unused;
    #error "Unknown Platform for stThread"
  #endif

    stThreadFn  function;
    void*       userData;
    stBool      isRunning;
} stThread;

/**
 * Initialise a thread object
 */
ST_INITIALIZER(For=stThread)
void stThread_Initialize(stThread* thread);

/**
 * Release a thread object
 */
ST_SHUTDOWN(For=stThread)
void stThread_Shutdown(stThread* thread);

/**
 * Start a function as a thread with the given user data.
 * @note stackSize is currently ignored
 * @threadsafe
 */
ST_PRIVATE_FUNCTION(Named="stThread_Start", Text="Start a thread", ThreadSafe)
void stThread_Start(stThread* thread, stThreadFn function, void* userData, u32 stackSize);

/**
 * Attempt to stop a thread
 * @threadsafe
 */
ST_PRIVATE_FUNCTION(Named="stThread_Stop", Text="Stop a thread", ThreadSafe)
void stThread_Stop(stThread* thread);



/**
 * A state object used with the stSleeper_Sleep
 * @see stSleeper_Sleep 
 */
ST_PRIVATE_STRUCT(Named="stSleeper", Text="Sleeper Object", MaybeThreadSafe)
typedef struct stSleeperT
{
  u32 spinCount;
} stSleeper;

#define ST_SLEEPER_DEFAULT { 0 }

/**
 * Used in a tight while loop to prevent CPU using High CPU in micro-waits. After a long period it switches to a normal sleep
 */
ST_PRIVATE_STRUCT(Named="stSleeper_Sleep", Text="Used in a tight while loop to prevent CPU using High CPU in micro-waits. After a long period it switches to a normal sleep", NotThreadSafe);
void stSleeper_Sleep(stSleeper* sleeper);

/**
 * Sleep in the called thread for the approximate milliseconds. OS dependent.
 */
ST_PRIVATE_STRUCT(Named="stSleep", Text="Sleep in the called thread for the approximate milliseconds. OS dependent");
void stSleep(u32 milliseconds);


#endif
