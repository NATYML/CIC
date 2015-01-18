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
	//Counters in x,y and z
	int i,j,k;

	/* Counter on x taking into account
	the	periodical conditions on x */
	
	// If particle falls in the last 
	// cell then xp + delta_x has to be
	// greater than the length of the box
	if ( (xp+prm[delta_x]) > prm[L] ){
	   i = prm[N]-1 ; 	
	   }
	   // If particle falls in the first
	   // cell then xp - delta_x has to be
	   // negative
	   else if ( (xp-prm[delta_x]) < 0 ){
	   i = 0 ;	
	   }
	   else{
	   i = abs( floor( xp / prm[L] * prm[N] ) );	   	
	   }	

	/* Counter on y taking into account
	the	periodical conditions on y */
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
	the	periodical conditions on z */
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

int CIC(struct Particle *parts, struct Cell *cells,float *prm){
 
	int n[9],ll;
	double frac[8];
	double x_temp,y_temp,z_temp, frac_temp;
	double xc_temp, yc_temp, zc_temp;	 

	/* For each particle it is found its contribution to 
	every of 26 cells, it can be zero */
	int summa = 0;
//	for (int i = 0; i < (int) prm[N_part]; i++){
//    for (int i = 0; i < 4000; i++){	
int i = 1690;
		/*-----------------------------------------*/
		//Cell centered in the particle: cells[n0]
		//printf("\t\t\t\t\tPARTICULA\t\t\t%d\n",i);		
						/*---------------*/
						//Cell of the particle 
		locate_cell( parts[i].xp, parts[i].yp, parts[i].zp, &n[0], prm );	

		//Corner of the cell that is used to find the volume
	
						/*--- X corner ---*/
		if ( parts[i].xp > cells[n[0]].xc + prm[delta_x]/2.0 ){
			  //Boundary condition, cell N
			  if( (parts[i].xp + prm[delta_x]) > prm[L] ){
				   xc_temp = cells[n[0]].xc - (N-4)*prm[delta_x];	    
			 	}
			  //It is taken the upper corner 	
			  else{ xc_temp = cells[n[0]].xc + prm[delta_x]; }
		    }
			  //Boundary condition, cell 1
		else { if( (parts[i].xp - prm[delta_x])< 0 ){
			        xc_temp = cells[n[0]].xc + N*prm[delta_x];
			      }
			   //It is taken the lower corner    
			   else { xc_temp = cells[n[0]].xc; }
			 } 

				 	    /*--- Y corner ---*/		
		if ( parts[i].yp > cells[n[0]].yc + prm[delta_x]/2.0 ){
			  //Boundary condition, cell N
			  if( (parts[i].yp + prm[delta_x]) > prm[L] ){
				   yc_temp = cells[n[0]].yc - (N-4)*prm[delta_x];	    
			 	}
			  //It is taken the upper corner 	
			  else{ yc_temp = cells[n[0]].yc + prm[delta_x]; }
		    }
		    //Boundary condition, cell 1
		else { if( (parts[i].yp - prm[delta_x])< 0 ){
			        yc_temp = cells[n[0]].yc + N*prm[delta_x];
			      }
			   //It is taken the lower corner    
			   else { yc_temp = cells[n[0]].yc; }
			 } 
				 	    /*--- Z corner ---*/		
		if ( parts[i].zp > cells[n[0]].zc + prm[delta_x]/2.0 ){
			  //Boundary condition, cell N
			  if( (parts[i].zp + prm[delta_x]) > prm[L] ){
				   zc_temp = cells[n[0]].zc - (N-4)*prm[delta_x];	    
			 	}
			  //It is taken the upper corner 	
			  else{ zc_temp = cells[n[0]].zc + prm[delta_x]; }
		    }
		    //Boundary condition, cell 1
		else { if( (parts[i].zp - prm[delta_x])< 0 ){
			        zc_temp = cells[n[0]].zc + N*prm[delta_x];
			      }
			   //It is taken the lower corner    
			   else { zc_temp = cells[n[0]].zc; }
			 } 


		ll = 1;
		frac_temp = 0;
		for (int p = 0; p < 2; p++){
			for (int q = 0; q < 2; q++){
			 	for (int r = 0; r < 2; r++){
			 		
			 				//Cell centered in the particle
			 		printf("Ciclo interno %d\n",ll);
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
			 		/*if (ll==9)
			 			{
			 			printf("Primera Ultima esquina%d\t n celda part %d, particle %d\n", n[ll],n[0],i);
			 			}	*/
			 		//printf("n celda corner%d\t n celda part %d\n", n[ll],n[0]);
			 		//if the neigbohr cell is itself then continue
			 		if (n[ll]==n[0]){ printf("HERE\n");ll++; continue; }	  			

			 		/*Fraction of volume of the cell centered in the particle 
					  that is on the cell n[ll] divided by the volume of a cell*/	
			 		frac[ll-1] =  fabs( ( x_temp - xc_temp )*
			 					  	   ( y_temp - yc_temp )*
			 				           ( z_temp - zc_temp ) ) / prm[v_celda];
			 		
			 		printf("fraccion %lf\n",frac[ll-1]);				
			 		/*				   			 		
			 		printf("Pos. esqui\t%1.5e\t%1.5e\t%1.5e\n", x_temp, 
			 						  y_temp,
			 						  z_temp );			
					printf("Pos. celdas %1.5e\t%1.5e\t%1.5e\n", cells[n[ll]].xc , 
			 						  					  cells[n[ll]].yc,
			 						        			  cells[n[ll]].zc );						   			
			 		printf("Resta %1.5e\t%1.5e\t%1.5e\n", ( x_temp - xc_temp ), 
			 						  ( y_temp - yc_temp ),
			 						   ( z_temp - zc_temp ) );				   */
			 		//Adding the fraction of mass in the neighbors cells
			 		frac_temp = frac_temp + frac[ll-1];
			 		printf("fraccion sumada %lf\n", frac_temp);

			 		//Contribution to mass due to the particle to the cell n[ll]
					cells[n[ll]].mc = frac[ll-1]*parts[i].mp + cells[n[ll]].mc;

					//Counter of corners
			 		ll++;
			 		
			 	}	
			}

		}	  
			
	//Contribution to mass to the cell where particle is located
	cells[n[0]].mc = ( 1.0 - frac_temp )*parts[i].mp + cells[n[0]].mc;
	int aa = 9;
	//if(frac_temp>1.0) {printf("Index, n part and frac_temp \t%d\t%d\t%lf\n",i,n[0],frac_temp);}
	//printf("%d\n",frac_temp );
	//printf("%f\t%lf\n",frac_temp,cells[n[0]].mc);

	//} 
	//printf("%d\n",summa );
return 0;	
}

/*************************************************************/

