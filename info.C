//root
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TMultiGraph.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

void read_single_pe_file( TString single_pe_file, TGraphErrors *gr_err);
Double_t pol_one(Double_t *x, Double_t *par);

int info(){
  string mot;
  Double_t a;
  Double_t v;
  Double_t Vbd = 0.0;
  Double_t dv_safe = 3.0;
  Double_t v_max_tot;
  Double_t v_min = 36;
  Double_t v_max = 50;
  Double_t a_min = 18.7;
  Double_t a_max = 91.0;
  Int_t i;
  TGraphErrors *gr_err = new TGraphErrors();
  //read_single_pe_file("fit_DCR_single.pe", gr_err);
  //read_single_pe_file("fit_CT_single.pe", gr_err);
  if(gr_err->GetN() < 5){
    ifstream file_info ("info");
    if (file_info.is_open()){
      while(file_info>>mot){
	//cout<<mot<<endl;
	if(mot == "Single_p_e_amplitude_s"){
	  file_info>>mot>>mot>>mot;
	  while(file_info>>mot>>a){
	    v = atof(mot.c_str());
	    a = a/1000.0;
	    //cout<<v<<endl;
	    if(mot != "Single_p_e_amplitude_s"){
	      i = gr_err->GetN();
	      if(i == 0)
	        v_max_tot = v;
	      gr_err->SetPoint(i,v,a);
	      gr_err->SetPointError(i,v*0.001,a*0.01 + 0.002);
	    }
	    else{
	      break;
	    }
	  }
	}
	else if (mot == "Vbd"){
	  file_info>>mot>>Vbd;	
	}
      }
    }
    else{
      cout<<"Unable to open file"<<endl;
    }
    file_info.close();
  }
  //
  TCanvas *c1 = new TCanvas("c1","c1",10,10,800,800);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  //
  c1->SetGridx();
  c1->SetGridy();
  c1->SetRightMargin(0.015);
  c1->SetLeftMargin(0.13);
  c1->SetTopMargin(0.015);
  c1->SetBottomMargin(0.08);
  //
  cout<<"Vbd "<<Vbd<<endl;
  //
  gr_err->SetMarkerStyle(20);
  gr_err->SetMarkerColor(kBlack);
  gr_err->SetLineColor(kBlack);
  gr_err->SetLineWidth(2);
  //
  gr_err->Draw("APL");
  //
  TF1 *func = new TF1("pol_one",pol_one,v_min,v_max,2);
  func->SetParameter(0,(a_max - a_min)/(v_max - v_min));
  func->SetParameter(1,0.0);
  if(Vbd > 0.0){
    cout<<"Vbd     "<<Vbd<<endl
	<<"dv_safe "<<dv_safe<<endl
	<<"v_max   "<<v_max<<endl;
    //gr_err->Fit("pol1","","",Vbd + dv_safe,v_max);
    //gr_err->Fit("pol1");
    gr_err->Fit("pol_one");
    for(Int_t ii = 0;ii<gr_err->GetN();ii++){
      Double_t xx, yy;
      gr_err->GetPoint(ii,xx,yy);
      cout<<xx<<" "<<yy<<endl;
    }
  }
  else{
    //cout<<"sqwqqwq     "<<endl;
    //gr_err->Fit("pol1","L","",32,50);
    //gr_err->Fit("pol1");
    gr_err->Fit("pol_one");
  }
  //
  gr_err->GetXaxis()->SetTitle("V_{bias}, V");
  gr_err->GetYaxis()->SetTitle("Single p.e. amplitude, V");
  //
  //TF1 *f_fit = gr_err->GetFunction("pol1");
  if(func != NULL){
    //cout<<f_fit->Eval(34.0)<<endl;
    //
    //
    const Int_t nn = 19;
    Double_t Vbias[nn]={ 30.0,    35.0,    35.5,    36.0,    36.5,
			 37.0,    38.0,    39.0,    40.0,    41.0,
			 42.0,    43.0,    44.0,    45.0,    46.0,
			 47.0,    48.0,    49.0,    50.0};
    ////////////////
    const Int_t nscale = 5;
    Double_t scale[nscale]         = {  0.005,  0.010,  0.020,  0.050,  0.100};
    Double_t offset_nscale[nscale] = {   -2.0,   -3.0,   -3.0,   -3.0,   -3.0};
    Double_t scale_v;
    Double_t offset_v;
    Double_t two_pe;
    ////////////////
    //
    //
    //
    cout<<"-------------------------------------------------------------------------------------"<<endl;
    cout<<"Preliminary_s"<<endl;
    cout<<setw(10)<<"ID"
        <<setw(10)<<"Vbias"
        <<setw(15)<<"1.0p.e."
        <<setw(15)<<"0.5p.e."
        <<setw(15)<<"2.0p.e."
        <<setw(10)<<"scale"
        <<setw(10)<<"offset"<<endl;
    for(Int_t i = 0;i<nn;i++){
      two_pe = floor(2.0*func->Eval(Vbias[i])*1000.0)/1000.0;
      cout<<setw(10)<<i
	  <<setw(10)<<Vbias[i]
	  <<setw(15)<<floor(func->Eval(Vbias[i])*1000.0)/1000.0
	  <<setw(15)<<floor(func->Eval(Vbias[i])/2.0*1000.0)/1000.0
	  <<setw(15)<<two_pe;
      for(Int_t j = 0;j<nscale;j++){
	if(two_pe<(scale[j]*8 - (4 + offset_nscale[j])*scale[j])){
	  scale_v = scale[j];
	  offset_v = offset_nscale[j]*scale[j];
	  break;
	}
      }
      cout<<setw(10)<<scale_v
	  <<setw(10)<<offset_v<<endl;
    }
    cout<<"Preliminary_e"<<endl;
    cout<<"-------------------------------------------------------------------------------------"<<endl;
    //
    //
    //
  }
  return 0;
}

void read_single_pe_file( TString single_pe_file, TGraphErrors *gr_err){
  ifstream file_info (single_pe_file.Data());
  Double_t v, fit_status ,a;
  if (file_info.is_open()){
    while(file_info>>v>>fit_status>>a){
      if(fit_status==1.0){
	Int_t i = gr_err->GetN();
	gr_err->SetPoint(i,v,a);
	gr_err->SetPointError(i,v*0.003,a*0.003 + 0.003);
	cout<<v<<"  "<<a<<endl;
      }
    }
  }
}

Double_t pol_one(Double_t *x, Double_t *par){
   return par[0]*x[0]+par[1];
}
