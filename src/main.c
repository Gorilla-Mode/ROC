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
    f64_t a1 = 350000;
    f64_t r2 = 200000;

    Orbit orbit1 = CreateOrbit_circular_smj(&Planets[MOHO], a1);
    Orbit orbit2 = CreateOrbit_ellipse(&Planets[MOHO], r1, r2);

    printf("delta v: %lf\n", DeltaVCircToEllip(&orbit1, &orbit2));

    printf("Moho grav surf: %lf\n", Planets[MOHO].GravSurf);
    printf("Eve grav param: %lf\n", Planets[EVE].GravParam);

    return 0;
}