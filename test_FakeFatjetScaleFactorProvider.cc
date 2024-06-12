#include "FakeFatjetScaleFactorProvider.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<float> > LV;

int test_FakeFatjetScaleFactorProvider(){
	FakeFatjetScaleFactorProvider fakefatjetsfprovider;

        TChain *chain=new TChain("t");
        vector<LV> *fatjetp4=0,*genp4=0;
        vector<int> *genpdgid=0,*genmotheridx=0,*genmotherid=0;
        chain->SetBranchAddress("Common_fatjet_p4",             &fatjetp4);
        chain->SetBranchAddress("Common_gen_mother_idx",        &genmotheridx);
        chain->SetBranchAddress("Common_gen_mother_id", 	&genmotherid);
        chain->SetBranchAddress("Common_gen_pdgid",             &genpdgid);
        chain->SetBranchAddress("Common_gen_p4s",               &genp4);
	chain->Add("../MC/2018/TTDL.root");
	for(unsigned int i=0;i<chain->GetEntries();i++){
		chain->GetEntry(i);
		if(!(fatjetp4->size()>=0 && fatjetp4->at(0).pt()>200)) continue;
		LV fjp4=fatjetp4->at(0);
		int type = fakefatjetsfprovider.type(*genp4,*genpdgid,*genmotherid,fjp4);
		if(type==-1){
			cout<<"true fatjet, should use common branch"<<endl;
		}
		else if(type==1){
			cout<<"fake fatjet b, sf="<<fakefatjetsfprovider.eval(2018,type,1)
				<<",sf_up="<<fakefatjetsfprovider.eval_up(2018,type,1)
				<<",sf_dn="<<fakefatjetsfprovider.eval_down(2018,type,1)<<endl;
		}
		else if(type==2){
			cout<<"fake fatjet isr, sf="<<fakefatjetsfprovider.eval(2018,type,1)
				<<",sf_up="<<fakefatjetsfprovider.eval_up(2018,type,1)
				<<",sf_dn="<<fakefatjetsfprovider.eval_down(2018,type,1)<<endl;
		}
		else{
			cout<<"something wrong"<<endl;
		}
	}
	return 0;
}
