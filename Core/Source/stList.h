// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * stList.h
 *
 * Double Linked-list macros
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#ifndef ST_LIST_H
#define ST_LIST_H

/**
 * Is the list empty?
 */
#define ST_LIST_NONE(LIST) (LIST->first == NULL)

/**
 * Is the list not empty?
 */
#define ST_LIST_ANY(LIST)  (LIST->first != NULL)

/**
 * Is the list got exactly one item?
 */
#define ST_LIST_ONE(LIST)  (LIST->first != NULL && LIST->first == LIST->last)

/**
 * Is the list got more than one item?
 */
#define ST_LIST_MANY(LIST) (LIST->first != NULL && LIST->first != LIST->last)

/**
 * Add an item to the list
 */
#define ST_LIST_ADD(LIST, ITEM) \
  do { \
    ITEM->next = NULL;\
    ITEM->prev = NULL;\
    if (LIST->first == NULL) \
    {\
      LIST->first = LIST->last = ITEM;\
    }\
    else\
    {\
      LIST->last->next = ITEM;\
      ITEM->prev = LIST->last;\
      LIST->last = ITEM;\
    }\
  } while(0)
  
/**
 * Remove item from the list
 */
#define ST_LIST_REMOVE(TYPE, LIST, ITEM) \
  do { \
    if (LIST->first == ITEM && LIST->last == ITEM)\
    {\
      LIST->first = NULL;\
      LIST->last  = NULL;\
    }\
    else if (LIST->first == ITEM) \
    {\
      LIST->first = ITEM->next; \
      LIST->first->prev = NULL; \
    }\
    else if (LIST->last == ITEM)\
    {\
      LIST->last = ITEM->prev;\
      LIST->last->next = NULL;\
    }\
    else\
    {\
      TYPE* next = ITEM->next;\
      TYPE* prev = ITEM->prev;\
      next->prev = prev;\
      prev->next = next;\
    }\
    ITEM->next = NULL;\
    ITEM->prev = NULL;\
  } while(0)
  
/**
 * Reset the list to empty
 */
#define ST_LIST_RESET(LIST) \
  do { LIST->first = NULL; LIST->last = NULL; } while(0)
  
/**
 * Clear the list, and delete items via ITEM_DELETE_FN
 */
#define ST_LIST_CLEAR(TYPE, LIST, ITEM_DELETE_FN) \
  do { \
    TYPE* iterator = LIST->first; \
    while(iterator != NULL) \
    {\
      TYPE* nextIterator = iterator->next;\
      ITEM_DELETE_FN(iterator);\
      iterator = nextIterator;\
    }\
    LIST->first = NULL; \
    LIST->last  = NULL; \
  } while(0)

/**
 * Try and fetch an item from the list using TERM and ITEM_FETCH_FN
 */
#define ST_LIST_MAYBE_FETCH(TYPE, LIST, INTO, TERM, ITEM_FETCH_FN) \
  do { \
    TYPE* iterator = LIST->first; \
    INTO = NULL; \
    while(iterator != NULL) \
    {\
      TYPE* nextIterator = iterator->next;\
      if (ITEM_FETCH_FN(iterator, TERM))\
      {\
        INTO = iterator;\
        break;\
      }\
      iterator = nextIterator;\
    }\
  } while(0)

/**
 * Macro to iterate through the list
 * @see ST_LIST_FOR_END
 * @see ST_LIST_FOR_CONTINUE
 */
#define ST_LIST_FOR_BEGIN(TYPE, LIST, ITERATOR) \
  do { TYPE* ITERATOR = LIST->first; while(ITERATOR != NULL) { TYPE* nextIterator = ITERATOR->next
  
/**
 * Macro to iterate through the list. Ending part
 * @see ST_LIST_FOR_END
 * @see ST_LIST_FOR_CONTINUE
 */
#define ST_LIST_FOR_END(TYPE, LIST, ITERATOR) \
  ITERATOR = nextIterator; }} while(0)
  
/**
 * Macro to iterate through the list. Use this instead of <code>continue;</code>
 * @see ST_LIST_FOR_END
 * @see ST_LIST_FOR_CONTINUE
 */
#define ST_LIST_FOR_CONTINUE(TYPE, LIST, ITERATOR) \
  ITERATOR = nextIterator; continue;
  
/**
 * Count number of items in list
 */
#define ST_LIST_COUNT(COUNTER, TYPE, LIST) \
  do { COUNTER = 0; TYPE* ITERATOR = LIST->first; while(ITERATOR != NULL) { ITERATOR = ITERATOR->next; COUNTER++; } } while(0)

#endif
