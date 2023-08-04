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
// FemtoDst headers
#include "../../StFemtoEvent/StFemtoDstReader.h"
#include "../../StFemtoEvent/StFemtoDst.h"
#include "../../StFemtoEvent/StFemtoEvent.h"
#include "../../StFemtoEvent/StFemtoTrack.h"
#include "../../StFemtoEvent/StFemtoV0.h"
#include "../../StFemtoEvent/StFemtoXi.h"

// Load libraries (for ROOT_VERSTION_CODE >= 393215)
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
R__LOAD_LIBRARY(../../StFemtoEvent/libStFemtoDst.so)
#endif

// Forward declarations
Bool_t isMyEvent(const TVector3& vtx);
Bool_t isMyTrack(const TVector3& mom, const Int_t& nHits,
                   const Int_t& nHitsPoss, const Float_t& DCA ) ;
Bool_t isGoodEvent(const TVector3& vtx, const Float_t& vpdVz);
Bool_t isGoodTrack(const TVector3& vect, const Int_t& nHits,
                   const Int_t& nHitsPoss);

const Float_t electron_mass = 0.0005485799;
const Float_t pion_mass = 0.13957061;
const Float_t kaon_mass = 0.493677;
const Float_t proton_mass = 0.9382720813;

const Float_t electron_mass_sqr = 0.000000301;
const Float_t pion_mass_sqr = 0.019479955;
const Float_t kaon_mass_sqr = 0.24371698;
const Float_t proton_mass_sqr = 0.880354499;









void BBC()
 {
          TCanvas *c2=new TCanvas("c2","c2",1000,1000);
          TFile *profile1=new TFile("furie_l.root","READ");
          TGraph *L_bbc  =(TGraph*)profile1->Get("Graph;11");
 L_bbc->SetMarkerStyle(21);
 L_bbc->SetMarkerColor(kBlue);
 L_bbc->SetTitle("Analysis for #Lambda  (BBC);centrality (%); Polarization (%)");
 L_bbc->Draw("AP");

  TFile *profile2=new TFile("furie_antyl.root","READ");
  TGraph *AntyL_bbc  =(TGraph*)profile2->Get("Graph;11");
 AntyL_bbc->SetMarkerStyle(21);
 AntyL_bbc->SetMarkerColor(kRed);
 AntyL_bbc->SetTitle("Analysis for #bar{#Lambda} (BBC);centrality (%); Polarization (%)");
 AntyL_bbc->Draw("AP");

 TFile *profile3=new TFile("../Macros/furie_l.root","READ");
          TGraph *L_epd  =(TGraph*)profile3->Get("Graph;11");
 L_epd->SetMarkerStyle(8);
 L_epd->SetMarkerColor(kRed);
 L_epd->SetTitle("Analysis for #Lambda (EPD);centrality (%); Polarization (%)");
 //L_epd->Draw("P");
  TFile *profile4=new TFile("../Macros/furie_antyl.root","READ");
  TGraph *AntyL_epd  =(TGraph*)profile4->Get("Graph;11");
 AntyL_epd->SetMarkerStyle(8);
 AntyL_epd->SetMarkerColor(1);
 AntyL_epd->SetTitle("Analysis for #bar{#Lambda} (EPD);centrality (%); Polarization (%)");
 AntyL_epd->Draw("P");
 c2->BuildLegend();
                
              

}
 
  


   
 
 

   

  
  