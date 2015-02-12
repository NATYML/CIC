#include "allvars.h"


int main(){
  	
  	//Reading parameters
  	float prm[N_params];
  	read_parameters( prm, "param_archive.dat" );
	prm[delta_x] = prm[L]/prm[N];
	prm[v_celda] = pow( prm[delta_x],3 );

  	//Array of Structures Particles, size NMAX 
	struct Particle *parts;
	parts = (struct Particle *)calloc( NMAX, sizeof( struct Particle) );
	
	//Reading particle positions 
	Read(parts, &prm[N_part]);
	
	//Array of Structures Cells, size N^3
	struct Cell *cells; 
	cells = (struct Cell *)calloc( pow(prm[N],3), sizeof( struct Cell) );	
	
	//The cells are created 		
	Grid( cells,prm );    

	//CIC code 
	CIC( parts,cells,prm );

	//Associated field stored per cell
	Write_out_file( cells,prm );

return 0;
}

 