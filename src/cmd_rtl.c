/********************************************************************/
/*                                                                  */
/*  cmd_rtl.c     Primitive actions for various commands.           */
/*  Copyright (C) 1989 - 2006  Thomas Mertes                        */
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
/*  Free Software Foundation, Inc., 59 Temple Place, Suite 330,     */
/*  Boston, MA 02111-1307 USA                                       */
/*                                                                  */
/*  Module: Seed7 Runtime Library                                   */
/*  File: seed7/src/cmd_rtl.c                                       */
/*  Changes: 1994, 2006  Thomas Mertes                              */
/*  Content: Primitive actions for various commands.                */
/*                                                                  */
/********************************************************************/

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "errno.h"

#include "version.h"
#include "common.h"
#include "heaputl.h"
#include "striutl.h"
#include "rtl_err.h"
#include "dir_drv.h"

#ifdef USE_MMAP
#include "sys/mman.h"
#endif

#undef EXTERN
#define EXTERN
#include "cmd_rtl.h"


#undef TRACE_CMD_RTL


#ifndef S_ISLNK
#ifdef S_IFLNK
#define S_ISLNK(MODE) (((MODE) & S_IFMT) == S_IFLNK)
#else
#define S_ISLNK(MODE) FALSE
#endif
#endif

#ifndef S_ISDIR
#define S_ISDIR(MODE) (((MODE) & S_IFMT) == S_IFDIR)
#endif

#ifndef S_ISREG
#define S_ISREG(MODE) (((MODE) & S_IFMT) == S_IFREG)
#endif

#ifndef S_IRUSR
#ifdef S_IREAD
#define S_IRUSR S_IREAD
#else
#define S_IRUSR 0000400
#endif
#endif

#ifndef S_IWUSR
#ifdef S_IREAD
#define S_IWUSR S_IWRITE
#else
#define S_IWUSR 0000200
#endif
#endif

#ifndef S_IXUSR
#ifdef S_IREAD
#define S_IXUSR S_IEXEC
#else
#define S_IXUSR 0000100
#endif
#endif

#ifndef S_IRGRP
#define S_IRGRP 0000040
#endif

#ifndef S_IWGRP
#define S_IWGRP 0000020
#endif

#ifndef S_IXGRP
#define S_IXGRP 0000010
#endif

#ifndef S_IROTH
#define S_IROTH 0000004
#endif

#ifndef S_IWOTH
#define S_IWOTH 0000002
#endif

#ifndef S_IXOTH
#define S_IXOTH 0000001
#endif

#ifndef S_IRWXU
#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#endif

#ifndef S_IRWXG
#define S_IRWXG (S_IRGRP | S_IWGRP | S_IXGRP)
#endif

#ifndef S_IRWXO
#define S_IRWXO (S_IROTH | S_IWOTH | S_IXOTH)
#endif


#define SIZE_NORMAL_BUFFER   32768
#define SIZE_RESERVE_BUFFER   2048



#ifdef ANSI_C
static void remove_any_file (char *, errinfotype *);
static void copy_any_file (char *, char *, errinfotype *);
#else
static void remove_any_file ();
static void copy_any_file ();
#endif



#ifdef ANSI_C

static void remove_dir (char *file_name, errinfotype *err_info)
#else

static void remove_dir (file_name, err_info)
char *file_name;
errinfotype *err_info;
#endif

  {
    DIR *directory;
    struct dirent *current_entry;
    SIZE_TYPE rem_file_name_size;
    cstritype rem_file_name;

  /* remove_dir */
#ifdef TRACE_CMD_RTL
    printf("BEGIN remove_dir (\"%s\")\n", file_name);
#endif
/*  printf("opendir(%s);\n", file_name);
    fflush(stdout); */
    if ((directory = opendir(file_name)) == NULL) {
      *err_info = FILE_ERROR;
    } else {
      do {
        current_entry = readdir(directory);
/*      printf("$%ld$\n", (long) current_entry);
        fflush(stdout); */
      } while (current_entry != NULL &&
        (strcmp(current_entry->d_name, ".") == 0 ||
        strcmp(current_entry->d_name, "..") == 0));
      while (*err_info == OKAY_NO_ERROR && current_entry != NULL) {
/*      printf("!%s!\n", current_entry->d_name);
        fflush(stdout); */
        rem_file_name_size = strlen(file_name) + 1 + strlen(current_entry->d_name);
        if (ALLOC_CSTRI(rem_file_name, rem_file_name_size)) {
          strcpy(rem_file_name, file_name);
          strcat(rem_file_name, "/");
          strcat(rem_file_name, current_entry->d_name);
          remove_any_file(rem_file_name, err_info);
          UNALLOC_CSTRI(rem_file_name, rem_file_name_size);
        } else {
          *err_info = MEMORY_ERROR;
        } /* if */
        do {
          current_entry = readdir(directory);
/*        printf("$%ld$\n", (long) current_entry);
          fflush(stdout); */
        } while (current_entry != NULL &&
          (strcmp(current_entry->d_name, ".") == 0 ||
          strcmp(current_entry->d_name, "..") == 0));
      } /* while */
      closedir(directory);
      if (*err_info == OKAY_NO_ERROR) {
        /* printf("before remove directory <%s>\n", file_name); */
        if (rmdir(file_name) != 0) {
          *err_info = FILE_ERROR;
        } /* if */
/*      okay = errno;
        printf("errno=%d\n", okay);
        printf("EACCES=%d  EBUSY=%d  EEXIST=%d  ENOTEMPTY=%d  ENOENT=%d  ENOTDIR=%d  EROFS=%d\n",
            EACCES, EBUSY, EEXIST, ENOTEMPTY, ENOENT, ENOTDIR, EROFS); */
        /* printf("remove ==> %d\n", remove(file_name)); */
      } /* if */
    } /* if */
#ifdef TRACE_CMD_RTL
    printf("END remove_dir (\"%s\")\n", file_name);
#endif
  } /* remove_dir */



#ifdef ANSI_C

static void remove_any_file (char *file_name, errinfotype *err_info)
#else

static void remove_any_file (file_name, err_info)
char *file_name;
errinfotype *err_info;
#endif

  {
    struct stat file_stat;

  /* remove_any_file */
#ifdef TRACE_CMD_RTL
    printf("BEGIN remove_any_file (\"%s\")\n", file_name);
#endif
    if (stat(file_name, &file_stat) == 0) {
      *err_info = FILE_ERROR;
    } else {
      if (S_ISLNK(file_stat.st_mode)) {
      } else if (S_ISREG(file_stat.st_mode)) {
        if (remove(file_name) != 0) {
          *err_info = FILE_ERROR;
        } /* if */
      } else if (S_ISDIR(file_stat.st_mode)) {
        remove_dir(file_name, err_info);
      } /* if */
    } /* if */
#ifdef TRACE_CMD_RTL
    printf("END remove_any_file (\"%s\")\n", file_name);
#endif
  } /* remove_any_file */



#ifdef ANSI_C

static void copy_file (char *from_name, char *to_name, errinfotype *err_info)
#else

static void copy_file (from_name, to_name, err_info)
char *from_name;
char *to_name;
errinfotype *err_info;
#endif

  {
    FILE *from_file;
    FILE *to_file;
#ifdef USE_MMAP
    struct stat file_stat;
    memsizetype file_length;
    ustritype file_content;
#else
    char *buffer;
    SIZE_TYPE buffer_size;
    char reserve_buffer[SIZE_RESERVE_BUFFER];
    SIZE_TYPE bytes_read;
#endif

  /* copy_file */
#ifdef TRACE_CMD_RTL
    printf("BEGIN copy_file (\"%s\", \"%s\")\n", from_name, to_name);
#endif
    if ((from_file = fopen(from_name, "rb")) != NULL) {
      if ((to_file = fopen(to_name, "wb")) != NULL) {
#ifdef USE_MMAP
        if (fstat(fileno(from_file), &file_stat) == 0) {
          file_length = file_stat.st_size;
          if ((file_content = (ustritype) mmap(NULL, file_length,
              PROT_READ, MAP_PRIVATE, fileno(from_file),
              0)) != (ustritype) -1) {
            if (fwrite(file_content, 1, file_length, to_file) != file_length) {
              *err_info = FILE_ERROR;
            } /* if */
          } /* if */
        } /* if */
#else
        if (ALLOC_BYTES(buffer, SIZE_NORMAL_BUFFER)) {
          COUNT_BYTES(SIZE_NORMAL_BUFFER);
          buffer_size = SIZE_NORMAL_BUFFER;
        } else {
          buffer = reserve_buffer;
          buffer_size = SIZE_RESERVE_BUFFER;
        } /* if */
        while (*err_info == OKAY_NO_ERROR && (bytes_read =
            fread(buffer, 1, buffer_size, from_file)) != 0) {
          if (fwrite(buffer, 1, bytes_read, to_file) != bytes_read) {
            *err_info = FILE_ERROR;
          } /* if */
        } /* while */
        if (buffer != reserve_buffer) {
          FREE_BYTES(buffer, SIZE_NORMAL_BUFFER);
        } /* if */
#endif
        if (fclose(from_file) != 0) {
          *err_info = FILE_ERROR;
        } /* if */
        if (fclose(to_file) != 0) {
          *err_info = FILE_ERROR;
        } /* if */
        if (*err_info != OKAY_NO_ERROR) {
          remove(to_name);
        } /* if */
      } else {
        fclose(from_file);
        *err_info = FILE_ERROR;
      } /* if */
    } else {
      *err_info = FILE_ERROR;
    } /* if */
#ifdef TRACE_CMD_RTL
    printf("END copy_file (\"%s\", \"%s\")\n", from_name, to_name);
#endif
  } /* copy_file */



#ifdef ANSI_C

static void copy_dir (char *from_name, char *to_name, errinfotype *err_info)
#else

static void copy_dir (from_name, to_name, err_info)
char *from_name;
char *to_name;
errinfotype *err_info;
#endif

  {
    DIR *directory;
    struct dirent *current_entry;
    char from_file_name[2000];
    char to_file_name[2000];
    struct stat to_stat;

  /* copy_dir */
#ifdef TRACE_CMD_RTL
    printf("BEGIN copy_dir (\"%s\", \"%s\")\n", from_name, to_name);
#endif
/*  printf("opendir(%s);\n", from_name);
    fflush(stdout); */
    if ((directory = opendir(from_name)) == NULL) {
      *err_info = FILE_ERROR;
    } else {
      if (stat(to_name, &to_stat) == 0) {
        /* Directore exists already */
        *err_info = FILE_ERROR;
      } else {
        if (mkdir(to_name, (S_IRWXU | S_IRWXG | S_IRWXO)) != 0) {
          *err_info = FILE_ERROR;
        } else {
          do {
            current_entry = readdir(directory);
  /*        printf("$%ld$\n", (long) current_entry);
            fflush(stdout); */
          } while (current_entry != NULL &&
            (strcmp(current_entry->d_name, ".") == 0 ||
            strcmp(current_entry->d_name, "..") == 0));
          while (*err_info == OKAY_NO_ERROR && current_entry != NULL) {
  /*        printf("!%s!\n", current_entry->d_name);
            fflush(stdout); */
            strcpy(from_file_name, from_name);
            strcat(from_file_name, "/");
            strcat(from_file_name, current_entry->d_name);
            strcpy(to_file_name, to_name);
            strcat(to_file_name, "/");
            strcat(to_file_name, current_entry->d_name);
            copy_any_file(from_file_name, to_file_name, err_info);
            do {
              current_entry = readdir(directory);
  /*          printf("$%ld$\n", (long) current_entry);
              fflush(stdout); */
            } while (current_entry != NULL &&
              (strcmp(current_entry->d_name, ".") == 0 ||
              strcmp(current_entry->d_name, "..") == 0));
          } /* while */
          if (*err_info != OKAY_NO_ERROR) {
            remove_dir(to_name, err_info);
          } /* if */
        } /* if */
      } /* if */
      closedir(directory);
    } /* if */
#ifdef TRACE_CMD_RTL
    printf("END copy_dir (\"%s\", \"%s\")\n", from_name, to_name);
#endif
  } /* copy_dir */



#ifdef ANSI_C

static void copy_any_file (char *from_name, char *to_name, errinfotype *err_info)
#else

static void copy_any_file (from_name, to_name, err_info)
char *from_name;
char *to_name;
errinfotype *err_info;
#endif

  {
    struct stat from_stat;
    struct stat to_stat;

  /* copy_any_file */
#ifdef TRACE_CMD_RTL
    printf("BEGIN copy_any_file (\"%s\", \"%s\")\n", from_name, to_name);
#endif
    if (stat(from_name, &from_stat) != 0) {
      *err_info = FILE_ERROR;
    } else {
      if (stat(to_name, &to_stat) == 0) {
        *err_info = FILE_ERROR;
      } else {
        if (S_ISLNK(from_stat.st_mode)) {
        } else if (S_ISREG(from_stat.st_mode)) {
          copy_file(from_name, to_name, err_info);
        } else if (S_ISDIR(from_stat.st_mode)) {
          copy_dir(from_name, to_name, err_info);
        } /* if */
        chmod(to_name, from_stat.st_mode);
        chown(to_name, from_stat.st_uid, from_stat.st_gid);
      } /* if */
    } /* if */
#ifdef TRACE_CMD_RTL
    printf("END copy_any_file (\"%s\", \"%s\")\n", from_name, to_name);
#endif
  } /* copy_any_file */



#ifdef ANSI_C

static void move_any_file (char *from_name, char *to_name, errinfotype *err_info)
#else

static void move_any_file (from_name, to_name, err_info)
char *from_name;
char *to_name;
errinfotype *err_info;
#endif

  { /* move_any_file */
#ifdef TRACE_CMD_RTL
    printf("BEGIN move_any_file (\"%s\", \"%s\")\n", from_name, to_name);
#endif
    if (rename(from_name, to_name) != 0) {
      switch (errno) {
        case EXDEV:
          copy_any_file(from_name, to_name, err_info);
          if (*err_info == OKAY_NO_ERROR) {
            remove_any_file(from_name, err_info);
          } else {
            remove_any_file(to_name, err_info);
          } /* if */
          break;
      } /* switch */
    } /* if */
#ifdef TRACE_CMD_RTL
    printf("END move_any_file (\"%s\", \"%s\", %d)\n", from_name, to_name, *err_info);
#endif
  } /* move_any_file */



#ifdef OUT_OF_ORDER
#ifdef ANSI_C

static arraytype read_dir (char *dir_name)
#else

static arraytype read_dir (dir_name)
char *dir_name;
#endif

  {
    arraytype dir_array;
    memsizetype max_array_size;
    memsizetype used_array_size;
    memsizetype position;
    DIR *directory;
    struct dirent *current_entry;
    stritype str1;
    booltype okay;

  /* read_dir */
/*  printf("opendir(%s);\n", dir_name);
    fflush(stdout); */
    if ((directory = opendir(dir_name)) != NULL) {
      max_array_size = 256;
      if (ALLOC_ARRAY(dir_array, max_array_size)) {
        COUNT_ARRAY(max_array_size);
        used_array_size = 0;
        do {
          current_entry = readdir(directory);
/*        printf("$%ld$\n", (long) current_entry);
          fflush(stdout); */
        } while (current_entry != NULL &&
          (strcmp(current_entry->d_name, ".") == 0 ||
          strcmp(current_entry->d_name, "..") == 0));
        okay = TRUE;
        while (okay && current_entry != NULL) {
          if (used_array_size >= max_array_size) {
            if (!RESIZE_ARRAY(dir_array, max_array_size,
                max_array_size + 256)) {
              okay = FALSE;
            } else {
              COUNT3_ARRAY(max_array_size, max_array_size + 256);
              max_array_size = max_array_size + 256;
            } /* if */
          } /* if */
          if (okay) {
            str1 = cp_to_stri(current_entry->d_name);
            if (str1 == NULL) {
              okay = FALSE;
            } else {
              dir_array->arr[(int) used_array_size].type_of = take_type(SYS_STRI_TYPE);
              dir_array->arr[(int) used_array_size].entity = NULL;
              dir_array->arr[(int) used_array_size].value.strivalue = str1;
              INIT_CLASS_OF_VAR(&dir_array->arr[(int) used_array_size],
                  STRIOBJECT);
              used_array_size++;
              do {
                current_entry = readdir(directory);
              } while (current_entry != NULL &&
                (strcmp(current_entry->d_name, ".") == 0 ||
                strcmp(current_entry->d_name, "..") == 0));
            } /* if */
          } /* if */
        } /* while */
        if (okay) {
          if (!RESIZE_ARRAY(dir_array, max_array_size,
              used_array_size)) {
            okay = FALSE;
          } else {
            COUNT3_ARRAY(max_array_size, used_array_size);
            dir_array->min_position = 1;
            dir_array->max_position = used_array_size;
          } /* if */
        } /* if */
        if (!okay) {
          for (position = 0; position < used_array_size; position++) {
            FREE_STRI(dir_array->arr[(int) position].value.strivalue,
                dir_array->arr[(int) position].value.strivalue->size);
          } /* for */
          FREE_ARRAY(dir_array, max_array_size);
          dir_array = NULL;
        } /* if */
      } /* if */
      closedir(directory);
    } else {
      if (ALLOC_ARRAY(dir_array, (memsizetype) 0)) {
        COUNT_ARRAY(0);
        dir_array->min_position = 1;
        dir_array->max_position = 0;
      } /* if */
    } /* if */
    return(dir_array);
  } /* read_dir */
#endif



#ifdef ANSI_C

void cmdChdir (stritype dir_name)
#else

void cmdChdir (dir_name)
stritype dir_name;
#endif

  {
    cstritype os_dir_name;

  /* cmdChdir */
    os_dir_name = cp_to_cstri(dir_name);
    if (os_dir_name == NULL) {
      raise_error(MEMORY_ERROR);
    } else {
      chdir(os_dir_name);
      free_cstri(os_dir_name, dir_name);
    } /* if */
  } /* cmdChdir */



#ifdef ANSI_C

void cmdCopy (stritype source_name, stritype dest_name)
#else

void cmdCopy (source_name, dest_name)
stritype source_name;
stritype dest_name;
#endif

  {
    cstritype os_source_name;
    cstritype os_dest_name;
    errinfotype err_info = OKAY_NO_ERROR;

  /* cmdCopy */
    os_source_name = cp_to_cstri(source_name);
    if (os_source_name == NULL) {
      err_info = MEMORY_ERROR;
    } else {
      os_dest_name = cp_to_cstri(dest_name);
      if (os_dest_name == NULL) {
	err_info = MEMORY_ERROR;
      } else {
        copy_any_file(os_source_name, os_dest_name, &err_info); /* SYS_FILE_EXCEPTION */
        free_cstri(os_dest_name, dest_name);
      } /* if */
      free_cstri(os_source_name, source_name);
    } /* if */
    if (err_info != OKAY_NO_ERROR) {
      raise_error(err_info);
    } /* if */
  } /* cmdCopy */



#ifdef ANSI_C

stritype cmdGetcwd (void)
#else

stritype cmdGetcwd ()
#endif

  {
    char buffer[2000];
    char *cwd;
    stritype result;

  /* cmdGetcwd */
    if ((cwd = getcwd(buffer, 2000)) == NULL) {
      raise_error(MEMORY_ERROR);
      return(NULL);
    } else {
      result = cp_to_stri(cwd);
      if (result == NULL) {
        raise_error(MEMORY_ERROR);
        return(NULL);
      } else {
        return(result);
      } /* if */
    } /* if */
  } /* cmdGetcwd */



#ifdef ANSI_C

inttype cmdLng (stritype file_name)
#else

inttype cmdLng (file_name)
stritype file_name;
#endif

  {
    cstritype os_file_name;
    struct stat stat_buf;
    inttype result;

  /* cmdLng */
    os_file_name = cp_to_cstri(file_name);
    if (os_file_name == NULL) {
      raise_error(MEMORY_ERROR);
      return(0);
    } else {
      if (stat(os_file_name, &stat_buf) == 0) {
        result = (inttype) stat_buf.st_size;
      } else {
        result = 0;
      } /* if */
      free_cstri(os_file_name, file_name);
    } /* if */
    return(result);
  } /* cmdLng */



#ifdef OUT_OF_ORDER
#ifdef ANSI_C

arraytype cmdLs (stritype dir_name)
#else

arraytype cmdLs (dir_name)
stritype dir_name;
#endif

  {
    cstritype os_dir_name;
    arraytype result;

  /* cmdLs */
    os_dir_name = cp_to_cstri(dir_name);
    if (os_dir_name == NULL) {
      raise_error(MEMORY_ERROR);
      return(NULL);
    } else {
      if ((result = read_dir(os_dir_name)) == NULL) {
        raise_error(MEMORY_ERROR);
        return(NULL);
      } else {
        free_cstri(os_dir_name, dir_name);
        qsort((void *) result->arr,
            (size_t) (result->max_position - result->min_position + 1),
            sizeof(objectrecord), &cmp_mem);
        return(result);
      } /* if */
    } /* if */
  } /* cmdLs */
#endif



#ifdef ANSI_C

void cmdMove (stritype source_name, stritype dest_name)
#else

void cmdMove (source_name, dest_name)
stritype source_name;
stritype dest_name;
#endif

  {
    cstritype os_source_name;
    cstritype os_dest_name;
    errinfotype err_info = OKAY_NO_ERROR;

  /* cmdMove */
    os_source_name = cp_to_cstri(source_name);
    if (os_source_name == NULL) {
      err_info = MEMORY_ERROR;
    } else {
      os_dest_name = cp_to_cstri(dest_name);
      if (os_dest_name == NULL) {
	err_info = MEMORY_ERROR;
      } else {
        move_any_file(os_source_name, os_dest_name, &err_info); /* SYS_FILE_EXCEPTION */
        free_cstri(os_dest_name, dest_name);
      } /* if */
      free_cstri(os_source_name, source_name);
    } /* if */
    if (err_info != OKAY_NO_ERROR) {
      raise_error(err_info);
    } /* if */
  } /* cmdMove */



#ifdef ANSI_C

void cmdRemove (stritype file_name)
#else

void cmdRemove (file_name)
stritype file_name;
#endif

  {
    cstritype os_file_name;

  /* cmdRemove */
    os_file_name = cp_to_cstri(file_name);
    if (os_file_name == NULL) {
      raise_error(MEMORY_ERROR);
    } else {
      remove(os_file_name);
      free_cstri(os_file_name, file_name);
    } /* if */
  } /* cmdRemove */



#ifdef ANSI_C

void cmdSh (stritype command_stri)
#else

void cmdSh (command_stri)
stritype command_stri;
#endif

  {
    cstritype os_command_stri;

  /* cmdSh */
    os_command_stri = cp_to_cstri(command_stri);
    if (os_command_stri == NULL) {
      raise_error(MEMORY_ERROR);
    } else {
      system(os_command_stri);
      free_cstri(os_command_stri, command_stri);
    } /* if */
  } /* cmdSh */