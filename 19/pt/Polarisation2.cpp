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



void Polarisation2 ()
{
  //////////////////////////////////////////////////
  TFile *profile1=TFile::Open("furie_antyl.root", "UPDATE");
  TGraphErrors *Pol=(TGraphErrors*)profile1->Get("Graph;6");
 TFile *profile =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUTpt.root", "READ"); 
  TProfile *res=(TProfile*)profile->Get("AveCosDeltaPsi1");
  TH3D *Lambda= (TH3D*)profile->Get("Antylambdy_tot");
 TGraphErrors *gr[9];
 TProfile *for_l=new TProfile("for l","f",5,0.15,2);


for(int c=0;c<5;c++)
{
    TH1D *hist=Lambda->ProjectionX(Form("x_%d",c),1,10,c+1,c+1,"e");
for_l->SetBinContent(c+1,hist->GetEntries()*Pol->GetY()[c]);
for_l->SetBinError(c+1,hist->GetEntries()*Pol->GetEY()[c]);
for_l->SetBinEntries(c+1,hist->GetEntries());

 
}


profile1->cd();

for_l->Write();

profile1->Close();
   

  




  
}


 
 

