#ifndef DATA_H
#define DATA_H

// class TH2F;
class TTree;
class TH1F;
class TFile;
class TEllipse;
class TBox;

#include <vector>

class Data {
public:
    Data();
    Data(const char* filename);

    virtual ~Data();

    TFile *rootFile;
    TTree *T_data;
    TTree *T_flash;

    int NPMT;
    int runNo;
    int subRunNo;
    int eventNo;
    int nFlash;

    std::vector<TH1F*> wfs_beam;
    std::vector<TH1F*> wfs_cosmic;
    std::vector<float> pmt_x;
    std::vector<float> pmt_y;
    std::vector<float> pmt_z;

    int current_flash;
    int type;
    double low_time, high_time;
    double time;
    double total_PE;
    double PE[32],PE_err[32];
    std::vector<int> *fired_channels;
    std::vector<double> *l1_fired_time;
    std::vector<double> *l1_fired_pe;

    void load_flash(int i=0);

    void draw_beam();
    void draw_cosmic();
    void draw_pmts();
    void draw_flash();
    void draw_time();

private:
    std::vector<TEllipse*> list_of_els;
    std::vector<TBox*> list_of_boxes;

    void load_runinfo();
    void load_location();
    void load_beam();
    void load_cosmic();
    void load_flash_tree();

    void print_flash();
    void printinfo();

};

#endif
