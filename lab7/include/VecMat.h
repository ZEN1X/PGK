#ifndef VECMAT_H
#define VECMAT_H

class Vector {
 private:
  double _data[3];

 public:
  Vector(double x = 0.0, double y = 0.0);

  void SetX(double x);
  void SetY(double y);

  double GetX() const;
  double GetY() const;

  friend class Matrix; // need to friend the matrix class, as we overload operator* for matrix * vector operation
};

class Matrix {
 private:
  double _data[3][3];

 public:
  Matrix();

  Matrix operator*(const Matrix& other) const;
  Vector operator*(const Vector& vector) const;

  static Matrix Translate(double dx, double dy);
  static Matrix Scale(double Sx, double Sy);
  static Matrix Rotate(double deg);
  static Matrix Shear(double horizontal, double vertical);
};

#endif /* VECMAT_H */
