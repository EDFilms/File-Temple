// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	1.0.0
 */

#ifndef ST_LIST_H
#define ST_LIST_H

#define ST_LIST_NONE(LIST) (LIST->first == NULL)
#define ST_LIST_ANY(LIST)  (LIST->first != NULL)
#define ST_LIST_ONE(LIST)  (LIST->first != NULL && LIST->first == LIST->last)
#define ST_LIST_MANY(LIST) (LIST->first != NULL && LIST->first != LIST->last)

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

#define ST_LIST_RESET(LIST) \
  do { LIST->first = NULL; LIST->last = NULL; } while(0)

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


#define ST_LIST_FOR_BEGIN(TYPE, LIST, ITERATOR) \
  do { TYPE* ITERATOR = LIST->first; while(ITERATOR != NULL) { TYPE* nextIterator = ITERATOR->next

#define ST_LIST_FOR_END(TYPE, LIST, ITERATOR) \
  ITERATOR = nextIterator; }} while(0)

#define ST_LIST_FOR_CONTINUE(TYPE, LIST, ITERATOR) \
  ITERATOR = nextIterator; continue;

#define ST_LIST_COUNT(COUNTER, TYPE, LIST) \
  do { COUNTER = 0; TYPE* ITERATOR = LIST->first; while(ITERATOR != NULL) { ITERATOR = ITERATOR->next; COUNTER++; } } while(0)

#endif
