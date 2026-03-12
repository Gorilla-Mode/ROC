#define UNITY_BUILD

#include <stdio.h>
#include "global_typedefs.c"
#include "error_enums.c"
#include "celestial_body.c"
#include "planets.c"
#include "orbit.c"
#include "orbital_mechanics.c"

int main(void)
{
    OrbitError O_Err;
    f64_t r1 = 100000;
    f64_t a1 = 9646663.0 - (f64_t)Kerbol[MOHO].EqRadiusM;
    f64_t r2 = 200000;

    Orbit orbit1 = CreateOrbitCircularAlt(&Kerbol[MOHO], a1);
    Orbit orbit2 = CreateOrbitEllipse(&Kerbol[MOHO], r1, r2);
    Orbit resonantOrbit = CalcResonantOrbitProg(&orbit1, 3, &O_Err);
    
    if (O_Err == ORBIT_SUCCESS)
    {
        printf("delta v: %lf\n\n", DeltaVCircToEllip(&orbit1, &resonantOrbit));

        printf("Orbital period orbit 1: %lf\n", orbit1.OPeriod(&orbit1));
        printf("Orbital periapsis orbit 1: %lf\n", orbit1.PeriapsisHeight(&orbit1));
        printf("Orbital apoapsis orbit 1: %lf\n\n", orbit1.ApoapsisHeight(&orbit1));
        printf("Orbital period orbit res: %lf\n", resonantOrbit.OPeriod(&resonantOrbit));
        printf("Orbital periapsis orbit res: %lf\n", resonantOrbit.PeriapsisHeight(&resonantOrbit));
        printf("Orbital apoapsis orbit res: %lf\n", resonantOrbit.ApoapsisHeight(&resonantOrbit));

        printf("Moho grav surf: %lf\n", Kerbol[MOHO].GravSurf);
        printf("Eve grav param: %lf\n", Kerbol[EVE].GravParam);
    }
    return 0;
}