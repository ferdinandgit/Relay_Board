#pragma once 
#include <wx/wx.h>
#include <string>



class DrawingCanva : public wxPanel{
    public:
       
        DrawingCanva(int type,wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
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
};