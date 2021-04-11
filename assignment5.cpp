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

    matrix& operator=( matrix &old_mat);
    matrix& operator=( matrix &&old_mat);
    void set_element(int i, int j, double num);
    double get_element(int i, int j);

    matrix operator+(const matrix& mat);
    matrix operator-(const matrix& mat);
    matrix operator*(const matrix& mat);
    matrix get_minor(int i, int j);
    double determinant();
    
    friend std::ostream& operator<<(std::ostream &os, const matrix& mat);
    friend std::istream&  operator>>(std::istream &is, matrix& mat);
};

matrix::matrix(int rows, int cols){
    if (rows < 1 || cols < 1){
        throw("Matrix dimensions not valid (<1).");
    }
    array = nullptr;
    n_rows = rows;
    n_cols = cols;

    array = new double [n_rows * n_cols];

    for (std::size_t i{0}; i < (n_rows * n_cols); i++){
        array[i] = 0;
    } 
}

matrix::matrix( matrix& old_mat){
    std::cout << "Copy constructor." << std::endl;
    array = nullptr;
    n_rows = old_mat.n_rows;
    n_cols = old_mat.n_cols;

    if (n_cols > 0 && n_rows > 0){
        array = new double[n_rows * n_cols];
        for (std::size_t i{0}; i < (n_rows * n_cols); i++) {
            array[i] = old_mat.array[i];
        }
    } 

}

//move constructor
matrix::matrix(matrix&& old_mat){
    std::cout << "Move constructor." << std::endl;
    array = nullptr;

    n_rows = old_mat.n_rows;
    n_cols = old_mat.n_cols;
    array = old_mat.array;

    old_mat.n_cols = 0; old_mat.n_rows = 0;
    old_mat.array = nullptr;
}

matrix& matrix::operator=( matrix &&old_mat) {
    std::cout << "Move Assignemnt." << std::endl;
    std::swap(n_cols, old_mat.n_cols);
    std::swap(n_rows, old_mat.n_rows);
    std::swap(array, old_mat.array);
    return *this;
}



matrix& matrix::operator=( matrix &old_mat) {
    std::cout << "Assignemnt." << std::endl;
    if (&old_mat == this) {
        return *this;
    }

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
    if (i < 1 || i >= (n_rows * n_cols) || j < 1 || j >= (n_rows * n_cols)) {
        throw("Dimensions not valid for this matrix.");
    }
    array[(j-1) + (i-1)*n_cols] = num;
}

double matrix::get_element(int i, int j){
    if (i < 1 || i >= (n_rows * n_cols) || j < 1 || j >= (n_rows * n_cols)) {
        throw("Dimensions not valid for this matrix.");
    }
    return array[(j-1) + (i-1)*n_cols];
}

//overload + operator for matrix
matrix matrix::operator+(const matrix& mat) {
    matrix new_matrix;
    if ( n_rows == mat.n_rows && n_cols == mat.n_cols) {
        new_matrix.n_rows = n_rows;
        new_matrix.n_cols = n_cols;
        new_matrix.array = new double[n_rows * n_cols];
        for (std::size_t i{0}; i < (n_rows * n_cols); i++){
            new_matrix.array[i] = this->array[i] + mat.array[i];
        }
    }
    else {
        throw("Matrix dimensions do not match.");
    }
    return new_matrix;
}

//overload - operator for matrix
matrix matrix::operator-(const matrix& mat) {
    matrix new_matrix;
    if ( n_rows == mat.n_rows && n_cols == mat.n_cols) {
        new_matrix.n_rows = n_rows;
        new_matrix.n_cols = n_cols;
        new_matrix.array = new double[n_rows * n_cols];
        for (std::size_t i{0}; i < (n_rows * n_cols); i++){
            new_matrix.array[i] = this->array[i] - mat.array[i];
        }
    }
    else {
        throw("Matrix dimensions do not match.");
    }  
    return new_matrix;
}

//overload * operator for matrix class
matrix matrix::operator*(const matrix& mat) {
    matrix new_matrix;
    if (n_cols == mat.n_rows) {
        new_matrix.n_rows = n_rows;
        new_matrix.n_cols = mat.n_cols;
        new_matrix.array = new double[n_rows * mat.n_cols];
        for (std::size_t i{1}; i <= n_rows; i++){
            for (std::size_t j{1}; j <= mat.n_cols; j++){
                for (std::size_t k{1}; k <= n_cols; k++){
                    new_matrix.array[(j-1) + (i-1) * new_matrix.n_cols] += 
                                            (this->array[(k-1)  + (i-1) * n_cols]) * 
                                            (mat.array[(j-1) + (k-1)* mat.n_cols]);
                }
            }
        }  
    }
    else {
        throw("Matrix dimensions do not match.");
    }  
    return new_matrix;
}

matrix matrix::get_minor(int i, int j){
    matrix temp;
    temp.n_cols = n_cols - 1;
    temp.n_rows = n_rows - 1;
    temp.array = new double[temp.n_cols * temp.n_rows];
    int index{0};
    for (std::size_t r{0}; r < n_rows; r++) {
        if (r == (i - 1)){ continue;}
        for (std::size_t c{0}; c < n_cols; c++){
            if (c == (j - 1)) { continue;}
            temp.array[index] = this->array[ c + r*n_cols ];
            index ++;
        }
    }
    return temp;
}

double matrix::determinant(){
    double det;
    double res;
    if (n_cols == 2) {
        res = (this-> array[0] * this->array[3]) - (this->array[1] * this->array[2]);
        return res;
    }
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

std::istream& operator>>(std::istream &is, matrix & mat){
    is >> mat.n_rows >> mat.n_cols;
    delete[] mat.array; mat.array = nullptr;
    mat.array = new double[mat.n_rows * mat.n_cols];
    for (std::size_t i{0}; i < (mat.n_rows * mat.n_cols); i++){
        is >> mat.array[i];
    }
    return is;
}


int main () {
    matrix a(3,3);
    matrix b{a};
    matrix c = a;
    matrix d(a);
    matrix g;
    g = a;
    matrix e(a.get_minor(1,1));
    matrix k;
    k = a.get_minor(1,1);
    //std::cout << e;
    return 0;
}