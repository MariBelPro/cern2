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
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TMath.h"
#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TObjArray.h"

void resolution()
{
TFile *profile =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUT.root", "READ"); 
  TProfile *res=(TProfile*)profile->Get("AveCosDeltaPsi1");
  Double_t ress[9];
   Double_t cent[9]={3,8,13,23,33,43,53,63,73};
   for(int i=0;i<9;i++)
   { 
       ress[i]= sqrt(2*res->GetBinContent(9-i));

   }
   TGraph *Resol=new TGraph(9,cent,ress);
   Resol->SetMarkerStyle(8);
   Resol->SetMarkerColor(kBlue);
   Resol->Draw("AP");
}


 