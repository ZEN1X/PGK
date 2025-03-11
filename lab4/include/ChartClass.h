#ifndef CHARTCLASS_H
#define CHARTCLASS_H

#include <memory>
#include "ConfigClass.h"
#include "vecmat.h"

class ChartClass {
 private:
  std::shared_ptr<ConfigClass> cfg;
  int x_step;                         // liczba odcinkow z jakich bedzie sie skladal wykres
  double x_min, x_max;                // zakres zmiennej x
  double y_min, y_max;                // zakres wartosci przyjmowanych przez funkcje
  double GetFunctionValue(double x);  // zwraca wartosci rysowanej funkcji

  void line2d(wxDC* dc, const Matrix& transformation, double x0, double y0, double x1, double y1);
  // powyzej sugerowana funkcja transformujaca odcinki (jesli ktos ma inny pomysl mozna ja usunac)
  void text2d(wxDC* dc, const Matrix& transformation, double x, double y, double alpha, double value);

  Matrix Rotate(double alpha);             // zwraca macierz obrotu
  Matrix Translate(double dx, double dy);  // zwraca macierz przesuniecia
  Matrix Scale(double sx, double sy);      // zwraca macierz skalowania
  Matrix XFlip();

 public:
  ChartClass(std::shared_ptr<ConfigClass> c);
  void Set_Range();                   // ustala wartosci zmiennych x_min,y_min,x_max,y_max
  double Get_Y_min();                 // zwraca y_min
  double Get_Y_max();                 // zwraca y_max
  void Draw(wxDC* dc, int w, int h);  // rysuje wykres
};

#endif /* CHARTCLASS_H */
