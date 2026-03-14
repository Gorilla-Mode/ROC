#define UNITY_BUILD

#include "global_typedefs.c"
#include "celestial_body.c"
#include "error_enums.c"
#include "orbit.c"
#include "orbital_mechanics.c"
#include "planets.c"
#include <stdio.h>

int main(void)
{
    ResonantError O_Err = 0;
    LosError LOS_Err = 0;
    f64_t r1 = 100000;
    f64_t a1 = 250000;
    f64_t r2 = 200000;

    Orbit orbit1 = CreateOrbitCircularAlt(&Kerbol[MOHO], a1);
    Orbit orbit2 = CreateOrbitEllipse(&Kerbol[MOHO], r1, r2);
    Orbit resonantOrbit = CalcResonantOrbitProg(&orbit1, 3, &O_Err);
    if (O_Err != RES_SUCCESS)
    {
        (void)fprintf_s(stderr, "Error: %s\n", OrbitErrorToString(O_Err));
        return 0;
    }

    printf("delta v: %lf\n\n", DeltaVCircToEllip(&orbit1, &resonantOrbit, &O_Err));
    if (O_Err != RES_SUCCESS)
    {
        (void)fprintf_s(stderr, "Error: %s\n", OrbitErrorToString(O_Err));
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