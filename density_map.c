#include "allvars.h"

/*************************************************************/
//Grid to construct Cells

int Grid(struct Cell *struct_array, float *prm){
int N_celda;

for (int n = 0; n < prm[N]; n++){
	for (int m = 0; m < prm[N]; m++){
		for (int l= 0; l< prm[N] ;l++){
			N_celda = l+prm[N]*(m+prm[N]*n);
			struct_array[N_celda].xc = n*prm[delta_x];
			struct_array[N_celda].yc = m*prm[delta_x];
			struct_array[N_celda].zc = l*prm[delta_x];
			}
		}
	}
return 0;
}

/*************************************************************/
/*************************************************************/

		//Find Cell where particle is located

int locate_cell(double xp, double yp, double zp, int *n, float *prm ){

	//Numbers in order to find the cell
	//Counters in x,y y z
	int i,j,k;

	/* Counter on x taking into account
	the periodical conditions on x */
	if ( (xp+prm[delta_x]) > prm[L] ){
	i = prm[N]-1;
	}
	else if ( (xp-prm[delta_x]) < 0 ){
	i = 0;
	}
	else{
	i = abs( floor( xp / prm[L] * prm[N] ) );
	}
	/* Counter on y taking into account
	the periodical conditions on y */
	if ( (yp+prm[delta_x]) > prm[L] ){
	j = prm[N]-1;
	}
	else if ( (yp-prm[delta_x]) < 0 ){
	j = 0;
	}
	else{
	j = abs( floor( yp / prm[L] * prm[N] ) );
	}
	/* Counter on z taking into account
	the periodical conditions on z */
	if ( (zp+prm[delta_x]) > prm[L] ){
	k = prm[N]-1;
	}	
	else if ( (zp-prm[delta_x]) < 0 ){
	k = 0;
	}
	else{
	k = abs( floor( zp / prm[L] * prm[N] ) );
	}
	*n = k+prm[N]*(j+prm[N]*i);
	return 0;
	}

/*************************************************************/
/*************************************************************/
			//Cloud in Cell Method
int CIC(struct Particle *parts, struct Cell *cells,float *prm ){
	
	int n[9],ll;
	double frac[8];
	double x_temp,y_temp,z_temp, frac_temp; 
	double xc_temp, yc_temp, zc_temp; 

	for (int i = 0; i < (int) prm[N_part]; i++){
	//int i = 133844;
	//int i = 173702;
	//int i = 231740;	
				
			/*Cell centered in the particle: cells[n0]
      				Cell of the particle*/ 
	locate_cell( parts[i].xp, parts[i].yp, parts[i].zp, &n[0], prm );
	
	//Corner of the cell that is used to find the volume
						/*--- X corner ---*/
	if ( parts[i].xp > cells[n[0]].xc + prm[delta_x]/2.0 ){
		 xc_temp = cells[n[0]].xc + prm[delta_x];
		}
	else { xc_temp = cells[n[0]].xc; }
						/*--- Y corner ---*/
	if ( parts[i].yp > cells[n[0]].yc + prm[delta_x]/2.0 ){
		yc_temp = cells[n[0]].yc + prm[delta_x];
		}
	else { yc_temp = cells[n[0]].yc; }
   					   /*--- Z corner ---*/
	if ( parts[i].zp > cells[n[0]].zc + prm[delta_x]/2.0 ){
		zc_temp = cells[n[0]].zc + prm[delta_x];
		}
	else { zc_temp = cells[n[0]].zc; }

				/*---------Boundary conditions---------*/
	if( parts[i].xp + prm[delta_x]/2.0 > prm[L] ){ xc_temp = xc_temp -(prm[N]-2)*delta_x; }
	if( parts[i].yp + prm[delta_x]/2.0 > prm[L] ){ yc_temp = yc_temp -(prm[N]-2)*delta_x; }
	if( parts[i].zp + prm[delta_x]/2.0 > prm[L] ){ zc_temp = zc_temp -(prm[N]-2)*delta_x; }

	if( parts[i].xp - prm[delta_x]/2.0 < 0 ){ xc_temp = xc_temp + prm[N]*delta_x; }
	if( parts[i].yp - prm[delta_x]/2.0 < 0 ){ yc_temp = yc_temp + prm[N]*delta_x; }
	if( parts[i].zp - prm[delta_x]/2.0 < 0 ){ zc_temp = zc_temp + prm[N]*delta_x; }
	/*
	printf("Delta x %lf\n", prm[delta_x]);
	printf("Pos de esquina de cel_p\t%1.5e\t%1.5e\t%1.5e\n", xc_temp,
								  							 yc_temp,
															 zc_temp );
	*/
	ll = 1;
	frac_temp = 0;
	for (int p = 0; p < 2; p++){
		for (int q = 0; q < 2; q++){
			for (int r = 0; r < 2; r++){
				
				//Cell centered in the particle
				//printf("Ciclo interno %d\n",ll);
				//Position of the corner
				x_temp = parts[i].xp + pow(-1,p)*prm[delta_x]/2.0;
				y_temp = parts[i].yp + pow(-1,q)*prm[delta_x]/2.0;
				z_temp = parts[i].zp + pow(-1,r)*prm[delta_x]/2.0;
				
				//Cell in which corner pqr is located
				locate_cell( x_temp,
							 y_temp,
							 z_temp,
							 &n[ll],
							 prm );
				//printf("n celda %d\n", n[ll]);
				if (n[ll]==n[0]){ ll++; continue; }

				/*Fraction of volume of the cell centered in the particle
				that is on the cell n[ll] divided by the volume of a cell*/	
				frac[ll-1] = fabs( ( x_temp - xc_temp )*
								   ( y_temp - yc_temp )*
								   ( z_temp - zc_temp ) ) / prm[v_celda];
				
				//printf("%lf\n",frac[ll-1]);
				/*printf("Pos. esqui\t%1.5e\t%1.5e\t%1.5e\n", x_temp,
															y_temp,
															z_temp );
				printf("Pos. celdas %1.5e\t%1.5e\t%1.5e\n", cells[n[ll]].xc,
															cells[n[ll]].yc,
															cells[n[ll]].zc );
				printf("Resta %1.5e\t%1.5e\t%1.5e\n", ( x_temp - xc_temp ),
				( y_temp - yc_temp ),
				( z_temp - zc_temp ) );	*/
				//Adding the fraction of mass in the neighbors cells
				frac_temp = frac_temp + frac[ll-1];
				//printf("%lf\n", frac_temp);
				//Contribution to mass due to the particle to the cell n[ll]
				cells[n[ll]].mc = frac[ll-1]*parts[i].mp + cells[n[ll]].mc;
				//Counter of corners
				ll++;
				}
			}
	}
	//Contribution to mass to the cell where particle is located
	cells[n[0]].mc = ( 1.0 - frac_temp )*parts[i].mp+ cells[n[0]].mc;
	//if(frac_temp>1.0) {printf("HOLA Index, n part, frac_temp and m\t%d\t%d\t%lf\t\n",i,n[0],cells[n[0]].mc);}
	//printf("%f\t%lf\n",frac_temp,cells[n[0]].mc);
	}

return 0;
}
/*************************************************************/
