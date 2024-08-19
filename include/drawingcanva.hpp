#pragma once 
#include <wx/wx.h>
#include <string>
#include <serialrelay.hpp>


class DrawingCanva : public wxPanel{
    public:
       
        DrawingCanva(int type,relayboard boardtype,wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
        int getstep();
        int getrelaylenght();
        int getrelaywidth(); 
    private:
        void OnPaint(wxPaintEvent &event);
        void DrawRelay(wxPoint pos,wxSize size,wxString id,wxGraphicsContext* gc);
        void DrawBoard(int relaynumber,int centerx, int centery ,wxGraphicsContext* gc);
        int canvatype;
        int lenght;
        int width;
        int step = 10;
        int relaylenght = 120;
        int relaywidth = 70;
        relayboard board; 

};