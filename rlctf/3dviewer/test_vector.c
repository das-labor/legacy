#include <stdio.h>
#include "vector.h"

int main (int argc, char* argv[])
{
	vec3_t t = {17.0f, 10.0f, 5.0f};
	vec3_t u;

	printf("len: %lf\n", vec3len(t));
	vec3normalize (t);
	printf("normalized: [%lf,%lf,%lf]\n", t[0], t[1], t[2]);

	vec3xyortho (u, t);
	printf("orthogonal on the x-y plane: [%lf,%lf,%lf]\n", u[0], u[1], u[2]);
	
	vec3scale (u, 5.0f);
	printf("orthogonal vector *5 : [%lf,%lf,%lf]\n", u[0], u[1], u[2]);
	
	vec3inv (u);
	printf("...inverted : [%lf,%lf,%lf]\n", u[0], u[1], u[2]);
}
