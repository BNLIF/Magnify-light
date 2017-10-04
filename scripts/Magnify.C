// void Magnify(const char* filename=0, const char* frame="decon");
void Magnify(const char* filename=0)
{
    // gROOT->Reset();
    // gROOT->ProcessLine(".x loadClasses.C" );

    GuiController *gc = new GuiController(
        gClient->GetRoot(),
        1600,
        900,
        filename
    );
}
