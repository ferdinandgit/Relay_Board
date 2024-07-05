#pragma once 
#include <wx/wx.h>
#include <string>



class DrawingCanva : public wxPanel{
    public:
        DrawingCanva(int type,wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
    private:
        void OnPaint(wxPaintEvent &event);
        void DrawRelay(wxPoint pos,wxString id,wxGraphicsContext* gc);
        void DrawBoard();
        int canvatype;
};