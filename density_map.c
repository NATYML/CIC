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

int locate_cell(double xp, double yp, double zp, int idpart, 
				int *n, float *prm, struct Cell *cells){

	//Numbers in order to find the cell
	//Counters in x,y y z
	int i,j,k;
	float delta = 1e-2;

	if(prm[L]-xp <= delta){i = prm[N]-1;}
	else{i = floor( xp / prm[L] * prm[N] );}
	if(prm[L]-yp <= delta){j = prm[N]-1;}
	else{j = floor( yp / prm[L] * prm[N] );}
	if(prm[L]-zp <= delta){k = prm[N]-1;}
	else{k = floor( zp / prm[L] * prm[N] );}

	*n = k+prm[N]*(j+prm[N]*i);

	//Storaging particles in cell
	if( idpart > 0 ) 
	  { cells[*n].Np_cell = cells[*n].Np_cell+1;
	    cells[*n].id_part = (long int*) realloc(cells[*n].id_part,
	    										cells[*n].Np_cell
	    	                                    *sizeof(long int)); 
	  	cells[*n].id_part[cells[*n].Np_cell-1] = idpart;
	  }
	
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
	//printf("%lf\n",prm[delta_x] );
	for (int i = 0; i < (int) prm[N_part]; i++){
				
			/*Cell centered in the particle: cells[n0]
      				Cell of the particle*/ 
	//printf("%d\n",i );

	locate_cell( parts[i].xp, 
				 parts[i].yp, 
				 parts[i].zp,
				 i, 
				 &n[0], 
				 prm,
				 cells );

	//Corner of the cell that is used to find the volume
						/*--- X corner ---*/
	if ( parts[i].xp >= cells[n[0]].xc + prm[delta_x]/2.0 ){
		 xc_temp = cells[n[0]].xc + prm[delta_x];
		}
	else { xc_temp = cells[n[0]].xc; }
						/*--- Y corner ---*/
	if ( parts[i].yp >= cells[n[0]].yc + prm[delta_x]/2.0 ){
		yc_temp = cells[n[0]].yc + prm[delta_x];
		}
	else { yc_temp = cells[n[0]].yc; }
   					   /*--- Z corner ---*/
	if ( parts[i].zp >= cells[n[0]].zc + prm[delta_x]/2.0 ){
		zc_temp = cells[n[0]].zc + prm[delta_x];
		}
	else { zc_temp = cells[n[0]].zc; }
	/*
	if(i==239415){printf("Celda\t%1.5e\t%1.5e\t%1.5e\n", cells[n[0]].xc,
															cells[n[0]].yc,
															cells[n[0]].zc);}
	if(i==239415){printf("Particula \t%1.5e\t%1.5e\t%1.5e\n", parts[i].xp,
														     parts[i].yp,
															 parts[i].zp);}*/
																
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
				
				/*
				printf("Pos. esqui\t%1.5e\t%1.5e\t%1.5e\n", x_temp,
															y_temp,
															z_temp );
				*/
				if (x_temp > prm[L] || x_temp < 0){ ll++; continue; }
				if (y_temp > prm[L] || y_temp < 0){ ll++; continue; }
				if (z_temp > prm[L] || z_temp < 0){ ll++; continue; }

				//Cell in which corner pqr is located
				locate_cell( x_temp,
							 y_temp,
							 z_temp,
							 -1,
							 &n[ll],
							 prm,
							 cells );

				//printf("n celda %d\n", n[ll]);
				if (n[ll]==n[0]){ ll++; continue; }
		
				/*Fraction of volume of the cell centered in the particle
				that is on the cell n[ll] divided by the volume of a cell*/	
				frac[ll-1] = fabs( ( x_temp - xc_temp )*
								   ( y_temp - yc_temp )*
								   ( z_temp - zc_temp ) ) / prm[v_celda];
				/*if(i==239415){printf("%lf\n",frac[ll-1]);
							 printf("Pos. Cent. part. \t%1.5e\t%1.5e\t%1.5e\n", x_temp,
																				y_temp,
																				z_temp );
							 printf("Esqui esc \t%1.5e\t%1.5e\t%1.5e\n", xc_temp,
															yc_temp,
															zc_temp );								
																}*/
				//printf("%lf\n",frac[ll-1]);
				/*printf("Pos. esqui\t%1.5e\t%1.5e\t%1.5e\n", x_temp,
															y_temp,
															z_temp );
				printf("Pos. celdas %1.5e\t%1.5e\t%1.5e\n", cells[n[ll]].xc,
															cells[n[ll]].yc,
															cells[n[ll]].zc );

				printf("Resta %1.5e\t%1.5e\t%1.5e\n", ( x_temp - xc_temp ),
																	( y_temp - yc_temp ),
																	( z_temp - zc_temp ) );											*/

				//Adding the fraction  of mass in the neighbors cells
				frac_temp = frac_temp + frac[ll-1];
				//printf("%lf\n", frac_temp);
				//Contribution to mass due to the particle to the cell n[ll]
				cells[n[ll]].mc = frac[ll-1]*parts[i].mp + cells[n[ll]].mc;
					/*{printf("Pos.\t%1.5e\t%1.5e\t%1.5e\n", x_temp,
															y_temp,
															z_temp );}*/
				//Counter of corners
				ll++;
				}
			}
	}
	//Contribution to mass to the cell where particle is located
	cells[n[0]].mc = ( 1.0 - frac_temp )*parts[i].mp+ cells[n[0]].mc;
	if(frac_temp>1.0) {printf("Fra >1 Index part i, n, frac_temp \t%d\t%d\t%lf\t\n",i,n[0],frac_temp);}
	//printf("%f\t%lf\n",frac_temp,cells[n[0]].mc);
	}

return 0;
}
/*************************************************************/
