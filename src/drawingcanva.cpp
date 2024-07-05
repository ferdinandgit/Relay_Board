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
                DrawRelay(wxPoint(0,0),wxT("K1"),gc);
            }
        break;
    }
}       

void DrawingCanva::DrawRelay(wxPoint pos,wxString id,wxGraphicsContext* gc){
    wxPoint rectOrigin = this->FromDIP(wxPoint(0,0));
    wxSize rectsize = this->FromDIP(wxSize(120,70));
    gc->SetPen( wxPen(wxColor(0,0,0),3));
    gc->DrawRectangle(rectOrigin.x,rectOrigin.y,rectsize.GetWidth(),rectsize.GetHeight());
}

void DrawingCanva::DrawBoard(){
    
}