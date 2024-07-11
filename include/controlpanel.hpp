#pragma once 
#include <wx/wx.h>
#include <vector>
#include <serialrelay.hpp>
#include <drawingcanva.hpp>

using relay::Serialrelay;
class ControlPanel : public wxPanel {

public:

    ControlPanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    void CreateManuallayout(int relaynumber);
    void CreateManualControls(int relaynumber);
    void CreateManualevent();
    void CreateTestlayout();
    void CreateTestControls();
    void CreateTestevent();
    void CreateProgrammerlayout();
    void CreateProgrammerControls();
    void CreateProgrammerevent();
    void AddBoard(Serialrelay board);
    void RmBoard(Serialrelay board);
    void InitControlPanel(); 

private:
    DrawingCanva* canva;
    std::vector<Serialrelay> boards;
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
    k16Id = wxID_LAST + 20,
};
