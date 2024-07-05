#pragma once
#include<wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <drawingcanva.hpp>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);
private:
    void CreatePanels();
    void CreateControls();
    void CreateEvents();
    void OnAdd(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnManualMode(wxCommandEvent& event);
    void OnTestMode(wxCommandEvent& event);
    void OnProgrammerMode(wxCommandEvent& event);
    

    wxPanel *mainpanel;
    wxPanel *menupanel;
    wxPanel *managerpanel;
    wxPanel *relaypanel;
    wxPanel *controlpanel;
    wxBitmapButton *manualmodebutton;
    wxBitmapButton *progammermodebutton;
    wxBitmapButton *testmodebutton;
    wxBitmapButton *addbutton;
    wxBitmapButton *clearbutton;
    wxListCtrl *relaylist; 
    wxComboBox *baudrate; 
    wxComboBox *type;
    DrawingCanva *canva;
    wxDECLARE_EVENT_TABLE();
};

enum ButtonId {
    clearbuttonId = wxID_LAST + 1,
    addbuttonId = wxID_LAST + 2,
    testmodebuttonId = wxID_LAST + 3,
    progammermodebuttonId = wxID_LAST + 4,
    manualmodebuttonId = wxID_LAST + 5,
};


    