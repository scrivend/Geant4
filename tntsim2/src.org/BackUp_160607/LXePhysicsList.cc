//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: LXePhysicsList.cc 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical/LXe/src/LXePhysicsList.cc
/// \brief Implementation of the LXePhysicsList class
//
//
#include "LXePhysicsList.hh"

#include "LXeGeneralPhysics.hh"
#include "LXeEMPhysics.hh"
#include "LXeMuonPhysics.hh"

#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"

#include "G4SystemOfUnits.hh"

//by Shuya 160404
#include "LXeNuclearPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePhysicsList::LXePhysicsList() : G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  defaultCutValue = 1.0*mm;

  // General Physics
  RegisterPhysics( new LXeGeneralPhysics("general") );

  // EM Physics
  RegisterPhysics( new LXeEMPhysics("standard EM"));

  // Muon Physics
  RegisterPhysics( new LXeMuonPhysics("muon"));

  // Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics( opticalPhysics );

  opticalPhysics->SetWLSTimeProfile("delta");

  opticalPhysics->SetScintillationYieldFactor(1.0);
  opticalPhysics->SetScintillationExcitationRatio(0.0);

  opticalPhysics->SetMaxNumPhotonsPerStep(100);
  opticalPhysics->SetMaxBetaChangePerStep(10.0);

  opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
  opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);

//by Shuya 160404
  AddTransportation();
  // Nuclear Physics
  RegisterPhysics( new LXeNuclearPhysics("nuclear"));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePhysicsList::~LXePhysicsList() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePhysicsList::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();

//by Shuya 160406.
//Comment by Shuya 160513. Here em_cuts means produce only particles having a larger range than it (=1mm this case).
  G4double em_cuts = 1.0*mm;
  SetCutValue(em_cuts,"gamma");
  SetCutValue(em_cuts,"e-");
  SetCutValue(em_cuts,"e+");
}
