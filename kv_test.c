#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "kv_store.h"

void usage() {
    printf("Usage: kv_test -n <number> [-f <name>] \n");
    printf("  -n <number> Test with the specified number of key value pairs.\n");
    printf("  -f <name> Test only the named function\n");
}


int main(int argc, char** argv) {
    int c = 0, num_pairs = 256;
    char *test_fname = NULL;
 

    srandom(210);
    while ((c = getopt(argc, argv, "n:f:")) != -1) {
        if (c == 'f') {
            test_fname = strdup(optarg);
        } else if (c == 'n') {
            num_pairs = atoi(optarg);
        } else {
            usage();
            exit(1);
        }
    }

    run_tests(num_pairs, test_fname);
    if (test_fname) {
        free(test_fname);
    }
    return 0;
}