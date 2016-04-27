#ifdef SINGLE
#define REAL float
#else /* not SINGLE */
#define REAL double
#endif /* not SINGLE */
extern "C" 
{
#include "triangle.h"
}

int main(){
	struct triangulateio in, mid, out, vorout;
  triangulate("pczAevn", &in, &mid, &vorout);


}