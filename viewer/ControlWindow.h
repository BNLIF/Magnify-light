#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGNumberEntry;
class TGCheckButton;
class TGTextButton;

class ControlWindow: public TGHorizontalFrame
{
public:
    ControlWindow(const TGWindow *p, int w, int h);
    virtual ~ControlWindow();

    TGNumberEntry *flashEntry;
    TGCheckButton *allFlashButton;
    TGCheckButton *pmtIdButton;

    TGNumberEntry *beamWfEntry;
    TGCheckButton *beamRawWfButton;
    TGCheckButton *beamL1WfButton;

    ClassDef(ControlWindow, 0)
};

#endif
