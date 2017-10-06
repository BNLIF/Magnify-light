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
#include "TTree.h"
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
    data->draw_beam_flashes();


    vw->can->cd(2);
    data->draw_cosmic();
    gPad->SetLogz();

    vw->can->cd(3);
    data->draw_totalPE_vs_time();

    vw->can->cd(4);
    data->draw_pmts();
    FlashChanged();

    InitConnections();
}

GuiController::~GuiController()
{
    // gApplication->Terminate(0);
}

void GuiController::InitConnections()
{
    mw->fMenuFile->Connect("Activated(int)", "GuiController", this, "HandleMenu(int)");

    cw->flashEntry->SetLimitValues(0, data->nFlash-1);
    cw->flashEntry->Connect("ValueSet(Long_t)", "GuiController", this, "FlashChanged()");
    cw->allFlashButton->Connect("Clicked()", "GuiController", this, "UpdateShowAllFlashes()");
    cw->beamWfEntry->Connect("ValueSet(Long_t)", "GuiController", this, "BeamWfChanged()");
    cw->beamRawWfButton->Connect("Clicked()", "GuiController", this, "BeamWfChanged()");
    cw->beamL1WfButton->Connect("Clicked()", "GuiController", this, "BeamWfChanged()");

    vw->can->Connect(
        "ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
        "GuiController",
        this,
        "ProcessCanvasEvent(Int_t,Int_t,Int_t,TObject*)"
    );
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

    vw->can->cd(2);
    data->draw_time();
    vw->can->GetPad(2)->Modified();
    vw->can->GetPad(2)->Update();
}

void GuiController::UpdateShowAllFlashes()
{
    vw->can->cd(2);

    if (cw->allFlashButton->IsDown()) {
        // cout << "show all flashes" << endl;
        vw->SetPalette(1);
        data->T_flash->Draw("Iteration$:time>>hAllflashes(8000,-3200,4800,32,0,32)","PE>0.1","same,colz");
        TH1F *h = (TH1F*)gDirectory->FindObject("hAllflashes");
        if (h) {
            h->GetZaxis()->SetLabelSize(0);
        }
    }
    else {
        vw->SetPalette(4);
        TH1F *h = (TH1F*)gDirectory->FindObject("hAllflashes");
        if (h) delete h;
    }

    vw->can->GetPad(2)->Modified();
    vw->can->GetPad(2)->Update();
}

void GuiController::BeamWfChanged()
{
    int id = cw->beamWfEntry->GetNumber();

    vw->can->cd(3);
    data->set_current_beam_wf(id);
    data->draw_beam_wf();
    data->draw_beam_wf_l1(cw->beamL1WfButton->IsDown());
    data->draw_beam_wf_raw(cw->beamRawWfButton->IsDown());
    vw->can->GetPad(3)->Modified();
    vw->can->GetPad(3)->Update();
}

void GuiController::ProcessCanvasEvent(Int_t ev, Int_t x, Int_t y, TObject *selected)
{
    if (ev == 11) { // clicked
        // if (!(selected->IsA() == TH2F::Class()
        //     || selected->IsA() == TBox::Class()
        //     || selected->IsA() == TLine::Class()
        // )) return;
        TVirtualPad* pad = vw->can->GetClickSelectedPad();
        int padNo = pad->GetNumber();
        double xx = pad->AbsPixeltoX(x);
        double yy = pad->AbsPixeltoY(y);
        cout << "pad " << padNo << ": (" << xx << ", " << yy << ")" << endl;
        if (padNo == 1) {
            data->set_current_beam_wf(yy);
            cw->beamWfEntry->SetNumber(data->current_beam_wf);
            BeamWfChanged();
        }
    }

}

void GuiController::HandleMenu(int id)
{
    // const char *filetypes[] = {"ROOT files", "*.root", 0, 0};
    switch (id) {
        case M_FILE_EXIT:
            gApplication->Terminate(0);
            break;
    }
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