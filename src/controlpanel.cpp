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
            this->canva->SetSizerAndFit(controlssizer);
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