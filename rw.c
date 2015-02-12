
/*********************************/
//	Save and read data
/*********************************/

#include "allvars.h"

/**************************************************************/

			//Convert data file text in plain text 

int conf2dump( char filename[] )
{
    char cmd[100];
    sprintf( cmd, 
    "grep -v \"#\" %s | grep -v \"^$\" | gawk -F\"=\" '{print $2}' > %s.dump", 
	filename, filename );
    system( cmd );

    return 0;
}

/*************************************************************/



/*************************************************************/

			//Read file with and load information  

int read_parameters( float parameters[],
		     char filename[] )
{
    char cmd[100], filenamedump[100];
    int i=0;
    FILE *file;

    //Load of File
    file = fopen( filename, "r" );
    if( file==NULL ){
	printf( "  * The file '%s' don't exist!\n", filename );
	return 1;}
    fclose(file);
    
    //Converting to plain text
    conf2dump( filename );
    sprintf( filenamedump, "%s.dump", filename );
    file = fopen( filenamedump, "r" );
    
    //Reading
    while( getc( file ) != EOF ){
	fscanf( file, "%f", &parameters[i] );
	i++;}
    
    fclose( file );
    
    printf( "  * The file '%s' has been loaded!\n", filename );

    sprintf( cmd, "rm -rf %s.dump", filename );
    system( cmd );
    
    return 0;
}

/*************************************************************/

			//Read file with the data from a simulation

int Read(struct Particle *str_array, float *N_particles){

	int i;
	double dumb;
	FILE *pt;


	//Open the data file 
	pt = fopen("./data.dat","r"); 	

	i = 0;   

	while(  getc(pt) != EOF  ){		
	        fscanf( pt, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf",
	        		&dumb, 
	        		&str_array[i].xp,
	        		&str_array[i].yp,
	        		&str_array[i].zp,
	        		&dumb,&dumb,&dumb,
	        		&str_array[i].mp,
	        		&dumb,&dumb,&dumb,
	        		&dumb,&dumb,&dumb
	        		);			
	        i++;
	        }
	*N_particles =  1.0*i;
	fclose(pt);

	return 0;
}

/*************************************************************/

				//Write file with the Field 
int Write_out_file( struct Cell *CELLS,float *prm ){

	int N_celda; 
	FILE *fp;

	//Create document to write out
	fp = fopen("./out.dat","w"); 

	fprintf( fp, "#First Raw: Divisions Grid N, Box length L(kpc), Size cells, Number Particles\n");
	fprintf( fp, "#Second Raw: Position x, y, z($kpc$) and mass($10^{12}M_{sun}$)\n ");

	//Some parameters are stored
	fprintf( fp, "%lf\n%lf\n%lf\n%lf\n", prm[N],
										 prm[L],
										 prm[delta_x],
										 prm[N_part] );

	//Position and mass assigned is stored per cell
	for (int n = 0; n < prm[N]; n++){
		for (int m = 0; m < prm[N]; m++){
			for (int l= 0; l< prm[N]; l++){
				 N_celda = l+prm[N]*(m+prm[N]*n);
				 fprintf( fp, "%lf\n",CELLS[N_celda].mc	);
			}
		}
	} 


	fclose(fp);

	return 0;
}
