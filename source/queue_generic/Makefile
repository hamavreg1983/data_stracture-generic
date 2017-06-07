# Yuval Hamberg
# yuval.hamberg@gmail.com
# 30/4/2017
# Queue-Generic


CC = gcc 
HPATH = ../../header
LIBPATH = ../../lib
CFLAGS = $(bits) -g -ansi -pedantic -Wall -I $(HPATH) -I $(LIBPATH)

EXE_NAME = QUEUE
PROG_FILE = queue_generic
TEST_FILE = queue_generic_test
LIBNEEDED1 = lib_list_generic.a 

OBJS = $(PROG_FILE).o $(TEST_FILE).o
LIBSCREATE = lib_$(PROG_FILE).a
DYN_LIB_PATH = LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$(LIBPATH)" 

.Phony : clean rebuild

$(EXE_NAME) : $(OBJS) $(LIBPATH)/$(LIBNEEDED1) 
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(OBJS) $(LIBPATH)/$(LIBNEEDED1) -lm


$(PROG_FILE).o : $(PROG_FILE).c $(HPATH)/queue_generic.h $(HPATH)/list.h $(HPATH)/list_result.h $(HPATH)/defines.h
	$(CC) $(CFLAGS) -c $(PROG_FILE).c -lm
	$(CC) $(CFLAGS) -c -fPIC -o ../../object/$(PROG_FILE).o $(PROG_FILE).c -lm
# if the c file got updated, update the librey that based on it. do not include required lib.
	ar -cvrs $(LIBPATH)/$(LIBSCREATE) $(PROG_FILE).o

$(TEST_FILE).o : $(TEST_FILE).c $(PROG_FILE).o $(HPATH)/mu_test.h
	$(CC) $(CFLAGS) -c $(TEST_FILE).c

$(LIBPATH)/$(LIBSCREATE) : $(LIBSCREATE)

$(LIBSCREATE) : $(PROG_FILE).c
	ar -cvrs $(LIBPATH)/$(LIBSCREATE) $(PROG_FILE).o

$(LIBPATH)/$(LIBNEEDED1) : $(LIBNEEDED1)

$(LIBNEEDED1) : ../list_generic_itr/list_generic.c $(HPATH)/list*.h
	$(MAKE) $(LIBNEEDED1) -C ../list_generic_itr/



clean:
	rm -f *.o
	rm -f *~
	rm -f $(EXE_NAME)
	rm -f $(LIBPATH)/$(LIBSCREATE)
	#rm -f $(LIBPATH)/$(LIBNEEDED1)

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
