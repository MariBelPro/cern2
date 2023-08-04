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



R__LOAD_LIBRARY(libStFemtoDst.so)


void Raw(const Char_t *inFile = "olo.list",
                      const Char_t *oFileName = "oTest.root") {

  std::cout << "Hi! Lets do some physics, Balda!" << std::endl;


  
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
 

int order=3;

           TH1D *phi_W[9];
      for(int i=0;i<9; i++)
        {
         phi_W[i]= new TH1D(Form("#phi_W for centrality %i",i ),Form("#phi_W for centrality bin %i",i ),100,0,2*TMath::Pi()/order);
         phi_W[i]->SetTitle(";#phi;");
        }  
           TH1D *phi_E[9];
      for(int i=0;i<9; i++)
        {
         phi_E[i]= new TH1D(Form("#phi_E for centrality %i",i ),Form("#phi_E for centrality bin %i",i ),100,0,2*TMath::Pi()/order);
         phi_E[i]->SetTitle(";#phi;");
        }  

  //Profiles                     
    TProfile *Qxe_profile= new TProfile ("Qxe_profile","Qxe_centrelity",9,-0.5,8.5);  
    TProfile *Qye_profile= new TProfile ("Qye_profile","Qye_centrelity",9,-0.5,8.5);  
    TProfile *Qyw_profile= new TProfile ("Qyw_profile","Qyw_centrelity",9,-0.5,8.5);  
    TProfile *Qxw_profile= new TProfile ("Qxw_profile","Qxw_centrelity",9,-0.5,8.5);


                                                                                                         
     

 Int_t hundredIter = 0;
Int_t eventCounter = 0;
for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {
    Bool_t readEvent = femtoReader->readFemtoEvent(iEvent);
    eventCounter++;
    if( eventCounter >= 100000 ) {
      eventCounter = 0;
      hundredIter++;
      std::cout << "Working on event #[" << (hundredIter * 100000)
        	      << "/" << events2read << "]" << std::endl;
    }

    // Retrieve femtoDst
    StFemtoDst *dst = femtoReader->femtoDst();

    // Retrieve event information
    StFemtoEvent *event = dst->event();
        TVector3 pVtx = event->primaryVertex();
    if ( !isGoodEvent(pVtx, event->vpdVz() ) ) continue;

    TVector3 q_E;
    TVector3 q_W;
 
    TVector3 Q_E;
    TVector3 Q_W;
   Int_t nTracks = dst->numberOfTracks();
    for(Int_t iTrk=0; iTrk<nTracks; iTrk++) 
    {

      // Retrieve i-th femto track
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
           else if(femtoTrack->eta()<-0.05) 
          {
           q_E.SetX(cos(order*femtoTrack->phi())*femtoTrack->pt());
           q_E.SetY(sin(order*femtoTrack->phi())*femtoTrack->pt());
           Q_E+=q_E;
           }
    
    }//over tracks

    

  //Filling histograms      
    
      phi_W[event->cent9()]->Fill(GetPsiInRange(Q_W.X(),Q_W.Y(),order));
      phi_E[event->cent9()]->Fill(GetPsiInRange(Q_E.X(),Q_E.Y(),order));
       Qye_profile-> Fill(event->cent9(), Q_E.Y());
       Qyw_profile-> Fill(event->cent9(), Q_W.Y());
       Qxe_profile-> Fill(event->cent9(), Q_E.X());
       Qxw_profile-> Fill(event->cent9(), Q_W.X());

  
    Q_W.SetX(0);
    Q_W.SetY(0);
    Q_E.SetX(0);
    Q_E.SetY(0);

   
  } //for(Long64_t iEvent=0; iEvent<events2read; iEvent++)
  

  TFile *oFile = new TFile(oFileName, "RECREATE");
  oFile->cd();

   for(int i=0;i<9; i++)
        {
        
         phi_W[i]->Write();
         phi_E[i]->Write();
        
        }  


Qye_profile-> Write();
Qyw_profile-> Write();
Qxe_profile-> Write();
Qxw_profile->Write(); 


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

  Raw(fileName, oFileName);
  return 1;
}


