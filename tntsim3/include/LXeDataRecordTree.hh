//-----------------------------------------------------------------
// TntDataRecordTree.hh
//
// - written by: Brian Roeder, LPC Caen, 18 Jan 07 (in Serra)
// - email - roeder@lpccaen.in2p3.fr
//
// - modified: 14/02/07 for use with Tnt Tracking
//
// - Usage: A C++ class for GEANT4 for creating a ROOT tree for 
// -        the Tnt neutron detector simulation event by event.
//
///////////////////////////////////////////////////////////////////
//
//

#ifndef DATARECORD_H
#define DATARECORD_H

#include <iostream>
using namespace std;

// C++ formatting and file io headers

#include <iomanip>
#include <fstream>

// Root Analysis header files for C++

#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
#include "TH1.h"
#include "TNtuple.h"
#include "TTree.h"
#include "globals.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//by Shuya 160407
#include "LXeDataRecordTree.hh"
#include "TH2.h"


class LXeDataRecordTree
{

private:

  // Initialized in class constructor in TntDataRecordTree.cc 
  
  TFile* DataFile;
  TTree* LXeEventTree;
//by Shuya 160422.
  TTree* LXeEventTree2;
  G4double eng_int;
  G4double eng_LXe;
  G4double eng_LXe_proton;
  G4double eng_LXe_alpha;
  G4double eng_LXe_C12;
  G4double eng_LXe_EG;
  G4double eng_LXe_Exotic;
//by Shuya 160407
  G4int eng_LXe_PhotonFront;
  G4int eng_LXe_PhotonBack;
//by Shuya 160502
  G4int eng_LXe_PhotonTotal;
//by Shuya 160502
  G4double edep_LXe;
  G4double edep_LXe_proton;
  G4double edep_LXe_alpha;
  G4double edep_LXe_C12;
  G4double edep_LXe_EG;
  G4double edep_LXe_Exotic;
//by Shuya 160504
  G4double num_LXe_NonPMT;
  G4double num_LXe_Abs;
//by Shuya 160422
  G4int PmtFrontHit[4][4];
  G4int PmtBackHit[4][4];
  //std::vector<std::vector<int> > PmtFrontHit;
  //std::vector<std::vector<int> > PmtBackHit;
//by Shuya 160509. This is needed since array size is not passed until you get extern G4int in LXeDataRecord(), which is originally fixed in LXe.cc file.
  //G4int** PmtFrontHit;
  //G4int** PmtBackHit;


  G4double FirstHitTime;
  G4double FirstHitMag;
//by Shuya 160502
  G4ThreeVector FirstHitPosition;

  G4double Xpos;
  G4double Ypos;
  G4double Zpos;

  G4double Det_Threshold; // Threshold for Detector in MeVee

  // Particle Counters

  int event_counter;
  int number_total;
  int number_protons;
  int number_alphas;
  int number_C12;
  int number_EG;
  int number_Exotic;
//by Shuya 160407
  int number_Photon;

  // Efficiency Calculators
  int number_at_this_energy;
  double efficiency;


  
 public:
    LXeDataRecordTree(G4double Threshold);
   ~LXeDataRecordTree();

   TH1F *h_Energy_Initial;  
   TH1D *h_Energy_LXe;  
   TH1F *h_Energy_Proton;  
   TH1F *h_Energy_Alpha;  
   TH1F *h_Energy_C12;  
   TH1F *h_Energy_EG;  
   TH1F *h_Energy_Exotic;  
   TH1F *h_Energy_Photon;  
   //TH2I *h_Count_PMT_Front; 
   //TH2I *h_Count_PMT_Back; 
   vector<TH2I*> h_Count_PMT_Front; 
   vector<TH2I*> h_Count_PMT_Back; 


 // Creates Pointer to Data Analysis in main() program (Tnt.cc).    
  static LXeDataRecordTree* LXePointer;
 
  // Definition of member functions - access and distribute data
  // See "TntDataRecordTree.cc for the function listings.

//by Shuya 160408
  //void senddataPMT(int id, double value1);
//by Shuya 160421
  void senddataPMT(int id, int value1, int evid);
  void createdataPMT(int evid);

  void senddataPG(double value1);
  void senddataEV(int type, double value1);
  void senddataPosition(G4ThreeVector pos);
  void senddataTOF(G4double time);
  void ShowDataFromEvent();
  void FillTree();
//by Shuya 160422.
  void FillTree2(int evid);
  void GetParticleTotals();
  void CalculateEff(int ch_eng);

private:
  LXeDataRecordTree() {;}   // Hide Default Constructor
}; 
#endif
