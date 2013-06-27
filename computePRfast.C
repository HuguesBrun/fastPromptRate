
TFile *myFile = new TFile("prompt_rateElectrons.root","RECREATE");

TChain *chain = new TChain("treeClusterShape");




computePRfast(){
    chain->Add("/afs/cern.ch/work/h/hbrun/public/TnP_CLtree_DYJets.root");
    
    
    TString trigeringElectrons = "((absSCeta<1.479&&CL_sigmaIetaIeta<0.01&&CL_deltaPhiIn<0.15&&CL_deltaEtaIn<0.007&&CL_HoE<0.12&&(CL_isoECAL-1)/pt<0.2)||(absSCeta>=1.479&&CL_sigmaIetaIeta<0.03&&CL_deltaPhiIn<0.1&&CL_deltaEtaIn<0.009&&CL_HoE<0.1&&(CL_isoECAL)/pt<0.2)&&CL_isoHCAL<0.2&&CL_isoTracker<0.2)";
    TString looseID = trigeringElectrons+"&&CL_relatPFiso03<1&&CL_dZ<0.1&&CL_MVA>-0.1";
    TString tightID = looseID+"&&CL_relatPFiso03<0.15&&CL_dZ<0.04&&CL_MVA>0.5&&CL_passConversionVeto==0&&CL_hnHits<1";
    
    
    
    float ptBins[6]={10,15,20,25,50,200};
    float etaBins[4] = {0,  1.4442, 1.556, 2.5};
    
    TH2F *passingLooseID = new TH2F("passingLooseID","",3, etaBins, 5, ptBins);
    chain->Draw("pt:absSCeta>>passingLooseID",looseID+"&&mass>80&&mass<100&&eventMatched==1");//matched with MC truth and in the Z peak (similar with TnP)
    
    TH2F *passingTightID = new TH2F("passingTightID","",3, etaBins, 5, ptBins);
    chain->Draw("pt:absSCeta>>passingTightID",tightID+"&&mass>80&&mass<100&&eventMatched==1");//matched with MC truth and in the Z peak (similar with TnP)
    

    TH2F *promptRate = (TH2F*) passingTightID->Clone("promptRate");
    promptRate->Sumw2();
    promptRate->Divide(passingTightID, passingLooseID,1,1);
    
    
    //save in the file
    myFile->cd();
    promptRate->Write("effMC_Prompt_Rate");
    myFile->Close();
    
    
}
