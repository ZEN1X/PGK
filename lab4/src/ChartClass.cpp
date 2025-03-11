#include "ChartClass.h"
#include <wx/dc.h>
#include <memory>

ChartClass::ChartClass(std::shared_ptr<ConfigClass> c) {
  cfg = std::move(c);
  x_step = 200;
  Set_Range();
}

void ChartClass::Set_Range() {
  double xmin = 9999.9, xmax = -9999.9, ymin = 9999.9, ymax = -9999.9;
  double x, y, step;
  int i;

  xmin = cfg->Get_x_start();
  xmax = cfg->Get_x_stop();

  step = (cfg->Get_x_stop() - cfg->Get_x_start()) / (double)x_step;
  x = cfg->Get_x_start();

  for (i = 0; i <= x_step; i++) {
    y = GetFunctionValue(x);
    if (y > ymax)
      ymax = y;
    if (y < ymin)
      ymin = y;
    x = x + step;
  }

  y_min = ymin;
  y_max = ymax;
  x_min = xmin;
  x_max = xmax;
}

double ChartClass::GetFunctionValue(double x) {
  if (cfg->Get_F_type() == 1)
    return x * x;
  if (cfg->Get_F_type() == 2)
    return 0.5 * exp(4 * x - 3 * x * x);
  return x + sin(x * 4.0);
}

void ChartClass::Draw(wxDC* dc, int w, int h) {
  dc->SetBackground(wxBrush(wxColour(255, 255, 255)));
  dc->Clear();
  dc->SetPen(wxPen(wxColour(255, 0, 0)));
  dc->DrawRectangle(10, 10, w - 20, h - 20);
  dc->SetClippingRegion(10, 10, w - 20, h - 20);
  
  // if we dont do this, we're screwed if macos decides to change the theme
  // light mode has dark system text
  // and dark mode has pure effing white...
  // sometimes its not you, who needs the meds
  // its the os...
  dc->SetTextForeground(wxColour(0, 0, 0));

  // dc->SetDeviceOrigin(0, h);

  // define variables so we can use the same notation as in the lecture
  double x0 = cfg->Get_x0(), x1 = cfg->Get_x1();
  double y0 = cfg->Get_y0(), y1 = cfg->Get_y1();
  double xmin = cfg->Get_dX(), xmax = cfg->Get_dX() + w;
  double ymin = cfg->Get_dY(), ymax = cfg->Get_dY() + h;
  double alpha = cfg->Get_Alpha();

  double Sx = (xmax - xmin) / (x1 - x0);
  double Sy = (ymax - ymin) / (y1 - y0);

  // windowing + scaling
  Matrix t = Translate(xmin, ymin) * Scale(Sx, Sy) * Translate(-x0, -y0);

  // x-axis flip-flop fix
  // it needs to be on the rhs
  // because we actually want this transformation to happen first, before others
  t = t * XFlip();

  // kinda fix
  t = Translate(0, y1 * Sy) * t;
  t = Translate(0, y0 * Sy) * t;
  // basically invert the blank space on the top with the one at the bottom

  // rotation
  if (cfg->RotateScreenCenter()) {
    // to git
    // this needs the fix before rotation
    t = Translate(-w / 2.0, -h / 2.0) * t;
    t = Rotate(-alpha) * t;
    t = Translate(w / 2.0, h / 2.0) * t;

  } else {
    // t = Translate(0, -ymin * Sy) * t;

    // now we need to undo our "kinda fix"
    // this neeeds the fix after rotation
    t = Translate(0, -y0 * Sy) * t;
    t = Translate(0, -y1 * Sy) * t;

    // god knows how it works
    double x_trans = xmin - x0 * Sx;
    double y_trans = ymin - y0 * Sy;

    t = Translate(-x_trans, -y_trans) * t;
    t = Rotate(-alpha) * t;
    t = Translate(x_trans, y_trans) * t;

    // and redo it...
    t = Translate(0, y1 * Sy) * t;
    t = Translate(0, y0 * Sy) * t;

    // t = Translate(0, ymin * Sy) * t;
  }

  // axis department
  dc->SetPen(wxPen(wxColour(160, 131, 240), 2));
  line2d(dc, t, x_min, 0, x_max, 0);  // x-axis
  line2d(dc, t, 0, y_min, 0, y_max);  // y-axis

  // arrows
  line2d(dc, t, x_max, 0, x_max - 0.2, 0.2);   // upper x-axis arrow
  line2d(dc, t, x_max, 0, x_max - 0.2, -0.2);  // lower x-axis arrow
  line2d(dc, t, 0, y_max, 0.2, y_max - 0.2);   // right y-axis arrow
  line2d(dc, t, 0, y_max, -0.2, y_max - 0.2);  // left y-axis arrow

  line2d(dc, t, x_max / 2.0, -0.2, x_max / 2.0, 0.2);  // positive x-axis tick
  text2d(dc, t, x_max / 2.0 - 0.05, -0.2, alpha, x_max / 2.0);

  line2d(dc, t, x_min / 2.0, -0.2, x_min / 2.0, 0.2);  // negative x-axis tick
  text2d(dc, t, x_min / 2.0 - 0.05, -0.2, alpha, x_min / 2.0);

  line2d(dc, t, -0.2, y_max / 2.0, 0.2, y_max / 2.0);  // positive y-axis tick
  text2d(dc, t, 0.2, y_max / 2.0 + 0.05, alpha, y_max / 2.0);

  line2d(dc, t, -0.2, y_min / 2.0, 0.2, y_min / 2.0);
  text2d(dc, t, 0.2, y_min / 2.0 + 0.05, alpha, y_min / 2.0);

  // function yipee!!
  dc->SetPen(wxPen(wxColour(0, 255, 0)));
  double step = (x_max - x_min) / x_step;
  for (double x = x_min; x < x_max; x += step) {
    double y = GetFunctionValue(x);
    line2d(dc, t, x, GetFunctionValue(x), x + step, GetFunctionValue(x + step));
  }
}

void ChartClass::line2d(wxDC* dc, const Matrix& transformation, double x0, double y0, double x1, double y1) {
  Vector start, end;
  start.Set(x0, y0);
  end.Set(x1, y1);

  start = transformation * start;
  end = transformation * end;

  dc->DrawLine(start.GetX(), start.GetY(), end.GetX(), end.GetY());
}

void ChartClass::text2d(wxDC* dc, const Matrix& transformation, double x, double y, double alpha, double value) {
  Vector point;
  point.Set(x, y);

  point = transformation * point;

  dc->DrawRotatedText(wxString::Format("%.2lf", value), point.GetX(), point.GetY(), alpha);
}

Matrix ChartClass::Rotate(double alpha) {
  Matrix rotation;

  double radians = alpha * M_PI / 180.0;

  rotation.data[0][0] = std::cos(radians);
  rotation.data[0][1] = -std::sin(radians);
  rotation.data[1][0] = std::sin(radians);
  rotation.data[1][1] = std::cos(radians);

  return rotation;
}

Matrix ChartClass::Translate(double dx, double dy) {
  Matrix translation;

  translation.data[0][0] = 1.0;
  translation.data[0][2] = dx;
  translation.data[1][1] = 1.0;
  translation.data[1][2] = dy;

  return translation;
}

Matrix ChartClass::Scale(double sx, double sy) {
  Matrix scaling;

  scaling.data[0][0] = sx;
  scaling.data[1][1] = sy;

  return scaling;
}

Matrix ChartClass::XFlip() {
  Matrix xflip;

  xflip.data[0][0] = 1;
  xflip.data[1][1] = -1;
  return xflip;
}

double ChartClass::Get_Y_min() {
  Set_Range();
  return y_min;
}

double ChartClass::Get_Y_max() {
  Set_Range();
  return y_max;
}
