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


        

void acceptance ()
{
/////////////////////////////////////////////////////////////////////////////////////////

TFile *profile1 =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUTksi_l.root", "READ");
 TProfile *L  =(TProfile*)profile1->Get("acceptKsi_l");

  TProfile *Anty_L  =(TProfile*)profile1->Get("accept_barKsi_l");


  Double_t pol[8], epol[8] ,pol_a[8], epol_a[8];
  Double_t cent[8]={6,13,23,33,43,53,63,73};
  Double_t cent2[8]={6.5,13.5,23.5,33.5,43.5,53.5,63.5,73.5};
     Double_t ecent[8]={0,0,0,0,0,0,0,0};
     for(int i=0;i<9;i++)
     {
        pol[8-i]=1.273*L->GetBinContent(i+1);
       epol[8-i]=1.273*L->GetBinError(i+1);
        pol_a[8-i]=1.273*Anty_L->GetBinContent(i+1);
       epol_a[8-i]=1.273*Anty_L->GetBinError(i+1);
     }     
      TGraph *Polar=new TGraph(8,cent,pol);
  TGraph *Polar_a=new TGraph(8,cent2,pol_a);
     Polar->SetMarkerStyle(8);
     Polar->SetMarkerSize(3);
 Polar->SetMarkerColor(kRed);
 Polar->SetTitle("Analysis for #Lambda (#Xi^{-}) (EPD);centrality (%); A_{0}");
  TCanvas *c2=new TCanvas("c2","c2",1000,1000);
 Polar->Draw("AP");

 Polar_a->SetMarkerStyle(22);
 Polar_a->SetMarkerColor(1);
   Polar_a->SetMarkerSize(3);
 Polar_a->SetTitle("Analysis for #bar{#Lambda} (#bar{#Xi^{+}}) (EPD);centrality (%); A_{0}");
Polar_a->Draw("P"); 
c2->BuildLegend();

}