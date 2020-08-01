/********************************************************************/
/*                                                                  */
/*  chkccomp   Check properties of C compiler and runtime.          */
/*  Copyright (C) 2010, 2011  Thomas Mertes                         */
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
/*  Module: Chkccomp                                                */
/*  File: seed7/src/chkccomp.c                                      */
/*  Changes: 2010, 2011  Thomas Mertes                              */
/*  Content: Program to Check properties of C compiler and runtime. */
/*                                                                  */
/********************************************************************/

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "time.h"
#include "float.h"
#include "math.h"

#include "chkccomp.h"

/**
 *  The file chkccomp.h is NOT part of the Seed7 release.
 *  Instead chkccomp.h is generated by the makefile and
 *  removed after chkccomp was compiled and executed.
 *  In chkccomp.h the following macros might be defined:
 *
 *  popen
 *      Name of Posix function popen.
 *      E.g.: #define popen _popen
 *  mkdir(path,mode)
 *      Macro to replace the Posix function mkdir.
 *      E.g.: #define mkdir(path,mode) mkdir(path)
 *            #define mkdir(path,mode) _mkdir(path)
 *  rmdir
 *      Name of Posix function rmdir.
 *      E.g.: #define rmdir _rmdir
 *  long_long_EXISTS
 *      Defined when the type long long exists.
 *      E.g.: #define long_long_EXISTS
 *  long_long_SUFFIX_LL
 *      Defined when long long literals are suffixed with ll.
 *      E.g.: #define long_long_SUFFIX_LL
 *  __int64_EXISTS
 *      Defined when the type__int64 exists.
 *      E.g.: #define __int64_EXISTS
 *  __int64_SUFFIX_LL
 *      Defined when long long literals are suffixed with ll.
 *      E.g.: #define __int64_SUFFIX_LL
 *  WRITE_CC_VERSION_INFO
 *      Write the version of the C compiler to the file "cc_vers.txt".
 *      E.g.: #define WRITE_CC_VERSION_INFO system("$(GET_CC_VERSION_INFO) cc_vers.txt");
 *  LIST_DIRECTORY_CONTENTS
 *      Either "ls" or "dir".
 *      E.g.: #define LIST_DIRECTORY_CONTENTS "ls"
 *            #define LIST_DIRECTORY_CONTENTS "dir"
 *  TURN_OFF_FP_EXCEPTIONS
 *      Use the function _control87() to turn off floating point exceptions.
 *  DEFINE_MATHERR_FUNCTION
 *      Define the function _matherr() which handles floating point errors.
 *
 *  The macros described above are only used in the program chkccomp.
 *  This macros are not used in the Seed7 Interpreter (hi) or in the
 *  Seed7 Runtime Library.
 */



#ifdef DEFINE_MATHERR_FUNCTION
int _matherr (struct _exception *a)

  { /* _matherr */
    a->retval = a->retval;
    return 1;
  } /* _matherr */
#endif



/**
 *  Program to Check properties of C compiler and runtime.
 */
int main (int argc, char **argv)

  {
    FILE *aFile;
    time_t timestamp;
    struct tm *local_time;
    char buffer[4096];
    long number;
    int ch;
    float zero = 0.0;
    float negativeZero;
    float minusZero;
    float nanValue1;
    float nanValue2;
    float plusInf;
    float minusInf;

  /* main */
#ifdef WRITE_CC_VERSION_INFO
    WRITE_CC_VERSION_INFO
#endif
    aFile = fopen("cc_vers.txt","r");
    printf("#define C_COMPILER_VERSION \"");
    for (ch=getc(aFile); ch!=EOF && ch!=10 && ch!=13; ch=getc(aFile)) {
      if (ch>=' ' && ch<='~') {
        if (ch==34 || ch==39 || ch==92) {
          putchar(92);
        } /* if */
        putchar(ch);
      } else {
        putchar(92);
        printf("%3o", ch);
      } /* if */
    } /* for */
    puts("\"");
    fclose(aFile);
    aFile = popen(LIST_DIRECTORY_CONTENTS,"r");
    if (ftell(aFile) != -1) {
      puts("#define FTELL_WRONG_FOR_PIPE");
    } /* if */
    if ((aFile = fopen("tmp_test_file","w")) != NULL) {
      fwrite("asdf",1,4,aFile);
      fclose(aFile);
      if ((aFile = fopen("tmp_test_file","r")) != NULL) {
        if (fwrite("qwert",1,5,aFile) != 0) {
          puts("#define FWRITE_WRONG_FOR_READ_ONLY_FILES");
        } /* if */
        fclose(aFile);
      } /* if */
      remove("tmp_test_file");
    } /* if */
    mkdir("tmp_empty_dir",0x755);
    if (remove("tmp_empty_dir") != 0) {
      puts("#define REMOVE_FAILS_FOR_EMPTY_DIRS");
      rmdir("tmp_empty_dir");
    } /* if */
    aFile = fopen(".","r");
    if (aFile != NULL) {
      puts("#define FOPEN_OPENS_DIRECTORIES");
      fclose(aFile);
    } /* if */
    printf("#define POINTER_SIZE %lu", (long unsigned)(8 * sizeof(char *)));
    puts("");
    printf("#define FLOAT_SIZE %lu", (long unsigned)(8 * sizeof(float)));
    puts("");
    printf("#define DOUBLE_SIZE %lu", (long unsigned)(8 * sizeof(double)));
    puts("");
    printf("#define TIME_T_SIZE %lu", (long unsigned)(8 * sizeof(time_t)));
    puts("");
    timestamp = -2147483648;
    local_time = localtime(&timestamp);
    if (local_time != NULL && local_time->tm_year == 1) {
      puts("#define TIME_T_SIGNED");
    } /* if */
    if (sizeof(int) == 4) {
      puts("#define INT32TYPE int");
      puts("#define INT32TYPE_STRI \"int\"");
      puts("#define UINT32TYPE unsigned int");
      puts("#define UINT32TYPE_STRI \"unsigned int\"");
    } else if (sizeof(long) == 4) {
      puts("#define INT32TYPE long");
      puts("#define INT32TYPE_STRI \"long\"");
      puts("#define UINT32TYPE unsigned long");
      puts("#define UINT32TYPE_STRI \"unsigned long\"");
      puts("#define INT32TYPE_SUFFIX_L");
      puts("#define INT32TYPE_FORMAT_L");
    } /* if */
    if (sizeof(long) == 8) {
      puts("#define INT64TYPE long");
      puts("#define INT64TYPE_STRI \"long\"");
      puts("#define UINT64TYPE unsigned long");
      puts("#define UINT64TYPE_STRI \"unsigned long\"");
      puts("#define INT64TYPE_SUFFIX_L");
      puts("#define INT64TYPE_FORMAT_L");
#ifdef long_long_EXISTS
    } else if (sizeof(long long) == 8) {
      puts("#define INT64TYPE long long");
      puts("#define INT64TYPE_STRI \"long long\"");
      puts("#define UINT64TYPE unsigned long long");
      puts("#define UINT64TYPE_STRI \"unsigned long long\"");
#ifdef long_long_SUFFIX_LL
      puts("#define INT64TYPE_SUFFIX_LL");
#endif
      sprintf(buffer, "A%lldB", (long long) 1 << 32);
      if (strcmp(buffer, "A4294967296B") == 0) {
        puts("#define INT64TYPE_FORMAT_LL");
      } else {
        sprintf(buffer, "A%LdB", (long long) 1 << 32);
        if (strcmp(buffer, "A4294967296B") == 0) {
          puts("#define INT64TYPE_FORMAT_CAPITAL_L");
        } else {
          sprintf(buffer, "A%I64dB", (long long) 1 << 32);
          if (strcmp(buffer, "A4294967296B") == 0) {
            puts("#define INT64TYPE_FORMAT_I64");
          } /* if */
        } /* if */
      } /* if */
#endif
#ifdef __int64_EXISTS
    } else if (sizeof(__int64) == 8) {
      puts("#define INT64TYPE __int64");
      puts("#define INT64TYPE_STRI \"__int64\"");
      puts("#define UINT64TYPE unsigned __int64");
      puts("#define UINT64TYPE_STRI \"unsigned __int64\"");
#ifdef __int64_SUFFIX_LL
      puts("#define INT64TYPE_SUFFIX_LL");
#endif
      sprintf(buffer, "A%lldB", (__int64) 1 << 32);
      if (strcmp(buffer, "A4294967296B") == 0) {
        puts("#define INT64TYPE_FORMAT_LL");
      } else {
        sprintf(buffer, "A%LdB", (__int64) 1 << 32);
        if (strcmp(buffer, "A4294967296B") == 0) {
          puts("#define INT64TYPE_FORMAT_CAPITAL_L");
        } else {
          sprintf(buffer, "A%I64dB", (__int64) 1 << 32);
          if (strcmp(buffer, "A4294967296B") == 0) {
            puts("#define INT64TYPE_FORMAT_I64");
          } /* if */
        } /* if */
      } /* if */
#endif
    } /* if */
    number = -1;
    if (number >> 1 == (long) -1) {
      puts("#define RSHIFT_DOES_SIGN_EXTEND");
    } /* if */
    if (~number == (long) 0) {
      puts("#define TWOS_COMPLEMENT_INTTYPE");
    } /* if */
    number = 1;
    if (((char *) &number)[0] == 1) {
      puts("#define LITTLE_ENDIAN_INTTYPE");
    } /* if */
#ifdef TURN_OFF_FP_EXCEPTIONS
    _control87(MCW_EM, MCW_EM);
#endif
    sprintf(buffer, "%1.0f %1.0f %1.0f %1.1f %1.1f %1.2f %1.2f %1.0f %1.0f %1.0f %1.1f %1.1f %1.2f %1.2f",
            0.5, 1.5, 2.5, 1.25, 1.75, 0.125, 0.375, -0,5, -1.5, -2.5, -1.25, -1.75, -0.125, -0.375);
    if (strcmp(buffer, "0 2 2 1.2 1.8 0.12 0.38 0 -2 -2 -1.2 -1.8 -0.12 -0.38") == 0) {
      puts("#define ROUND_HALF_TO_EVEN");
    } else if (strcmp(buffer, "1 2 3 1.3 1.8 0.13 0.38 -1 -2 -3 -1.3 -1.8 -0.13 -0.38") == 0) {
      puts("#define ROUND_HALF_AWAY_FROM_ZERO");
    } else if (strcmp(buffer, "1 2 3 1.3 1.8 0.13 0.38 0 -1 -2 -1.2 -1.7 -0.12 -0.37") == 0) {
      puts("#define ROUND_HALF_UP");
    } /* if */
    nanValue1 = 0.0 / zero;
    nanValue2 = 0.0 / zero;
    if (nanValue1 == nanValue2 ||
        nanValue1 <  nanValue2 || nanValue1 >  nanValue2 ||
        nanValue1 <= nanValue2 || nanValue1 <= nanValue2) {
      puts("#define NAN_COMPARISON_WRONG");
    } /* if */
    plusInf = 1.0 / zero;
    minusInf = -plusInf;
    negativeZero = -1.0 / plusInf;
    minusZero = -zero;
    if (memcmp(&negativeZero, &minusZero, sizeof(float)) != 0) {
      puts("#define USE_NEGATIVE_ZERO_BITPATTERN");
    } /* if */
    if (pow(zero, -2.0) != plusInf || pow(negativeZero, -1.0) != minusInf) {
      puts("#define POWER_OF_ZERO_WRONG");
    } /* if */
    return 0;
  } /* main */
