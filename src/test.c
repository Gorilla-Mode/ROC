#define UNITY_BUILD

#include "windows.h"
#include "global_typedefs.c"
#include "celestial_body.c"
#include "error.h"
#include "orbit.c"
#include "error.c"
#include "orbital_mechanics.c"
#include "planets.c"
#include <stdio.h>

#define EPSILON        1e-9
#define TEXT_GREEN     "\x1b[32m"
#define TEXT_RED "\x1b[31m"
#define TEXT_RESET     "\x1b[0m"




/**
 * Returns string after the first two underscores in the function name. As per naming convention,
 * the tests start with "TEST_FileWhereFunctionIsDefined", which is not necessary for output.
 * Who made that convention? ME, BITCH!
 * @param func_name
 * @return Chopped string
 */
static const char* get_test_name(const char* func_name)
{
    int underscore_count = 0;
    for (int i = 0; func_name[i] != '\0'; i++)
    {
        if (func_name[i] != '_')
        {
            continue;
        }

        underscore_count++;

        if (underscore_count != 2)
        {
           continue;
        }

        return &func_name[i + 1];
    }
    return func_name;
}

static bool assert_true(bool condition, const char* func_name, const char* msg)
{
    if (!condition) {

        printf(TEXT_RED "FAIL: %s - %s\n" TEXT_RESET, get_test_name(func_name), msg);
        return false;
    }
    printf(TEXT_GREEN "PASS: %s" TEXT_RESET "\n", get_test_name(func_name));
    return true;
}

static bool assert_equal_double(f64_t actual, f64_t expected, const char* func_name, const char* msg)
{
    if (fabs(actual - expected) > EPSILON)
    {
        printf(TEXT_RED "FAIL: %s - %s (expected %.9f, got %.9f)\n" TEXT_RESET,
            get_test_name(func_name), msg, expected, actual);
        return false;
    }
        printf(TEXT_GREEN "PASS: %s" TEXT_RESET "\n", get_test_name(func_name));
    return true;
}

static bool assert_greater_than(f64_t actual, f64_t threshold, const char* func_name, const char* msg)
{
    if (actual <= threshold)
    {
        printf(TEXT_RED"FAIL: %s - %s (expected > %.9f, got %.9f)\n" TEXT_RESET,
            get_test_name(func_name), msg, threshold, actual);
        return false;
    }
    printf(TEXT_GREEN "PASS: %s" TEXT_RESET "\n", get_test_name(func_name));
    return true;
}

bool TEST_Error_ValidateLOSParams_MissingPrimary_ReturnsFalse()
{
    f64_t alt = 150000;
    LosError LosErr;
    OrbitError OrbErr;
    Orbit OrbitKerbin = CreateOrbitCircularAlt(&Kerbol[KERBIN], alt, &OrbErr);
    OrbitKerbin.PrimaryBody = nullptr;

    bool res = ValidateLosParams(&OrbitKerbin, 10, &LosErr);

    return assert_true(!res, __func__, "Missing primary error is not triggered");
}

typedef bool (*TestFunc)(void);

static struct
{
    TestFunc func;
} tests[] = {
    {TEST_Error_ValidateLOSParams_MissingPrimary_ReturnsFalse},
};

int main(void)
{
    //Enable ansi escape sequence in windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    size_t testSuccessCount = 0;
    size_t testFailureCount = 0;

    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++)
    {
        if (tests[i].func())
        {
            testSuccessCount++;
            continue;
        }
        testFailureCount++;
    }

    printf("\nTests completed:     %zu/%zu\n", testSuccessCount + testFailureCount, sizeof(tests) / sizeof(tests[0]));
    printf("  Test success rate: %.1f%%\n", ((f32_t)testSuccessCount / (f32_t)(testSuccessCount + testFailureCount)) * 100);
    printf(TEXT_GREEN "  Tests passed:      %zu\n" TEXT_RESET, testSuccessCount);
    printf(TEXT_RED"  Tests failed:      %zu\n" TEXT_RESET, testFailureCount);
    return 0;
}