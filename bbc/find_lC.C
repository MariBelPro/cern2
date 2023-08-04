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
#include "TH3.h"
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
#include "StFemtoKFP.h"
#include "StFemtoV0.h"
#include "StFemtoXi.h"
#include "StFemtoHelix.h"
#include "StFemtoPhysicalHelix.h"

// Load libraries (for ROOT_VERSTION_CODE >= 393215)
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0)
R__LOAD_LIBRARY(libStFemtoDst.so)
#endif

// Forward declarations
Bool_t isGoodEvent(const TVector3& vtx, const Float_t& vpdVz);
Bool_t isGoodTrack(const TVector3& vect, const Int_t& nHits,
                   const Int_t& nHitsPoss);
Bool_t isMyEvent(const TVector3& vtx);
Bool_t isMyTrack(const TVector3& mom, const Int_t& nHits,
                   const Int_t& nHitsPoss, const Float_t& DCA ) ;
const Float_t electron_mass = 0.0005485799;
const Float_t pion_mass = 0.13957061;
const Float_t kaon_mass = 0.493677;
const Float_t proton_mass = 0.9382720813;

const Float_t electron_mass_sqr = 0.000000301;
const Float_t pion_mass_sqr = 0.019479955;
const Float_t kaon_mass_sqr = 0.24371698;
const Float_t proton_mass_sqr = 0.880354499;
const Float_t ah=0.738;

// inFile - is a name of name.FemtoDst.root file or a name
//          of a name.lis(t) files that contains a list of
//          name1.FemtoDst.root files
//_________________


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
if(bbc_phi<0.0) bbc_phi +=2*pi;
if(bbc_phi>2*pi) bbc_phi -=2*pi;
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

    
if(bbc_phi<0.0) bbc_phi +=2*pi;
if(bbc_phi>2*pi) bbc_phi -=2*pi;

return bbc_phi;
}



void find_lC(const Char_t *inFile = "olo.root",
                      const Char_t *oFileName = "oTest.root") {

  std::cout << "Hi! Lets do some physics, Balda!" << std::endl;

  Int_t runIdBins = 2000000;
  Int_t runIdRange[2] = { 11000000, 13000000 };

 // gSystem->Load("./libStFemtoDst.so");
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
  femtoReader->SetStatus("KFP",1);
  std::cout << "Status has been set" << std::endl;

  std::cout << "Now I know what to read, Master!" << std::endl;

  if( !femtoReader->chain() ) {
    std::cout << "No chain has been found." << std::endl;
  }
  Long64_t eventsInTree = femtoReader->tree()->GetEntries();
  std::cout << "eventsInTree: "  << eventsInTree << std::endl;
  Long64_t events2read = femtoReader->chain()->GetEntries();

  std::cout << "Number of events to read: " << events2read << std::endl;

  
 TFile *profile1 =TFile::Open("Q_vector.root", "READ");
  TProfile *defined_Qye= (TProfile*)profile1->Get("Qye_profile");
  TProfile *defined_Qyw= (TProfile*)profile1->Get("Qyw_profile");
  TProfile *defined_Qxe= (TProfile*)profile1->Get("Qxe_profile");
  TProfile *defined_Qxw= (TProfile*)profile1->Get("Qxw_profile");
  TProfile *defined_Qxtot= (TProfile*)profile1->Get("Qxtotal");
  TProfile *defined_Qytot= (TProfile*)profile1->Get("Qytotal");
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
      TProfile *Totol[10];
     for(int i=0;i<10;i++)
     {
         Totol[i]= (TProfile*)f->Get(Form("tot_flat_%i",i));
     } 
  
   TH3D *Totol_of_lambda= new TH3D ("lambdy_tot","amunt;massa;angle;cent",200,1.105,1.125,10,-3.2,3.2,9,1,10);
   TH3D *Totol_of_Antylambda= new TH3D ("Antylambdy_tot","amunt;massa;angle;cent",200,1.105,1.125,10,-3.2,3.2,9,1,10);
  





     
                
  TH1D *phi[9];
      for(int i=0;i<9; i++)
        {
         phi[i]= new TH1D(Form("phi_for_cent_%i",i ),Form("phi_for_cent_%i",i ),220,-3.5,3.5);
        }
    TH1D *phi_E[9];
      for(int i=0;i<9; i++)
        {
         phi_E[i]= new TH1D(Form("phiE_for_cent_%i",i ),Form("phiE_for_cent_%i",i ),220,-3.5,3.5);
        }  
            TH1D *phi_W[9];
      for(int i=0;i<9; i++)
        {
         phi_W[i]= new TH1D(Form("phiW_for_cent_%i",i ),Form("phiW_for_cent_%i",i ),220,-3.5,3.5);
        }    
     
 

                          
 

  Int_t eventCounter = 0;
  Int_t hundredIter = 0;
  

  // Loop over events

   
  
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

    // Simple event cut
   // if ( !isGoodEvent(pVtx, event->vpdVz() ) ) continue;
    if ( !isMyEvent(pVtx) ) continue;
      
    Double_t pi = TMath::Pi();
Double_t phi_div = pi/6.0;
float bbc_phi = phi_div;
float phi_help=0;
float phi_help2=0;
float phi_help3=0;
    TVector3 q;
    
    TVector3 q_E;
    TVector3 q_W;
    TVector3 Q;
    TVector3 Q_E;
    TVector3 Q_W;
  
     
 
      //East
       for( Int_t iTile=0; iTile<16; iTile++ ) 
       {
       
          bbc_phi=angle_East(iTile);   
q_E.SetX(cos(bbc_phi)*event->bbcAdcEast(iTile));
q_E.SetY(sin(bbc_phi)* event->bbcAdcEast(iTile));
      Q_E+=q_E;
    
     

    }
     
    
    //West
      for( Int_t iTile=0; iTile<16; iTile++ ) 
       {
        
        bbc_phi=angle_West(iTile);


q_W.SetX(cos(bbc_phi)*event->bbcAdcWest(iTile));
q_W.SetY(sin(bbc_phi)* event->bbcAdcWest(iTile));
      Q_W+=q_W;

    }

    Q.SetX(Q_W.X()-Q_E.X()-defined_Qxtot->GetBinContent(event->cent9()+1));
    Q.SetY(Q_W.Y()-Q_E.Y()-defined_Qytot->GetBinContent(event->cent9()+1));
    phi_help=Q.Phi();
   float psi=0;
      for(int i=1;i<10;i++)
      {
          psi+= Totol[i]->GetBinContent(event->cent9()+1)*(-2)*cos(i*Q.Phi())+Totol[i]->GetBinContent(event->cent9()+10)*2*sin(Q.Phi()*i);
      }
      phi_help+=psi;
      phi[event->cent9()]->Fill(phi_help);
      
   
   q.SetX(Q_E.X());
    Q_E.SetX(q.X()-defined_Qxe->GetBinContent(event->cent9()+1));
     q.SetY(Q_E.Y());
    Q_E.SetY(q.Y()-defined_Qye->GetBinContent(event->cent9()+1));

      phi_help2=Q_E.Phi();
     psi=0;
      for(int i=1;i<10;i++)
      {
          psi+= East[i]->GetBinContent(event->cent9()+1)*(-2)*cos(i*Q_E.Phi())+East[i]->GetBinContent(event->cent9()+10)*2*sin(i*Q_E.Phi());
      }
      phi_help2+=psi;
      phi_E[event->cent9()]->Fill(phi_help2);
    q.SetX(Q_W.X());
    Q_W.SetX(q.X()-defined_Qxw->GetBinContent(event->cent9()+1));
     q.SetY(Q_W.Y());
    Q_W.SetY(q.Y()-defined_Qyw->GetBinContent(event->cent9()+1));
    phi_help3=Q_W.Phi();
    psi=0;
      for(int i=1;i<10;i++)
      {
          psi+= West[i]->GetBinContent(event->cent9()+1)*(-2)*cos(i*Q_W.Phi())+West[i]->GetBinContent(event->cent9()+10)*2*sin(i*Q_W.Phi());
      }
      phi_help3+=psi;
       phi_W[event->cent9()]->Fill(phi_help3);
    
       
 

    //find limbda
    
      for(int i=0; i<dst->numberOfKFPs(); i++)
     {
         
       TLorentzVector lambda;
       TLorentzVector proton;
       TLorentzVector anty_lambda;
       TLorentzVector anty_proton;
       TVector3 b;
       TVector3 p;
       TVector3 l;
    

        StFemtoKFP *kfp= dst->kfparticle(i);
        StFemtoKFP *doter;

        //for limbda
        if (kfp->pdg()==3122)
        {
          lambda=kfp->fourMomentum();
            b= lambda.BoostVector();
          
          doter=dst->kfparticle(kfp->daughterIds()[1]);
          if(doter->pdgId()==2212)
          {
            StFemtoPhysicalHelix *proton_hell=new StFemtoPhysicalHelix(doter->p(),doter->decayPoint(),event->magneticField()/pow(10,14),1 );
            Double_t s1=proton_hell->pathLength(kfp->decayPoint());
            proton.SetVectM(proton_hell->momentumAt(s1,event->magneticField()/pow(10,14)),0.938272);
            proton.Boost(-b);
           Totol_of_lambda->Fill(lambda.M(),proton.Phi()-phi_help,event->cent9()+1);
         
          }
          

        }

        //for anty_lambda
        if (kfp->pdg()== -3122)
        {
          anty_lambda=kfp->fourMomentum();
           b= anty_lambda.BoostVector();
          doter=dst->kfparticle(kfp->daughterIds()[0]);
          if(doter->pdgId()==-2212)
          {
            StFemtoPhysicalHelix *proton_hell=new StFemtoPhysicalHelix(doter->p(),doter->decayPoint(),event->magneticField()/pow(10,14),-1 );
            Double_t s1=proton_hell->pathLength(kfp->decayPoint());
            anty_proton.SetVectM(proton_hell->momentumAt(s1,event->magneticField()/pow(10,14)),0.938272);
            anty_proton.Boost(-b);
            Totol_of_Antylambda->Fill(anty_lambda.M(),anty_proton.Phi()-phi_help,event->cent9()+1);
          }
             
            
           
        }
  }
  } //event loop
   TFile *oFile = new TFile(oFileName, "RECREATE");
  oFile->cd();
   Totol_of_Antylambda->Write();
  Totol_of_lambda->Write();
  oFile->Close();


  femtoReader->Finish();

  std::cout << "I'm done with analysis. We'll have a Nobel Prize, Master!"
	    << std::endl;
}
Bool_t isMyEvent(const TVector3& vtx) {
  return ( TMath::Abs( vtx.Z() ) <= 40. );
}

Bool_t isMyTrack(const TVector3& mom, const Int_t& nHits,
                   const Int_t& nHitsPoss, const Float_t& DCA ) 
                   {
  return ( TMath::Abs( mom.Eta() ) <= 1. &&
           TMath::Abs( mom.Pt() )>0.4 &&
           TMath::Abs( DCA ) <=3. 
            &&  nHits >= 15. &&
           (Float_t)nHits/nHitsPoss > 0.52 );

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

  find_lC(fileName, oFileName);
  return 1;
}