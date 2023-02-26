//my
#include "src/acm.hh"

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]){
  if(argc == 11 && atoi(argv[1])==0){
    TString dataDir = argv[2];
    TString rootFilesList = argv[3];
    TString info_file = argv[4];
    TString measCond_file = argv[5];
    TString temperature_file = argv[6];
    TString trigger_window_file = argv[7];
    TString outRootFileF = argv[8];
    TString fit_DCR_single = argv[9];
    TString fit_CT_single = argv[10];
    cout<<"--> Parameter calculation from the WF <--"<<endl
	<<"dataDir             : "<<dataDir<<endl
	<<"rootFilesList       : "<<rootFilesList<<endl
      	<<"info_file           : "<<info_file<<endl
	<<"measCond_file       : "<<measCond_file<<endl
      	<<"temperature_file    : "<<temperature_file<<endl
	<<"trigger_window_file : "<<trigger_window_file<<endl
	<<"outRootFileF        : "<<outRootFileF<<endl
	<<"fit_DCR_single      : "<<fit_DCR_single<<endl
	<<"fit_CT_single       : "<<fit_CT_single<<endl;
    acm a( rootFilesList, info_file, measCond_file, temperature_file, trigger_window_file, outRootFileF, fit_DCR_single, fit_CT_single, dataDir);
  }
  else if(argc == 4 && atoi(argv[1])==1){
    TString inRootFiles = argv[2];
    TString outRootFileF = argv[3];
    cout<<"--> Parameter calculation from the WF <--"<<endl
	<<"inRootFiles  : "<<inRootFiles<<endl
	<<"outRootFileF : "<<outRootFileF<<endl;
    acm a( inRootFiles, atoi(argv[1]));
    a.Loop(outRootFileF);
  }
  else if(argc == 4 && atoi(argv[1])==2){
    // if else
  }
  else{
    cout<<" --> ERROR in input arguments "<<endl
	<<" runID [1] = 0 (execution ID number)"<<endl
	<<"       [2] - dataDir"<<endl
      	<<"       [3] - file with list of the root files"<<endl
      	<<"       [4] - info. file"<<endl
	<<"       [5] - meas. cond. file"<<endl
      	<<"       [6] - temperature file"<<endl
      	<<"       [7] - trigger window file"<<endl      
	<<"       [8] - name of root file with histograms"<<endl
      	<<"       [9] - fit_DCR_single"<<endl
	<<"       [10] - fit_CT_single"<<endl;
    cout<<" runID [1] = 1 (execution ID number)"<<endl
      	<<"       [2] - in root files"<<endl
	<<"       [3] - name of root file with histograms"<<endl;
  }
  return 0;
}
