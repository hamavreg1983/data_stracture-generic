# Makefile for data stracute
# dynamic libray
# Yuval Hamberg
# yuval.hamberg@gmail.com
# 30/04/2017

#EXE_NAME = VECTOR
#PROG_FILE = vector_generic
#TEST_FILE = vector_generic_main
LIBNEEDED1 = 
SO_NAME_REAl_VER = 1.0
SO_NAME_VER = 1

#OBJS = $(PROG_FILE).o $(TEST_FILE).o
LIBSCREATE = lib_DB.so
DYN_LIB_PATH = LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$(LIBPATH)" 
H_FILES = $(HMYPATH)/$(wildcard *.h)
# $(XXX)

CC = gcc
HMYPATH = ../header
LIBMYPATH = ../lib
OBJS_LIB = ../object
CFLAGS = $(bits) -g -ansi -pedantic -Wall -I $(HMYPATH) -I $(LIBMYPATH)

OBJS = $(OBJS_LIB)/vector_generic.o $(OBJS_LIB)/list_generic.o $(OBJS_LIB)/stack_generic.o $(OBJS_LIB)/heap_tree_generic.o $(OBJS_LIB)/hash_map.o $(OBJS_LIB)/queue_generic.o

.Phony : clean rebuild

$(LIBMYPATH)/$(LIBSCREATE) : $(LIBSCREATE)
	
$(LIBSCREATE) : $(OBJS) $(H_FILES)
	$(CC) $(CFLAGS) -shared -Wl,-rpath,${LIBMYPATH} -Wl,-soname,$(LIBSCREATE).$(SO_NAME_VER) -o $(LIBSCREATE).$(SO_NAME_REAl_VER) $(OBJS)
	
	# create links and move them so the real name would be only an option
	ln -sf $(LIBSCREATE).$(SO_NAME_REAl_VER) $(LIBSCREATE).$(SO_NAME_VER)
	ln -sf $(LIBSCREATE).$(SO_NAME_VER) $(LIBSCREATE)
	mv $(LIBSCREATE)* $(LIBMYPATH)
	
$(OBJS) : 
	make vector_generic.o -C vector_generic
	make list_generic.o -C list_generic_itr
	make stack_generic.o -C stack_generic
	make heap_tree_generic.o -C heap_tree_generic
	make hash_map.o -C hash_generic
	make queue_generic.o -C queue_generic


clean:
	rm -f *.o
	rm -f $(OBJS)
	rm -f *~
	#rm -f $(EXE_NAME)
	rm -f $(LIBMYPATH)/$(LIBSCREATE)*
	make clean -C vector_generic
	make clean -C list_generic_itr
	make clean -C stack_generic
	make clean -C heap_tree_generic
	make clean -C hash_generic
	make clean -C queue_generic


rebuild : clean $(EXE_NAME)


# find out if its a 64bit machine
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
	# do 64 bit stuff here, like set some CFLAGS
	bits = -m32
else
	# do 32 bit stuff here
	bits = 
endif


