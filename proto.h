
int conf2dump( char *);
int read_parameters( float *, char *);
int Read(struct Particle *,float * );
int Write_out_file( struct Cell *,float * );
int Grid(struct Cell *, float *);
int locate_cell(double, double, double, int ,int *, float *, struct Cell *);
int CIC(struct Particle *,struct Cell *, float * );