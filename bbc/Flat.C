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
#include "StFemtoDstReader.h"
#include "StFemtoDst.h"
#include "StFemtoEvent.h"
#include "StFemtoTrack.h"
#include "StFemtoV0.h"
#include "StFemtoXi.h"

// Load libraries (for ROOT_VERSTION_CODE >= 393215)
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
R__LOAD_LIBRARY(libStFemtoDst.so)
#endif

// Forward declarations

Bool_t isGoodEvent(const TVector3& vtx, const Float_t& vpdVz);
Bool_t isGoodTrack(const TVector3& vect, const Int_t& nHits,
                   const Int_t& nHitsPoss);
double GetPsiInRange(double Qx, double Qy, int order){
  double temp;
  if ((Qx==0.0)||(Qy==0.0)) temp=-999;
  else{
    temp = atan2(Qy,Qx)/((double)order);
    double AngleWrapAround = 2.0*TMath::Pi()/(double)order;
    if (temp<0.0) temp+= AngleWrapAround;
    else if (temp>AngleWrapAround) temp -= AngleWrapAround;
  }
  return temp;
}
double GetPhiInRange(double phi, int order)
  {

    double AngleWrapAround = 2.0*TMath::Pi()/(double)order;
    if (phi<0.0) phi+= AngleWrapAround;
    else if (phi>AngleWrapAround) phi -= AngleWrapAround;
  return phi;
}



R__LOAD_LIBRARY(libStFemtoDst.so)




void Flat(const Char_t *inFile = "olo.list",
                      const Char_t *oFileName = "oTest.root") {

int order=3;
  std::cout << "Hi! Lets do some physics, Balda!" << std::endl;

  //gSystem->Load("./libStFemtoDst.so");
  #if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
    gSystem->Load("./libStFemtoDst.so");
  #endif

  StFemtoDstReader* femtoReader = new StFemtoDstReader(inFile);
  femtoReader->Init();

  //Long64_t events2read = femtoReader->chain()->GetEntries();

  // This is a way if you want to spead up IO
  std::cout << "Explicit read status for some branches" << std::endl;
  femtoReader->SetStatus("*",0);
  femtoReader->SetStatus("Event",1);
  femtoReader->SetStatus("Track",1);
  std::cout << "Status has been set" << std::endl;

  std::cout << "Now I know what to read, Master!" << std::endl;

  if( !femtoReader->chain() ) {
    std::cout << "No chain has been found." << std::endl;
  }
  Long64_t eventsInTree = femtoReader->tree()->GetEntries();
  std::cout << "eventsInTree: "  << eventsInTree << std::endl;
  Long64_t events2read = femtoReader->chain()->GetEntries();

  std::cout << "Number of events to read: " << events2read << std::endl;

  //TFile *oFile = new TFile(oFileName, "RECREATE");

  // Histogramming
  // Event 
 
 
 TFile *profile1 =TFile::Open("Q_vector.root", "READ");
  TProfile *defined_Qye= (TProfile*)profile1->Get("Qye_profile");
  TProfile *defined_Qyw= (TProfile*)profile1->Get("Qyw_profile");
  TProfile *defined_Qxe= (TProfile*)profile1->Get("Qxe_profile");
  TProfile *defined_Qxw= (TProfile*)profile1->Get("Qxw_profile");


   TFile *f= new TFile("Q_recentered.root","READ");
  TProfile *East[10];
     for(int i=0;i<10;i++)
     {
         East[i]= (TProfile*)f->Get(Form("east_flat_%i",i));
     }
    TProfile *West[10];
     for(int i=0;i<10;i++)
     {
         West[i]= (TProfile*)f->Get(Form("west_flat_%i",i));
     } 
    
     
       TProfile *Res= new TProfile ("res","res",9,-0.5,8.5);
    TH1D *phi_E[9];
      for(int i=0;i<9; i++)
        {
         phi_E[i]= new TH1D(Form("phiE_for_cent_%i",i ),Form("phiE_for_cent_%i",i ),50,0,2*TMath::Pi()/order);
        }  
            TH1D *phi_W[9];
      for(int i=0;i<9; i++)
        {
         phi_W[i]= new TH1D(Form("phiW_for_cent_%i",i ),Form("phiW_for_cent_%i",i ),50,0,2*TMath::Pi()/order);
        }             

  Int_t eventCounter = 0;
  Int_t hundredIter = 0;
  

  
  for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {

    eventCounter++;
    if( eventCounter >= 100000 ) {
      eventCounter = 0;
      hundredIter++;
      std::cout << "Working on event #[" << (hundredIter * 100000)
        	      << "/" << events2read << "]" << std::endl;
    }


    Bool_t readEvent = femtoReader->readFemtoEvent(iEvent);
    if( !readEvent ) {
      std::cout << "Something went wrong, Master! Nothing to analyze..." << std::endl;
      break;
    }

    // Retrieve femtoDst
    StFemtoDst *dst = femtoReader->femtoDst();

    // Retrieve event information
    StFemtoEvent *event = dst->event();
    if( !event ) {
      std::cout << "Something went wrong, Master! Event is hiding from me..." << std::endl;
      break;
    }

    TVector3 pVtx = event->primaryVertex();

   if ( !isGoodEvent(pVtx, event->vpdVz() ) ) continue;
  
    TVector3 q;
    TVector3 q_E;
    TVector3 q_W;
   
    TVector3 Q_E;
    TVector3 Q_W;
    Double_t phi_help, phi_help2, phi_help3;
     Int_t nTracks = dst->numberOfTracks();
    for(Int_t iTrk=0; iTrk<nTracks; iTrk++) 
    {
      StFemtoTrack *femtoTrack = dst->track(iTrk);

      if (!femtoTrack) continue;
      if( !isGoodTrack( femtoTrack->pMom(), femtoTrack->nHits(), femtoTrack->nHitsPoss() ) ) continue;
        
         if ( !femtoTrack->isPrimary() ) continue;
         if(femtoTrack->eta()>0.05) 
          {
           q_W.SetX(cos(order*femtoTrack->phi())*femtoTrack->pt());
           q_W.SetY(sin(order*femtoTrack->phi())*femtoTrack->pt());
           Q_W+=q_W;
           }
            if(femtoTrack->eta()<-0.05) 
          {
           q_E.SetX(cos(order*femtoTrack->phi())*femtoTrack->pt());
           q_E.SetY(sin(order*femtoTrack->phi())*femtoTrack->pt());
           Q_E+=q_E;
           }
    
    }//over tracks
//Flattening
   
    
  
     
    q.SetX(Q_E.X());
    Q_E.SetX(q.X()-defined_Qxe->GetBinContent(event->cent9()+1));
     q.SetY(Q_E.Y());
    Q_E.SetY(q.Y()-defined_Qye->GetBinContent(event->cent9()+1));

      phi_help2=GetPsiInRange(Q_E.X(),Q_E.Y(),order);
    Double_t psi=0;
      for(int i=1;i<10;i++)
      {
          psi+=(East[i]->GetBinContent(event->cent9()+1)*(-2)*cos(i*order*Q_E.Phi())+East[i]->GetBinContent(event->cent9()+10)*sin(i*order*Q_E.Phi()))/(order*i);
      }
      phi_help2+=GetPhiInRange(psi,order);
      phi_E[event->cent9()]->Fill(GetPhiInRange(phi_help2,order));

    q.SetX(Q_W.X());
    Q_W.SetX(q.X()-defined_Qxw->GetBinContent(event->cent9()+1));
    q.SetY(Q_W.Y());
    Q_W.SetY(q.Y()-defined_Qyw->GetBinContent(event->cent9()+1));
    phi_help3=GetPsiInRange(Q_W.X(),Q_W.Y(),order);
    psi=0;
      for(int i=1;i<10;i++)
      {
          psi+=(West[i]->GetBinContent(event->cent9()+1)*(-2)*cos(i*order*Q_W.Phi())+West[i]->GetBinContent(event->cent9()+10)*2*sin(i*order*Q_W.Phi()))/(order*i);
      }
      phi_help3+=GetPhiInRange(psi,order);
       phi_W[event->cent9()]->Fill(GetPhiInRange(phi_help3,order));

       //Resolution
    Res->Fill(event->cent9(),cos(order*(phi_help3-phi_help2)));
       
      
  }//over events

  // Saving
 TFile *oFile = new TFile(oFileName, "RECREATE");
  oFile->cd();

      for(int i=0;i<9; i++)
        {
         phi_E[i]->Write();
         phi_W[i]->Write();
        }
      Res->Write();
  
     oFile->Close();
  femtoReader->Finish();

  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!"
	    << std::endl;
}


//________________
Bool_t isGoodEvent(const TVector3& vtx, const Float_t& vpdVz) {
  return ( TMath::Abs( vtx.Z() ) <= 70. &&
           TMath::Abs( vtx.Z() - vpdVz ) <= 15. &&
           vtx.Perp() <= 2. );
}

//________________
Bool_t isGoodTrack(const TVector3& mom, const Int_t& nHits,
                   const Int_t& nHitsPoss) {
  return ( mom.Mag() >= 0.15 &&
           mom.Mag() <= 2.1 &&
           TMath::Abs( mom.Eta() ) <= 1. &&
           nHits >= 11 &&
           (Float_t)nHits/nHitsPoss >= 0.51 );
}
int main(int argc, char* argv[]) {

  const char* fileName;
  const char* oFileName;

  switch (argc) {
  case 3:
    fileName = argv[1];
    oFileName = argv[2];
    break;
  default:
    std::cout << "Usage: ./FemtoDstAnalyzer inputFileName outputFileName.root" << std::endl;
    return -1;
  }
  std::cout << " inputFileName : " << fileName << std::endl;
  std::cout << " outputFileName: " << oFileName << std::endl;

  Flat(fileName, oFileName);
  return 1;
}