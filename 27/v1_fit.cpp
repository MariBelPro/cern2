/*

*/
// This is needed for calling standalone classes (not needed on RACF)
#ifndef _VANILLA_ROOT_
#define _VANILLA_ROOT_
#endif
// C++ headers
#include <iostream>
#include <string>
#include <vector>
// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TProfile.h"
#include "TMath.h"
#include "TProfile.h"
#include "TString.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TObjArray.h"
// FemtoDst headers
#include "../../StFemtoEvent/StFemtoDstReader.h"
#include "../../StFemtoEvent/StFemtoDst.h"
#include "../../StFemtoEvent/StFemtoEvent.h"
#include "../../StFemtoEvent/StFemtoTrack.h"
#include "../../StFemtoEvent/StFemtoKFP.h"
#include "../../StFemtoEvent/StFemtoV0.h"
#include "../../StFemtoEvent/StFemtoXi.h"
#include "../../StFemtoEvent/StFemtoEpdHit.h"
#include "../../StFemtoEvent/StFemtoHelix.h"
#include "../../StFemtoEvent/StFemtoPhysicalHelix.h"
// StEpdUtil headers
#include "../../StEpdUtil/StEpdEpFinder.h"
#include "../../StEpdUtil/StEpdEpInfo.h"
#include "../../StEpdUtil/StEpdGeom.h"



double fit(Double_t *x, Double_t* par)
{
    return par[0]*x[0]+par[1]*x[0]*x[0]*x[0];
}
void v1_fit()
{
    Double_t line[9];
    Double_t cub[9];
    Double_t c[9]={0,1,2,3,4,5,6,7,8};
     TFile *profile1 =TFile::Open("v1.root", "READ");
  
  TProfile *v1[9];
  TCanvas *c1 =new TCanvas("c","c",1000,500);
  TF1 *f=new TF1("f",fit,-6,6,2);
  c1->Divide(4,3);
  gStyle->SetOptStat(kFALSE);
for(int i=0;i<9; i++)
{
v1[i]=(TProfile*)profile1->Get(Form("for_centrality_bin_%d",i)); 
v1[i]->SetTitle(Form("%d",i));
v1[i]->GetYaxis()->SetTitle("v_{1}");
v1[i]->GetXaxis()->SetTitle("#eta");
c1->cd(i+1);
v1[i]->Fit(f);
line[i]=f->GetParameter(0);
cub[i]=f->GetParameter(1);
v1[i]->Draw();
} 







TGraph * kof1=new TGraph(9,c,cub);
//c1->cd(10);
//kof1->Draw("AP*");
TGraph * kof2=new TGraph(9,c,line);
kof2->SetTitle(";centrality_bin;line_kof");
//kof2->Draw("AP*");
TFile *oFile = new TFile("potok_fit.root", "RECREATE");
oFile->cd();
for(int i=0;i<9;i++)
{
 v1[i]->Write();
 cout<<", "<<cub[i];
}
kof1->Write();
kof2->Write();
oFile->Close();
}
// Double_t line[9] ={-0.00113784-0.00191896-0.00313357-0.0040704-0.0045704-0.00409848-0.00304222-0.00157762-0.00044567};
  //  Double_t cub[9];