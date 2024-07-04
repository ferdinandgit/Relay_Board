#include <relayapp.hpp>
#include <mainframe.hpp>


wxIMPLEMENT_APP(relayApp);




bool relayApp::OnInit()
{
    MainFrame *mainFrame = new MainFrame("Seeit Relay Controller");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    mainFrame->Show(true);
    return true;
};
