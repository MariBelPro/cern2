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

double GetPsiInRange(TVector3 Q,int order){
 
  double wrap = 2.0*TMath::Pi()/order;
  double psi=Q.Phi()/order;
 
  return psi;
  
}


R__LOAD_LIBRARY(../../StFemtoEvent/libStFemtoDst.so)

void check2(const Char_t *inFile = "st_physics_adc_20069061_raw_7000003.femtoDst.root",
                      const Char_t *oFileName = "oTest.root")
{
 int order=1;
 StFemtoDstReader* femtoReader = new StFemtoDstReader(inFile);
  femtoReader->Init();
    TH1D *h=new TH1D("g","h",50,-3.2,3.2);
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
    

    TVector3 q_E;
    TVector3 q_W;
    TVector3 Q_E;
    TVector3 Q_W;
   Int_t nTracks = dst->numberOfTracks();
    for(Int_t iTrk=0; iTrk<nTracks; iTrk++) 
    {

      // Retrieve i-th femto track
      StFemtoTrack *femtoTrack = dst->track(iTrk);

    
         if(femtoTrack->eta()>0.05) 
          {
           q_W.SetX(cos(order*femtoTrack->phi())*femtoTrack->pt());
           q_W.SetY(sin(order*femtoTrack->phi())*femtoTrack->pt());
           Q_W+=q_W;
           cout<<cos(femtoTrack->phi())<<"W";
           }
           else if(femtoTrack->eta()<-0.05) 
          {
           q_E.SetX(cos(order*femtoTrack->phi())*femtoTrack->pt());
           q_E.SetY(sin(order*femtoTrack->phi())*femtoTrack->pt());
           Q_E+=q_E;
            cout<<cos(femtoTrack->phi())<<"E";
           }
           // cout<<Q_W.Phi()<<" "<<Q_E.Phi()<<"    ";
    }//over tracks
   
    //cout<<Q_W.Phi()<<" "<<Q_E.Phi()<<endl;
 


}

}