#ifndef FakeFatjetScaleFactorProvider_h
#define FakeFatjetScaleFactorProvider_h

#include <TLorentzVector.h>
#include <TVector3.h>
#include <TMath.h>
#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/PtEtaPhiM4D.h"
#include "Math/GenVector/VectorUtil.h"
#include "Math/Point3D.h"
/*
 * This class provide scale factor and uncertainties of fake fatjet, please check https://indico.cern.ch/event/1421212/contributions/5976640/attachments/2868594/5021713/CR_fake_fatjet_flavor_pt_reweighted.pdf
 */

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > LV;
class FakeFatjetScaleFactorProvider{
	private:
		constexpr static float b_sf_central[4][3]={
			{1.383,1.406,1.36},
			{1.089,1.102,1.208},
			{1.172,1.455,1.37},
			{1.046,1.232,1.405},
		};
		constexpr static float b_sf_error[4][3]={
			{0.09763,0.1727,0.2444},
			{0.146,0.2347,0.3096},
			{0.1638,0.2186,0.369},
			{0.1625,0.2343,0.2796},
		};
		constexpr static float isr_sf_central[4][3]={
			{1.263,1.208,1.157},
			{1.302,1.25,1.227},
			{1.174,1.053,1.274},
			{1.263,1.085,0.9987},
		};
		constexpr static float isr_sf_error[4][3]={
			{0.0786,0.176,0.2856},
			{0.09267,0.1987,0.318},
			{0.1259,0.2409,0.4176},
			{0.1271,0.2547,0.3814},
		};

	public:
		FakeFatjetScaleFactorProvider();
		int type(vector<LV> const &genp4, vector<int> const &genpdgid, vector<int> const &genmotherid,LV const &fatjetp4);
		float eval(int year,int type,int WPid);
		float eval_up(int year,int type,int WPid);
		float eval_down(int year, int type,int WPid);
};

FakeFatjetScaleFactorProvider::FakeFatjetScaleFactorProvider(){}
int FakeFatjetScaleFactorProvider::type(vector<LV> const &genp4, vector<int> const &genpdgid, vector<int> const &genmotherid,LV const &fatjetp4){
	bool istrue=false;
	bool isb=false;
	for(unsigned igen=0;igen<genpdgid.size();igen++){
		if(!(((abs(genmotherid.at(igen))==24) || (abs(genmotherid.at(igen))==23))&&(abs(genpdgid.at(igen))<=6))) continue;
		if(ROOT::Math::VectorUtil::DeltaR(genp4.at(igen),fatjetp4)<0.8)
			return -1;
	}

	for(unsigned igen=0;igen<genpdgid.size();igen++){
		if(!(abs(genpdgid.at(igen))==5)) continue;
		if(ROOT::Math::VectorUtil::DeltaR(genp4.at(igen),fatjetp4)<0.8){
			return 1;
		}
	}
	return 2;
}

float FakeFatjetScaleFactorProvider::eval(int year,int type,int WPid){
	//return 1, if true fatjet or year not found
	/*
		* year follows the definition in looper
		* https://github.com/yulunmiao/VVVNanoLooper/blob/ultralegacy/src/Process_Common.cc#L2371
		* use 10 for 2016pre, 2016 for 2016post
		*/
	int yidx;
	if(year==2018) yidx=0;
	else if(year==2017) yidx=1;
	else if(year==2016) yidx=2;
	else if(year==10) yidx=3;
	else return 1.;

	/*
		* WPid follows the definition in looper
		* https://github.com/yulunmiao/VVVNanoLooper/blob/ultralegacy/src/Process_Common.cc#L2364-L2366
		* 1 for loose, 2 for medium, 3 for tight
		*/
	if(type==1){
		return b_sf_central[yidx][WPid-1];
	}
	else if(type==2){
		return isr_sf_central[yidx][WPid-1];
	}
	return 1.;
}

float FakeFatjetScaleFactorProvider::eval_up(int year,int type,int WPid){
	int yidx;
	if(year==2018) yidx=0;
	else if(year==2017) yidx=1;
	else if(year==2016) yidx=2;
	else if(year==10) yidx=3;
	else return 1.;
	if(type==1){
		return b_sf_central[yidx][WPid-1]+b_sf_error[yidx][WPid-1];
	}
	else if(type==2){
		return isr_sf_central[yidx][WPid-1]+isr_sf_error[yidx][WPid-1];
	}
	return 1.;
}

float FakeFatjetScaleFactorProvider::eval_down(int year, int type,int WPid){
	int yidx;
	if(year==2018) yidx=0;
	else if(year==2017) yidx=1;
	else if(year==2016) yidx=2;
	else if(year==10) yidx=3;
	else return 1.;
	if(type==1){
		return b_sf_central[yidx][WPid-1]-b_sf_error[yidx][WPid-1];
	}
	else if(type==2){
		return isr_sf_central[yidx][WPid-1]-isr_sf_error[yidx][WPid-1];
	}
	return 1.;
}
#endif

