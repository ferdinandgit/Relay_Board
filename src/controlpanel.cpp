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
    auto init = new DrawingCanva(0,NOBOARD,this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    controlpanelsizer->Add(init,1,wxEXPAND | wxALL,0);
    this->SetSizerAndFit(controlpanelsizer);
}

void ControlPanel::CreateManuallayout(int relaynumber,relayboard board){
    this->DestroyChildren();
    this->canva = new DrawingCanva(relaynumber,board,this, wxID_ANY, wxDefaultPosition,this->GetSize());
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

void ControlPanel::CreateManualControls(int relaynumber,relayboard board){ 
    switch(relaynumber){
        case 1:
            Relay1Controls(board);
        break;
        case 2:
            Relay2Controls(board);
        break;
        case 4:
            Relay4Controls(board);
        break;
        case 8:
            Relay8Controls(board);
        break;
        case 16:
            Relay16Contols(board);
        break;
    }
}


void ControlPanel::Relay1Controls(relayboard board){
    wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
    this->k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    this->statek1 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    UpdateState();
    controlssizer->AddStretchSpacer(1);
    controlssizer->Add(this->statek1,0,wxALL|wxALIGN_CENTER,0);
    controlssizer->AddSpacer(40);
    controlssizer->Add(this->k1,0,wxALL|wxALIGN_CENTER,0);
    controlssizer->AddStretchSpacer(1);
    this->canva->SetSizerAndFit(controlssizer);
    Layout();
}

void ControlPanel::Relay2Controls(relayboard board){
    int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
    int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
    wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
    k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k2 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    statek1 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek2 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    UpdateState();
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

void ControlPanel::Relay4Controls(relayboard board){
    int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
    int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
    wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
    k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k2 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k3 = new wxButton(this->canva ,k3Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k4 = new wxButton(this->canva ,k4Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    statek1 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek2 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek3 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek4 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    UpdateState();
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

void ControlPanel::Relay8Controls(relayboard board){
    int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
    int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
    wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer *frame1 = new wxGridSizer(4,2,30,10);
    wxGridSizer *frame2 = new wxGridSizer(4,2,30,10);
    k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k2 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k3 = new wxButton(this->canva ,k3Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k4 = new wxButton(this->canva ,k4Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k5 = new wxButton(this->canva ,k5Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k6 = new wxButton(this->canva ,k6Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k7 = new wxButton(this->canva ,k7Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k8 = new wxButton(this->canva ,k8Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    statek1 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek2 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek3 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek4 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek5 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek6 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek7 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek8 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    UpdateState();
    std::vector<wxStaticText*> relaystatus = {statek1,statek2,statek3,statek4,statek5,statek6,statek7,statek8};
    std::vector<wxButton*> relaybutton = {k1,k2,k3,k4,k5,k6,k7,k8};
    std::vector<int> usbrelayorderframe1 = {7,6,5,4};
    std::vector<int> usbmrelayorderframe1 = {3,2,1,0};
    std::vector<int> usbbrelayorderframe1 = {3,2,1,0};
    std::vector<int> usbrelayorderframe2 = {3,2,1,0};
    std::vector<int> usbmrelayorderframe2 = {4,5,6,7};
    std::vector<int> usbbrelayorderframe2 = {3,2,1,0};
    switch(board){
        case USBRELAY:
            for(int pos : usbrelayorderframe1){
                frame1->Add(relaystatus[pos],wxALIGN_CENTER);
                frame1->Add(relaybutton[pos],wxALIGN_CENTER);
            }
        for(int pos : usbrelayorderframe2){
                frame2->Add(relaystatus[pos],wxALIGN_CENTER);
                frame2->Add(relaybutton[pos],wxALIGN_CENTER);
            }
        break;
        case USBMRELAY:
             for(int pos : usbmrelayorderframe1){
                frame1->Add(relaystatus[pos],wxALIGN_CENTER);
                frame1->Add(relaybutton[pos],wxALIGN_CENTER);
            }
        for(int pos : usbmrelayorderframe2){
                frame2->Add(relaystatus[pos],wxALIGN_CENTER);
                frame2->Add(relaybutton[pos],wxALIGN_CENTER);
            }
        break;
        case USBBRELAY:
        for(int pos : usbbrelayorderframe1){
                frame1->Add(relaystatus[pos],wxALIGN_CENTER);
                frame1->Add(relaybutton[pos],wxALIGN_CENTER);
            }
        for(int pos : usbbrelayorderframe2){
                frame2->Add(relaystatus[pos],wxALIGN_CENTER);
                frame2->Add(relaybutton[pos],wxALIGN_CENTER);
            }
        break;

    }
    controlssizer->AddStretchSpacer(1);
    controlssizer->Add(frame1,0,wxALIGN_CENTER);
    controlssizer->AddSpacer(20);
    controlssizer->Add(frame2,0,wxALIGN_CENTER);
    controlssizer->AddStretchSpacer(1);
    this->canva->SetSizerAndFit(controlssizer);
    Layout();
}

void ControlPanel::Relay16Contols(relayboard board){

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

void ControlPanel::AssignToBoard(int id){
    activeboard=openboards[id];
}


void ControlPanel::UpdateState(){
    std::vector<int> status = activeboard->getState();
    std::vector<wxStaticText*> relaystatus = {statek1,statek2,statek3,statek4,statek5,statek6,statek7,statek8,statek9,statek10,statek11,statek12,statek13,statek14,statek15,statek16};
    for(int k=0;k<activeboard->getRelayNumber();k++){
        wxStaticText* kstate = relaystatus[k];
        if(status[k]){
            kstate->SetLabel("ON");
        }
        else{
            kstate->SetLabel("OFF");
        }
    }
}

void ControlPanel::OnK1(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[0]=!command[0];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK2(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[1]=!command[1];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK3(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[2]=!command[2];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK4(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[3]=!command[3];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK5(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[4]=!command[4];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK6(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[5]=!command[5];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK7(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[6]=!command[6];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK8(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[7]=!command[7];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK9(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[8]=!command[8];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK10(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[9]=!command[9];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK11(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[10]=!command[10];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK12(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[11]=!command[11];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK13(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[12]=!command[12];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK14(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[13]=!command[13];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK15(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[14]=!command[14];
    activeboard->setState(command);
    UpdateState();
}

void ControlPanel::OnK16(wxCommandEvent &event){
    std::vector<int> command = activeboard->getState();
    command[15]=!command[15];
    activeboard->setState(command);
    UpdateState();
}

wxBEGIN_EVENT_TABLE(ControlPanel, wxPanel)
    EVT_BUTTON(k1Id, ControlPanel::OnK1)
    EVT_BUTTON(k2Id, ControlPanel::OnK2)
    EVT_BUTTON(k3Id, ControlPanel::OnK3)
    EVT_BUTTON(k4Id, ControlPanel::OnK4)
    EVT_BUTTON(k5Id, ControlPanel::OnK5)
    EVT_BUTTON(k6Id, ControlPanel::OnK6)
    EVT_BUTTON(k7Id, ControlPanel::OnK7)
    EVT_BUTTON(k8Id, ControlPanel::OnK8)
    EVT_BUTTON(k9Id, ControlPanel::OnK9)
    EVT_BUTTON(k10Id, ControlPanel::OnK10)
    EVT_BUTTON(k11Id, ControlPanel::OnK11)
    EVT_BUTTON(k12Id, ControlPanel::OnK12)
    EVT_BUTTON(k13Id, ControlPanel::OnK13)
    EVT_BUTTON(k14Id, ControlPanel::OnK14)
    EVT_BUTTON(k15Id, ControlPanel::OnK15)
    EVT_BUTTON(k16Id, ControlPanel::OnK16)
wxEND_EVENT_TABLE()

/*int columns = 2;
        int rows = 8;
        std::vector<wxButton*> relaytoggle = {k1,k2,k3,k4,k5,k6,k7,k8,k9,k10,k11,k12,k13,k14,k15,k16};
        std::vector<wxStaticText*> relaystatus = {statek1,statek2,statek3,statek4,statek5,statek6,statek7,statek8,statek9,statek10,statek11,statek12,statek13,statek14,statek15,statek16};
        int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
        int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
        wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);*/