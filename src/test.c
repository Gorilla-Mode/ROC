#define UNITY_BUILD

#include "windows.h"
#include "global_typedefs.c"
#include "celestial_body.c"
#include "orbit.c"
#include "error.c"
#include "orbital_mechanics.c"
#include "planets.c"
#include <stdio.h>

//region Macros

#define EPSILON        1e-9
#define TEXT_GREEN     "\x1b[32m"
#define TEXT_RED       "\x1b[31m"
#define TEXT_RESET     "\x1b[0m"

#define CREATE_TEST_ORBIT_ALT(alt_value, body_index) \
    f64_t alt = alt_value; \
    LosError LosErr = 0; \
    OrbitError OrbErr = 0; \
    ResonantError ResErr = 0; \
    Orbit Orbit = CreateOrbitCircularAlt(&Kerbol[body_index], alt, &OrbErr)

//endregion

//region String functions

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

//endregion

//region Asserts

static bool assert_true(bool condition, const char* func_name, const char* msg)
{
    if (!condition) {

        printf(TEXT_RED "FAIL: %s - %s\n" TEXT_RESET, get_test_name(func_name), msg);
        return false;
    }
    printf(TEXT_GREEN "PASS: %s" TEXT_RESET "\n", get_test_name(func_name));
    return true;
}

static bool assert_false(bool condition, const char* func_name, const char* msg)
{
    if (!condition) {

        printf(TEXT_GREEN "PASS: %s" TEXT_RESET "\n", get_test_name(func_name));
        return true;
    }
    printf(TEXT_RED "FAIL: %s - %s\n" TEXT_RESET, get_test_name(func_name), msg);
    return false;
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

//endregion

//region Test ValidateLOSParams

bool TEST_Error_ValidateLOSParams_MissingPrimary_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(150000, KERBIN);
    Orbit.PrimaryBody = nullptr;

    bool res = ValidateLosParams(&Orbit, 10, &LosErr);

    return assert_false(res, __func__, "Missing primary error is not triggered");
}

bool TEST_Error_ValidateLOSParams_EccentricOrbit_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(150000, KERBIN);
    Orbit.Eccentricity = 0.5;

    bool res = ValidateLosParams(&Orbit, 10, &LosErr);

    return assert_false(res, __func__, "Eccentric orbit error is not triggered");
}

bool TEST_Error_ValidateLOSParams_InvalidSatCount_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(150000, KERBIN);

    bool res = ValidateLosParams(&Orbit, 2, &LosErr);

    return assert_false(res, __func__, "Invalid satellite count error is not triggered");
}

bool TEST_Error_ValidateLOSParams_Valid_ReturnsTrue()
{
    CREATE_TEST_ORBIT_ALT(150000, KERBIN);

    bool res = ValidateLosParams(&Orbit, 5, &LosErr);

    return assert_true(res, __func__, "Valid orbit triggers error");
}

//endregion

//region Test ValidateOrbit

bool TEST_Error_ValidateOrbit_MissingPrimary_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(150000, KERBIN);
    Orbit.PrimaryBody = nullptr;

    bool res = ValidateOrbit(&Orbit, &OrbErr);

    return assert_false(res, __func__, "Missing primary error is not triggered");
}

bool TEST_Error_ValidateOrbit_SurfIntersect_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(1000, KERBIN);

    bool res = ValidateOrbit(&Orbit, &OrbErr);

    return assert_false(res, __func__, "Orbit intersecting surface error is not triggered");
}

bool TEST_Error_ValidateOrbit_OutsideSOI_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(150000000, KERBIN);

    bool res = ValidateOrbit(&Orbit, &OrbErr);

    return assert_false(res, __func__, "Orbit outside SOI error is not triggered");
}

bool TEST_Error_ValidateOrbit_BelowAtmosphere_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(50000, KERBIN);

    bool res = ValidateOrbit(&Orbit, &OrbErr);

    return assert_false(res, __func__, "Orbit below atmosphere error is not triggered");
}

bool TEST_Error_ValidateOrbit_Valid_ReturnsTrue()
{
    CREATE_TEST_ORBIT_ALT(150000, KERBIN);

    bool res = ValidateOrbit(&Orbit, &OrbErr);

    return assert_true(res, __func__, "Valid orbit triggers error");
}

//endregion

//region Test ValidateResParams

bool Test_Error_ValidateResOrbParams_OutsideSOI_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(100000, KERBIN);
    Orbit.SMajorAxisM += 100000000;

    bool res  = ValidateResOrbParams(&Orbit, 10, &ResErr);

    return assert_false(res, __func__, "Orbit outside SOI error is not triggered");
}

bool Test_Error_ValidateResOrbParams_InvalidSatCount_ReturnsFalse()
{
    CREATE_TEST_ORBIT_ALT(150000, KERBIN);

    bool res = ValidateResOrbParams(&Orbit, 2, &ResErr);

    return assert_false(res, __func__, "Invalid satellite count error is not triggered");
}

bool Test_Error_ValidateResOrbParams_Valid_ReturnsTrue()
{
    CREATE_TEST_ORBIT_ALT(150000, KERBIN);

    bool res = ValidateResOrbParams(&Orbit, 10, &ResErr);

    return assert_true(res, __func__, "Valid orbit triggers error");
}

//endregion

//region Test collection

typedef bool (*TestFunc)(void);

static struct
{
    TestFunc func;
} tests[] = {
    {TEST_Error_ValidateLOSParams_MissingPrimary_ReturnsFalse},
    {TEST_Error_ValidateLOSParams_EccentricOrbit_ReturnsFalse},
    {TEST_Error_ValidateLOSParams_InvalidSatCount_ReturnsFalse},
    {TEST_Error_ValidateLOSParams_Valid_ReturnsTrue},
    {TEST_Error_ValidateOrbit_MissingPrimary_ReturnsFalse},
    {TEST_Error_ValidateOrbit_SurfIntersect_ReturnsFalse},
    {TEST_Error_ValidateOrbit_OutsideSOI_ReturnsFalse},
    {TEST_Error_ValidateOrbit_BelowAtmosphere_ReturnsFalse},
    {TEST_Error_ValidateOrbit_Valid_ReturnsTrue},
    {Test_Error_ValidateResOrbParams_OutsideSOI_ReturnsFalse},
    {Test_Error_ValidateResOrbParams_InvalidSatCount_ReturnsFalse},
    {Test_Error_ValidateResOrbParams_Valid_ReturnsTrue}
};

//endregion

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