//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <time.h>

using namespace std;

Int_t plot_wf_LECROY(){
  //  
  TCanvas *c1 = new TCanvas("c1","c1",10,10,800,800);
  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  //gStyle->SetOptStat(kFALSE);
  //
  c1->SetRightMargin(0.03);
  c1->SetLeftMargin(0.12);
  c1->SetTopMargin(0.03);
  c1->SetBottomMargin(0.1);
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  ////////////////////////////////////
  TFile *f1_dose = new TFile("../data/03.03.2023_01/42.000_V_T_25.099_C.txt.root_gr.root");
  TFile *f1_dose_30 = new TFile("../data/03.03.2023_01/30.000_V_T_25.096_C.txt.root_gr.root");
  TFile *f1 = new TFile("../data/26.02.2023_01/42.000_V_T_23.679_C.txt.root_gr.root");
  //
  TGraph *gr_dose = (TGraph*)f1_dose->Get("gr_9963");
  TGraph *gr_dose_30 = (TGraph*)f1_dose_30->Get("gr_9963");
  TGraph *gr = (TGraph*)f1->Get("gr_9963");
  //
  gr_dose->SetMarkerStyle(20);
  gr_dose->SetLineColor(kBlack);
  gr_dose->SetLineWidth(3.0);
  gr_dose->SetMarkerColor(kBlack);
  gr_dose->GetXaxis()->SetTitle("time, ns");
  gr_dose->GetYaxis()->SetTitle("amplitude, V");
  //
  gr_dose_30->SetMarkerStyle(20);
  gr_dose_30->SetLineColor(kBlue);
  gr_dose_30->SetLineWidth(3.0);
  gr_dose_30->SetMarkerColor(kBlue);
  gr_dose_30->GetXaxis()->SetTitle("time, ns");
  gr_dose_30->GetYaxis()->SetTitle("amplitude, V");
  //
  gr->SetMarkerStyle(20);
  gr->SetLineColor(kRed);
  gr->SetLineWidth(3.0);
  gr->SetMarkerColor(kRed);
  gr->GetXaxis()->SetTitle("time, ns");
  gr->GetYaxis()->SetTitle("amplitude, V");
  //
  ////////////////////////////////////
  //
  gPad->SetGridx();
  gPad->SetGridy();
  //
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(gr_dose);
  mg->Add(gr_dose_30);
  mg->Add(gr);
  //
  mg->GetXaxis()->SetRangeUser(1550,1650);
  mg->Draw("APL");
  mg->SetMinimum(-0.10);
  mg->SetMaximum( 0.15);
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("amplitude, V");
  //
  TLegend *leg = new TLegend(0.6,0.6,0.9,0.9,"","brNDC");
  leg->AddEntry(gr_dose,    "30 Gy (@ 42 V, 25.1 ^{o}C)", "pl");
  leg->AddEntry(gr_dose_30, "30 Gy (@ 30 V, 25.1 ^{o}C)", "pl");
  leg->AddEntry(gr,         " 0 Gy (@ 42 V, 23.7 ^{o}C)", "pl");
  leg->Draw();
  //
  return 0;
}
