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
    TCanvas *c1=new TCanvas("c","c",1000,1000);
TFile *profile =TFile::Open("bbc_resol.root", "READ"); 
  TProfile *res=(TProfile*)profile->Get("res");
  Double_t ress[9];
   Double_t cent[9]={3,8,13,23,33,43,53,63,73};
   for(int i=0;i<9;i++)
   { 
       ress[i]= sqrt(-2*res->GetBinContent(9-i));

   }
   TGraph *Resol=new TGraph(9,cent,ress);
   Resol->SetMarkerStyle(8);
   Resol->SetMarkerColor(kBlue);


TFile *profile1 =TFile::Open("../Macros/StEpdEpFinderCorrectionHistograms_INPUT.root", "READ"); 
  TProfile *res2=(TProfile*)profile1->Get("AveCosDeltaPsi1");
  Double_t ress2[9];
   Double_t cent2[9]={3,8,13,23,33,43,53,63,73};
   for(int i=0;i<9;i++)
   { 
       ress2[i]= sqrt(2*res2->GetBinContent(9-i));

   }
   TGraph *Resol2=new TGraph(9,cent2,ress2);
   Resol2->SetMarkerStyle(22);
   Resol2->SetMarkerColor(kRed);
   Resol->SetTitle("for BBC;Centrality (%); Res_{#psi_{1}}");
   Resol2->SetTitle("for EPD;Centrality (%); Res_{#psi_{1}}");
   Resol2->Draw("AP");
   Resol->Draw("P");
   c1->BuildLegend();
}


 