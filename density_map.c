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
	float delta = 1e-4;

				// x
	//Points that fall in L 
	if( prm[L]-xp <= delta){i = prm[N]-1;}
	//Periodical boundary conditions 
	else if(xp > prm[L]){ i=0; }
	else if(xp < 0){ i = prm[N]-1; }
 	//Cell in which x falls 
	else{i = floor( xp / prm[L] * prm[N] );}

				// y
	//Points that fall in L 
	if(prm[L]-yp <= delta){j = prm[N]-1;}
	//Periodical boundary conditions 
	else if(yp > prm[L]){ j=0; }
	else if(yp < 0){ j = prm[N]-1; }
 	//Cell in which y falls 
	else{j = floor( yp / prm[L] * prm[N] );}
	
			   // z
	//Points that fall in L 
	if(prm[L]-zp <= delta){k = prm[N]-1;}
	//Periodical boundary conditions 
	else if(zp > prm[L]){ k=0; }
	else if(zp < 0){ k = prm[N]-1; }
 	//Cell in which z falls 
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

	double mp_temp=0, mp_temp_cells=0;

	for (int i = 0; i < (int) prm[N_part]; i++){
				
			/*Cell centered in the particle: cells[n0]
      				Cell of the particle*/ 

	locate_cell( parts[i].xp, 
				 parts[i].yp, 
				 parts[i].zp,
				 i, 
				 &n[0], 
				 prm,
				 cells );
	//Test for cloud in cell
    //Sum particles masses 	
	mp_temp = parts[i].mp + mp_temp;

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
																
	ll = 1;
	frac_temp = 0;
	for (int p = 0; p < 2; p++){
		for (int q = 0; q < 2; q++){
			for (int r = 0; r < 2; r++){
						
				//Cell centered in the particle
				
				//Position of the corner
				x_temp = parts[i].xp + pow(-1,p)*prm[delta_x]/2.0;
				y_temp = parts[i].yp + pow(-1,q)*prm[delta_x]/2.0;
				z_temp = parts[i].zp + pow(-1,r)*prm[delta_x]/2.0;
				
				//Cell in which corner pqr is located
				locate_cell( x_temp,
							 y_temp,
							 z_temp,
							 -1,
							 &n[ll],
							 prm,
							 cells );

				if (n[ll]==n[0]){ ll++; continue; }

				/*Fraction of volume of the cell centered in the particle
				that is on the cell n[ll] divided by the volume of a cell*/	
				frac[ll-1] = fabs( ( x_temp - xc_temp )*
								   ( y_temp - yc_temp )*
								   ( z_temp - zc_temp ) ) / prm[v_celda];

				//Adding the fraction  of mass in the neighbors cells
				frac_temp = frac_temp + frac[ll-1];
				//Contribution to mass due to the particle to the cell n[ll]
				cells[n[ll]].mc = frac[ll-1]*parts[i].mp + cells[n[ll]].mc;
					
				//Counter of corners
				ll++;
				}
			}
	}
	//Contribution to mass to the cell where particle is located
	cells[n[0]].mc = ( 1.0 - frac_temp )*parts[i].mp+ cells[n[0]].mc;
	}
//Test for cloud in cell
//Sum cells masses 	
for(int k=0; k<pow(prm[N],3);k++){
	mp_temp_cells = mp_temp_cells + cells[k].mc;}
printf("Particles total mass %lf \t Cells total mass %lf\n", mp_temp,mp_temp_cells);

return 0;
}
/*************************************************************/
