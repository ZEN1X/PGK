#include "GUIMyFrame1.h"
#include <wx/dcbuffer.h>
#include <wx/filedlg.h>
#include <FreeImage.h>
// #define cimg_display 0 // i dont want to link against X11 (eww), why is it on by default?
// it's now in the CMakeLists.txt
#include <CImg.h>

GUIMyFrame1::GUIMyFrame1(wxWindow* parent)
    : MyFrame1(parent), hasAnimBeenGenerated{false}, frame_no{0} {
  wxImage::AddHandler(new wxJPEGHandler);  // .jpg handler for opening a file
  m_panel_image->SetBackgroundStyle(wxBG_STYLE_PAINT);

  timer.Bind(wxEVT_TIMER, &GUIMyFrame1::OnTimer, this);
}

GUIMyFrame1::~GUIMyFrame1() {
  timer.Unbind(wxEVT_TIMER, &GUIMyFrame1::OnTimer, this);
}

void GUIMyFrame1::m_buttonLoadFileOnButtonClick(wxCommandEvent& event) {
  wxFileDialog openImageDialog(this, _("Wybierz obrazek"), "", "", "pliki JPG/JPEG (*.jpeg;*.jpg)|*.jpeg;*.jpg");

  if (openImageDialog.ShowModal() == wxID_CANCEL) {
    return;  // early return to abort the file loading if the user has canceled the selection
  }

  // otherwise, load the image
  // first load it temporalily using FreeImage to extract the EXIF data, we will delete this shortly
  FIBITMAP* fimg_bitmap = FreeImage_Load(FIF_JPEG, openImageDialog.GetPath(), JPEG_DEFAULT);

  // get the metadata
  std::string metadata_string = "Rozmiar obrazka: " + std::to_string(FreeImage_GetWidth(fimg_bitmap)) + "x" + std::to_string(FreeImage_GetHeight(fimg_bitmap)) + "\n";
  metadata_string += "EXIF Info:\n";

  FITAG* tag;
  FIMETADATA* fimg_metadata = FreeImage_FindFirstMetadata(FIMD_EXIF_MAIN, fimg_bitmap, &tag);

  if (fimg_metadata) {
    do {
      metadata_string += std::string(FreeImage_GetTagKey(tag)) + ": " + std::string(FreeImage_TagToString(FIMD_EXIF_MAIN, tag)) + "\n";
    } while (FreeImage_FindNextMetadata(fimg_metadata, &tag));
  }

  FreeImage_FindCloseMetadata(fimg_metadata);

  m_textCtrlMetadata->ChangeValue(metadata_string);  // as to not generate and wxEVT_TEXT event

  FreeImage_Unload(fimg_bitmap);

  // now load the image into a wxImage, for use in wxWidgets window
  img_org.LoadFile(openImageDialog.GetPath(), wxBITMAP_TYPE_JPEG);
  img_cpy = img_org;

  hasAnimBeenGenerated = false;
}

void GUIMyFrame1::m_buttonCensorshipOnButtonClick(wxCommandEvent& event) {
  if (img_org.IsOk()) {
    img_cpy = img_org;  // return to the original image as it could've been changed beforehand

    // dimensions and offset for the blur effect
    int width = 0.2 * img_cpy.GetWidth();
    int height = 0.2 * img_cpy.GetHeight();
    int offset_x = 0.55 * img_cpy.GetWidth();
    int offset_y = 0.03 * img_cpy.GetHeight();

    cimg_library::CImg<unsigned char> cimg_img(width, height, 1, 3);  // create 256-valued RGB image using CImg
                                                                      // it's only as big as needed for the blur

    // effectively copy from wxImage to CImg
    for (size_t x = 0; x < width; ++x) {
      for (size_t y = 0; y < height; ++y) {
        cimg_img(x, y, 0) = img_cpy.GetRed(offset_x + x, offset_y + y);
        cimg_img(x, y, 1) = img_cpy.GetGreen(offset_x + x, offset_y + y);
        cimg_img(x, y, 2) = img_cpy.GetBlue(offset_x + x, offset_y + y);
      }
    }

    // apply the blur to that cutout (inside of cimg_img)
    cimg_img.blur(5);  // anisotropic blur with stdev ...

    // "copy" (or rather set) the cimg_img back to img_cpy
    for (size_t x = 0; x < width; ++x) {
      for (size_t y = 0; y < height; ++y) {
        img_cpy.SetRGB(offset_x + x, offset_y + y, cimg_img(x, y, 0), cimg_img(x, y, 1), cimg_img(x, y, 2));
        // supposedly this is slow according to wxWidgets devs, but it works fast on my machine :)
        // could've used GetData and SetData but oh well... (i totally didn't forget about that...)
      }
    }
  }
}

void GUIMyFrame1::m_buttonErodeOnButtonClick(wxCommandEvent& event) {
  // copy paste of Censorship
  if (img_org.IsOk()) {
    img_cpy = img_org;  // return to the original image as it could've been changed beforehand

    // dimensions of the image
    int width = img_cpy.GetWidth();
    int height = img_cpy.GetHeight();

    cimg_library::CImg<unsigned char> cimg_img(width, height, 1, 3);  // create 256-valued RGB image using CImg

    // effectively copy from wxImage to CImg
    for (size_t x = 0; x < width; ++x) {
      for (size_t y = 0; y < height; ++y) {
        cimg_img(x, y, 0) = img_cpy.GetRed(x, y);
        cimg_img(x, y, 1) = img_cpy.GetGreen(x, y);
        cimg_img(x, y, 2) = img_cpy.GetBlue(x, y);
      }
    }

    // apply the erode effect to the whole image
    cimg_img.erode(5);  // erode effect with square of size ...

    // "copy" (or rather set) the cimg_img back to img_cpy
    for (size_t x = 0; x < width; ++x) {
      for (size_t y = 0; y < height; ++y) {
        img_cpy.SetRGB(x, y, cimg_img(x, y, 0), cimg_img(x, y, 1), cimg_img(x, y, 2));
        // supposedly this is slow according to wxWidgets devs, but it works fast on my machine :)
        // could've used GetData and SetData but oh well... (i totally didn't forget about that...)
      }
    }
  }
}

void GUIMyFrame1::OnTimer(wxTimerEvent& event) {
  m_panel_image->Refresh();
}

void GUIMyFrame1::m_checkBoxAnimationOnCheckBox(wxCommandEvent& event) {
  if (event.IsChecked()) {
    timer.Start(1000 / 60);
  } else {
    timer.Stop();
  }

  if (img_org.IsOk() && m_checkBoxAnimation->IsChecked() && !hasAnimBeenGenerated) {
    // show the progress bar
    m_gaugeAnimProgress->Show();

    // get the dimensions
    int width = img_org.GetWidth();
    int height = img_org.GetHeight();

    // need to convert wxImage to CImg so that we can call mult()
    cimg_library::CImg<unsigned char> base_img(width, height, 1, 3);

    for (size_t x = 0; x < width; ++x) {
      for (size_t y = 0; y < height; ++y) {
        base_img(x, y, 0) = img_cpy.GetRed(x, y);
        base_img(x, y, 1) = img_cpy.GetGreen(x, y);
        base_img(x, y, 2) = img_cpy.GetBlue(x, y);
      }
    }

    // now we create the bitmaps for the circle (we will mirror it to get the other one)
    cimg_library::CImg<float> gauss(width, height, 1, 3);

    // we also need a CImg for the current frame
    cimg_library::CImg<float> frame_cimg(width, height, 1, 3);

    // "raw bitmap" that we will use to set array of wxBitmaps with (first wxImage though)
    // unsigned char frame_raw[width*height*3];
    unsigned char* frame_raw = static_cast<unsigned char*>(malloc(width * height * 3 * sizeof(unsigned char)));

    // loop over the 60 frames
    for (size_t i = 0; i < 60; ++i) {
      // m_gaugeAnimProgress->SetValue(i + 1);
      // m_gaugeAnimProgress->Refresh();
      // m_gaugeAnimProgress->Update();
      // something makes it not work when refreshing a PaintDC...

      // make a copy of the base_img
      frame_cimg = base_img;

      // specify the coords at which to draw the gaussian circle
      // ellipsis is simply achieved because the image is a rectangle and not a square
      float x = (width / 2.0) + (width * 0.2) * std::cos(i / 60.0 * M_PI);
      float y = (height / 2.0) + (height * 0.2) * std::sin(i / 60.0 * M_PI);

      // color is simply white
      const float color[] = {1, 1, 1};

      gauss.draw_gaussian(x, y, 80, color, 1);

      frame_cimg.mul(gauss + gauss.get_mirror("xy"));  // circle + circle mirrored around (0; 0) times the image

      // "draw" to the "raw bitmap"
      // CImg data is structured as RRR...GGG...BBB instead of RGB...RGB...RGB
      for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
          size_t coordinate = (y * width) + x;
          frame_raw[coordinate * 3 + 0] = frame_cimg.data()[coordinate];
          frame_raw[coordinate * 3 + 1] = frame_cimg.data()[coordinate + width * height];
          frame_raw[coordinate * 3 + 2] = frame_cimg.data()[coordinate + width * height * 2];
        }
      }

      wxImage frame_img(width, height, frame_raw, true);
      anim_frames[i] = wxBitmap(frame_img);
    }

    free(frame_raw);
    hasAnimBeenGenerated = true;
    m_gaugeAnimProgress->Hide();
  }
}

void GUIMyFrame1::m_panel_imageOnPaint(wxPaintEvent& event) {
  wxAutoBufferedPaintDC dc(m_panel_image);

  if (img_cpy.IsOk()) {
    if (m_checkBoxAnimation->IsChecked()) {
      // wxBitmap::Rescale(anim_frames[frame_no], dc.GetSize());
      dc.DrawBitmap(anim_frames[frame_no], 0, 0);
      frame_no = (frame_no + 1) % 60;
    } else {
      wxBitmap bmp_from_image_copy(img_cpy);
      wxBitmap::Rescale(bmp_from_image_copy, dc.GetSize());
      dc.DrawBitmap(bmp_from_image_copy, 0, 0);
    }
  }
}

void GUIMyFrame1::m_panel_imageOnUpdateUI(wxUpdateUIEvent& event) {
  m_panel_image->Refresh();
}
