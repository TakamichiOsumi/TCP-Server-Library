#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TcpClientUtil.h"

static void
compare_strings(char *result, char *expected){
    if (strncmp(result, expected, strlen(expected)) != 0){
	fprintf(stderr, "debug : the result was '%s', but expected was '%s'\n",
		result, expected);
	exit(-1);
    }else{
	printf("debug : returned the expected result = '%s'\n",
	       expected);
    }
}

static void
calc_digits(void){
    char buf[HDR_LEN + 1];

    UT_get_strlen_as_HDR_string(buf, "");
    compare_strings(buf, "");

    UT_get_strlen_as_HDR_string(buf, "1");
    compare_strings(buf, "01");

    UT_get_strlen_as_HDR_string(buf, "12");
    compare_strings(buf, "02");

    UT_get_strlen_as_HDR_string(buf, "123456789");
    compare_strings(buf, "09");

    UT_get_strlen_as_HDR_string(buf,
				"1234567890"
				"12345");
    compare_strings(buf, "15");

    UT_get_strlen_as_HDR_string(buf,
				"12345678901234567890"
				"12345678901234567890"
				"12345678901234567890"
				"12345678901234567890"
				"1234567890123456789");
    compare_strings(buf, "99");

    UT_get_strlen_as_HDR_string(buf,
				"12345678901234567890"
				"12345678901234567890"
				"12345678901234567890"
				"12345678901234567890"
				"12345678901234567890");
    compare_strings(buf, "");
}

int
main(int argc, char **argv){

    calc_digits();

    return 0;
}
