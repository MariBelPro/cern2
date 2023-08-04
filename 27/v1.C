
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
#include "TF1.h"
#include "TProfile.h"
#include "TMath.h"
#include "TString.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TObjArray.h"
#include "StFemtoDstReader.h"
#include "StFemtoDst.h"
#include "StFemtoEvent.h"
#include "StFemtoTrack.h"
#include "StFemtoV0.h"
#include "StFemtoXi.h"
#include "StFemtoEpdHit.h"
// StEpdUtil headers
#include "StEpdEpFinder.h"
#include "StEpdEpInfo.h"
#include "StEpdGeom.h"



R__LOAD_LIBRARY(libStFemtoDst.so)
R__LOAD_LIBRARY(libStEpdUtil.so)
Bool_t isMyTrack(const TVector3& mom, const Int_t& nHits,
const Int_t& nHitsPoss, const Float_t& DCA ) ;
void v1(const Char_t *inFile = "StEpdEpFinderCorrectionHistograms_INPUT.root",
const Char_t *oFileName = "oTest.root")
{


StFemtoDstReader* femtoReader = new StFemtoDstReader(inFile);
femtoReader->Init();
femtoReader->SetStatus("*",0);
femtoReader->SetStatus("Event",1);
femtoReader->SetStatus("Track",1);
femtoReader->SetStatus("KFP",0);
femtoReader->SetStatus("EpdHit",1);
std::cout << "Status has been set" << std::endl;

std::cout << "Now I know what to read, Master!" << std::endl;

if( !femtoReader->chain() ) {
std::cout << "No chain has been found." << std::endl;
}
Long64_t eventsInTree = femtoReader->tree()->GetEntries();
std::cout << "eventsInTree: " << eventsInTree << std::endl;
Long64_t events2read = femtoReader->chain()->GetEntries();

std::cout << "Number of events to read: " << events2read << std::endl;


 //TFile *profile1 =TFile::Open("psi.root", "READ");
//TProfile *Res=(TProfile*)profile1->Get("cos"));

StEpdEpFinder *find= new StEpdEpFinder(9,oFileName);
find->SetEpdHitFormat(2);
find->SetnMipThreshold(0.3);
find->SetMaxTileWeight(1);
TH1D *nMip_ring= new TH1D("nMip","; ring nomber",16,1,16);
TH1D *till_center= new TH1D("til","til",100,-3,6);
TProfile *v1[9];
for(int i=0;i<9; i++)
{
v1[i]= new TProfile (Form("for_centrality_bin_%d",i),Form("for_centrality_bin_%d",i),10,-5,5);
}

Int_t hundredIter = 0;
Int_t eventCounter = 0;
for (Long64_t iEvent=0; iEvent<events2read; iEvent++) //loop over events
{
  Bool_t readEvent = femtoReader->readFemtoEvent(iEvent);
  eventCounter++;
  if( eventCounter >= 100000 ) {
  eventCounter = 0;
  hundredIter++;
  std::cout << "Working on event #[" << (hundredIter * 100000)
  << "/" << events2read << "]" << std::endl;
  }
  StEpdGeom* geo = new StEpdGeom();
  StFemtoDst *dst = femtoReader->femtoDst();
  StFemtoEvent *Event=dst->event();
  TClonesArray *ar=dst->femtoArray(4);
  
  StEpdEpInfo information = find->Results( dst->femtoArray(4), Event->primaryVertex(), Event->cent9());
  
  //for west


Int_t nHits = dst->numberOfEpdHits();
for(int ihit=0;ihit<nHits;ihit++)
{

    StFemtoEpdHit *hit= dst->epdHit(ihit);
    nMip_ring->Fill(hit->row());
    if(hit->side()==1)
    {
      
      TVector3 tilecenter=geo->TileCenter(hit->position(), hit->tile(), hit->side());
      TVector3 tilecenter2=tilecenter-Event->primaryVertex();
         Double_t z=geo->TileCenter(hit->position(), hit->tile(), hit->side()).Z()-Event->primaryVertex().Z();
         Double_t cosn=z/tilecenter2.Mag();
         till_center->Fill(tilecenter2.Phi());
     v1[Event->cent9()]->Fill(0.5*TMath::Log((1+cosn)/(1-cosn)),cos(tilecenter2.Phi()-information.EastPhiWeightedAndShiftedPsi(1)),hit->nMIP());
    }
   else 
    {
      
      TVector3 tilecenter=geo->TileCenter(hit->position(), hit->tile(), hit->side());
      TVector3 tilecenter2=tilecenter-Event->primaryVertex();
         Double_t z=geo->TileCenter(hit->position(), hit->tile(), hit->side()).Z()-Event->primaryVertex().Z();
         Double_t cosn=z/tilecenter2.Mag();
        // cout<<0.5*TMath::Log((1+cosn)/(1-cosn))<<endl;
      v1[Event->cent9()]->Fill(0.5*TMath::Log((1+cosn)/(1-cosn)),cos(tilecenter.Phi()-information.WestPhiWeightedAndShiftedPsi(1)),hit->nMIP());
    } 

}
    
}
         

find->Finish();

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
    std::cout << "Usage: ./v1 inputFileName outputFileName.root" << std::endl;
    return -1;
  }
  std::cout << " inputFileName : " << fileName << std::endl;
  std::cout << " outputFileName: " << oFileName << std::endl;

  v1(fileName, oFileName);
  return 1;
}
