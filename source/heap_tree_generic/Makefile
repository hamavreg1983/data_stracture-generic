#heap tree
# Yuval Hamberg
# yuval.hamberg@gmail.com
# 23/3/2017

CC = gcc
HPATH = ../../header
LIBPATH = ../../lib
CFLAGS = -m32 -g -ansi -pedantic -Wall -I $(HPATH) -I $(LIBPATH)

OUTPUT_NAME = HEAP_TREE
OBJS = heap_tree_generic.o heap_tree_main.o
LIBSNEEDED = lib_vector_generic.a
LIBSCREATE = heap_tree_generic.a


.Phony : clean rebuild

# $(LIBSNEEDED)
# $(LIBPATH)/
$(OUTPUT_NAME) : $(OBJS) $(LIBPATH)/$(LIBSNEEDED)
	$(CC) $(CFLAGS) -o $(OUTPUT_NAME) $(OBJS) $(LIBPATH)/$(LIBSNEEDED)


heap_tree_generic.o : heap_tree_generic.c $(HPATH)/heap_tree_generic.h $(HPATH)/vector_generic.h $(HPATH)/Vector_Result.h $(LIBPATH)/$(LIBSNEEDED)
	$(CC) $(CFLAGS) -c heap_tree_generic.c 
	$(CC) $(CFLAGS) -c -fPIC -o ../../object/heap_tree_generic.o heap_tree_generic.c
# if the c file got updated, update the librey that based on it
	ar -cvrs $(LIBPATH)/$(LIBSCREATE) heap_tree_generic.o

heap_tree_main.o : heap_tree_main.c $(HPATH)/heap_tree_generic.h $(HPATH)/vector_generic.h $(HPATH)/Vector_Result.h $(LIBPATH)/$(LIBSNEEDED)
	$(CC) $(CFLAGS) -c heap_tree_main.c
	
$(LIBPATH)/$(LIBSNEEDED) : 
	$(MAKE) $(LIBNEEDED1) -C ../vector_generic/


clean:
	rm -f *.o
	rm -f *~
	rm -f $(OUTPUT_NAME)
	rm -f $(LIBPATH)/$(LIBSCREATE)

rebuild : clean HEAP_TREE
