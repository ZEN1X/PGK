#include <wx/wx.h>
#include "GUIMyFrame1.h"

class MyApp : public wxApp {
 public:
  bool OnInit() override;
  int OnExit() override;
};

IMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
  GUIMyFrame1* mainFrame = new GUIMyFrame1(NULL);

  mainFrame->Show(true);
  SetTopWindow(mainFrame);

  return true;
}

int MyApp::OnExit() {
  return 0;
}
