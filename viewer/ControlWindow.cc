#include "ControlWindow.h"

#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGButtonGroup.h"
#include "TGListBox.h"
#include "TGLabel.h"
#include "TGDoubleSlider.h"

#include <iostream>
using namespace std;

ControlWindow::ControlWindow(const TGWindow *p, int w, int h)
    :TGHorizontalFrame(p, w, h)
{
    // Beam Wfs
    TGGroupFrame *group_beam = new TGGroupFrame(this, "Beam window", kHorizontalFrame);
    group_beam->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(group_beam, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

    group_beam->AddFrame(new TGLabel(group_beam, "PMT: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
    beamWfEntry = new TGNumberEntry(group_beam, 0, 5, -1,
        TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEANonNegative,
        TGNumberFormat::kNELLimitMinMax,
        0, 31);
    group_beam->AddFrame(beamWfEntry, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    beamRawWfButton = new TGCheckButton(group_beam, "raw waveform  ");
    beamRawWfButton->SetState(kButtonUp);
    group_beam->AddFrame(beamRawWfButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    beamL1WfButton = new TGCheckButton(group_beam, "L1 waveform  ");
    beamL1WfButton->SetState(kButtonDown);
    group_beam->AddFrame(beamL1WfButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    // Flashes
    TGGroupFrame *group_flashes = new TGGroupFrame(this, "Flashes", kHorizontalFrame);
    group_flashes->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(group_flashes, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

    group_flashes->AddFrame(new TGLabel(group_flashes, "Flash: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
    flashEntry = new TGNumberEntry(group_flashes, 0, 5, -1,
        TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEANonNegative,
        TGNumberFormat::kNELLimitMinMax,
        0, 10);
    group_flashes->AddFrame(flashEntry, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    allFlashButton = new TGCheckButton(group_flashes, "all flashes  ");
    allFlashButton->SetState(kButtonUp);
    group_flashes->AddFrame(allFlashButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));
}

ControlWindow::~ControlWindow()
{
}
