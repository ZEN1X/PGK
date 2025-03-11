#include "GUIMyFrame1.h"
#include <wx/dcbuffer.h>  // for wxAutoBufferedPaintDC
#include <algorithm>      // for std::min and std::max
#include <limits>         // for infinity
#include "PrepareData.h"
#include "VecMat.h"

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
    : MyFrame1(parent), _isColour{m_cb_color->IsChecked()}, _rotation{m_s_rotation->GetValue()}, _tilt{m_s_tilt->GetValue()} {
  m_panel1->SetBackgroundStyle(wxBG_STYLE_PAINT);  // for buffered dc
  PrepareData(0);
}

void GUIMyFrame1::m_panel1OnPaint(wxPaintEvent& event) {
  // flicker-free!! yay :)
  // (also doesnt use up resources when the Desktop Environment already has a double buffer)
  wxAutoBufferedPaintDC dc(m_panel1);

  // prepare the bg
  dc.SetBrush(*wxWHITE_BRUSH);  // white bg
  dc.Clear();                   // clears the panel with set brush

  // prepare the pen (voxels)
  dc.SetPen(*wxTRANSPARENT_PEN);  // clear outline (only infill is coloured)

  // the panel has a 500x500 size
  // we create an array of 100x100 points (actually rectangles of width 5) such as: (x, y, f_val)
  // where x and y are the bottom-left anchor points of the vertical rectangle, and f_val will constitute its heigh
  double points[100][100][3];

  // instruction says that functions take on values from -2.5 to 2.5 but that's not true...
  // we gotta keep the min and max values of a function in a variable
  double f_min = -std::numeric_limits<double>::infinity();
  double f_max = std::numeric_limits<double>::infinity();

  // we first need to define a transformation matrix for the function's points
  // (the parallelogram stays the same, it's just the function that changes)
  // x and y in range of -2.5 and 2.5 give nice values (similar to demo)

  // FINALLY ROTATE THE FUNCTION (NOT THE PARALLELOGRAM) + 270 so that it matches the demo <- NOW THE (x, y) is in range -2.5 to 2.5 <- CENTER AROUND THE (0, 0) POINT
  Matrix function_transformation = Matrix::Rotate(_rotation + 270) * Matrix::Scale(0.01, 0.01) * Matrix::Translate(-250, -250);

  // iterate over points to calculate their z value
  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      // since rectangles have a width of 5, every 5th x and y coord we place a new anchor for the rectangle
      Vector point = function_transformation * Vector(i * 5, j * 5);  // now we have the transformed (x, y), to calculate the z and this location
      double z = Shepard(point.GetX(), point.GetY(), FunctionData, NoPoints);

      // needed for normalization
      f_min = std::max(f_min, z);
      f_max = std::min(f_max, z);

      // x and y values that we save are from before the transformation, as the parallelogram stays the same
      // we only needed the transformations to calculate the value of a function at the transformed coords
      points[i][j][0] = i * 5;
      points[i][j][1] = j * 5;
      points[i][j][2] = z;
    }
  }

  // now we define the transformation to get a pseudo 3d effect using a shear
  // translate back to the centre of the panel <- scale for psuedo camera angle / tilt <- apply the shear for psuedo 3d (negative so it shears to the right) <- scale the parallelogram a bit so it fits in the panel <- translate the (x, y) coords to the (0, 0) centre for proper scaling (rememeber we saved the untransformed pts)
  Matrix pseudo_3d = Matrix::Translate(250, 250) * Matrix::Scale(1, (50.0 + _tilt) / 200.0) * Matrix::Shear(0, -0.2) * Matrix::Scale(0.8, 0.8) * Matrix::Translate(-250, -250);

  // draw the parallelogram (made up of rectangles)
  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      Vector anchor = pseudo_3d * Vector(points[i][j][0], points[i][j][1]);
      double z = (points[i][j][2] - f_min) / (f_max - f_min);  // normalize to [0, 1.0]

      // set the colour of the rectangle
      wxColour voxel_colour;
      if (_isColour) {
        voxel_colour = wxColour(255 * z, 0, 255 * (1 - z));
      } else {
        voxel_colour = wxColour(255 * z, 255 * z, 255 * z); // cant use brace initialization because of narrowing conversions...
                                                            // and i'm too lazy to static_cast<unsigned int>() everything
      }

      dc.SetBrush(voxel_colour); // implicit conversion to brush
      dc.DrawRectangle(anchor.GetX(), anchor.GetY(), 5, std::min(-20.0, -80 * z)); // we need a minimum value such that we dont get empty spots
                                                                                  // -50 is so we draw upwards, not downwards
    }
  }
}

void GUIMyFrame1::m_panel1OnUpdateUI(wxUpdateUIEvent& event) {
  m_panel1->Refresh();
}

void GUIMyFrame1::m_button1_click(wxCommandEvent& event) {
  PrepareData(1);
}

void GUIMyFrame1::m_button2_click(wxCommandEvent& event) {
  PrepareData(2);
}

void GUIMyFrame1::m_button3_click(wxCommandEvent& event) {
  PrepareData(3);
}

void GUIMyFrame1::m_button4_click(wxCommandEvent& event) {
  PrepareData(4);
}

void GUIMyFrame1::m_cb_color_click(wxCommandEvent& event) {
  _isColour = m_cb_color->IsChecked();
}

void GUIMyFrame1::m_s_rotation_onscroll(wxScrollEvent& event) {
  m_st_rotation->SetLabel(wxString::Format("Obrot: %d stopni.", m_s_rotation->GetValue()));
  _rotation = m_s_rotation->GetValue();
}

void GUIMyFrame1::m_s_tilt_onscroll(wxScrollEvent& event) {
  _tilt = m_s_tilt->GetValue();
}
