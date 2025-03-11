///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d-dirty)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef GUI_H
#define GUI_H


#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/button.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checkbox.h>
#include <wx/gauge.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MyFrame1
///////////////////////////////////////////////////////////////////////////////
class MyFrame1 : public wxFrame
{
	private:

	protected:
		wxButton* m_buttonLoadFile;
		wxButton* m_buttonCensorship;
		wxButton* m_buttonErode;
		wxCheckBox* m_checkBoxAnimation;
		wxGauge* m_gaugeAnimProgress;
		wxTextCtrl* m_textCtrlMetadata;
		wxPanel* m_panel_image;

		// Virtual event handlers, override them in your derived class
		virtual void m_buttonLoadFileOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonCensorshipOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_buttonErodeOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_checkBoxAnimationOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_panel_imageOnPaint( wxPaintEvent& event ) { event.Skip(); }
		virtual void m_panel_imageOnUpdateUI( wxUpdateUIEvent& event ) { event.Skip(); }


	public:

		MyFrame1( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = "GFK Lab 08 by Jan Kwiatkowski", const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1000,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~MyFrame1();

};



#endif /* GUI_H */
