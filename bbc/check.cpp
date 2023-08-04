/**
 * \brief Example of how to read a file (list of files) using StFemtoEvent classes
 *
 * RunFemtoDstAnalyzer.C is an example of reading STAR FemtoDst format.
 * One can use either FemtoDst file or a list of femtoDst files (inFile.lis or
 * inFile.list) as an input, and preform physics analysis
 *
 * \author Grigory Nigmatkulov
 * \date May 29, 2018
 */

// This is needed for calling standalone classes (not needed on RACF)
#ifndef _VANILLA_ROOT_
#define _VANILLA_ROOT_
#endif

// C++ headers
#include <iostream>
#include <string> 

// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TMath.h"
#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include <vector>
void check()
{
   TCanvas *c1=new TCanvas("c","c",500,700);
  gStyle->SetOptStat(kFALSE);
  TFile *V1=new TFile("/home/mary/Desktop/NIRS/EPD/StEpd/bbc/v1_tpc.root","READ");
  TFile *V2=new TFile("v2.root","READ");
 TFile *V3=new TFile("v3.root","READ");
 TFile *f1=new TFile("/home/mary/Desktop/NIRS/EPD/StEpd/19point6/StEpdEpFinderCorrectionHistograms_INPUT19.6.root");
 TProfile *ref1= (TProfile*)f1->Get("cos");
 
 TFile *f2=new TFile("Q_recentered2.root","READ");
 TProfile *ref2= (TProfile*)f2->Get("res");
  TFile *f3=new TFile("Q_recentered3.root","READ");
  TProfile *ref3= (TProfile*)f3->Get("res");


  TProfile * v1=(TProfile*)V1->Get("for_centrality_bin_6")  ;
   v1->SetMarkerStyle(23);
   v1->SetMarkerSize(2);
   v1->SetMarkerColor(kBlue);
   v1->SetTitle("v1; #eta");
TProfile * v2=(TProfile*)V2->Get("for_centrality_bin_6")  ;
   v2->SetMarkerStyle(23);
   v2->SetMarkerSize(2);
   v2->SetTitle("v2");
   v2->SetMarkerColor(1);
 
TProfile * v3=(TProfile*)V3->Get("for_centrality_bin_6")  ;
   v3->SetMarkerStyle(23);
   v3->SetMarkerSize(2);
   v3->SetMarkerColor(kRed);
   v3->SetTitle("v3"); 
  Double_t resol1[9];
  Double_t resol2[9];
  Double_t resol3[9];
  Double_t v_1[12];
  Double_t v_2[12];
  Double_t v_3[12];
  Double_t ev_1[12];
  Double_t ev_2[12];
  Double_t ev_3[12];
  Double_t cent[12]={-1,-0.9,-0.7,-0.5,-0.3,-0.1,0.1,0.3,0.5,0.7,0.9,1};
   Double_t ecent[12]={0,0,0,0,0,0,0,0,0,0,0,0};
       
       //c1->Divide(5,2);
      for(int i=0;i<9; i++)
        {
        
        resol1[i]=sqrt(ref1->GetBinContent(9-i));
        resol2[i]=sqrt(ref2->GetBinContent(9-i));
        resol3[i]=sqrt(ref3->GetBinContent(9-i));
        
   }
   for(int i=0;i<12;i++)
   {
         v_1[i]=v1->GetBinContent(i+1)/sqrt(ref1->GetBinContent(6));
         v_2[i]=v2->GetBinContent(i+1)/sqrt(ref2->GetBinContent(6));
         v_3[i]=v3->GetBinContent(i+1)/sqrt(ref3->GetBinContent(6));
         
         ev_1[i]=v1->GetBinError(i+1)/sqrt(ref1->GetBinContent(6));
         ev_2[i]=v2->GetBinError(i+1)/sqrt(ref2->GetBinContent(6));
         ev_3[i]=v3->GetBinError(i+1)/sqrt(ref3->GetBinContent(6));
   }
cout<<sqrt(ref2->GetBinContent(6))<<" "<<v2->GetBinContent(6)<<endl;
      TGraphErrors *flow1=new TGraphErrors(12,cent,v_1,ecent,ev_1);
      TGraphErrors *flow2=new TGraphErrors(12,cent,v_2,ecent,ev_2);
      TGraphErrors *flow3=new TGraphErrors(12,cent,v_3,ecent,ev_3);
   flow1->SetMarkerStyle(23);
   flow1->SetMarkerSize(3);
   flow1->SetTitle("v1");
   flow1->SetMarkerColor(kRed);
  flow1->Draw("AP");   
   flow2->SetMarkerStyle(23);
   flow2->SetMarkerSize(3);
   flow2->SetTitle("v2");
   flow2->SetMarkerColor(1);
  flow2->Draw("P");
   flow3->SetMarkerStyle(22);
   flow3->SetMarkerSize(3);
   flow3->SetTitle("v3");
   flow3->SetMarkerColor(kBlue);
 flow3->Draw("P");
  //flow_1->Draw("same");




c1->BuildLegend();






}