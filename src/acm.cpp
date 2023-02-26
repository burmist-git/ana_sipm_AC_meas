//my
#include "acm.hh"

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
#include <TRandom3.h>
#include <TMath.h>
#include <TLine.h>
#include <TF1.h>
#include <TLatex.h>
#include <TGraphErrors.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

acm::acm(TString fileList, TString info_file, TString measCond_file, TString temperature_file, TString trigger_window_file, TString histOut, TString fit_DCR_single, TString fit_CT_single, TString dataDir) : acmbase( fileList, info_file, measCond_file, temperature_file, trigger_window_file, fit_DCR_single, fit_CT_single)
{
  load_data();
  if(metadata_str.Measurements_type == "DCR_and_CT")
    analyze_DCR_and_CT( histOut, fit_DCR_single, fit_CT_single, dataDir);
  else
    assert(0);
}

void acm::load_data(){
  for(Int_t i = 0;i<nChannels;i++){
    meas m_meas;
    v_meas.push_back(m_meas);
  }
  //unsigned int vbias_i = 0;
  unsigned int wf_i = 0;
  /////////////////////////
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries = "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  Int_t Vbias_counter_old = -1;
  Int_t d_Vbias_counter = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //for (Long64_t jentry=0; jentry<nentries/10000;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    d_Vbias_counter = Vbias_counter_old - Vbias_counter;
    if(d_Vbias_counter){
      //cout<<"wf_i     "<<wf_i<<endl;
      wf_i = 0;
      v_meas.at((unsigned int)Vbias_counter).fileName = metadata_str.v_rootFileList.at((unsigned int)Vbias_counter);
      v_meas.at((unsigned int)Vbias_counter).V_bias = Vbias;
      v_meas.at((unsigned int)Vbias_counter).T = metadata_str.v_temperature.at((unsigned int)Vbias_counter);
      v_meas.at((unsigned int)Vbias_counter).I_diode = IDiode;
      cout<<metadata_str.v_rootFileList.at((unsigned int)Vbias_counter)<<endl
	  <<"Vbias_counter     "<<Vbias_counter<<endl
	  <<"Vbias             "<<Vbias<<endl
      	  <<"Vbias metadata    "<<metadata_str.v_Vbias.at((unsigned int)Vbias_counter)<<endl
	  <<"T                 "<<metadata_str.v_temperature.at((unsigned int)Vbias_counter)<<endl;
      if(TMath::Abs(Vbias - metadata_str.v_Vbias.at((unsigned int)Vbias_counter))>0.01){
	  cout<<" ERROR --> Vbias != metadata_str.v_Vbias.at((unsigned int)Vbias_counter)"<<endl
	      <<"Vbias             "<<Vbias<<endl
	      <<"Vbias metadata    "<<metadata_str.v_Vbias.at((unsigned int)Vbias_counter)<<endl;
	  assert(0);
      }
      for(int j =0;j<NPoints;j++){
	v_meas.at((unsigned int)Vbias_counter).wf_t.at((unsigned int)j) = dt*1.0e+9*j;
      }
      /////////////////////////////////
      /////////////////////////////////
    }
    Vbias_counter_old = Vbias_counter;
    ///////////////////////////////////
    for(int j =0;j<NPoints;j++){
      v_meas.at((unsigned int)Vbias_counter).wf_a.at(wf_i).at((unsigned int)j) = Volt[j];
    }
    wf_i++;
    ///////////////////////////////////
  }
}
 
void acm::Loop(TString histOut){
}

void acm::analyze_DCR_and_CT(TString histOut, TString fit_DCR_single, TString fit_CT_single, TString dataDir){
  cout<<"acm::analyze_DCR_and_CT ---> B "<<endl;
  Double_t amin_hist = 0.0;
  Double_t amax_hist = 0.2;
  Int_t nbins_hist = 200;
  //
  //amin_hist = metadata_str.v_measCond_scale.at(metadata_str.v_measCond_Vbias.size()-1)*(-4.0)
  //  - metadata_str.v_measCond_offset.at(metadata_str.v_measCond_offset.size()-1);
  //amax_hist = metadata_str.v_measCond_scale.at(metadata_str.v_measCond_Vbias.size()-1)*4.0
  //  - metadata_str.v_measCond_offset.at(metadata_str.v_measCond_offset.size()-1);
  //
  TH1D *h1_amax[nChannels];
  h1D1Init(h1_amax,"h1_amax","h1_amax", 2*nbins_hist, amin_hist, 2*amax_hist);
  TH1D *h1_amin[nChannels];
  h1D1Init(h1_amin,"h1_amin","h1_amin", 2*nbins_hist, amin_hist, 2*amax_hist);
  TH1D *h1_t_amax[nChannels];
  h1D1Init(h1_t_amax,"h1_t_amax","h1_t_amax",2000,-10,2010);
  TH1D *h1_t_amin[nChannels];
  h1D1Init(h1_t_amin,"h1_t_amin","h1_t_amin",2000,-10,2010);
  //
  TH1D *h1_amax_DCR[nChannels];
  h1D1Init(h1_amax_DCR,"h1_amax_DCR","h1_amax_DCR",nbins_hist,amin_hist,amax_hist);
  TH1D *h1_amax_TRG[nChannels];
  h1D1Init(h1_amax_TRG,"h1_amax_TRG","h1_amax_TRG",2*nbins_hist,amin_hist,2*amax_hist);
  TH1D *h1_amax_AFT[nChannels];
  h1D1Init(h1_amax_AFT,"h1_amax_AFT","h1_amax_AFT",nbins_hist,amin_hist,amax_hist);
  //
  unsigned int vbias_i = 0;
  unsigned int wf_i = 0;
  unsigned int point_i = 0;
  Float_t amin =  10;
  Float_t amax = -10;
  Float_t t_amin = 0.0;
  Float_t t_amax = 0.0;
  Float_t amax_DCR = -10;
  Float_t amax_TRG = -10;
  Float_t amax_AFT = -10;
  Float_t a;
  Float_t t;
  for(vbias_i = 0; vbias_i<v_meas.size();vbias_i++){
    cout<<v_meas.at(vbias_i).V_bias<<endl;
    for(wf_i = 0; wf_i < v_meas.at(vbias_i).wf_a.size(); wf_i++){
      amin =  10;
      amax = -10;
      amin =  10;
      amax_DCR = -10;
      amax_TRG = -10;
      amax_AFT = -10;
      for(point_i = 0; point_i < v_meas.at(vbias_i).wf_a.at(wf_i).size(); point_i++){
	a = v_meas.at(vbias_i).wf_a.at(wf_i).at(point_i);
	t = v_meas.at(vbias_i).wf_t.at(point_i);
	if(amin>a){
	  amin = a;
	  t_amin = t;
	}
	if(amax<a){
	  amax = a;
	  t_amax = t;
	}
	//DCR
	if((amax_DCR<a) && (t<metadata_str.v_trigger_window_L.at(vbias_i))){
	  amax_DCR = a; 
	}
	//TRG
	if((amax_TRG<a) && (t>=metadata_str.v_trigger_window_L.at(vbias_i)) && (t<=metadata_str.v_trigger_window_R.at(vbias_i))){
	  amax_TRG = a;
	}
	//AFT
	if((amax_AFT<a) && (t>metadata_str.v_trigger_window_R.at(vbias_i))){
	  amax_AFT = a;
	}
      }
      v_meas.at(vbias_i).t_amin.at(wf_i) = t_amin;
      v_meas.at(vbias_i).amin.at(wf_i) = amin;
      v_meas.at(vbias_i).t_amax.at(wf_i) = t_amax;
      v_meas.at(vbias_i).amax.at(wf_i) = amax;
      v_meas.at(vbias_i).amax_DCR.at(wf_i) = amax_DCR;
      v_meas.at(vbias_i).amax_TRG.at(wf_i) = amax_TRG;
      v_meas.at(vbias_i).amax_AFT.at(wf_i) = amax_AFT;
    }
  }
  ///////////////////////
  for(vbias_i = 0; vbias_i<v_meas.size();vbias_i++){
    for(wf_i = 0; wf_i < v_meas.at(vbias_i).wf_a.size(); wf_i++){
      //////////////////////
      h1_amax[vbias_i]->Fill(v_meas.at(vbias_i).amax.at(wf_i));
      h1_amin[vbias_i]->Fill(v_meas.at(vbias_i).amin.at(wf_i));
      h1_t_amax[vbias_i]->Fill(v_meas.at(vbias_i).t_amax.at(wf_i));
      h1_t_amin[vbias_i]->Fill(v_meas.at(vbias_i).t_amin.at(wf_i));
      //////////////////////
      h1_amax_DCR[vbias_i]->Fill(v_meas.at(vbias_i).amax_DCR.at(wf_i));
      h1_amax_TRG[vbias_i]->Fill(v_meas.at(vbias_i).amax_TRG.at(wf_i));
      h1_amax_AFT[vbias_i]->Fill(v_meas.at(vbias_i).amax_AFT.at(wf_i));
      //if(v_meas.at(vbias_i).V_bias>metadata_str.Vbd){
      //if(v_meas.at(vbias_i).amax_TRG.at(wf_i)>metadata_str.v_fit_CT_single_mean.at(vbias_i)*1.5){
      //cout<<setw(10)<<metadata_str.v_Vbias.at(vbias_i)<<setw(10)<<wf_i<<endl;
      //}
      //}
    }
  }
  //
  ///////////////////////
  //DCR calculations  
  ///////////////////////
  cout<<"DCR calculations"<<endl;
  TGraphErrors *gr_DCR_vs_vbias = new TGraphErrors();
  gr_DCR_vs_vbias->SetNameTitle("gr_DCR_vs_vbias","gr_DCR_vs_vbias");
  Double_t rate_DCR;
  Double_t threshold_DCR;
  for(vbias_i = 0; vbias_i<v_meas.size();vbias_i++){
    if(metadata_str.v_fit_DCR_single_status.at(vbias_i) == 1.0)
      threshold_DCR = metadata_str.v_fit_DCR_single_mean.at(vbias_i)/2.0;
    else
      threshold_DCR = metadata_str.v_halfpe_pre.at(vbias_i);
    rate_DCR = calculate_DCR(h1_amax_DCR[vbias_i],threshold_DCR,metadata_str.v_trigger_window_L.at(vbias_i));
    //
    if( metadata_str.v_Vbias.at(vbias_i)>=36.0){
      Int_t n_gr_point = gr_DCR_vs_vbias->GetN();
      gr_DCR_vs_vbias->SetPoint( n_gr_point, metadata_str.v_Vbias.at(vbias_i), rate_DCR);
      gr_DCR_vs_vbias->SetPointError(n_gr_point,metadata_str.v_Vbias.at(vbias_i)*0.0, rate_DCR*0.1);
    }
    //cout<<"metadata_str.v_Vbias.at(vbias_i) "<<metadata_str.v_Vbias.at(vbias_i)<<endl
    //	<<"rate_DCR                         "<<rate_DCR<<endl;
    //
  }
  ///////////////////////
  //
  ///////////////////////
  //CT calculations  
  ///////////////////////
  cout<<"CT calculations"<<endl;
  TGraphErrors *gr_CT_vs_vbias = new TGraphErrors();
  gr_CT_vs_vbias->SetNameTitle("gr_CT_vs_vbias","gr_CT_vs_vbias");
  Double_t p_CT;
  Double_t threshold_CT;
  for(vbias_i = 0; vbias_i<v_meas.size();vbias_i++){
    if(metadata_str.v_fit_CT_single_status.at(vbias_i) == 1.0)
      threshold_CT = metadata_str.v_fit_CT_single_mean.at(vbias_i)*1.5;
    else
      threshold_CT = metadata_str.v_singlepe_pre.at(vbias_i)*1.5;
    p_CT = calculate_p_CT(h1_amax_TRG[vbias_i],threshold_CT);
    //
    if( metadata_str.v_Vbias.at(vbias_i)>=35.0){
      Int_t n_gr_point = gr_CT_vs_vbias->GetN();
      gr_CT_vs_vbias->SetPoint( n_gr_point, metadata_str.v_Vbias.at(vbias_i), p_CT);
      gr_CT_vs_vbias->SetPointError(n_gr_point,metadata_str.v_Vbias.at(vbias_i)*0.0, p_CT*0.05+0.0002);
    }
    //cout<<"metadata_str.v_Vbias.at(vbias_i) "<<metadata_str.v_Vbias.at(vbias_i)<<endl
    //	<<"rate_DCR                         "<<rate_DCR<<endl;
    //
  }
  ///////////////////////
  //
  TCanvas *c1_DCR[nChannels];
  //
  ofstream file_with_singlepe_fit_DCR;
  file_with_singlepe_fit_DCR.open(fit_DCR_single.Data());
  //  
  for(vbias_i = 0; vbias_i<v_meas.size();vbias_i++){
    TString canvas_name_title = "c1_DCR_";
    canvas_name_title += vbias_i; 
    c1_DCR[vbias_i] = new TCanvas(canvas_name_title.Data(),canvas_name_title.Data(),800,600);
    c1_DCR[vbias_i]->SetRightMargin(0.03);
    c1_DCR[vbias_i]->SetLeftMargin(0.05);
    c1_DCR[vbias_i]->SetTopMargin(0.015);
    c1_DCR[vbias_i]->SetBottomMargin(0.08);
    c1_DCR[vbias_i]->SetGridx();
    c1_DCR[vbias_i]->SetGridy();
    c1_DCR[vbias_i]->SetLogy();
    //
    gStyle->SetPalette(1);
    gStyle->SetFrameBorderMode(0);
    gROOT->ForceStyle();
    gStyle->SetStatColor(kWhite);
    //gStyle->SetOptStat(kFALSE);
    //
    h1_amax_DCR[vbias_i]->SetLineColor(kBlack);
    h1_amax_DCR[vbias_i]->SetLineWidth(2.0);
    h1_amax_DCR[vbias_i]->SetMinimum(0.1);
    h1_amax_DCR[vbias_i]->SetMaximum(h1_amax_DCR[0]->GetMaximum());
    //
    //h1_amax_AFT[vbias_i]->SetLineColor(kGreen+2);
    //h1_amax_AFT[vbias_i]->SetLineWidth(2.0);
    //TString hist_
    //h1_amax_DCR[vbias_i]->SetTitle();
    h1_amax_DCR[vbias_i]->Draw();
    h1_amax_DCR[0]->SetLineColor(kBlue);
    h1_amax_DCR[0]->Draw("same");
    //h1_amax_AFT[vbias_i]->Draw("same");
    h1_amax_DCR[vbias_i]->GetXaxis()->SetTitle("Maximum amplitude, V");
    //
    Double_t l_x1;
    Double_t l_y1;
    Double_t l_x2;
    Double_t l_y2;
    //
    if(metadata_str.v_fit_DCR_single_status.at(vbias_i) == 1.0){
      l_x1 = metadata_str.v_fit_DCR_single_mean.at(vbias_i)/2.0;
      l_x2 = metadata_str.v_fit_DCR_single_mean.at(vbias_i)/2.0;
    }
    else{
      l_x1 = metadata_str.v_halfpe_pre.at(vbias_i);
      l_x2 = metadata_str.v_halfpe_pre.at(vbias_i);
    }
    l_y1 = 0.1;
    l_y2 = h1_amax_DCR[vbias_i]->GetMaximum();
    TLine *l1_05pe = new TLine(l_x1,l_y1,l_x2,l_y2);
    l1_05pe->SetLineWidth(2.0);
    l1_05pe->SetLineColor(kRed);
    l1_05pe->Draw("same");
    if(metadata_str.v_fit_DCR_single_status.at(vbias_i) == 1.0){
      l_x1 = metadata_str.v_fit_DCR_single_mean.at(vbias_i);
      l_x2 = metadata_str.v_fit_DCR_single_mean.at(vbias_i);
    }
    else{
      l_x1 = metadata_str.v_singlepe_pre.at(vbias_i);
      l_x2 = metadata_str.v_singlepe_pre.at(vbias_i);
    }
    l_y1 = 0.1;
    l_y2 = h1_amax_DCR[vbias_i]->GetMaximum();
    TLine *l1_1pe = new TLine(l_x1,l_y1,l_x2,l_y2);
    l1_1pe->SetLineWidth(2.0);
    l1_1pe->SetLineColor(kBlue);
    l1_1pe->Draw("same");
    //
    TLatex l01;
    l01.SetTextSize(0.02);
    TString date_meas = "Vbias: ";
    date_meas += metadata_str.v_Vbias.at(vbias_i);
    date_meas += " T: ";
    date_meas += metadata_str.v_temperature.at(vbias_i);
    l01.DrawLatex(0.0,h1_amax_DCR[vbias_i]->GetMaximum()*0.8,date_meas.Data());
    //
    if(vbias_i>3){
      h1_amax_DCR[vbias_i]->Fit("gaus","","R",0.85*metadata_str.v_singlepe_pre.at(vbias_i)-0.005,1.15*metadata_str.v_singlepe_pre.at(vbias_i)+0.005);
      TF1 *f_fit = h1_amax_DCR[vbias_i]->GetFunction("gaus");
      if(f_fit != NULL)
	file_with_singlepe_fit_DCR<<setw(15)<<metadata_str.v_Vbias.at(vbias_i)
				  <<setw(15)<<1.0
				  <<setw(15)<<f_fit->GetParameter(1)<<endl;
      else
	file_with_singlepe_fit_DCR<<setw(15)<<metadata_str.v_Vbias.at(vbias_i)
				  <<setw(15)<<0.0
				  <<setw(15)<<-999.0<<endl;
      //f_fit->GetParameter(1);//mean
    }
    else{
      file_with_singlepe_fit_DCR<<setw(15)<<metadata_str.v_Vbias.at(vbias_i)
				<<setw(15)<<0.0
				<<setw(15)<<-999.0<<endl;
    }
  }
  ///////////////////////
  //
  TCanvas *c1_CT[nChannels];
  //
  ofstream file_with_singlepe_fit_CT;
  file_with_singlepe_fit_CT.open(fit_CT_single.Data());
  //  
  for(vbias_i = 0; vbias_i<v_meas.size();vbias_i++){
    TString canvas_name_title = "c1_CT_";
    canvas_name_title += vbias_i; 
    c1_CT[vbias_i] = new TCanvas(canvas_name_title.Data(),canvas_name_title.Data(),800,600);
    c1_CT[vbias_i]->SetRightMargin(0.03);
    c1_CT[vbias_i]->SetLeftMargin(0.05);
    c1_CT[vbias_i]->SetTopMargin(0.015);
    c1_CT[vbias_i]->SetBottomMargin(0.08);
    c1_CT[vbias_i]->SetGridx();
    c1_CT[vbias_i]->SetGridy();
    c1_CT[vbias_i]->SetLogy();
    //
    gStyle->SetPalette(1);
    gStyle->SetFrameBorderMode(0);
    gROOT->ForceStyle();
    gStyle->SetStatColor(kWhite);
    //gStyle->SetOptStat(kFALSE);
    //
    h1_amax_TRG[vbias_i]->SetLineColor(kBlack);
    h1_amax_TRG[vbias_i]->SetLineWidth(2.0);
    h1_amax_TRG[vbias_i]->SetMinimum(0.1);
    h1_amax_TRG[vbias_i]->SetMaximum(h1_amax_TRG[0]->GetMaximum());
    //TString hist_
    //h1_amax_DCR[vbias_i]->SetTitle();
    h1_amax_TRG[vbias_i]->Draw();
    h1_amax_TRG[0]->SetLineColor(kBlue);
    h1_amax_TRG[0]->Draw("same");
    h1_amax_TRG[vbias_i]->GetXaxis()->SetTitle("Maximum amplitude, V");
    //
    Double_t l_x1;
    Double_t l_y1;
    Double_t l_x2;
    Double_t l_y2;
    //
    if(metadata_str.v_fit_CT_single_status.at(vbias_i) == 1.0){
      l_x1 = metadata_str.v_fit_CT_single_mean.at(vbias_i)*1.5;
      l_x2 = metadata_str.v_fit_CT_single_mean.at(vbias_i)*1.5;
    }
    else{
      l_x1 = metadata_str.v_singlepe_pre.at(vbias_i)*1.5;
      l_x2 = metadata_str.v_singlepe_pre.at(vbias_i)*1.5;
    }
    l_y1 = 0.1;
    l_y2 = h1_amax_TRG[vbias_i]->GetMaximum();
    TLine *l1_15pe = new TLine(l_x1,l_y1,l_x2,l_y2);
    l1_15pe->SetLineWidth(2.0);
    l1_15pe->SetLineColor(kRed);
    l1_15pe->Draw("same");
    if(metadata_str.v_fit_CT_single_status.at(vbias_i) == 1.0){
      l_x1 = metadata_str.v_fit_CT_single_mean.at(vbias_i);
      l_x2 = metadata_str.v_fit_CT_single_mean.at(vbias_i);
    }
    else{
      l_x1 = metadata_str.v_singlepe_pre.at(vbias_i);
      l_x2 = metadata_str.v_singlepe_pre.at(vbias_i);
    }
    l_y1 = 0.1;
    l_y2 = h1_amax_TRG[vbias_i]->GetMaximum();
    TLine *l1_1pe = new TLine(l_x1,l_y1,l_x2,l_y2);
    l1_1pe->SetLineWidth(2.0);
    l1_1pe->SetLineColor(kBlue);
    l1_1pe->Draw("same");
    //
    TLatex l01;
    l01.SetTextSize(0.02);
    TString date_meas = "Vbias: ";
    date_meas += metadata_str.v_Vbias.at(vbias_i);
    date_meas += " T: ";
    date_meas += metadata_str.v_temperature.at(vbias_i);
    l01.DrawLatex(0.0,h1_amax_TRG[vbias_i]->GetMaximum()*0.8,date_meas.Data());
    //
    if(vbias_i>0){
      h1_amax_TRG[vbias_i]->Fit("gaus","Q","R",0.85*metadata_str.v_singlepe_pre.at(vbias_i)-0.005,1.15*metadata_str.v_singlepe_pre.at(vbias_i)+0.005);
      TF1 *f_fit_CT = h1_amax_TRG[vbias_i]->GetFunction("gaus");
      if(f_fit_CT != NULL)
	file_with_singlepe_fit_CT<<setw(15)<<metadata_str.v_Vbias.at(vbias_i)
				 <<setw(15)<<1.0
				 <<setw(15)<<f_fit_CT->GetParameter(1)<<endl;
      else
	file_with_singlepe_fit_CT<<setw(15)<<metadata_str.v_Vbias.at(vbias_i)
				 <<setw(15)<<0.0
				 <<setw(15)<<-999.0<<endl;
      //f_fit->GetParameter(1);//mean
    }
    else{
      file_with_singlepe_fit_CT<<setw(15)<<metadata_str.v_Vbias.at(vbias_i)
			       <<setw(15)<<0.0
			       <<setw(15)<<-999.0<<endl;
    }
  }
  ///////////////////////
  cout<<"acm::analyze_DCR_and_CT ---> E "<<endl;
  //////////////////////  
  TFile* rootFile = new TFile(histOut.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histOut.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histOut.Data()<<endl;
  TString c1_DCR_pdf_tot = " ";
  TString c1_CT_pdf_tot = " ";
  for(vbias_i = 0; vbias_i<v_meas.size();vbias_i++){
    h1_amax[vbias_i]->Write();
    h1_amin[vbias_i]->Write();
    h1_t_amax[vbias_i]->Write();
    h1_t_amin[vbias_i]->Write();
    //
    h1_amax_DCR[vbias_i]->Write();
    h1_amax_TRG[vbias_i]->Write();
    h1_amax_AFT[vbias_i]->Write();
    //
    c1_DCR[vbias_i]->Write();
    //
    c1_CT[vbias_i]->Write();
    //
    //char buffer [5];
    //sprintf(buffer ,"%04d", vbias_i);
    //TString c1_DCR_ID = buffer;
    TString c1_DCR_pdf = dataDir;
    c1_DCR_pdf += "/";
    c1_DCR_pdf += "c1_DCR_";
    c1_DCR_pdf += vbias_i;
    c1_DCR_pdf += ".pdf";
    c1_DCR_pdf_tot += c1_DCR_pdf;
    c1_DCR_pdf_tot += " ";
    c1_DCR[vbias_i]->SaveAs(c1_DCR_pdf.Data());
    TString c1_CT_pdf = dataDir;
    c1_CT_pdf += "/";
    c1_CT_pdf += "c1_CT_";
    c1_CT_pdf += vbias_i;
    c1_CT_pdf += ".pdf";
    c1_CT_pdf_tot += c1_CT_pdf;
    c1_CT_pdf_tot += " ";
    c1_CT[vbias_i]->SaveAs(c1_CT_pdf.Data());
  }
  gr_DCR_vs_vbias->Write();
  gr_CT_vs_vbias->Write();
  //
  //gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=$root_data_dir/c1_DCR.pdf `ls -lrt $root_data_dir/c1_DCR_*pdf | awk '{print $9}' | xargs`
  ofstream merg_c1_DCR;
  merg_c1_DCR.open("merg_c1_DCR");
  cout<<"gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile="<<dataDir<<"/c1_DCR.pdf "<<c1_DCR_pdf_tot<<endl;
  merg_c1_DCR<<"gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile="<<dataDir<<"/c1_DCR.pdf "<<c1_DCR_pdf_tot<<endl;
  merg_c1_DCR.close();
  //  
  ofstream merg_c1_CT;
  merg_c1_CT.open("merg_c1_CT");
  cout<<"gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile="<<dataDir<<"/c1_CT.pdf "<<c1_CT_pdf_tot<<endl;
  merg_c1_CT<<"gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile="<<dataDir<<"/c1_CT.pdf "<<c1_CT_pdf_tot<<endl;
  merg_c1_CT.close();
  //  
  rootFile->Close();
  file_with_singlepe_fit_DCR.close();
  file_with_singlepe_fit_CT.close();
}

void acm::analyze(){
}

Double_t acm::calculate_DCR(TH1D *h1_amax, Double_t threshold, Double_t d_time_window_ns){
  Double_t n_dcr=h1_amax->Integral(0,h1_amax->FindBin(threshold)); 
  Double_t n_tot=h1_amax->GetEntries();
  //cout<<"n_dcr "<<n_dcr<<endl
  //  <<"n_tot "<<n_tot<<endl;
  if(n_tot>0.0 && n_dcr > 0.0)
    return -TMath::Log(n_dcr/n_tot)/(d_time_window_ns*1.0e-9);
  return 0.0;
}

Double_t acm::calculate_p_CT(TH1D *h1_amax, Double_t threshold){
  Double_t n_one = h1_amax->Integral(0,h1_amax->FindBin(threshold)); 
  Double_t n_tot = h1_amax->GetEntries();
  //cout<<"n_one "<<n_one<<endl
  //  <<"n_tot "<<n_tot<<endl;
  if(n_one>0.0 && n_tot > 0.0)
    return 1.0-n_one/n_tot;
  return 0.0;
}
