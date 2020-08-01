# Makefile for linux/bsd/unix, emcc from Emscripten and gcc. Commands executed by: bash
# To compile use a command shell and call:
#   make -f mk_emccl.mak depend
#   make -f mk_emccl.mak

# CFLAGS = -O2 -fomit-frame-pointer -funroll-loops -Wall
# CFLAGS = -O2 -fomit-frame-pointer -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
CFLAGS = -O2 -g -ffunction-sections -fdata-sections -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -g -ffunction-sections -fdata-sections -Wall -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -g -pg -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -pg -Wall -Wstrict-prototypes -Winline -Wconversion -Wshadow -Wpointer-arith
# CFLAGS = -O2 -funroll-loops -Wall -pg
LDFLAGS = -s ASSERTIONS=0 -s ALLOW_MEMORY_GROWTH=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap','UTF8ToString']
# LDFLAGS = -Wl,--gc-sections,--stack,8388608
# LDFLAGS = -pg
# LDFLAGS = -pg -lc_p
SYSTEM_LIBS =
# SYSTEM_LIBS = -lm -lws2_32 -lgmp
SYSTEM_CONSOLE_LIBS =
SYSTEM_DRAW_LIBS =
SEED7_LIB = seed7_05.a
CONSOLE_LIB = s7_con.a
DRAW_LIB = s7_draw.a
COMP_DATA_LIB = s7_data.a
COMPILER_LIB = s7_comp.a
ALL_S7_LIBS = ../bin/$(COMPILER_LIB) ../bin/$(COMP_DATA_LIB) ../bin/$(DRAW_LIB) ../bin/$(CONSOLE_LIB) ../bin/$(SEED7_LIB)
# CC = em++
CC = emcc
GET_CC_VERSION_INFO = $(CC) --version >

TERMINFO_OR_TERMCAP = USE_TERMINFO

MOBJ = s7.o
POBJ = runerr.o option.o primitiv.o
LOBJ = actlib.o arrlib.o biglib.o binlib.o blnlib.o bstlib.o chrlib.o cmdlib.o conlib.o dcllib.o \
       drwlib.o enulib.o fillib.o fltlib.o hshlib.o intlib.o itflib.o kbdlib.o lstlib.o pcslib.o \
       pollib.o prclib.o prglib.o reflib.o rfllib.o sctlib.o setlib.o soclib.o sqllib.o strlib.o \
       timlib.o typlib.o ut8lib.o
EOBJ = exec.o doany.o objutl.o
AOBJ = act_comp.o prg_comp.o analyze.o syntax.o token.o parser.o name.o type.o \
       expr.o atom.o object.o scanner.o literal.o numlit.o findid.o \
       error.o infile.o libpath.o symbol.o info.o stat.o fatal.o match.o
GOBJ = syvarutl.o traceutl.o actutl.o executl.o blockutl.o \
       entutl.o identutl.o chclsutl.o arrutl.o
ROBJ = arr_rtl.o bln_rtl.o bst_rtl.o chr_rtl.o cmd_rtl.o con_rtl.o dir_rtl.o drw_rtl.o fil_rtl.o \
       flt_rtl.o hsh_rtl.o int_rtl.o itf_rtl.o pcs_rtl.o set_rtl.o soc_rtl.o sql_rtl.o str_rtl.o \
       tim_rtl.o ut8_rtl.o heaputl.o numutl.o sigutl.o striutl.o
DOBJ = big_rtl.o big_gmp.o cmd_unx.o dir_win.o dll_unx.o fil_unx.o pcs_unx.o pol_unx.o soc_none.o \
       sql_base.o sql_fire.o sql_lite.o sql_my.o sql_oci.o sql_odbc.o sql_post.o tim_unx.o
OBJ = $(MOBJ)
SEED7_LIB_OBJ = $(ROBJ) $(DOBJ)
DRAW_LIB_OBJ = gkb_rtl.o drw_dos.o gkb_x11.o
CONSOLE_LIB_OBJ = kbd_rtl.o con_emc.o
COMP_DATA_LIB_OBJ = typ_data.o rfl_data.o ref_data.o listutl.o flistutl.o typeutl.o datautl.o
COMPILER_LIB_OBJ = $(POBJ) $(LOBJ) $(EOBJ) $(AOBJ) $(GOBJ)

MSRC = s7.c
PSRC = runerr.c option.c primitiv.c
LSRC = actlib.c arrlib.c biglib.c binlib.c blnlib.c bstlib.c chrlib.c cmdlib.c conlib.c dcllib.c \
       drwlib.c enulib.c fillib.c fltlib.c hshlib.c intlib.c itflib.c kbdlib.c lstlib.c pcslib.c \
       pollib.c prclib.c prglib.c reflib.c rfllib.c sctlib.c setlib.c soclib.c sqllib.c strlib.c \
       timlib.c typlib.c ut8lib.c
ESRC = exec.c doany.c objutl.c
ASRC = act_comp.c prg_comp.c analyze.c syntax.c token.c parser.c name.c type.c \
       expr.c atom.c object.c scanner.c literal.c numlit.c findid.c \
       error.c infile.c libpath.c symbol.c info.c stat.c fatal.c match.c
GSRC = syvarutl.c traceutl.c actutl.c executl.c blockutl.c \
       entutl.c identutl.c chclsutl.c arrutl.c
RSRC = arr_rtl.c bln_rtl.c bst_rtl.c chr_rtl.c cmd_rtl.c con_rtl.c dir_rtl.c drw_rtl.c fil_rtl.c \
       flt_rtl.c hsh_rtl.c int_rtl.c itf_rtl.c pcs_rtl.c set_rtl.c soc_rtl.c sql_rtl.c str_rtl.c \
       tim_rtl.c ut8_rtl.c heaputl.c numutl.c sigutl.c striutl.c
DSRC = big_rtl.c big_gmp.c cmd_unx.c dir_win.c dll_unx.c fil_unx.c pcs_unx.c pol_unx.c soc_none.c \
       sql_base.c sql_fire.c sql_lite.c sql_my.c sql_oci.c sql_odbc.c sql_post.c tim_unx.c
SRC = $(MSRC)
SEED7_LIB_SRC = $(RSRC) $(DSRC)
DRAW_LIB_SRC = gkb_rtl.c drw_dos.c gkb_x11.c
CONSOLE_LIB_SRC = kbd_rtl.c con_emc.c
COMP_DATA_LIB_SRC = typ_data.c rfl_data.c ref_data.c listutl.c flistutl.c typeutl.c datautl.c
COMPILER_LIB_SRC = $(PSRC) $(LSRC) $(ESRC) $(ASRC) $(GSRC)

s7: ../bin/s7.js ../prg/s7.js
	node ../bin/s7.js -l ../lib level
	@echo
	@echo "  Use 'make s7c' (with your make command) to create the compiler."
	@echo

s7c: ../bin/s7c.js ../prg/s7c.js
	@echo
	@echo "  Use 'make test' (with your make command) to check Seed7."
	@echo

../bin/s7.js: $(OBJ) $(ALL_S7_LIBS)
	$(CC) $(LDFLAGS) $(OBJ) $(ALL_S7_LIBS) $(SYSTEM_DRAW_LIBS) $(SYSTEM_CONSOLE_LIBS) $(SYSTEM_LIBS) $(ADDITIONAL_SYSTEM_LIBS) -o ../bin/s7.js

../prg/s7.js: ../bin/s7.js
	ln -s ../bin/s7.js ../prg
	ln -s ../bin/s7.wasm ../prg

../bin/s7c.js: ../prg/s7c.js
	cp -p ../prg/s7c.js ../bin
	cp -p ../prg/s7c.wasm ../bin

../prg/s7c.js: ../prg/s7c.sd7 $(ALL_S7_LIBS)
	node --stack_size=2048 ../bin/s7.js -l ../lib ../prg/s7c -l ../lib -b ../bin -O2 ../prg/s7c

sql_%.o: sql_%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDE_OPTIONS) -c $< -o $@

big_%.o: big_%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INCLUDE_OPTIONS) -c $< -o $@

clear: clean

clean:
	rm -f *.o ../bin/*.a ../bin/s7.js ../bin/s7.wasm ../bin/s7c.js ../bin/s7c.wasm
	rm -f ../prg/s7.js ../prg/s7.wasm ../prg/s7c.js ../prg/s7c.wasm
	rm -f depend macros chkccomp.h version.h setwpath wrdepend sudo
	@echo
	@echo "  Use 'make depend' (with your make command) to create the dependencies."
	@echo

distclean: clean
	cp level_bk.h level.h
	rm -f vers_emccl.h

test:
	node ../bin/s7.js -l ../lib ../prg/chk_all build
	@echo
	@echo "  Use 'sudo make install' (with your make command) to install Seed7."
	@echo

install: setwpath
	./setwpath add ../bin

uninstall: setwpath
	./setwpath remove ../bin

dep: depend

strip:
	strip ../bin/s7

chkccomp.h:
	echo "#define LIST_DIRECTORY_CONTENTS \"dir\"" >> chkccomp.h
	echo "#define MYSQL_LIBS \"-lmysqlclient\"" >> chkccomp.h
	echo "#define MYSQL_DLL \"libmysqlclient.so\"" >> chkccomp.h
	echo "#define MYSQL_USE_LIB" >> chkccomp.h
	echo "#define SQLITE_LIBS \"-lsqlite3\"" >> chkccomp.h
	echo "#define SQLITE_DLL \"libsqlite3.so\"" >> chkccomp.h
	echo "#define SQLITE_USE_LIB" >> chkccomp.h
	echo "#define POSTGRESQL_LIBS \"-lpq\"" >> chkccomp.h
	echo "#define POSTGRESQL_DLL \"libpq.so\",\"libpq.so.5\"" >> chkccomp.h
	echo "#define POSTGRESQL_USE_LIB" >> chkccomp.h
	echo "#define ODBC_LIBS \"-lodbc\"" >> chkccomp.h
	echo "#define ODBC_DLL \"libodbc.so\"" >> chkccomp.h
	echo "#define ODBC_USE_LIB" >> chkccomp.h
	echo "#define OCI_LIBS \"-lclntsh\"" >> chkccomp.h
	echo "#define OCI_DLL \"libclntsh.so\"" >> chkccomp.h
	echo "#define OCI_USE_DLL" >> chkccomp.h
	echo "#define FIRE_LIBS \"-lfbclient\"" >> chkccomp.h
	echo "#define FIRE_DLL \"libfbclient.so\"" >> chkccomp.h
	echo "#define FIRE_USE_DLL" >> chkccomp.h

version.h: chkccomp.h
	echo "#define PATH_DELIMITER '/'" > version.h
	echo "#define SEARCH_PATH_DELIMITER 0" >> version.h
	echo "#define AWAIT_WITH_NANOSLEEP" >> version.h
	echo "#define IMPLEMENT_PTY_WITH_PIPE2" >> version.h
	echo "#define USE_EGL" >> version.h
	echo "#define $(TERMINFO_OR_TERMCAP)" >> version.h
	echo "#define CONSOLE_UTF8" >> version.h
	echo "#define OS_STRI_UTF8" >> version.h
	echo "#define ESCAPE_SHELL_COMMANDS" >> version.h
	echo "#define OBJECT_FILE_EXTENSION \".o\"" >> version.h
	echo "#define LIBRARY_FILE_EXTENSION \".a\"" >> version.h
	echo "#define LINKED_PROGRAM_EXTENSION \".js\"" >> version.h
	echo "#define INTERPRETER_FOR_LINKED_PROGRAM \"node\"" >> version.h
	echo "#define MOUNT_NODEFS" >> version.h
	echo "#define EMULATE_ENVIRONMENT" >> version.h
	echo "#define DEFINE_SYSTEM_FUNCTION" >> version.h
	echo "#define C_COMPILER \"$(CC)\"" >> version.h
	echo "#define CALL_C_COMPILER_FROM_SHELL 1" >> version.h
	echo "#define CPLUSPLUS_COMPILER \"em++\"" >> version.h
	echo "#define GET_CC_VERSION_INFO \"$(GET_CC_VERSION_INFO)\"" >> version.h
	echo "#define CC_OPT_DEBUG_INFO \"-g\"" >> version.h
	echo "#define CC_OPT_NO_WARNINGS \"-w\"" >> version.h
	echo "#define CC_FLAGS \"\"" >> version.h
	echo "#define CC_ERROR_FILDES 2" >> version.h
	echo "#define LINKER_OPT_OUTPUT_FILE \"-o \"" >> version.h
	echo "#define LINKER_FLAGS \"$(LDFLAGS)\"" >> version.h
	echo "#define SYSTEM_LIBS \"$(SYSTEM_LIBS)\"" >> version.h
	echo "#define SYSTEM_CONSOLE_LIBS \"$(SYSTEM_CONSOLE_LIBS)\"" >> version.h
	echo "#define SYSTEM_DRAW_LIBS \"$(SYSTEM_DRAW_LIBS)\"" >> version.h
	$(GET_CC_VERSION_INFO) cc_vers.txt
	gcc chkccomp.c -o chkccomp
	./chkccomp version.h
	rm chkccomp
	rm cc_vers.txt
	rm -f ctest*.wasm
	echo "#define SEED7_LIB \"$(SEED7_LIB)\"" >> version.h
	echo "#define CONSOLE_LIB \"$(CONSOLE_LIB)\"" >> version.h
	echo "#define DRAW_LIB \"$(DRAW_LIB)\"" >> version.h
	echo "#define COMP_DATA_LIB \"$(COMP_DATA_LIB)\"" >> version.h
	echo "#define COMPILER_LIB \"$(COMPILER_LIB)\"" >> version.h
	gcc -o setpaths setpaths.c
	./setpaths "S7_LIB_DIR=$(S7_LIB_DIR)" "SEED7_LIBRARY=$(SEED7_LIBRARY)" >> version.h
	rm setpaths
	gcc wrdepend.c -o wrdepend
	cp version.h vers_emccl.h

depend: version.h
	./wrdepend $(CFLAGS) -M $(SRC) "> depend"
	./wrdepend $(CFLAGS) -M $(SEED7_LIB_SRC) ">> depend"
	./wrdepend $(CFLAGS) -M $(CONSOLE_LIB_SRC) ">> depend"
	./wrdepend $(CFLAGS) -M $(DRAW_LIB_SRC) ">> depend"
	./wrdepend $(CFLAGS) -M $(COMP_DATA_LIB_SRC) ">> depend"
	./wrdepend $(CFLAGS) -M $(COMPILER_LIB_SRC) ">> depend"
	@echo
	@echo "  Use 'make' (with your make command) to create the interpreter."
	@echo

level.h:
	node ../bin/s7.js -l ../lib level

../bin/$(SEED7_LIB): $(SEED7_LIB_OBJ)
	ar r ../bin/$(SEED7_LIB) $(SEED7_LIB_OBJ)

../bin/$(CONSOLE_LIB): $(CONSOLE_LIB_OBJ)
	ar r ../bin/$(CONSOLE_LIB) $(CONSOLE_LIB_OBJ)

../bin/$(DRAW_LIB): $(DRAW_LIB_OBJ)
	ar r ../bin/$(DRAW_LIB) $(DRAW_LIB_OBJ)

../bin/$(COMP_DATA_LIB): $(COMP_DATA_LIB_OBJ)
	ar r ../bin/$(COMP_DATA_LIB) $(COMP_DATA_LIB_OBJ)

../bin/$(COMPILER_LIB): $(COMPILER_LIB_OBJ)
	ar r ../bin/$(COMPILER_LIB) $(COMPILER_LIB_OBJ)

make7: ../bin/make7

../bin/make7: ../prg/make7.sd7 ../bin/s7c
	../bin/s7c -l ../lib -b ../bin -O2 ../prg/make7
	mv ../prg/make7 ../bin

calc7: ../bin/calc7

../bin/calc7: ../prg/calc7.sd7 ../bin/s7c
	../bin/s7c -l ../lib -b ../bin -O2 ../prg/calc7
	mv ../prg/calc7 ../bin

tar7: ../bin/tar7

../bin/tar7: ../prg/tar7.sd7 ../bin/s7c
	../bin/s7c -l ../lib -b ../bin -O2 ../prg/tar7
	mv ../prg/tar7 ../bin

ftp7: ../bin/ftp7

../bin/ftp7: ../prg/ftp7.sd7 ../bin/s7c
	../bin/s7c -l ../lib -b ../bin -O2 ../prg/ftp7
	mv ../prg/ftp7 ../bin

ftpserv: ../bin/ftpserv

../bin/ftpserv: ../prg/ftpserv.sd7 ../bin/s7c
	../bin/s7c -l ../lib -b ../bin -O2 ../prg/ftpserv
	mv ../prg/ftpserv ../bin

wc: $(SRC)
	@echo SRC:
	wc $(SRC)
	@echo SEED7_LIB_SRC:
	wc $(SEED7_LIB_SRC)
	@echo CONSOLE_LIB_SRC:
	wc $(CONSOLE_LIB_SRC)
	@echo DRAW_LIB_SRC:
	wc $(DRAW_LIB_SRC)
	@echo COMP_DATA_LIB_SRC:
	wc $(COMP_DATA_LIB_SRC)
	@echo COMPILER_LIB_SRC:
	wc $(COMPILER_LIB_SRC)

lint: $(SRC)
	lint -p $(SRC) $(SYSTEM_DRAW_LIBS) $(SYSTEM_CONSOLE_LIBS) $(SYSTEM_LIBS) $(ADDITIONAL_SYSTEM_LIBS)

lint2: $(SRC)
	lint -Zn2048 $(SRC) $(SYSTEM_DRAW_LIBS) $(SYSTEM_CONSOLE_LIBS) $(SYSTEM_LIBS) $(ADDITIONAL_SYSTEM_LIBS)

ifeq (depend,$(wildcard depend))
include depend
endif

ifeq (macros,$(wildcard macros))
include macros
endif
