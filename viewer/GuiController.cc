#include "GuiController.h"
#include "MainWindow.h"
#include "ViewWindow.h"
#include "ControlWindow.h"
#include "Data.h"

#include "TApplication.h"
#include "TSystem.h"
#include "TExec.h"
#include "TROOT.h"
#include "TMath.h"
#include "TGFileDialog.h"

#include "TGMenu.h"
#include "TGNumberEntry.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TH2I.h"
#include "TH1I.h"
#include "TBox.h"
#include "TLine.h"
#include "TColor.h"
#include "TString.h"

#include <iostream>
#include <vector>
// #include <algorithm>
using namespace std;

GuiController::GuiController(const TGWindow *p, int w, int h, const char* fn)
:mw(0), vw(0), data(0)
{

    mw = new MainWindow(p, w, h);
    vw = mw->fViewWindow;
    cw = mw->fControlWindow;
    // cout << fn << endl;

    TString filename;
    if (!fn) {
        filename = OpenDialog();
    }
    else {
        filename = fn;
    }

    data = new Data(filename.Data());

    mw->SetWindowName(TString::Format("Magnify: run %i, sub-run %i, event %i",
        data->runNo, data->subRunNo, data->eventNo));

    vw->can->cd(1);
    data->draw_beam();

    vw->can->cd(2);
    data->draw_cosmic();
    gPad->SetLogz();

    vw->can->cd(4);
    data->draw_pmts();
    data->draw_flash();

    InitConnections();
}

GuiController::~GuiController()
{
    // gApplication->Terminate(0);
}

void GuiController::InitConnections()
{
    // mw->fMenuFile->Connect("Activated(int)", "GuiController", this, "HandleMenu(int)");

    cw->flashEntry->SetLimitValues(0, data->nFlash);
    cw->flashEntry->Connect("ValueSet(Long_t)", "GuiController", this, "FlashChanged()");

    // for (int i=0; i<3; i++) {
    //     cw->threshEntry[i]->SetNumber(data->wfs.at(i)->threshold);
    // }
    // cw->threshEntry[0]->Connect("ValueSet(Long_t)", "GuiController", this, "ThresholdUChanged()");
    // cw->threshEntry[1]->Connect("ValueSet(Long_t)", "GuiController", this, "ThresholdVChanged()");
    // cw->threshEntry[2]->Connect("ValueSet(Long_t)", "GuiController", this, "ThresholdWChanged()");
    // cw->setThreshButton->Connect("Clicked()", "GuiController", this, "SetChannelThreshold()");
    // cw->threshScaleEntry->Connect("ValueSet(Long_t)", "GuiController", this, "SetChannelThreshold()");

    // cw->zAxisRangeEntry[0]->SetNumber(data->wfs.at(0)->zmin);
    // cw->zAxisRangeEntry[1]->SetNumber(data->wfs.at(0)->zmax);
    // cw->zAxisRangeEntry[0]->Connect("ValueSet(Long_t)", "GuiController", this, "ZRangeChanged()");
    // cw->zAxisRangeEntry[1]->Connect("ValueSet(Long_t)", "GuiController", this, "ZRangeChanged()");

    // cw->timeRangeEntry[0]->SetNumber(0);
    // cw->timeRangeEntry[1]->SetNumber(data->wfs.at(0)->nTDCs);

    // cw->channelEntry->Connect("ValueSet(Long_t)", "GuiController", this, "ChannelChanged()");
    // cw->timeEntry->Connect("ValueSet(Long_t)", "GuiController", this, "TimeChanged()");
    // cw->badChanelButton->Connect("Clicked()", "GuiController", this, "UpdateShowBadChannel()");
    // cw->badChanelButton->SetToolTipText(TString::Format("U: %lu, V: %lu, Y: %lu",
    //     data->wfs.at(0)->lines.size(),
    //     data->wfs.at(1)->lines.size(),
    //     data->wfs.at(2)->lines.size()
    // ));
    // cw->rawWfButton->Connect("Clicked()", "GuiController", this, "UpdateShowRaw()");
    // cw->unZoomButton->Connect("Clicked()", "GuiController", this, "UnZoom()");

    // // stupid way to connect signal and slots
    // vw->can->GetPad(1)->Connect("RangeChanged()", "GuiController", this, "SyncTimeAxis0()");
    // vw->can->GetPad(2)->Connect("RangeChanged()", "GuiController", this, "SyncTimeAxis1()");
    // vw->can->GetPad(3)->Connect("RangeChanged()", "GuiController", this, "SyncTimeAxis2()");
    // vw->can->GetPad(4)->Connect("RangeChanged()", "GuiController", this, "SyncTimeAxis3()");
    // vw->can->GetPad(5)->Connect("RangeChanged()", "GuiController", this, "SyncTimeAxis4()");
    // vw->can->GetPad(6)->Connect("RangeChanged()", "GuiController", this, "SyncTimeAxis5()");
    // // vw->can->GetPad(7)->Connect("RangeChanged()", "GuiController", this, "WfRangeChanged0()");
    // // vw->can->GetPad(8)->Connect("RangeChanged()", "GuiController", this, "WfRangeChanged1()");
    // // vw->can->GetPad(9)->Connect("RangeChanged()", "GuiController", this, "WfRangeChanged2()");


    // vw->can->Connect(
    //     "ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
    //     "GuiController",
    //     this,
    //     "ProcessCanvasEvent(Int_t,Int_t,Int_t,TObject*)"
    // );
}

void GuiController::FlashChanged()
{
    int flashId = cw->flashEntry->GetNumber();
    data->load_flash(flashId);
    vw->can->cd(4);
    data->draw_flash();
    data->print_flash();

    vw->can->GetPad(4)->Modified();
    vw->can->GetPad(4)->Update();
}


TString GuiController::OpenDialog()
{
    const char *filetypes[] = {"ROOT files", "*.root", 0, 0};
    TString currentDir(gSystem->WorkingDirectory());
    static TString dir(".");
    TGFileInfo fi;
    fi.fFileTypes = filetypes;
    fi.fIniDir    = StrDup(dir);
    new TGFileDialog(gClient->GetRoot(), mw, kFDOpen, &fi);
    dir = fi.fIniDir;
    gSystem->cd(currentDir.Data());

    if (fi.fFilename) {
        // UnZoom();
        cout << "open file: " << fi.fFilename << endl;
        return fi.fFilename;
    }
    else {
        gApplication->Terminate(0);
    }
    return "";

}