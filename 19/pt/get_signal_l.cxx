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
  TFile *profile1=new TFile("furie_l.root","READ");
 TGraphErrors *Polar=(TGraphErrors*)profile1->Get("Graph;6");
 TProfile *pol=(TProfile*)profile1->Get("for l");
//L->Draw();
  TFile *profile2=new TFile("furie_antyl.root","READ");
  TGraphErrors *Polar_a=(TGraphErrors*)profile2->Get("Graph;6");
  TProfile *pol_a=(TProfile*)profile2->Get("for l");
 pol->Add(pol_a,1);
 pol->SetMarkerStyle(22);
 pol->SetMarkerSize(3);
 pol->SetLineWidth(3);
 pol->SetMarkerColor(4);
   pol->Draw();

/////////////////////////////////////////////////////////




 
 Polar->SetMarkerStyle(22);
 Polar->SetMarkerSize(3);
 Polar->SetLineWidth(3);
 Polar->SetMarkerColor(4);
 Polar->SetTitle("Analysis for #Lambda  (EPD)  ;centrality (%); Polarization (%)");
 //Polar->Draw("AP");

 Polar_a->SetMarkerStyle(22);
 Polar_a->SetMarkerColor(1);
 Polar_a->SetMarkerSize(3);
  Polar_a->SetLineWidth(3);
 Polar_a->SetTitle("Analysis for #bar{#Lambda} (EPD);centrality (%); Polarization (%)");
//Polar_a->Draw("P");

  
 
 
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