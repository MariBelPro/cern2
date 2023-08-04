// This is needed for calling standalone classes (not needed on RACF)
#ifndef _VANILLA_ROOT_
#define _VANILLA_ROOT_
#endif
// C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
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
double fit(Double_t *x, Double_t* par)
{
    return par[0]+3*par[1];
}
double fit2(Double_t *x, Double_t* par)
{
    return par[0]-3*par[1];
}
void run_profile_hit()
{
  

   Double_t mean_Y=52999.9;
Double_t st_Y=70.8324;
Double_t mean_error=61.6401;
Double_t st_error=10.0098;
    TFile *file=TFile::Open("run.root", "READ");
    TProfile *ref=(TProfile*)file->Get("hHitProfile_0");
  TProfile *get_everage=new TProfile("everage","ever",2,-0.5,1.5);
   TProfile *get_error=new TProfile("everage","ever",2,-0.5,1.5);
  int n_bad_Y=0;
  int n_bad_err=0;

  for(int bin=0;bin<7000;bin++)
  {
    if (ref->GetBinEntries(bin+1)==0) continue;
      get_everage->Fill(1,ref->GetBinContent(bin+1),ref->GetBinEntries(bin+1));
    //  cout<<ref->GetBinContent(bin+1)<<" "<<get_everage->GetBinContent(2)<< " "<<get_everage->GetBinError(2)<<endl;
  }
  cout<<get_everage->GetBinContent(2)<<" "<<get_everage->GetBinError(2)<<endl;

 
 TF1 *mid=new TF1("mid",fit, 20062000, 20069000,2);
 mid->SetParameters(get_everage->GetBinContent(2),0);
  TF1 *down=new TF1("mid",fit2,20062000,20070000,2);
 down->SetParameters(get_everage->GetBinContent(2),get_everage->GetBinError(2));
  TF1 *up=new TF1("mid",fit,20062000,20070000,2);
 up->SetParameters(get_everage->GetBinContent(2),get_everage->GetBinError(2));
 ref->Draw();


  TF1 *down0=new TF1("mid",fit2,20062000,20070000,2);
 down0->SetParameters(0,get_everage->GetBinError(2));
  TF1 *up0=new TF1("mid",fit,20062000,20070000,2);
 up0->SetParameters(0,get_everage->GetBinError(2));
 ref->Draw();
//deviatioan of error
for(int bin=0;bin<7000;bin++)
  { if (ref->GetBinEntries(bin+1)==0) continue;
      get_error->Fill(1,ref->GetBinError(bin+1));
     // cout<<ref->GetBinError(bin+1)<<" "<<ref->GetBinEntries(bin+1)<<endl;
  }
  
 cout<<get_error->GetBinContent(2)<<" "<<get_error->GetBinError(2)<<endl;

//devide profile

for(int bin=0;bin<7000;bin++)
  {
      if(ref->GetBinContent(bin+1)==0) continue;
           if(ref->GetBinContent(bin+1)>mean_Y+3*st_Y ||ref->GetBinContent(bin+1)<mean_Y-3*st_Y)
             {
               n_bad_Y++;
               continue;
             }
            
              if(ref->GetBinError(bin+1)>mean_error+3*st_error)
             {
               n_bad_err++;
             }
           
  }
  Double_t bad_y[n_bad_Y];
  Double_t bad_ey[n_bad_Y];
   Double_t bad_y_run[n_bad_Y];
Double_t zeroY[n_bad_Y];

     Double_t bad_err_y[n_bad_err];
       Double_t bad_err_ey[n_bad_err];
   Double_t bad_err_run[n_bad_err];
   Double_t zeroER[n_bad_err];
   int i=0;
   int j=0;
for(int bin=0;bin<7000;bin++)
  {
      if(ref->GetBinContent(bin+1)==0) continue;
           if(ref->GetBinContent(bin+1)>mean_Y+3*st_Y ||ref->GetBinContent(bin+1)<mean_Y-3*st_Y)
             {
               bad_y_run[i]=bin+20062000.5;
               bad_y[i]=ref->GetBinContent(bin+1);
               bad_ey[i]=ref->GetBinError(bin+1);
               zeroY[i]=0;
               i++;
               continue;
             }
            
              if(ref->GetBinError(bin+1)>mean_error+3*st_error)
             {
               bad_err_run[j]=bin+20062000.5;
               bad_err_y[j]=ref->GetBinContent(bin+1);
               bad_err_ey[j]=ref->GetBinError(bin+1);
               zeroER[j]=0;
               j++;
             }
            //  cout<<ref->GetBinContent(bin+1)<<endl;
  }


TGraphErrors *badrun=new TGraphErrors(n_bad_Y,bad_y_run, bad_y,zeroY,bad_ey);
TGraphErrors *badrun2=new TGraphErrors(n_bad_err,bad_err_run, bad_err_y,zeroER, bad_err_ey);


badrun->SetMarkerColor(kRed);
badrun->SetLineColor(kRed);
badrun->SetLineWidth(2);
badrun->SetMarkerStyle(8);
badrun->SetMarkerSize(2); 
badrun2->SetMarkerColor(kBlue);
badrun2->SetLineWidth(2);
badrun2->SetLineColor(kBlue);
badrun2->SetMarkerStyle(8);
badrun2->SetMarkerSize(2); 
ref->SetMarkerStyle(8);
ref->SetMarkerSize(2); 
ref->SetLineWidth(2);
ref->Draw();
badrun2->Draw("P");
down->SetLineWidth(2);
down->SetLineStyle(9);
mid->SetLineStyle(9);
mid->SetLineWidth(2);
up->SetLineStyle(9);
mid->SetLineWidth(2);
down->Draw("same");
mid->Draw("same");
up->Draw("same");
badrun->Draw("P");
TFile *fil=new TFile("run_profile_hit.root","RECREATE");
ref->Write();
badrun->Write();
badrun2->Write();

}


