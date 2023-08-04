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

TFile *profile1 =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUT.root", "READ");
 TProfile *L  =(TProfile*)profile1->Get("acceptKsi");

  TProfile *Anty_L  =(TProfile*)profile1->Get("accept_barKsi");


  Double_t pol[8], pol_a[8];
  Double_t cent[8]={6,13,23,33,43,53,63,73};
  
     for(int i=0;i<9;i++)
     {
        pol[8-i]=1.273*L->GetBinContent(i+1);
        pol_a[8-i]=1.273*Anty_L->GetBinContent(i+1);
      
     }     
      TGraph *Polar=new TGraph(8,cent,pol);
  TGraph *Polar_a=new TGraph(8,cent,pol_a);
     Polar->SetMarkerStyle(8);
     Polar->SetMarkerSize(3);
 Polar->SetMarkerColor(kRed);
 Polar->SetTitle("Analysis for #Xi^{-} (EPD);centrality (%); A_{0}");
  TCanvas *c2=new TCanvas("c2","c2",1000,1000);
 Polar->Draw("AP");

 Polar_a->SetMarkerStyle(22);
 Polar_a->SetMarkerColor(1);
   Polar_a->SetMarkerSize(3);
 Polar_a->SetTitle("Analysis for #bar{#Xi^{+}} (EPD);centrality (%); A_{0}");
Polar_a->Draw("P"); 
c2->BuildLegend();

}