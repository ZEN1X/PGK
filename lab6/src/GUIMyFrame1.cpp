#include "GUIMyFrame1.h"

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
    : MyFrame1(parent) {
  m_staticText1->SetLabel(_(L"Jasno\u015B\u0107"));
  m_b_threshold->SetLabel(_(L"Pr\u00F3g 128"));
  this->SetBackgroundColour(wxColor(192, 192, 192));
  m_scrolledWindow->SetScrollbars(25, 25, 52, 40);
  m_scrolledWindow->SetBackgroundColour(wxColor(192, 192, 192));

  m_scrolledWindow->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void GUIMyFrame1::MainFormClose(wxCloseEvent& event) {
  Destroy();
}

void GUIMyFrame1::m_scrolledWindow_paint(wxPaintEvent& event) {
  wxAutoBufferedPaintDC dc(m_scrolledWindow);
  m_scrolledWindow->DoPrepareDC(dc);
  Repaint(dc);
}

void GUIMyFrame1::m_scrolledWindow_update(wxUpdateUIEvent& event) {
  m_scrolledWindow->Refresh();
}

void GUIMyFrame1::m_b_grayscale_click(wxCommandEvent& event) {
  // Img_Cpy = Img_Org.ConvertToGreyscale().Copy();

  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();

  size_t width = Img_Cpy.GetWidth();
  size_t height = Img_Cpy.GetHeight();

  unsigned char* img_raw = Img_Cpy.GetData();

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      size_t pos = (y * width + x) * 3;
      double r = img_raw[pos] / 255.0;
      double g = img_raw[pos+1] / 255.0;
      double b = img_raw[pos+2] / 255.0;

      unsigned char new_r = (0.5 * std::sin(2 * M_PI * r) + 0.5) * 255;
      unsigned char new_g = (0.25 * (std::sin(2 * M_PI * g) + std::cos(2 * M_PI * r) + 2)) * 255;
      unsigned char new_b = (0.5 * std::cos(2 * M_PI * b) + 0.5) * 255;

      img_raw[pos] = new_r;
      img_raw[pos+1] = new_g;
      img_raw[pos+2] = new_b;
    }
  }
}

void GUIMyFrame1::m_b_blur_click(wxCommandEvent& event) {
  // i guess since we dont have a wxSlider or wxTextCtrl,
  // the radius is gonna get hardcoded
  Img_Cpy = Img_Org.Blur(10).Copy();
}

void GUIMyFrame1::m_b_mirror_click(wxCommandEvent& event) {
  Img_Cpy = Img_Org.Mirror().Copy();
}

void GUIMyFrame1::m_b_replace_click(wxCommandEvent& event) {
  // this directly modifies the object
  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();
  Img_Cpy.Replace(254, 0, 0, 0, 0, 255);
}

void GUIMyFrame1::m_b_rescale_click(wxCommandEvent& event) {
  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();
  Img_Cpy.Rescale(320, 240);
  // no need to do anything funky with Clear() etc, as we have
  // a PaintDC and have set SetBackgroundStyle(wxBG_STYLE_PAINT)
}

void GUIMyFrame1::m_b_rotate_click(wxCommandEvent& event) {
  Img_Cpy = Img_Org.Rotate(30 * M_PI / 180, {0, 0}).Copy();
}

void GUIMyFrame1::m_b_rotate_hue_click(wxCommandEvent& event) {
  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();
  Img_Cpy.RotateHue(0.5);  // angle is [-1, 1] for god knows what reason
                           // might've been as well 180.0 / 360.0
}

void GUIMyFrame1::m_b_mask_click(wxCommandEvent& event) {
  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();
  Img_Cpy.SetMaskFromImage(Img_Mask, 0, 0, 0);
}

void GUIMyFrame1::m_s_brightness_scroll(wxScrollEvent& event) {
  // Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
  // Brightness(...), ktora zmienia jasnosc. W tym miejscu nic nie
  // zmieniamy. Do uzupelnienia pozostaje funkcja Brightness(...)
  Brightness(m_s_brightness->GetValue() - 100);
  m_scrolledWindow->Refresh();
}

void GUIMyFrame1::m_s_contrast_scroll(wxScrollEvent& event) {
  // Tutaj, w reakcji na zmiane polozenia suwaka, wywolywana jest funkcja
  // Contrast(...), ktora zmienia kontrast. W tym miejscu nic nie
  // zmieniamy. Do uzupelnienia pozostaje funkcja Contrast(...)
  Contrast(m_s_contrast->GetValue() - 100);
  m_scrolledWindow->Refresh();
}

void GUIMyFrame1::m_b_prewitt_click(wxCommandEvent& event) {
  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();

  size_t width = Img_Cpy.GetWidth();
  size_t height = Img_Cpy.GetHeight();

  unsigned char* img_raw = Img_Cpy.GetData();
  unsigned char* new_img = static_cast<unsigned char*>(malloc(width * height * 3));  // wxWidgets docs call for malloc huh

  for (size_t y = 1; y < height - 1; ++y) {  // +1 offset to target the center of a 3x3 matrix
    for (size_t x = 1; x < width - 1; ++x) {
      int sum_r = 0;
      int sum_g = 0;
      int sum_b = 0;

      for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
          int px = (x + dx) * 3;          // x in the data array
          int py = (y + dy) * width * 3;  // y in the data array

          sum_r += dx * img_raw[py + px];
          sum_g += dx * img_raw[py + px + 1];
          sum_b += dx * img_raw[py + px + 2];
        }
      }
      // without abs, we get a cool effect, sorta what we want, but not exactly (some edges are missing)
      // with abs, we're 99% there, the issue is that it's brighter than the demo
      // division by 3.0 somehow makes it same as the provided demo (lol)

      sum_r = std::abs(sum_r) / 3.0;
      sum_g = std::abs(sum_g) / 3.0;
      sum_b = std::abs(sum_b) / 3.0;

      int idx = (y * width + x) * 3;
      new_img[idx] = std::clamp(sum_r, 0, 255);
      new_img[idx + 1] = std::clamp(sum_g, 0, 255);
      new_img[idx + 2] = std::clamp(sum_b, 0, 255);
    }
  }

  Img_Cpy.SetData(new_img);
}

void GUIMyFrame1::m_b_threshold_click(wxCommandEvent& event) {
  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();

  size_t img_raw_size = Img_Cpy.GetWidth() * Img_Cpy.GetHeight() * 3;  // w * h * channels
  unsigned char* img_raw = Img_Cpy.GetData();

  for (size_t i = 0; i < img_raw_size; ++i) {
    img_raw[i] = img_raw[i] > 128 ? 255 : 0;
  }
}

void GUIMyFrame1::Contrast(int value) {
  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();

  size_t img_raw_size = Img_Cpy.GetWidth() * Img_Cpy.GetHeight() * 3;  // w * h * channels
  unsigned char* img_raw = Img_Cpy.GetData();

  for (size_t i = 0; i < img_raw_size; ++i) {
    short min = 0;
    short max = 255;

    // contrast is naturally written as a number in the range [0, inf]
    // for example an OELD could have an inf contrast (division by 0 for a black pixel), while a 0 contrast means 1 color (gray)
    // we need to convert [-100, +100] into a range [0, inf)

    // a suitable mapping would be a rational function
    double contrast = (100.0 + value) / (101.0 - value);  // +1 to avoid division by 0 :/
    short new_color = (img_raw[i] - 128) * contrast + 128;
    img_raw[i] = std::clamp(new_color, min, max);
  }
}

void GUIMyFrame1::Repaint(wxDC& dc) {
  wxBitmap bitmap(Img_Cpy);           // Tworzymy tymczasowa bitmape na podstawie Img_Cpy
                                      //  wxClientDC dc(m_scrolledWindow);   // Pobieramy kontekst okna
                                      //  m_scrolledWindow->DoPrepareDC(dc); // Musimy wywolac w przypadku wxScrolledWindow, zeby suwaki prawidlowo dzialaly
  dc.DrawBitmap(bitmap, 0, 0, true);  // Rysujemy bitmape na kontekscie urzadzenia
}

void GUIMyFrame1::Brightness(int value) {
  // make a copy of the original first, since Img_Cpy might've been modified already
  Img_Cpy = Img_Org.Copy();

  // Img_Cpy.ChangeBrightness(value / 100.0); // i guess that's not what we want
  // Img_Cpy = Img_Org.ChangeLightness(value + 100); // that's neither
  // i guess we're doing some crude RGB trickery?

  size_t img_raw_size = Img_Cpy.GetWidth() * Img_Cpy.GetHeight() * 3;  // w * h * channels
  unsigned char* img_raw = Img_Cpy.GetData();

  // decayed pointer, thus no nice range-based for loop :(
  for (size_t i = 0; i < img_raw_size; ++i) {
    short min = 0;
    short max = 255;
    short new_color = img_raw[i] + value;
    img_raw[i] = std::clamp(new_color, min, max);
  }
  // one question: why? who hurt you
}