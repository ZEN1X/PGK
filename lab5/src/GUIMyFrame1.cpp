#include "GUIMyFrame1.h"
#include <fstream>
#include <vector>

struct Point {
  float x, y, z;
  Point(float _x, float _y, float _z)
      : x(_x), y(_y), z(_z) {}
};

struct Color {
  int R, G, B;
  Color(int _R, int _G, int _B)
      : R(_R), G(_G), B(_B) {}
};

struct Segment {
  Point begin, end;
  Color color;
  Segment(Point _begin, Point _end, Color _color)
      : begin(_begin), end(_end), color(_color) {}
};

std::vector<Segment> data;

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
    : MyFrame1(parent) {
  m_button_load_geometry->SetLabel(_("Wczytaj Geometri\u0119"));
  m_staticText25->SetLabel(_("Obr\u00F3t X:"));
  m_staticText27->SetLabel(_("Obr\u00F3t Y:"));
  m_staticText29->SetLabel(_("Obr\u00F3t Z:"));

  WxSB_TranslationX->SetRange(0, 200);
  WxSB_TranslationX->SetValue(100);
  WxSB_TranslationY->SetRange(0, 200);
  WxSB_TranslationY->SetValue(100);
  WxSB_TranslationZ->SetRange(0, 200);
  WxSB_TranslationZ->SetValue(100);

  WxSB_RotateX->SetRange(0, 360);
  WxSB_RotateX->SetValue(0);
  WxSB_RotateY->SetRange(0, 360);
  WxSB_RotateY->SetValue(0);
  WxSB_RotateZ->SetRange(0, 360);
  WxSB_RotateZ->SetValue(0);

  WxSB_ScaleX->SetRange(1, 200);
  WxSB_ScaleX->SetValue(100);
  WxSB_ScaleY->SetRange(1, 200);
  WxSB_ScaleY->SetValue(100);
  WxSB_ScaleZ->SetRange(1, 200);
  WxSB_ScaleZ->SetValue(100);

  observerZ = -2;
  WxPanel->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void GUIMyFrame1::MainFormClose(wxCloseEvent& event) {
  Destroy();
}

void GUIMyFrame1::WxPanel_Paint(wxPaintEvent& event) {
  wxAutoBufferedPaintDC dc(WxPanel);
  Repaint(dc);
}

void GUIMyFrame1::WxPanel_Repaint(wxUpdateUIEvent& event) {
  WxPanel->Refresh();
}

void GUIMyFrame1::m_button_load_geometry_click(wxCommandEvent& event) {
  wxFileDialog WxOpenFileDialog(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("Geometry file (*.geo)|*.geo"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (WxOpenFileDialog.ShowModal() == wxID_OK) {
    double x1, y1, z1, x2, y2, z2;
    int r, g, b;

    std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
    if (in.is_open()) {
      data.clear();
      while (!in.eof()) {
        in >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> r >> g >> b;
        data.push_back(Segment(Point(x1, y1, z1), Point(x2, y2, z2), Color(r, g, b)));
      }
      in.close();
    }
  }
}

void GUIMyFrame1::Scrolls_Updated(wxScrollEvent& event) {
  WxST_TranslationX->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationX->GetValue() - 100) / 50.0));
  WxST_TranslationY->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationY->GetValue() - 100) / 50.0));
  WxST_TranslationZ->SetLabel(wxString::Format(wxT("%g"), (WxSB_TranslationZ->GetValue() - 100) / 50.0));

  WxST_RotateX->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateX->GetValue()));
  WxST_RotateY->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateY->GetValue()));
  WxST_RotateZ->SetLabel(wxString::Format(wxT("%d"), WxSB_RotateZ->GetValue()));

  WxST_ScaleX->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleX->GetValue() / 100.0));
  WxST_ScaleY->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleY->GetValue() / 100.0));
  WxST_ScaleZ->SetLabel(wxString::Format(wxT("%g"), WxSB_ScaleZ->GetValue() / 100.0));

  WxPanel->Refresh();
}

Matrix4 GUIMyFrame1::Translate(double dx, double dy, double dz) {
  Matrix4 Translation;

  Translation.data[0][0] = 1;
  Translation.data[0][3] = dx;
  Translation.data[1][1] = 1;
  Translation.data[1][3] = dy;
  Translation.data[2][2] = 1;
  Translation.data[2][3] = dz;

  return Translation;
}

Matrix4 GUIMyFrame1::RotateX(double alpha) {
  Matrix4 Rotation;

  double radians = alpha * M_PI / 180.0;

  Rotation.data[0][0] = 1;
  Rotation.data[1][1] = std::cos(radians);
  Rotation.data[1][2] = -std::sin(radians);
  Rotation.data[2][1] = std::sin(radians);
  Rotation.data[2][2] = std::cos(radians);

  return Rotation;
}

Matrix4 GUIMyFrame1::RotateY(double alpha) {
  Matrix4 Rotation;

  double radians = alpha * M_PI / 180.0;

  Rotation.data[0][0] = std::cos(radians);
  Rotation.data[0][2] = std::sin(radians);
  Rotation.data[1][1] = 1;
  Rotation.data[2][0] = -std::sin(radians);
  Rotation.data[2][2] = std::cos(radians);

  return Rotation;
}

Matrix4 GUIMyFrame1::RotateZ(double alpha) {
  Matrix4 Rotation;

  double radians = alpha * M_PI / 180.0;

  Rotation.data[0][0] = std::cos(radians);
  Rotation.data[0][1] = -std::sin(radians);
  Rotation.data[1][0] = std::sin(radians);
  Rotation.data[1][1] = std::cos(radians);
  Rotation.data[2][2] = 1;

  return Rotation;
}

Matrix4 GUIMyFrame1::Scale(double Sx, double Sy, double Sz) {
  Matrix4 Scaling;

  Scaling.data[0][0] = Sx;
  Scaling.data[1][1] = Sy;
  Scaling.data[2][2] = Sz;

  return Scaling;
}

Matrix4 GUIMyFrame1::FlipY() {
  Matrix4 Flip;
  Flip.data[0][0] = 1;
  Flip.data[1][1] = -1;
  Flip.data[2][2] = 1;

  return Flip;
}

Matrix4 GUIMyFrame1::Perspective() {
  Matrix4 Perspectivize;

  Perspectivize.data[0][0] = 1;
  Perspectivize.data[1][1] = 1;
  Perspectivize.data[3][2] = 1.0 / -observerZ;

  return Perspectivize;
}

void GUIMyFrame1::Normalize(Vector4& point) {
  double w = point.data[3];

  point.Set(point.GetX() / w, point.GetY() / w, point.GetZ() / w);
}

bool GUIMyFrame1::IsBehindClipPlane(Vector4 point) {
  return point.GetZ() < observerZ;
}

bool GUIMyFrame1::Clip(Vector4& start, Vector4& end) {
  // return value decides whether to attempt drawing or not

  bool start_behind = IsBehindClipPlane(start);
  bool end_behind = IsBehindClipPlane(end);

  if (start_behind && end_behind) {
    return false;  // the whole segment is behind the plane, abort drawing
    // early return
  } else if (start_behind || end_behind) {
    // one of ends of the segment is behind the plane
    // decide which one is behind and which in front
    Vector4 behind = start_behind ? start : end;
    Vector4 front = start_behind ? end : start;

    // calculate the interpolation ratio
    double behind_to_observer_distance = std::abs(observerZ - behind.GetZ());
    double front_to_behind_distance = std::abs(front.GetZ() - behind.GetZ());
    double interp_ratio = behind_to_observer_distance / front_to_behind_distance;

    // set correct x, y and z according to the newly clipped z value
    behind.Set(behind.GetX() + (front.GetX() - behind.GetX()) * interp_ratio, behind.GetY() + (front.GetY() - behind.GetY()) * interp_ratio, observerZ + 0.0002);

    // update the start and end points
    // the "direction" doesnt matter, as its a line / segment, not a vector
    start = behind;
    end = front;

    // now we can draw
    return true;
  } else {
    // no clipping occurs
    return true;
  }
}

void GUIMyFrame1::Repaint(wxDC& dc) {
  // get the slider values
  double dx = (WxSB_TranslationX->GetValue() - 100) / 50.0;
  double dy = (WxSB_TranslationY->GetValue() - 100) / 50.0;
  double dz = (WxSB_TranslationZ->GetValue() - 100) / 50.0;

  double degX = WxSB_RotateX->GetValue();
  double degY = WxSB_RotateY->GetValue();
  double degZ = WxSB_RotateZ->GetValue();

  double Sx = WxSB_ScaleX->GetValue() / 100.0;
  double Sy = WxSB_ScaleY->GetValue() / 100.0;
  double Sz = WxSB_ScaleZ->GetValue() / 100.0;

  // get the panel dimensions
  double width = WxPanel->GetSize().GetWidth();
  double height = WxPanel->GetSize().GetHeight();

  // prepare the panel
  dc.SetBackground(*wxWHITE_BRUSH);
  dc.Clear();

  // get the matrices
  Matrix4 t = Translate(dx, dy, dz) * RotateX(-degX) * RotateY(-degY) * RotateZ(-degZ) * Scale(Sx, Sy, Sz);

  for (Segment curr : data) {
    // each segment has its own color
    dc.SetPen(wxPen(wxColour(curr.color.R, curr.color.G, curr.color.B)));

    Vector4 begin, end;
    begin.Set(curr.begin.x, curr.begin.y, curr.begin.z);
    end.Set(curr.end.x, curr.end.y, curr.end.z);

    // for now transform just object-wise
    begin = t * begin;
    end = t * end;

    if (Clip(begin, end)) {
      Matrix4 project_and_view = Translate(width / 2.0, height / 2.0, 0) * Scale(width / 2.0, height / 2.0, 1) * FlipY() * Perspective();
      begin = project_and_view * begin;
      end = project_and_view * end;

      // non-affine transformation, thus we need to normalize
      Normalize(begin);
      Normalize(end);

      dc.DrawLine(begin.GetX(), begin.GetY(), end.GetX(), end.GetY());
    }
  }
}
