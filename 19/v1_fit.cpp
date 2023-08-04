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



double fit(Double_t *x, Double_t* par)
{
    return par[0]*x[0]+par[1]*x[0]*x[0]*x[0];
}
void v1_fit()
{
    Double_t line[9];
    Double_t cub[9];
    Double_t c[9]={0,1,2,3,4,5,6,7,8};
     TFile *profile1 =TFile::Open("/home/mary/Desktop/NIRS/EPD/StEpd/bbc/v1_tpc.root", "READ");
  
  TProfile *v1[9];
  TCanvas *c1 =new TCanvas("c","c",1000,500);
  TF1 *f=new TF1("f",fit,-6,6,2);
  c1->Divide(5,2);
  gStyle->SetOptStat(kFALSE);
  for(int i=0;i<9; i++)
{
  c1->cd(i+1);
v1[i]=(TProfile*)profile1->Get(Form("for_centrality_bin_%d",i));
//v1[i]->Fit(f);
//line[i]=f->GetParameter(0);
//cub[i]=f->GetParameter(1);
v1[i]->Draw();
} 
 




for(int i=0;i<9;i++)
{

 //cout<<", "<<cub[i];
}




}
