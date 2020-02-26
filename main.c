#include "utest.h"

#include <stdio.h>
#include <stdint.h>

int main (int argc, char **argv) {
    int ret = test_case ();
    if (ALLOC_FREE_OK != ret) {
        printf ("errorCode: [%s] \n\n", to_name_str (ret));
    } else {
        printf ("\n uTestOk!\n");
    }
    printf ("Done!\n\n");
    getchar ();
    getchar ();
    return 0;
}
