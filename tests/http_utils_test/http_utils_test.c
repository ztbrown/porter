#include <check.h>
#include <stdlib.h>

#include "http_utils_test.h"

#include "../../src/http_utils.h"
#include "../../lib/mock.h"

SIMULACRUM(int, send, 4, int, char*, int, int)

struct send_args_s {
    char *message;
    int socket;
    int length;
    int flags;
};
struct send_args_s send_args;

void send_callback(int socket, char *message, int length, int flags)
{
    send_args.message = malloc(length + 1);
    memcpy(send_args.message, message, length + 1);

    send_args.socket = socket;
    send_args.length = length;
    send_args.flags = flags;
}

static void setup()
{
    int success = 0;
    mock_set_return_value(&send_mock, &success);
}

static void teardown()
{
    mock_reset_call_count(&send_mock);
}

// getRequestType

START_TEST(it_gets_request_type_for_GET)
{
    // Arrange
    char *request_type = "GET";
    int actual = -1;

    // Act
    actual = getRequestType(request_type);

    // Assert
    ck_assert_int_eq(actual, 1);
}
END_TEST

START_TEST(it_gets_request_type_for_HEAD)
{
    // Arrange
    char *request_type = "HEAD";
    int actual = -1;

    // Act
    actual = getRequestType(request_type);

    // Assert
    ck_assert_int_eq(actual, 2);
}
END_TEST

START_TEST(it_gets_request_type_for_POST)
{
    // Arrange
    char *request_type = "POST";
    int actual = -1;

    // Act
    actual = getRequestType(request_type);

    // Assert
    ck_assert_int_eq(actual, 0);
}
END_TEST

START_TEST(it_returns_error_code_for_invalid_request)
{
    // Arrange
    char *request_type = "NOTHING";
    int actual = 0;

    // Act
    actual = getRequestType(request_type);

    // Assert
    ck_assert_int_eq(actual, -1);
}
END_TEST

// send_string

START_TEST(it_sends_string_over_connecting_socket)
{
    // Arrange
    char *message = "Hello World!";
    int connecting_socket = 0;

    mock_set_callback(&send_mock, &send_callback);

    // Act
    send_string(message, connecting_socket);

    // Assert
    ck_assert_int_eq(mock_get_call_count(&send_mock), 1);
    ck_assert_str_eq(send_args.message, message);
    ck_assert_int_eq(send_args.socket, connecting_socket);

    // Clean up
    free(send_args.message);
}
END_TEST


Suite *make_http_utils_test_suite()
{
    Suite *s;
    TCase *tc, *tc2;

    s = suite_create("Http Utils");
    tc = tcase_create("getRequestType");

    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_gets_request_type_for_GET);
    tcase_add_test(tc, it_gets_request_type_for_HEAD);
    tcase_add_test(tc, it_gets_request_type_for_POST);
    tcase_add_test(tc, it_returns_error_code_for_invalid_request);

    suite_add_tcase(s, tc);

    tc = tcase_create("send_string");
    tcase_add_checked_fixture(tc, &setup, &teardown);

    tcase_add_test(tc, it_sends_string_over_connecting_socket);

    suite_add_tcase(s, tc);

    return s;
}