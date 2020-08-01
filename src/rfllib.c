/********************************************************************/
/*                                                                  */
/*  hi   Interpreter for Seed7 programs.                            */
/*  Copyright (C) 1990 - 2008  Thomas Mertes                        */
/*                                                                  */
/*  This program is free software; you can redistribute it and/or   */
/*  modify it under the terms of the GNU General Public License as  */
/*  published by the Free Software Foundation; either version 2 of  */
/*  the License, or (at your option) any later version.             */
/*                                                                  */
/*  This program is distributed in the hope that it will be useful, */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of  */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the   */
/*  GNU General Public License for more details.                    */
/*                                                                  */
/*  You should have received a copy of the GNU General Public       */
/*  License along with this program; if not, write to the           */
/*  Free Software Foundation, Inc., 51 Franklin Street,             */
/*  Fifth Floor, Boston, MA  02110-1301, USA.                       */
/*                                                                  */
/*  Module: Library                                                 */
/*  File: seed7/src/rfllib.c                                        */
/*  Changes: 1991, 1992, 1993, 1994, 1995  Thomas Mertes            */
/*  Content: All primitive actions for the ref_list type.           */
/*                                                                  */
/********************************************************************/

#include "version.h"

#include "stdlib.h"
#include "stdio.h"

#include "common.h"
#include "data.h"
#include "heaputl.h"
#include "flistutl.h"
#include "syvarutl.h"
#include "traceutl.h"
#include "listutl.h"
#include "exec.h"
#include "runerr.h"
#include "memory.h"

#undef EXTERN
#define EXTERN
#include "rfllib.h"



#ifdef ANSI_C

objecttype rfl_append (listtype arguments)
#else

objecttype rfl_append (arguments)
listtype arguments;
#endif

  {
    objecttype rfl_variable;
    listtype rfl_to;
    listtype rfl_from;
    listtype list1_end;
    listtype list2_start;
    errinfotype err_info = OKAY_NO_ERROR;

  /* rfl_append */
    rfl_variable = arg_1(arguments);
    isit_reflist(rfl_variable);
    is_variable(rfl_variable);
    rfl_to = take_reflist(rfl_variable);
    isit_reflist(arg_3(arguments));
    rfl_from = take_reflist(arg_3(arguments));
    if (TEMP_OBJECT(arg_3(arguments))) {
      list2_start = rfl_from;
      arg_3(arguments)->value.listvalue = NULL;
    } else {
      list2_start = copy_list(rfl_from, &err_info);
      if (err_info != OKAY_NO_ERROR) {
        return(raise_exception(SYS_MEM_EXCEPTION));
      } /* if */
    } /* if */
    if (rfl_to != NULL) {
      list1_end = rfl_to;
      while (list1_end->next != NULL) {
        list1_end = list1_end->next;
      } /* while */
      list1_end->next = list2_start;
    } else {
      rfl_variable->value.listvalue = list2_start;
    } /* if */
    return(SYS_EMPTY_OBJECT);
  } /* rfl_append */



#ifdef ANSI_C

objecttype rfl_cat (listtype arguments)
#else

objecttype rfl_cat (arguments)
listtype arguments;
#endif

  {
    objecttype arg1;
    objecttype arg2;
    listtype list1_end;
    listtype list2_start;
    errinfotype err_info = OKAY_NO_ERROR;
    listtype result;

  /* rfl_cat */
    arg1 = arg_1(arguments);
    arg2 = arg_3(arguments);
    isit_reflist(arg1);
    isit_reflist(arg2);
    if (TEMP_OBJECT(arg1)) {
      result = take_reflist(arg1);
    } else {
      result = copy_list(take_reflist(arg1), &err_info);
      if (err_info != OKAY_NO_ERROR) {
        return(raise_exception(SYS_MEM_EXCEPTION));
      } /* if */
    } /* if */
    if (TEMP_OBJECT(arg2)) {
      list2_start = take_reflist(arg2);
      arg2->value.listvalue = NULL;
    } else {
      list2_start = copy_list(take_reflist(arg2), &err_info);
      if (err_info != OKAY_NO_ERROR) {
        return(raise_exception(SYS_MEM_EXCEPTION));
      } /* if */
    } /* if */
    if (TEMP_OBJECT(arg1)) {
      /* Necessary at this place: */
      /* When an exception is raised the arguments must */
      /* be unchanged to give a correct exception warning */
      /* and to give a clean state to the exception handler. */
      arg1->value.listvalue = NULL;
    } /* if */
    if (result != NULL) {
      list1_end = result;
      while (list1_end->next != NULL) {
        list1_end = list1_end->next;
      } /* while */
      list1_end->next = list2_start;
    } else {
      result = list2_start;
    } /* if */
    return(bld_reflist_temp(result));
  } /* rfl_cat */



#ifdef ANSI_C

objecttype rfl_cpy (listtype arguments)
#else

objecttype rfl_cpy (arguments)
listtype arguments;
#endif

  {
    objecttype list_to;
    objecttype list_from;
    listtype help_list;
    errinfotype err_info = OKAY_NO_ERROR;

  /* rfl_cpy */
    list_to = arg_1(arguments);
    list_from = arg_3(arguments);
    isit_reflist(list_to);
    isit_reflist(list_from);
    is_variable(list_to);
    if (list_from != list_to) {
      if (TEMP_OBJECT(list_from)) {
        emptylist(take_reflist(list_to));
        list_to->value.listvalue = take_reflist(list_from);
        list_from->value.listvalue = NULL;
      } else {
        help_list = copy_list(take_reflist(list_from), &err_info);
        if (err_info != OKAY_NO_ERROR) {
          return(raise_exception(SYS_MEM_EXCEPTION));
        } else {
          emptylist(take_reflist(list_to));
          list_to->value.listvalue = help_list;
        } /* if */
      } /* if */
    } /* if */
    return(SYS_EMPTY_OBJECT);
  } /* rfl_cpy */



#ifdef ANSI_C

objecttype rfl_create (listtype arguments)
#else

objecttype rfl_create (arguments)
listtype arguments;
#endif

  {
    objecttype list_to;
    objecttype list_from;
    errinfotype err_info = OKAY_NO_ERROR;

  /* rfl_create */
    list_to = arg_1(arguments);
    list_from = arg_3(arguments);
    SET_CATEGORY_OF_OBJ(list_to, REFLISTOBJECT);
    isit_reflist(list_from);
    if (TEMP_OBJECT(list_from)) {
      list_to->value.listvalue = take_reflist(list_from);
      list_from->value.listvalue = NULL;
    } else {
      list_to->value.listvalue = copy_list(take_reflist(list_from), &err_info);
      if (err_info != OKAY_NO_ERROR) {
        list_to->value.listvalue = NULL;
        return(raise_exception(SYS_MEM_EXCEPTION));
      } /* if */
    } /* if */
    return(SYS_EMPTY_OBJECT);
  } /* rfl_create */



#ifdef ANSI_C

objecttype rfl_destr (listtype arguments)
#else

objecttype rfl_destr (arguments)
listtype arguments;
#endif

  { /* rfl_destr */
    isit_reflist(arg_1(arguments));
    emptylist(take_reflist(arg_1(arguments)));
    arg_1(arguments)->value.listvalue = NULL;
    return(SYS_EMPTY_OBJECT);
  } /* rfl_destr */



#ifdef ANSI_C

objecttype rfl_elem (listtype arguments)
#else

objecttype rfl_elem (arguments)
listtype arguments;
#endif

  {
    objecttype searched_object;
    listtype list_element;

  /* rfl_elem */
    isit_reference(arg_1(arguments));
    isit_reflist(arg_3(arguments));
    searched_object = take_reference(arg_1(arguments));
    list_element = take_reflist(arg_3(arguments));
    while (list_element != NULL && list_element->obj != searched_object) {
      list_element = list_element->next;
    } /* while */
    if (list_element != NULL) {
      return(SYS_TRUE_OBJECT);
    } else {
      return(SYS_FALSE_OBJECT);
    } /* if */
  } /* rfl_elem */



#ifdef ANSI_C

objecttype rfl_elemcpy (listtype arguments)
#else

objecttype rfl_elemcpy (arguments)
listtype arguments;
#endif

  {
    memsizetype position;
    inttype number;
    listtype list_element;

  /* rfl_elemcpy */
    isit_reflist(arg_1(arguments));
    isit_int(arg_4(arguments));
    isit_reference(arg_6(arguments));
    is_variable(arg_1(arguments));
    list_element = take_reflist(arg_1(arguments));
    position = take_int(arg_4(arguments));
    if (position >= 1) {
      number = 1;
      while (number < position && list_element != NULL) {
        number++;
        list_element = list_element->next;
      } /* while */
      if (list_element != NULL) {
        list_element->obj = take_reference(arg_6(arguments));
      } else {
        return(raise_exception(SYS_RNG_EXCEPTION));
      } /* if */
    } else {
      return(raise_exception(SYS_RNG_EXCEPTION));
    } /* if */
    return(SYS_EMPTY_OBJECT);
  } /* rfl_elemcpy */



#ifdef ANSI_C

objecttype rfl_empty (listtype arguments)
#else

objecttype rfl_empty (arguments)
listtype arguments;
#endif

  { /* rfl_empty */
    return(bld_reflist_temp(NULL));
  } /* rfl_empty */



#ifdef ANSI_C

objecttype rfl_eq (listtype arguments)
#else

objecttype rfl_eq (arguments)
listtype arguments;
#endif

  {
    listtype list1;
    listtype list2;
    objecttype result;

  /* rfl_eq */
    isit_reflist(arg_1(arguments));
    isit_reflist(arg_3(arguments));
    list1 = take_reflist(arg_1(arguments));
    list2 = take_reflist(arg_3(arguments));
    while (list1 != NULL && list2 != NULL &&
        list1->obj == list2->obj) {
      list1 = list1->next;
      list2 = list2->next;
    } /* while */
    if (list1 == NULL && list2 == NULL) {
      result = SYS_TRUE_OBJECT;
    } else {
      result = SYS_FALSE_OBJECT;
    } /* if */
    return(result);
  } /* rfl_eq */



#ifdef ANSI_C

objecttype rfl_excl (listtype arguments)
#else

objecttype rfl_excl (arguments)
listtype arguments;
#endif

  { /* rfl_excl */
    isit_reflist(arg_1(arguments));
    isit_reference(arg_2(arguments));
    excl_list(&arg_1(arguments)->value.listvalue,
        take_reference(arg_2(arguments)));
    return(SYS_EMPTY_OBJECT);
  } /* rfl_excl */



#ifdef ANSI_C

objecttype rfl_expr (listtype arguments)
#else

objecttype rfl_expr (arguments)
listtype arguments;
#endif

  {
    listtype list1;
    objecttype arg1_object;
    objecttype operator;
    objecttype arg3_object;
    booltype okay;
    errinfotype err_info = OKAY_NO_ERROR;
    listtype result;

  /* rfl_expr */
    /*
    printf("\n");
    printf("arg_1: ");
    trace1(arg_1(arguments));
    printf("\n");
    */
    isit_list(arg_1(arguments));
    list1 = take_list(arg_1(arguments));
    /*
    printf("list: ");
    prot_list(list1);
    printf("\n");
    */
    result = NULL;
    if (list1 != NULL &&
        list1->next == NULL) {
      arg1_object = arg_1(list1);
      incl_list(&result, arg1_object, &err_info);
    } else {
      okay = TRUE;
      do {
        if (list1 != NULL &&
            list1->next != NULL &&
            list1->next->next != NULL &&
            list1->next->next->next == NULL) {
          arg1_object = arg_1(list1);
          operator = arg_2(list1);
          arg3_object = arg_3(list1);
          incl_list(&result, arg3_object, &err_info);
          if (CATEGORY_OF_OBJ(arg1_object) == EXPROBJECT) {
            list1 = take_list(arg1_object);
          } else {
            incl_list(&result, arg1_object, &err_info);
            okay = FALSE;
          } /* if */
        } else {
          printf("not ok\n");
          printf("list1: %lX\n",                   (long unsigned) list1);
          printf("list1->next: %lX\n",             (long unsigned) list1->next);
          printf("list1->next->next: %lX\n",       (long unsigned) list1->next->next);
          printf("list1->next->next->next: %lX\n", (long unsigned) list1->next->next->next);
          okay = FALSE;
        } /* if */
      } while (okay);
    } /* if */
    /* printf("result: ");
    prot_list(result);
    printf("\n"); */
    return(bld_reflist_temp(result));
  } /* rfl_expr */



#ifdef ANSI_C

objecttype rfl_for (listtype arguments)
#else

objecttype rfl_for (arguments)
listtype arguments;
#endif

  {
    objecttype statement;
    objecttype elementlist;
    objecttype for_variable;
    listtype helplist;
    listtype listelement;
    errinfotype err_info = OKAY_NO_ERROR;
    objecttype result;

  /* rfl_for */
/*  prot_list(take_list(arg_4(arguments))); */
    for_variable = arg_2(arguments);
    elementlist = arg_4(arguments);
    statement = arg_6(arguments);
    isit_reference (for_variable);
    isit_reflist(elementlist);
    helplist = copy_list(take_list(elementlist), &err_info);
    if (err_info != OKAY_NO_ERROR) {
      return(raise_exception(SYS_MEM_EXCEPTION));
    } else {
      listelement = helplist;
      result = SYS_EMPTY_OBJECT;
      while (listelement != (listtype) NULL &&
          result != (objecttype) NULL) {
        for_variable->value.objvalue = listelement->obj;
        result = evaluate(statement);
        listelement = listelement->next;
      } /* while */
      emptylist(helplist);
      return(result);
    } /* if */
  } /* rfl_for */



#ifdef ANSI_C

objecttype rfl_head (listtype arguments)
#else

objecttype rfl_head (arguments)
listtype arguments;
#endif

  {
    objecttype list;
    inttype stop;
    inttype number;
    listtype stop_element;
    listtype saved_list_rest;
    errinfotype err_info = OKAY_NO_ERROR;
    listtype result;

  /* rfl_head */
    isit_int(arg_4(arguments));
    list = arg_1(arguments);
    isit_reflist(list);
    stop = take_int(arg_4(arguments));
    if (stop >= 1) {
      number = 1;
      stop_element = take_reflist(list);
      while (number < stop && stop_element != NULL) {
        number++;
        stop_element = stop_element->next;
      } /* while */
      if (TEMP_OBJECT(list)) {
        if (stop_element != NULL) {
          emptylist(stop_element->next);
          stop_element->next = NULL;
        } /* if */
        result = take_reflist(list);
        list->value.listvalue = NULL;
      } else {
        if (stop_element != NULL) {
          saved_list_rest = stop_element->next;
          stop_element->next = NULL;
          result = copy_list(take_reflist(list), &err_info);
          stop_element->next = saved_list_rest;
        } else {
          result = copy_list(take_reflist(list), &err_info);
        } /* if */
      } /* if */
    } else {
      result = NULL;
    } /* if */
    if (err_info != OKAY_NO_ERROR) {
      return(raise_exception(SYS_MEM_EXCEPTION));
    } else {
      return(bld_reflist_temp(result));
    } /* if */
  } /* rfl_head */



#ifdef ANSI_C

objecttype rfl_idx (listtype arguments)
#else

objecttype rfl_idx (arguments)
listtype arguments;
#endif

  {
    inttype position;
    inttype number;
    listtype list_element;
    objecttype result;

  /* rfl_idx */
    isit_reflist(arg_1(arguments));
    isit_int(arg_3(arguments));
    list_element = take_reflist(arg_1(arguments));
    position = take_int(arg_3(arguments));
    if (position >= 1) {
      number = 1;
      while (number < position && list_element != NULL) {
        number++;
        list_element = list_element->next;
      } /* while */
      if (list_element != NULL) {
        result = bld_reference_temp(list_element->obj);
      } else {
        result = raise_exception(SYS_RNG_EXCEPTION);
      } /* if */
    } else {
      result = raise_exception(SYS_RNG_EXCEPTION);
    } /* if */
    return(result);
  } /* rfl_idx */



#ifdef ANSI_C

objecttype rfl_incl (listtype arguments)
#else

objecttype rfl_incl (arguments)
listtype arguments;
#endif

  {
    errinfotype err_info = OKAY_NO_ERROR;

  /* rfl_incl */
    isit_reflist(arg_1(arguments));
    isit_reference(arg_2(arguments));
    incl_list(&arg_1(arguments)->value.listvalue,
        take_reference(arg_2(arguments)), &err_info);
    if (err_info != OKAY_NO_ERROR) {
      return(raise_exception(SYS_MEM_EXCEPTION));
    } else {
      return(SYS_EMPTY_OBJECT);
    } /* if */
  } /* rfl_incl */



#ifdef ANSI_C

objecttype rfl_lng (listtype arguments)
#else

objecttype rfl_lng (arguments)
listtype arguments;
#endif

  {
    listtype list_element;
    inttype result;

  /* rfl_lng */
    isit_reflist(arg_1(arguments));
    list_element = take_reflist(arg_1(arguments));
    result = 0;
    while (list_element != NULL) {
      list_element = list_element->next;
      result++;
    } /* while */
    return(bld_int_temp(result));
  } /* rfl_lng */



#ifdef ANSI_C

objecttype rfl_mklist (listtype arguments)
#else

objecttype rfl_mklist (arguments)
listtype arguments;
#endif

  {
    listtype result;

  /* rfl_mklist */
    isit_reference(arg_1(arguments));
    if (ALLOC_L_ELEM(result)) {
      result->next = NULL;
      result->obj = take_reference(arg_1(arguments));
      return(bld_reflist_temp(result));
    } else {
      return(raise_exception(SYS_MEM_EXCEPTION));
    } /* if */
  } /* rfl_mklist */



#ifdef ANSI_C

objecttype rfl_ne (listtype arguments)
#else

objecttype rfl_ne (arguments)
listtype arguments;
#endif

  {
    listtype list1;
    listtype list2;
    objecttype result;

  /* rfl_ne */
    isit_reflist(arg_1(arguments));
    isit_reflist(arg_3(arguments));
    list1 = take_reflist(arg_1(arguments));
    list2 = take_reflist(arg_3(arguments));
    while (list1 != NULL && list2 != NULL &&
        list1->obj == list2->obj) {
      list1 = list1->next;
      list2 = list2->next;
    } /* while */
    if (list1 != NULL || list2 != NULL) {
      result = SYS_TRUE_OBJECT;
    } else {
      result = SYS_FALSE_OBJECT;
    } /* if */
    return(result);
  } /* rfl_ne */



#ifdef ANSI_C

objecttype rfl_not_elem (listtype arguments)
#else

objecttype rfl_not_elem (arguments)
listtype arguments;
#endif

  {
    objecttype searched_object;
    listtype list_element;

  /* rfl_not_elem */
    isit_reference(arg_1(arguments));
    isit_reflist(arg_4(arguments));
    searched_object = take_reference(arg_1(arguments));
    list_element = take_reflist(arg_4(arguments));
    while (list_element != NULL && list_element->obj != searched_object) {
      list_element = list_element->next;
    } /* while */
    if (list_element == NULL) {
      return(SYS_TRUE_OBJECT);
    } else {
      return(SYS_FALSE_OBJECT);
    } /* if */
  } /* rfl_not_elem */



#ifdef ANSI_C

objecttype rfl_pos (listtype arguments)
#else

objecttype rfl_pos (arguments)
listtype arguments;
#endif

  {
    listtype list_element;
    objecttype searched_object;
    inttype result;

  /* rfl_pos */
    isit_reflist(arg_1(arguments));
    isit_reference(arg_2(arguments));
    list_element = take_reflist(arg_1(arguments));
    searched_object = take_reference(arg_2(arguments));
    result = 1;
    while (list_element != NULL && list_element->obj != searched_object) {
      list_element = list_element->next;
      result++;
    } /* while */
    if (list_element != NULL) {
      return(bld_int_temp(result));
    } else {
      return(bld_int_temp(0));
    } /* if */
  } /* rfl_pos */



#ifdef ANSI_C

objecttype rfl_range (listtype arguments)
#else

objecttype rfl_range (arguments)
listtype arguments;
#endif

  {
    objecttype list;
    inttype start;
    inttype stop;
    inttype number;
    listtype start_element;
    listtype stop_element;
    listtype *start_address;
    listtype saved_list_rest;
    errinfotype err_info = OKAY_NO_ERROR;
    listtype result;

  /* rfl_range */
    isit_reflist(arg_1(arguments));
    isit_int(arg_3(arguments));
    isit_int(arg_5(arguments));
    list = arg_1(arguments);
    start = take_int(arg_3(arguments));
    stop = take_int(arg_5(arguments));
    number = 1;
    start_address = &take_reflist(list);
    start_element = take_reflist(list);
    while (number < start && start_element != NULL) {
      number++;
      start_address = &start_element->next;
      start_element = start_element->next;
    } /* while */
    if (start_element != NULL && stop >= start) {
      stop_element = start_element;
      while (number < stop && stop_element != NULL) {
        number++;
        stop_element = stop_element->next;
      } /* while */
      if (TEMP_OBJECT(list)) {
        *start_address = stop_element;
        result = start_element;
      } else {
        if (stop_element != NULL) {
          saved_list_rest = stop_element->next;
          stop_element->next = NULL;
          result = copy_list(start_element, &err_info);
          stop_element->next = saved_list_rest;
        } else {
          result = copy_list(start_element, &err_info);
        } /* if */
      } /* if */
    } else {
      result = NULL;
    } /* if */
    if (err_info != OKAY_NO_ERROR) {
      return(raise_exception(SYS_MEM_EXCEPTION));
    } else {
      return(bld_reflist_temp(result));
    } /* if */
  } /* rfl_range */



#ifdef ANSI_C

objecttype rfl_setvalue (listtype arguments)
#else

objecttype rfl_setvalue (arguments)
listtype arguments;
#endif

  {
    objecttype list_to;
    objecttype list_from;
    listtype help_list;
    errinfotype err_info = OKAY_NO_ERROR;

  /* rfl_setvalue */
    isit_reference(arg_1(arguments));
    is_variable(arg_1(arguments));
    list_to = take_reference(arg_1(arguments));
    list_from = arg_2(arguments);
    isit_reflist(list_from);
    if (CATEGORY_OF_OBJ(list_to) == MATCHOBJECT ||
        CATEGORY_OF_OBJ(list_to) == CALLOBJECT ||
        CATEGORY_OF_OBJ(list_to) == REFLISTOBJECT) {
      if (list_from != list_to) {
        if (TEMP_OBJECT(list_from)) {
          emptylist(take_reflist(list_to));
          list_to->value.listvalue = take_reflist(list_from);
          list_from->value.listvalue = NULL;
        } else {
          help_list = copy_list(take_reflist(list_from), &err_info);
          if (err_info != OKAY_NO_ERROR) {
            return(raise_exception(SYS_MEM_EXCEPTION));
          } else {
            emptylist(take_reflist(list_to));
            list_to->value.listvalue = help_list;
          } /* if */
        } /* if */
      } /* if */
    } else {
      run_error(REFLISTOBJECT, list_to);
    } /* if */
    return(SYS_EMPTY_OBJECT);
  } /* rfl_setvalue */



#ifdef ANSI_C

objecttype rfl_tail (listtype arguments)
#else

objecttype rfl_tail (arguments)
listtype arguments;
#endif

  {
    objecttype list;
    inttype start;
    inttype number;
    listtype list_element;
    errinfotype err_info = OKAY_NO_ERROR;
    listtype result;

  /* rfl_tail */
    isit_reflist(arg_1(arguments));
    isit_int(arg_3(arguments));
    list = arg_1(arguments);
    start = take_int(arg_3(arguments));
    list_element = take_reflist(list);
    if (start > 1 && list_element != NULL) {
      number = 2;
      while (number < start && list_element->next != NULL) {
        number++;
        list_element = list_element->next;
      } /* while */
      if (number >= start) {
        if (TEMP_OBJECT(list)) {
          result = list_element->next;
          list_element->next = NULL;
        } else {
          result = copy_list(list_element->next, &err_info);
        } /* if */
      } else {
        result = NULL;
      } /* if */
    } else {
      if (TEMP_OBJECT(list)) {
        result = list_element;
        list->value.listvalue = NULL;
      } else {
        result = copy_list(list_element, &err_info);
      } /* if */
    } /* if */
    if (err_info != OKAY_NO_ERROR) {
      return(raise_exception(SYS_MEM_EXCEPTION));
    } else {
      return(bld_reflist_temp(result));
    } /* if */
  } /* rfl_tail */



#ifdef ANSI_C

objecttype rfl_trace (listtype arguments)
#else

objecttype rfl_trace (arguments)
listtype arguments;
#endif

  { /* rfl_trace */
    isit_reflist(arg_1(arguments));
    prot_list(take_reflist(arg_1(arguments)));
    return(SYS_EMPTY_OBJECT);
  } /* rfl_trace */



#ifdef ANSI_C

objecttype rfl_value (listtype arguments)
#else

objecttype rfl_value (arguments)
listtype arguments;
#endif

  {
    objecttype obj_arg;
    errinfotype err_info = OKAY_NO_ERROR;
    listtype result;

  /* rfl_value */
    isit_reference(arg_1(arguments));
    obj_arg = take_reference(arg_1(arguments));
    if (obj_arg != NULL) {
      if (CATEGORY_OF_OBJ(obj_arg) == MATCHOBJECT ||
          CATEGORY_OF_OBJ(obj_arg) == CALLOBJECT ||
          CATEGORY_OF_OBJ(obj_arg) == REFLISTOBJECT) {
        result = copy_list(take_reflist(obj_arg), &err_info);
      } else {
        run_error(REFLISTOBJECT, obj_arg);
        result = NULL;
      } /* if */
    } else {
      return(raise_exception(SYS_RNG_EXCEPTION));
    } /* if */
    if (err_info != OKAY_NO_ERROR) {
      return(raise_exception(SYS_MEM_EXCEPTION));
    } /* if */
    return(bld_reflist_temp(result));
  } /* rfl_value */
