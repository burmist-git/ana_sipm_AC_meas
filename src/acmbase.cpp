//my
#include "acmbase.hh"

//root
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

acmbase::acmbase(TString fileList, TString info_file, TString measCond_file, TString temperature_file, TString trigger_window_file, TString fit_DCR_single, TString fit_CT_single) : fChain(0), Vbias_counter(-2)
{
  parse_fileList(fileList);
  parse_info_file(info_file);
  parse_measCond_file(measCond_file);
  parse_temperature_file(temperature_file);
  parse_trigger_window_file(trigger_window_file);
  parse_fit_DCR_single(fit_DCR_single);
  parse_fit_CT_single(fit_CT_single);
  metadata_str.calculate_Vbd_current();
  metadata_str.printInfo();
  //assert(0);
  TChain *theChain = new TChain(treeName.Data());
  for(unsigned int j =0;j<metadata_str.v_rootFileList.size();j++){
    cout<<"    adding "<<metadata_str.v_rootFileList.at(j)<<endl;
    theChain->Add(metadata_str.v_rootFileList.at(j).Data(),-1);
  }
  Init(theChain);
}

acmbase::acmbase(TString inFileName, Int_t keyID) : fChain(0), Vbias_counter(-2)
{
  if(keyID == 1){
    ifstream indata;
    //TChain *theChain = new TChain("T");
    TChain *theChain = new TChain(treeName.Data());
    cout<<"        adding "<<inFileName<<endl;
    theChain->Add(inFileName.Data(),-1);
    Init(theChain);
  }
  else
    assert(0);
}

void acmbase::parse_fileList(TString fileList){
  string mot;
  ifstream file_in(fileList.Data());
  if (file_in.is_open()){
    while(file_in>>mot){
      //cout<<mot<<endl;
      metadata_str.v_rootFileList.push_back(mot);
    }
  }
  else{
    cout<<"Unable to open file : "<<fileList<<endl;
  }
}

void acmbase::parse_info_file(TString info_file){
  string mot;
  ifstream file_in(info_file.Data());
  if (file_in.is_open()){
    while(file_in>>mot){
      if(mot == "DeviceShortName"){
	file_in>>mot;
	file_in>>metadata_str.DeviceShortName;
      }
      if(mot == "Date_of_measurements"){
	file_in>>mot;
	file_in>>metadata_str.Date_of_measurements;
      }
      if(mot == "Measurements_type"){
	file_in>>mot;
	file_in>>metadata_str.Measurements_type;
      }
      if(mot == "Vbd"){
	file_in>>mot;
	file_in>>metadata_str.Vbd;
      }
      if(mot == "temperature_breakdown"){
	file_in>>mot;
	file_in>>metadata_str.temperature_breakdown;
      }
      if(mot == "vbd_temperature_coeff"){
	file_in>>mot;
	file_in>>metadata_str.vbd_temperature_coeff;
      }
      if(mot == "Preliminary_s"){
        file_in>>mot>>mot>>mot>>mot>>mot>>mot>>mot;
	Float_t Vbias_val;
	Float_t single_pe_val;
	Float_t halpf_pe_val;
	Float_t two_pe_val;
	Float_t scale_val;
	Float_t offset_val;
        while(file_in>>mot>>Vbias_val>>single_pe_val>>halpf_pe_val>>two_pe_val>>scale_val>>offset_val){
          if(mot != "Preliminary_e"){
	    metadata_str.v_Vbias.push_back(Vbias_val);
	    metadata_str.v_singlepe_pre.push_back(single_pe_val);
	    metadata_str.v_halfpe_pre.push_back(halpf_pe_val);
	    metadata_str.v_twope_pre.push_back(two_pe_val);
	    metadata_str.v_scale.push_back(scale_val);
	    metadata_str.v_offset.push_back(offset_val);
          }
          else{
            break;
          }
        }
      }
      //cout<<mot<<endl;
      //metadata_str.v_rootFileList.push_back(mot);
    }
  }
  else{
    cout<<"Unable to open file : "<<info_file<<endl;
  }
}

void acmbase::parse_measCond_file(TString measCond_file){
  //
  Float_t measCond_Vbias_val;
  Float_t measCond_threshold_val;
  Float_t measCond_scale_val;
  Float_t measCond_offset_val;
  //
  ifstream file_in(measCond_file.Data());
  if (file_in.is_open()){
    while(file_in>>measCond_Vbias_val>>measCond_threshold_val>>measCond_scale_val>>measCond_offset_val){  
      metadata_str.v_measCond_Vbias.push_back(measCond_Vbias_val);
      metadata_str.v_measCond_threshold.push_back(measCond_threshold_val);
      metadata_str.v_measCond_scale.push_back(measCond_scale_val);
      metadata_str.v_measCond_offset.push_back(measCond_offset_val);
    }
  }
  else{
    cout<<"Unable to open file : "<<measCond_file<<endl;
  }
}

void acmbase::parse_temperature_file(TString temperature_file){
  string mot;
  Float_t tempr;
  ifstream file_in(temperature_file.Data());
  if (file_in.is_open()){
    while(file_in>>mot>>tempr){
      metadata_str.v_temperature.push_back(tempr);
    }
  }
  else{
    cout<<"Unable to open file : "<<temperature_file<<endl;
  }
}

void acmbase::parse_trigger_window_file(TString trigger_window_file){
  string mot;
  Float_t trigger_window_L;
  Float_t trigger_window_R;
  ifstream file_in(trigger_window_file.Data());
  if (file_in.is_open()){
    while(file_in>>mot>>trigger_window_L>>trigger_window_R){
      metadata_str.v_trigger_window_L.push_back(trigger_window_L);
      metadata_str.v_trigger_window_R.push_back(trigger_window_R);
    }
  }
  else{
    cout<<"Unable to open file : "<<trigger_window_file<<endl;
  }
}

void acmbase::parse_fit_DCR_single(TString fit_DCR_single){
  string mot;
  Float_t fit_status;
  Float_t fit_mean;
  ifstream file_in(fit_DCR_single.Data());
  if (file_in.is_open()){
    while(file_in>>mot>>fit_status>>fit_mean){
      metadata_str.v_fit_DCR_single_status.push_back(fit_status);
      metadata_str.v_fit_DCR_single_mean.push_back(fit_mean);
    }
  }
  else{
    cout<<"Unable to open file : "<<fit_DCR_single<<endl;
  }
}

void acmbase::parse_fit_CT_single(TString fit_CT_single){
  string mot;
  Float_t fit_status;
  Float_t fit_mean;
  ifstream file_in(fit_CT_single.Data());
  if (file_in.is_open()){
    while(file_in>>mot>>fit_status>>fit_mean){
      metadata_str.v_fit_CT_single_status.push_back(fit_status);
      metadata_str.v_fit_CT_single_mean.push_back(fit_mean);
    }
  }
  else{
    cout<<"Unable to open file : "<<fit_CT_single<<endl;
  }
}

void acmbase::tGraphInit(TGraph *gr[nChannels], TString grName, TString grTitle){
  Int_t i;
  TString grNameh;
  TString grTitleh;
  for(i = 0;i<nChannels;i++){
    grNameh = grName; grNameh += "_"; grNameh += "ch_"; grNameh += i;
    grTitleh = grTitle; grTitleh += " "; grTitleh += "ch "; grTitleh += i;
    gr[i] = new TGraph();
    gr[i]->SetTitle(grTitleh.Data());
    gr[i]->SetName(grNameh.Data());
  }
}

void acmbase::h1D1Init(TH1D *h1D1[nChannels],TString h1name, TString h1Title,
		      Int_t Nbin, Float_t Vmin, Float_t Vmax){
  Int_t i;
  TString h1nameh;
  TString h1Titleh;
  for(i = 0;i<nChannels;i++){    
    h1nameh = h1name; h1nameh += "_"; h1nameh += "ch_"; h1nameh += i;
    h1Titleh = h1Title; h1nameh += " "; h1Titleh += "ch "; h1Titleh += i;
    h1D1[i] = new TH1D(h1nameh.Data(), h1Titleh.Data(),
                       Nbin, Vmin, Vmax);
  }
}

void acmbase::h2D2Init(TH2D *h2D1[nChannels],TString h2name, TString h2Title,
		      Int_t Nbin1, Float_t Vmin1, Float_t Vmax1,
		      Int_t Nbin2, Float_t Vmin2, Float_t Vmax2){
  Int_t i;
  TString h2nameh;
  TString h2Titleh;
  for(i = 0;i<nChannels;i++){    
    h2nameh = h2name; h2nameh += "_"; h2nameh += "ch_"; h2nameh += i;
    h2Titleh = h2Title; h2nameh += " "; h2Titleh += "ch "; h2Titleh += i;
    h2D1[i] = new TH2D(h2nameh.Data(), h2Titleh.Data(),
                       Nbin1, Vmin1, Vmax1,
		       Nbin2, Vmin2, Vmax2);
  }  
}

void acmbase::tProfInit(TProfile *tprof[nChannels],TString prname, TString prTitle,
		       Int_t Nbin, Float_t Vmin, Float_t Vmax){
  Int_t i;
  TString prnameh;
  TString prTitleh;
  for(i = 0;i<nChannels;i++){    
    prnameh = prname; prnameh += "_"; prnameh += "ch_"; prnameh += i;
    prTitleh = prTitle; prnameh += " "; prTitleh += "ch "; prTitleh += i;
    tprof[i] = new TProfile(prnameh.Data(), prTitleh.Data(), Nbin, Vmin, Vmax,"");
  }
}

double acmbase::getUnixTimeFromTime(double d_year, double d_month, double d_day, double d_hour, double d_min, double d_sec){
  //time_t timer;
  struct tm y2k = {0};
  y2k.tm_year = d_year - 1900; y2k.tm_mon = d_month-1; y2k.tm_mday = d_day;
  y2k.tm_hour = d_hour;   y2k.tm_min = d_min; y2k.tm_sec = d_sec;
  return difftime(mktime(&y2k),0);
}

acmbase::~acmbase(){
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void acmbase::Loop(TString histOut){
}

Int_t acmbase::GetEntry(Long64_t entry){
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t acmbase::LoadTree(Long64_t entry){
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void acmbase::Init(TTree *tree){
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);
  //fChain->SetBranchAddress("evt", &evt, &b_evt);
  //fChain->SetBranchAddress("run", &run, &b_run);
  //fChain->SetBranchAddress("pValue", &pValue, &b_pValue);
  //...
  //...
  //
  //---------------------------------------------------
  // ADD HERE :
  fChain->SetBranchAddress("waveformID", &waveformID, &b_waveformID);
  fChain->SetBranchAddress("dt", &dt, &b_dt);
  fChain->SetBranchAddress("NPoints", &NPoints, &b_NPoints);
  fChain->SetBranchAddress("Vbias", &Vbias, &b_Vbias);
  fChain->SetBranchAddress("Ibias", &Ibias, &b_Ibias);
  fChain->SetBranchAddress("IDiode", &IDiode, &b_IDiode);
  fChain->SetBranchAddress("IDiodeStd", &IDiodeStd, &b_IDiodeStd);
  fChain->SetBranchAddress("Volt", Volt, &b_Volt);
  //---------------------------------------------------
  Notify();
}

Bool_t acmbase::Notify(){
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  // cout<<"acmbase::Notify() "<<endl
  //  <<"NPoints           "<<NPoints<<endl
  //  <<"dt                "<<dt<<endl
  //  <<"Vbias             "<<Vbias<<endl;
  Vbias_counter++;
  return kTRUE;
}

void acmbase::Show(Long64_t entry){
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t acmbase::Cut(Long64_t entry){
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}
