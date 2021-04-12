//Afonso Sequeira ID: 10139091
//Assignment 5
//A C++ class for matrices

#include<iostream>
#include<sstream>
#include<cmath>

class matrix 
{
private:
    int n_rows{0};
    int n_cols{0};
    double* array {nullptr};

public:
    //Member functions
    //Parametrized constructors & destructor
    matrix() = default;
    matrix(int rows, int cols);
    matrix( matrix& old_mat);
    matrix( matrix&& old_mat);
    ~matrix(){
        delete[] array; std::cout<<"Destructor called."<<std::endl;
    }

    //copy and move assignemnt operators
    matrix& operator=( matrix &old_mat);
    matrix& operator=( matrix &&old_mat);

    //setter and getter functions
    void set_element(int i, int j, double num);
    double get_element(int i, int j);

    //overloaded algebraic operators
    matrix operator+(const matrix& mat);
    matrix operator-(const matrix& mat);
    matrix operator*(const matrix& mat);

    //member functions to get the minor and determinant
    matrix get_minor(int i, int j);
    double determinant();
    
    //make << and >> friends of the class
    friend std::ostream& operator<<(std::ostream &os, const matrix& mat);
    friend std::istream&  operator>>(std::istream &is, matrix& mat);
};

//parametrised constructor implementation
matrix::matrix(int rows, int cols){
    //check if dimensions are valid
    try{
        if (rows < 1 || cols < 1){
            throw(0);
        }
        array = nullptr;
        n_rows = rows;
        n_cols = cols;

        array = new double [n_rows * n_cols];

        for (std::size_t i{0}; i < (n_rows * n_cols); i++){
            array[i] = 0;
        } 
    }
    catch (int err) {
        std::cout << "Matrix dimensions not valid (must be >= 1)." << std::endl;
        n_rows = 0;
        n_cols = 0;
        array = nullptr;
    }
}

//copy constructor implementation
matrix::matrix( matrix& old_mat){
    std::cout << "Copy constructor." << std::endl;
    array = nullptr;
    n_rows = old_mat.n_rows;
    n_cols = old_mat.n_cols;

    array = new double[n_rows * n_cols];
    for (std::size_t i{0}; i < (n_rows * n_cols); i++) {
        array[i] = old_mat.array[i];
    }
}

//move constructor implementation
matrix::matrix(matrix&& old_mat){
    std::cout << "Move constructor." << std::endl;
    array = nullptr;

    //move data into new object
    n_rows = old_mat.n_rows;
    n_cols = old_mat.n_cols;
    array = old_mat.array;

    //reset data of old object
    old_mat.n_cols = 0; old_mat.n_rows = 0;
    old_mat.array = nullptr;
}

//move assignment implementation
matrix& matrix::operator=( matrix &&old_mat) {
    std::cout << "Move Assignemnt." << std::endl;
    std::swap(n_cols, old_mat.n_cols);
    std::swap(n_rows, old_mat.n_rows);
    std::swap(array, old_mat.array);
    return *this;
}


//copy assignment operator implementation
matrix& matrix::operator=( matrix &old_mat) {
    std::cout << "Copy Assignemnt." << std::endl;
    //Handle self-assignemnt
    if (&old_mat == this) {
        return *this;
    }

    //delete old array
    delete[] array; array=nullptr; n_rows = 0; n_cols = 0;

    n_rows = old_mat.n_rows;
    n_cols = old_mat.n_cols;

    if (n_cols > 0 && n_rows > 0){
        array = new double[n_rows * n_cols];
        for (std::size_t i{0}; i < (n_rows * n_cols); i++) {
            array[i] = old_mat.array[i];
        }
    } 
    return *this;
}

void matrix::set_element(int i, int j, double num){
    try {
        //check if dimensions are valid
        if (i < 1 || i >= (n_rows * n_cols) || j < 1 || j >= (n_rows * n_cols)) {
            throw(0);
        }
        //set element corresponding to ith row and jth column to be = num
        array[(j-1) + (i-1)*n_cols] = num;
    }
    catch (int err){
        std::cout << "Element position not valid." << std::endl;
    }
}

double matrix::get_element(int i, int j){
    try{
        if (i < 1 || i >= (n_rows * n_cols) || j < 1 || j >= (n_rows * n_cols)) {
            throw(0);
        }
        return array[(j-1) + (i-1)*n_cols];
    }
    catch (int err) {
        std::cout<< "Element position not valid." << std::endl;
        return -1;
    }   
}

//overload + operator for matrix
matrix matrix::operator+(const matrix& mat) {
    matrix new_matrix;
    try {
        //check if matrices have same dimensions
        if ( n_rows == mat.n_rows && n_cols == mat.n_cols) {
            new_matrix.n_rows = n_rows;
            new_matrix.n_cols = n_cols;
            new_matrix.array = new double[n_rows * n_cols];
            for (std::size_t i{0}; i < (n_rows * n_cols); i++){
                new_matrix.array[i] = this->array[i] + mat.array[i]; //add elements in order
            }
        }
        else {
            throw(0);
        }
    }
    catch (int err){
        std::cout << "Matrix dimentions do not match" << std::endl;
    }
    return new_matrix;
}

//overload - operator for matrix
matrix matrix::operator-(const matrix& mat) {
    matrix new_matrix;
    try {
        if ( n_rows == mat.n_rows && n_cols == mat.n_cols) {
            new_matrix.n_rows = n_rows;
            new_matrix.n_cols = n_cols;
            new_matrix.array = new double[n_rows * n_cols];
            for (std::size_t i{0}; i < (n_rows * n_cols); i++){
                new_matrix.array[i] = this->array[i] - mat.array[i];
            }
        }
        else {
            throw(0);
        }  
    }
    catch (int err) {
        std::cout << "Matrix dimentions do not match" << std::endl;
    }
    return new_matrix;
}

//overload * operator for matrix class
matrix matrix::operator*(const matrix& mat) {
    matrix new_matrix;
    try {
        if (n_cols == mat.n_rows) {
            new_matrix.n_rows = n_rows;
            new_matrix.n_cols = mat.n_cols;
            new_matrix.array = new double[n_rows * mat.n_cols];
            //iterate through rows of first matrix
            for (std::size_t i{1}; i <= n_rows; i++){
                //iterate through columns of second matrix
                for (std::size_t j{1}; j <= mat.n_cols; j++){
                    //iterate through common index of matrix multiplication
                    for (std::size_t k{1}; k <= n_cols; k++){
                        new_matrix.array[(j-1) + (i-1) * new_matrix.n_cols] += 
                                                (this->array[(k-1)  + (i-1) * n_cols]) * 
                                                (mat.array[(j-1) + (k-1)* mat.n_cols]);
                    }
                }
            }  
        }
        else {
            throw(0);
        }  
    }
    catch(int err){
        std::cout << "Matrix dimensions do not match." << std::endl;
    }
    return new_matrix;
}

//returns a matrix with the ith row and jth column deleted, the minor
matrix matrix::get_minor(int i, int j){
    matrix temp;
    temp.n_cols = n_cols - 1;
    temp.n_rows = n_rows - 1;
    temp.array = new double[temp.n_cols * temp.n_rows];
    int index{0};
    for (std::size_t r{0}; r < n_rows; r++) {
        if (r == (i - 1)){ continue;} //if row index corresponds to deleted row, do nothing
        for (std::size_t c{0}; c < n_cols; c++){
            if (c == (j - 1)) { continue;} //if column index corresponds to deleted column, do nothing
            temp.array[index] = this->array[ c + r*n_cols ]; //copy element
            index ++;
        }
    }
    return temp;
}

//calculate recursively the determinant of the matrix
double matrix::determinant(){
    double det;
    double res; 
    //check if matrix is square, if not return 0 and error message
    if (n_cols != n_rows){
        std::cout << "Must be a square matrix to calculate determinant." << std::endl;
        return 0;
    }
    //if num. of columns = 2, calculate determinant directly
    if (n_cols == 2) {
        res = (this-> array[0] * this->array[3]) - (this->array[1] * this->array[2]);
        return res;
    }
    //iterate through column index and add up cooresponding minors
    for (std::size_t j{0}; j < (n_cols); j++){
        det += pow(-1,1 + (j + 1)) * this->array[j] * this->get_minor(1,j+1).determinant();
    }
    return det;
}

std::ostream& operator<<(std::ostream &os, const matrix & mat){
    //iterate through rows
    for (std::size_t i{0}; i < (mat.n_rows); i++){
        os << "[ ";
        //iterate through columns 
        for (std::size_t j{0}; j < (mat.n_cols); j++){
            os << mat.array[j + i * mat.n_cols] << " ";
        }
        os << "]\n";
    }
    return os;
}

//Overload inputstream operator. To use, input numbers in one line separated by whitespace, the first two
//digits will specify the matrix dimensions, and the rest will be taken as the elements, going row by row
//e.g >> 2 2 3 4 5 6 will input the 2*2 matrix [[4 5][5 6]]
std::istream& operator>>(std::istream &is, matrix & mat){
    is >> mat.n_rows >> mat.n_cols;
    delete[] mat.array; mat.array = nullptr;
    mat.array = new double[mat.n_rows * mat.n_cols];
    for (std::size_t i{0}; i < (mat.n_rows * mat.n_cols); i++){
        is >> mat.array[i];
    }
    return is;
}

//main program
int main () {
    matrix o(-2,0); //create invalid matrix to catch exception
    matrix a(3,3); //parametrised constructor
    std::cout << " a = \n" << a << std::endl; //show overloaded << operator
    matrix b{a}; //copy constructor
    matrix c; 
    c = a; //copy assingment
    a.set_element(1,1,5); //set i = 1, j = 1 element of a to 5
    //show deep copy
    std::cout << " a = \n" << a << std::endl; 
    std::cout << " b = \n" << b << std::endl;
    std::cout << " c = \n" << c << std::endl;
    
    a = a; //can handle self-assignemnt
    matrix d(std::move(a));
    matrix e;
    e = std::move(b);

    matrix i(2,2);
    matrix j(2,2);
    i.set_element(1,1,1);
    i.set_element(1,2,2);
    i.set_element(2,1,3);
    i.set_element(2,2,4);

    j.set_element(1,1,2);
    j.set_element(1,2,4);
    j.set_element(2,1,6);
    j.set_element(2,2,8);
    
    std::cout << "i = \n" << i << "j = \n" << j << "i + j =\n" << i+j << "i - j = \n" << i - j 
                << "i * j = \n" << i*j << std::endl;
    
    matrix k(2,3);
    matrix l(4,2);
    std::cout << "k*l = \n" << k*l << std::endl;

    //ask for input matrix
    std::cout << "Please input a matrix: " << std::endl;
    matrix g;
    std::cin >> g;
    std::cout << "Your matrix = \n" << g << std::endl;

    std::cout << "Your matrix with 1st row and 1st column deleted is: \n" << g.get_minor(1,1) << std::endl; 
    std::cout << "The determinant of your matrix is: \n" << g.determinant() << std::endl;

    return 0;
}