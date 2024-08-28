#include "drawingcanva.hpp"
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <serialrelay.hpp>

DrawingCanva::DrawingCanva(int relaynumber,relayboard board,wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size): wxPanel(parent, id, pos, size){
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->SetBackgroundColour(wxColour(100,100,100));
    this->Bind(wxEVT_PAINT,&DrawingCanva::OnPaint,this);
    this->canvatype=relaynumber;
    this->board=board;
}

void DrawingCanva::OnPaint(wxPaintEvent &event){
    GetClientSize(&this->lenght,&this->width);
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc); 
    switch(this->canvatype){
        case 0:
        if(gc){
                wxPoint rectOrigin = this->FromDIP(wxPoint(0,0));
                wxSize rectsize = this->FromDIP(wxSize(100,200));;
                delete gc;
            }
        case 1:
            if(gc){
                DrawBoard(this->canvatype,this->lenght,this->width,gc);
            }
        break;
        case 2:
            if(gc){
                DrawBoard(this->canvatype,this->lenght,this->width,gc);
            }
        break;
        case 4:
            if(gc){
                DrawBoard(this->canvatype,this->lenght,this->width,gc);
            }
        break;
        case 8:
            if(gc){
                DrawBoard(this->canvatype,this->lenght,this->width,gc);
            }
        break;
        case 16:
            if(gc){
                DrawBoard(this->canvatype,this->lenght,this->width,gc);
            }
        break;
    }
}       

void DrawingCanva::DrawRelay(wxPoint pos,wxSize size,wxString id,wxGraphicsContext* gc){
    wxPoint rectOrigin = this->FromDIP(pos);
    wxSize rectsize = this->FromDIP(size);
    gc->SetPen( wxPen(wxColor(0,0,0),3));
    gc->DrawRectangle(rectOrigin.x,rectOrigin.y,rectsize.GetWidth(),rectsize.GetHeight());
    wxPoint2DDouble points[2] = { wxPoint2DDouble(rectOrigin.x+30,rectOrigin.y),wxPoint2DDouble(rectOrigin.x+30,rectOrigin.y+rectsize.GetHeight())};
    gc->DrawLines(2,points);
    wxFont f1(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    gc->SetFont(f1, *wxBLACK);
    if(id.length()==3){
        gc->DrawText(id,rectOrigin.x+33,rectOrigin.y+rectsize.GetHeight()/2);
    }
    else{
        gc->DrawText(id,rectOrigin.x+40,rectOrigin.y+rectsize.GetHeight()/2);
    }   
}

void DrawingCanva::DrawBoard(int relaynumber,int gclenght, int gcwidth ,wxGraphicsContext* gc){
    switch(relaynumber){
        case 1:
            DrawRelay(wxPoint((gclenght-relaylenght)/2,(gcwidth-relaywidth)/2),wxSize(relaylenght,relaywidth),wxT("K1"),gc);
        break; 
        case 2: 
            DrawRelay(wxPoint((gclenght-relaylenght)/2,gcwidth/2-(relaywidth+step/2)),wxSize(relaylenght,relaywidth),wxT("K1"),gc);
            DrawRelay(wxPoint((gclenght-relaylenght)/2,gcwidth/2+step/2),wxSize(relaylenght,relaywidth),wxT("K2"),gc);
        break; 
        case 4:
            DrawRelay(wxPoint((gclenght-relaylenght)/2,gcwidth/2-(2*relaywidth+step*3/2)),wxSize(relaylenght,relaywidth),wxT("K1"),gc);
            DrawRelay(wxPoint((gclenght-relaylenght)/2,gcwidth/2-(relaywidth+step/2)),wxSize(relaylenght,relaywidth),wxT("K2"),gc);
            DrawRelay(wxPoint((gclenght-relaylenght)/2,gcwidth/2+step/2),wxSize(relaylenght,relaywidth),wxT("K3"),gc);
            DrawRelay(wxPoint((gclenght-relaylenght)/2,gcwidth/2+relaywidth+step*3/2),wxSize(relaylenght,relaywidth),wxT("K4"),gc);
        break;
        case 8:
            {
                int leftcol = gclenght/2+step/2;
                int rightcol = gclenght/2-(relaylenght+step/2);
                wxPoint left1 = wxPoint(leftcol,gcwidth/2-(2*relaywidth+step*3/2));
                wxPoint left2 = wxPoint(leftcol,gcwidth/2-(relaywidth+step/2));
                wxPoint left3 = wxPoint(leftcol,gcwidth/2+step/2);
                wxPoint left4 = wxPoint(leftcol,gcwidth/2+relaywidth+step*3/2);
                wxPoint right1 = wxPoint(rightcol,gcwidth/2-(2*relaywidth+step*3/2));
                wxPoint right2 = wxPoint(rightcol,gcwidth/2-(relaywidth+step/2));
                wxPoint right3 = wxPoint(rightcol,gcwidth/2+step/2);
                wxPoint right4 = wxPoint(rightcol,gcwidth/2+relaywidth+step*3/2);
                std::vector<wxPoint> leftcolpos = {left1,left2,left3,left4};
                std::vector<wxPoint> rightcolpos = {right1,right2,right3,right4};
                std::vector<wxString> usbrelayleft   = {wxString("K4"),wxString("K3"),wxString("K2"),wxString("K1")};
                std::vector<wxString> usbrelayright  = {wxString("K8"),wxString("K7"),wxString("K6"),wxString("K5")};
                std::vector<wxString> usbmrelayleft  = {wxString("K5"),wxString("K6"),wxString("K7"),wxString("K8")};
                std::vector<wxString> usbmrelayright = {wxString("K4"),wxString("K3"),wxString("K2"),wxString("K1")};
                std::vector<wxString> usbbrelayleft  = {wxString("K8"),wxString("K7"),wxString("K6"),wxString("K5")};
                std::vector<wxString> usbbrelayright = {wxString("K1"),wxString("K2"),wxString("K3"),wxString("K4")};
                switch(board){
                    case USBRELAY:
                        for(int k = 0;k<4;k++){
                            DrawRelay(leftcolpos[k],wxSize(relaylenght,relaywidth),usbrelayleft[k],gc);
                        }
                        for(int k = 0;k<4;k++){
                            DrawRelay(rightcolpos[k],wxSize(relaylenght,relaywidth),usbrelayright[k],gc);
                        }
                    break;
                    case USBMRELAY:
                        for(int k = 0;k<4;k++){
                            DrawRelay(leftcolpos[k],wxSize(relaylenght,relaywidth),usbmrelayleft[k],gc);
                        }
                        for(int k = 0;k<4;k++){
                            DrawRelay(rightcolpos[k],wxSize(relaylenght,relaywidth),usbmrelayright[k],gc);
                        }
                    break;
                    case USBBRELAY:
                        for(int k = 0;k<4;k++){
                            DrawRelay(leftcolpos[k],wxSize(relaylenght,relaywidth),usbbrelayleft[k],gc);
                        }
                        for(int k = 0;k<4;k++){
                            DrawRelay(rightcolpos[k],wxSize(relaylenght,relaywidth),usbbrelayright[k],gc);
                        }
                    break;
                }  
            }
        break;
        case 16:
            {
                int leftcol = gclenght/2+step/2;
                int rightcol = gclenght/2-(relaylenght+step/2);
                wxPoint left1 = wxPoint(leftcol,gcwidth/2-(4*relaywidth+step*7/2));
                wxPoint left2 = wxPoint(leftcol,gcwidth/2-(3*relaywidth+step*5/2));
                wxPoint left3 = wxPoint(leftcol,gcwidth/2-(2*relaywidth+step*3/2));
                wxPoint left4 = wxPoint(leftcol,gcwidth/2-(relaywidth+step/2));
                wxPoint left5 = wxPoint(leftcol,gcwidth/2+step/2);
                wxPoint left6 = wxPoint(leftcol,gcwidth/2+relaywidth+step*3/2);
                wxPoint left7 = wxPoint(leftcol,gcwidth/2+2*relaywidth+step*5/2);
                wxPoint left8 = wxPoint(leftcol,gcwidth/2+3*relaywidth+step*7/2);
                wxPoint right1 = wxPoint(rightcol,gcwidth/2-(4*relaywidth+step*7/2));
                wxPoint right2 = wxPoint(rightcol,gcwidth/2-(3*relaywidth+step*5/2));
                wxPoint right3 = wxPoint(rightcol,gcwidth/2-(2*relaywidth+step*3/2));
                wxPoint right4 = wxPoint(rightcol,gcwidth/2-(relaywidth+step/2));
                wxPoint right5 = wxPoint(rightcol,gcwidth/2+step/2);
                wxPoint right6 = wxPoint(rightcol,gcwidth/2+relaywidth+step*3/2);
                wxPoint right7 = wxPoint(rightcol,gcwidth/2+2*relaywidth+step*5/2);
                wxPoint right8 = wxPoint(rightcol,gcwidth/2+3*relaywidth+step*7/2);

                std::vector<wxPoint> leftcolpos = {left1,left2,left3,left4,left5,left6,left7,left8};
                std::vector<wxPoint> rightcolpos = {right1,right2,right3,right4,right5,right6,right7,right8};

                std::vector<wxString> usbcrelayright   = {wxString("K1"),wxString("K2"),wxString("K3"),wxString("K4"),wxString("K5"),wxString("K6"),wxString("K7"),wxString("K8")};
                std::vector<wxString> usbcrelayleft  = {wxString("K16"),wxString("K15"),wxString("K14"),wxString("K13"),wxString("K12"),wxString("K11"),wxString("K10"),wxString("K9")};

                switch(board){
                    case USBCRELAY:
                        for(int k = 0;k<8;k++){
                            DrawRelay(leftcolpos[k],wxSize(relaylenght,relaywidth),usbcrelayleft[k],gc);
                        }
                        for(int k = 0;k<8;k++){
                            DrawRelay(rightcolpos[k],wxSize(relaylenght,relaywidth),usbcrelayright[k],gc);
                        }
                    break;
                    /*add other 16 relays board canva*/
                
                }
            }
        break; 
    }
}

int DrawingCanva::getstep(){
    return step;
}

int DrawingCanva::getrelaylenght(){
    return relaylenght;
}

int DrawingCanva::getrelaywidth(){
    return relaywidth;
}