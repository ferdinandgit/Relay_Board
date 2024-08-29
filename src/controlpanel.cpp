#include<controlpanel.hpp>
#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/listctrl.h>
#include <drawingcanva.hpp>
#include <wx/grid.h>
#include <interpreter.hpp>


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
    // Create the text control and browse button
    m_filePathCtrl = new wxTextCtrl(programmerpanel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 25));
    browseButton = new wxButton(programmerpanel,browseButtonID, "Browse...", wxDefaultPosition, wxSize(80, 25));
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(m_filePathCtrl, 1, wxEXPAND | wxALL, 5);
    topSizer->Add(browseButton, 0, wxALL, 5);

    // Create the Load button, and Add/Clear buttons
    loadButton = new wxButton(programmerpanel, loadButtonID, "Load", wxDefaultPosition, wxSize(80, 25));
    addButton = new wxButton(programmerpanel, addButtonID, "Add", wxDefaultPosition, wxSize(80, 25));
    clearButton = new wxButton(programmerpanel, clearButtonID, "Clear", wxDefaultPosition, wxSize(80, 25));

    // Create titles for the lists
    wxStaticText* title1 = new wxStaticText(programmerpanel, wxID_ANY, "Program Board ID", wxDefaultPosition, wxDefaultSize);
    wxStaticText* title2 = new wxStaticText(programmerpanel, wxID_ANY, "Hardware Board", wxDefaultPosition, wxDefaultSize);

    // Create sizers to align the Load, Add, and Clear buttons above the lists
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(loadButton, 0, wxALL, 5);    // Align with left list
    buttonSizer->AddStretchSpacer(1);             // Add a spacer between the buttons
    buttonSizer->Add(addButton, 0, wxALL, 5);     // Align with right list
    buttonSizer->Add(clearButton, 0, wxALL, 5);   // Align with right list

    // Create the list controls
    m_listCtrl1 = new wxListCtrl(programmerpanel, wxID_ANY, wxDefaultPosition, wxSize(200, 150), wxLC_REPORT);
    m_listCtrl2 = new wxListCtrl(programmerpanel, wxID_ANY, wxDefaultPosition, wxSize(200, 150), wxLC_REPORT);

    // Add columns to the list controls
    m_listCtrl1->InsertColumn(0, "Name");
    m_listCtrl1->InsertColumn(1, "Relaynumber");
    m_listCtrl1->InsertColumn(2, "Timebase");
    m_listCtrl1->InsertColumn(3, "loop");
    m_listCtrl2->InsertColumn(0, "Id");
    m_listCtrl2->InsertColumn(0, "Type");

    // Create a sizer to hold the titles and lists
    wxBoxSizer* listSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* leftListSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* rightListSizer = new wxBoxSizer(wxVERTICAL);

    leftListSizer->Add(title1, 0, wxALIGN_LEFT | wxALL, 5);     // Add title above the left list
    leftListSizer->Add(m_listCtrl1, 1, wxEXPAND | wxALL, 5);    // Add the left list

    rightListSizer->Add(title2, 0, wxALIGN_LEFT | wxALL, 5);    // Add title above the right list
    rightListSizer->Add(m_listCtrl2, 1, wxEXPAND | wxALL, 5);   // Add the right list

    listSizer->Add(leftListSizer, 1, wxEXPAND | wxALL, 5);      // Add left list and title
    listSizer->Add(rightListSizer, 1, wxEXPAND | wxALL, 5);     // Add right list and title

    // Create the Start and Stop buttons
    startButton = new wxButton(programmerpanel, wxID_ANY, "Start", wxDefaultPosition, wxSize(80, 25));
    stopButton = new wxButton(programmerpanel, wxID_ANY, "Stop", wxDefaultPosition, wxSize(80, 25));

    // Create a sizer for the Start and Stop buttons
    wxBoxSizer* buttonSizer2 = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer2->Add(startButton, startButtonID, wxEXPAND | wxALL, 5);  // Expand to fill half the space
    buttonSizer2->Add(stopButton, stopButtonID, wxEXPAND | wxALL, 5);   // Expand to fill half the space

    // Create the main sizer and add all components to it
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(topSizer, 0, wxEXPAND);
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);        // Add the Load, Add, and Clear buttons
    mainSizer->Add(listSizer, 4, wxEXPAND);                     // Add the lists
    mainSizer->Add(buttonSizer2, 1, wxEXPAND | wxALL, 5);       // Add the Start and Stop buttons

    programmerpanel->SetSizer(mainSizer);
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

void ControlPanel::CreateTestlayout(){
    this->DestroyChildren();
    testpanel = new wxPanel(this,wxID_ANY,wxDefaultPosition,this->GetSize());
    testpanel->SetBackgroundColour(wxColour(100,100,100));
    this->GetSizer()->Add(testpanel,1,wxEXPAND,0);
}

void ControlPanel::CreateTestControls(int id, relayboard board) {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* textLabelSizer = new wxBoxSizer(wxHORIZONTAL);
    std::string idstring;
    std::string typestring;
    switch(board){
        case USBRELAY:
            typestring = "BOARD : USBRELAY";
            idstring = "ID : "+std::to_string(id);
        break;
        case USBMRELAY:
            typestring = "BOARD : USBMRELAY";
            idstring = "ID : "+std::to_string(id);
        break;
        case USBBRELAY:
            typestring = "BOARD : USBBRELAY";
            idstring = "ID : "+std::to_string(id);
        break;
        case USBCRELAY:
            typestring = "BOARD : USBCRELAY";
            idstring = "ID : "+std::to_string(id);
        break;
        default:
            typestring = "BOARD : NONE";
            idstring = "ID : NONE";
    }
    wxStaticText* label1 = new wxStaticText(testpanel, wxID_ANY, idstring);
    wxStaticText* label2 = new wxStaticText(testpanel, wxID_ANY, typestring);
    textLabelSizer->Add(label1, 1, wxALIGN_CENTER | wxALL, 10);
    textLabelSizer->Add(label2, 1, wxALIGN_CENTER | wxALL, 10);
    mainSizer->Add(textLabelSizer, 0, wxEXPAND | wxALL, 10);
    displayPanel = new wxPanel(testpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    displayPanel->SetBackgroundColour(*wxRED);
    testrelaytext = new wxStaticText(displayPanel, wxID_ANY, "K1", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL);
    wxFont font(100, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    testrelaytext->SetFont(font);
    wxBoxSizer* displaySizer = new wxBoxSizer(wxHORIZONTAL);
    displaySizer->Add(testrelaytext, 1, wxALIGN_CENTER | wxALL, 10);
    displayPanel->SetSizer(displaySizer);
    mainSizer->Add(displayPanel, 3, wxEXPAND | wxALL, 10);
    testButton = new wxButton(testpanel, testButtonId, "Test");
    mainSizer->Add(testButton, 1, wxEXPAND | wxALL, 10);

    testpanel->SetSizer(mainSizer);
    Layout();
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
    std::vector<int> usbbrelayorderframe1 = {0,1,2,3};
    std::vector<int> usbrelayorderframe2 = {3,2,1,0};
    std::vector<int> usbmrelayorderframe2 = {4,5,6,7};
    std::vector<int> usbbrelayorderframe2 = {7,6,5,4};
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
        /*add other 16 relays board layout*/
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
    int panelwidth = canva->getrelaywidth()+canva->getstep()/2;
    int panellenght = canva->getrelaylenght()/2+canva->getstep()/2;
    wxBoxSizer* controlssizer = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer *frame1 = new wxGridSizer(8,2,30,10);
    wxGridSizer *frame2 = new wxGridSizer(8,2,30,10);
    k1 = new wxButton(this->canva ,k1Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k2 = new wxButton(this->canva ,k2Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k3 = new wxButton(this->canva ,k3Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k4 = new wxButton(this->canva ,k4Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k5 = new wxButton(this->canva ,k5Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k6 = new wxButton(this->canva ,k6Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k7 = new wxButton(this->canva ,k7Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k8 = new wxButton(this->canva ,k8Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k9 = new wxButton(this->canva ,k9Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k10 = new wxButton(this->canva ,k10Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k11 = new wxButton(this->canva ,k11Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k12 = new wxButton(this->canva ,k12Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k13 = new wxButton(this->canva ,k13Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k14 = new wxButton(this->canva ,k14Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k15 = new wxButton(this->canva ,k15Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    k16 = new wxButton(this->canva ,k16Id,"Toggle",wxPoint(100,100),wxSize(50,50));
    statek1 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek2 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek3 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek4 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek5 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek6 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek7 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek8 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek9 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek10 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek11 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek12 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek13 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek14 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek15 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    statek16 = new wxStaticText(this->canva ,-1,"OFF",wxPoint(100,100),wxSize(20,20));
    UpdateState();
    std::vector<wxStaticText*> relaystatus = {statek1,statek2,statek3,statek4,statek5,statek6,statek7,statek8,statek9,statek10,statek11,statek12,statek13,statek14,statek15,statek16};
    std::vector<wxButton*> relaybutton = {k1,k2,k3,k4,k5,k6,k7,k8,k9,k10,k11,k12,k13,k14,k15,k16};
    std::vector<int> usbcrelayorderframe1 = {0,1,2,3,4,5,6,7};
    std::vector<int> usbcrelayorderframe2 = {15,14,13,12,11,10,9,8};
    switch(board){
        case USBCRELAY:
            for(int pos : usbcrelayorderframe1){
                frame1->Add(relaystatus[pos],wxALIGN_CENTER);
                frame1->Add(relaybutton[pos],wxALIGN_CENTER);
            }
            for(int pos : usbcrelayorderframe2){
                    frame2->Add(relaystatus[pos],wxALIGN_CENTER);
                    frame2->Add(relaybutton[pos],wxALIGN_CENTER);
            }
        break;
        /*add other 16 relays board layout*/
    }
    controlssizer->AddStretchSpacer(1);
    controlssizer->Add(frame1,0,wxALIGN_CENTER);
    controlssizer->AddSpacer(20);
    controlssizer->Add(frame2,0,wxALIGN_CENTER);
    controlssizer->AddStretchSpacer(1);
    this->canva->SetSizerAndFit(controlssizer);
    Layout();
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

void ControlPanel::AssignToNull(){
    activeboard=NULL;
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

void ControlPanel::OntestButton(wxCommandEvent &event){
    std::vector<int> command(16,0);
    if(activeboard == NULL){
        wxMessageBox("No board to test", "Error", wxOK | wxICON_ERROR);
    }
    else{
        for(int k=0;k<activeboard->getRelayNumber();k++){
            std::string relay = "K"+std::to_string(k+1);
            testrelaytext->SetLabel(relay);
            Layout();
            command[k]=1;
            activeboard->setState(command);
            displayPanel->SetBackgroundColour(*wxGREEN);
            displayPanel->Refresh();
            displayPanel->Update();
            my_sleep(500);
            command[k]=0;
            activeboard->setState(command);
            displayPanel->SetBackgroundColour(*wxRED);
            displayPanel->Refresh();
            displayPanel->Update();
            my_sleep(500);
        }
    }
        
}

void ControlPanel::OnbrowseButton(wxCommandEvent &event){
    wxFileDialog openFileDialog(this, _("Open file"), "", "",
                                "All files (*.*)|*.*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wxString filePath = openFileDialog.GetPath();
    m_filePathCtrl->SetValue(filePath);
}

void ControlPanel::OnloadButton(wxCommandEvent &event){
    std::string filePath = m_filePathCtrl->GetValue().ToStdString();
    interpreter = new Interpreter(filePath);
    if(interpreter->get_errorString() != ""){
        wxMessageBox(interpreter->get_errorString(), "Error", wxOK | wxICON_ERROR);
    }
    if(interpreter->match_conf_prog() != 1){
        wxMessageBox(interpreter->get_errorString(), "Error", wxOK | wxICON_ERROR);
    }
    
    for(boardprogram prog : interpreter->get_boardprogram()){
        long itemIndex = m_listCtrl1->InsertItem(m_listCtrl1->GetItemCount(), wxString(prog.configuration.id));
        m_listCtrl1->SetItem(itemIndex, 1, wxString(std::to_string(prog.configuration.relaynumber)));
        m_listCtrl1->SetItem(itemIndex, 2, wxString(prog.configuration.timebase));
        if(prog.configuration.loop){
            m_listCtrl1->SetItem(itemIndex, 3, wxString("True"));
        }
        else{
            m_listCtrl1->SetItem(itemIndex, 3, wxString("False"));
        }
    }
}

void ControlPanel::OnaddButton(wxCommandEvent &event){
    Serialrelay* selectedboard = openboards[selecteditem];
    long itemIndex = m_listCtrl2->InsertItem(m_listCtrl2->GetItemCount(), wxString(std::to_string(selectedboard->getId())));
    

}

void ControlPanel::OnclearButton(wxCommandEvent &event){

}

void ControlPanel::OnstartButton(wxCommandEvent &event){

}

void ControlPanel::OnstopButton(wxCommandEvent &event){

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

void ControlPanel::SelectedItem(int id){
    selecteditem=id;
}

wxBEGIN_EVENT_TABLE(ControlPanel, wxPanel)
    EVT_BUTTON(testButtonId, ControlPanel::OntestButton)
    EVT_BUTTON(browseButtonID, ControlPanel::OnbrowseButton)
    EVT_BUTTON(loadButtonID, ControlPanel::OnloadButton)
    EVT_BUTTON(addButtonID, ControlPanel::OnaddButton)
    EVT_BUTTON(clearButtonID, ControlPanel::OnclearButton)
    EVT_BUTTON(startButtonID, ControlPanel::OnstartButton)
    EVT_BUTTON(stopButtonID, ControlPanel::OnstopButton)
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

