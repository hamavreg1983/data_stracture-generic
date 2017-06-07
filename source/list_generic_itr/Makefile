# Yuval Hamberg
# yuval.hamberg@gmail.com
# 30/4/2017
# LIST

CC = gcc
HPATH = ../../header
LIBPATH = ../../lib
CFLAGS = $(bits) -g -ansi -pedantic -Wall -I $(HPATH)

EXE_NAME = LIST
PROG_FILE = list_generic
TEST_FILE = list_generic_main
LIBNEEDED1 = 

OBJS = $(PROG_FILE).o $(TEST_FILE).o
LIBSNEEDED =  
LIBSCREATE = lib_$(PROG_FILE).a

# $(X)
.Phony : clean rebuild

$(EXE_NAME) : $(OBJS) $(LIBPATH)/$(LIBSNEEDED) 
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(OBJS) 

$(PROG_FILE).o : $(PROG_FILE).c $(HPATH)/list*.h $(HPATH)/defines.h 
	$(CC) $(CFLAGS) -c $(PROG_FILE).c
	$(CC) $(CFLAGS) -c -fPIC -o ../../object/$(PROG_FILE).o $(PROG_FILE).c
	ar -cvrs $(LIBPATH)/$(LIBSCREATE) $(PROG_FILE).o

$(TEST_FILE).o : $(TEST_FILE).c $(HPATH)/list*.h
	$(CC) $(CFLAGS) -c $(TEST_FILE).c

# if the c file got updated, update the librey that based on it	
$(LIBPATH)/$(LIBSCREATE) : $(LIBSCREATE)

$(LIBSCREATE) : $(PROG_FILE).c
	ar -cvrs $(LIBPATH)/$(LIBSCREATE) $(PROG_FILE).o

clean:
	rm -f *.o
	rm -f *~
	rm -f LIST
	rm -f LIST_local
	rm -f $(LIBPATH)/$(LIBSCREATE)

rebuild : clean LIST

# find out if its a 64bit machine
LBITS := $(shell getconf LONG_BIT)
ifeq ($(LBITS),64)
	# do 64 bit stuff here, like set some CFLAGS
	bits = -m32
else
	# do 32 bit stuff here
	bits = 
endif
