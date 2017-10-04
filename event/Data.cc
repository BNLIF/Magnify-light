#include "Data.h"

#include "TH2F.h"
#include "TH1I.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TEllipse.h"
#include "TMath.h"
#include "TLine.h"
#include "TBox.h"

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>

using namespace std;

Data::Data()
{}

Data::Data(const char* filename)
{
    NPMT = 32;
    rootFile = TFile::Open(filename);
    if (!rootFile) {
        string msg = "Unable to open ";
        msg += filename;
        throw runtime_error(msg.c_str());
    }

    load_runinfo();
    load_location();
    load_beam();
    load_cosmic();
    load_flash_tree();
    load_flash(0);
    printinfo();

}

Data::~Data()
{
    delete rootFile;
}

void Data::load_runinfo()
{
    T_data = (TTree*)rootFile->Get("T_data");
    if (T_data) {
        T_data->SetBranchAddress("runNo", &runNo);
        T_data->SetBranchAddress("subRunNo", &subRunNo);
        T_data->SetBranchAddress("eventNo", &eventNo);
        T_data->GetEntry(0);
    }
    else {
        runNo = 0;
        subRunNo = 0;
        eventNo = 0;
    }
}

void Data::load_location()
{
    pmt_x.resize(NPMT, 0);
    pmt_y.resize(NPMT, 0);
    pmt_z.resize(NPMT, 0);

    char line[256];
    int tmp, ch;
    float x, y, z;
    ifstream in("uBOpDetGeometry.txt");
    in.getline(line, 256);
    while (in.good()) {
        in >> tmp >> ch >> x >> y >> z;
        pmt_x[ch] = x;
        pmt_y[ch] = y;
        pmt_z[ch] = z;
    }
    in.close();
}

void Data::load_beam()
{
    TH2F* h2 = dynamic_cast<TH2F*>(rootFile->Get("hdecon"));
    for (int i=0; i<NPMT; i++) {
        TString name = TString::Format("hdecon_%i", i);
        h2->ProjectionX(name.Data(), i+1, i+1);
        TH1F *h = (TH1F*)gDirectory->FindObject(name);
        wfs_beam.push_back(h);
    }
}

void Data::load_cosmic()
{
    TClonesArray* op_wf = new TClonesArray;
    std::vector<int> *op_femch = new std::vector<int>;
    std::vector<double> *op_timestamp = new std::vector<double>;
    std::vector<double> *op_gain = new std::vector<double>;
    std::vector<double> *op_gainerror = new std::vector<double>;
    double triggerTime;

    T_data->SetBranchAddress("op_femch",&op_femch);
    T_data->SetBranchAddress("op_gain",&op_gain);
    T_data->SetBranchAddress("op_gainerror",&op_gainerror);
    T_data->SetBranchAddress("op_timestamp",&op_timestamp);
    T_data->SetBranchAddress("op_wf",&op_wf);
    T_data->SetBranchAddress("triggerTime",&triggerTime);

    T_data->GetEntry(0);
    int NTrigger = op_femch->size();
    cout << "#triggers: " << NTrigger << endl;
    cout << "#gains: " << op_gain->size() << endl;
    // cout << op_wf->GetEntries() << endl;

    for (int i=0; i<NPMT; i++) {
        TH1F *h = new TH1F(TString::Format("hcosimc_%i", i), "", 8000, -3200, 4800);
        wfs_cosmic.push_back(h);
    }

    for (int i=NPMT; i<NTrigger; i++) {
        double t = op_timestamp->at(i) - triggerTime;
        int ch = op_femch->at(i);
        TH1S *wf = (TH1S*)op_wf->At(i);
        int nBins = wf->GetNbinsX();
        double pe = 0;
        int baseline = 0;
        for (int j=1; j<nBins; j++) {
            int adc = wf->GetBinContent(j);
            if (j==1) {
                baseline = adc;
            }
            pe += (adc-baseline) / op_gain->at(ch) *2;
        }
        wfs_cosmic[ch]->Fill(t, pe);
        // cout << "ch: " << ch << ", t: " << t << ", pe: " << pe << endl;
    }

}

void Data::load_flash_tree()
{
    T_flash = (TTree*)rootFile->Get("T_flash");

    fired_channels = new std::vector<int>;
    l1_fired_time = new std::vector<double>;
    l1_fired_pe = new std::vector<double>;

    T_flash->SetBranchAddress("type",&type);
    T_flash->SetBranchAddress("low_time",&low_time);
    T_flash->SetBranchAddress("high_time",&high_time);
    T_flash->SetBranchAddress("time",&time);
    T_flash->SetBranchAddress("total_PE",&total_PE);
    T_flash->SetBranchAddress("PE",PE);
    T_flash->SetBranchAddress("PE_err",PE_err);
    T_flash->SetBranchAddress("fired_channels",&fired_channels);
    T_flash->SetBranchAddress("l1_fired_time",&l1_fired_time);
    T_flash->SetBranchAddress("l1_fired_pe",&l1_fired_pe);

    nFlash = T_flash->GetEntries();
}

void Data::load_flash(int i)
{
    current_flash = i;
    T_flash->GetEntry(i);
}

void Data::print_flash()
{
    cout << "flash #" << current_flash
    << ", @" << time << " (" << low_time << ", " << high_time << ")"
    << endl;
    cout << "PE: ";
    for (int i = 0; i < NPMT; ++i) {
        cout << PE[i] << " ";
    }
    cout << endl;
}

void Data::draw_beam()
{
    TH1F *hc = (TH1F*) wfs_beam[0]->Clone();
    hc->Reset();
    int nBins = hc->GetNbinsX();
    for (int i=0; i<nBins; i++) {
        hc->SetBinContent(i, 1);
    }
    TH2F *hDummy = new TH2F("hDummy","Beam Trigger", 250, 0, 245, 100, -1.0, 32.0);
    hDummy->Draw();
    hDummy->GetXaxis()->SetTitle("x 0.94 #mus");
    hDummy->GetYaxis()->SetTitle("PMT ID");
    for (int i=0; i<NPMT; i++) {
        TH1F *h = (TH1F*)wfs_beam[i]->Clone();
        h->SetName(TString::Format("hdecon_clone_%i", i));
        h->Add(hc, i*10);
        h->Scale(1./10);
        // h->SetLineColor(kGray+1);
        h->Draw("HIST, same");
    }
}

void Data::draw_cosmic()
{
    TH1F *hc = (TH1F*) wfs_cosmic[0]->Clone();
    hc->Reset();
    int nBins = hc->GetNbinsX();
    for (int i=0; i<nBins; i++) {
        hc->SetBinContent(i, 1);
    }
    TH2F *hDummy = new TH2F("hDummy_cosmic","Cosmic Trigger", 8000, -3200, 4800, 32, 0, 32);
    hDummy->Draw();
    hDummy->GetXaxis()->SetTitle("#mus");
    hDummy->GetYaxis()->SetTitle("PMT ID");
    for (int i=0; i<NPMT; i++) {
        TH1F *h = (TH1F*)wfs_cosmic[i];
        for (int j=1; j<8000; j++) {
            double content = h->GetBinContent(j);
            hDummy->Fill(h->GetBinCenter(j), i, content);
        }
    }
    hDummy->Draw("colz");
    // hDummy->GetZaxis()->SetRangeUser(20, 500);
}

void Data::draw_pmts()
{
    TH2F *hBoundary = new TH2F("hBoundary", "PMT Hit Map", 100, 0, 1040, 100, -116, 116);
    hBoundary->Draw();
    hBoundary->GetXaxis()->SetTitle("z [cm]");
    hBoundary->GetYaxis()->SetTitle("y [cm]");
    for (int i=0; i<NPMT; i++) {
        TEllipse *el = new TEllipse(pmt_z[i], pmt_y[i], 10.);
        el->Draw();
        // cout << i << " " << pmt_x[i] << " " << pmt_y[i] << " " << pmt_z[i] << " " << endl;
    }
}

void Data::draw_flash()
{
    int size = list_of_els.size();
    for (int i=0; i<size; i++) {
        list_of_els[i]->Delete();
    }
    list_of_els.clear();

    for (int i=0; i<NPMT; i++) {
        if (PE[i]<0.1) continue;
        double radius = TMath::Sqrt(PE[i]);
        TEllipse *el = new TEllipse(pmt_z[i], pmt_y[i], radius);
        list_of_els.push_back(el);

        el->SetFillColor(kRed);
        // el->SetLineColor(kRed);
        el->Draw();
    }

}

void Data::draw_time()
{
    int size = list_of_boxes.size();
    for (int i=0; i<size; i++) {
        list_of_boxes[i]->Delete();
    }
    list_of_boxes.clear();

    for (int i=0; i<NPMT; i++) {
        if (PE[i]<0.1) continue;
        TBox *box = new TBox(low_time, i, high_time, i+1);
        list_of_boxes.push_back(box);

        box->SetFillColor(kRed);
        // box->SetLineColor(kRed);
        box->Draw();
    }

}

void Data::printinfo()
{
    cout << runNo << " " << subRunNo << " " << eventNo << endl;
    cout << "#waveforms: " << wfs_beam.size() << endl;
}





