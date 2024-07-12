#include<controlpanel.hpp>
#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/listctrl.h>
#include <drawingcanva.hpp>

ControlPanel::ControlPanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size): wxPanel(parent, id, pos, size){
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void ControlPanel::InitControlPanel(){
    this->SetBackgroundColour(wxColour(100,100,100));
    wxSizer *controlpanelsizer = new wxBoxSizer(wxVERTICAL);
    auto init = new DrawingCanva(0,this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    controlpanelsizer->Add(init,1,wxEXPAND | wxALL,0);
    this->SetSizerAndFit(controlpanelsizer);
}

void ControlPanel::CreateManuallayout(int relaynumber){
    this->DestroyChildren();
    this->canva = new DrawingCanva(2,this, wxID_ANY, wxDefaultPosition,this->GetSize());
    this->GetSizer()->Add(canva,1,wxEXPAND);
}

void ControlPanel::CreateProgrammerlayout(){
    this->DestroyChildren();
    programmerpanel = new wxPanel(this,wxID_ANY,wxDefaultPosition,this->GetSize());
    programmerpanel->SetBackgroundColour(wxColour(100,100,100));
    this->GetSizer()->Add(programmerpanel,1,wxEXPAND,0);
   
}

void ControlPanel::CreateProgrammerControls(){
    wxBoxSizer* programctrlsizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* topcontrols = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* bottomcontrols = new wxBoxSizer(wxHORIZONTAL); 

    program = new wxListCtrl(programmerpanel, -1, wxDefaultPosition,wxDefaultSize, wxLC_REPORT|wxRAISED_BORDER|wxLC_VRULES);
    program->InsertColumn(0,"Id",wxLIST_ALIGN_LEFT,90);
    for(int k = 1;k<=16;k++){
        program->InsertColumn(k,"K"+std::to_string(k),wxLIST_ALIGN_LEFT,40);
    }

    wxButton* save = new wxButton(programmerpanel ,wxID_ANY,"Save",wxDefaultPosition,wxSize(50,20));
    wxButton* open = new wxButton(programmerpanel ,wxID_ANY,"Open",wxDefaultPosition,wxSize(50,20));
    wxCheckBox* loop = new wxCheckBox(programmerpanel,wxID_ANY,"Loop",wxDefaultPosition,wxSize(20,20));
    wxCheckBox* sequence = new wxCheckBox(programmerpanel,wxID_ANY,"Sequence",wxDefaultPosition,wxSize(20,20));
    wxButton* Add = new wxButton(programmerpanel ,wxID_ANY,"Add",wxDefaultPosition,wxSize(50,20));
    wxButton* Clear = new wxButton(programmerpanel ,wxID_ANY,"Clear",wxDefaultPosition,wxSize(50,20));
    wxButton* start = new wxButton(programmerpanel ,wxID_ANY,"Start",wxDefaultPosition,wxSize(100,100));
    wxButton* stop = new wxButton(programmerpanel ,wxID_ANY,"Stop",wxDefaultPosition,wxSize(100,100));
    
    topcontrols->Add(open,wxALIGN_LEFT);
    topcontrols->AddSpacer(20);
    topcontrols->Add(save,wxALIGN_LEFT);
    topcontrols->AddSpacer(20);
    topcontrols->Add(loop, wxALL,5);
    topcontrols->AddStretchSpacer(20);
    topcontrols->Add(sequence,wxALL,5);
    topcontrols->Add(Add,wxALIGN_RIGHT,5);
    topcontrols->AddSpacer(20);
    topcontrols->Add(Clear,wxALIGN_RIGHT);

    bottomcontrols->Add(start,wxALIGN_CENTER,5);
    bottomcontrols->AddSpacer(20); 
    bottomcontrols->Add(stop,wxALIGN_CENTER,5);

    programctrlsizer->Add(topcontrols,0,wxEXPAND|wxTOP|wxLEFT|wxRIGHT,20);
    programctrlsizer->Add(program,1, wxGROW|wxALL,20);
    programctrlsizer->Add(bottomcontrols,0,wxEXPAND|wxBOTTOM|wxLEFT|wxRIGHT,20);
    programmerpanel->SetSizerAndFit(programctrlsizer);


    Layout();
}

void ControlPanel::CreateManualControls(int relaynumber){ 
    switch(relaynumber){
        case 1:
        {
            wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
            this->k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            this->statek1 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
            controlssizer->AddStretchSpacer(1);
            controlssizer->Add(this->statek1,0,wxALL|wxALIGN_CENTER,0);
            controlssizer->AddSpacer(40);
            controlssizer->Add(this->k1,0,wxALL|wxALIGN_CENTER,0);
            controlssizer->AddStretchSpacer(1);
            Layout();
        }
        break;
        case 2:
        {
            wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
            this->k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            this->k2 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            this->statek1 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
            this->statek2 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
           
            wxGridBagSizer *frame = new wxGridBagSizer();
            
            std::vector<std::pair<wxGBPosition,wxGBSpan>> items =  {
                {{0,0},{1,1}},
                {{1,0},{1,1}},
                {{0,1},{1,1}},
                {{1,1},{1,1}}
                 };
    
            frame->Add(this->statek1,items[0].first,items[0].second,wxBOTTOM|wxRIGHT||wxALIGN_BOTTOM|wxALIGN_RIGHT,20);
            frame->Add(this->statek2,items[1].first,items[1].second,wxTOP|wxRIGHT|wxALIGN_TOP|wxALIGN_RIGHT,20);
            frame->Add(this->k1,items[2].first,items[2].second,wxBOTTOM|wxLEFT|wxALIGN_BOTTOM|wxALIGN_LEFT,15);
            frame->Add(this->k2,items[3].first,items[3].second,wxTOP|wxLEFT|wxALIGN_TOP|wxALIGN_LEFT,15);
           
            controlssizer->AddStretchSpacer(1);
            controlssizer->Add(frame,0,wxALIGN_CENTER);
            this->canva->SetSizerAndFit(controlssizer);
            controlssizer->AddStretchSpacer(1);
            Layout();
        }
        break;
        case 4:
        break;
        case 8:
        break;
        case 16:
        break;
    }
}