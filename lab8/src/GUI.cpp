///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.1.0-0-g733bf3d-dirty)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "GUI.h"

///////////////////////////////////////////////////////////////////////////

MyFrame1::MyFrame1( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 500,300 ), wxDefaultSize );

	wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizerControls;
	bSizerControls = new wxBoxSizer( wxVERTICAL );

	bSizerControls->SetMinSize( wxSize( 350,-1 ) );
	m_buttonLoadFile = new wxButton( this, wxID_ANY, wxT("Wczytaj obrazek"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerControls->Add( m_buttonLoadFile, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_buttonCensorship = new wxButton( this, wxID_ANY, wxT("Cenzura"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerControls->Add( m_buttonCensorship, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_buttonErode = new wxButton( this, wxID_ANY, wxT("Erode"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerControls->Add( m_buttonErode, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_checkBoxAnimation = new wxCheckBox( this, wxID_ANY, wxT("Animacja"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerControls->Add( m_checkBoxAnimation, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

	m_gaugeAnimProgress = new wxGauge( this, wxID_ANY, 60, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_gaugeAnimProgress->SetValue( 0 );
	m_gaugeAnimProgress->Hide();

	bSizerControls->Add( m_gaugeAnimProgress, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxRESERVE_SPACE_EVEN_IF_HIDDEN, 5 );

	m_textCtrlMetadata = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizerControls->Add( m_textCtrlMetadata, 1, wxALL|wxEXPAND, 5 );


	bSizerMain->Add( bSizerControls, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizerPanel;
	bSizerPanel = new wxBoxSizer( wxVERTICAL );

	m_panel_image = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizerPanel->Add( m_panel_image, 1, wxEXPAND | wxALL, 5 );


	bSizerMain->Add( bSizerPanel, 1, wxEXPAND, 5 );


	this->SetSizer( bSizerMain );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	m_buttonLoadFile->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonLoadFileOnButtonClick ), NULL, this );
	m_buttonCensorship->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonCensorshipOnButtonClick ), NULL, this );
	m_buttonErode->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonErodeOnButtonClick ), NULL, this );
	m_checkBoxAnimation->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBoxAnimationOnCheckBox ), NULL, this );
	m_panel_image->Connect( wxEVT_PAINT, wxPaintEventHandler( MyFrame1::m_panel_imageOnPaint ), NULL, this );
	m_panel_image->Connect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MyFrame1::m_panel_imageOnUpdateUI ), NULL, this );
}

MyFrame1::~MyFrame1()
{
	// Disconnect Events
	m_buttonLoadFile->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonLoadFileOnButtonClick ), NULL, this );
	m_buttonCensorship->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonCensorshipOnButtonClick ), NULL, this );
	m_buttonErode->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MyFrame1::m_buttonErodeOnButtonClick ), NULL, this );
	m_checkBoxAnimation->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MyFrame1::m_checkBoxAnimationOnCheckBox ), NULL, this );
	m_panel_image->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MyFrame1::m_panel_imageOnPaint ), NULL, this );
	m_panel_image->Disconnect( wxEVT_UPDATE_UI, wxUpdateUIEventHandler( MyFrame1::m_panel_imageOnUpdateUI ), NULL, this );

}
