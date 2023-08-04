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
  TGraphErrors *Pol=(TGraphErrors*)profile1->Get("Graph;10");
 TFile *profile =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUT.root", "READ"); 
  TProfile *res=(TProfile*)profile->Get("AveCosDeltaPsi1");
  TH3D *Lambda= (TH3D*)profile->Get("Antylambdy_tot");
 TGraphErrors *gr[9];
 TProfile *for_l=new TProfile("for l","f",8,0,80);
 TProfile *help1=new TProfile ("1","for l",2,-0.5,1.5);
TProfile *help2=new TProfile ("2","for l",2,-0.5,1.5);
Double_t pol[8], epol[8];
  Double_t cent[8]={6,13,23,33,43,53,63,73};
     Double_t ecent[8]={0,0,0,0,0,0,0,0};
for(int c=0;c<7;c++)
{
    TH1D *hist=Lambda->ProjectionX(Form("x_%d",c),1,10,c+1,c+1,"e");
for_l->SetBinContent(8-c,hist->GetEntries()*Pol->GetY()[8-c]);
for_l->SetBinError(8-c,hist->GetEntries()*Pol->GetEY()[8-c]);
for_l->SetBinEntries(8-c,hist->GetEntries());
pol[7-c]=Pol->GetY()[8-c];
epol[7-c]=Pol->GetEY()[8-c];
 
}
// 0-5 
TH1D *hist0=Lambda->ProjectionX(Form("x_%d",0),1,10,1,1,"e");
help1->SetBinContent(1,hist0->GetEntries()*Pol->GetY()[0]);
help1->SetBinError(1,hist0->GetEntries()*Pol->GetEY()[0]);
help1->SetBinEntries(1,hist0->GetEntries());
//for 5-10
TH1D *hist1=Lambda->ProjectionX(Form("x_%d",0),1,10,2,2,"e");
help2->SetBinContent(1,hist1->GetEntries()*Pol->GetY()[1]);
help2->SetBinError(1,hist1->GetEntries()*Pol->GetEY()[1]);
help2->SetBinEntries(1,hist1->GetEntries());
help2->Add(help1,1);
for_l->SetBinContent(1,help2->GetBinEntries(1)*help2->GetBinContent(1));
for_l->SetBinError(1,help2->GetBinEntries(1)*help2->GetBinError(1));
for_l->SetBinEntries(1,help2->GetBinEntries(1));
pol[0]=help2->GetBinContent(1);
epol[0]=help2->GetBinError(1);
//Pol->Draw();
//for_l->Draw("same");
TGraphErrors *Polar=new TGraphErrors(8,cent,pol,ecent,epol);
Polar->Draw();
for_l->Draw("same");
profile1->cd();
Polar->Write();
for_l->Write();

profile1->Close();
   

  




  
}


 
 

