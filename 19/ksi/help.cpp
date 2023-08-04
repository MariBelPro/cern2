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
double furie_fit(Double_t *x, Double_t* par)
{
    return par[0]*(1+2*par[1]*sin(x[0])+2*par[2]*cos(x[0])+2*par[3]*sin(2*x[0])+2*par[4]*cos(2*x[0]));
}
double cosn(Double_t *x, Double_t* par)
{
    return par[0]*(1+2*par[1]*cos(x[0]));
}
double sins(Double_t *x, Double_t* par)
{
    return par[0]*(1+2*par[1]*sin(x[0]));
}
void delete_bg( TH1D *hist_in, Double_t* sum)
{
    TH1D *hist_out=new TH1D ("h","h",200,1.305,1.345);
    float xL;
    float xR ;
    float bg;
    float fe;
    float all;
            TF1 *full=new TF1 ("full",fit,1.305,1.345,7);
            full->SetParameter(0,2);
            full->SetParameter(1,0);
            full->SetParameter(2,40);
            full->SetParameter(3,1.3216);
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
    
    TF1 *back_g =new TF1("bg",b_g,1.305,1.345,2);
    back_g->SetParameter(0,full->GetParameter(0));
    back_g->SetParameter(1,full->GetParameter(1)); 
    back_g->Draw("same");
    
        for( int i=0; i<200; i++ )
        {
                
                xL = hist_in->GetBinLowEdge(i+1);
                xR = hist_in->GetBinLowEdge(i+2);
                bg =200*back_g->Integral(xL,xR)/0.04;
                // 0.02/200 - width of one bin
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
         
}
        

void help ()
{
/////////////////////////////////////////////////////////////////////////////////////////

TFile *profile1 =TFile::Open("ksi.root", "READ");

         
}