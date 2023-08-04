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
   // c1->Divide(2,1);
TFile *profile =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUT19.6.root", "READ");

TFile *profile2 =TFile::Open("../27gev/StEpdEpFinderCorrectionHistograms_INPUTnew.root", "READ");
TProfile *ref_mult= (TProfile*)profile->Get("hEventProfile_0");
TProfile *adc= (TProfile*)profile->Get("hHitProfile_0");
TH1D *row=(TH1D*)profile->Get("phiR");
row->SetLineColor(1);
row->SetLineWidth(3);
row->SetTitle("raw;#psi (radian)");
TH1D *weigth=(TH1D*)profile->Get("phiW");
weigth->SetLineColor(2);
weigth->SetLineWidth(3);
weigth->SetTitle("phi-weighting");
TH1D *flat=(TH1D*)profile->Get("phiTot");
flat->SetLineColor(4);
flat->SetLineWidth(3);
flat->SetTitle("flattening");
row->Draw();
weigth->Draw("same");
flat->Draw("same");
c1->BuildLegend();
/*
  TProfile *res19=(TProfile*)profile->Get("AveCosDeltaPsi1");
  Double_t ress19[9];
   Double_t cent19[9]={3,8,13,23,33,43,53,63,73};
   for(int i=0;i<9;i++)
   { 
       ress19[i]= sqrt(2*res19->GetBinContent(9-i));

   }
   TGraph *Resol19=new TGraph(9,cent19,ress19);
   Resol19->SetMarkerStyle(8);
   Resol19->SetMarkerColor(kBlue);
   Resol19->SetMarkerSize(2);
    Resol19->SetTitle("for 19.6 Gev;centrality(%);Res_{#psi_{1}}");
   Resol19->Draw("AP");

TProfile *res27=(TProfile*)profile2->Get("AveCosDeltaPsi1");
  Double_t ress27[9];
   Double_t cent27[9]={3,8,13,23,33,43,53,63,73};
   for(int i=0;i<9;i++)
   { 
       ress27[i]= sqrt(res27->GetBinContent(9-i));

   }
   TGraph *Resol27=new TGraph(9,cent27,ress27);
   Resol27->SetMarkerStyle(23);
   Resol27->SetMarkerSize(2);
   Resol27->SetMarkerColor(kRed);
   Resol27->SetTitle("for 27 Gev");
   Resol27->Draw("P");
   c1->BuildLegend();
   
   c1->cd(1);
   ref_mult->SetMarkerStyle(8);
   ref_mult->SetMarkerSize(2);
   ref_mult->SetMarkerColor(1);
   ref_mult->SetLineWidth(2);
   ref_mult->Draw();
   TF1 * mid= new TF1 ("mid","108",20060000, 20070000);
   mid->SetLineStyle(2);
   mid->SetLineWidth(4);
   mid->SetLineColor(kBlue);
 mid->Draw("same");
   TF1 *hight= new TF1 ("mid","108+0.93*3",20060000, 20070000);
   hight->SetLineStyle(2);
   hight->SetLineWidth(4);
   hight->SetLineColor(kBlue);
    hight->Draw("same");
   TF1 *low= new TF1 ("mid","108-0.93*3",20060000, 20070000);
   low->SetLineStyle(2);
   low->SetLineWidth(4);
   low->SetLineColor(kBlue);
   low->Draw("same");



   c1->cd(2);
   adc->SetMarkerStyle(8);
   adc->SetMarkerSize(2);
   adc->SetMarkerColor(1);
   adc->SetLineWidth(2);
   adc->Draw();
   TF1 * mid2= new TF1 ("mid","53240",20060000, 20070000);
   mid2->SetLineStyle(2);
   mid2->SetLineWidth(4);
   mid2->SetLineColor(kBlue);
 mid2->Draw("same");
   TF1 *hight2= new TF1 ("mid","53240+270*3",20060000, 20070000);
   hight2->SetLineStyle(2);
   hight2->SetLineWidth(4);
   hight2->SetLineColor(kBlue);
    hight2->Draw("same");
   TF1 *low2= new TF1 ("mid","53240-270*3",20060000, 20070000);
   low2->SetLineStyle(2);
   low2->SetLineWidth(4);
   low2->SetLineColor(kBlue);
   low2->Draw("same");
  */
}


 