# Yuval Hamberg
# yuval.hamberg@gmail.com
# 26/3/2017
# HashMap-Generic


CC = gcc 
HPATH = ../../header
LIBPATH = ../../lib
CFLAGS = -m32 -g -ansi -pedantic -Wall -I $(HPATH) -I $(LIBPATH)

EXE_NAME = HASHMAP
PROG_FILE = hash_map
TEST_FILE = hash_test_main
LIBNEEDED1 = lib_list_generic.a 

OBJS = $(PROG_FILE).o $(TEST_FILE).o
LIBSCREATE = lib_$(PROG_FILE).a


.Phony : clean rebuild run

$(EXE_NAME) : $(OBJS) $(LIBPATH)/$(LIBNEEDED1) 
	$(CC) $(CFLAGS) -o $(EXE_NAME) $(OBJS) $(LIBPATH)/$(LIBNEEDED1) -lm


$(PROG_FILE).o : $(PROG_FILE).c $(HPATH)/hash_map.h $(HPATH)/list.h $(HPATH)/list_result.h $(HPATH)/defines.h
	$(CC) $(CFLAGS) -c $(PROG_FILE).c -lm
	$(CC) $(CFLAGS) -c -fPIC -o ../../object/$(PROG_FILE).o $(PROG_FILE).c -lm
# if the c file got updated, update the librey that based on it. do not include required lib.
	ar -cvrs $(LIBPATH)/$(LIBSCREATE) $(PROG_FILE).o

$(TEST_FILE).o : $(TEST_FILE).c $(PROG_FILE).o $(HPATH)/mu_test.h
	$(CC) $(CFLAGS) -c $(TEST_FILE).c

$(LIBPATH)/$(LIBSCREATE) : $(PROG_FILE).c
	ar -cvrs $(LIBPATH)/$(LIBSCREATE) $(PROG_FILE).o

$(LIBPATH)/$(LIBNEEDED1) : ../list_generic_itr/list_generic.c $(HPATH)/list*.h
	$(MAKE) $(LIBNEEDED1) -C ../list_generic_itr/

run: $(EXE_NAME)
	./$(EXE_NAME)

clean:
	rm -f *.o
	rm -f *~
	rm -f $(EXE_NAME)
	rm -f $(LIBPATH)/$(LIBSCREATE)
	#rm -f $(LIBPATH)/$(LIBNEEDED1)

rebuild : clean $(EXE_NAME)
