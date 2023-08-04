#ifndef _VANILLA_ROOT_
#define _VANILLA_ROOT_
#endif
#include <iostream>
#include <string>
#include <vector>
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH3D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TMath.h"
#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TObjArray.h"
#include "TGraphErrors.h"
R__LOAD_LIBRARY(furie_for_EPD_cxx.so)
//R__LOAD_LIBRARY(furie_for_EPDanty_cxx.so)
R__LOAD_LIBRARY(Polarisation2_cxx.so)
R__LOAD_LIBRARY(Polarisationanty_cxx.so)
//R__LOAD_LIBRARY(get_signal_sum_cxx.so)
R__LOAD_LIBRARY(get_signal_l_cxx.so)
//R__LOAD_LIBRARY(get_signal_antyl_cxx.so)

void all()
{
    gSystem->Load("furie_for_EPD_cxx.so");
     //gSystem->Load("furie_for_EPDanty_cxx.so");
     gSystem->Load("Polarisation2_cxx.so");
      gSystem->Load("Polarisationanty_cxx.so");
     // gSystem->Load("get_signal_sum_cxx.so");
       gSystem->Load("get_signal_l_cxx.so");
       //gSystem->Load("get_signal_antyl_cxx.so");
      furie_for_EPD();
      Polarisation2();
      Polarisationanty();
     // get_signal_sum();
     get_signal_l();
      
}