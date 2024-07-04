#include <mainframe.hpp>
#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/listctrl.h>

MainFrame::MainFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title){
    CreatePanels();
    CreateControls();
}

void MainFrame::CreatePanels(){
    
    const auto margin = FromDIP(10);
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    this->mainpanel = new wxPanel(this, wxID_ANY);
    this->SetBackgroundColour(this->mainpanel->GetBackgroundColour());
    auto sizer = new wxGridBagSizer(margin,margin); 
    
    //Manual mode Panel items 
    std::vector<std::pair<wxGBPosition,wxGBSpan>> items =  {
    {{0,0},{1,2}},
    {{1,0},{1,1}},
    {{2,0},{1,1}},
    {{1,1},{2,1}}
    };

    //Manuel mode frame Panel definition 
    auto initialsize = wxSize(75,75);
    //define menu panel 
    this->menupanel = new wxPanel(this->mainpanel,wxID_ANY, wxDefaultPosition,initialsize);
    this->menupanel->SetBackgroundColour(wxColour(100,100,100));
    sizer->Add(menupanel,items[0].first,items[0].second,wxEXPAND);
    //define relay manager panel
    this->managerpanel = new wxPanel(this->mainpanel,wxID_ANY, wxDefaultPosition,initialsize);
    this->managerpanel->SetBackgroundColour(wxColour(100,100,100));
    sizer->Add(this->managerpanel,items[1].first,items[1].second,wxEXPAND);
    //define relay panel 
    this->relaypanel = new wxPanel(this->mainpanel,wxID_ANY, wxDefaultPosition,initialsize);
    this->relaypanel->SetBackgroundColour(wxColour(100,100,100));
    sizer->Add(this->relaypanel,items[2].first,items[2].second,wxEXPAND);
    //define contorl panel
    this->controlpanel = new wxPanel(this->mainpanel,wxID_ANY, wxDefaultPosition,initialsize);
    this->controlpanel->SetBackgroundColour(wxColour(100,100,100));
    sizer->Add(this->controlpanel,items[3].first,items[3].second,wxEXPAND);
    //Sizer parameter for panel sizing 
    sizer->AddGrowableCol(0);
    sizer->AddGrowableCol(1);
    sizer->AddGrowableRow(2);
    sizer->SetMinSize(FromDIP(wxSize(600,400)));
    this->mainpanel->SetSizer(sizer);
    mainSizer->Add(this->mainpanel, 1, wxEXPAND | wxALL, margin);
    this->SetSizerAndFit(mainSizer);
}

void MainFrame::CreateControls(){
    int buttonwidth = 75;
    int buttonhight = 75;
    auto buttonsize = wxSize(75,75);
    //menu panel controls 
    wxBitmap manualicon;
    manualicon.LoadFile("iconmanual.bmp", wxBITMAP_TYPE_BMP);
    this->manualmodebutton = new wxBitmapButton(this->menupanel,manualmodebuttonId,manualicon, wxPoint(0,0),buttonsize, 0);
    wxBitmap programmericon;
    programmericon.LoadFile("iconmanual.bmp", wxBITMAP_TYPE_BMP);
    this->progammermodebutton = new wxBitmapButton(this->menupanel,progammermodebuttonId,programmericon, wxPoint(buttonwidth,0),buttonsize, 0);
    wxBitmap testicon;
    testicon.LoadFile("iconmanual.bmp", wxBITMAP_TYPE_BMP);
    this->testmodebutton = new wxBitmapButton(this->menupanel,testmodebuttonId,testicon, wxPoint(buttonwidth*2,0),buttonsize, 0);
    //manager panel control 
    wxBitmap addicon;
    addicon.LoadFile("iconmanual.bmp", wxBITMAP_TYPE_BMP);
    this->addbutton = new wxBitmapButton(this->managerpanel,addbuttonId, addicon, wxPoint(0,0),buttonsize, 0);
    wxBitmap clearicon;
    clearicon.LoadFile("iconmanual.bmp", wxBITMAP_TYPE_BMP);
    this->clearbutton = new wxBitmapButton(this->managerpanel,clearbuttonId, addicon, wxPoint(buttonwidth,0),buttonsize, 0);
    

    wxBoxSizer* sizerrelaypanel = new wxBoxSizer(wxHORIZONTAL);
    relaypanel->SetSizer(sizerrelaypanel);
    this->relaylist = new wxListCtrl(relaypanel, -1, wxDefaultPosition,wxDefaultSize, wxLC_REPORT|wxRAISED_BORDER|wxLC_VRULES);
    sizerrelaypanel->Add(relaylist, 1, wxGROW, 0);
    this->relaylist->InsertColumn(0, _("id"), wxLIST_FORMAT_LEFT, 30);
    this->relaylist->InsertColumn(1, _("type"), wxLIST_FORMAT_LEFT, 80);
    this->relaylist->InsertColumn(2, _("relay number"), wxLIST_FORMAT_LEFT, 130);
    this->relaylist->InsertColumn(3, _("baudrate"), wxLIST_FORMAT_LEFT, 800);


    // testing 

    wxBoxSizer* siercontrolpanel;      
}


/*wxArrayString strings;
    strings.Add(wxT("1"));
    strings.Add(wxT("2"));
    strings.Add(wxT("3"));
    strings.Add(wxT("4"));
    wxComboBox *boardtype = new wxComboBox(this->managerpanel, -1, wxT("..."), wxPoint(buttonwidth*2,0),buttonsize, strings);*/