#include "drawingcanva.hpp"
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

DrawingCanva::DrawingCanva(int type,wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size): wxPanel(parent, id, pos, size){
    this->SetBackgroundStyle(wxBG_STYLE_PAINT);
    this->SetBackgroundColour(wxColour(100,100,100));
    this->Bind(wxEVT_PAINT,&DrawingCanva::OnPaint,this);
    this->canvatype=type;
    
}

void DrawingCanva::OnPaint(wxPaintEvent &event){
    GetClientSize(&this->lenght,&this->width);
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();
    wxGraphicsContext *gc = wxGraphicsContext::Create(dc); 
    switch(this->canvatype){
        case 1:
            if(gc){
                wxPoint rectOrigin = this->FromDIP(wxPoint(0,0));
                wxSize rectsize = this->FromDIP(wxSize(100,200));
                gc->SetBrush(*wxRED_BRUSH);
                gc->DrawRectangle(rectOrigin.x,rectOrigin.y,rectsize.GetWidth(),rectsize.GetHeight());
                delete gc;
            }
        break;
        case 2:
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
    gc->DrawText(id,rectOrigin.x+40,rectOrigin.y+rectsize.GetHeight()/2);
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
        int leftcol = gclenght/2+step/2;
        int rightcol = gclenght/2-(relaylenght+step/2);
        DrawRelay(wxPoint(leftcol,gcwidth/2-(2*relaywidth+step*3/2)),wxSize(relaylenght,relaywidth),wxT("K4"),gc);
        DrawRelay(wxPoint(leftcol,gcwidth/2-(relaywidth+step/2)),wxSize(relaylenght,relaywidth),wxT("K3"),gc);
        DrawRelay(wxPoint(leftcol,gcwidth/2+step/2),wxSize(relaylenght,relaywidth),wxT("K2"),gc);
        DrawRelay(wxPoint(leftcol,gcwidth/2+relaywidth+step*3/2),wxSize(relaylenght,relaywidth),wxT("K1"),gc); 
        DrawRelay(wxPoint(rightcol,gcwidth/2-(2*relaywidth+step*3/2)),wxSize(relaylenght,relaywidth),wxT("K8"),gc);
        DrawRelay(wxPoint(rightcol,gcwidth/2-(relaywidth+step/2)),wxSize(relaylenght,relaywidth),wxT("K7"),gc);
        DrawRelay(wxPoint(rightcol,gcwidth/2+step/2),wxSize(relaylenght,relaywidth),wxT("K6"),gc);
        DrawRelay(wxPoint(rightcol,gcwidth/2+relaywidth+step*3/2),wxSize(relaylenght,relaywidth),wxT("K5"),gc);   
        break; 

    }
}