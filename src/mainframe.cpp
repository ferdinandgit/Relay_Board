#include <mainframe.hpp>
#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/listctrl.h>
#include <drawingcanva.hpp>
#include <controlpanel.hpp>
#include <serialrelay.hpp>
#include <wx/timer.h>
#include <usb_relay_device.hpp>

static int id = 0; 

MainFrame::MainFrame(const wxString &title) : wxFrame(NULL, wxID_ANY, title){
    CreatePanels();
    CreateControls();

    refreshtimer = new wxTimer(managerpanel,timerid);
    refreshtimer->Start(1000);
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
    sizer->Add(this->relaypanel,items[2].first,items[2].second,wxEXPAND| wxALL,0);
    //define contorl panel
    this->controlpanel = new ControlPanel(this->mainpanel,wxID_ANY, wxDefaultPosition,initialsize);
    this->controlpanel->SetBackgroundColour(wxColour(100,100,100));
    sizer->Add(this->controlpanel,items[3].first,items[3].second,wxEXPAND);
    this->controlpanel->InitControlPanel();
    //Sizer parameter for panel sizing 
    sizer->AddGrowableCol(0);
    sizer->AddGrowableCol(1);
    sizer->AddGrowableRow(2);
    sizer->SetMinSize(FromDIP(wxSize(1000,800)));
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
    programmericon.LoadFile("prog.bmp", wxBITMAP_TYPE_BMP);
    this->progammermodebutton = new wxBitmapButton(this->menupanel,progammermodebuttonId,programmericon, wxPoint(buttonwidth,0),buttonsize, 0);
    wxBitmap testicon;
    testicon.LoadFile("test.bmp", wxBITMAP_TYPE_BMP);
    this->testmodebutton = new wxBitmapButton(this->menupanel,testmodebuttonId,testicon, wxPoint(buttonwidth*2,0),buttonsize, 0);
    //manager panel control 
    wxBitmap addicon;
    addicon.LoadFile("add.bmp", wxBITMAP_TYPE_BMP);
    this->addbutton = new wxBitmapButton(this->managerpanel,addbuttonId, addicon, wxPoint(0,0),buttonsize, 0);
    wxBitmap clearicon;
    clearicon.LoadFile("clear.bmp", wxBITMAP_TYPE_BMP);
    this->clearbutton = new wxBitmapButton(this->managerpanel,clearbuttonId, clearicon, wxPoint(buttonwidth,0),buttonsize, 0);
   
    wxArrayString boardtypelist;
    std::vector<std::string> boardref = {"USB-RELAY","USBB-RELAY","USBM-RELAY"};
    for(auto ref : boardref){
        boardtypelist.Add(wxString(ref));
    }
    boardtype = new wxComboBox(this->managerpanel, -1, wxT("type"), wxPoint(buttonwidth*2,0),wxSize(120,100),boardtypelist);

    wxArrayString relaynumberlist;
    relaynumberlist.Add(wxT("1"));
    relaynumberlist.Add(wxT("2"));
    relaynumberlist.Add(wxT("4"));
    relaynumberlist.Add(wxT("8"));
    relaynumberlist.Add(wxT("16"));
    boardrelaynumber = new wxComboBox(this->managerpanel, -1, wxT("relaynumber"), wxPoint(buttonwidth*2+120,0),wxSize(120,100),relaynumberlist);


    port = new wxComboBox(this->managerpanel, -1, wxT("usbdev"), wxPoint(buttonwidth*2,22),wxSize(120,100));
    GetDevices();



    //Relay panel ctrl 
    wxBoxSizer* sizerrelaypanel = new wxBoxSizer(wxHORIZONTAL);
    relaypanel->SetSizer(sizerrelaypanel);
    this->relaylist = new wxListCtrl(relaypanel,listctrlid, wxDefaultPosition,wxDefaultSize, wxLC_REPORT|wxRAISED_BORDER|wxLC_VRULES);
    sizerrelaypanel->Add(relaylist, 1, wxGROW, 0);
    this->relaylist->InsertColumn(0, _("id"), wxLIST_FORMAT_LEFT, 30);
    this->relaylist->InsertColumn(1, _("port"),wxLIST_FORMAT_LEFT,50);
    this->relaylist->InsertColumn(2, _("type"), wxLIST_FORMAT_LEFT, 100);
    this->relaylist->InsertColumn(3, _("relay number"), wxLIST_FORMAT_LEFT, 100);
    this->relaylist->InsertColumn(4, _("baudrate"), wxLIST_FORMAT_LEFT, 800);
}



//Event Command

void MainFrame::OnAdd(wxCommandEvent& event){
    relayboard boardtype; 
    wxString port = this->port->GetStringSelection();
    wxString type = this->boardtype->GetStringSelection();
    wxString relaynumber = this->boardrelaynumber->GetStringSelection();
    wxString boardrelaynumber = this->boardrelaynumber->GetStringSelection();
    
    for(auto openedboard : controlpanel->GetOpenBoards()){
        if(openedboard!=NULL){
            if( wxString(openedboard->getPort()) == port){
                wxMessageBox("Board already connected", "Error", wxOK | wxICON_ERROR);
                return;   
            }
        }
    }
    if(type ==  wxString("USB-RELAY")){
        boardtype = USBRELAY;
    }
    else if(type == wxString("USBM-RELAY")){
        boardtype = USBMRELAY;
    }
    else if(type == wxString("USBB-RELAY")){
        boardtype = USBBRELAY;
    }
    else{
        wxMessageBox("Please select a board", "Error", wxOK | wxICON_ERROR);
        return;
    }
    Serialrelay* board;
    pusb_relay_device_info_t usbbptr;
    for(int k = 0;k<dev.usbdevice.size();k++){
        if(dev.usbdevice[k] == port.ToStdString()){
            usbbptr = dev.usbbptr[k];
        }
    }
    if( boardtype == USBBRELAY){
        board = new Serialrelay(id,boardtype,usbbptr);
    }
    else{
        board = new Serialrelay(id,boardtype,port.ToStdString(),wxAtoi(relaynumber));
    }
    board->openCom();
    board->initBoard();
    controlpanel->AddBoard(board);
    long itemIndex = relaylist->InsertItem(relaylist->GetItemCount(), wxString::Format("%d", id));
    relaylist->SetItem(itemIndex, 1, port);
    relaylist->SetItem(itemIndex, 2, type);
    relaylist->SetItem(itemIndex, 3, wxString(std::to_string(board->getRelayNumber())));
    relaylist->SetItem(itemIndex, 4, wxString(std::to_string(board->getSpeed())));
    id++;
}

void MainFrame::OnClear(wxCommandEvent& event){
    long itemIndex = relaylist->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if(itemIndex != -1 & manual == 1){
        int deletedid = wxAtoi(relaylist->GetItemText(itemIndex));
        relaylist->DeleteItem(itemIndex);   
        controlpanel->GetOpenBoards()[deletedid]->closeCom();
        controlpanel->RmBoard(deletedid);
        long itemIndex = relaylist->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if(itemIndex != -1){
            int id = wxAtoi(relaylist->GetItemText(relaylist->GetTopItem(), 0));
            Serialrelay* activeboard = controlpanel->GetOpenBoards()[id];
            relayboard boardtype = activeboard->getType();
            int relaynumber = activeboard-> getRelayNumber();
            controlpanel->AssignToBoard(id);
            controlpanel->CreateManuallayout(relaynumber,boardtype);
            controlpanel->CreateManualControls(relaynumber,boardtype);
        }
        else{
            controlpanel->CreateManuallayout(0,NOBOARD);
            controlpanel->CreateManualControls(0,NOBOARD);
        }   
    }
    else if(itemIndex != -1){
        int deletedid = wxAtoi(relaylist->GetItemText(itemIndex));
        relaylist->DeleteItem(itemIndex);   
        controlpanel->GetOpenBoards()[deletedid]->closeCom();
        controlpanel->RmBoard(deletedid);
    }
    else
    {
        wxMessageBox("No board to remove", "Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnManualMode(wxCommandEvent& event){
    manual=1;
    programmer=0;
    test=0;
    if(relaylist->GetItemCount() > 0){
        int id = wxAtoi(relaylist->GetItemText(relaylist->GetTopItem(),0));
        Serialrelay* activeboard = controlpanel->GetOpenBoards()[id];
        relayboard boardtype = activeboard->getType();
        int relaynumber = activeboard-> getRelayNumber();
        controlpanel->AssignToBoard(id);
        controlpanel->CreateManuallayout(relaynumber,boardtype);
        controlpanel->CreateManualControls(relaynumber,boardtype);
    }
    else{
        controlpanel->CreateManuallayout(0,NOBOARD);
        controlpanel->CreateManualControls(0,NOBOARD);
    }
}

void MainFrame::OnTestMode(wxCommandEvent& event){
    manual=0;
    programmer=1;
    test=0;
    this->controlpanel->DestroyChildren();
    auto canva = new DrawingCanva(0,NOBOARD,controlpanel, wxID_ANY, wxDefaultPosition,controlpanel->GetSize());
    controlpanel->GetSizer()->Add(canva);
}

void MainFrame::OnProgrammerMode(wxCommandEvent& event){
    manual=0;
    programmer=0;
    test=1;
    controlpanel->CreateProgrammerlayout();
    controlpanel->CreateProgrammerControls();
}

void MainFrame::OnTimer(wxTimerEvent& event){
      
}

void MainFrame::DoubleClickItem(wxListEvent& event){
    int index = event.GetIndex();
    if(manual){
        int id = wxAtoi(relaylist->GetItemText(index, 0));
        Serialrelay* activeboard = controlpanel->GetOpenBoards()[id];
        relayboard boardtype = activeboard->getType();
        int relaynumber = activeboard-> getRelayNumber();
        controlpanel->AssignToBoard(id);
        controlpanel->CreateManuallayout(relaynumber,boardtype);
        controlpanel->CreateManualControls(relaynumber,boardtype);
    }
    
}

void MainFrame::GetDevices(){
    dev = scanBoard();
    std::vector<wxString> usblist;
    for(auto device : dev.usbdevice){
        #if defined (_WIN32) || defined( _WIN64)
        std::string path = "\\\\.\\";
        if(device.substr(0,4)==path){
            device=device.substr(4,device.length());
        }
        #endif
        usblist.push_back(wxString(device));
    }
    port->Clear();
    port->SetValue("usbdev");
    for(auto usbdev : usblist){
        port->Append(usbdev);
    }
}

// Event Table
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(addbuttonId, MainFrame::OnAdd)
    EVT_BUTTON(clearbuttonId, MainFrame::OnClear)
    EVT_BUTTON(manualmodebuttonId, MainFrame::OnManualMode)
    EVT_BUTTON(testmodebuttonId, MainFrame::OnTestMode)
    EVT_BUTTON(progammermodebuttonId, MainFrame::OnProgrammerMode)
    EVT_TIMER(timerid, MainFrame::OnTimer)
    EVT_LIST_ITEM_ACTIVATED(listctrlid,MainFrame::DoubleClickItem)
wxEND_EVENT_TABLE()