// This is needed for calling standalone classes (not needed on RACF)
#ifndef _VANILLA_ROOT_
#define _VANILLA_ROOT_
#endif
// C++ headers
#include <iostream>
#include <string>
#include <vector>
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
#include "../../StFemtoEvent/StFemtoDstReader.h"
#include "../../StFemtoEvent/StFemtoDst.h"
#include "../../StFemtoEvent/StFemtoEvent.h"
#include "../../StFemtoEvent/StFemtoTrack.h"
#include "../../StFemtoEvent/StFemtoKFP.h"
#include "../../StFemtoEvent/StFemtoV0.h"
#include "../../StFemtoEvent/StFemtoXi.h"
#include "../../StFemtoEvent/StFemtoEpdHit.h"
// StEpdUtil headers
#include "../../StEpdUtil/StEpdEpFinder.h"
#include "../../StEpdUtil/StEpdEpInfo.h"



  



R__LOAD_LIBRARY(../../StFemtoEvent/libStFemtoDst.so)
R__LOAD_LIBRARY(../../StEpdUtil/libStEpdUtil.so)
void epd(const Char_t *inFile = "/home/mary/Desktop/NIRS/EPD/StEpd/Macros/st_physics_20154010_raw_2000001.femtoDst.root",
const Char_t *oFileName = "oTest.root")
{

StFemtoDstReader* femtoReader = new StFemtoDstReader(inFile);
femtoReader->Init();
femtoReader->SetStatus("*",0);
femtoReader->SetStatus("Event",1);
femtoReader->SetStatus("Track",1);
femtoReader->SetStatus("KFP",1);
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


/*
 Double_t lin[9] ={-0.135261, -0.198461, -0.298412, -0.392054, -0.440712, -0.419807, -0.314728, -0.164544, -0.0500619};
  Double_t cub[9]={ 0.00831431, 0.0165188, 0.0276656, 0.0390844, 0.0469043, 0.0473444, 0.0370741, 0.0201307, 0.00671543};
TH2D wt ("Order1etaWeight","Order1etaWeight",100,1.5,6.5,9,0,9);
  for (int ix=1; ix<101; ix++){
    for (int iy=0; iy<9; iy++){
      double eta = wt.GetXaxis()->GetBinCenter(ix);
      wt.SetBinContent(ix,iy+1,lin[iy]*eta+cub[iy]*pow(eta,3));
    }
  }
*/


StEpdEpFinder *find= new StEpdEpFinder(9,oFileName);

find->SetEpdHitFormat(2);
find->SetnMipThreshold(0.3);
find->SetMaxTileWeight(1);

//find->SetEtaWeights(1,wt);

TH1D *phiRaw =new TH1D("phiR",";#phi (radian)",100,-3,3);
TH1D *phiWeigh =new TH1D("phiW",";#phi (radian)",100,-3,3);
TH1D *phiWeightedShifted =new TH1D("phiTot",";#phi (radian)",100,0-3,3);
TH1D *phiWeightedShiftedEast =new TH1D("phiEst",";#phi (radian)",100,-3,3);
TH1D *phiWeightedShiftedWest =new TH1D("phiWst",";#phi (radian)" ,100,-3,3);
TProfile *cn= new TProfile("cos",";centrality bin;#LT cos(#psi_{West}-#psi_{East}) #GT",9,-0.5,8.5);
TH3D *Totol_of_lambda= new TH3D ("lambdy_tot","amunt;massa;angle;cent",200,1.105,1.125,10,-3.14,3.14,9,-0.5,8.5);
TH3D *Totol_of_Antylambda= new TH3D ("Antylambdy_tot","amunt;massa;angle;cent",200,1.105,1.125,10,-3.14,3.14,9,-0.5,8.5);


TProfile *mid_sin=new TProfile("mid_sin","mid_sin",9,-0.5,8.5);
TProfile *mid_sin_anty=new TProfile("mid_sin_anty","mid_sin_anty",9,-0.5,8.5);
Int_t hundredIter = 0;
Int_t eventCounter = 0;

for (Long64_t iEvent=0; iEvent<events2read; iEvent++) //loop over events
{


Bool_t readEvent = femtoReader->readFemtoEvent(iEvent);
StFemtoDst *dst = femtoReader->femtoDst();
StFemtoEvent *Event=dst->event();
TClonesArray *ar=dst->femtoArray(4);
eventCounter++;
  if( eventCounter >= 100000 ) {
  eventCounter = 0;
  hundredIter++;
  std::cout << "Working on event #[" << (hundredIter * 100000)
  << "/" << events2read << "]" << std::endl;
  }

cout<<Event->cent16()<<endl;
//StEpdEpInfo information = find->Results( dst->femtoArray(4), Event->primaryVertex(), Event->cent9());

//cout<<information.WestPhiWeightedAndShiftedPsi(1)<<" "<<information.EastPhiWeightedAndShiftedPsi(1)<<endl;




 /*
phiRaw->Fill(information.FullRawPsi(1));
phiRaw->SetTitle("Raw");
phiWeigh->SetTitle("Weight");
phiWeightedShifted->SetTitle("shift");
phiWeigh->Fill(information.FullPhiWeightedPsi(1));
phiWeightedShifted->Fill(information.FullPhiWeightedAndShiftedPsi(1));
phiWeightedShiftedEast->Fill(information.EastPhiWeightedAndShiftedPsi(1));
phiWeightedShiftedWest->Fill(information.WestPhiWeightedAndShiftedPsi(1));

*/
 
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
          lambda.SetVect(kfp->p());
          lambda.SetE(sqrt((kfp->p())*(kfp->p())+(kfp->mass())*(kfp->mass())));
          //cout<<lambda.M()<<" "<<kfp->mass()<<endl;
            b= lambda.BoostVector();
          
          doter=dst->kfparticle(kfp->daughterIds()[1]);
          if(doter->pdgId()==2212)
          {
           proton.SetVect(doter->p());
          proton.SetE(sqrt((doter->p())*(doter->p())+(doter->mass())*(doter->mass())));
          proton.Boost(-b);
          // Totol_of_lambda->Fill(lambda.M(),proton.Vect().Phi()-information.FullPhiWeightedAndShiftedPsi(1),Event->cent9());
          // cn->Fill(Event->cent9(), (proton.Phi()-information.EastPhiWeightedAndShiftedPsi(1)));
          // mid_sin->Fill(Event->cent9(),sin(proton.Phi()));
          }
          

        }

        //for anty_lambda
        if (kfp->pdg()== -3122)
        {
          anty_lambda.SetVect(kfp->p());
          anty_lambda.SetE(sqrt((kfp->p())*(kfp->p())+(kfp->mass())*(kfp->mass())));
           b= anty_lambda.BoostVector();
          doter=dst->kfparticle(kfp->daughterIds()[1]);
           anty_proton.SetVect(doter->p());
          anty_proton.SetE(sqrt((doter->p())*(doter->p())+(doter->mass())*(doter->mass())));
           anty_proton.Boost(-b);   
           // Totol_of_Antylambda->Fill(anty_lambda.M(),anty_proton.Vect().Phi()-information.FullPhiWeightedAndShiftedPsi(1),Event->cent9());
          // mid_sin_anty->Fill(Event->cent9(),anty_proton.Vect().Phi()-information.FullPhiWeightedAndShiftedPsi(1));
        

           
        }
  } //loop over V0





}

find->Finish();


}
