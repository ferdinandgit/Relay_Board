#include<controlpanel.hpp>
#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/listctrl.h>
#include <drawingcanva.hpp>
#include <wx/grid.h>
#include <wx/event.h>
#include <interpreter.hpp>
#include <thread>
#include <chrono>

ControlPanel::ControlPanel (wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size): wxPanel(parent, id, pos, size){
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
}

void ControlPanel::SetRelayList(wxListCtrl* relaylist){
    this->relaylist = relaylist;
}
void ControlPanel::InitControlPanel(){
    this->SetBackgroundColour(wxColour(100,100,100));
    wxSizer *controlpanelsizer = new wxBoxSizer(wxVERTICAL);
    auto init = new DrawingCanva(0,NOBOARD,this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    controlpanelsizer->Add(init,1,wxEXPAND | wxALL,0);
    this->SetSizerAndFit(controlpanelsizer);
}

void ControlPanel::CreateManuallayout(int relaynumber,relayboard board){
    CleanAfterProgrammer();
    this->DestroyChildren();
    this->canva = new DrawingCanva(relaynumber,board,this, wxID_ANY, wxDefaultPosition,this->GetSize());
    this->GetSizer()->Add(canva,1,wxEXPAND);
}

void ControlPanel::CreateProgrammerlayout(){
    CleanAfterProgrammer();
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
    blankButton = new wxButton(programmerpanel, blankButtonID, "Free", wxDefaultPosition, wxSize(80, 25));
    // Create titles for the lists
    wxStaticText* title1 = new wxStaticText(programmerpanel, wxID_ANY, "Program Board ID", wxDefaultPosition, wxDefaultSize);
    wxStaticText* title2 = new wxStaticText(programmerpanel, wxID_ANY, "Hardware Board", wxDefaultPosition, wxDefaultSize);
    wxStaticText* title3 = new wxStaticText(programmerpanel, wxID_ANY, "  ", wxDefaultPosition, wxDefaultSize);
    
    // Create sizers to align the Load, Add, and Clear buttons above the lists
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(loadButton, 0, wxALL, 5);    // Align with left list
    buttonSizer->AddStretchSpacer(1); 
    buttonSizer->Add(blankButton, 0, wxALL, 5);            // Add a spacer between the buttons
    buttonSizer->Add(addButton, 0, wxALL, 5);     // Align with right list
    buttonSizer->Add(clearButton, 0, wxALL, 5);   // Align with right list

    // Create the list controls
    m_listCtrl1 = new wxListCtrl(programmerpanel, wxID_ANY, wxDefaultPosition, wxSize(200, 150), wxLC_REPORT);
    m_listCtrl2 = new wxListCtrl(programmerpanel, wxID_ANY, wxDefaultPosition, wxSize(200, 150), wxLC_REPORT);
    
    m_listCtrl3 = new wxListCtrl(programmerpanel, wxID_ANY, wxDefaultPosition, wxSize(30, 150), wxLC_REPORT | wxLC_SINGLE_SEL);
    m_listCtrl3->InsertColumn(0, "->", wxLIST_FORMAT_CENTRE,25);


    // Add columns to the list controls
    m_listCtrl1->InsertColumn(0, "Name");
    m_listCtrl1->InsertColumn(1, "Relaynumber");
    m_listCtrl1->InsertColumn(2, "Timebase");
    m_listCtrl1->InsertColumn(3, "loop"); 

    m_listCtrl2->InsertColumn(0, _("id"), wxLIST_FORMAT_LEFT, 30);
    m_listCtrl2->InsertColumn(1, _("port"),wxLIST_FORMAT_LEFT,50);
    m_listCtrl2->InsertColumn(2, _("type"), wxLIST_FORMAT_LEFT, 100);
    m_listCtrl2->InsertColumn(3, _("relay number"), wxLIST_FORMAT_LEFT, 100);
    m_listCtrl2->InsertColumn(4, _("baudrate"), wxLIST_FORMAT_LEFT, 100);

    // Create a sizer to hold the titles and lists
    wxBoxSizer* listSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* leftListSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* rightListSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* midListSizer = new wxBoxSizer(wxVERTICAL);
    


    leftListSizer->Add(title1, 0, wxALIGN_LEFT | wxALL, 5);     // Add title above the left list
    leftListSizer->Add(m_listCtrl1, 1, wxEXPAND | wxALL, 5);    // Add the left list

    rightListSizer->Add(title2, 0, wxALIGN_LEFT | wxALL, 5);    // Add title above the right list
    rightListSizer->Add(m_listCtrl2, 1, wxEXPAND | wxALL, 5);   // Add the right list
   
    midListSizer->Add(title3, 0, wxALIGN_LEFT | wxALL, 5);
    midListSizer->Add(m_listCtrl3, 1, wxTOP | wxBOTTOM , 5);

    listSizer->Add(leftListSizer, 1, wxEXPAND | wxLEFT | wxTOP | wxBOTTOM, 5);    // Add left list and title
    listSizer->Add(midListSizer, 0, wxEXPAND | wxTOP | wxBOTTOM , 5);   
    listSizer->Add(rightListSizer, 1, wxEXPAND | wxRIGHT | wxTOP | wxBOTTOM, 5);     // Add right list and title

    // Create the Start and Stop buttons
    startButton = new wxButton(programmerpanel,startButtonID, "Start", wxDefaultPosition, wxSize(80, 25));
    stopButton = new wxButton(programmerpanel,stopButtonID, "Stop", wxDefaultPosition, wxSize(80, 25));

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
    CleanAfterProgrammer();
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
            kstate->SetForegroundColour(wxColour(0,255,0));
            kstate->SetLabel("ON");
        }
        else{
            kstate->SetForegroundColour(wxColour(255,0,0));
            kstate->SetLabel("OFF");     
        }
    }
}


void ControlPanel::OntestButton(wxCommandEvent& event) {
    if(*teststarted){
        return;
    }
    *teststarted = true;
    if (activeboard == nullptr) {
        wxMessageBox("No board to test", "Error", wxOK | wxICON_ERROR);
        return;
    }

    std::thread testThread([this]() {
        std::vector<int> commandtest(16, 0);
        for (int k = 0; k < activeboard->getRelayNumber(); ++k) {
            std::string relay = "K" + std::to_string(k + 1);

            // Update UI safely using wxCallAfter
            CallAfter([this, relay]() {
                testrelaytext->SetLabel(relay);
                Layout();
            });

            commandtest[k] = 1;
            if (activeboard->setState(commandtest) != 1) {
                CallAfter([this]() {
                    wxMessageBox("Check hardware board disconnected", "Error", wxOK | wxICON_ERROR);
                    // Perform cleanup or reset operations
                    this->CleanupBoard();
                });
                break;
            }

            CallAfter([this]() {
                displayPanel->SetBackgroundColour(*wxGREEN);
                displayPanel->Refresh();
                Layout();
            });

            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            commandtest[k] = 0;
            if (activeboard->setState(commandtest) != 1) {
                CallAfter([this]() {
                    wxMessageBox("Check hardware board disconnected", "Error", wxOK | wxICON_ERROR);
                    // Perform cleanup or reset operations
                    this->CleanupBoard();
                });
                break;
            }

            CallAfter([this]() {
                displayPanel->SetBackgroundColour(*wxRED);
                displayPanel->Refresh();
                Layout();
            });

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        *teststarted = false;
    });

    testThread.detach();
}

void ControlPanel::OnbrowseButton(wxCommandEvent &event){
    wxFileDialog openFileDialog(this, _("Open file"), "", "",
                                "YAML files (*.yaml;*.yml)|*.yaml;*.yml", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;
    wxString filePath = openFileDialog.GetPath();
    m_filePathCtrl->SetValue(filePath);
}

void ControlPanel::OnloadButton(wxCommandEvent &event){
    if(filePath==m_filePathCtrl->GetValue().ToStdString() && filePath != ""){
        wxMessageBox("File allready loaded", "Error", wxOK | wxICON_ERROR);
        return; 
    }
    else{
        filePath=m_filePathCtrl->GetValue().ToStdString();
        m_listCtrl2->DeleteAllItems();
        m_listCtrl1->DeleteAllItems();
        programmerboards.clear();
    }
    interpreter = new Interpreter(filePath,&threadstarted);
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

void ControlPanel::OnblankButton(wxCommandEvent &event){
     if(m_listCtrl1->IsEmpty()){
        wxMessageBox("Please load a program file before mapping", "Error", wxOK | wxICON_ERROR);
        return;
    }
    if(m_listCtrl1->GetItemCount()==m_listCtrl2->GetItemCount()){
        wxMessageBox("All boards mapped", "Error", wxOK | wxICON_ERROR);
        return;
    }
    Serialrelay* freeboard = NULL;
    m_listCtrl2->InsertItem(m_listCtrl2->GetItemCount(), wxString("Free"));
    m_listCtrl3->InsertItem(0,"->");
    programmerboards.push_back(freeboard);
}

void ControlPanel::OnaddButton(wxCommandEvent &event){
    if(m_listCtrl1->IsEmpty()){
        wxMessageBox("Please load a program file before mapping", "Error", wxOK | wxICON_ERROR);
        return;
    }
    Serialrelay* selectedboard;
    if(openboards.empty()){
        wxMessageBox("Please connect a board", "Error", wxOK | wxICON_ERROR);
        return;
    }
    selectedboard = openboards[selecteditem];
    if(m_listCtrl1->GetItemCount()==m_listCtrl2->GetItemCount()){
        wxMessageBox("All boards mapped", "Error", wxOK | wxICON_ERROR);
        return;
    }
        if(std::binary_search(programmerboards.begin(), programmerboards.end(), selectedboard)){
        wxMessageBox("Already mapped", "Error", wxOK | wxICON_ERROR);
        return; 
    }
    if(wxAtoi(m_listCtrl1->GetItemText(m_listCtrl2->GetItemCount(),1))!=selectedboard->getRelayNumber()){
        wxMessageBox("Hardware device not compatible", "Error", wxOK | wxICON_ERROR);
        return;
    }
    programmerboards.push_back(selectedboard);
    if(selectedboard!=NULL){
        long itemIndex = m_listCtrl2->InsertItem(m_listCtrl2->GetItemCount(), wxString(std::to_string(selectedboard->getId())));
        m_listCtrl2->SetItem(itemIndex, 1, wxString(selectedboard->getPort()));
        m_listCtrl2->SetItem(itemIndex, 3, wxString(std::to_string(selectedboard->getRelayNumber())));
        m_listCtrl2->SetItem(itemIndex, 4, wxString(std::to_string(selectedboard->getSpeed())));
        switch(selectedboard->getType()){
            case USBRELAY:
                m_listCtrl2->SetItem(itemIndex, 2, wxString("USB-RELAY"));
            break;
            case USBBRELAY:
                m_listCtrl2->SetItem(itemIndex, 2, wxString("USBB-RELAY"));
            break;
            case USBMRELAY: 
                m_listCtrl2->SetItem(itemIndex, 2, wxString("USBM-RELAY"));
            break;
            case USBCRELAY:
                m_listCtrl2->SetItem(itemIndex, 2, wxString("USBC-RELAY"));
            break;  
        }
        m_listCtrl3->InsertItem(0,"->");
    }
    else{
        wxMessageBox("Please select or connect a board", "Error", wxOK | wxICON_ERROR);
    }
}

void ControlPanel::OnclearButton(wxCommandEvent &event){
    m_listCtrl2->DeleteAllItems();
    m_listCtrl3->DeleteAllItems();
    programmerboards.clear();
    interpreter->unmap_all();
}

void ControlPanel::OnstartButton(wxCommandEvent &event){
    if(m_listCtrl2->IsEmpty()){
        wxMessageBox("No hardware match defined", "Error", wxOK | wxICON_ERROR);
        return;
    }
    std::vector<boardprogram> programs = interpreter->get_boardprogram();
    for(int k=0;k<programmerboards.size();k++){
        if(interpreter->match_hardware(programmerboards[k],programs[k].id)!=1){
            wxMessageBox("Failed to match program with hardware", "Error", wxOK | wxICON_ERROR);
            return;
        }
    }
    if(interpreter->create_thread()!=1){
        wxMessageBox("Failed to create threads", "Error", wxOK | wxICON_ERROR);
        return;
    }
    if(interpreter->start_thread()!=1){
        wxMessageBox("Failed to start threads", "Error", wxOK | wxICON_ERROR);
        return;
    }
}

void ControlPanel::CleanAfterProgrammer(){
    if(threadstarted){
        interpreter->stop_thread();
    }
    interpreter = NULL;
    filePath="";
}

void ControlPanel::OnstopButton(wxCommandEvent &event){
    if(threadstarted){  
        interpreter->stop_thread();
    }
    else{
        wxMessageBox("No program to stop", "Error", wxOK | wxICON_ERROR);
    }
}

void ControlPanel::ToggleSafe(int relay){
    std::vector<int> command = activeboard->getState();
    command[relay-1]=!command[relay-1];
    if(activeboard->setState(command)!=1){
        wxMessageBox("Check hardware board disconnected", "Error", wxOK | wxICON_ERROR);
        for(auto board : openboards){
            if(board==activeboard){
                int id = board->getId();
                long itemIndex = -1;
                while ((itemIndex = relaylist->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE)) != wxNOT_FOUND){
                    wxListItem item;
                    item.SetId(itemIndex);
                    item.SetMask(wxLIST_MASK_TEXT);
                    if(id==wxAtoi(relaylist->GetItemText(itemIndex,0))){
                        break;
                    }
                }
                relaylist->DeleteItem(itemIndex);   
                this->GetOpenBoards()[id]->closeCom();
                this->RmBoard(id);
                this->CreateManuallayout(0,NOBOARD);
                this->CreateManualControls(0,NOBOARD);
                break;
            }
        }
    }
    else{
        UpdateState();
    }
}

int ControlPanel::OnOffSafe(int relay,int state){
    std::vector<int> command(16,0);
    command[relay-1]=state;
    if(activeboard->setState(command)!=1){
        wxMessageBox("Check hardware board disconnected", "Error", wxOK | wxICON_ERROR);
        for(auto board : openboards){
            if(board==activeboard){
                int id = board->getId();
                long itemIndex = -1;
                while ((itemIndex = relaylist->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE)) != wxNOT_FOUND){
                    wxListItem item;
                    item.SetId(itemIndex);
                    item.SetMask(wxLIST_MASK_TEXT);
                    if(id==wxAtoi(relaylist->GetItemText(itemIndex,0))){
                        break;
                    }
                }
                relaylist->DeleteItem(itemIndex);   
                this->GetOpenBoards()[id]->closeCom();
                this->RmBoard(id);
                this->CreateTestlayout();
                this->CreateTestControls(0,NOBOARD);
                this->AssignToNull();
                break;
            }
        }
        return -1;
    }
    return 1;
}

void ControlPanel::OnK1(wxCommandEvent &event){
    ToggleSafe(1);
}

void ControlPanel::OnK2(wxCommandEvent &event){
    ToggleSafe(2);
}

void ControlPanel::OnK3(wxCommandEvent &event){
    ToggleSafe(3);
}

void ControlPanel::OnK4(wxCommandEvent &event){
    ToggleSafe(4);
}

void ControlPanel::OnK5(wxCommandEvent &event){
    ToggleSafe(5);
}

void ControlPanel::OnK6(wxCommandEvent &event){
    ToggleSafe(6);
}

void ControlPanel::OnK7(wxCommandEvent &event){
    ToggleSafe(7);
}

void ControlPanel::OnK8(wxCommandEvent &event){
    ToggleSafe(8);
}

void ControlPanel::OnK9(wxCommandEvent &event){
    ToggleSafe(9);
}

void ControlPanel::OnK10(wxCommandEvent &event){
    ToggleSafe(10);
}

void ControlPanel::OnK11(wxCommandEvent &event){
    ToggleSafe(11);
}

void ControlPanel::OnK12(wxCommandEvent &event){
    ToggleSafe(12);
}

void ControlPanel::OnK13(wxCommandEvent &event){
    ToggleSafe(13);
}

void ControlPanel::OnK14(wxCommandEvent &event){
    ToggleSafe(14);
}

void ControlPanel::OnK15(wxCommandEvent &event){
    ToggleSafe(15);
}

void ControlPanel::OnK16(wxCommandEvent &event){
    ToggleSafe(16);
}

void ControlPanel::SelectedItem(int id){
    selecteditem=id;
}

void ControlPanel::SetTestStarted(bool* test){
    teststarted=test;
}

void ControlPanel::CleanupBoard() {
    for (auto board : openboards) {
        if (board == activeboard) {
            int id = board->getId();
            long itemIndex = -1;
            while ((itemIndex = relaylist->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE)) != wxNOT_FOUND) {
                wxListItem item;
                item.SetId(itemIndex);
                item.SetMask(wxLIST_MASK_TEXT);
                if (id == wxAtoi(relaylist->GetItemText(itemIndex, 0))) {
                    relaylist->DeleteItem(itemIndex);
                    break;
                }
            }
            GetOpenBoards()[id]->closeCom();
            RmBoard(id);
            CreateTestlayout();
            CreateTestControls(0, NOBOARD);
            AssignToNull();
            break;
        }
    }
}

wxBEGIN_EVENT_TABLE(ControlPanel, wxPanel)
    EVT_BUTTON(testButtonId, ControlPanel::OntestButton)
    EVT_BUTTON(browseButtonID, ControlPanel::OnbrowseButton)
    EVT_BUTTON(loadButtonID, ControlPanel::OnloadButton)
    EVT_BUTTON(addButtonID, ControlPanel::OnaddButton)
    EVT_BUTTON(clearButtonID, ControlPanel::OnclearButton)
    EVT_BUTTON(startButtonID, ControlPanel::OnstartButton)
    EVT_BUTTON(stopButtonID, ControlPanel::OnstopButton)
    EVT_BUTTON(blankButtonID,ControlPanel::OnblankButton)
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

