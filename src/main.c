#define UNITY_BUILD

#include <stdio.h>
#include "global_typedefs.c"
#include "celestial_body.c"
#include "planets.c"
#include "orbit.c"
#include "orbital_mechanics.c"

int main(void)
{
    f64_t r1 = 100000;
    f64_t a1 = 150000;
    f64_t r2 = 200000;

    Orbit orbit1 = CreateOrbit(&Planets[MOHO], r1, a1);
    Orbit orbit2 = CreateOrbit(&Planets[MOHO], r1, r2);

    printf("delta v: %lf\n", DeltaVCircToEllip(&orbit1, &orbit2));

    printf("Moho grav surf: %lf\n", Planets[MOHO].GravSurf);
    printf("Eve grav param: %lf\n", Planets[EVE].GravParam);

    return 0;
}