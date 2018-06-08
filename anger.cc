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
	bool FillHit(TTree* t, Int_t hit);
	UInt_t GetSeed() { return gRandom ? gRandom->GetSeed() : -1; }
	void SetSeed(UInt_t seed) { if(gRandom) gRandom->SetSeed(seed); }

private:
	AngerCamera(const AngerCamera&) :
		TNamed("",""),
		fHit(0), kNumBins(0), kXbins(0), kYbins(0),
		fBin(0), fX(0), fY(0), fT(0), fI(0), fIres(0),
		kPrcntAnthracene(0.6), kAttenLength(-1), kTimeRes(1),
		kX(200), kY(200), kZ(30)
		// kDigitizerBins(256), kDigitizerRate(250)//, fDigi(0)
		{ }
	AngerCamera& operator= (const AngerCamera&) { return *this; }

public:
	static const Int_t kNumAnthracene = 17400; //!
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
	AngerCameraAnalyzer(const char* name, const char* title,
											Int_t xbins, Double_t xsize,
											Int_t ybins, Double_t ysize,
											Double_t zsize,
											Double_t prcntAnthracene,
											Double_t attenLength,
											Double_t timeResolution):
		AngerCamera(name, title, xbins, xsize, ybins, ysize, zsize,
								prcntAnthracene, attenLength, timeResolution)
		{ }

	void GenIntensityTree(TTree* tin, TTree* tout);
	void CalculatePositions(TTree* tin, TTree* tout, Int_t method,
													Long64_t numEvents = 0xffffffffffff,
													Long64_t firstEvent = 0,
													const char* fitOptions = "qns");
	TFile* CompleteAnalysis(TTree* tsim, const char* outputfile, int numMethods, int* methods,
													Long64_t numEvents = 0xffffffffffff, Long64_t firstEvent = 0,
													const char* fitOptions = "qns");
	TFile* CompleteAnalysis1(TTree* tsim, const char* outputfile, int method,
													 Long64_t numEvents = 0xffffffffffff, Long64_t firstEvent = 0,
													 const char* fitOptions = "qns")
		{ return CompleteAnalysis(tsim, outputfile, 1, &method, numEvents, firstEvent, fitOptions); }

	Double_t IntensityFunction1d(Double_t* x, Double_t *p);
	Double_t IntensityFunction2d(Double_t* x, Double_t *p);
	Double_t IntensityFunction3d(Double_t* x, Double_t *p);

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
	kPrcntAnthracene(0.6), kAttenLength(-1), kTimeRes(1),
	kX(200), kY(200), kZ(30),
	// kDigitizerBins(256), kDigitizerRate(250), // fDigi(0),
	fRandom(new TRandom3(1))
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
	fRandom(new TRandom3(1))
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

inline bool AngerCamera::FillHit(TTree* t, Int_t hit)
{
	if(hit == 0) {
		std::fill(fI, fI + kNumBins, 0);
		std::fill(fIres, fIres + kNumBins, 0);
		// for(int i=0; i< 64; ++i)
		// 	std::fill(fDigi[i], fDigi[i] + kDigitizerBins /*fDigiBinsTotal*/, 0);
	}

	TLeaf* lfX = t->GetLeaf("All_Event_Data.Xpos");
	TLeaf* lfY = t->GetLeaf("All_Event_Data.Ypos");
	TLeaf* lfZ = t->GetLeaf("All_Event_Data.Zpos");
	TLeaf* lfE = t->GetLeaf("All_Event_Data.eng_Tnt");
	TLeaf* lfT = t->GetLeaf("All_Event_Data.HitTime");

	if(!lfX) { Error("AngerCamera::Fill", "BadLeafX"); return false; }
	if(!lfY) { Error("AngerCamera::Fill", "BadLeafY"); return false; }
	if(!lfZ) { Error("AngerCamera::Fill", "BadLeafZ"); return false; }
	if(!lfE) { Error("AngerCamera::Fill", "BadLeafE"); return false; }
	if(!lfT) { Error("AngerCamera::Fill", "BadLeafT"); return false; }


	fHit = hit;
	double x = lfX->GetValue(hit) / 10;       // cm
	double y = lfY->GetValue(hit) / 10;       // cm
	double z = lfZ->GetValue(hit) / 10 - 185; // cm
	double e = lfE->GetValue(hit);            // MeV
	double time = lfT->GetValue(hit);
	double nphotons = e * kNumAnthracene * kPrcntAnthracene;

	AngerCameraAnalyzer aca("", "", kXbins, kX, kYbins, kY, kZ,
													kPrcntAnthracene, kAttenLength, kTimeRes);
	// TH1F hbin("", "", kDigitizerBins, 0, kDigitizerBins*1e3/kDigitizerRate);
	for(int bin = 0; bin< kNumBins; ++bin) {
		double vars[] = { fX[bin], fY[bin], kZ };
		double params[] = { e, x, y, z };
		double I = aca.IntensityFunction2d(vars, params);
		fI[bin]    = I;
		fIres[bin] = gRandom->Poisson(I);
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
	Double_t norm = p[0];
	Double_t xpos = p[1], ypos = p[2], zpos = p[3];
	Double_t R = sqrt(pow(x[0] - xpos, 2) + pow(x[1] - ypos, 2) + pow(kZ - zpos, 2));
	Double_t intensity =  // (photons / area)
		kNumAnthracene*kPrcntAnthracene / (4*TMath::Pi()*R*R);

	// attenuation
	if(kAttenLength > 0) {
		intensity *= exp(-R / kAttenLength);
	}

	Double_t binArea = kX*kY/kNumBins;
	Double_t number = intensity*binArea;
	return norm * number;
}

inline Double_t AngerCameraAnalyzer::IntensityFunction1d(Double_t *x, Double_t *p)
{
	double xx[] = { x[0], 0 };
	double pp[] = { p[0], p[1], 0, p[2] };
	return IntensityFunction2d(xx, pp);
}


inline void AngerCameraAnalyzer::GenIntensityTree(TTree* tin, TTree* tout)
{
	TTree* t = tin;
	TLeaf* lfX = t->GetLeaf("All_Event_Data.Xpos");
	if(!lfX) {
		Error("AngerCameraAnalyzer::GenIntensityTree", "BadLeafX");
		return;
	}

	DirRemember_t remember_Directory_;
	TTree* tanger = tout;

	TClonesArray* angerArray = new TClonesArray("AngerCamera");
	tanger->Branch("hit", &angerArray);
	TClonesArray& ar = *angerArray;

	for(Long64_t eventNum = 0; eventNum < t->GetEntries(); ++eventNum) {
		angerArray->Clear();
		t->GetEntry(eventNum);
		Int_t numHits = lfX->GetNdata();

		for(Int_t hit = 0; hit< numHits; ++hit) {
			AngerCamera* ac = new(ar[hit]) AngerCamera( Form("achit%i", hit),
																									Form("Anger camera: hit %i", hit),
																									kXbins, kX, kYbins, kY, kZ,
																									kPrcntAnthracene, kAttenLength, kTimeRes );
			bool success = ac->FillHit(t, hit);
			if(!success) return;
		}

		tanger->Fill();
	}

	tanger->AutoSave();
}


inline void AngerCameraAnalyzer::CalculatePositions(TTree* tin, TTree* tout, Int_t method,
																										Long64_t numEvents, Long64_t firstEvent,
																										const char* fitOptions)
{
	TTree* tanger = tin;
	if(numEvents > tanger->GetEntries())
		numEvents = tanger->GetEntries();

	double meanx(0),meany(0),meanz(0),rmsx(0),rmsy(0),meane(0),meant(0),chi2(0);
	tout->Branch("meanx", &meanx, "meanx/D");
	tout->Branch("meany", &meany, "meany/D");
	tout->Branch("meanz", &meanz, "meanz/D");
	tout->Branch("meane", &meane, "meane/D");
	tout->Branch("meant", &meant, "meant/D");
	tout->Branch("rmsx",  &rmsx,  "rmsx/D");
	tout->Branch("rmsy",  &rmsy,  "rmsy/D");
	tout->Branch("chi2",  &chi2,  "chi2/D");

	std::auto_ptr<TF1> fpos1( Create_fpos1("fpos1") );
	std::auto_ptr<TF2> fpos2( Create_fpos2("fpos2") );
	fpos1->SetParameters(1, 0, 0);
	fpos2->SetParameters(1, 0, 0, 0);
	// fpos2->SetNpx(1000);
	// fpos2->SetNpy(1000);

	for(Long64_t i=0; i< numEvents; ++i) {
		{
			TH1F ht("HT", "", 300, 0, 300);
			Long64_t n = tanger->Project("HT", "fT", "fHit == 0", "", 1, i);
			meant = TMath::Mean(n, tanger->GetV1());
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
				//tanger->Project("HXY", "fY:fX", "fIres*(fHit == 0)", "", 1, i);
				tanger->Project("HXY", "fY:fX", "fIres", "", 1, i);
				meanx = meany = rmsx = rmsy = -1;

				Double_t I = hxy.Integral();
				if(I != 0 && TMath::IsNaN(I) == 0) {

					// Fit with analytic intensity function //
					double E = I / (kPrcntAnthracene*kNumAnthracene);
					fpos2->SetParameters(E, hxy.GetMean(1), hxy.GetMean(2), kZ/2);
					TFitResultPtr fit = hxy.Fit(fpos2.get(), fitOptions);
					if(fit->GetParams()) {
						meane = fit->GetParams()[0];
						meanx = fit->GetParams()[1];
						meany = fit->GetParams()[2];
						meanz = fit->GetParams()[3];
						rmsx  = fit->GetParams()[3];
						rmsy  = fit->GetParams()[3];
						chi2  = fit->Chi2();
					}
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
	}

	tout->AutoSave();
}


inline TFile* AngerCameraAnalyzer::CompleteAnalysis(TTree* tsim,
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
		TTree* tanger = new TTree("tanger", "Anger Camera Pixels");
		TreeStyle(tnew);
		TreeStyle(tanger);


		std::cout << "Generating Anger Camera pixel tree...\n";
		GenIntensityTree(tnew, tanger);
		tnew->AddFriend(tanger, "tanger", true);

		for(int i=0; i< numMethods; ++i) {
			int method = methods[i];
			std::cout << "Generating position analyzed tree, method " << method << "...\n";
			TTree* tpos = new TTree(Form("tpos%i", method),
															Form("Anger Camera Positions, method %i", method));
			TreeStyle(tpos);
			CalculatePositions(tanger, tpos, method, numEvents, firstEvent, fitOptions);
			tnew->AddFriend(tpos, Form("tpos%i", i), true);
		}

		tnew->Write();
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
	int bins[] = { 8 }; // 5, 8, 10, 15, 30 };
	int meth [] = {4} ; //, 5, 0};

	TFile* fileSim = TFile::Open("/home/gacgroup/gchristian/packages/tntsim/build/tntsim1.root");
	TTree* tsim = (TTree*)fileSim->Get("t");
	TFile* fnew = 0;

	const double TIME_RES = 0;
	const double ATTEN_LENGTH = -1;

	for(size_t i=0; i< sizeof(bins)/sizeof(bins[0]); ++i) {
		cout << "Analyzing with " << bins[i] << " bins\n";
		AngerCameraAnalyzer aca("aca", "anger camera analyzer", bins[i], 200, bins[i], 200, 30,
														0.6*0.2, ATTEN_LENGTH, TIME_RES);
		aca.SetSeed(2);
		fnew = aca.CompleteAnalysis( tsim,
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
