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


double fit(Double_t *x, Double_t* par)
{
    return par[0]+par[1]*x[0]+par[2]*TMath::Gaus(x[0],par[3],par[4])+par[5]*TMath::Gaus(x[0],par[3],par[6]);
}
double b_g(Double_t *x, Double_t* par)
{
    return par[0]+par[1]*x[0];
}
double signal (Double_t *x, Double_t* par)
{
    par[0]*TMath::Gaus(x[0],par[1],par[2]);
}
void delete_bg( TH1D *hist_in )
{
   
            TF1 *full=new TF1 ("full",fit,1.105,1.125,7);
            full->SetParameter(0,2);
            full->SetParameter(1,0);
            full->SetParameter(2,40);
            full->SetParameter(3,1.1156);
            full->SetParameter(4,0.002);
            full->SetParameter(5,80);
            full->SetParameter(6,0.001);
            
                 full->SetParLimits(4,0,100);
                 full->SetParLimits(2,0,100);
                  full->SetParLimits(1,0,10000000);
                  full->SetParLimits(5,0,10000000);
                 full->SetParLimits(0,0,10000000);
                  
            full->SetParLimits(6,0,100);
    hist_in->Fit(full,"RME");

    TF1 *back_g =new TF1("bg",b_g,1.105,1.125,2);
    back_g->SetParameter(0,full->GetParameter(0));
    back_g->SetParameter(1,full->GetParameter(1)); 
    back_g->SetLineColor(kBlue);
     back_g->SetLineStyle(9);
    back_g->Draw("same");
    /*
    TF1 *sig =new TF1("sig",signal,1.105,1.125,3);
    sig->SetParameter(0,full->GetParameter(2));
    sig->SetParameter(1,full->GetParameter(3)); 
    sig->SetParameter(2,full->GetParameter(4)); 
   
    
    sig->Draw("same");
       
       */
      
         
}
        

void inv_mas ()
{
/////////////////////////////////////////////////////////////////////////////////////////

TFile *profile1 =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUTnew.root", "READ");
TH3D *Lambda= (TH3D*)profile1->Get("lambdy_tot");




TCanvas *c1=new TCanvas("c","c",1000,1000);

c1->Divide(2,2);
//gStyle->SetOptStat(kFALSE);

 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
   //Filling furie[i]  

    
         for(int i=0;i<4;i++)
         {
           TH1D *hist_in= Lambda->ProjectionX(Form("x_%d_%d",1,i),4,4,i+1,i+1,"e");
           hist_in->SetTitle(";m_{inv}[GeV/c^{2}] (#Lambda #rightarrow p + #pi^{-})");
           c1->cd(i+1);
          // hist_in->Draw();
           delete_bg(hist_in);
           TLatex latex(1.112,400,Form("#font[12]{%d-%d centrality (%)}",70-10*i,80-10*i));
latex.SetTextSize(0.04);
latex.DrawClone("Same");

          
          } 
}