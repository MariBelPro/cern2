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
double cosn2(Double_t *x, Double_t* par)
{
    return par[0]*(1+2*par[1]*cos(2*x[0]));
}
double sins2(Double_t *x, Double_t* par)
{
    return par[0]*(1+2*par[1]*sin(2*x[0]));
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
       
        sum[0]=hist_in->IntegralAndError(1,200,sum[1]);
         
}
        

void furie_draw() 
{
/////////////////////////////////////////////////////////////////////////////////////////

TFile *profile1 =TFile::Open("StEpdEpFinderCorrectionHistograms_INPUTksi.root", "READ");
TH3D *Lambda= (TH3D*)profile1->Get("Antylambdy_tot");

//TH3D *Anty_Lambda= (TH3D*)profile1->Get("Antylambdy_tot");

TF1 *fit_f=new TF1 ("par[0]*(1+2*par[1]*sin(x[0])+2*par[2]*cos(x[0])+2*par[3]*sin(2*x[0])+2*par[4]*cos(2*x[0]))",furie_fit,-3,3,5);
fit_f->SetTitle("p_{0}+2#sum_{i=1}^{#infty}{p_{n,s}sin(n#Delta#phi)+p_{n,c}cos(n#Delta#phi)}");
fit_f->GetXaxis()->SetTitle("#Delta#phi*=#psi-#phi*_{p} (rad)");
TF1 *cos_f=new TF1("par[0]*(1+2*par[2]*cos(x))",cosn,-3.2,3.2,2); 
cos_f->SetTitle("p_{0}+2p_{1,c}cos(#Delta#phi*)") ;
cos_f->SetLineWidth(3);
cos_f->SetLineColor(1);
TF1 *sin_f=new TF1("par[0]*(1+2*par[1]*sin(x))",sins,-3.2,3.2,2);
sin_f->SetTitle("p_{0}+2p_{1,s}sin(#Delta#phi*)") ;
sin_f->SetLineWidth(3);
sin_f->SetLineColor(6);
sin_f->SetLineStyle(3);
TF1 *cos2_f=new TF1("par[0]*(1+2*par[2]*cos(2x))",cosn2,-3.2,3.2,2); 
cos2_f->SetTitle("p_{0}+2p_{1,c}cos(2#Delta#phi*)") ;
cos2_f->SetLineWidth(3);
cos2_f->SetLineColor(kBlue);
TF1 *sin2_f=new TF1("par[0]*(1+2*par[1]*sin(2x))",sins2,-3.2,3.2,2);
sin2_f->SetTitle("p_{0}+2p_{1,s}sin(2#Delta#phi*)") ;
sin2_f->SetLineWidth(3);
sin2_f->SetLineColor(9);
sin2_f->SetLineStyle(2);
TGraphErrors *furie[9];
  
    Double_t phi[10]={-2.7,-2.1,-1.5,-0.9,-0.3,0.3,0.9,1.5,2.1,2.7};
    Double_t ephi[10]={0,0,0,0,0,0,0,0,0,0};
    Double_t amunt[10];
    Double_t eamunt[10], sum[2]; 

TCanvas *c1=new TCanvas("c","c",1000,1000);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
   //Filling furie[i]  
    
         for(int i=0;i<10;i++)
         {
           TH1D *hist_in= Lambda->ProjectionX(Form("x_%d_%d",i,5),i+1,i+1,6,6,"e");
           hist_in->SetTitle(";m_{inv}");
         
           delete_bg(hist_in,sum);
           amunt[i]=sum[0];
           eamunt[i]=sum[1];
          } 

    
 furie[4]=new TGraphErrors(10,phi,amunt,ephi,eamunt);
  
            fit_f->SetParameter(0,400);
            fit_f->SetParameter(1,-0.002);
           // fit_f->SetParLimits(1,-10,0);
            fit_f->SetParameter(2,1);
            fit_f->SetParameter(3,0);
            fit_f->SetParameter(4,0.002);

 furie[4]->Fit(fit_f,"RMEN");
 
    cos_f->SetParameter(0,fit_f->GetParameter(0));
    cos_f->SetParameter(1,fit_f->GetParameter(2));

    cos2_f->SetParameter(0,fit_f->GetParameter(0));
    cos2_f->SetParameter(1,fit_f->GetParameter(4));
   // cos_f->SetLineColor(1);
    
    sin_f->SetParameter(0,fit_f->GetParameter(0));
    sin_f->SetParameter(1,fit_f->GetParameter(1));

    sin2_f->SetParameter(0,fit_f->GetParameter(0));
    sin2_f->SetParameter(1,fit_f->GetParameter(3));
 //   sin_f->SetLineColor(kBlue);
furie[4]->SetMarkerStyle(22);
furie[4]->SetMarkerSize(3);
furie[4]->SetMarkerColor(kRed);
furie[4]->SetLineWidth(3);

fit_f->Draw();
furie[4]->SetTitle(";#Delta#phi*=#psi-#phi*_{p} (rad);dN/d#Delta#phi*");
sin_f->Draw("same");
 cos_f->Draw("same");
 sin2_f->Draw("same");
 cos2_f->Draw("same");
 c1->BuildLegend();
furie[4]->Draw("P");

TLatex latex2(-2,15800,"#font[12]{#bar{#Lambda}   Au+Au #sqrt{s_{NN}}=19.6 GeV}");
latex2.SetTextSize(0.04);
latex2.DrawClone("Same");

TLatex latex(-2,15800,"#font[12]{20-30% centrality}");
latex.SetTextSize(0.04);
latex.DrawClone("Same");
//over centralities


}