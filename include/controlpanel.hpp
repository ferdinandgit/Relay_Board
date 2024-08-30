#pragma once 
#include <wx/wx.h>
#include <vector>
#include <serialrelay.hpp>
#include <drawingcanva.hpp>
#include <wx/listctrl.h>
#include <interpreter.hpp>


class ControlPanel : public wxPanel {

public:

    ControlPanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    void CreateManuallayout(int relaynumber,relayboard board);
    void CreateManualControls(int relaynumber,relayboard board);
    void CreateTestlayout();
    void CreateTestControls(int id, relayboard board);
    void CreateProgrammerlayout();
    void CreateProgrammerControls();
    std::vector<Serialrelay*> GetOpenBoards();
    void AddBoard(Serialrelay* board);
    void RmBoard(int id);
    void InitControlPanel(); 
    void BoardControlsbuild();
    void AssignToBoard(int id);
    void AssignToNull();
    void SelectedItem(int id);
    
     

private:

    void Relay1Controls(relayboard board);
    void Relay2Controls(relayboard board);
    void Relay4Controls(relayboard board);
    void Relay8Controls(relayboard board);
    void Relay16Contols(relayboard board);
    void UpdateState();

    void OntestButton(wxCommandEvent &event);
    void OnbrowseButton(wxCommandEvent &event);
    void OnloadButton(wxCommandEvent &event);
    void OnaddButton(wxCommandEvent &event);
    void OnclearButton(wxCommandEvent &event);
    void OnstartButton(wxCommandEvent &event);
    void OnstopButton(wxCommandEvent &event);
    void OnK1(wxCommandEvent &event);
    void OnK2(wxCommandEvent &event);
    void OnK3(wxCommandEvent &event);
    void OnK4(wxCommandEvent &event);
    void OnK5(wxCommandEvent &event);
    void OnK6(wxCommandEvent &event);
    void OnK7(wxCommandEvent &event);
    void OnK8(wxCommandEvent &event);
    void OnK9(wxCommandEvent &event);
    void OnK10(wxCommandEvent &event);
    void OnK11(wxCommandEvent &event);
    void OnK12(wxCommandEvent &event);
    void OnK13(wxCommandEvent &event);
    void OnK14(wxCommandEvent &event);
    void OnK15(wxCommandEvent &event);
    void OnK16(wxCommandEvent &event);
    
    Serialrelay* activeboard;
    Interpreter* interpreter; 
    DrawingCanva* canva;
    wxPanel* programmerpanel;
    wxPanel* testpanel;
    wxPanel* displayPanel;
    std::vector<Serialrelay*> openboards;
    std::vector<Serialrelay*> programmerboards;
    int selecteditem = 0;
    std::string filePath;
    
    wxButton* testButton;
    wxButton* browseButton;
    wxButton* loadButton;
    wxButton* addButton;
    wxButton* clearButton;
    wxButton* startButton;
    wxButton* stopButton;
    wxButton* k1;
    wxButton* k2;
    wxButton* k3;
    wxButton* k4;
    wxButton* k5;
    wxButton* k6;
    wxButton* k7;
    wxButton* k8;
    wxButton* k9;
    wxButton* k10;
    wxButton* k11;
    wxButton* k12;
    wxButton* k13;
    wxButton* k14;
    wxButton* k15;
    wxButton* k16;

    wxStaticText* testrelaytext;
    wxStaticText* statek1;
    wxStaticText* statek2;
    wxStaticText* statek3;
    wxStaticText* statek4;
    wxStaticText* statek5;
    wxStaticText* statek6;
    wxStaticText* statek7;
    wxStaticText* statek8;
    wxStaticText* statek9;
    wxStaticText* statek10;
    wxStaticText* statek11;
    wxStaticText* statek12;
    wxStaticText* statek13;
    wxStaticText* statek14;
    wxStaticText* statek15;
    wxStaticText* statek16;

    wxTextCtrl* m_filePathCtrl;

    wxListCtrl* m_listCtrl1;
    wxListCtrl* m_listCtrl2;

    wxDECLARE_EVENT_TABLE();
};

enum ControlpanelButtonId{
    k1Id = wxID_LAST + 6,
    k2Id = wxID_LAST + 7,
    k3Id = wxID_LAST + 8,
    k4Id = wxID_LAST + 9,
    k5Id = wxID_LAST + 10,
    k6Id = wxID_LAST + 11,
    k7Id = wxID_LAST + 12,
    k8Id = wxID_LAST + 13,
    k9Id = wxID_LAST + 14,
    k10Id = wxID_LAST + 15,
    k11Id = wxID_LAST + 16,
    k12Id = wxID_LAST + 17,
    k13Id = wxID_LAST + 18,
    k14Id = wxID_LAST + 19,
    k15Id = wxID_LAST + 20,
    k16Id = wxID_LAST + 21,
    testButtonId = wxID_LAST + 22,
    loadButtonID = wxID_LAST + 23,
    addButtonID  = wxID_LAST + 24,
    clearButtonID = wxID_LAST + 25,
    startButtonID = wxID_LAST + 26,
    stopButtonID  = wxID_LAST + 27,
    browseButtonID = wxID_LAST + 28
};
