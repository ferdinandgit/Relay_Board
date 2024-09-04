#include <relayapp.hpp>
#include <mainframe.hpp>


wxIMPLEMENT_APP(relayApp);




bool relayApp::OnInit()
{
    MainFrame *mainFrame = new MainFrame("Drive Relay");
    mainFrame->SetIcon(wxIcon("IDI_ICON1"));
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    mainFrame->Show(true);
    return true;
};
