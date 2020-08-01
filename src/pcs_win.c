/********************************************************************/
/*                                                                  */
/*  pcs_win.c     Process functions which use the Windows API.      */
/*  Copyright (C) 1989 - 2014  Thomas Mertes                        */
/*                                                                  */
/*  This file is part of the Seed7 Runtime Library.                 */
/*                                                                  */
/*  The Seed7 Runtime Library is free software; you can             */
/*  redistribute it and/or modify it under the terms of the GNU     */
/*  Lesser General Public License as published by the Free Software */
/*  Foundation; either version 2.1 of the License, or (at your      */
/*  option) any later version.                                      */
/*                                                                  */
/*  The Seed7 Runtime Library is distributed in the hope that it    */
/*  will be useful, but WITHOUT ANY WARRANTY; without even the      */
/*  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR */
/*  PURPOSE.  See the GNU Lesser General Public License for more    */
/*  details.                                                        */
/*                                                                  */
/*  You should have received a copy of the GNU Lesser General       */
/*  Public License along with this program; if not, write to the    */
/*  Free Software Foundation, Inc., 51 Franklin Street,             */
/*  Fifth Floor, Boston, MA  02110-1301, USA.                       */
/*                                                                  */
/*  Module: Seed7 Runtime Library                                   */
/*  File: seed7/src/pcs_win.c                                       */
/*  Changes: 2010, 2012 - 2014  Thomas Mertes                       */
/*  Content: Process functions which use the Windows API.           */
/*                                                                  */
/********************************************************************/

#include "version.h"

#include "stdio.h"
#include "windows.h"
#include "io.h"
#include "fcntl.h"

#include "common.h"
#include "data_rtl.h"
#include "heaputl.h"
#include "striutl.h"
#include "int_rtl.h"
#include "rtl_err.h"

#undef TRACE_PCS_WIN
#undef VERBOSE_EXCEPTIONS


#define MAXIMUM_COMMAND_LINE_LENGTH 32768

typedef struct {
    uintType usage_count;
    /* Up to here the structure is identical to struct processStruct */
    HANDLE hProcess;
    HANDLE hThread;
    DWORD pid;
    boolType isTerminated;
    DWORD exitValue;
    fileType stdIn;
    fileType stdOut;
    fileType stdErr;
  } win_processRecord, *win_processType;

typedef const win_processRecord *const_win_processType;

#ifdef DO_HEAP_STATISTIC
size_t sizeof_processRecord = sizeof(win_processRecord);
#endif

#define to_hProcess(process)     (((const_win_processType) process)->hProcess)
#define to_hThread(process)      (((const_win_processType) process)->hThread)
#define to_pid(process)          (((const_win_processType) process)->pid)
#define to_isTerminated(process) (((const_win_processType) process)->isTerminated)
#define to_exitValue(process)    (((const_win_processType) process)->exitValue)

#define to_var_hProcess(process)     (((win_processType) process)->hProcess)
#define to_var_hThread(process)      (((win_processType) process)->hThread)
#define to_var_pid(process)          (((win_processType) process)->pid)
#define to_var_isTerminated(process) (((win_processType) process)->isTerminated)
#define to_var_exitValue(process)    (((win_processType) process)->exitValue)

#if POINTER_SIZE == 32
typedef int32Type intPtrType;
#elif POINTER_SIZE == 64
typedef int64Type intPtrType;
#endif

#ifdef VERBOSE_EXCEPTIONS
#define logError(logStatements) logStatements
#else
#define logError(logStatements)
#endif



/**
 *  Create a command line string that can be used by CreateProcessW().
 *  The command line string must be freed with os_stri_free().
 */
static os_striType prepareCommandLine (const const_os_striType os_command_stri,
    const const_rtlArrayType parameters, errInfoType *err_info)

  {
    memSizeType arraySize;
    memSizeType striSize;
    memSizeType pos;
    os_striType argument;
    os_charType *sourceChar;
    os_charType *destChar;
    os_charType *beyondDest;
    memSizeType countBackslash;
    os_striType command_line;

  /* prepareCommandLine */
    arraySize = arraySize(parameters);
    if (unlikely(!os_stri_alloc(command_line, MAXIMUM_COMMAND_LINE_LENGTH - 1))) {
      *err_info = MEMORY_ERROR;
    } else {
      beyondDest = &command_line[MAXIMUM_COMMAND_LINE_LENGTH];
      /* fprintf(stderr, "\ncommand_stri=\"%ls\"\n", os_command_stri); */
      striSize = os_stri_strlen(os_command_stri);
      if (striSize > MAXIMUM_COMMAND_LINE_LENGTH - 2 ||
          &command_line[striSize] > beyondDest) {
        *err_info = MEMORY_ERROR;
        destChar = beyondDest;
      } else {
        command_line[0] = '\"';
        memcpy(&command_line[1], os_command_stri, sizeof(os_charType) * striSize);
        command_line[striSize + 1] = '\"';
        destChar = &command_line[striSize + 2];
      } /* if */
      for (pos = 0; pos < arraySize && *err_info == OKAY_NO_ERROR; pos++) {
        argument = stri_to_os_stri(parameters->arr[pos].value.striValue, err_info);
        if (argument != NULL) {
          /* fprintf(stderr, "argument[%d]=%ls\n", pos + 1, argument); */
          if (&destChar[2] > beyondDest) {
            destChar = beyondDest;
          } else {
            *(destChar++) = ' ';
            *(destChar++) = '"';
          } /* if */
          for (sourceChar = argument; *sourceChar != '\0' && destChar < beyondDest;
              sourceChar++, destChar++) {
            if (*sourceChar == '"') {
              if (&destChar[2] > beyondDest) {
                destChar = beyondDest;
              } else {
                *(destChar++) = '\\';
                *destChar = *sourceChar;
              } /* if */
            } else if (*sourceChar == '\\') {
              sourceChar++;
              countBackslash = 1;
              while (*sourceChar == '\\') {
                sourceChar++;
                countBackslash++;
              } /* while */
              /* fprintf(stderr, "countBackslash=%lu\n", countBackslash);
                 fprintf(stderr, "sourceChar=%c\n", *sourceChar); */
              if (*sourceChar == '"' || *sourceChar == '\0') {
                countBackslash *= 2;
              } /* if */
              sourceChar--;
              if (countBackslash > MAXIMUM_COMMAND_LINE_LENGTH ||
                  &destChar[countBackslash] > beyondDest) {
                destChar = beyondDest;
              } else {
                do {
                  *(destChar++) = '\\';
                  countBackslash--;
                } while (countBackslash != 0);
                destChar--;
              } /* if */
            } else {
              *destChar = *sourceChar;
            } /* if */
          } /* for */
          if (destChar >= beyondDest) {
            *err_info = MEMORY_ERROR;
          } else {
            *(destChar++) = '"';
          } /* if */
          os_stri_free(argument);
        } /* if */
      } /* for */
      if (destChar >= beyondDest) {
        *err_info = MEMORY_ERROR;
      } else {
        *destChar = '\0';
      } /* if */
      if (*err_info != OKAY_NO_ERROR) {
        os_stri_free(command_line);
        command_line = NULL;
      } else {
        /* fprintf(stderr, "command_line=%ls\n", command_line); */
      } /* if */
    } /* if */
    return command_line;
  } /* prepareCommandLine */



/**
 *  Compare two processes.
 *  @return -1, 0 or 1 if the first argument is considered to be
 *          respectively less than, equal to, or greater than the
 *          second.
 */
intType pcsCmp (const const_processType process1, const const_processType process2)

  {
    intType signumValue;

  /* pcsCmp */
    if (process1 == NULL) {
      if (process2 != NULL) {
        signumValue = -1;
      } else {
        signumValue = 0;
      } /* if */
    } else if (process2 == NULL) {
      signumValue = 1;
    } else if (to_pid(process1) < to_pid(process2)) {
      signumValue = -1;
    } else if (to_pid(process1) == to_pid(process2)) {
      signumValue = 0;
    } else {
      signumValue = 1;
    } /* if */
    return signumValue;
  } /* pcsCmp */



/**
 *  Check if two processes are equal.
 *  @return TRUE if both processes are equal,
 *          FALSE otherwise.
 */
boolType pcsEq (const const_processType process1, const const_processType process2)

  { /* pcsEq */
    if (process1 == NULL) {
      return process2 == NULL;
    } else if (process2 == NULL) {
      return FALSE;
    } else {
      return to_pid(process1) == to_pid(process2);
    } /* if */
  } /* pcsEq */



/**
 *  Return the exit value of the specified process.
 *  By convention, the value 0 indicates normal termination.
 *  @return the exit value of the specified process.
 */
intType pcsExitValue (const const_processType process)

  {
    intType exitValue;

  /* pcsExitValue */
    if (to_isTerminated(process)) {
      exitValue = (intType) to_exitValue(process);
    } else {
      raise_error(FILE_ERROR);
      exitValue = -1;
    } /* if */
    return exitValue;
  } /* pcsExitValue */



void pcsFree (processType old_process)

  { /* pcsFree */
#ifdef TRACE_PCS_WIN
    printf("pcsFree(" FMT_U_MEM ") (usage=" FMT_U ")\n",
           (memSizeType) old_process,
           old_process != NULL ? old_process->usage_count : (uintType) 0);
    printf("PID=%lu\n", (long unsigned) to_pid(old_process));
#endif
    CloseHandle(to_hProcess(old_process));
    CloseHandle(to_hThread(old_process));
    FREE_RECORD(old_process, win_processRecord, count.process);
  } /* pcsFree */



/**
 *  Compute the hash value of a process.
 *  @return the hash value.
 */
intType pcsHashCode (const const_processType process)

  {
    intType hashCode;

  /* pcsHashCode */
    if (process == NULL) {
      hashCode = 0;
    } else {
      hashCode = to_pid(process);
    } /* if */
    return hashCode;
  } /* pcsHashCode */



/**
 *  Test whether the specified process is alive.
 *  @return TRUE if the specified process has not yet terminated,
 *          FALSE otherwise.
 */
boolType pcsIsAlive (const processType process)

  {
    DWORD exitCode = 0;
    boolType isAlive;

  /* pcsIsAlive */
    if (to_isTerminated(process)) {
      isAlive = FALSE;
    } else {
      if (GetExitCodeProcess(to_hProcess(process), &exitCode) != 0) {
        if (exitCode == STILL_ACTIVE) {
          if (WaitForSingleObject(to_hProcess(process), 0) == WAIT_OBJECT_0) {
            to_var_isTerminated(process) = TRUE;
          } /* if */
        } else {
          to_var_isTerminated(process) = TRUE;
        } /* if */
        isAlive = !to_isTerminated(process);
        if (!isAlive) {
          to_var_exitValue(process) = exitCode;
        } /* if */
      } else {
        logError(printf(" *** pcsIsAlive: GetExitCodeProcess(" FMT_U_MEM ", 0) failed:\nGetLastError=%d\n",
                        (memSizeType) to_hProcess(process), GetLastError());
                 printf("PID=%lu\n", (long unsigned) to_pid(process)););
        raise_error(FILE_ERROR);
        isAlive = TRUE;
      } /* if */
    } /* if */
    return isAlive;
  } /* pcsIsAlive */



/**
 *  Kill the specified process.
 *  @exception FILE_ERROR It was not possible to kill the process.
 */
void pcsKill (const processType process)

  { /* pcsKill */
#ifdef TRACE_PCS_WIN
    printf("pcsKill(" FMT_U_MEM ") (hProcess=" FMT_U_MEM ")\n",
           (memSizeType) process,
           process != NULL ? (memSizeType) to_hProcess(process) : (memSizeType) 0);
#endif
    if (process == NULL) {
      logError(printf(" *** pcsKill: process == NULL\n"););
      raise_error(FILE_ERROR);
    } else if (TerminateProcess(to_hProcess(process), 0) == 0) {
      logError(printf(" *** pcsKill: TerminateProcess(" FMT_U_MEM ", 0) failed:\nGetLastError=%d\n",
                      (memSizeType) to_hProcess(process), GetLastError());
               printf("PID=%lu\n", (long unsigned) to_pid(process)););
      raise_error(FILE_ERROR);
    } /* if */
  } /* pcsKill */



void pcsPipe2 (const const_striType command, const const_rtlArrayType parameters,
    fileType *childStdin, fileType *childStdout)

  {
    os_striType os_command_stri;
    os_striType command_line;
    SECURITY_ATTRIBUTES saAttr;
    HANDLE childInputRead;
    HANDLE childInputWrite;
    HANDLE childInputWriteTemp;
    HANDLE childOutputReadTemp;
    HANDLE childOutputRead;
    HANDLE childOutputWrite;
    HANDLE childErrorWrite;
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInformation;
    int path_info = PATH_IS_NORMAL;
    errInfoType err_info = OKAY_NO_ERROR;

  /* pcsPipe2 */
    os_command_stri = cp_to_os_path(command, &path_info, &err_info);
    if (likely(err_info == OKAY_NO_ERROR)) {
      command_line = prepareCommandLine(os_command_stri, parameters, &err_info);
      if (likely(err_info == OKAY_NO_ERROR)) {
        /* printf("pcsPipe2(%ls, %ls)\n", os_command_stri, command_line); */
        saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
        saAttr.bInheritHandle = TRUE;
        saAttr.lpSecurityDescriptor = NULL;
        if (CreatePipe(&childInputRead, &childInputWriteTemp, &saAttr, 0) &&
            DuplicateHandle(GetCurrentProcess(), childInputWriteTemp,
                            GetCurrentProcess(), &childInputWrite,
                            0, FALSE, // Handle is not inherited
                            DUPLICATE_SAME_ACCESS) &&
            CreatePipe(&childOutputReadTemp, &childOutputWrite, &saAttr, 0) &&
            DuplicateHandle(GetCurrentProcess(),childOutputReadTemp,
                            GetCurrentProcess(), &childOutputRead,
                            0, FALSE, // Handle is not inherited
                            DUPLICATE_SAME_ACCESS) &&
            DuplicateHandle(GetCurrentProcess(),childOutputWrite,
                            GetCurrentProcess(),&childErrorWrite,
                            0, TRUE, DUPLICATE_SAME_ACCESS) &&
            CloseHandle(childInputWriteTemp) &&
            CloseHandle(childOutputReadTemp)) {
          memset(&startupInfo, 0, sizeof(startupInfo));
          /* memset(&processInformation, 0, sizeof(processInformation)); */
          startupInfo.cb = sizeof(startupInfo);
          startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
          startupInfo.wShowWindow = 0;
          startupInfo.hStdInput = childInputRead;
          startupInfo.hStdOutput = childOutputWrite;
          startupInfo.hStdError = childErrorWrite;
          /* printf("before CreateProcessW\n"); */
          if (CreateProcessW(os_command_stri,
                             command_line /* lpCommandLine */,
                             NULL /* lpProcessAttributes */,
                             NULL /* lpThreadAttributes */,
                             1  /* bInheritHandles */,
                             0 /* dwCreationFlags */,
                             NULL /* lpEnvironment */,
                             NULL /* lpCurrentDirectory */,
                             &startupInfo,
                             &processInformation) != 0) {
            CloseHandle(childInputRead);
            CloseHandle(childOutputWrite);
            CloseHandle(childErrorWrite);
            *childStdin  = fdopen(_open_osfhandle((intPtrType) (childInputWrite), _O_TEXT), "w");
            *childStdout = fdopen(_open_osfhandle((intPtrType) (childOutputRead), _O_TEXT), "r");
            CloseHandle(processInformation.hProcess);
            CloseHandle(processInformation.hThread);
          } else {
            logError(printf(" *** pcsPipe2: CreateProcessW(" FMT_S_OS ", " FMT_S_OS ") failed.\nGetLastError=%d\n",
                            os_command_stri, command_line, GetLastError());
                     printf("ERROR_FILE_NOT_FOUND=%d\n", ERROR_FILE_NOT_FOUND););
            err_info = FILE_ERROR;
          } /* if */
        } else {
          err_info = FILE_ERROR;
        } /* if */
        /* printf("after CreateProcessW\n"); */
        os_stri_free(command_line);
      } /* if */
      os_stri_free(os_command_stri);
    } /* if */
    if (unlikely(err_info != OKAY_NO_ERROR)) {
      raise_error(err_info);
    } /* if */
  } /* pcsPipe2 */



void pcsPty (const const_striType command, const const_rtlArrayType parameters,
    fileType *childStdin, fileType *childStdout)

  { /* pcsPty */
    pcsPipe2(command, parameters, childStdin, childStdout);
  } /* pcsPty */



processType pcsStart (const const_striType command, const const_rtlArrayType parameters)

  {
    os_striType os_command_stri;
    os_striType command_line;
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInformation;
    int path_info = PATH_IS_NORMAL;
    errInfoType err_info = OKAY_NO_ERROR;
    win_processType process = NULL;

  /* pcsStart */
    /* printf("BEGIN pcsStart\n"); */
    os_command_stri = cp_to_os_path(command, &path_info, &err_info);
    if (likely(err_info == OKAY_NO_ERROR)) {
      command_line = prepareCommandLine(os_command_stri, parameters, &err_info);
      if (likely(err_info == OKAY_NO_ERROR)) {
        if (!ALLOC_RECORD(process, win_processRecord, count.process)) {
          err_info = MEMORY_ERROR;
        } else {
          memset(&startupInfo, 0, sizeof(startupInfo));
          /* memset(&processInformation, 0, sizeof(processInformation)); */
          startupInfo.cb = sizeof(startupInfo);
          startupInfo.dwFlags = STARTF_USESHOWWINDOW;
          startupInfo.wShowWindow = 1;
          /* printf("before CreateProcessW(%ls, %ls, ...)\n", os_command_stri, command_line); */
          if (CreateProcessW(os_command_stri,
                             command_line /* lpCommandLine */,
                             NULL /* lpProcessAttributes */,
                             NULL /* lpThreadAttributes */,
                             1  /* bInheritHandles */,
                             0 /* dwCreationFlags */,
                             NULL /* lpEnvironment */,
                             NULL /* lpCurrentDirectory */,
                             &startupInfo,
                             &processInformation) != 0) {
            /* printf("pcsStart: pProcess=" FMT_U_MEM "\n", (memSizeType) (processInformation.hProcess)); */
            /* printf("pcsStart: PID=%lu\n", (long unsigned) processInformation.dwProcessId); */
            memset(process, 0, sizeof(win_processRecord));
            process->usage_count = 1;
            process->hProcess = processInformation.hProcess;
            process->hThread  = processInformation.hThread;
            process->pid      = processInformation.dwProcessId;
            process->isTerminated = FALSE;
          } else {
            logError(printf(" *** pcsStart: CreateProcessW(" FMT_S_OS ", " FMT_S_OS ") failed.\n",
                            os_command_stri, command_line);
                     printf("GetLastError=%d\n", GetLastError());
                     printf("ERROR_FILE_NOT_FOUND=%d\n", ERROR_FILE_NOT_FOUND););
            FREE_RECORD(process, win_processRecord, count.process);
            process = NULL;
            err_info = FILE_ERROR;
          } /* if */
        } /* if */
        /* printf("after CreateProcessW\n"); */
        os_stri_free(command_line);
      } /* if */
      os_stri_free(os_command_stri);
    } /* if */
    if (unlikely(err_info != OKAY_NO_ERROR)) {
      raise_error(err_info);
    } /* if */
    /* printf("END pcsStart -> " FMT_U_MEM "\n", (memSizeType) process); */
    return (processType) process;
  } /* pcsStart */



processType pcsStartPipe (const const_striType command, const const_rtlArrayType parameters)

  {
    os_striType os_command_stri;
    os_striType command_line;
    SECURITY_ATTRIBUTES saAttr;
    HANDLE childInputRead;
    HANDLE childInputWrite;
    HANDLE childInputWriteTemp;
    HANDLE childOutputReadTemp;
    HANDLE childOutputRead;
    HANDLE childOutputWrite;
    HANDLE childErrorReadTemp;
    HANDLE childErrorRead;
    HANDLE childErrorWrite;
    STARTUPINFOW startupInfo;
    PROCESS_INFORMATION processInformation;
    int path_info = PATH_IS_NORMAL;
    errInfoType err_info = OKAY_NO_ERROR;
    win_processType process = NULL;

  /* pcsStartPipe */
    os_command_stri = cp_to_os_path(command, &path_info, &err_info);
    if (likely(err_info == OKAY_NO_ERROR)) {
      command_line = prepareCommandLine(os_command_stri, parameters, &err_info);
      if (likely(err_info == OKAY_NO_ERROR)) {
        if (!ALLOC_RECORD(process, win_processRecord, count.process)) {
          err_info = MEMORY_ERROR;
        } else {
          /* printf("pcsStartPipe(%ls, %ls)\n", os_command_stri, command_line); */
          saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
          saAttr.bInheritHandle = TRUE;
          saAttr.lpSecurityDescriptor = NULL;
          if (CreatePipe(&childInputRead, &childInputWriteTemp, &saAttr, 0) &&
              DuplicateHandle(GetCurrentProcess(), childInputWriteTemp,
                              GetCurrentProcess(), &childInputWrite,
                              0, FALSE, // Handle is not inherited
                              DUPLICATE_SAME_ACCESS) &&
              CreatePipe(&childOutputReadTemp, &childOutputWrite, &saAttr, 0) &&
              DuplicateHandle(GetCurrentProcess(),childOutputReadTemp,
                              GetCurrentProcess(), &childOutputRead,
                              0, FALSE, // Handle is not inherited
                              DUPLICATE_SAME_ACCESS) &&
              CreatePipe(&childErrorReadTemp, &childErrorWrite, &saAttr, 0) &&
              DuplicateHandle(GetCurrentProcess(),childErrorReadTemp,
                              GetCurrentProcess(), &childErrorRead,
                              0, FALSE, // Handle is not inherited
                              DUPLICATE_SAME_ACCESS) &&
              CloseHandle(childInputWriteTemp) &&
              CloseHandle(childOutputReadTemp) &&
              CloseHandle(childErrorReadTemp)) {
            memset(&startupInfo, 0, sizeof(startupInfo));
            /* memset(&processInformation, 0, sizeof(processInformation)); */
            startupInfo.cb = sizeof(startupInfo);
            startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
            startupInfo.wShowWindow = 0;
            startupInfo.hStdInput = childInputRead;
            startupInfo.hStdOutput = childOutputWrite;
            startupInfo.hStdError = childErrorWrite;
            /* printf("before CreateProcessW\n"); */
            if (CreateProcessW(os_command_stri,
                               command_line /* lpCommandLine */,
                               NULL /* lpProcessAttributes */,
                               NULL /* lpThreadAttributes */,
                               1  /* bInheritHandles */,
                               0 /* dwCreationFlags */,
                               NULL /* lpEnvironment */,
                               NULL /* lpCurrentDirectory */,
                               &startupInfo,
                               &processInformation) != 0) {
              CloseHandle(childInputRead);
              CloseHandle(childOutputWrite);
              CloseHandle(childErrorWrite);
              memset(process, 0, sizeof(win_processRecord));
              process->usage_count = 1;
              process->hProcess = processInformation.hProcess;
              process->hThread  = processInformation.hThread;
              process->pid      = processInformation.dwProcessId;
              process->isTerminated = FALSE;
              process->stdIn  = fdopen(_open_osfhandle((intPtrType) (childInputWrite), _O_TEXT), "w");
              process->stdOut = fdopen(_open_osfhandle((intPtrType) (childOutputRead), _O_TEXT), "r");
              process->stdErr = fdopen(_open_osfhandle((intPtrType) (childErrorRead), _O_TEXT), "r");
            } else {
              logError(printf(" *** pcsStartPipe: CreateProcessW(" FMT_S_OS ", " FMT_S_OS ") failed.\nGetLastError=%d\n",
                              os_command_stri, command_line, GetLastError());
                       printf("ERROR_FILE_NOT_FOUND=%d\n", ERROR_FILE_NOT_FOUND););
              FREE_RECORD(process, win_processRecord, count.process);
              process = NULL;
              err_info = FILE_ERROR;
            } /* if */
          } else {
            FREE_RECORD(process, win_processRecord, count.process);
            process = NULL;
            err_info = FILE_ERROR;
          } /* if */
        } /* if */
        /* printf("after CreateProcessW\n"); */
        os_stri_free(command_line);
      } /* if */
      os_stri_free(os_command_stri);
    } /* if */
    if (unlikely(err_info != OKAY_NO_ERROR)) {
      raise_error(err_info);
    } /* if */
    return (processType) process;
  } /* pcsStartPipe */



/**
 *  Convert a 'process' to a string.
 *  The process is converted to a string with the process identifier (PID).
 *  @return the string result of the conversion.
 *  @exception MEMORY_ERROR Not enough memory to represent the result.
 */
striType pcsStr (const const_processType process)

  {
    striType result;

  /* pcsStr */
    if (process == NULL) {
      result = cstri_to_stri("NULL");
      if (unlikely(result == NULL)) {
        raise_error(MEMORY_ERROR);
      } /* if */
    } else {
      result = intStr((intType) to_pid(process));
    } /* if */
    return result;
  } /* pcsStr */



/**
 *  Wait until the specified child process has terminated.
 *  Suspend the execution of the calling process until the
 *  specified child has terminated.
 */
void pcsWaitFor (const processType process)

  {
    DWORD exitCode = 0;

  /* pcsWaitFor */
    if (!to_isTerminated(process)) {
      if (WaitForSingleObject(to_hProcess(process), INFINITE) == WAIT_OBJECT_0) {
        if (GetExitCodeProcess(to_hProcess(process), &exitCode) != 0) {
          to_var_isTerminated(process) = TRUE;
          to_var_exitValue(process) = exitCode;
        } else {
          logError(printf(" *** pcsWaitFor: GetExitCodeProcess(" FMT_U_MEM ", 0) failed:\nGetLastError=%d\n",
                          (memSizeType) to_hProcess(process), GetLastError());
                   printf("PID=%lu\n", (long unsigned) to_pid(process)););
        } /* if */
      } else {
        raise_error(FILE_ERROR);
      } /* if */
    } /* if */
  } /* pcsWaitFor */