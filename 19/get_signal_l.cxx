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
   
TFile *profile00=new TFile("/home/mary/Desktop/NIRS/EPD/StEpd/19point6/ksi_l/ksi_l_pol.root","READ");
TGraphErrors *ksi_l=(TGraphErrors*)profile00->Get("Graph;1");
 ksi_l->SetMarkerStyle(23);
 ksi_l->SetMarkerColor(kRed);
 ksi_l->SetMarkerSize(3);
 ksi_l->SetTitle("Analysis for #Lambda (#Xi^{-} -> #Lambda) and #bar{#Lambda} (#bar{#Xi}^{+} -> #bar{#Lambda});centrality (%); Polarization (%)");
     
   TCanvas *c2=new TCanvas("c2","c2",1000,1000);
  TFile *profile1=new TFile("furie_l.root","READ");
 TProfile *L  =(TProfile*)profile1->Get("for l");
//L->Draw();
  TFile *profile2=new TFile("furie_antyl.root","READ");
  TProfile *Anty_L  =(TProfile*)profile2->Get("for l");

L->Add(Anty_L,1);
  Double_t pol[8], epol[8] ,pol_a[8], epol_a[8];
  Double_t cent[8]={6,13,23,33,43,53,63,73};
  Double_t cent2[8]={6.5,13.5,23.5,33.5,43.5,53.5,63.5,73.5};
     Double_t ecent[8]={0,0,0,0,0,0,0,0};
     for(int i=0;i<8;i++)
     {
       pol[i]=L->GetBinContent(i+1);
       epol[i]=L->GetBinError(i+1);
       pol_a[i]=Anty_L->GetBinContent(i+1);
       epol_a[i]=Anty_L->GetBinError(i+1);
     }
  TGraphErrors *Polar=new TGraphErrors(8,cent,pol,ecent,epol);
  TGraphErrors *Polar_a=new TGraphErrors(8,cent2,pol_a,ecent,epol_a);



/////////////////////////////////////////////////////////




 
 Polar->SetMarkerStyle(22);
 Polar->SetMarkerSize(3);
 Polar->SetLineWidth(3);
 Polar->SetMarkerColor(4);
 Polar->SetTitle("Analysis for #Lambda and #bar{#Lambda} (EPD)  ;centrality (%); Polarization (%)");
 Polar->Draw("AP");
  ksi_l->SetLineWidth(3);

ksi_l->Draw("P");
 Polar_a->SetMarkerStyle(22);
 Polar_a->SetMarkerColor(1);
 Polar_a->SetMarkerSize(3);
  Polar_a->SetLineWidth(3);
 Polar_a->SetTitle("Analysis for #bar{#Lambda} (EPD);centrality (%); Polarization (%)");
//Polar_a->Draw("P");

  
 Double_t Pol0[8]={0.3, 0.5,0.51,0.6,0.8,1,1.65,1.55};
 Double_t ePol0[8]={0.1,0.1,0.12,0.1,0.17,0.18,0.2,0.3};
 Double_t cent0[8]={6.5,13.5,23.5,33.5,43.5,53.5,63.5,73.5}; //5,10,20,30,40,50,60,70,80
 Double_t ecent0[8]={0,0,0,0,0,0,0,0};
  TGraphErrors *Polar0=new TGraphErrors (8,cent0,Pol0,ecent0,ePol0);
  Polar0->SetMarkerStyle(22);
  Polar0->SetMarkerColor(1);
  Polar0->SetTitle("[16]");

 //Polar0->Draw("P");
 
 c2->BuildLegend();
  // TLatex latex1(-0.6,4,"#font[12]{20-30% centrality}");
//latex1.SetTextSize(0.04);
 //   latex1.DrawClone("Same"); 
TLatex latex2(6,3.5,"#font[12]{Au+Au #sqrt{s_{NN}}=19.6 GeV}");
latex2.SetTextSize(0.04);
latex2.DrawClone("Same");
   //Polar->Write();
   TF1 *f=new TF1("f","0",0,80);
f->SetLineStyle(2);
f->SetLineColor(1);
f->Draw("same");


    }