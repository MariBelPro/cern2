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


 

 void get_signal_l()
 {
     
   TCanvas *c2=new TCanvas("c2","c2",1000,1000);
  TFile *f_l_out=new TFile("furie_l.root","READ");
 *L  =(TProfile*)f_l_out->Get("for l");
//L->Draw();
  TFile *f_antyl_out=new TFile("furie_antyl.root","READ");
  TProfile *Anty_L  =(TProfile*)f_antyl_out->Get("for antyl");

 //L->Add(Anty_L,1);
  Double_t pol[8], epol[8];
  Double_t cent[8]={-0.75,-0.35,0.15,0.55,0.95};
   Double_t cent2[8]={-0.7,-0.3,0.1,0.5,0.9};
     Double_t ecent[8]={0,0,0,0,0,0,0,0};
     for(int i=0;i<5;i++)
     {
       pol[i]=L->GetBinContent(i+1);
       epol[i]=L->GetBinError(i+1);
     }
  TGraphErrors *Polar=new TGraphErrors(8,cent,pol,ecent,epol);
   for(int i=0;i<5;i++)
     {
       pol[i]=Anty_L->GetBinContent(i+1);
       epol[i]=Anty_L->GetBinError(i+1);
     }
  TGraphErrors *antyPolar=new TGraphErrors(8,cent2,pol,ecent,epol);



/////////////////////////////////////////////////////////




 
 Polar->SetMarkerStyle(22);
 Polar->SetMarkerColor(kBlue);
 Polar->SetTitle("Analysis for #Lambda (EPD);centrality (%); Polarization (%)");
 Polar->Draw("AP");
  f_l_out->cd();
Polar->Write();   
f_l_out->Close();
 antyPolar->SetMarkerStyle(23);
 antyPolar->SetMarkerColor(kRed);
  antyPolar->SetTitle("Analysis for #bar{#Lambda} (EPD);centrality (%); Polarization (%)");
 antyPolar->Draw("P");
 f_antyl_out->cd();
antyPolar->Write();   
 f_antyl_out->Close();
  
 
 
 c2->BuildLegend();
   // 


    }