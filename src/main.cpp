#include <wx/wx.h>
#include <wx/gbsizer.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Seeit Relay Controller", wxDefaultPosition, wxSize(600,600));
    frame->SetBackgroundColour(wxColour(0,0,0));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    const auto margin = FromDIP(10);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    wxPanel *panel = new wxPanel(this, wxID_ANY);
    this->SetBackgroundColour(panel->GetBackgroundColour());

    auto Manual_Mode_Button = new wxButton(panel, wxID_ANY, "ManualMode");

    auto sizer = new wxGridBagSizer(margin,margin);
    std::vector<std::pair<wxGBPosition,wxGBSpan>> items =  {
    {{0,0},{1,2}},
    {{1,0},{1,1}},
    {{2,0},{1,1}},
    {{1,1},{2,1}}
    };

    for(auto &item : items){
        auto initialsize = sizer->GetEmptyCellSize() * 2 ;
        auto p = new wxPanel(panel,wxID_ANY, wxDefaultPosition,initialsize);
        p->SetBackgroundColour(wxColour(100,100,200));
        sizer->Add(p,item.first,item.second,wxEXPAND);
    };
    
    
    sizer->AddGrowableCol(0);
    sizer->AddGrowableCol(1);
    sizer->AddGrowableRow(2);


    sizer->SetMinSize(FromDIP(wxSize(600,400)));

    

    panel->SetSizer(sizer);

    mainSizer->Add(panel, 1, wxEXPAND | wxALL, margin);
    this->SetSizerAndFit(mainSizer);
    


   
}
