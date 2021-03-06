#include <check.h>
#include <stdlib.h>

#include "http_utils_test.h"

int main()
{
    int number_failed;
    SRunner *sr;

    sr = srunner_create(make_http_utils_test_suite());

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed==0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
