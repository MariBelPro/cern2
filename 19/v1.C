
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


 Double_t lin[9] ={ -0.00282782, -0.00405769, -0.00546768, -0.00643697, -0.00661555, -0.00585217, -0.00398546, -0.00188664, -0.000526342};
  Double_t cub[9]={ 0.000275531, 0.000433643, 0.000629528, 0.000797496, 0.0008821, 0.000841984, 0.000622294, 0.000320302, 0.000100225};
TH2D wt ("Order1etaWeight","Order1etaWeight",100,1.5,6.5,9,0,9);
  for (int ix=1; ix<101; ix++){
    for (int iy=0; iy<9; iy++){
      double eta = wt.GetXaxis()->GetBinCenter(ix);
      wt.SetBinContent(ix,iy+1,lin[iy]*eta+cub[iy]*pow(eta,3));
    }
  }

StEpdEpFinder *find= new StEpdEpFinder(9,oFileName);
find->SetEpdHitFormat(2);
find->SetnMipThreshold(0.3);
find->SetMaxTileWeight(1);
find->SetEtaWeights(1,wt);
TH1D *nMip_ring= new TH1D("nMip","; ring nomber",16,1,16);
TH1D *n_eta= new TH1D("eta","eta",10,-5,5);
TProfile *v1[9];
TProfile *v1_TPC[9];
for(int i=0;i<9; i++)
{
v1[i]= new TProfile (Form("for_centrality_bin_%d",i),Form("for_centrality_bin_%d",i),20,-5,5);
v1_TPC[i]= new TProfile (Form("tpc_for_centrality_bin_%d",i),Form("for_centrality_bin_%d",i),12,-1,1);
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

}//epd_hit
Int_t nTracks = dst->numberOfTracks();
 for(Int_t iTrk=0; iTrk<nTracks; iTrk++) 
    {
      StFemtoTrack *femtoTrack = dst->track(iTrk);

      
        
         if ( !femtoTrack->isPrimary() ) continue;
         if(femtoTrack->eta()>0) 
          {
           v1_TPC[Event->cent9()]->Fill(femtoTrack->eta(),cos(femtoTrack->phi()-information.EastPhiWeightedAndShiftedPsi(1)));
           
           }
          else if(femtoTrack->eta()<0) 
          {
          
            v1_TPC[Event->cent9()]->Fill(femtoTrack->eta(),cos(femtoTrack->phi()-information.WestPhiWeightedAndShiftedPsi(1)));
           }
    
    }//over tracks
    
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
