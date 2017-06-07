# Makefile for vector_generic
# Yuval Hamberg
# yuval.hamberg@gmail.com
# 29/03/2017

EXE_NAME = VECTOR
PROG_FILE = vector_generic
TEST_FILE = vector_generic_main
LIBNEEDED1 = 

OBJS = $(PROG_FILE).o $(TEST_FILE).o
LIBSCREATE = lib_$(PROG_FILE).a

CC = gcc
HMYPATH = ../../header
LIBMYPATH = ../../lib
CFLAGS = -g -m32 -ansi -pedantic -Wall -I $(HMYPATH)

OBJS = $(PROG_FILE).o $(TEST_FILE).o

.Phony : clean rebuild


$(EXE_NAME) : $(OBJS) $(LIBMYPATH)/$(LIBSCREATE)
	$(CC) $(CFLAGS) -o VECTOR $(OBJS)

$(PROG_FILE).o : $(PROG_FILE).c $(HMYPATH)/vector_generic.h $(HMYPATH)/Vector_Result.h
	$(CC) $(CFLAGS) -c -o $(PROG_FILE).o $(PROG_FILE).c
	$(CC) $(CFLAGS) -c -fPIC -o ../../object/$(PROG_FILE).o $(PROG_FILE).c
	ar -cvrs $(LIBMYPATH)/$(LIBSCREATE) $(PROG_FILE).o

$(LIBMYPATH)/$(LIBSCREATE) : $(LIBSCREATE)
	
$(LIBSCREATE) : $(PROG_FILE).o
	ar -cvr $(LIBMYPATH)/$(LIBSCREATE) $(PROG_FILE).o

$(TEST_FILE).o : $(TEST_FILE).c $(HMYPATH)/vector_generic.h
	$(CC) $(CFLAGS) -c -o $(TEST_FILE).o $(TEST_FILE).c



clean:
	rm -f *.o
	rm -f *~
	rm -f $(EXE_NAME)
	rm -f $(LIBMYPATH)/$(LIBSCREATE)

rebuild : clean $(EXE_NAME)
