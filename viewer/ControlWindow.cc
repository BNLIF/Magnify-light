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
    TGGroupFrame *group_general = new TGGroupFrame(this, "Flashes", kHorizontalFrame);
    group_general->SetTitlePos(TGGroupFrame::kLeft);
    AddFrame(group_general, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 1, 1));

    group_general->AddFrame(new TGLabel(group_general, "Flash: "), new TGLayoutHints(kLHintsTop | kLHintsLeft,  2, 2, 1, 1));
    flashEntry = new TGNumberEntry(group_general, 0, 5, -1,
        TGNumberFormat::kNESInteger,
        TGNumberFormat::kNEANonNegative,
        TGNumberFormat::kNELLimitMinMax,
        0, 10);
    group_general->AddFrame(flashEntry, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

    allFlashButton = new TGCheckButton(group_general, "all flashes  ");
    allFlashButton->SetState(kButtonUp);
    group_general->AddFrame(allFlashButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));

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

    // rawWfButton = new TGCheckButton(group_general, "raw waveform  ");
    // rawWfButton->SetState(kButtonUp);
    // group_general->AddFrame(rawWfButton, new TGLayoutHints(kLHintsTop | kLHintsLeft,  1, 1, 1, 1));


}

ControlWindow::~ControlWindow()
{
}
