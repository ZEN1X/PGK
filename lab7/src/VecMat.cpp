#include "VecMat.h"
#include <cmath>

Vector::Vector(double x, double y)
    : _data{x, y, 1.0} {}

void Vector::SetX(double x) {
  _data[0] = x;
}

void Vector::SetY(double y) {
  _data[1] = y;
}

double Vector::GetX() const {
  return _data[0];
}

double Vector::GetY() const {
  return _data[1];
}

Matrix::Matrix()
    : _data{{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}} {}

Matrix Matrix::operator*(const Matrix& other) const {
  Matrix result;
  
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      result._data[i][j] = 0.0; // zero out cause we initialize the matrix to be
                                // 1 0 0
                                // 0 1 0
                                // 0 0 1

      for (int k = 0; k < 3; ++k) {
        result._data[i][j] += _data[i][k] * other._data[k][j];
      }
    }
  }

  return result;
}

Vector Matrix::operator*(const Vector& vector) const {
  Vector result;

  for (int i = 0; i < 3; ++i) {
    result._data[i] = 0.0; // zero out cause the vector gets default initialized to
                           // 0 0 1
                           // here is why we need the friend

    for (int k = 0; k < 3; ++k) {
      result._data[i] += _data[i][k] * vector._data[k];
    }
  }

  return result;
}

Matrix Matrix::Translate(double dx, double dy) {
  Matrix translation;

  translation._data[0][2] = dx;
  translation._data[1][2] = dy;

  return translation;
}

Matrix Matrix::Scale(double Sx, double Sy) {
  Matrix scaling;

  scaling._data[0][0] = Sx;
  scaling._data[1][1] = Sy;

  return scaling;
}

Matrix Matrix::Rotate(double deg) {
  Matrix rotation;
  double rad = deg * M_PI / 180.0;

  rotation._data[0][0] = std::cos(rad);
  rotation._data[0][1] = -std::sin(rad);
  rotation._data[1][0] = std::sin(rad);
  rotation._data[1][1] = std::cos(rad);

  return rotation;
}

Matrix Matrix::Shear(double horizontal, double vertical) {
  Matrix shearing;

  shearing._data[0][1] = vertical;
  shearing._data[1][0] = horizontal;

  return shearing;
}


