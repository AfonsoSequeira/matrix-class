# matrix-class
A class to store 2D matrices.

The minimum content of the class: it must correctly implement the following member functions:
 - Parameterized constructor (utilizing new).
 - and destructor (utilizing delete).
 - Member function to overload assignment operator that performs a deep copy of data.
 - Must also handle self-assignment.
 - A copy constructor and copy assignment operator that both perform a deep copy of the class data.
 - An efficiently coded move constructor and move assignment.
 - A friend function to overload << producing nicely formatted output for a matrix object; and a friend function to overload >> to input a matrix in a sensible form of your choice.

Operational functionality:
 - Functions overloading + and - for matrix addition and subtraction (functions must check both matrices have same dimensions).
 - Function overloading * for matrix multiplication (must check operation is possible)
 - A member function to return a matrix with the i th row and j th column deleted [1 mark]
  Challenge: A function to calculate recursively the determinant of any square matrix (using the expansion in minors–see e.g., mathworld). [The (i,j) minor is the determinant of     the matrix with the i th row and j th column deleted]

All of these must be demonstrated successfully in the main() function. For copy and assignment operations, demonstrate deep copying by changing the original matrix and showing that the new matrix remains unchanged
