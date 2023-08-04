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

float angle_East(int i)
{
  Double_t pi = TMath::Pi();
Double_t phi_div = pi/6.0;
float bbc_phi = phi_div;

 
        switch(i) 
        {
case 0: bbc_phi=3*phi_div;
break;
case 1: bbc_phi=phi_div;
break;
case 2: bbc_phi=-1*phi_div;
break;
case 3: bbc_phi=-3*phi_div;
break;
case 4: bbc_phi=-5*phi_div;
break;
case 5: bbc_phi=5*phi_div;
break;
case 6: bbc_phi= 3*phi_div;
break;
case 7: bbc_phi=3*phi_div;
break;
case 8: bbc_phi=phi_div;
break;
case 9: bbc_phi=0.;
break;
case 10: bbc_phi=-phi_div;
break;
case 11: bbc_phi=-3*phi_div;
break;
case 12: bbc_phi=-3*phi_div;
break;
case 13: bbc_phi=-5*phi_div;
break;
case 14: bbc_phi=pi;
break;
case 15: bbc_phi=5*phi_div;
break;
 }

    if (bbc_phi > -0.001)
{ 
    bbc_phi = pi-bbc_phi;
}
else 
{
    bbc_phi= -pi-bbc_phi;
     }
     /*
 double wrap = 2.0*TMath::Pi();
  if (bbc_phi<0.0) bbc_phi += (1.0+(int)(fabs(bbc_phi)/wrap))*wrap;
  else{ if (bbc_phi>wrap) bbc_phi -= ((int)(bbc_phi/wrap))*wrap;}
 // if(bbc_phi>TMath::Pi()) bbc_phi -=2*TMath::Pi();*/
 return bbc_phi;
  
}

float angle_West(int i)
{
  Double_t pi = TMath::Pi();
Double_t phi_div = pi/6.0;
float bbc_phi = phi_div;
switch(i) 
        {
case 0: bbc_phi=3*phi_div;
break;
case 1: bbc_phi=phi_div;
break;
case 2: bbc_phi=-1*phi_div;
break;
case 3: bbc_phi=-3*phi_div;
break;
case 4: bbc_phi=-5*phi_div;
break;
case 5: bbc_phi=5*phi_div;
break;
case 6: bbc_phi= 3*phi_div;
break;
case 7: bbc_phi=3*phi_div;
break;
case 8: bbc_phi=phi_div;
break;
case 9: bbc_phi=0.;
break;
case 10: bbc_phi=-phi_div;
break;
case 11: bbc_phi=-3*phi_div;
break;
case 12: bbc_phi=-3*phi_div;
break;
case 13: bbc_phi=-5*phi_div;
break;
case 14: bbc_phi=pi;
break;
case 15: bbc_phi=5*phi_div;
break;
 }
return bbc_phi;
}




void Raw(const Char_t *inFile = "st_physics_19168040_raw_2000006.femtoDst.root",
                      const Char_t *oFileName = "olo.list") {

  std::cout << "Hi! Lets do some physics, Balda!" << std::endl;

  Int_t runIdBins = 2000000;
  Int_t runIdRange[2] = { 11000000, 13000000 };

  //gSystem->Load("./libStFemtoDst.so");
  #if ROOT_VERSION_CODE < ROOT_VERSION(6,0,0)
    gSystem->Load("../../StFemtoEvent/libStFemtoDst.so");
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

  


   
 
 

   

  
                                                                                                         
 
 TH1D *central= new TH1D ("cent","cent",9,1,10);


  //Histograms
    TH1D *Q_X[9];
      for(int i=0;i<9; i++)
        {
         Q_X[i]= new TH1D(Form("Q_X_for_cent_%i",i ),Form("Q_X_for_cent_%i",i ),100,-30,40);
        } 
         TH1D *Q_Y[9];
      for(int i=0;i<9; i++)
        {
         Q_Y[i]= new TH1D(Form("Q_Y_for_cent_%i",i ),Form("Q_Y_for_cent_%i",i ),100,-30,40);
        } 
     TH1D *phi[9];
      for(int i=0;i<9; i++)
        {
         phi[i]= new TH1D(Form("#phi_{0} for centrality %i",i ),Form("#phi_{0} for centrality bin %i",i ),100,-3.5,3.5);
         phi[i]->SetTitle(";#phi;");
        }  

  //Profiles                     
   TProfile *Qxe_profile= new TProfile ("Qxe_profile","Qxe_profile",9,1,10);  
    TProfile *Qye_profile= new TProfile ("Qye_profile","Qye_centrelity",9,1,10);  
    TProfile *Qyw_profile= new TProfile ("Qyw_profile","Qyw_centrelity",9,1,10);  
    TProfile *Qxw_profile= new TProfile ("Qxw_profile","Qxw_centrelity",9,1,10);
    TProfile *Qxtotal= new TProfile ("Qxtotal","Qxtotal",9,1,10);    
     TProfile *Qytotal= new TProfile ("Qytotal","Qytotal",9,1,10);  
TProfile *besit= new TProfile ("sdcd","svfv",9,1,10);
                                                                                                         
     

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


    TVector3 q_E;
    TVector3 q_W;
    TVector3 Q;
    TVector3 Q_E;
    TVector3 Q_W;
    float bbc_phi;
      //East
     
       for( Int_t iTile=0; iTile<16; iTile++ ) 
       {
       bbc_phi= angle_East(iTile);
       if(bbc_phi<0) cout<<bbc_phi<<endl;
q_E.SetX(cos(bbc_phi)*event->bbcAdcEast(iTile));
q_E.SetY(sin(bbc_phi)* event->bbcAdcEast(iTile));
      Q_E+=q_E;
       }

    //West
      for( Int_t iTile=0; iTile<16; iTile++ ) 
       {
        bbc_phi= angle_West(iTile);

q_W.SetX(cos(bbc_phi)*event->bbcAdcWest(iTile));
q_W.SetY(sin(bbc_phi)* event->bbcAdcWest(iTile));
      Q_W+=q_W;
    }

//Set totol vector
    Q.SetX(Q_W.X()-Q_E.X());
    Q.SetY(Q_W.Y()-Q_E.Y());
    

  //Filling histograms      
       Q_X[event->cent9()]-> Fill(Q_W.X()-Q_E.X());
       Q_Y[event->cent9()]-> Fill(Q_W.Y()-Q_E.Y());
       phi[event->cent9()]-> Fill(Q.Phi());
     //  if(Q.Phi()>3.14) cout<<Q.Phi()<<endl;
       Qye_profile-> Fill(event->cent9()+1, Q_E.Y());
       Qyw_profile-> Fill(event->cent9()+1, Q_W.Y());
       Qxe_profile-> Fill(event->cent9()+1, Q_E.X());
       Qxw_profile-> Fill(event->cent9()+1, Q_W.X());
      Qxtotal->Fill(event->cent9()+1, Q.X()); 
      Qytotal->Fill(event->cent9()+1, Q.Y()); 
      besit->Fill(event->cent9()+1,Q.Phi());
    central->Fill(event->cent9()+1);
   
  } //for(Long64_t iEvent=0; iEvent<events2read; iEvent++)
  

  TFile *oFile = new TFile(oFileName, "RECREATE");
  oFile->cd();

   for(int i=0;i<9; i++)
        {
         phi[i]->Write();
          Q_X[i]->Write();
          Q_Y[i]->Write();
        }  



  femtoReader->Finish();

  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!"
	    << std::endl;
}