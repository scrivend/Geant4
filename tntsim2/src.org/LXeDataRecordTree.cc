//----------------------------------------------------------------
// TntDataRecordTree.cc
//
// - written by: Brian Roeder, LPC Caen, 18 Jan 07
// - email - roeder@lpccaen.in2p3.fr
//
// - Usage: A C++ class for GEANT4 for creating a ROOT tree for 
// -        the Tnt neutron detector simulation event by event.
//
///////////////////////////////////////////////////////////////////
//
// 19 Jan 07 - For methods and functions associated with TntDataRecordTree
//           - pointers, see TntDataRecordTree.hh 
//
// 25 Apr 08 - Detector Threshold is now set in main() of Tnt_EMIS.cc
// 
// 
//
 
#include "LXeDataRecordTree.hh"

//by Shuya 160426.
#include <vector>
#ifdef __MAKECINT__
#pragma link C++ class vector<vector<double> >+;
#endif

//by Shuya 160509
  extern G4int NX;
  extern G4int NY;

// Access to Analysis pointer! (see TntSD.cc EndOfEvent() for Example)
LXeDataRecordTree* LXeDataRecordTree::LXePointer;

LXeDataRecordTree::LXeDataRecordTree(G4double Threshold) : 
  // Initialized Values
  eng_int(0), eng_LXe(0), eng_LXe_alpha(0), eng_LXe_C12(0), 
  eng_LXe_EG(0), eng_LXe_Exotic(0),  FirstHitTime(0), FirstHitMag(0),
//by Shuya 160407
  eng_LXe_PhotonFront(0), eng_LXe_PhotonBack(0), eng_LXe_PhotonTotal(0), number_Photon(0),
  Xpos(0), Ypos(0), Zpos(0), Det_Threshold(Threshold),
  event_counter(0), number_total(0), 
  number_protons(0), number_alphas(0), number_C12(0), number_EG(0), 
  number_Exotic(0), number_at_this_energy(0), efficiency(0),
//by Shuya 160502
  eng_LXe_proton(0), edep_LXe(0), edep_LXe_proton(0), edep_LXe_alpha(0), edep_LXe_C12(0), edep_LXe_EG(0), edep_LXe_Exotic(0),
//by Shuya 160504
  num_LXe_NonPMT(0), num_LXe_Abs(0)
{ /* Constructor */ 
  LXePointer = this;  // When Pointer is constructed, assigns address of this class to it.
  //
  // Create new data storage text file
  // Create new text file for data storage - (Data Recorded by TntDataRecordTree class)
 
  char EffFile[] = "eff_results_file.dat";
  ofstream outfile2(EffFile,ios::trunc);
  outfile2.close();

//by Shuya 160422.
/*
PmtFrontHit.resize(NX);
for(int i=0;i<NX;i++)	PmtFrontHit[i].resize(NY);
PmtBackHit.resize(NX);
for(int i=0;i<NX;i++)	PmtBackHit[i].resize(NY);
*/


//by Shuya 160418? Initialization just in case...
//by Shuya 160509
//Comment by Shuya 160510. I tried to make a 2D variable vector as below, but this didn't give right values when you filled in TBranch.. (don't know why)
//So I have to set the size of array in LXeDataRecord.hh so far...
  //PmtFrontHit = new G4int* [NX];
  for(int i=0;i<NX;i++)
  {
  	//PmtFrontHit[i] = new G4int [NY];
	for(int j=0;j<NY;j++)
	{
		PmtFrontHit[i][j] = 0;
		//G4cout << "TEST PMTFRONTHIT ADDRESS " << &PmtFrontHit[i][j] << G4endl;
	}
  }

  //PmtBackHit = new G4int* [NX];
  for(int i=0;i<NX;i++)
  {
  	//PmtBackHit[i] = new G4int [NY];
	for(int j=0;j<NY;j++)
	{
		PmtBackHit[i][j] = 0;
		//G4cout << "TEST PMTFRONTHIT ADDRESS " << &DAMMY[i][j] << G4endl;
	}
  }

  //
  // Create pointers to ROOT Analysis Tree and Branches
  //
  cout <<"\n Starting Data Tree Constructor" << endl;
  
  const Char_t* evt_file = "LXeDataTree.root";

  DataFile = new TFile(evt_file, "RECREATE");
//G4cout << DataFile << "!!" << G4endl;

//////////////////////////// BY Shuya 160407 TO CHANGE TREE TO HISTOGRAMS IN ROOT ////////////////////////////////////
//TFile *foutR = new TFile(rootFileName,"RECREATE");
if ( DataFile->IsOpen() )
{
        printf("root file opened for writing.\n");
}
else
{
        printf("root file didn't open.\n");
        exit (1);
}

printf("Output the data files\n");

//By Shuya 160426. I removed all these histograms to replace TTree. 
/*
TH1F *h_Energy_Initial = new TH1F("Energy_Initial","Energy_Initial", 500, 0, 5000);
h_Energy_Initial->GetXaxis()->SetTitle("Energy (keV)");
h_Energy_Initial->GetYaxis()->SetTitle("Counts / bin");

TH1D *h_Energy_LXe = new TH1D("Energy_LXe","Energy_LXe", 500, 0, 1.);
h_Energy_LXe->GetXaxis()->SetTitle("Energy (keV)");
h_Energy_LXe->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Energy_Proton = new TH1F("Energy_Proton","Energy_Proton", 500, 0, 1.);
h_Energy_Proton->GetXaxis()->SetTitle("Energy (keV)");
h_Energy_Proton->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Energy_Alpha = new TH1F("Energy_Alpha","Energy_Alpha", 500, 0, 1.);
h_Energy_Alpha->GetXaxis()->SetTitle("Energy (keV)");
h_Energy_Alpha->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Energy_C12 = new TH1F("Energy_C12","Energy_C12", 500, 0, 1.);
h_Energy_C12->GetXaxis()->SetTitle("Energy (keV)");
h_Energy_C12->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Energy_EG = new TH1F("Energy_EG","Energy_EG", 500, 0, 1.);
h_Energy_EG->GetXaxis()->SetTitle("Energy (keV)");
h_Energy_EG->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Energy_Exotic = new TH1F("Energy_Exotic","Energy_Exotic", 500, 0, 1.);
h_Energy_Exotic->GetXaxis()->SetTitle("Energy (keV)");
h_Energy_Exotic->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Energy_Photon = new TH1F("Energy_Photon","Energy_Photon", 500, 0, 1000);
h_Energy_Photon->GetXaxis()->SetTitle("Energy (keV)");
h_Energy_Photon->GetYaxis()->SetTitle("Counts / bin");
*/

/*
TH1F *h_First_Hit_Pos = new TH1F("First_Hit_Pos","First_Hit_Pos", 500, 0, 5000);
h_First_Hit_Pos->GetXaxis()->SetTitle("Pos (keV)");
h_First_Hit_Pos->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_First_Hit_Time = new TH1F("First_Hit_Time","First_Hit_Time", 500, 0, 5000);
h_First_Hit_Time->GetXaxis()->SetTitle("Pos (keV)");
h_First_Hit_Time->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Xpos = new TH1F("Xpos","Xpos", 500, 0, 5000);
h_Xpos->GetXaxis()->SetTitle("Pos (keV)");
h_Xpos->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Ypos = new TH1F("Ypos","Ypos", 500, 0, 5000);
h_Ypos->GetXaxis()->SetTitle("Pos (keV)");
h_Ypos->GetYaxis()->SetTitle("Counts / bin");

TH1F *h_Zpos = new TH1F("Zpos","Zpos", 500, 0, 5000);
h_Zpos->GetXaxis()->SetTitle("Pos (keV)");
h_Zpos->GetYaxis()->SetTitle("Counts / bin");
*/
//by Shuya 160421
/*
TH2I *h_Count_PMT_Front[100];
for(int i=0;i<100;i++)
{
	char brN[300];
        sprintf(brN, "Count_PMT_Front_run%d",i);
	h_Count_PMT_Front[i] = new TH2I(brN,brN, 10, 0, 10, 10, 0, 10);
	h_Count_PMT_Front[i]->GetXaxis()->SetTitle("X (no.)");
	h_Count_PMT_Front[i]->GetYaxis()->SetTitle("Y (no.)");
}

TH2I *h_Count_PMT_Back[100];
for(int i=0;i<100;i++)
{
	char brN[300];
        sprintf(brN, "Count_PMT_Back_run%d",i);
	h_Count_PMT_Back[i] = new TH2I(brN,brN, 10, 0, 10, 10, 0, 10);
	h_Count_PMT_Back[i]->GetXaxis()->SetTitle("X (no.)");
	h_Count_PMT_Back[i]->GetYaxis()->SetTitle("Y (no.)");
}
*/

//by Shuya 160426. Removed the histogram to replace with TTree.
//vector<TH2I*> h_Count_PMT_Front;
vector<TH2I*> h_Count_PMT_Back;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  

  LXeEventTree = new TTree("t","LXe Scintillator Simulation Data");
  LXeEventTree->Branch("Energy_Initial",&eng_int,"eng_int/D");
  LXeEventTree->Branch("LightOutput_LXe",&eng_LXe,"eng_LXe/D");
  LXeEventTree->Branch("LightOutput_Proton",&eng_LXe_proton,"eng_LXe_proton/D");
  LXeEventTree->Branch("LightOutput_Alpha",&eng_LXe_alpha,"eng_LXe_alpha/D");
  LXeEventTree->Branch("LightOutput_C12", &eng_LXe_C12,"eng_LXe_C12/D");
  LXeEventTree->Branch("LightOutput_EG",&eng_LXe_EG,"eng_LXe_EG/D");
  LXeEventTree->Branch("LightOutput_Exotic",&eng_LXe_Exotic,"eng_LXe_Exotic/D");
//by Shuya 160502
  LXeEventTree->Branch("EnergyDeposit_Total",&edep_LXe,"edep_LXe/D");
  LXeEventTree->Branch("EnergyDeposit_Proton",&edep_LXe_proton,"edep_LXe_proton/D");
  LXeEventTree->Branch("EnergyDeposit_Alpha",&edep_LXe_alpha,"edep_LXe_alpha/D");
  LXeEventTree->Branch("EnergyDeposit_C12", &edep_LXe_C12,"edep_LXe_C12/D");
  LXeEventTree->Branch("EnergyDeposit_EG",&edep_LXe_EG,"edep_LXe_EG/D");
  LXeEventTree->Branch("EnergyDeposit_Exotic",&edep_LXe_Exotic,"edep_LXe_Exotic/D");
//by Shuya 160407
  LXeEventTree->Branch("Num_DetectedPhotonFront",&eng_LXe_PhotonFront,"eng_LXe_PhotonFront/I");
//by Shuya 160427
  LXeEventTree->Branch("Num_DetectedPhotonBack",&eng_LXe_PhotonBack,"eng_LXe_PhotonBack/I");
//by Shuya 160502
  LXeEventTree->Branch("Num_CreatedPhotonTotal",&eng_LXe_PhotonTotal,"eng_LXe_PhotonTotal/I");
//by Shuya 160504
  LXeEventTree->Branch("Num_NonPMTCountTotal",&num_LXe_NonPMT,"num_LXe_NonPMT/D");
  LXeEventTree->Branch("Num_AbsorptionInDetectorTotal",&num_LXe_Abs,"num_LXe_Abs/D");

  LXeEventTree->Branch("First_Hit_Pos",&FirstHitMag,"FirstHitMag/D");
  LXeEventTree->Branch("First_Hit_Time",&FirstHitTime,"FirstHitTime/D");

  LXeEventTree->Branch("Xpos",&Xpos,"Xpos/D");
  LXeEventTree->Branch("Ypos",&Ypos,"Ypos/D");
  LXeEventTree->Branch("Zpos",&Zpos,"Zpos/D");



//by Shuya 160422. Making tree for photon hits on each pmt.
  LXeEventTree2 = new TTree("t2","LXe Scintillator Simulation Data");
//by Shuya 160426. NOTE THIS IS REQUIRED otherwise branches are not drawn in the tree. # of entries is 1 this case because each branch just filled one time with [10][10](i.e.,100) data.  
  LXeEventTree2->SetEntries(1);

/*
  for(int i=0;i<10;i++)
  {
	for(int j=0;j<10;j++)
	{
		char brN[300];
		char brN2[300];
        	sprintf(brN, "PMT_Front_Hit_%d_%d",i,j);
        	sprintf(brN2, "PMT_Front_Hit_%d_%d/I",i,j);
  		LXeEventTree2->Branch(brN,&PmtFrontHit[i][j],brN2);
        	sprintf(brN, "PMT_Back_Hit_%d_%d",i,j);
        	sprintf(brN2, "PMT_Back_Hit_%d_%d/I",i,j);
  		LXeEventTree2->Branch(brN,&PmtBackHit[i][j],brN2);
	}
  }
*/

  cout << LXePointer << endl;
}

LXeDataRecordTree::~LXeDataRecordTree()
{/* Destructor, Close root file */
 
  DataFile->Write(); 
  DataFile->Close();
  cout << "############################################################" << endl;
  cout << "Created Root Tree File = \"LXeDataTree.root\"" << endl;
  cout << "Got the ROOT Tree File from Data Record Routine!" << endl; 
  delete DataFile;

/*
//by Shuya 160509
  for(int i=0;i<NX;i++)
  {
  	delete [] PmtFrontHit[i];
  	delete [] PmtBackHit[i];
  }
  delete [] PmtFrontHit;
  delete [] PmtBackHit;
*/
}

// Member functions of TntDataRecordTree class

void LXeDataRecordTree::senddataPG(double value1=0.)
{
    eng_int = value1;
    event_counter++;
    //  cout << "eng_int = " << eng_int << endl;
}

//by Shuya 160408
//void LXeDataRecordTree::senddataPMT(int id, double value1)
//by Shuya 160421
void LXeDataRecordTree::senddataPMT(int id, int value1, int evid)
{
	int x, y;
	char brN[300];

	//if(id < 100)	//Front side
	//by Shuya 160509
	if(id < (NX*NY))	//Front side
	{
        	sprintf(brN, "Count_PMT_Front_Event%d",evid);
		x = id / NX;
		y = id % NY;
		//((TH2I*)DataFile->Get("Count_PMT_Front"))->Fill(x,y,value1);
		//for(int i = 0;i<value1;i++)	((TH2I*)DataFile->Get("Count_PMT_Front"))->Fill(x,y);
	//by Shuya 160426. I removed this to replace with TTree branch.
		//for(int i = 0;i<value1;i++)	((TH2I*)DataFile->Get(brN))->Fill(x,y);

		PmtFrontHit[x][y] = value1;
	}
	//else if(id >= 100 && id < 200)	//Back side
	else if(id >= (NX*NY) && id < (2*NX*NY))	//Back side
	{
        	sprintf(brN, "Count_PMT_Back_Event%d",evid);
		//by Shuya 160509
		//x = (id-100) / 10;
		//y = (id-100) % 10;
		x = (id-NX*NY) / NX;
		y = (id-NX*NY) % NY;
		//((TH2I*)DataFile->Get("Count_PMT_Back"))->Fill(x,y,value1);
		//for(int i = 0;i<value1;i++)	((TH2I*)DataFile->Get("Count_PMT_Back"))->Fill(x,y);

		//by Shuya 160426. I removed this to replace with TTree branch.
		//by Shuya 160428. Let's just make histograms once in a while.
		if(evid%10==1)
		{
			for(int i = 0;i<value1;i++)	((TH2I*)DataFile->Get(brN))->Fill(x,y);
		}

		PmtBackHit[x][y] = value1;
		//G4cout << "PMTBACKTEST 160509 " << PmtBackHit[x][y] << endl;
	}
}

//by Shuya 160421
void LXeDataRecordTree::createdataPMT(int evid)
{
	char brN[300];

        sprintf(brN, "PMT_Front_Hit_Event_%d", evid);
	//G4cout << "FORMATTEST " << TString::Format("PmtFrontHit[%d][%d]/I", NX,NY) << G4endl;
        //sprintf(brN2, "PMT_Front_Hit_Event_%d/I", evid);
        //by Shuya 160509
  	//LXeEventTree2->Branch(brN,PmtFrontHit,"PmtFrontHit[8][8]/I");
  	LXeEventTree2->Branch(brN,PmtFrontHit,TString::Format("PmtFrontHit[%d][%d]/I", NX,NY));
  	//LXeEventTree->Branch(brN,&PmtFrontHit);
        //G4cout << "TEST OF GETBRANCH FILL " << evid << " " << brN << G4endl;

        sprintf(brN, "PMT_Back_Hit_Event_%d", evid);
        //sprintf(brN2, "PMT_Back_Hit_Event_%d[10][10]/I", evid);
        //by Shuya 160509
  	//LXeEventTree2->Branch(brN,PmtBackHit,"PmtBackHit[8][8]/I");
  	LXeEventTree2->Branch(brN,PmtBackHit,TString::Format("PmtBackHit[%d][%d]/I", NX,NY));
  	//LXeEventTree->Branch(brN,&PmtBackHit);
        //G4cout << "TEST OF GETBRANCH FILL " << evid << " " << brN << G4endl;

//by Shuya 160426. Removed this because of replacement with TTree branch.
/*
	//G4cout << "!!!!CREATE!!! " << evid << G4endl;
	sprintf(brN, "Count_PMT_Front_run%d",evid);
	h_Count_PMT_Front.push_back (new TH2I(brN,brN, 10, 0, 10, 10, 0, 10));
	h_Count_PMT_Front[evid]->GetXaxis()->SetTitle("X (no.)");
	h_Count_PMT_Front[evid]->GetYaxis()->SetTitle("Y (no.)");

*/
	//by Shuya 160428. Let's just make a histogram once in a while.
	if(evid%10==1)
	{
	sprintf(brN, "Count_PMT_Back_Event%d",evid);
	//h_Count_PMT_Back.push_back (new TH2I(brN,brN, 10, 0, 10, 10, 0, 10));
	h_Count_PMT_Back.push_back (new TH2I(brN,brN, NX, 0, NX, NY, 0, NY));
	h_Count_PMT_Back[evid/10]->GetXaxis()->SetTitle("X (no.)");
	h_Count_PMT_Back[evid/10]->GetYaxis()->SetTitle("Y (no.)");
	}
}

void LXeDataRecordTree::senddataEV(int type, double value1)
{
    // Writes data values from TntEndofEventAction to EventTree
    // Changes from Serra >>> Light Conversion in TntSD.cc, EndOfEvent()
    // Hard coded for Tnt
    switch(type)
      {
      case 1:
	eng_LXe = value1;  // Sum of all energy in event!
	if (eng_LXe > Det_Threshold)
	{number_total++;
	//G4cout << "!!! " << eng_LXe << G4endl;
//G4cout << (TH1D*)DataFile->Get("Energy_LXe") << "!! !!" << G4endl;
//by Shuya 160426. Removed the histogram to replace with TTree.
	//((TH1D*)DataFile->Get("Energy_LXe"))->Fill(eng_LXe);
	//h_Energy_LXe->Fill(eng_LXe);
	//h_Energy_LXe->Fill(10, 20);
	}
	break;
      case 2:
	eng_LXe_proton = value1; 
	if (eng_LXe_proton > Det_Threshold)
	  {number_protons++;
//by Shuya 160426. Removed the histogram to replace with TTree.
	  //((TH1D*)DataFile->Get("Energy_Proton"))->Fill(eng_LXe_proton);
	   //h_Energy_Proton->Fill(eng_LXe_proton);
	  }
	break;
      case 3:
	eng_LXe_alpha = value1;
	if (eng_LXe_alpha > Det_Threshold) 
	  {number_alphas++;
//by Shuya 160426. Removed the histogram to replace with TTree.
	  //((TH1D*)DataFile->Get("Energy_Alpha"))->Fill(eng_LXe_alpha);
	   //h_Energy_Alpha->Fill(eng_LXe_alpha);
	  }
	break;
      case 4:
	eng_LXe_C12 = value1; 
	if (eng_LXe_C12 > Det_Threshold)  
	  {number_C12++;
//by Shuya 160426. Removed the histogram to replace with TTree.
	  //((TH1D*)DataFile->Get("Energy_C12"))->Fill(eng_LXe_C12);
	   //h_Energy_C12->Fill(eng_LXe_C12);
	  }
	break;
      case 5:
	eng_LXe_EG = value1; 
	if (eng_LXe_EG > Det_Threshold)  
	  {number_EG++;
//by Shuya 160426. Removed the histogram to replace with TTree.
	  //((TH1D*)DataFile->Get("Energy_EG"))->Fill(eng_LXe_EG);
	   //h_Energy_EG->Fill(eng_LXe_EG);
	  }
	break;
      case 6:
	eng_LXe_Exotic = value1;  
	if (eng_LXe_Exotic > Det_Threshold)
	  {number_Exotic++;
//by Shuya 160426. Removed the histogram to replace with TTree.
	  //((TH1D*)DataFile->Get("Energy_Exotic"))->Fill(eng_LXe_Exotic);
	   //h_Energy_Exotic->Fill(eng_LXe_Exotic);
	  }
	break;
//by Shuya 160407
      case 7:
	eng_LXe_PhotonFront = (int)value1;  
	if (eng_LXe_PhotonFront > Det_Threshold)
	  {number_Photon++;
//by Shuya 160426. Removed the histogram to replace with TTree.
	  //((TH1D*)DataFile->Get("Energy_Photon"))->Fill(eng_LXe_Photon);
	   //h_Energy_Photon->Fill(eng_LXe_Photon);
	  }
	break;
//by Shuya 160427
      case 8:
	eng_LXe_PhotonBack = (int)value1;  
	if (eng_LXe_PhotonBack > Det_Threshold)
	  {number_Photon++;
	  }
	break;
//by Shuya 160502
      case 9:
	eng_LXe_PhotonTotal = (int)value1;  
	if (eng_LXe_PhotonTotal > Det_Threshold)
	  {number_Photon++;
	  }
	break;
//by Shuya 160502
      case 10:
	edep_LXe = value1;  
	break;
      case 11:
	edep_LXe_proton = value1;  
	break;
      case 12:
	edep_LXe_alpha = value1;  
	break;
      case 13:
	edep_LXe_C12 = value1;  
	break;
      case 14:
	edep_LXe_EG = value1;  
	break;
      case 15:
	edep_LXe_Exotic = value1;  
      case 16:
	num_LXe_NonPMT = value1;  
      case 17:
	num_LXe_Abs = value1;  
	break;
      default:
	G4cout << "Data Transfer Error!" << G4endl;
	G4ExceptionSeverity severity=FatalException;
	G4Exception("Program Aborted in LXeDataRecordTree.cc::senddataEV()", "DataError",severity,"Data Transfer Error!");
	//Note: the enum G4ExceptionSeverity has been defined since Geant4 release 5.0 and its values are: FatalException, FatalErrorInArgument, EventMustBeAborted, FatalException, and JustWarning. 
  //G4Exception("Program Aborted in TntDataRecordTree.cc::senddataEV()");
        break;
      }
	// cout << value1 << " <- This was put in the data file!" << endl;
}

void LXeDataRecordTree::senddataPosition(G4ThreeVector pos)
{
  // const G4double pi = 3.14159265;
  FirstHitMag = 0;
  // HitAngle = 360;       // Some value that one would never get! 
  Xpos = pos(0);
  Ypos = pos(1);
  Zpos = pos(2);

    FirstHitMag = sqrt(pow(Xpos,2)+pow(Ypos,2)+pow(Zpos,2))/cm;

//by Shuya 160502
	FirstHitPosition = pos;

    // HitAngle = acos(x/HitRad)*180/pi;

    //   G4cout << "Hit Distance s = " << FirstHitMag << G4endl;
}

void LXeDataRecordTree::senddataTOF(G4double time)
{
  FirstHitTime = time/ns;
  // G4cout << "The first hit time was : " << FirstHitTime << G4endl;
}
 
void LXeDataRecordTree::ShowDataFromEvent()
{
      G4cout << "================OUTPUT SENT FROM LXeDataRecordTree====================================" << G4endl;
      G4cout << "The Energy of the Initial Particle was:    " << eng_int << G4endl;
      G4cout << "The Position of the First Hit was (cm <-note!) (Distance from (x=0,y=0,z=0)):    " << FirstHitMag << G4endl;
      G4cout << "The Position of the First Hit was (mm <-note!) (x,y,z):    " << FirstHitPosition << G4endl;
      G4cout << "The Time of Flight of First Hit was (ns):  " << FirstHitTime << G4endl;
      G4cout << "Measured Total Light Output (unless otherwise light_Conv=NULL) in this Event:  " << eng_LXe  << G4endl;
      G4cout << "Measured Proton Light Output (unless otherwise light_Conv=NULL) in this Event: " << eng_LXe_proton << G4endl;
      G4cout << "Measured Alpha Light Output (unless otherwise light_Conv=NULL) in this Event:  " << eng_LXe_alpha << G4endl;
      G4cout << "Measured C12 Light Output (unless otherwise light_Conv=NULL) in this Event:    " << eng_LXe_C12 << G4endl;
      G4cout << "Measured Electron (/Gamma) Eng. Loss:               " << eng_LXe_EG << G4endl;
      G4cout << "Measured Energy Loss from Exotic Particles (Be9,etc):" << eng_LXe_Exotic << G4endl;
      G4cout << "Measured Energy Loss from Photons (Front):" << eng_LXe_PhotonFront << G4endl;
      G4cout << "Measured Energy Loss from Photons (Back):" << eng_LXe_PhotonBack << G4endl;
//by Shuya 160502
      G4cout << "Measured Total Energy Loss in this Event (MeV):  " << edep_LXe  << G4endl;
      G4cout << "Measured Proton Energy Loss in this Event (MeV): " << edep_LXe_proton << G4endl;
      G4cout << "Measured Alpha Energy Loss in this Event (MeV):  " << edep_LXe_alpha << G4endl;
      G4cout << "Measured C12 Energy Loss in this Event (MeV):    " << edep_LXe_C12 << G4endl;
      G4cout << "Measured Electron/Gamma Eng. Loss (MeV):               " << edep_LXe_EG << G4endl;
      G4cout << "Measured Energy Loss from Exotic Particles (Be9,etc) (MeV):" << edep_LXe_Exotic << G4endl;
}

void LXeDataRecordTree::FillTree()
{
  if (eng_LXe > Det_Threshold)  // Threshold set in main()
     {number_at_this_energy++;}
    LXeEventTree->Fill();  

	//G4cout << "FillTree1!" << G4endl;
}

//by Shuya 160422
void LXeDataRecordTree::FillTree2(int evid)
{
	char brN[300];

        sprintf(brN, "PMT_Front_Hit_Event_%d", evid);
        //sprintf(brN2, "PMT_Front_Hit_Event_%d/I", evid);
        //G4cout << "TEST OF GETBRANCH FILL " << evid << " " << brN << G4endl;

	///for(int i=0;i<NX;i++)
	//{
	//	for(int j=0;j<NY;j++)	G4cout << PmtFrontHit[i][j] << G4endl;
		//for(int j=0;j<NY;j++)	G4cout << "TEST PMTFRONTHIT ADDRESS " << &PmtFrontHit[i][j] << G4endl;
	//}
	
  	LXeEventTree2->GetBranch(brN)->Fill();
  	//LXeEventTree->Branch(brN,&PmtFrontHit);

        sprintf(brN, "PMT_Back_Hit_Event_%d", evid);
        //sprintf(brN2, "PMT_Back_Hit_Event_%d[10][10]/I", evid);
        //G4cout << "TEST OF GETBRANCH FILL " << evid << " " << brN << G4endl;

	//for(int i=0;i<NX;i++)
	//{
	//	for(int j=0;j<NY;j++)	G4cout << PmtBackHit[i][j] << G4endl;
	//}
	
  	LXeEventTree2->GetBranch(brN)->Fill();
  	//LXeEventTree->Branch(brN,&PmtBackHit);
  	
    //LXeEventTree2->Fill();  

//Initialization.
  //for(int i=0;i<10;i++)
  //by Shuya 160509
  for(int i=0;i<NX;i++)
  {
	//for(int j=0;j<10;j++)
	for(int j=0;j<NY;j++)
	{
		//G4cout << "TEST NO 422 " << PmtFrontHit[i][j] << G4endl;
		PmtFrontHit[i][j] = 0;
		PmtBackHit[i][j] = 0;
	}
  }
}

void LXeDataRecordTree::GetParticleTotals()
{
    cout << "The Initial Number of Beam Particles was: " << event_counter << endl;
    cout << "The Detection Threshold is set at : " << Det_Threshold << " MeVee." << endl;
    cout << "The Total Number of Detected Events was:  " << number_total << endl;
    cout << "The Total Number of Protons Detected was: " << number_protons << endl;
    cout << "The Total Number of Alphas Detected was:  " << number_alphas << endl;
    cout << "The Total Number of C12 Detected was:     " << number_C12 << endl;
    cout << "The Total Number of e- or e+    was:      " << number_EG << endl;
    cout << "The Total Number of Exotic Particles was: " << number_Exotic << endl;
    cout << "The Total Number of Photons was: " << number_Photon << endl;
}

void LXeDataRecordTree::CalculateEff(int ch_eng)
{
   char EffFile[] = "eff_results_file.dat";
   ofstream outfile2(EffFile,ios::app);

   cout << number_at_this_energy << endl;
   efficiency = 100*(static_cast<double>(number_at_this_energy)/static_cast<double>(ch_eng));
   cout << "Efficiency was: " << number_at_this_energy << "/" << ch_eng << " = " << efficiency << " %" << endl;

   outfile2 << setiosflags(ios::fixed)
	   << setprecision(4)
           << eng_int << "  "
	   << efficiency 
	   << endl;
   outfile2.close();

   number_at_this_energy = 0;
}
 
