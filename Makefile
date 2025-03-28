CC = gcc
CFLAGS = -Wall -g

LIBS = -lgsl -lgslcblas -lm -lsomnus_lab -llapacke -llapack -lblas


LIB_NAME = somnus_lab
LIB_SRC = somnus_qr.c
LIB_HDR = gen_matrix_somnus_data.h
LIB_OBJ = $(LIB_SRC:.c=.o)

MAIN_SRC = somnus_qr.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)

all: lib$(LIB_NAME).a main

lib$(LIB_NAME).a: $(LIB_OBJ)
	$(AR) rcs $@ $^

main: $(MAIN_OBJ) lib$(LIB_NAME).a
	$(CC) $(CFLAGS) -o $@ $^ -L. $(LIBS)

%.o: %.c $(LIB_HDR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJ) $(MAIN_OBJ) lib$(LIB_NAME).a main
