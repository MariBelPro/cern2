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
// FemtoDst headers
#include "StFemtoDstReader.h"
#include "StFemtoDst.h"
#include "StFemtoEvent.h"
#include "StFemtoTrack.h"

#include "StFemtoEpdHit.h"

// StEpdUtil headers

R__LOAD_LIBRARY(libStFemtoDst.so)

 void run_analisis(const Char_t *inFile = "olo.list",
const Char_t *oFileName = "oTest.root")

{
    StFemtoDstReader* femtoReader = new StFemtoDstReader(inFile);
femtoReader->Init();
femtoReader->SetStatus("*",0);
femtoReader->SetStatus("Event",1);
femtoReader->SetStatus("Track",1);
femtoReader->SetStatus("EpdHit",1);
std::cout << "Status has been set" << std::endl;

std::cout << "Now I know what to read, Master!" << std::endl;

if( !femtoReader->chain() ) {
std::cout << "No chain has been found." << std::endl;
}
Long64_t eventsInTree = femtoReader->tree()->GetEntries();
std::cout << "eventsInTree: "<< eventsInTree<< std::endl;
Long64_t events2read = femtoReader->chain()->GetEntries();

std::cout <<"Number of events to read: " << events2read << std::endl;

 TProfile *hEventProfile = new TProfile("hEventProfile_0","Profile of refMult;Run ID;<refMult>",
                                7000  , 20062000, 20069000 );
                              
TProfile *hHitProfile = new TProfile("hHitProfile_0","Profile of epdHits;Run ID;<adc>",
                                  7000  , 20062000, 20069000 );

//over events
Int_t hundredIter = 0;
Int_t eventCounter = 0;
Double_t ADC=0;
for (Long64_t iEvent=0; iEvent<events2read; iEvent++) //loop over events
{


Bool_t readEvent = femtoReader->readFemtoEvent(iEvent);
StFemtoDst *dst = femtoReader->femtoDst();
StFemtoEvent *Event=dst->event();

eventCounter++;
  if( eventCounter >= 100000 ) {
  eventCounter = 0;
  hundredIter++;
  std::cout << "Working on event #[" << (hundredIter * 100000)
  << "/" << events2read << "]" << std::endl;
  }
  hEventProfile->Fill(Event->runId(),Event->refMult());

  //over hits
  for(int i=0;i<dst->numberOfEpdHits();i++)
{
  StFemtoEpdHit* epdhit =dst->epdHit(i);
  	ADC += epdhit->adc();

}
hHitProfile->Fill(Event->runId(),ADC);
ADC=0;
} //over events

Double_t stat_ref[6];
Double_t stat_hit[6];
hEventProfile->GetStats(stat_ref);
hHitProfile->GetStats(stat_hit);
Double_t mean_Y_ref=stat_ref[4]/stat_ref[0];
Double_t mean_Y_hit=stat_hit[4]/stat_hit[0];
Double_t mean_erY_ref=sqrt(stat_ref[5]/stat_ref[0]-pow(stat_ref[4]/stat_ref[0],2))/sqrt(stat_ref[0]);
Double_t mean_erY_hit=sqrt(stat_hit[5]/stat_hit[0]-pow(stat_hit[4]/stat_hit[0],2))/sqrt(stat_hit[0]);

TFile *oFile = new TFile(oFileName, "RECREATE");
hEventProfile->Write();
hHitProfile->Write();

oFile->Close();
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
    std::cout << "Usage: ./epd inputFileName outputFileName.root" << std::endl;
    return -1;
  }
  std::cout << " inputFileName : " << fileName << std::endl;
  std::cout << " outputFileName: " << oFileName << std::endl;

  run_analisis(fileName, oFileName);
  return 1;
}