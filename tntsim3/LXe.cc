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
// $Id: LXe.cc 77782 2013-11-28 08:12:12Z gcosmo $
//
/// \file optical/LXe/LXe.cc
/// \brief Main program of the optical/LXe example
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4String.hh"

#include "LXePhysicsList.hh"
#include "LXeDetectorConstruction.hh"

#include "LXeActionInitialization.hh"

#include "LXeRecorderBase.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//by Shuya 160406
#include "LXeDataRecordTree.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//by Shuya 160421. I added this to count the Event Number instead of EventID. 
//Because this code is multithreading, GetEventID gives a random (not ordered) event number, which is inconvenient to create Root File in LXeDataRecord.cc.
  G4int Counter = 0;

//by Shuya 160502. I added this to count the total number of photons created in one event (including Detectgor, housing, etc). 
  G4int NumOfCreatedPhotons = 0;

//by Shuya 160509. 
//NOTE!!!! You have to change the parameters for PmtBackHit,PmtFronHit arrays in LXeDataRecord.hh
  G4int NX = 4;
  G4int NY = 4;

int main(int argc, char** argv)
{



//by Shuya 160421. All copied from tntsim.cc
//  G4int numberOfEvent = 10;
  // Set VerboseFlag to "1" to run in verbose mode
  G4int VerboseFlag = 1;
  // Set VisFlag to "1" to run in visualization mode
  G4int VisFlag = 0;
  //**** Note! You should run fewer events (such as < 1000) when using the viewer!
  G4String VisType = "OPENGL";   // Set vis type. OPENGL works in UBUNTU 12.04 and later
   //G4String VisType = "VRMLVIEW";
  G4int SaveVisFile = 0;         // Set to 1 to save OPENGL picture to G4OpenGL.eps file


//by Shuya 160407
  G4double Det_Threshold = -0.5*MeV;  // Det. Threshold in MeVee

#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
//by Shuya 160502. If you don't want multi-threading, use this.
 runManager->SetNumberOfThreads(1);
#else
  G4RunManager * runManager = new G4RunManager;
#endif


//by Shuya 160407
  // Possible LightConv Settings:
  // "none" - no light conversion - get only energy deposited
  // "light-conv" - convert energy dep. to light following method of Cecil et al.
  // "light+resol" - convert to light with Cecil's formula and Dekempner's resolution 
  G4String LightConv = "light+resol";

  runManager->SetUserInitialization(new LXeDetectorConstruction(LightConv));
  runManager->SetUserInitialization(new LXePhysicsList());

  LXeRecorderBase* recorder = NULL; //No recording is done in this example

  runManager->SetUserInitialization(new LXeActionInitialization(recorder));

//by Shuya 160407
  // Generate Analysis Pointer Class
  LXeDataRecordTree* LXePointer = new LXeDataRecordTree(Det_Threshold);
  cout << LXePointer << endl;

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // runManager->Initialize();
 
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if(argc==1){
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
#endif
    if (ui->IsGUI())
       UImanager->ApplyCommand("/control/execute gui.mac");
    ui->SessionStart();
    delete ui;
#endif
/*
		G4String command  = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
*/
  }
  else{
    G4String command = "/control/execute ";
    G4String filename = argv[1];
    UImanager->ApplyCommand(command+filename);
  }

///*
		if (VisFlag == 1)
    {
      if(VisType == "VRMLVIEW")
			{
				// commands to open vrmlview file
				UImanager->ApplyCommand("/vis/scene/create");
				UImanager->ApplyCommand("/vis/open VRML2FILE");
				UImanager->ApplyCommand("/tracking/storeTrajectory 1");
				UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
				UImanager->ApplyCommand("/vis/scene/add/trajectories");
			}
			else if(VisType == "OPENGL")
			{
				UImanager->ApplyCommand("/vis/open OGL 900x600-0+0");
				//UI->ApplyCommand("/vis/open OGLI 900x600-0+0");
				//UI->ApplyCommand("/vis/open DAWNFILE");
				UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
				//UI->ApplyCommand("/vis/viewer/set/background red ! ! 0.2");
				UImanager->ApplyCommand("/vis/viewer/set/background 0.5 0.5 0.5 0.1");
				UImanager->ApplyCommand("/vis/verbose errors");
				UImanager->ApplyCommand("/vis/drawVolume");
				UImanager->ApplyCommand("/vis/viewer/set/viewpointVector -1 0 0");
				UImanager->ApplyCommand("/vis/viewer/set/lightsVector -1 0 0");
				UImanager->ApplyCommand("/vis/viewer/set/style wireframe");
				//UI->ApplyCommand("/vis/viewer/set/style surface");
				// UI->ApplyCommand("/vis/viewer/set/auxiliaryEdge true");
				UImanager->ApplyCommand("/vis/viewer/set/lineSegmentsPerCircle 100");
				UImanager->ApplyCommand("/tracking/storeTrajectory 1");
				UImanager->ApplyCommand("/vis/scene/endOfEventAction accumulate");
				UImanager->ApplyCommand("/vis/scent/endOfEventAction accumulate 2000"); // view more events!
				UImanager->ApplyCommand("/vis/scene/add/trajectories");
				UImanager->ApplyCommand("/vis/modeling/trajectories/create/drawByCharge");
				UImanager->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true");
				UImanager->ApplyCommand("/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 2");
				//UI->ApplyCommand("/vis/scene/add/hits");
				UImanager->ApplyCommand("/vis/set/textColour green");
				UImanager->ApplyCommand("/vis/set/textLayout right");
				UImanager->ApplyCommand("/vis/scene/add/text2D 0.9 -.9 24 ! ! Demon Simulation"); 
				UImanager->ApplyCommand("/vis/scene/add/text 0 10 35 cm 18 10 10 Det Module");
				UImanager->ApplyCommand("/vis/scene/add/scale 25 cm");   //Simple scale line
				//UI->ApplyCommand("/vis/scene/add/axes");    //Simple axes: x=red, y=green, z=blue.
				UImanager->ApplyCommand("/vis/scene/add/logo2D");  //Simple logo
				//UI->ApplyCommand("/vis/geometry/set/visibility Envelope 0 true");	
				//UI->ApplyCommand("/vis/viewer/set/hiddenMarker true");

				// Set viewing angle!
				//UI->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 135 150");
				UImanager->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 90 180");
				//UI->ApplyCommand("/vis/viewer/set/viewpointThetaPhi 135 135");


				UImanager->ApplyCommand("/vis/viewer/pan -10 0 cm");

				UImanager->ApplyCommand("/vis/viewer/zoom 0.5");  // Zoom in, > 1, Zoom out < 1	
			}
    }
		else
    {//No Visualization! }

		//runManager->BeamOn(numberOfEvent);

		if(VisType == "OPENGL" && SaveVisFile == 1)
    {
			// print vis output
      UImanager->ApplyCommand("/vis/ogl/set/printMode pixmap"); // note: vectored mode crashes! 
      UImanager->ApplyCommand("/vis/ogl/set/printSize 900 600"); 
      UImanager->ApplyCommand("/vis/ogl/printEPS"); 
      G4cout << "OPENGL Visual Scene saved to G4OpenGL_x.eps. " << G4endl;
    }
	}
//*/

//  if(recorder)delete recorder;

//by Shuya 160407
  LXePointer->GetParticleTotals();
  delete LXePointer;

#ifdef G4VIS_USE
  delete visManager;
#endif

  // job termination
  delete runManager;
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
