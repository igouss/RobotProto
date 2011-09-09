#ifndef vector_h
#define vector_h

#include <inttypes.h>

typedef struct vector
{
	int16_t x;
	int16_t y;
	int16_t z;
} vector;

extern void vector_cross(const vector *a, const vector *b, vector *out);
extern float vector_dot(const vector *a,const vector *b);
extern void vector_normalize(vector *a);
#endif
