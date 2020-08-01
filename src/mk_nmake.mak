# Makefile for nmake from windows and gcc from MinGW.
# To compile use a windows console and call:
#   nmake /f mk_nmake.mak depend
#   nmake /f mk_nmake.mak
# If you use MSYS with MinGW you should use mk_msys.mak instead.

# CFLAGS = -O2 -fomit-frame-pointer -funroll-loops -Wall
CFLAGS = -O2 -fomit-frame-pointer -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -g -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -g -pg -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -pg -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -funroll-loops -Wall -pg
LFLAGS =
# LFLAGS = -pg
# LFLAGS = -pg -lc_p
LIBS = -lm -lgdi32 -lws2_32
SEED7_LIB = seed7_05.a
COMP_DATA_LIB = s7_data.a
COMPILER_LIB = s7_comp.a
CC = gcc

# SCREEN_OBJ = scr_x11.o
# SCREEN_SRC = scr_x11.c
# SCREEN_OBJ = scr_infi.o kbd_infi.o trm_inf.o
# SCREEN_SRC = scr_inf.c kbd_inf.c trm_inf.c
# SCREEN_OBJ = scr_infp.o kbd_infp.o trm_cap.o
# SCREEN_SRC = scr_inf.c kbd_inf.c trm_cap.c
# SCREEN_OBJ = scr_cur.o
# SCREEN_SRC = scr_cur.c
# SCREEN_OBJ = scr_cap.o
# SCREEN_SRC = scr_cap.c
# SCREEN_OBJ = scr_tcp.o
# SCREEN_SRC = scr_tcp.c
SCREEN_OBJ = scr_win.o
SCREEN_SRC = scr_win.c

MOBJ1 = hi.o
POBJ1 = runerr.o option.o primitiv.o
LOBJ1 = actlib.o arrlib.o biglib.o blnlib.o bstlib.o chrlib.o cmdlib.o dcllib.o drwlib.o enulib.o
LOBJ2 = fillib.o fltlib.o hshlib.o intlib.o itflib.o kbdlib.o lstlib.o prclib.o prglib.o reflib.o
LOBJ3 = rfllib.o scrlib.o sctlib.o setlib.o soclib.o strlib.o timlib.o typlib.o ut8lib.o
EOBJ1 = exec.o doany.o memory.o
AOBJ1 = act_comp.o prg_comp.o analyze.o syntax.o token.o parser.o name.o type.o
AOBJ2 = expr.o atom.o object.o scanner.o literal.o numlit.o findid.o
AOBJ3 = error.o infile.o symbol.o info.o stat.o fatal.o match.o
GOBJ1 = syvarutl.o traceutl.o actutl.o arrutl.o executl.o blockutl.o
GOBJ2 = entutl.o identutl.o chclsutl.o sigutl.o
ROBJ1 = arr_rtl.o big_rtl.o bln_rtl.o bst_rtl.o chr_rtl.o cmd_rtl.o drw_rtl.o fil_rtl.o flt_rtl.o
ROBJ2 = hsh_rtl.o int_rtl.o kbd_rtl.o scr_rtl.o set_rtl.o soc_rtl.o str_rtl.o ut8_rtl.o heaputl.o
ROBJ3 = striutl.o
DOBJ1 = $(SCREEN_OBJ) tim_win.o drw_win.o
OBJ = $(MOBJ1)
SEED7_LIB_OBJ = $(ROBJ1) $(ROBJ2) $(ROBJ3) $(DOBJ1)
COMP_DATA_LIB_OBJ = typ_data.o rfl_data.o ref_data.o listutl.o flistutl.o typeutl.o datautl.o
COMPILER_LIB_OBJ = $(POBJ1) $(LOBJ1) $(LOBJ2) $(LOBJ3) $(EOBJ1) $(AOBJ1) $(AOBJ2) $(AOBJ3) $(GOBJ1) $(GOBJ2)

MSRC1 = hi.c
PSRC1 = runerr.c option.c primitiv.c
LSRC1 = actlib.c arrlib.c biglib.c blnlib.c bstlib.c chrlib.c cmdlib.c dcllib.c drwlib.c enulib.c
LSRC2 = fillib.c fltlib.c hshlib.c intlib.c itflib.c kbdlib.c lstlib.c prclib.c prglib.c reflib.c
LSRC3 = rfllib.c scrlib.c sctlib.c setlib.c soclib.c strlib.c timlib.c typlib.c ut8lib.c
ESRC1 = exec.c doany.c memory.c
ASRC1 = act_comp.c prg_comp.c analyze.c syntax.c token.c parser.c name.c type.c
ASRC2 = expr.c atom.c object.c scanner.c literal.c numlit.c findid.c
ASRC3 = error.c infile.c symbol.c info.c stat.c fatal.c match.c
GSRC1 = syvarutl.c traceutl.c actutl.c arrutl.c executl.c blockutl.c
GSRC2 = entutl.c identutl.c chclsutl.c sigutl.c
RSRC1 = arr_rtl.c big_rtl.c bln_rtl.c bst_rtl.c chr_rtl.c cmd_rtl.c drw_rtl.c fil_rtl.c flt_rtl.c
RSRC2 = hsh_rtl.c int_rtl.c kbd_rtl.c scr_rtl.c set_rtl.c soc_rtl.c str_rtl.c ut8_rtl.c heaputl.c
RSRC3 = striutl.c
DSRC1 = $(SCREEN_SRC) tim_win.c drw_win.c
SRC = $(MSRC1)
SEED7_LIB_SRC = $(RSRC1) $(RSRC2) $(RSRC3) $(DSRC1)
COMP_DATA_LIB_SRC = typ_data.c rfl_data.c ref_data.c listutl.c flistutl.c typeutl.c datautl.c
COMPILER_LIB_SRC = $(PSRC1) $(LSRC1) $(LSRC2) $(LSRC3) $(ESRC1) $(ASRC1) $(ASRC2) $(ASRC3) $(GSRC1) $(GSRC2)

hi: $(OBJ) $(COMPILER_LIB) $(COMP_DATA_LIB) $(SEED7_LIB)
	$(CC) $(LFLAGS) $(OBJ) $(COMPILER_LIB) $(COMP_DATA_LIB) $(SEED7_LIB) $(LIBS) -o hi
	copy hi.exe ..\prg /Y
	.\hi level

hi.gp: $(OBJ)
	$(CC) $(LFLAGS) $(OBJ) $(LIBS) -o /usr/local/bin/hi.gp
	hi level

scr_x11.o: scr_x11.c version.h scr_drv.h trm_drv.h
	$(CC) $(CFLAGS) -c scr_x11.c

scr_infi.o: scr_inf.c version.h scr_drv.h trm_drv.h
	echo "#undef  USE_TERMCAP" > inf_conf.h
	$(CC) $(CFLAGS) -c scr_inf.c
	mv scr_inf.o scr_infi.o

scr_infp.o: scr_inf.c version.h scr_drv.h trm_drv.h
	echo "#define USE_TERMCAP" > inf_conf.h
	$(CC) $(CFLAGS) -c scr_inf.c
	mv scr_inf.o scr_infp.o

kbd_infi.o: kbd_inf.c version.h kbd_drv.h trm_drv.h
	echo "#undef  USE_TERMCAP" > inf_conf.h
	$(CC) $(CFLAGS) -c kbd_inf.c
	mv kbd_inf.o kbd_infi.o

kbd_infp.o: kbd_inf.c version.h kbd_drv.h trm_drv.h
	echo "#define USE_TERMCAP" > inf_conf.h
	$(CC) $(CFLAGS) -c kbd_inf.c
	mv kbd_inf.o kbd_infp.o

trm_inf.o: trm_inf.c version.h trm_drv.h
	$(CC) $(CFLAGS) -c trm_inf.c

trm_cap.o: trm_cap.c version.h trm_drv.h
	$(CC) $(CFLAGS) -c trm_cap.c

scr_cur.o: scr_cur.c version.h scr_drv.h
	$(CC) $(CFLAGS) -c scr_cur.c


clear:
	del *.o
	del *.a
	del depend
	del a_depend
	del b_depend
	del c_depend
	del version.h

dep: depend

strip:
	strip /usr/local/bin/hi

version.h:
	echo #define ANSI_C > version.h
	echo #define USE_DIRENT >> version.h
	echo #define PATH_DELIMITER '\\' >> version.h
	echo #define CATCH_SIGNALS >> version.h
	echo #undef  USE_MMAP >> version.h
	echo #undef  INCL_NCURSES_TERM >> version.h
	echo #undef  INCL_CURSES_BEFORE_TERM >> version.h
	echo #define MKDIR_WITH_ONE_PARAMETER >> version.h
	echo #define CHOWN_MISSING >> version.h
	echo #undef  CHMOD_MISSING >> version.h
	echo #define HAS_LONGTYPE_64 >> version.h
	echo #define USE_WFOPEN >> version.h
	echo #define USE_FSEEKO64 >> version.h
	echo #define USE_WINSOCK >> version.h
	echo #include "stdio.h" > chkftell.c
	echo int main (int argc, char **argv) >> chkftell.c
	echo { >> chkftell.c
	echo FILE *aFile; >> chkftell.c
	echo aFile = popen("dir","r"); >> chkftell.c
	echo if (ftell(aFile) != -1) { >> chkftell.c
	echo printf("\043define FTELL_WRONG_FOR_PIPE\n"); >> chkftell.c
	echo } >> chkftell.c
	echo return 0; >> chkftell.c
	echo } >> chkftell.c
	$(CC) chkftell.c -o chkftell
	chkftell >> version.h
	del chkftell.c
	del chkftell.exe
	echo #define OBJECT_FILE_EXTENSION ".o" >> version.h
	echo #define EXECUTABLE_FILE_EXTENSION ".exe" >> version.h
	echo #define C_COMPILER "$(CC)" >> version.h
	echo #define REDIRECT_C_ERRORS "2>" >> version.h
	echo #define SYSTEM_LIBS "$(LIBS)" >> version.h
	echo #include "stdio.h" > seed7lib.c
	echo #include "stddef.h" >> seed7lib.c
	echo char *getcwd(char *buf, size_t size); >> seed7lib.c
	echo int main (int argc, char **argv) >> seed7lib.c
	echo { >> seed7lib.c
	echo char buffer[4096]; >> seed7lib.c
	echo int position; >> seed7lib.c
	echo getcwd(buffer, sizeof(buffer)); >> seed7lib.c
	echo printf("\043define SEED7_LIB \042"); >> seed7lib.c
	echo for (position = 0; buffer[position] != '\0'; position++) { >> seed7lib.c
	echo putchar(buffer[position] == '\\' ? '/' : buffer[position]); >> seed7lib.c
	echo } >> seed7lib.c
	echo printf("/$(SEED7_LIB)\042\n"); >> seed7lib.c
	echo return 0; >> seed7lib.c
	echo } >> seed7lib.c
	$(CC) seed7lib.c -o seed7lib
	seed7lib >> ..\src\version.h
	del seed7lib.c
	del seed7lib.exe
	echo #include "stdio.h" > datalib.c
	echo #include "stddef.h" >> datalib.c
	echo char *getcwd(char *buf, size_t size); >> datalib.c
	echo int main (int argc, char **argv) >> datalib.c
	echo { >> datalib.c
	echo char buffer[4096]; >> datalib.c
	echo int position; >> datalib.c
	echo getcwd(buffer, sizeof(buffer)); >> datalib.c
	echo printf("\043define COMP_DATA_LIB \042"); >> datalib.c
	echo for (position = 0; buffer[position] != '\0'; position++) { >> datalib.c
	echo putchar(buffer[position] == '\\' ? '/' : buffer[position]); >> datalib.c
	echo } >> datalib.c
	echo printf("/$(COMP_DATA_LIB)\042\n"); >> datalib.c
	echo return 0; >> datalib.c
	echo } >> datalib.c
	$(CC) datalib.c -o datalib
	datalib >> ..\src\version.h
	del datalib.c
	del datalib.exe
	echo #include "stdio.h" > complib.c
	echo #include "stddef.h" >> complib.c
	echo char *getcwd(char *buf, size_t size); >> complib.c
	echo int main (int argc, char **argv) >> complib.c
	echo { >> complib.c
	echo char buffer[4096]; >> complib.c
	echo int position; >> complib.c
	echo getcwd(buffer, sizeof(buffer)); >> complib.c
	echo printf("\043define COMPILER_LIB \042"); >> complib.c
	echo for (position = 0; buffer[position] != '\0'; position++) { >> complib.c
	echo putchar(buffer[position] == '\\' ? '/' : buffer[position]); >> complib.c
	echo } >> complib.c
	echo printf("/$(COMPILER_LIB)\042\n"); >> complib.c
	echo return 0; >> complib.c
	echo } >> complib.c
	$(CC) complib.c -o complib
	complib >> ..\src\version.h
	del complib.c
	del complib.exe
	echo #include "stdio.h" > libpath.c
	echo #include "stddef.h" >> libpath.c
	echo int chdir(char *path); >> libpath.c
	echo char *getcwd(char *buf, size_t size); >> libpath.c
	echo int main (int argc, char **argv) >> libpath.c
	echo { >> libpath.c
	echo char buffer[4096]; >> libpath.c
	echo int position; >> libpath.c
	echo chdir("../lib"); >> libpath.c
	echo getcwd(buffer, sizeof(buffer)); >> libpath.c
	echo printf("\043define SEED7_LIBRARY \042"); >> libpath.c
	echo for (position = 0; buffer[position] != '\0'; position++) { >> libpath.c
	echo putchar(buffer[position] == '\\' ? '/' : buffer[position]); >> libpath.c
	echo } >> libpath.c
	echo printf("\042\n"); >> libpath.c
	echo return 0; >> libpath.c
	echo } >> libpath.c
	$(CC) libpath.c -o libpath
	libpath >> ..\src\version.h
	del libpath.c
	del libpath.exe

hi.o: hi.c
	$(CC) $(CFLAGS) -c hi.c

.c.o:
	$(CC) $(CFLAGS) -c $<

depend: a_depend b_depend c_depend version.h
	$(CC) -M $(SRC) > depend

a_depend: version.h
	$(CC) -M $(SEED7_LIB_SRC) > a_depend

b_depend: version.h
	$(CC) -M $(COMP_DATA_LIB_SRC) > b_depend

c_depend: version.h
	$(CC) -M $(COMPILER_LIB_SRC) > c_depend

level.h:
	hi level

$(SEED7_LIB): $(SEED7_LIB_OBJ)
	ar r $(SEED7_LIB) $(SEED7_LIB_OBJ)

$(COMP_DATA_LIB): $(COMP_DATA_LIB_OBJ)
	ar r $(COMP_DATA_LIB) $(COMP_DATA_LIB_OBJ)

$(COMPILER_LIB): $(COMPILER_LIB_OBJ)
	ar r $(COMPILER_LIB) $(COMPILER_LIB_OBJ)

wc: $(SRC)
	wc $(GSRC1) $(GSRC2)
	wc $(ASRC1) $(ASRC2) $(ASRC3)
	wc $(RSRC1) $(RSRC2) $(RSRC3)
	wc $(ESRC1)
	wc $(LSRC1) $(LSRC2) $(LSRC3)
	wc $(DSRC1)
	wc $(MSRC1)
	wc $(SRC)

lint: $(SRC)
	lint -p $(SRC) $(LIBS)

lint2: $(SRC)
	lint -Zn2048 $(SRC) $(LIBS)

!if exist(depend)
!include depend
!endif
!if exist(a_depend)
!include a_depend
!endif
!if exist(b_depend)
!include b_depend
!endif
!if exist(c_depend)
!include c_depend
!endif
