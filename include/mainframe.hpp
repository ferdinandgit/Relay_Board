#pragma once
#include<wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <drawingcanva.hpp>
#include <controlpanel.hpp>
#include <wx/timer.h>

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
    void OnTimer(wxTimerEvent& event);
    void DoubleClickItem(wxListEvent& event); 
    void OnItemSelected(wxListEvent& event);
    void OnLogo(wxCommandEvent& event);
    void OnRefresh(wxCommandEvent& event);
    void  GetDevices();
    
    int manual = 1;
    int programmer =0;
    int test = 0;
    bool teststarted = false;
    onlinedev dev; 

    wxTimer *refreshtimer;
    wxPanel *mainpanel;
    wxPanel *menupanel;
    wxPanel *managerpanel;
    wxPanel *relaypanel;
    ControlPanel *controlpanel;
    wxBitmapButton *manualmodebutton;
    wxBitmapButton *progammermodebutton;
    wxBitmapButton *testmodebutton;
    wxButton *addbutton;
    wxButton *clearbutton;
    wxButton *logobutton; 
    wxButton* refreshbutton;
    wxListCtrl *relaylist; 
    wxComboBox *baudrate; 
    wxComboBox *boardtype;
    wxComboBox *boardrelaynumber;
    wxComboBox *port; 
    wxDECLARE_EVENT_TABLE();
};

enum EltId {
    logobuttonId = wxID_LAST,
    clearbuttonId = wxID_LAST + 1,
    addbuttonId = wxID_LAST + 2,
    testmodebuttonId = wxID_LAST + 3,
    progammermodebuttonId = wxID_LAST + 4,
    manualmodebuttonId = wxID_LAST + 5,
    timerid = wxID_LAST + 6,
    listctrlid = wxID_LAST +7,
    refreshbuttonId = wxID_LAST+8
};


    