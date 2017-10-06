#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "TGFrame.h"
#include "RQ_OBJECT.h"
#include "TString.h"

class MainWindow;
class ViewWindow;
class ControlWindow;
class Data;
// class TH1F;


class GuiController
{
    RQ_OBJECT("GuiController")
public:
    GuiController(const TGWindow *p, int w, int h, const char* filename=0);
    virtual ~GuiController();

    void InitConnections();
    void FlashChanged();
    void UpdateShowAllFlashes();
    void BeamWfChanged();

    void HandleMenu(int id);
    void ProcessCanvasEvent(Int_t ev, Int_t x, Int_t y, TObject *selected);


    TString OpenDialog();

    MainWindow *mw;
    ViewWindow *vw;
    ControlWindow *cw;
    Data *data;
    // TH1F *hCurrent[3];
};

#endif
