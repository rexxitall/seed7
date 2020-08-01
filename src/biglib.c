/********************************************************************/
/*                                                                  */
/*  hi   Interpreter for Seed7 programs.                            */
/*  Copyright (C) 1990 - 2006  Thomas Mertes                        */
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
/*  License along with this program; if not, write to the Free      */
/*  Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,  */
/*  MA 02111-1307 USA                                               */
/*                                                                  */
/*  Module: Library                                                 */
/*  File: seed7/src/biglib.c                                        */
/*  Changes: 2005, 2006  Thomas Mertes                              */
/*  Content: All primitive actions for the bigInteger type.         */
/*                                                                  */
/********************************************************************/

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "version.h"
#include "common.h"
#include "data.h"
#include "heaputl.h"
#include "syvarutl.h"
#include "runerr.h"
#include "memory.h"
#include "big_rtl.h"

#undef EXTERN
#define EXTERN
#include "biglib.h"



#ifdef ANSI_C

objecttype big_add (listtype arguments)
#else

objecttype big_add (arguments)
listtype arguments;
#endif

  { /* big_add */
    isit_bigint(arg_1(arguments));
    isit_bigint(arg_3(arguments));
    return(bld_bigint_temp(
        bigAdd(take_bigint(arg_1(arguments)), take_bigint(arg_3(arguments)))));
  } /* big_add */



#ifdef ANSI_C

objecttype big_cmp (listtype arguments)
#else

objecttype big_cmp (arguments)
listtype arguments;
#endif

  { /* big_cmp */
    isit_bigint(arg_1(arguments));
    isit_bigint(arg_2(arguments));
    return(bld_int_temp(
        bigCmp(take_bigint(arg_1(arguments)), take_bigint(arg_2(arguments)))));
  } /* big_cmp */



#ifdef ANSI_C

objecttype big_cpy (listtype arguments)
#else

objecttype big_cpy (arguments)
listtype arguments;
#endif

  {
    objecttype big_to;
    objecttype big_from;
    memsizetype new_size;
    biginttype new_big;

  /* big_cpy */
    big_to = arg_1(arguments);
    big_from = arg_3(arguments);
    isit_bigint(big_to);
    isit_bigint(big_from);
    is_variable(big_to);
    new_big = take_bigint(big_to);
    if (TEMP_OBJECT(big_from)) {
      FREE_BIG(new_big, new_big->size);
      big_to->value.bigintvalue = take_bigint(big_from);
      big_from->value.bigintvalue = NULL;
    } else {
      new_size = take_bigint(big_from)->size;
      if (new_big->size != new_size) {
        if (!ALLOC_BIG(new_big, new_size)) {
          return(raise_exception(SYS_MEM_EXCEPTION));
        } else {
          COUNT_BIG(new_size);
          FREE_BIG(take_bigint(big_to), take_bigint(big_to)->size);
          big_to->value.bigintvalue = new_big;
          new_big->size = new_size;
        } /* if */
      } /* if */
      memcpy(new_big->bigdigits, take_bigint(big_from)->bigdigits,
          (SIZE_TYPE) new_size * sizeof(bigdigittype));
    } /* if */
    return(SYS_EMPTY_OBJECT);
  } /* big_cpy */



#ifdef ANSI_C

objecttype big_decr (listtype arguments)
#else

objecttype big_decr (arguments)
listtype arguments;
#endif

  {
    objecttype big_variable;
    int carry = 1;

  /* big_decr */
#ifdef OUT_OF_ORDER
    big_variable = arg_1(arguments);
    isit_bigint(big_variable);
    is_variable(big_variable);
    big1 = take_bigint(big_variable);
    for (pos = 0; pos < big1->size; pos++) {
      if (carry) {
        if (big1->bigdigits[pos] == 0) {
          big1->bigdigits[pos] = BIGDIGIT_MAX;
        } else {
          big1->bigdigits[pos]--;
          carry = 0;
        } /* if */
      } /* if */
    } /* for */
    if (carry) {
      if (big1->sign == 0) {
        big1->sign = BIGDIGIT_MAX;
      } else {
        if (!RESIZE_BIG(big1, big1->size, big1->size + 1)) {
          return(raise_exception(SYS_MEM_EXCEPTION));
        } else {
          big1->bigdigits[pos] = BIGDIGIT_MAX - 1;
          big1->size++;
          big_variable->value.bigvalue = big1;
        } /* if */
      } /* if */
    } /* if */
#endif
    return(SYS_EMPTY_OBJECT);
  } /* big_decr */



#ifdef ANSI_C

objecttype big_eq (listtype arguments)
#else

objecttype big_eq (arguments)
listtype arguments;
#endif

  {
    biginttype big1;
    biginttype big2;

  /* big_eq */
    isit_bigint(arg_1(arguments));
    isit_bigint(arg_3(arguments));
    big1 = take_bigint(arg_1(arguments));
    big2 = take_bigint(arg_3(arguments));
    if (big1->size == big2->size &&
		memcmp(big1->bigdigits, big2->bigdigits,
		(SIZE_TYPE) big1->size * sizeof(bigdigittype)) == 0) {
      return(SYS_TRUE_OBJECT);
    } else {
      return(SYS_FALSE_OBJECT);
    } /* if */
  } /* big_eq */



#ifdef ANSI_C

objecttype big_incr (listtype arguments)
#else

objecttype big_incr (arguments)
listtype arguments;
#endif

  {
    objecttype big_variable;
    int carry = 1;

  /* big_incr */
#ifdef OUT_OF_ORDER
    big_variable = arg_1(arguments);
    isit_bigint(big_variable);
    is_variable(big_variable);
    big1 = take_bigint(big_variable);
    for (pos = 0; pos < big1->size; pos++) {
      if (carry) {
        if (big1->bigdigits[pos] == BIGDIGIT_MAX) {
          big1->bigdigits[pos] = 0;
        } else {
          big1->bigdigits[pos]++;
          carry = 0;
        } /* if */
      } /* if */
    } /* for */
    if (carry) {
      if (big1->sign != 0) {
        big1->sign = 0;
      } else {
        if (!RESIZE_BIG(big1, big1->size, big1->size + 1)) {
          return(raise_exception(SYS_MEM_EXCEPTION));
        } else {
          big1->bigdigits[pos] = 1;
          big1->size++;
          big_variable->value.bigvalue = big1;
        } /* if */
      } /* if */
    } /* if */
#endif
    return(SYS_EMPTY_OBJECT);
  } /* big_incr */



#ifdef ANSI_C

objecttype big_minus (listtype arguments)
#else

objecttype big_minus (arguments)
listtype arguments;
#endif

  { /* big_minus */
    isit_bigint(arg_2(arguments));
    return(bld_bigint_temp(
        bigMinus(take_bigint(arg_2(arguments)))));
  } /* big_minus */



#ifdef ANSI_C

objecttype big_mult (listtype arguments)
#else

objecttype big_mult (arguments)
listtype arguments;
#endif

  { /* big_mult */
    isit_bigint(arg_1(arguments));
    isit_bigint(arg_3(arguments));
    return(bld_bigint_temp(
        bigMult(take_bigint(arg_1(arguments)), take_bigint(arg_3(arguments)))));
  } /* big_mult */



#ifdef ANSI_C

objecttype big_ne (listtype arguments)
#else

objecttype big_ne (arguments)
listtype arguments;
#endif

  {
    biginttype big1;
    biginttype big2;

  /* big_ne */
    isit_bigint(arg_1(arguments));
    isit_bigint(arg_3(arguments));
    big1 = take_bigint(arg_1(arguments));
    big2 = take_bigint(arg_3(arguments));
    if (big1->size != big2->size ||
		memcmp(big1->bigdigits, big2->bigdigits,
		(SIZE_TYPE) big1->size * sizeof(bigdigittype)) != 0) {
      return(SYS_TRUE_OBJECT);
    } else {
      return(SYS_FALSE_OBJECT);
    } /* if */
  } /* big_ne */



#ifdef ANSI_C

objecttype big_parse (listtype arguments)
#else

objecttype big_parse (arguments)
listtype arguments;
#endif

  { /* big_parse */
    isit_stri(arg_3(arguments));
    return(bld_bigint_temp(bigParse(
        take_stri(arg_3(arguments)))));
  } /* big_parse */



#ifdef ANSI_C

objecttype big_sbtr (listtype arguments)
#else

objecttype big_sbtr (arguments)
listtype arguments;
#endif

  { /* big_sbtr */
    isit_bigint(arg_1(arguments));
    isit_bigint(arg_3(arguments));
    return(bld_bigint_temp(
        bigSbtr(take_bigint(arg_1(arguments)), take_bigint(arg_3(arguments)))));
  } /* big_sbtr */