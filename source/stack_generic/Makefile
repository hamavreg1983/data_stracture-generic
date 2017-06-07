# Yuval Hamberg
# yuval.hamberg@gmail.com
# 26/3/2017
# STACK


CC = gcc
HPATH = ../../header
LIBPATH = ../../lib
CFLAGS = -g -m32 -ansi -pedantic -Wall -I $(HPATH)

EXE_NAME = STACK
PROG_FILE = stack_generic
TEST_FILE = stack_generic_test
LIBNEEDED1 = lib_list_generic.a 

OBJS = $(PROG_FILE).o $(TEST_FILE).o
LIBSCREATE = lib_$(PROG_FILE).a


.Phony : clean rebuild

$(EXE_NAME) : $(OBJS) $(LIBPATH)/$(LIBNEEDED1) 
	$(CC) $(CFLAGS) -o $(EXE_NAME)_local $(OBJS) $(LIBPATH)/$(LIBNEEDED1)
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(OBJS) $(LIBPATH)/lib_DB.so -lm


$(PROG_FILE).o : $(PROG_FILE).c $(HPATH)/stack_generic.h $(HPATH)/list.h $(HPATH)/list_result.h
	$(CC) $(CFLAGS) -c $(PROG_FILE).c
	$(CC) $(CFLAGS) -c -fPIC -o ../../object/$(PROG_FILE).o $(PROG_FILE).c
# if the c file got updated, update the librey that based on it. do not include required lib.
	ar -cvrs $(LIBPATH)/$(LIBSCREATE) $(PROG_FILE).o

$(TEST_FILE).o : $(TEST_FILE).c $(PROG_FILE).o $(HPATH)/mu_test.h
	$(CC) $(CFLAGS) -c $(TEST_FILE).c

$(LIBPATH)/$(LIBNEEDED1) : ../list_generic_itr/list_generic.c $(HPATH)/list*.h
	$(MAKE) $(LIBNEEDED1) -C ../list_generic_itr/



clean:
	rm -f *.o
	rm -f *~
	rm -f $(EXE_NAME)
	rm -f $(LIBPATH)/$(LIBSCREATE)
	#rm -f $(LIBPATH)/$(LIBNEEDED1)

rebuild : clean $(EXE_NAME)
