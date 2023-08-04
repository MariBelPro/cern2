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

void delete_bg( TH1D *hist_in,Double_t* sum )
{
    TH1D *hist_out=new TH1D ("h","h",200,1.109,1.121);
    float xL;
    float xR ;
    float bg;
    float fe;
    float all;
            TF1 *full=new TF1 ("full",fit,1.109,1.121,7);
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

    TF1 *back_g =new TF1("bg",b_g,1.109,1.121,2);
    back_g->SetParameter(0,full->GetParameter(0));
    back_g->SetParameter(1,full->GetParameter(1)); 
    back_g->SetLineColor(kBlue);
     back_g->SetLineStyle(9);
    back_g->Draw("same");
    
        for( int i=0; i<200; i++ )
        {
                
                xL = hist_in->GetBinLowEdge(i+1);
               
                xR = hist_in->GetBinLowEdge(i+2);
                bg =200*back_g->Integral(xL,xR)/0.012;
                // 0.012/200 - width of one bin
                fe=hist_in->GetBinError(i+1);
                if(bg>hist_in->GetBinContent(i+1))
                {
                    bg=hist_in->GetBinContent(i+1);
                }
                all= hist_in->GetBinContent(i+1)-bg;
                hist_out->SetBinContent( i+1, all );
                hist_out->SetBinError( i+1, fe );
                      
        }
       
        sum[0]=hist_in->IntegralAndError(1,200,sum[1]);
         cout<< sum[0]<<" "<< hist_out->IntegralAndError(1,200,sum[1])<<endl;
       
      //hist_out->Draw();
         
}
        

void inv_mas ()
{
/////////////////////////////////////////////////////////////////////////////////////////

TFile *profile1 =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUT.root", "READ");
TH3D *Lambda= (TH3D*)profile1->Get("lambdy_tot");


Double_t sum[2];

TCanvas *c1=new TCanvas("c","c",1000,1000);

c1->Divide(5,2);
//gStyle->SetOptStat(kFALSE);

 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
   //Filling furie[i]  

    
         for(int i=0;i<9;i++)
         {
           TH1D *hist_in= Lambda->ProjectionX(Form("x_%d_%d",1,i),4,4,i+1,i+1,"e");
           hist_in->SetTitle(";m_{inv}[GeV/c^{2}] (#Lambda #rightarrow p + #pi^{-})");
           c1->cd(i+1);
          hist_in->Draw();
           //delete_bg(hist_in,sum);
         /*  TLatex latex(1.112,400,Form("#font[12]{%d-%d centrality (%)}",70-10*i,80-10*i));
latex.SetTextSize(0.04);
latex.DrawClone("Same"); */

          
          } 
}