// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stLazyLoad.h
 *
 * Macros to delay fetching of saved-objects until absolutely necessary
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_LAZY_LOAD_H
#define ST_LAZY_LOAD_H

#include "stPlatform.h"

/**
 * Macro that defines a lazy-load type
 */
#define ST_LAZY_LOAD_T(TYPE, P)                                     struct { u32 id; stBool32 bLazyLoad : 1; stBool32 bAdjustmentAllowed : 1; TYPE* object; P p; }

/**
 * Internal macro to fetch id (UNUSED)
 */
#define ST_LAZY_LOAD_GET_ID(LL)                                     ((X).id)

/**
 * Macro to lazy load an object
 */
#define ST_LAZY_FETCH_OBJECT(TYPE, LL)                              do {\
                                                                         (LL).object = stLazyLoadOrGet_##TYPE(&(LL));\
                                                                    } while(0)
/**
 * Macro to maybe fetch an object if it needs to.
 */
#define ST_LAZY_MAYBE_FETCH_OBJECT(TYPE, LL)                        do {\
                                                                       if ((LL).object == NULL && (LL).bLazyLoad) {\
                                                                         (LL).object = stLazyLoadOrGet_##TYPE(&(LL));\
                                                                       }\
                                                                    } while(0)

/**
 * Macro to load an object but fetch it first
 */
#define ST_LAZY_GET_OBJECT(TYPE, LL, INTO)                          do {\
                                                                       ST_LAZY_MAYBE_FETCH_OBJECT(TYPE, LL); \
                                                                       INTO = (LL).object;\
                                                                    } while(0)

/**
 * Macro to Initialise a lazy loader
 */
#define ST_LAZY_LOAD_INITIALISE(LL, TYPE, ID, LAZY_LOAD, P)         do { \
                                                                     (LL).id = ID; \
                                                                     (LL).bLazyLoad = LAZY_LOAD; \
                                                                     (LL).object = NULL; \
                                                                     (LL).p = P; \
                                                                     if ((LL).bLazyLoad) {\
                                                                       ST_LAZY_MAYBE_FETCH_OBJECT(TYPE, (LL)); \
                                                                     }\
                                                                    } while(0)

/**
 * Macro to Shutdown/Release a lazy loader
 */
#define ST_LAZY_LOAD_SHUTDOWN(LL)                                   do {\
                                                                      (LL).id = 0;\
                                                                      (LL).bLazyLoad = 0;\
                                                                      (LL).object = NULL;\
                                                                    } while(0)
#endif

