#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include "TF1.h"
#include "TF2.h"
#include "TF3.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TLeaf.h"
#include "TRandom3.h"
#include "TFitResult.h"
#include "TClonesArray.h"
//by Shuya 160503
//#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TSpectrum.h"
#include "THStack.h"

using namespace std;

namespace {
class DirRemember_t {
public:
	DirRemember_t() : fDir (gDirectory) { }
	~DirRemember_t() { fDir->cd(); }
private:
	TDirectory* fDir;
}; }


inline void TreeStyle(TTree* t)
{
	t->SetLineWidth(3);
	t->SetMarkerSize(2);
	t->SetMarkerStyle(20);
	t->SetMarkerColor(t->GetLineColor());
}



class AngerCamera :
	public TNamed {
public:
	AngerCamera();
	AngerCamera(const char* name, const char* title,
							Int_t xbins, Double_t xsize,
							Int_t ybins, Double_t ysize,
							Double_t zsize,
							Double_t prcntAnthracene,
							Double_t attenLength,
							Double_t timeResolution);
	~AngerCamera();
	//by Shuya 160425
	//bool FillHit(TTree* t, Int_t hit);
	bool FillHit(TTree* t, TTree* t2, Int_t hit, Long64_t eventNum);
	UInt_t GetSeed() { return gRandom ? gRandom->GetSeed() : -1; }
	void SetSeed(UInt_t seed) { if(gRandom) gRandom->SetSeed(seed); }

private:
	AngerCamera(const AngerCamera&) :
		TNamed("",""),
		fHit(0), kNumBins(0), kXbins(0), kYbins(0),
		fBin(0), fX(0), fY(0), fT(0), fI(0), fIres(0),
		//by Shuya 160504
		//kPrcntAnthracene(0.6), kAttenLength(-1), kTimeRes(1),
		kPrcntAnthracene(1), kAttenLength(100), kTimeRes(0),
		//by Shuya 160427
		//kX(200), kY(200), kZ(30)
		kX(30), kY(30), kZ(5)	//kz=15 because I set the center of the detector at x,y,z=0
		// kDigitizerBins(256), kDigitizerRate(250)//, fDigi(0)
		{ }
	AngerCamera& operator= (const AngerCamera&) { return *this; }

public:
	//by Shuya 160425. 12000 photons/eMeV (BC505) 
	//static const Int_t kNumAnthracene = 17400; //!
	//static const Int_t kNumAnthracene = 12000; //!
	//by Shuya 160506. To take into account the PMT efficiency
	//static const Int_t kNumAnthracene = (int)(12000.*0.2); //!
	//by Shuya 160425. 9500 photons/MeVee (BC519)
	//static const Int_t kNumAnthracene = (int)(9500.*0.2); //!
	//by Shuya 160512. For BC404.
	static const Int_t kNumAnthracene = (int)(10400.*0.2); //!
	//by Shuya 160512. For EJ309.
	//static const Int_t kNumAnthracene = (int)(11500.*0.2); //!
	//static const Int_t kNumAnthracene = (int)(11500.); //!
	Int_t fHit;
	const Int_t kNumBins;
	const Int_t kXbins;
	const Int_t kYbins;
	Int_t    *fBin;  // [kNumBins]
	Double_t *fX;    // [kNumBins]
	Double_t *fY;    // [kNumBins]
	Double_t *fT;    // [kNumBins]
	Double_t *fI;    // [kNumBins]
	Double_t *fIres; // [kNumBins]
	const Double_t kPrcntAnthracene;
	const Double_t kAttenLength;
	const Double_t kTimeRes;
	const Double_t kX;
	const Double_t kY;
	const Double_t kZ;

	//by Shuya 160509
	Double_t fw;
	Double_t fl;


	// const Int_t kDigitizerBins;
	// const Int_t kDigitizerRate;
	// Int_t fDigiBinsTotal;
	// Double_t fDigi[64][256]; // [kDigitizerBins]
	// Double_t fDigit[64][256];

private:
	TRandom3* fRandom; //!
	ClassDef(AngerCamera, 3);
};

class AngerCameraAnalyzer :
	private AngerCamera {
public:
	AngerCameraAnalyzer():
		AngerCamera()
		{ }
/*
	AngerCameraAnalyzer(const char* name, const char* title,
											Int_t xbins, Double_t xsize,
											Int_t ybins, Double_t ysize,
											Double_t zsize,
											Double_t prcntAnthracene,
											Double_t attenLength,
											Double_t timeResolution):
*/
	AngerCameraAnalyzer(const char* name, const char* title, Int_t xbins, Double_t xsize, Int_t ybins, Double_t ysize,Double_t zsize, Double_t prcntAnthracene,Double_t attenLength, Double_t timeResolution) :
	//AngerCameraAnalyzer(const char* name, const char* title, Int_t xbins, Double_t xsize, Int_t ybins, Double_t ysize,Double_t zsize, Double_t prcntAnthracene,Double_t attenLength, Double_t timeResolution);
		AngerCamera(name, title, xbins, xsize, ybins, ysize, zsize, prcntAnthracene, attenLength, timeResolution)
		{ }


	//by Shuya 160425
	//void GenIntensityTree(TTree* tin, TTree* tout);
	void GenIntensityTree(TTree* tin, TTree* tin2, TTree* tout);
	//by Shuya 160426
	//void CalculatePositions(TTree* tin, TTree* tout, Int_t method,
	void CalculatePositions(TTree* tin, TTree* tin2, TTree* tout, Int_t method,
													Long64_t numEvents = 0xffffffffffff,
													Long64_t firstEvent = 0,
													const char* fitOptions = "qns");
	//by Shuya 160425
	//TFile* CompleteAnalysis(TTree* tsim, const char* outputfile, int numMethods, int* methods,
	TFile* CompleteAnalysis(TTree* tsim, TTree* tsim2, const char* outputfile, int numMethods, int* methods,
													Long64_t numEvents = 0xffffffffffff, Long64_t firstEvent = 0,
													const char* fitOptions = "qns");
	//by Shuya 160425
	//TFile* CompleteAnalysis1(TTree* tsim, const char* outputfile, int method,
	TFile* CompleteAnalysis1(TTree* tsim, TTree* tsim2, const char* outputfile, int method,
													 Long64_t numEvents = 0xffffffffffff, Long64_t firstEvent = 0,
													 const char* fitOptions = "qns")
		//by Shuya 160425
		//{ return CompleteAnalysis(tsim, outputfile, 1, &method, numEvents, firstEvent, fitOptions); }
		{ return CompleteAnalysis(tsim, tsim2, outputfile, 1, &method, numEvents, firstEvent, fitOptions); }

	Double_t IntensityFunction1d(Double_t* x, Double_t *p);
	Double_t IntensityFunction2d(Double_t* x, Double_t *p);
	Double_t IntensityFunction3d(Double_t* x, Double_t *p);
	//by Shuya 160504
	Double_t IntensityFunction2d_FromData(Double_t* x, Double_t *p);

	TF1* Create_fpos1(const char* name)
		{ return new TF1(name, this, &AngerCameraAnalyzer::IntensityFunction1d,
										 -kX/2, kX/2, 3,
						"AngerCameraAnalyzer", "IntensityFunction1d");
		}
	TF2* Create_fpos2(const char* name)
		{ return new TF2(name, this, &AngerCameraAnalyzer::IntensityFunction2d,
										 -kX/2, kX/2, -kY/2, kY/2, 4,
										 "AngerCameraAnalyzer", "IntensityFunction2d");
		}
	TF3* Create_fpos3(const char* name)
		{ return new TF3(name, this, &AngerCameraAnalyzer::IntensityFunction3d,
										 -kX/2, kX/2, -kY/2, kY/2, 30, 70, 5,
										 "AngerCameraAnalyzer", "IntensityFunction3d");
		}


	ClassDef(AngerCameraAnalyzer, 0);
};


inline AngerCamera::AngerCamera():
	TNamed("", ""),
	fHit(0), kNumBins(0), kXbins(0), kYbins(0),
	fBin(0), fX(0), fY(0), fT(0), fI(0), fIres(0),
	//by Shuya 160504
	//kPrcntAnthracene(0.6), kAttenLength(-1), kTimeRes(1),
	kPrcntAnthracene(1), kAttenLength(100), kTimeRes(0),
	//kX(200), kY(200), kZ(30),
	//by Shuya 160503
	kX(30), kY(30), kZ(5),
	// kDigitizerBins(256), kDigitizerRate(250), // fDigi(0),
	fRandom(new TRandom3(1)),
	//by Shuya 160509
	fw(0), fl(0)
{ }

inline AngerCamera::AngerCamera(const char* name, const char* title,
																Int_t xbins, Double_t xsize,
																Int_t ybins, Double_t ysize,
																Double_t zsize,
																Double_t prcntAnthracene,
																Double_t attenLength,
																Double_t timeResolution):
	TNamed(name, title),
	fHit(0), kNumBins(xbins*ybins), kXbins(xbins), kYbins(ybins),
	fBin(0), fX(0), fY(0), fT(0), fI(0), fIres(0),
	kPrcntAnthracene(prcntAnthracene),
	kAttenLength(attenLength),
	kTimeRes(timeResolution),
	kX(xsize), kY(ysize), kZ(zsize),
	// kDigitizerBins(256), kDigitizerRate(250), // fDigi(0),
	fRandom(new TRandom3(1)),
	//by Shuya 160509
	fw(0), fl(0)
{
	if(kNumBins) {
		fBin  = new Int_t[kNumBins];
		fX    = new Double_t[kNumBins];
		fY    = new Double_t[kNumBins];
		fT    = new Double_t[kNumBins];
		fI    = new Double_t[kNumBins];
		fIres = new Double_t[kNumBins];

		// fDigiBinsTotal = kDigitizerBins*kNumBins;
		// for(int i=0; i< 64; ++i)
		// 	fDigi[i] = new Double_t[ kDigitizerBins ]; //fDigiBinsTotal];
	}

//Comment by Shuya 160422. This is correct. Just declaring hdummy, but should give correct bin center positions.
	TH2F hdummy("", "", kXbins, -kX/2, +kX/2, kYbins, -kY/2, +kY/2);
	int bin = 0;
	for(int i=0; i< kXbins; ++i) {
		for(int j=0; j< kYbins; ++j) {
			fBin[bin] = bin;
			fX[bin] = hdummy.GetXaxis()->GetBinCenter(i+1);
			fY[bin] = hdummy.GetYaxis()->GetBinCenter(j+1);
			++bin;
		}
	}

	//by Shuya 160509
	fw = kX / (Double_t)kXbins;	//width (x) of PMT (cm)
	fl = kY / (Double_t)kYbins;	//length (y) of PMT (cm)
}

inline AngerCamera::~AngerCamera()
{
	if(kNumBins) {
		delete[] fBin;
		delete[] fX;
		delete[] fY;
		delete[] fT;
		delete[] fI;
		delete[] fIres;

		// for(int i=0; i< 64; ++i)
		// 	delete[] fDigi[i];
	}
	if(fRandom)
		delete fRandom;
}

//by Shuya 160425
//inline bool AngerCamera::FillHit(TTree* t, Int_t hit)
//Comments by Shuya; eventNum is the event number. Nth data in t, t2 are already commanded in the for-loop where this function is incorporated (GetEntry(eventNum))
inline bool AngerCamera::FillHit(TTree* t, TTree* t2, Int_t hit, Long64_t eventNum)
{

	if(hit == 0) {
		std::fill(fI, fI + kNumBins, 0);
		std::fill(fIres, fIres + kNumBins, 0);
		// for(int i=0; i< 64; ++i)
		// 	std::fill(fDigi[i], fDigi[i] + kDigitizerBins /*fDigiBinsTotal*/, 0);
	}

//by Shuya 160422.
	//TLeaf* lfX = t->GetLeaf("All_Event_Data.Xpos");
	//TLeaf* lfY = t->GetLeaf("All_Event_Data.Ypos");
	//TLeaf* lfZ = t->GetLeaf("All_Event_Data.Zpos");
	//TLeaf* lfE = t->GetLeaf("All_Event_Data.eng_Tnt");
	//TLeaf* lfT = t->GetLeaf("All_Event_Data.HitTime");
	TLeaf* lfX = t->GetLeaf("Xpos");
	TLeaf* lfY = t->GetLeaf("Ypos");
	TLeaf* lfZ = t->GetLeaf("Zpos");
	//TLeaf* lfE = t->GetLeaf("eng_Tnt");
	//by Shuya 160427. Total energy deposit by p,alpha,gamma,e,etc.
	TLeaf* lfE = t->GetLeaf("eng_LXe");
	TLeaf* lfT = t->GetLeaf("FirstHitTime");
	TLeaf* lfPht = t->GetLeaf("eng_LXe_PhotonTotal");

	if(!lfX) { Error("AngerCamera::Fill", "BadLeafX"); return false; }
	if(!lfY) { Error("AngerCamera::Fill", "BadLeafY"); return false; }
	if(!lfZ) { Error("AngerCamera::Fill", "BadLeafZ"); return false; }
//by Shuya 160425
	if(!lfE) { Error("AngerCamera::Fill", "BadLeafE"); return false; }
	if(!lfT) { Error("AngerCamera::Fill", "BadLeafT"); return false; }
//by Shuya 160425
	if(!lfPht) { Error("AngerCamera::Fill", "BadLeafPht"); return false; }


	fHit = hit;
//Comments By Shuya 160422. Getting the first hit position of neutron from the simulation.
	double x = lfX->GetValue(hit) / 10;       // cm
	double y = lfY->GetValue(hit) / 10;       // cm
//by Shuya 160422. In my code, I set z position at Original coordinate (0,0,0). TntSim case, (0,0,200). 185 = 200-15 (half thickness of detector)
	//double z = lfZ->GetValue(hit) / 10 - 185; // cm
	//by Shuya 160422 +15cm is to relocate the front surface of detector at 0 position.
	//double z = lfZ->GetValue(hit) / 10 + 15; // cm
	//by Shuya 160427. I changed the style center of detector is at z=0.
	double z = lfZ->GetValue(hit) / 10; // cm
//by Shuya 160425.
	double e = lfE->GetValue(hit);            // MeV
	//double e;
	double time = lfT->GetValue(hit);
//by Shuya 160425.
	double nphotons = lfPht->GetValue(hit);            // MeV

//by Shuya 160425
	//double nphotons = e * kNumAnthracene * kPrcntAnthracene;

	AngerCameraAnalyzer aca("", "", kXbins, kX, kYbins, kY, kZ,
													kPrcntAnthracene, kAttenLength, kTimeRes);
	//by Shuya 160426
	int	test = 0;
	char brN[300];
	//by Shuya 160428
	//int  PmtBackHit[10][10];
	//by Shuya 160509
	int  PmtBackHit[kXbins][kYbins];
	sprintf(brN,"PMT_Back_Hit_Event_%d",eventNum);
	t2->GetEntry(0);
	//double I = t2->GetBranch(brN)->GetEvent(0);
	t2->SetBranchAddress(brN,&PmtBackHit);
	t2->GetBranch(brN)->GetEvent(0);
	//cout << PmtBackHit[0][1] << endl;
	
	//cout << "Num Photons: " << nphotons << endl;
	//cout << "Z Position: " << z << endl;

	//by Shuya 160505
	//double	I_AfterAtten = 0;

	// TH1F hbin("", "", kDigitizerBins, 0, kDigitizerBins*1e3/kDigitizerRate);
	for(int bin = 0; bin< kNumBins; ++bin) {
//Comments by Shuya 160422. vars[] give pmt positions. So kZ = 30 (cm) is correct because the pmts are placed behind the detector.
		double vars[] = { fX[bin], fY[bin], kZ };
		double params[] = { e, x, y, z };

		//by Shuya 160425
		//double I = aca.IntensityFunction2d(vars, params);

		//by Shuya 160427
		//sprintf(brN,"PMT_Back_Hit_%d_%d",bin/10,bin%10);
		///TLeaf* lfI = t2->GetLeaf(brN);
		//double I = lfI->GetValue(hit);
		//by Shuya 160428
		//t2->GetEntry(bin);
		//double I = t2->GetBranch(brN)->GetEvent(bin);
		//double I = t2->GetBranch(brN)->GetEvent(bin);
		//double I = lfI->GetValue(hit);

////////////COMMENT BY SHUYA 160509. This bloc is the one you change when you want to get PMT intensities from root data or analytical function with G4simulated number of photons and positions.
		//by Shuya 160504
		//double I =  (double)PmtBackHit[bin/10][bin%10];
		//by Shuya 160509
		double I =  (double)PmtBackHit[bin/kXbins][bin%kYbins];
		//Comment by Shuya 160504. This enables to calculate photon counts at each pmt based on data of total created photons and its position (from Geant4).
		//params[0] = nphotons;
		//double I = aca.IntensityFunction2d_FromData(vars, params);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//cout << "Num Counts " << bin << " " << I << endl;
		//I_AfterAtten += I;

		//by Shuya 160425 for test.
		/*
		test += I;
		if(eventNum==99)
		//if(bin==kNumBins-1)
		{
			cout << I << endl; 
			cout << "TEST!!!!! " << eventNum << "  " << test << endl; 
		}
		//test += I;
		//if(bin==kNumBins-1)	cout << "TEST!!!!! " << eventNum << "  " << test << endl; 
		*/
		

		fI[bin]    = I;
		fIres[bin] = gRandom->Poisson(I);
		//by Shuya 160425 test.
		//cout << "TEST!! " << fI[bin] << " " << fIres[bin] << endl;
		fT[bin] = gRandom->Gaus(time, kTimeRes);

		// for(int ch = 0; ch< kDigitizerBins; ++ch) {
		// 	double tau = 1, tau1 = 4; // time constants 4sec, 1sec
		// 	double I0 = fIres[bin] / (tau1 - tau);
		// 	double tch = hbin.GetBinCenter(ch + 1);
		// 	double T = tch - time;
		// 	// fDigi[bin][ch] += ( I0 * (exp(-T / tau) - exp(-T / tau1)) );

		// 	// fDigit[bin][ch] = tch;
		// }
	}

	//by Shuya 160505	
	//cout << nphotons << " " << I_AfterAtten << " " << z << endl;
	
	return true;
}

inline Double_t AngerCameraAnalyzer::IntensityFunction3d(Double_t *x, Double_t *p)
{
	Double_t t  = x[2];
	Double_t t0 = p[4];
	Double_t I = IntensityFunction2d(x, p); // x-y intensity function
	Double_t probTime = TMath::Gaus(t, t0, kTimeRes, true);
	return probTime*I;
}

inline Double_t AngerCameraAnalyzer::IntensityFunction2d(Double_t *x, Double_t *p)
{
	//cout << "fwfl TEST " << fw << " " << fl << endl;

	//comment by Shuya 160425. norm (p[0]) means total energy deposit in unit of eMeV. So, norm*number (see below) = total photons / cm2 (number should be photons/eMeV/cm2)
	Double_t norm = p[0];
	Double_t xpos = p[1], ypos = p[2], zpos = p[3];
	Double_t R = sqrt(pow(x[0] - xpos, 2) + pow(x[1] - ypos, 2) + pow(kZ - zpos, 2));
	Double_t intensity =  // (photons / area)
		//kNumAnthracene*kPrcntAnthracene / (4*TMath::Pi()*R*R);
		//by Shuya 160506
		kNumAnthracene*kPrcntAnthracene / (4*TMath::Pi());

	//by Shuya 160506
	//Double_t R0 = sqrt(pow(x[0] - xpos + 20./2, 2) + pow(x[1] - ypos + 20./2, 2) + pow(kZ - zpos, 2));
	//Double_t R1 = sqrt(pow(x[0] - xpos + 20./2, 2) + pow(x[1] - ypos - 20./2, 2) + pow(kZ - zpos, 2));
	//Double_t R2 = sqrt(pow(x[0] - xpos - 20./2, 2) + pow(x[1] - ypos + 20./2, 2) + pow(kZ - zpos, 2));
	//Double_t R3 = sqrt(pow(x[0] - xpos - 20./2, 2) + pow(x[1] - ypos - 20./2, 2) + pow(kZ - zpos, 2));
	//by Shuya 160509
	Double_t R0 = sqrt(pow(x[0] - xpos + fw/2, 2) + pow(x[1] - ypos + fl/2, 2) + pow(kZ - zpos, 2));
	Double_t R1 = sqrt(pow(x[0] - xpos + fw/2, 2) + pow(x[1] - ypos - fl/2, 2) + pow(kZ - zpos, 2));
	Double_t R2 = sqrt(pow(x[0] - xpos - fw/2, 2) + pow(x[1] - ypos + fl/2, 2) + pow(kZ - zpos, 2));
	Double_t R3 = sqrt(pow(x[0] - xpos - fw/2, 2) + pow(x[1] - ypos - fl/2, 2) + pow(kZ - zpos, 2));

	// attenuation
	if(kAttenLength > 0) {
		intensity *= exp(-R / kAttenLength);
	}

	//by Shuya 160428. because PMT surface is circle
	//Double_t binArea = kX*kY/kNumBins;
	//by Shuya 160506
	//Double_t binArea = kX*kY/kNumBins * TMath::Pi()/4.0;
	//by Shuya 160509
	//Double_t binArea = atan((x[0] - xpos + 20./2) * (x[1] - ypos + 20./2) / ((kZ - zpos) * R0)) - atan((x[0] - xpos - 20./2) * (x[1] - ypos + 20./2) / ((kZ - zpos) * R2)) - atan((x[0] - xpos + 20./2) * (x[1] - ypos - 20./2) / ((kZ - zpos) * R1)) + atan((x[0] - xpos - 20./2) * (x[1] - ypos - 20./2) / ((kZ - zpos) * R3));
	Double_t binArea = atan((x[0] - xpos + fw/2) * (x[1] - ypos + fl/2) / ((kZ - zpos) * R0)) - atan((x[0] - xpos - fw/2) * (x[1] - ypos + fl/2) / ((kZ - zpos) * R2)) - atan((x[0] - xpos + fw/2) * (x[1] - ypos - fl/2) / ((kZ - zpos) * R1)) + atan((x[0] - xpos - fw/2) * (x[1] - ypos - fl/2) / ((kZ - zpos) * R3));
	//cout << "BINAREA = " << binArea << endl;
	//binArea *= (TMath::Pi() / 4.0);

	Double_t number = intensity*binArea;
	return norm * number;
}

//by Shuya 160504
//I give Total number of created photons in a detector (TotalIntensity) and then calculate the expected number of photons at each PMT position
inline Double_t AngerCameraAnalyzer::IntensityFunction2d_FromData(Double_t *x, Double_t *p)
{
	//comment by Shuya 160425. norm (p[0]) means total energy deposit in unit of eMeV. So, norm*number (see below) = total photons / cm2 (number should be photons/eMeV/cm2)
	Double_t TotalIntensity = p[0];
	Double_t xpos = p[1], ypos = p[2], zpos = p[3];
	Double_t R = sqrt(pow(x[0] - xpos, 2) + pow(x[1] - ypos, 2) + pow(kZ - zpos, 2));
	Double_t intensity =  // (photons / area)
		//TotalIntensity*kPrcntAnthracene / (4*TMath::Pi()*R*R);
		//by Shuya 160506
		TotalIntensity*kPrcntAnthracene / (4*TMath::Pi());

	//by Shuya 160506
	//Double_t R0 = sqrt(pow(x[0] - xpos + 20./2, 2) + pow(x[1] - ypos + 20./2, 2) + pow(kZ - zpos, 2));
	//Double_t R1 = sqrt(pow(x[0] - xpos + 20./2, 2) + pow(x[1] - ypos - 20./2, 2) + pow(kZ - zpos, 2));
	//Double_t R2 = sqrt(pow(x[0] - xpos - 20./2, 2) + pow(x[1] - ypos + 20./2, 2) + pow(kZ - zpos, 2));
	//Double_t R3 = sqrt(pow(x[0] - xpos - 20./2, 2) + pow(x[1] - ypos - 20./2, 2) + pow(kZ - zpos, 2));
	//by Shuya 160509
	Double_t R0 = sqrt(pow(x[0] - xpos + fw/2, 2) + pow(x[1] - ypos + fl/2, 2) + pow(kZ - zpos, 2));
	Double_t R1 = sqrt(pow(x[0] - xpos + fw/2, 2) + pow(x[1] - ypos - fl/2, 2) + pow(kZ - zpos, 2));
	Double_t R2 = sqrt(pow(x[0] - xpos - fw/2, 2) + pow(x[1] - ypos + fl/2, 2) + pow(kZ - zpos, 2));
	Double_t R3 = sqrt(pow(x[0] - xpos - fw/2, 2) + pow(x[1] - ypos - fl/2, 2) + pow(kZ - zpos, 2));

	// attenuation
	if(kAttenLength > 0) {
		//cout << "Before Correction: " << intensity << "	";
		intensity *= exp(-R / kAttenLength);
		//cout << "After Correction: " << intensity << " R = " << R << endl;
	}

	//by Shuya 160428. because PMT surface is circle
	//Double_t binArea = kX*kY/kNumBins;
	//by Shuya 160506
	//Double_t binArea = kX*kY/kNumBins * TMath::Pi()/4.0;
	//by Shuya 160509
	//Double_t binArea = atan((x[0] - xpos + 20./2) * (x[1] - ypos + 20./2) / ((kZ - zpos) * R0)) - atan((x[0] - xpos - 20./2) * (x[1] - ypos + 20./2) / ((kZ - zpos) * R2)) - atan((x[0] - xpos + 20./2) * (x[1] - ypos - 20./2) / ((kZ - zpos) * R1)) + atan((x[0] - xpos - 20./2) * (x[1] - ypos - 20./2) / ((kZ - zpos) * R3));
	Double_t binArea = atan((x[0] - xpos + fw/2) * (x[1] - ypos + fl/2) / ((kZ - zpos) * R0)) - atan((x[0] - xpos - fw/2) * (x[1] - ypos + fl/2) / ((kZ - zpos) * R2)) - atan((x[0] - xpos + fw/2) * (x[1] - ypos - fl/2) / ((kZ - zpos) * R1)) + atan((x[0] - xpos - fw/2) * (x[1] - ypos - fl/2) / ((kZ - zpos) * R3));
	//binArea *= (TMath::Pi() / 4.0);

	//cout << "BINAREA = " << binArea << endl;
	Double_t number = intensity*binArea;
	return number;
}

inline Double_t AngerCameraAnalyzer::IntensityFunction1d(Double_t *x, Double_t *p)
{
	double xx[] = { x[0], 0 };
	double pp[] = { p[0], p[1], 0, p[2] };
	return IntensityFunction2d(xx, pp);
}


//by Shuya 160425
//inline void AngerCameraAnalyzer::GenIntensityTree(TTree* tin, TTree* tout)
inline void AngerCameraAnalyzer::GenIntensityTree(TTree* tin, TTree* tin2, TTree* tout)
{
	TTree* t = tin;
//by Shuya 160425
	TTree* t2 = tin2;
//by Shuya 160422.
	//TLeaf* lfX = t->GetLeaf("All_Event_Data.Xpos");
	TLeaf* lfX = t->GetLeaf("Xpos");
	if(!lfX) {
		Error("AngerCameraAnalyzer::GenIntensityTree", "BadLeafX");
		return;
	}

	DirRemember_t remember_Directory_;
	TTree* tanger = tout;

	//Comment by Shuya 160426. Using TClonesArray() is just needed in case numHits (=lfX->GetNdata()) is > 1.
	TClonesArray* angerArray = new TClonesArray("AngerCamera");
	tanger->Branch("hit", &angerArray);
	TClonesArray& ar = *angerArray;

	for(Long64_t eventNum = 0; eventNum < t->GetEntries(); ++eventNum) {
		angerArray->Clear();
		t->GetEntry(eventNum);
		Int_t numHits = lfX->GetNdata();
		//by Shuya 160426
		//t2->GetEntry(eventNum);
		//by Shuya 160425
		//cout << "TEST!!! " << t->GetEntries() << endl;
		//cout << "TEST2!!! " << numHits << endl;

		for(Int_t hit = 0; hit< numHits; ++hit) {
			AngerCamera* ac = new(ar[hit]) AngerCamera( Form("achit%i", hit),
																									Form("Anger camera: hit %i", hit),
																									kXbins, kX, kYbins, kY, kZ,
																									kPrcntAnthracene, kAttenLength, kTimeRes );
			//by Shuya 160425
			//bool success = ac->FillHit(t, hit);
			bool success = ac->FillHit(t, t2, hit, eventNum);
			if(!success) return;
		}

		tanger->Fill();
	}

	tanger->AutoSave();
}


//by Shuya 160426
//inline void AngerCameraAnalyzer::CalculatePositions(TTree* tin, TTree* tout, Int_t method,
inline void AngerCameraAnalyzer::CalculatePositions(TTree* tin, TTree* tin2, TTree* tout, Int_t method,
																										Long64_t numEvents, Long64_t firstEvent,
																										const char* fitOptions)
{
	TTree* tanger = tin;
	if(numEvents > tanger->GetEntries())
		numEvents = tanger->GetEntries();

	//by Shuya 160426
	TTree* t = tin2;
	double	xpos = 0.0;
	double	ypos = 0.0;
	double	zpos = 0.0;
	//by Shuya 160509
	double	e = 0.0;

	//NOTE!! By Shuya 160427. The units are cm and MeV.
	double meanx(0),meany(0),meanz(0),rmsx(0),rmsy(0),meane(0),meant(0),chi2(0),rmsz(0),rmsr(0),rmse(0);
	tout->Branch("meanx", &meanx, "meanx/D");
	tout->Branch("meany", &meany, "meany/D");
	tout->Branch("meanz", &meanz, "meanz/D");
	tout->Branch("meane", &meane, "meane/D");
	tout->Branch("meant", &meant, "meant/D");
	tout->Branch("rmsx",  &rmsx,  "rmsx/D");
	tout->Branch("rmsy",  &rmsy,  "rmsy/D");
	//by Shuya 160427
	tout->Branch("rmsz",  &rmsz,  "rmsz/D");
	tout->Branch("rmsr",  &rmsr,  "rmsr/D");
	//by Shuya 160509
	tout->Branch("rmse",  &rmse,  "rmse/D");

	tout->Branch("chi2",  &chi2,  "chi2/D");

	std::auto_ptr<TF1> fpos1( Create_fpos1("fpos1") );
	std::auto_ptr<TF2> fpos2( Create_fpos2("fpos2") );
	fpos1->SetParameters(1, 0, 0);
	fpos2->SetParameters(1, 0, 0, 0);
	// fpos2->SetNpx(1000);
	// fpos2->SetNpy(1000);
	
	//by Shuya 160503
	std::auto_ptr<TF2> fpos2_fit( Create_fpos2("fpos2_fit") );
	TCanvas* cD[10000];
	char	cName[500];
	char	cName2[500];
	//by Shuya 160505
	double	nphotons;

	//by Shuya 160524
	int	flag = 0;

	for(Long64_t i=0; i< numEvents; ++i) {
		{
			//by Shuya 160524
			meanx = 0.;
			meany = 0.;
			meanz = 0.;
			rmsx = 0.;
			rmsy = 0.;
			rmsz = 0.;
			meane = 0.;
			meant = 0.;
			chi2 = 0.;
			rmsr = 0.;
			rmse = 0.;

			//by Shuya 160524
			flag = 0;

			TH1F ht("HT", "", 300, 0, 300);
			Long64_t n = tanger->Project("HT", "fT", "fHit == 0", "", 1, i);
			meant = TMath::Mean(n, tanger->GetV1());

			//by shuya 160426
			t->GetEntry(i);
			TLeaf* lfX = t->GetLeaf("Xpos");
			//Comments by Shuya 160426. I may need to set like lfX->GetValue(hit) as above, but I didn't....
			//Int_t numHits = lfX->GetNdata();
			TLeaf* lfY = t->GetLeaf("Ypos");
			TLeaf* lfZ = t->GetLeaf("Zpos");
			xpos = lfX->GetValue() / 10.0;	//mm->cm 
			ypos = lfY->GetValue() / 10.0;  //mm->cm
			zpos = lfZ->GetValue() / 10.0;  //mm->cm and set the front surface at z=0.
			//cout << "TEST!!!!! " << xpos << " " << ypos << endl;
			//by Shuya 160505
			TLeaf* lfPht = t->GetLeaf("eng_LXe_PhotonTotal");
			nphotons = lfPht->GetValue();         
			//by Shuya 160509
			TLeaf* lfE = t->GetLeaf("eng_LXe");
			e = lfE->GetValue();         
		}
		startSwitch:
		switch(method) {
		case 0:  // xy histogram mean, rms
			{
				TH2F hxy("HXY", "", kXbins, -kX/2, +kX/2, kYbins, -kY/2, +kY/2);
				tanger->Project("HXY", "fY:fX", "fIres*(fHit == 0)", "", 1, i);
				meanx = hxy.GetMean(1);
				meany = hxy.GetMean(2);
				rmsx = hxy.GetRMS(1);
				rmsy = hxy.GetRMS(2);
				break;
			}
		case 1:   // x, y histograms mean, rms
			{
				TH1F hx("HX", "", kXbins, -kX/2, +kX/2);
				TH1F hy("HY", "", kYbins, -kY/2, +kY/2);
				tanger->Project("HX", "fX", "fIres * (fHit == 0)", "", 1, i);
				tanger->Project("HY", "fY", "fIres * (fHit == 0)", "", 1, i);
				meanx = hx.GetMean();
				meany = hy.GetMean();
				rmsx = hx.GetRMS();
				rmsy = hy.GetRMS();
				break;
			}
		case 2:   // x, y histograms fit gaus "W"
			{
				TH1F hx("HX", "", kXbins, -kX/2, +kX/2);
				TH1F hy("HY", "", kYbins, -kY/2, +kY/2);
				tanger->Project("HX", "fX", "fIres * (fHit == 0)", "", 1, i);
				tanger->Project("HY", "fY", "fIres * (fHit == 0)", "", 1, i);
				meanx = meany = rmsx = rmsy = -1;

				if(hx.Integral() && hy.Integral()) {
					TFitResultPtr fitX = hx.Fit("gaus", fitOptions);
					TFitResultPtr fitY = hy.Fit("gaus", fitOptions);
					if(fitX->GetParams() && fitY->GetParams()) {
						meanx = fitX->GetParams()[1];
						meany = fitY->GetParams()[1];
						rmsx  = fitX->GetParams()[2];
						rmsy  = fitY->GetParams()[2];
					}
				}
				break;
			}
		case 3:   // x, y histograms fit model function
			{
				if(i != 0 && i%100 == 0) { cout << i << "... "; flush(cout); }
				if(i == numEvents - 1) { cout << endl; }
				TH1F hx("HX", "", kXbins, -kX/2, +kX/2);
				TH1F hy("HY", "", kYbins, -kY/2, +kY/2);

				meanx = meany = rmsx = rmsy = -1;
				tanger->Project("HX", "fX", "(fIres/10) * (fHit == 0)", "", 1, i);
				tanger->Project("HY", "fY", "(fIres/10) * (fHit == 0)", "", 1, i);
				if(hx.Integral() && hy.Integral()) {

					Double_t I = hx.Integral() / 10;
					double E = I / (kPrcntAnthracene*kNumAnthracene);
// 					fpos2->SetParameters(E, hxy.GetMean(1), hxy.GetMean(2), 0);
					fpos1->SetParameters(E, hx.GetMean(), 0);
					TFitResultPtr fitX = hx.Fit(fpos1.get(), fitOptions);

					fpos1->SetParameters(E, hy.GetMean(), 0);
					TFitResultPtr fitY = hy.Fit(fpos1.get(), fitOptions);
					if(fitX->GetParams() && fitY->GetParams()) {
						meanx = fitX->GetParams()[1];
						meany = fitY->GetParams()[1];
						rmsx  = fitX->GetParams()[2];
						rmsy  = fitY->GetParams()[2];
					}
				}
				break;
			}
		case 4:   // xy histogram fit model function
			{
				if(i && (i%100 == 0 || i< 100) ) { cout << i << "... "; flush(cout); }
				if(i == numEvents - 1) { cout << endl; }
				if(i == 0)
					Info("AngerCameraAnalyzer::CalculatePositions", "Fit option: \"%s\"", fitOptions);

				// Make histogram of photon intensity vs. X,Y position of PMTs
				//
				TH2F hxy("HXY", "", kXbins, -kX/2, +kX/2, kYbins, -kY/2, +kY/2);
				//by Shuya 160503
				hxy.GetXaxis()->SetTitle("X positon (cm)");
				hxy.GetXaxis()->SetLabelOffset(0.05);
				hxy.GetYaxis()->SetTitle("Y positon (cm)");
				hxy.GetYaxis()->SetLabelOffset(0.05);
				//hxy.GetZaxis()->SetTitle("Counts/bin");
				hxy.GetZaxis()->SetLabelSize(0.02);
				hxy.GetZaxis()->SetLabelOffset(0.01);
				//tanger->Project("HXY", "fY:fX", "fIres*(fHit == 0)", "", 1, i);
				//by Shuya 160425. fIres gives you a Poisson Distribution values from fI. But I already corrected for it in the Geant4 simulation. So not necessary here.
				//tanger->Project("HXY", "fY:fX", "fIres", "", 1, i);
				tanger->Project("HXY", "fY:fX", "fI", "", 1, i);
				meanx = meany = rmsx = rmsy = -11;
				meanz = rmsz = -11; // by Shuya 160524.
					//by Shuya 160503
					//hxy.Draw("");
					//hxy.SaveAs("test.png");

				Double_t I = hxy.Integral();
				//cout << "TEST NUMPHOTONS!! " << i << " " << nphotons << endl;
				//cout << "TEST PMTBACKHIT!! " << i << " " << I << endl;
				if(I != 0 && TMath::IsNaN(I) == 0) {
				//by Shuya 160524
				//rmsz = -9.0;
				//if(I >= 1000 && TMath::IsNaN(I) == 0) {

					//by shuya 160524
					//flag = 1;

					// Fit with analytic intensity function //
					double E = I / (kPrcntAnthracene*kNumAnthracene);
					//by Shuya 160425. Assuming most of neutron collisions take place nearby (0,0). Note GetMean gives you bin number (so, (0.0) is 4-5th bin) 
					//fpos2->SetParameters(E, hxy.GetMean(1), hxy.GetMean(2), kZ/2);
					//by Shuya 160427. I changed like this because I set the center of the detector at z=0.
					fpos2->SetParameters(E, hxy.GetMean(1), hxy.GetMean(2), 0.);
					//fpos2->SetParameters(10.0, hxy.GetMean(1), hxy.GetMean(2), 0.);
					//fpos2->SetParameters(E, 0., 0., kZ/2);
				//cout << "TEST!! " << hxy.GetMean(1) << endl;
				//cout << "TEST2!! " << hxy.GetMean(2) << endl;
					TFitResultPtr fit = hxy.Fit(fpos2.get(), fitOptions);
					if(fit->GetParams()) {
						meane = fit->GetParams()[0];
						meanx = fit->GetParams()[1];
						meany = fit->GetParams()[2];
						meanz = fit->GetParams()[3];
						//by Shuya 160426
						//rmsx  = fit->GetParams()[3];
						//rmsy  = fit->GetParams()[3];
						rmsx  = meanx - xpos;
						rmsy  = meany - ypos;
						rmsz  = meanz - zpos;
						rmsr  = sqrt(pow(rmsx,2.)+pow(rmsy,2.)+pow(rmsz,2.));
						rmse  = meane - e;
						chi2  = fit->Chi2();
					}

				//by Shuya 160524.
				if(I>1000 && rmsz>2.0)	cout << "TEST PMTBACKHIT!! " << i << " " << I << " " << meanz << " " << rmsz << endl;

				//BY SHUYA 160503. Draw histograms to compare data and fitting. 
				//And just draw and save once in a while
				/*
					if(i%10==1)
					{

 	           				sprintf(cName,"cQ%d",i);
						//cD[i]->GetFrame()->SetFillColor(21);
						//cD[i]->GetFrame()->SetBorderMode(1);
						//cD[i]->GetFrame()->SetBorderSize(200);
           
						AngerCameraAnalyzer aca_fit("", "", kXbins, kX, kYbins, kY, kZ, kPrcntAnthracene, kAttenLength, kTimeRes);
						TH2F hxy_fit("HXY_Fit", "", kXbins, -kX/2, +kX/2, kYbins, -kY/2, +kY/2);
						hxy_fit.GetXaxis()->SetTitle("X positon (cm)");
						hxy_fit.GetXaxis()->SetLabelOffset(0.05);
						hxy_fit.GetYaxis()->SetTitle("Y positon (cm)");
						hxy_fit.GetYaxis()->SetLabelOffset(0.05);
						//hxy_fit.GetZaxis()->SetTitle("Counts/bin");
						hxy_fit.GetZaxis()->SetLabelSize(0.02);
						hxy_fit.GetZaxis()->SetLabelOffset(0.01);

						for(int j = 0; j< kNumBins; ++j)
						{
							//Comments by Shuya 160422. vars[] give pmt positions. So kZ = 30 (cm) is correct because the pmts are placed behind the detector.

							double vars[] = { fX[j], fY[j], kZ };
							double params[] = { meane, meanx, meany, meanz };

							double I = aca_fit.IntensityFunction2d(vars, params);
							//cout << fX[j] << " " << fY[j] << " " << I << endl;
		
							//for(int k=0;k<(int)I;k++)	hxy_fit.Fill(fX[j],fY[j]);
							hxy_fit.Fill(fX[j],fY[j],(int)I);
						}

                        			cD[i] = new TCanvas(cName,cName, 200,40,1150, 1150);

						cD[i]->Divide(2,2);
						cD[i]->cd(1);
						//gStyle->SetPalette(kBird);

						hxy.Draw("lego2 Z");
						cD[i]->cd(2);
						//hxy_fit.Draw("lego2");
						hxy_fit.Draw("lego2 Z");
						cD[i]->cd(3);

   						gPad->SetFrameFillColor(17);
   						gPad->SetTheta(3.77);
   						gPad->SetPhi(2.9);

						//if you want Stack drawing... 
						//THStack *h = new THStack("a","Stacked 2D");
						//hxy.SetFillColor(38);
						//hxy_fit.SetFillColor(46);
						//h->Add(&hxy);
						//h->Add(&hxy_fit);
						//Comment by Shuya 160503. nostack option is not valid in 2Dhistogram...
						//h->Draw("lego1 nostack");
						//jh->Draw("lego1");
						hxy.Draw("lego2");

						cD[i]->cd(4);

   						gPad->SetFrameFillColor(17);
   						gPad->SetTheta(3.77);
   						gPad->SetPhi(2.9);
						//h->Draw("lego2 nostack");
						//fit->Draw("FUNC");
						hxy_fit.Draw("lego2");

					//by Shuya 160503. If you want any legends...
      						//TLegend *leg = new TLegend(0.1,0.7,0.48,0.9);
      						//leg->SetHeader("The Legend Title");
            					//char legName[100];
                  				//char leg2Name[100];
                              			//sprintf(legName,"test");
                                          	//leg->AddEntry("HXY",legName,"f");
                                                //sprintf(leg2Name,"test2");
                                                //leg->AddEntry("fpos2_fit",leg2Name,"f");
						//leg->Draw("same");

						sprintf(cName2,"Data_Fit_Comparison_Evt%d.png",i);
						cD[i]->SaveAs(cName2);
					}
				*/
				}
				break;
			}
		case 5:   // single bar histogram fit model function
			{
				if(i == 0)
					Info("AngerCameraAnalyzer::CalculatePositions", "Fit option: \"%s\"", fitOptions);
				TH2F hxy("HXY", "", kXbins, -kX/2, +kX/2, kYbins, -kY/2, +kY/2);
				tanger->Project("HXY", "fY:fX", "fIres*(fHit == 0)", "", 1, i);
				meanx = meany = rmsx = rmsy = -1;

				Double_t I = hxy.Integral();
				if(I != 0) {

					Int_t bx, by, bz;
					hxy.GetMaximumBin(bx, by, bz);
					TH1* hy = hxy.ProjectionX("_px", by, by);

					double E = I / (kPrcntAnthracene*kNumAnthracene);
					fpos1->SetParameters(E, hy->GetMean(1), 0);
					TFitResultPtr fit = hy->Fit(fpos1.get(), fitOptions);

					if(fit->GetParams()) {
						meane = fit->GetParams()[0];
						meanx = fit->GetParams()[1];
						meany = 0;
						meanz = fit->GetParams()[2];
						rmsx  = fit->GetParams()[2];
						rmsy  = 0;
					}

					hy->Delete();
				}
				break;
			}
		default:  // xy histogram mean, rms
			{
				method = 0;
				goto startSwitch;
				break;
			}
		}

		tout->Fill();
		//if(flag==1)	tout->Fill();
	}

	tout->AutoSave();
}


//by Shuya 160425
//inline TFile* AngerCameraAnalyzer::CompleteAnalysis(TTree* tsim,
inline TFile* AngerCameraAnalyzer::CompleteAnalysis(TTree* tsim, TTree* tsim2, 
																										const char* outputfile,
																										int numMethods, int* methods,
																										Long64_t numEvents, Long64_t firstEvent,
																										const char* fitOptions)
{
		TFile *fout = new TFile(outputfile, "recreate");
		if(!fout || fout->IsZombie()) {
			if(fout) { fout->Close(); delete fout; }
			return 0;
		}
		if(numEvents > tsim->GetEntries())
			numEvents = tsim->GetEntries();
		TTree* tnew = tsim->CopyTree("", "", numEvents, firstEvent);
	//by Shuya 160425
		TTree* tnew2 = tsim2->CopyTree("", "", numEvents, firstEvent);
		TTree* tanger = new TTree("tanger", "Anger Camera Pixels");
		TreeStyle(tnew);
	//by Shuya 160425
		TreeStyle(tnew2);
		TreeStyle(tanger);


		std::cout << "Generating Anger Camera pixel tree...\n";
	//by Shuya 160425
		//GenIntensityTree(tnew, tanger);
		GenIntensityTree(tnew, tnew2, tanger);
		tnew->AddFriend(tanger, "tanger", true);
		//tnew->AddFriend(tnew2, "t2", true);

		for(int i=0; i< numMethods; ++i) {
			int method = methods[i];
			std::cout << "Generating position analyzed tree, method " << method << "...\n";
			TTree* tpos = new TTree(Form("tpos%i", method),
															Form("Anger Camera Positions, method %i", method));
			TreeStyle(tpos);
			//by Shuya 160422
			//CalculatePositions(tanger, tpos, method, numEvents, firstEvent, fitOptions);
			CalculatePositions(tanger, tsim, tpos, method, numEvents, firstEvent, fitOptions);
			tnew->AddFriend(tpos, Form("tpos%i", i), true);
		}

		tnew->Write();
//by Shuya 160425
		tnew2->Write();
		// fout.Close();
		std::cout << "Done!\n";

	return fout; // TFile::Open(outputfile);
}

	
// struct AngerParams_t {
// 	TTree* tsim;
// 	const char* outFileName;
// 	int seed;
// 	int bins;
// 	int numMethods;
// 	int* methods;
// };

// void *runAngerThread(void* p)
// {
// 	AngerParams_t *par = (AngerParams_t*)p;
// 	AngerCameraAnalyzer aca("aca", "", par->bins, 200, par->bins, 200, 30, 0.6*0.2, -1, 1);
// 	aca.SetSeed(par->seed);
// 	TFile* fnew = aca.CompleteAnalysis(par->tsim, 
// }
	

TF1* florentz = new TF1("lorentz", "[0]*ROOT::Math::cauchy_pdf(x,[2]/2.,[1])", -10, 10);


#include "TFitEditor.h"
void anger(int nthreads = 1)
{
	florentz->SetParameters(10000,0,1);
	florentz->SetNpx(10000);

	// ROOT::Math::MinimizerOptions::SetDefaultMinimizer("GSLMultiMin", "conjugatefr"); 
	// by Shuya 160422
	//int bins[] = { 8 }; // 5, 8, 10, 15, 30 };
	//int bins[] = { 10 }; // 5, 8, 10, 15, 30 };
	Int_t bins[] = { 4 }; // 5, 8, 10, 15, 30 };
	int meth [] = {4} ; //, 5, 0};

	TFile* fileSim = TFile::Open("/home/gacgroup/shuyaota/Geant4/tntsim3/build/LXeDataTree.root");
	TTree* tsim = (TTree*)fileSim->Get("t");
	TTree* tsim2 = (TTree*)fileSim->Get("t2");
	TFile* fnew = 0;

	//const double TIME_RES = 0;
	Double_t TIME_RES = 0;
//by Shuya 160422.
	//const double ATTEN_LENGTH = -1;
	//const double ATTEN_LENGTH = 1;
	//by Shuya for BC404.
	const Double_t ATTEN_LENGTH = 160;
	//by Shuya for BC519.
	//const Double_t ATTEN_LENGTH = 100;

	for(size_t i=0; i< sizeof(bins)/sizeof(bins[0]); ++i) {
		cout << "Analyzing with " << bins[i] << " bins\n";
//by Shuya 160422
		//cout << "Number of Bins " <<  sizeof(bins)/sizeof(bins[0]) << " bins\n";


		//by Shuya 160425
		//AngerCameraAnalyzer aca("aca", "anger camera analyzer", bins[i], 200., bins[i], 200., 30., 0.6*0.2, ATTEN_LENGTH, TIME_RES);
		//AngerCameraAnalyzer aca("aca", "anger camera analyzer", bins[i], 200., bins[i], 200., 30., 1.0, ATTEN_LENGTH, TIME_RES);
		//by Shuya 160427. I set zsize = 15 because I set the detector's center at z=0 in my geant4 simulation.
		AngerCameraAnalyzer aca("aca", "anger camera analyzer", bins[i], 30., bins[i], 30., 5., 1.0, ATTEN_LENGTH, TIME_RES);
		//cout << "!!" << endl;
		aca.SetSeed(2);
		//by Shuya 160425
		//fnew = aca.CompleteAnalysis( tsim,
		fnew = aca.CompleteAnalysis( tsim, tsim2,
																 Form("angerAna-12prcnt-%ibins-nohit0.root",
																			bins[i]),
																 sizeof(meth)/sizeof(meth[0]), meth,
																 tsim->GetEntries(), 0,
																 "qnsL");
//		fnew->Close();
	}
	
	fileSim->Close();
	if(fnew) fnew->cd();
}




#if 0
struct Digi : public TObject {
	Double_t fTime;
	Double_t fSignal;

	ClassDef(Digi, 1);
};

const char* AddDigiTree(const char* file)
{
	TFile* f = new TFile(file, "update");
	TTree* t = (TTree*)f->Get("t");
	TTree* tanger = (TTree*)f->Get("tanger");
	TTree* tdigi = new TTree("tdigi", "digitizer tree");
	Double_t digi[64][256];
	Double_t time[256], x[64], y[64];
	// for(int i=0; i< 64; ++i) {
	// 	tdigi->Branch(Form("digi_%i", i), digi[i], Form("digi_%i[256]/D,i"));
	// 	tdigi->Branch("digi", digi, "digi[64][256]/D");
	// 	tdigi->Branch("time", time, "time[256]/D");
	// 	tdigi->Branch("x", x, "x[64]/D");
	// 	tdigi->Branch("y", y, "y[64]/D");
	// }

	TClonesArray* digiArray[64];
	for(int i=0; i< 64; ++i) {
		digiArray[i] = new TClonesArray("Digi");
		tdigi->Branch(Form("digi%d", i), &(digiArray[i]));
	}

	TLeaf* lfI = tanger->GetLeaf("hit.fIres[kNumBins]");
	TLeaf* lfT = t->GetLeaf("All_Event_Data.HitTime");
	if(!lfI) return 0;
	if(!lfT) return 0;

	double bw = 1e3/250;
	{
		TH2F h("", "", 8, -100, 100, 8, -100, 100);
		for(int j=0; j< 256; ++j) {
			time[j] = bw*(j+1) - bw/2;
		}
		int iii = 0;
		for(int j=0; j< 8; ++j) {
			for(int k=0; k< 8; ++k) {
				y[iii] = h.GetYaxis()->GetBinCenter(1+j);
				x[iii] = h.GetXaxis()->GetBinCenter(1+k);
				++iii;
			}
		}
	}

	for(int i=0; i< t->GetEntries(); ++i) {
		if(i && i%100 == 0) { cout << i << "... "; flush(cout); }
		t->GetEntry(i);
		tanger->GetEntry(i);
		for(int j=0; j< 64; ++j) {
			std::fill(digi[j], digi[j] + 256, 0);
		}

		// hits
		for(int j=0; j< lfT->GetNdata(); ++j) {
			Double_t T = lfT->GetValue(j);
			// TH2F h("H", "", 8, -100, 100, 8, -100, 100);
			// tanger->Project("H", "fY:fX", Form("fIres * (fHit == %i)", j), "goff", 1, i);
			tanger->Draw("fIres", Form("fHit == %i", j), "goff", 1, i);

			// bins
			for(int pix = 0; pix < 64; ++pix) {
				Double_t I = tanger->GetV1()[pix];
				// Double_t I = h.GetBinContent(pix+1);
				for(int k=0; k< 256; ++k) {
					double tau = 1, tau1 = 4; // time constants 4sec, 1sec
					double I0 = I / (tau1 - tau); // peak intensity
					double tbin = time[k] - T;
					double funcT = -I0 * (exp(-tbin / tau) - exp(-tbin / tau1));
					if(funcT > 0)
						digi[pix][k] += funcT;
					
					// if( T > (time[pix][k] - bw/2) && T < (time[pix][k] + bw/2) ) {
					// 	digi[pix][k] += I;
					// }
				}
			}
		}

		for(int ii=0; ii< 64; ++ii) {
			digiArray[ii]->Clear();
			for(int jj=0; jj< 256; ++jj) {
				if(digi[ii][jj] > 0) {
					TClonesArray& ar = *(digiArray[ii]);
					Digi *d = new(ar[jj]) Digi();
					d->fTime   = time[jj];
					d->fSignal = digi[ii][jj];
				}
			}
		}

		tdigi->Fill();
	}
	cout << "Done!" << endl;

	tdigi->AutoSave();
	t->AddFriend(tdigi, "tdigi");
	t->AutoSave();
	f->Close();
	return file;
}
#endif
