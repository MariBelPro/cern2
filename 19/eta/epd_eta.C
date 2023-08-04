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
#include "StFemtoDstReader.h"
#include "StFemtoDst.h"
#include "StFemtoEvent.h"
#include "StFemtoTrack.h"
#include "StFemtoKFP.h"
#include "StFemtoV0.h"
#include "StFemtoXi.h"
#include "StFemtoEpdHit.h"
#include "StFemtoHelix.h"
#include "StFemtoPhysicalHelix.h"
// StEpdUtil headers
#include "StEpdEpFinder.h"
#include "StEpdEpInfo.h"


double Range(double psi){
 
  double wrap = 2.0*TMath::Pi();
  if (psi<0.0) psi += (1.0+(int)(fabs(psi)/wrap))*wrap;
  else{ if (psi>wrap) psi -= ((int)(psi/wrap))*wrap;}
 
  return psi;
  
}
  



R__LOAD_LIBRARY(libStFemtoDst.so)
R__LOAD_LIBRARY(libStEpdUtil.so)
void epd_eta(const Char_t *inFile = "olo.list",
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




 Double_t lin[9] ={ -0.355884, -0.399569, -0.543481, -0.669422, -0.634701, -0.57688, -0.377199, -0.171404, -0.0460565};
  Double_t cub[9]={ 0.0286707, 0.0433385, 0.0620242, 0.0817873, 0.0863651, 0.082664, 0.0611622, 0.0316034, 0.00874059};
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
Double_t angle;
TH1D *phiRaw =new TH1D("phiR",";#phi (radian)",100,-3,3);
TH1D *phiWeigh =new TH1D("phiW",";#phi (radian)",100,-3,3);
TH1D *phiWeightedShifted =new TH1D("phiTot",";#phi (radian)",100,0-3,3);
TH1D *phiWeightedShiftedEast =new TH1D("phiEst",";#phi (radian)",100,-3,3);
TH1D *phiWeightedShiftedWest =new TH1D("phiWst",";#phi (radian)" ,100,-3,3);
TProfile *cn= new TProfile("cos",";centrality bin;#LT cos(#psi_{West}-#psi_{East}) #GT",9,-0.5,8.5);
TH3D *Totol_of_lambda= new TH3D ("lambdy_tot","amunt;massa;angle;cent",200,1.105,1.125,10,-3,3,5,-1,1);
TH3D *Totol_of_Antylambda= new TH3D ("Antylambdy_tot","amunt;massa;angle;cent",200,1.105,1.125,10,-3,3,5,-1,1);


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
  
if(Event->cent9()!=6) continue;
   

StEpdEpInfo information = find->Results( dst->femtoArray(4), Event->primaryVertex(), Event->cent9());

//cout<<information.WestPhiWeightedAndShiftedPsi(1)<<" "<<information.EastPhiWeightedAndShiftedPsi(1)<<endl;




 
 

phiRaw->Fill(information.FullRawPsi(1));
phiRaw->SetTitle("Raw");
phiWeigh->SetTitle("Weight");
phiWeightedShifted->SetTitle("shift");
phiWeigh->Fill(information.FullPhiWeightedPsi(1));
phiWeightedShifted->Fill(information.FullPhiWeightedAndShiftedPsi(1));
phiWeightedShiftedEast->Fill(information.EastPhiWeightedAndShiftedPsi(1));
phiWeightedShiftedWest->Fill(information.WestPhiWeightedAndShiftedPsi(1));

cn->Fill(Event->cent9(),cos(information.WestPhiWeightedAndShiftedPsi(1)-information.EastPhiWeightedAndShiftedPsi(1)));

 
 for(int i=0; i<dst->numberOfKFPs(); i++)
     {
         
       TLorentzVector lambda;
       TLorentzVector proton;
       TLorentzVector anty_lambda;
       TLorentzVector anty_proton; 
       TVector3 b;
      
    

        StFemtoKFP *kfp= dst->kfparticle(i);
        StFemtoKFP *doter_p;
       
      
        //for limbda
        if (kfp->pdg()==3122)
        {
          lambda=kfp->fourMomentum();
            b= lambda.BoostVector();
          
          doter_p=dst->kfparticle(kfp->daughterIds()[1]);
         
          if(doter_p->pdgId()==2212)
          {
            StFemtoPhysicalHelix *proton_hell=new StFemtoPhysicalHelix(doter_p->p(),doter_p->decayPoint(),Event->magneticField()/pow(10,14),1);
            Double_t s1=proton_hell->pathLength(kfp->decayPoint());
            proton.SetVectM(proton_hell->momentumAt(s1,Event->magneticField()/pow(10,14)),0.938272);
            proton.Boost(-b);
           Totol_of_lambda->Fill(kfp->fourMomentum().M(), information.FullPhiWeightedAndShiftedPsi(1)-proton.Phi(),kfp->eta());
           
          }
          

        }

        //for anty_lambda
        if (kfp->pdg()== -3122)
        {
          anty_lambda=kfp->fourMomentum();
           b= anty_lambda.BoostVector();
          doter_p=dst->kfparticle(kfp->daughterIds()[0]);
          if(doter_p->pdgId()==-2212)
          {
            StFemtoPhysicalHelix *proton_hell=new StFemtoPhysicalHelix(doter_p->p(),doter_p->decayPoint(),Event->magneticField()/pow(10,14),-1 );
            Double_t s1=proton_hell->pathLength(kfp->decayPoint());
            anty_proton.SetVectM(proton_hell->momentumAt(s1,Event->magneticField()/pow(10,14)),0.938272);
            anty_proton.Boost(-b);
            Totol_of_Antylambda->Fill(kfp->fourMomentum().M(),information.FullPhiWeightedAndShiftedPsi(1)-anty_proton.Phi(),kfp->eta());
          }
        

           
        }
  } //loop over V0





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
    std::cout << "Usage: ./epd inputFileName outputFileName.root" << std::endl;
    return -1;
  }
  std::cout << " inputFileName : " << fileName << std::endl;
  std::cout << " outputFileName: " << oFileName << std::endl;

  epd_eta(fileName, oFileName);
  return 1;
}