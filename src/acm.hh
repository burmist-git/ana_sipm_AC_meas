#ifndef acm_hh
#define acm_hh

//My
#include "acmbase.hh"

//c, c++
#include <stdlib.h>
#include <iostream>
#include <vector>

//root
#include <TROOT.h>

class TChain;
class TFile;
class TTree;
class TString;
class TBranch;

const unsigned int _npoints = 5000;
const unsigned int _nwf = 10000;

struct meas {
  //-------------------------------
  TString fileName;
  Float_t V_bias;
  Float_t T;
  Float_t I_diode;
  std::vector<std::vector<Float_t>> wf_a;
  std::vector<Float_t> wf_t;
  //-------------------------------
  std::vector<Float_t> t_amin;
  std::vector<Float_t> amin;
  std::vector<Float_t> t_amax;
  std::vector<Float_t> amax;
  //-------------------------------
  std::vector<Float_t> amax_DCR;
  std::vector<Float_t> amax_TRG;
  std::vector<Float_t> amax_AFT;
  //-------------------------------
  meas(){
   fileName = "";
   V_bias = -999.0;
   T = -999.0;
   I_diode = -999.0;
   for(unsigned int j = 0;j<_nwf;j++){
     std::vector<Float_t> v_f;
     for(unsigned int i = 0;i<_npoints;i++){
       v_f.push_back(0.0);
     }
     wf_a.push_back(v_f);
     //-------------------------------
     t_amin.push_back(-999.0);
     amin.push_back(-999.0);
     t_amax.push_back(-999.0);
     amax.push_back(-999.0);
     //-------------------------------
     amax_DCR.push_back(-999.0);
     amax_TRG.push_back(-999.0);
     amax_AFT.push_back(-999.0);
     //-------------------------------
   }
   for(unsigned int i = 0;i<_npoints;i++){
     wf_t.push_back(0.0);
   }
  }
  void printInfo(){
    std::cout<<fileName<<std::endl
	     <<V_bias<<std::endl;
  }
};

class acm: public acmbase {
public:
  acm(TString fileList, TString info_file, TString measCond_file, TString temperature_file, TString trigger_window_file, TString histOut, TString fit_DCR_single, TString fit_CT_single, TString dataDir);
  
  acm(TString file, Int_t key) : acmbase(file, key)
  {
  }

  void Loop(TString histOut);
  void analyze();
  void analyze_DCR_and_CT(TString histOut, TString fit_DCR_single, TString fit_CT_single, TString dataDir);
  
  void load_data();
  
  std::vector<meas> v_meas;

  Double_t calculate_DCR(TH1D *h1_amax, Double_t threshold, Double_t d_time_window_ns);
  Double_t calculate_p_CT(TH1D *h1_amax, Double_t threshold);
  
};

#endif
