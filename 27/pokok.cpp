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
// lambda :253820 754514 1.77407e+06 3.57975e+06 6.73398e+06 1.08722e+07 1.70522e+07 1.05701e+07 1.34845e+07
//anty_lambda:93687 266737 594332 1.13302e+06 2.01502e+06 3.08439e+06 4.62211e+06 2.78264e+06 3.49516e+06
double furie_fit(Double_t *x, Double_t* par)
{
    return par[0]*(1+2*par[1]*sin(x[0])+2*par[2]*cos(x[0])+2*par[3]*sin(2*x[0])+2*par[4]*cos(2*x[0]));
}

void pokok ()
{
  //////////////////////////////////////////////////
 TFile *profile =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUT.root", "READ");
  TH3D *Lambda= (TH3D*)profile->Get("lambdy_tot");
  TH3D *AntyLambda= (TH3D*)profile->Get("Antylambdy_tot");
  Double_t lambda[9];
  Double_t anty_lambda[9];
 for(int i=0;i<9;i++)
 {
   TH1D *hist0=Lambda->ProjectionX(Form("x_%d",0),1,10,i+1,i+1,"e");
   lambda[i]=hist0->GetEntries();
   TH1D *hist1=AntyLambda->ProjectionX(Form("Ax_%d",0),1,10,i+1,i+1,"e");
   anty_lambda[i]=hist1->GetEntries();

 }
  
for(int u=0;u<9;u++)
{
  cout<<anty_lambda[u]<<" ";
}



  
}


 
 

