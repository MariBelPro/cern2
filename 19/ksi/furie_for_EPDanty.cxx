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
    TH1D *hist_out=new TH1D ("h","h",200,1.105,1.125);
    float xL;
    float xR ;
    float bg;
    float fe;
    float all;
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
   
    
        for( int i=0; i<200; i++ )
        {
                
                xL = hist_in->GetBinLowEdge(i+1);
                xR = hist_in->GetBinLowEdge(i+2);
                bg =200*back_g->Integral(xL,xR)/0.02;
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
       
        sum[0]=hist_out->IntegralAndError(1,200,sum[1]);
         
}
        

void furie_for_EPDanty ()
{
/////////////////////////////////////////////////////////////////////////////////////////

TFile *profile1 =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUTnew.root", "READ");
TH3D *Lambda= (TH3D*)profile1->Get("Antylambdy_tot");
TProfile *res=(TProfile*)profile1->Get("AveCosDeltaPsi1");
//TH3D *Anty_Lambda= (TH3D*)profile1->Get("Antylambdy_tot");

TFile *f_l_out=new TFile("furie_antyl.root","RECREATE");
//TFile *f_antyl_out=new TFile("furie_antyl.root","RECREATE");



TF1 *fit_f=new TF1 ("par[0]*(1+2*par[1]*sin(x[0])+2*par[2]*cos(x[0])+2*par[3]*sin(2*x[0])+2*par[4]*cos(2*x[0]))",furie_fit,-3,3,5);
TF1 *cos_f=new TF1("par[0]*(1+2*par[2]*cos(x))",cosn,-3.2,3.2,2);  
TF1 *sin_f=new TF1("par[0]*(1+2*par[1]*sin(x))",sins,-3.2,3.2,2);
TGraphErrors *furie[9];
    Double_t sum[2];
    Double_t phi[10]={-2.7,-2.1,-1.5,-0.9,-0.3,0.3,0.9,1.5,2.1,2.7};
    Double_t ephi[10]={0,0,0,0,0,0,0,0,0,0};
    Double_t amunt[10];
    Double_t eamunt[10]; 

TCanvas *c1=new TCanvas("c","c",1000,1000);
TCanvas *c2=new TCanvas("c2","c",1000,1000);
c1->Divide(5,2);
//gStyle->SetOptStat(kFALSE);

 Double_t Pol[9]; 
     Double_t ePol[9]; 
     Double_t cent[9]={3,8,13,23,33,43,53,63,73};
     Double_t ecent[9]={0,0,0,0,0,0,0,0,0};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
   //Filling furie[i]  
for(int c=0;c<9;c++)
{
    
         for(int i=0;i<10;i++)
         {
           TH1D *hist_in= Lambda->ProjectionX(Form("x_%d_%d",i,c),i+1,i+1,c+1,c+1,"e");
           hist_in->SetTitle(";m_{inv}");
           c1->cd(i+1);
           delete_bg(hist_in,sum);
           amunt[i]=sum[0];
           eamunt[i]=sum[1];
          } 

    
 furie[c]=new TGraphErrors(10,phi,amunt,ephi,eamunt);
  
            fit_f->SetParameter(0,400);
            fit_f->SetParameter(1,-0.002);
           // fit_f->SetParLimits(1,-10,0);
            fit_f->SetParameter(2,1);
            fit_f->SetParameter(3,0);
            fit_f->SetParameter(4,0.002);
 c2->cd();
 furie[c]->Fit(fit_f,"RME");
 Pol[8-c]=-345*fit_f->GetParameter(1)/sqrt(2*res->GetBinContent(c+1));
 ePol[8-c]=345*fit_f->GetParError(1)/sqrt(2*res->GetBinContent(c+1));
    cos_f->SetParameter(0,fit_f->GetParameter(0));
    cos_f->SetParameter(1,fit_f->GetParameter(2));
    cos_f->SetLineColor(1);
    
    sin_f->SetParameter(0,fit_f->GetParameter(0));
    sin_f->SetParameter(1,fit_f->GetParameter(1));
    sin_f->SetLineColor(kBlue);

furie[c]->Draw("AP*");
fit_f->Draw("same");
furie[c]->SetTitle(Form("cent_%d",c));
sin_f->Draw("same");
 cos_f->Draw("same");
 c2->BuildLegend();
     f_l_out->cd();
furie[c]->Write();
 ////////////////////////////////////


} //over centralities

 TGraphErrors *Polar=new TGraphErrors(9,cent,Pol,ecent,ePol);
    Polar->SetTitle("Analysis for #Lambda;centrality (%); Polarisation (%)");
    Polar->SetMarkerStyle(8);
Polar->Write();   
f_l_out->Close();

}