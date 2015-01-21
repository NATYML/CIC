CLOUD IN CELL CODE(CIC)
==============================


Given some property of *N* particles contained in a box of size *L*,
CIC provides the field, *i.e.*, the value of the property in any
point between the size of the box. 
The field is found following the *next steps*:

**1.** A cartesian grid is built asigning a label to every cell of size delta generated. 

**2.** It is found the cell where every particle falls in.

**3.** It is created a cell centered in the particle with the size delta.

**4.** The fraction of the particle cell volume falling in every grid cell is stored and 
   multiplied to the property particle. 
   This is, not only the cell where particle falls in is having a contribution
   to the field, but also the grid cell neigbohrs. 

**5.** All the possible contributions to a grid cell due to different particles are 
   added and that way is assigned one property value to the grid cell, therefore, 
   finding the field.    