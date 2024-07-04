#pragma once 
#include <wx/wx.h>
#include <vector.h>

class ControlPanel : public wxPanel {

public:
    ControlPanel(wxWindow* parent);
    void CreateManuallayout(int relaynumber);
    void CreateManualControls();
    void CreateManualevent();
    void CreateTestlayout();
    void CreateTestControls();
    void CreateTestevent();
    void CreateProgrammerlayout();
    void CreateProgrammerControls();
    void CreateProgrammerevent();

private: 
    std::vector<wxPanel> relays;
};


