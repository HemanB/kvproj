CC = gcc
CFLAGS = -Wall -g
FILES = kv_store.o kv_test.c .kv_store_util.o

all: kv_test

kv_store.o: kv_store.c
	@echo "Building kv_store.c"
	@$(CC) $(CFLAGS) -c $< -o $@

kv_test: $(FILES)
	@echo "Building kv_test"
	@$(CC) $(CFLAGS) -Wconversion $(FILES) -o $@

clean:
	@echo "Cleaning/resetting build files"
	@$(RM) kv_test kv_store.o
	@$(RM) tests/*_diff*.txt tests/*_actual*.txt
