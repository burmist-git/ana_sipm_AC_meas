#ifndef acmbase_hh
#define acmbase_hh

#include <iostream>
#include <iomanip>

#include <TROOT.h>

class TChain;
class TFile;
class TTree;
class TString;
class TBranch;
class TGraph;
class TH1D;
class TH2D;
class TProfile;

struct metadata {
  //
  std::vector<TString> v_rootFileList;
  //
  TString DeviceShortName;
  TString Date_of_measurements;
  TString Measurements_type;
  //
  Float_t Vbd;
  Float_t temperature_breakdown;
  Float_t vbd_temperature_coeff;
  //
  std::vector<Float_t> v_temperature;
  std::vector<Float_t> v_Vbd_current;
  std::vector<Float_t> v_trigger_window_L;
  std::vector<Float_t> v_trigger_window_R;
  //
  std::vector<Float_t> v_Vbias;
  std::vector<Float_t> v_Vover;
  std::vector<Float_t> v_singlepe_pre;
  std::vector<Float_t> v_halfpe_pre;
  std::vector<Float_t> v_twope_pre;
  std::vector<Float_t> v_scale;
  std::vector<Float_t> v_offset;
  //
  std::vector<Float_t> v_measCond_Vbias;
  std::vector<Float_t> v_measCond_threshold;
  std::vector<Float_t> v_measCond_scale;
  std::vector<Float_t> v_measCond_offset;
  //
  std::vector<Float_t> v_fit_DCR_single_status;
  std::vector<Float_t> v_fit_DCR_single_mean;  
  //
  std::vector<Float_t> v_fit_CT_single_status;
  std::vector<Float_t> v_fit_CT_single_mean;  
  //-------------------------------
  metadata(){
    //
    DeviceShortName = " ";
    Date_of_measurements = " ";
    Measurements_type = " ";
    //
    Vbd = -999.0;
    temperature_breakdown = -999.0;
    vbd_temperature_coeff = -999.0;
  }
  void printInfo(){
    std::cout<<"DeviceShortName       "<<DeviceShortName<<std::endl
	     <<"Date_of_measurements  "<<Date_of_measurements<<std::endl
	     <<"Measurements_type     "<<Measurements_type<<std::endl
	     <<"Vbd                   "<<Vbd<<std::endl
	     <<"temperature_breakdown "<<temperature_breakdown<<std::endl
	     <<"vbd_temperature_coeff "<<vbd_temperature_coeff<<std::endl;
    for(unsigned int j = 0;j<v_rootFileList.size();j++)
      std::cout<<v_rootFileList.at(j)<<std::endl;
    std::cout<<std::setw(10)<<"Vbias"
	     <<std::setw(10)<<"1p.e."
	     <<std::setw(10)<<"0.5p.e."
	     <<std::setw(10)<<"2p.e."
      	     <<std::setw(10)<<"scale"
	     <<std::setw(10)<<"offset"
	     <<std::endl;
    for(unsigned int j = 0;j<v_Vbias.size();j++){
      std::cout<<std::setw(10)<<v_Vbias.at(j)
	       <<std::setw(10)<<v_singlepe_pre.at(j)
	       <<std::setw(10)<<v_halfpe_pre.at(j)
	       <<std::setw(10)<<v_twope_pre.at(j)
	       <<std::setw(10)<<v_scale.at(j)
	       <<std::setw(10)<<v_offset.at(j)
	       <<std::endl;
    }
    std::cout<<std::setw(10)<<"trig_L"
	     <<std::setw(10)<<"trig_R"
	     <<std::endl;
    for(unsigned int j = 0;j<v_trigger_window_L.size();j++)
      std::cout<<std::setw(10)<<v_trigger_window_L.at(j)
	       <<std::setw(10)<<v_trigger_window_R.at(j)
	       <<std::endl;
    std::cout<<std::setw(10)<<"t"
	     <<std::setw(10)<<"Vbd"
      	     <<std::setw(15)<<"dVbd"
	     <<std::endl;
    for(unsigned int j = 0;j<v_temperature.size();j++)
      std::cout<<std::setw(10)<<v_temperature.at(j)
	       <<std::setw(10)<<v_Vbd_current.at(j)
	       <<std::setw(15)<<v_Vbd_current.at(j)-Vbd
	       <<std::endl;

    std::cout<<std::setw(15)<<"measCond_Vbias"
	     <<std::setw(15)<<"measCond_thres"
      	     <<std::setw(15)<<"measCond_scale"
	     <<std::setw(15)<<"measCond_offse"
	     <<std::endl;
    for(unsigned int j = 0;j<v_measCond_Vbias.size();j++)
      std::cout<<std::setw(15)<<v_measCond_Vbias.at(j)
	       <<std::setw(15)<<v_measCond_threshold.at(j)
	       <<std::setw(15)<<v_measCond_scale.at(j)
	       <<std::setw(15)<<v_measCond_offset.at(j)
	       <<std::endl;
    std::cout<<std::setw(25)<<"fit_DCR_single_status"
	     <<std::setw(25)<<"fit_DCR_single_mean"
	     <<std::endl;
    for(unsigned int j = 0;j<v_fit_DCR_single_status.size();j++)
      std::cout<<std::setw(25)<<v_fit_DCR_single_status.at(j)
	       <<std::setw(25)<<v_fit_DCR_single_mean.at(j)
	       <<std::endl;
    std::cout<<std::setw(25)<<"fit_CT_single_status"
	     <<std::setw(25)<<"fit_CT_single_mean"
	     <<std::endl;
    for(unsigned int j = 0;j<v_fit_CT_single_status.size();j++)
      std::cout<<std::setw(25)<<v_fit_CT_single_status.at(j)
	       <<std::setw(25)<<v_fit_CT_single_mean.at(j)
	       <<std::endl;
  }
  void calculate_Vbd_current(){
    Float_t b_vbd = Vbd - vbd_temperature_coeff/1000.0*temperature_breakdown;
    for(unsigned int j = 0;j<v_temperature.size();j++)
      v_Vbd_current.push_back(vbd_temperature_coeff/1000.0*v_temperature.at(j) + b_vbd);
  }
  void calculate_Vover(){
    for(unsigned int j = 0;j<v_Vbias.size();j++)
      v_Vover.push_back(v_Vbias.at(j) - v_Vbd_current.at(j));
  }
};
  
class acmbase {

public :
  acmbase(TString fileList, TString info_file, TString measCond_file, TString temperature_file, TString trigger_window_file, TString fit_DCR_single, TString fit_CT_single);
  acmbase(TString inFileName, Int_t keyID);
  ~acmbase();
  Int_t GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void Init(TTree *tree);
  void Loop(TString histOut);
  Bool_t Notify();
  void Show(Long64_t entry = -1);
  Int_t Cut(Long64_t entry);
  
protected :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  //Int_t           evt;
  //Int_t           run;
  //Float_t         pValue;
  //...
  //...
  Int_t           waveformID;
  Float_t         dt;
  Int_t           NPoints;
  Float_t         Vbias;
  Float_t         Ibias;
  Float_t         IDiode;
  Float_t         IDiodeStd;
  Float_t         Volt[5000]; //[NPoints]
  //
  //---------------------------------------------------
  // ADD HERE :
  //Tree name
  //const TString treeName = "arich";
  const TString treeName = "T";
  //../data/21.02.2023/
  static const Int_t nChannels = 19;

  metadata metadata_str;
  
  Int_t Vbias_counter;
  
  //---------------------------------------------------
  
  // List of branches
  //TBranch        *b_evt;
  //TBranch        *b_run;
  //TBranch        *b_pValue;
  //...
  //...
  //
  //---------------------------------------------------
  // ADD HERE :
  TBranch        *b_waveformID;   //!
  TBranch        *b_dt;   //!
  TBranch        *b_NPoints;   //!
  TBranch        *b_Vbias;   //!
  TBranch        *b_Ibias;   //!
  TBranch        *b_IDiode;   //!
  TBranch        *b_IDiodeStd;   //!
  TBranch        *b_Volt;   //!
  //---------------------------------------------------
  void tGraphInit(TGraph *gr[nChannels], TString grName, TString grTitle);
  void h1D1Init(TH1D *h1D1[nChannels],TString h1name, TString h1Title,
		Int_t Nbin, Float_t Vmin, Float_t Vmax);
  void h2D2Init(TH2D *h2D1[nChannels],TString h2name, TString h2Title,
                Int_t Nbin1, Float_t Vmin1, Float_t Vmax1,
                Int_t Nbin2, Float_t Vmin2, Float_t Vmax2);
  void tProfInit(TProfile *tprof[nChannels],TString prname, TString prTitle,
                 Int_t Nbin, Float_t Vmin, Float_t Vmax);
  double getUnixTimeFromTime(double d_year, double d_month, double d_day, double d_hour, double d_min, double d_sec);  
  //
  void parse_fileList(TString fileList);
  void parse_info_file(TString info_file);
  void parse_measCond_file(TString measCond_file);
  void parse_temperature_file(TString temperature_file);
  void parse_trigger_window_file(TString trigger_window_file);
  void parse_fit_DCR_single(TString fit_DCR_single);
  void parse_fit_CT_single(TString fit_CT_single);
  //  
};

#endif
