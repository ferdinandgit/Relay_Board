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
    this->canva = new DrawingCanva(relaynumber,this, wxID_ANY, wxDefaultPosition,this->GetSize());
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
            int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
            int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
            wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
            k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k2 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            statek1 = new wxStaticText(this->canva ,k2Id,"OFF",wxPoint(100,100),wxSize(20,20));
            statek2 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
            wxGridSizer *frame = new wxGridSizer(2,2,30,10);
            frame->Add(statek1,wxALIGN_CENTRE);
            frame->Add(k1,wxALIGN_CENTRE);
            frame->Add(statek2,wxALIGN_CENTRE);
            frame->Add(k2,wxALIGN_CENTRE);
            controlssizer->AddStretchSpacer(1);
            controlssizer->Add(frame,0,wxALIGN_CENTER);
            this->canva->SetSizerAndFit(controlssizer);
            controlssizer->AddStretchSpacer(1);
            Layout();
        }
        break;
        case 4:
        {
            int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
            int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
            wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
            k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k2 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k3 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k4 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            statek1 = new wxStaticText(this->canva ,k2Id,"OFF",wxPoint(100,100),wxSize(20,20));
            statek2 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
            statek3 = new wxStaticText(this->canva ,k2Id,"OFF",wxPoint(100,100),wxSize(20,20));
            statek4 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
            wxGridSizer *frame = new wxGridSizer(4,2,30,10);
            frame->Add(statek1,wxALIGN_CENTRE);
            frame->Add(k1,wxALIGN_CENTRE);
            frame->Add(statek2,wxALIGN_CENTRE);
            frame->Add(k2,wxALIGN_CENTRE);
            frame->Add(statek3,wxALIGN_CENTRE);
            frame->Add(k3,wxALIGN_CENTRE);
            frame->Add(statek4,wxALIGN_CENTRE);
            frame->Add(k4,wxALIGN_CENTRE);
            controlssizer->AddStretchSpacer(1);
            controlssizer->Add(frame,0,wxALIGN_CENTER);
            this->canva->SetSizerAndFit(controlssizer);
            controlssizer->AddStretchSpacer(1);
            Layout();
        }
        break;
        case 8:
        {
            int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
            int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
            wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
            wxGridSizer *frame1 = new wxGridSizer(4,2,30,10);
            wxGridSizer *frame2 = new wxGridSizer(4,2,30,10);
            k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k2 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k3 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k4 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k5 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k6 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k7 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            k8 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
            statek1 = new wxStaticText(this->canva ,k2Id,"OFF",wxPoint(100,100),wxSize(20,20));
            statek2 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
            statek3 = new wxStaticText(this->canva ,k2Id,"OFF",wxPoint(100,100),wxSize(20,20));
            statek4 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
            statek5 = new wxStaticText(this->canva ,k2Id,"OFF",wxPoint(100,100),wxSize(20,20));
            statek6 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20));
            statek7 = new wxStaticText(this->canva ,k2Id,"OFF",wxPoint(100,100),wxSize(20,20));
            statek8 = new wxStaticText(this->canva ,k2Id,"ON",wxPoint(100,100),wxSize(20,20)); 
            frame1->Add(statek1,wxALIGN_CENTRE);
            frame1->Add(k1,wxALIGN_CENTRE);
            frame1->Add(statek2,wxALIGN_CENTRE);
            frame1->Add(k2,wxALIGN_CENTRE);
            frame1->Add(statek3,wxALIGN_CENTRE);
            frame1->Add(k3,wxALIGN_CENTRE);
            frame1->Add(statek4,wxALIGN_CENTRE);
            frame1->Add(k4,wxALIGN_CENTRE);
            frame2->Add(statek5,wxALIGN_CENTRE);
            frame2->Add(k5,wxALIGN_CENTRE);
            frame2->Add(statek6,wxALIGN_CENTRE);
            frame2->Add(k6,wxALIGN_CENTRE);
            frame2->Add(statek7,wxALIGN_CENTRE);
            frame2->Add(k7,wxALIGN_CENTRE);
            frame2->Add(statek8,wxALIGN_CENTRE);
            frame2->Add(k8,wxALIGN_CENTRE);
            controlssizer->AddStretchSpacer(1);
            controlssizer->Add(frame1,0,wxALIGN_CENTER);
            controlssizer->AddSpacer(20);
            controlssizer->Add(frame2,0,wxALIGN_CENTER);
            controlssizer->AddStretchSpacer(1);
            this->canva->SetSizerAndFit(controlssizer);
            Layout();
        }
        break;
        case 16:
        int columns = 2;
        int rows = 8;
         
        std::vector<wxButton*> relaytoggle = {k1,k2,k3,k4,k5,k6,k7,k8,k9,k10,k11,k12,k13,k14,k15,k16};
        std::vector<wxStaticText*> relaystatus = {statek1,statek2,statek3,statek4,statek5,statek6,statek7,statek8,statek9,statek10,statek11,statek12,statek13,statek14,statek15,statek16};
        
        int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
        int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
        wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);


        
        
        
        
        
        
        break;
    }
}

void ControlPanel::AddBoard(Serialrelay* board){
    openboards.push_back(board);
}

std::vector<Serialrelay*> ControlPanel::GetOpenBoards(){
    return openboards;
}

void ControlPanel::RmBoard(int id){
    openboards[id] = NULL;
}