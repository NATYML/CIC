
/**********************Macros**************************/

#define NMAX 400000 
#define N_params 1000
/*-------------------------*/ 
#define L 0
#define N 1
#define delta_x 2 
#define v_celda 3
#define N_part 4
/********************Structures************************/

struct Particle{
  double xp,yp,zp,mp;
  };
struct Cell{
  double xc,yc,zc,mc;  
  };

/********************Libraries Used********************/
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "proto.h"