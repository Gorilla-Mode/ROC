#define UNITY_BUILD

#include "global_typedefs.c"
#include "celestial_body.c"
#include "error.h"
#include "orbit.c"
#include "error.c"
#include "orbital_mechanics.c"
#include "planets.c"
#include <stdio.h>

int main(void)
{
    ResonantError Res_Err = 0;
    LosError LOS_Err = 0;
    OrbitError Orbit_Err = 0;
    f64_t r1 = 100000;
    f64_t a1 = 550000;
    f64_t r2 = 200000;

    Orbit orbit1 = CreateOrbitCircularAlt(&Kerbol[KERBIN], a1, &Orbit_Err);
    if (Orbit_Err != ORBIT_SUCCESS)
    {
        (void)fprintf_s(stderr, "Error: %s\n", OrbitErrorToString(Orbit_Err));
        return 0;
    }

    Orbit orbit2 = CreateOrbitEllipse(&Kerbol[MOHO], r1, r2, &Orbit_Err);
    if (Orbit_Err != ORBIT_SUCCESS)
    {
        (void)fprintf_s(stderr, "Error: %s\n", OrbitErrorToString(Orbit_Err));
        return 0;
    }

    Orbit resonantOrbit = CalcResonantOrbitRetr(&orbit1, 3, &Res_Err);
    if (Res_Err != RES_SUCCESS)
    {
        (void)fprintf_s(stderr, "Error: %s\n", ResonantErrorToString(Res_Err));
        return 0;
    }

    printf("delta v: %lf\n\n", DeltaVCircToEllip(&orbit1, &resonantOrbit, &Res_Err));
    if (Res_Err != RES_SUCCESS)
    {
        (void)fprintf_s(stderr, "Error: %s\n", ResonantErrorToString(Res_Err));
        return 0;
    }

    printf("Orbital period orbit 1: %lf\n", orbit1.OPeriod(&orbit1));
    printf("Orbital periapsis orbit 1: %lf\n", orbit1.PeriapsisHeight(&orbit1));
    printf("Orbital apoapsis orbit 1: %lf\n\n", orbit1.ApoapsisHeight(&orbit1));
    printf("Orbital period orbit res: %lf\n", resonantOrbit.OPeriod(&resonantOrbit));
    printf("Orbital periapsis orbit res: %lf\n", resonantOrbit.PeriapsisHeight(&resonantOrbit));
    printf("Orbital apoapsis orbit res: %lf\n", resonantOrbit.ApoapsisHeight(&resonantOrbit));
    printf("Line of sight check: %d\n", LineofSight(&orbit1, 3, &LOS_Err));
    printf("Atmospheric occolusion: %d\n", AtmosphericOccusion(&orbit1, 3, &LOS_Err));

    printf("Moho grav surf: %lf\n", Kerbol[MOHO].GravSurf);
    printf("Eve grav param: %lf\n", Kerbol[EVE].GravParam);

    return 0;
}