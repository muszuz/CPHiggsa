   #include <iostream>

   void pythia8(Int_t nev  = 1E4, Int_t ndeb = 1){
      const char *p8dataenv = gSystem->Getenv("PYTHIA8DATA"); 
      if (!p8dataenv) {
         const char *p8env = gSystem->Getenv("PYTHIA8"); 
         if (!p8env) {
            Error("pythia8.C", 
                  "Environment variable PYTHIA8 must contain path to pythia directory!");
            return;
         }
         TString p8d = p8env;
         p8d += "/xmldoc";
         gSystem->Setenv("PYTHIA8DATA", p8d);
      }
         
      char* path = gSystem->ExpandPathName("$PYTHIA8DATA");
      if (gSystem->AccessPathName(path)) {
            Error("pythia8.C", 
                  "Environment variable PYTHIA8DATA must contain path to $PYTHIA8/xmldoc directory !");
         return;
      }
       
   // Load libraries
      gSystem->Load("$PYTHIA8/lib/libpythia8");
      gSystem->Load("libEG");
      gSystem->Load("libEGPythia8");

   // Histograms
      TH1F* hMass = new TH1F("hMass", "Mass of #tau#tau",200,0,200);
      TH1D* marcin = new TH1D("marcin", "rozklad katowy", 6, 0, 3.1415); 

   // Array of particles
      TClonesArray* particles = new TClonesArray("TParticle", 1000);
   // Create pythia8 object
      TPythia8* pythia8 = new TPythia8();
       
   // Configure    
      pythia8->ReadString("Higgs:useBSM  = on");
      pythia8->ReadString("HiggsBSM:gg2A3 = on"); //Higgs production by gluon-gluon fusion
      pythia8->ReadString("36:m0 = 125");       //Higgs mass
      pythia8->ReadString("36:onMode = no");    //switch off all Higgs decay channels
      pythia8->ReadString("36:onIfMatch =  15 15"); //switch back on Higgs -> tau tau
      pythia8->ReadString("15:onMode = no");    //switch off all tau decay channels
      pythia8->ReadString("15:onIfMatch =  211 16"); //switch back on tau -> pi nu
   // Initialize     
      pythia8->Initialize(2212 /* p */, 2212 /* p */, 8000. /* TeV */);

   // Define the Tree
      TFile *file = new TFile("Data.root","RECREATE");
      TTree *tree = new TTree("Data","Pythia8 events");
      TBranch *branch = tree->Branch("Particles",particles);

   // Event loop
      for (Int_t iev = 0; iev < nev; iev++) {
         pythia8->GenerateEvent();
         if (iev < ndeb) pythia8->EventListing();
         pythia8->ImportParticles(particles,"All");
         Int_t np = particles->GetEntriesFast();
         //Fill the TTree with current data
         tree->Fill();

   // Particle loop
         TLorentzVector p4Sum;
         TLorentzVector piMinus,piPlus;
         TLorentzVector nuTau, nuTauBar;

         for (Int_t ip = 0; ip < np; ip++) {
            TParticle* part = (TParticle*) particles->At(ip);
            Int_t ist = part->GetStatusCode();
            // Positive codes are final particles.
            if (ist <= 0) continue;
            /// Look only at pions and neutrinos
            Int_t pdg = part->GetPdgCode();
             if(abs(pdg)!=211 && abs(pdg)!=16) continue;
             /// Select pions from tau decays
             Int_t motherId = part->GetMother(0);
             if(motherId<0) continue;
                  TParticle* mother = (TParticle*) particles->At(motherId);
             Int_t pdgMother = mother->GetPdgCode();     
             if(abs(pdg)==211 && abs(pdgMother)!=15) continue;
             ///////////////
             if(pdg==211 && piPlus.E()<1) piPlus = TLorentzVector(part->Px(),part->Py(),part->Pz(),part->Energy());
             if(pdg==-211 &&  piMinus.E()<1) piMinus = TLorentzVector(part->Px(),part->Py(),part->Pz(),part->Energy());

             if(pdg==16 && nuTau.E()<1) nuTau = TLorentzVector(part->Px(),part->Py(),part->Pz(),part->Energy());
             if(pdg==-16 && nuTauBar.E()<1) nuTauBar = TLorentzVector(part->Px(),part->Py(),part->Pz(),part->Energy());
         }

         // 
         //TVector3 cross_P1 = TVector3(piPlus.Px(),piPlus.Py(),piPlus.Pz()).Cross(TVector3(nuTauBar.Px(),nuTauBar.Py(),nuTauBar.Pz()));
         //TVector3 cross_P2 = TVector3(piMinus.Px(),piMinus.Py(),piMinus.Pz()).Cross(TVector3(nuTau.Px(),nuTau.Py(),nuTau.Pz()));
          // wyznaczam 4-wektor tau (suma 4-wektorów piPlus i nutaubar)
         TLorentzVector tauplus = TLorentzVector( piPlus.Px() + nuTauBar.Px(), piPlus.Py() + nuTauBar.Py(), 
            piPlus.Pz() + nuTauBar.Pz(), piPlus.Energy() + nuTauBar.Energy());
         TLorentzVector tauminus = TLorentzVector( piMinus.Px() + nuTau.Px(), piMinus.Py() + nuTau.Py(), 
            piMinus.Pz() + nuTau.Pz(), piMinus.Energy() + nuTau.Energy());
         // iloczyn wektorowy 
         TVector3 cross_tpp = TVector3(tauplus.Px(), tauplus.Py(), tauplus.Pz()).Cross(TVector3(piPlus.Px(), piPlus.Py(), piPlus.Pz()));
         TVector3 cross_tpm = TVector3(tauplus.Px(), tauplus.Py(), tauplus.Pz()).Cross(TVector3(-piMinus.Px(), -piMinus.Py(), -piMinus.Pz()));
         // wyznaczenie kata azymutalnego 

         //Double_t vec1[3] = {cross_P1.Px(), cross_P1.Py(),cross_P1.Pz()};
         //Double_t vec2[3] = {cross_P2.Px(), cross_P2.Py(),cross_P2.Pz()};
         //Double_t cth = cross_P1.Dot(cross_P2) / ( TMath::Normalize(vec1) * TMath::Normalize(vec2) );
         //Double_t th = TMath::ACos(cth);

         Double_t v_tpp[3] = {cross_tpp.Px(), cross_tpp.Py(), cross_tpp.Pz()};
         Double_t v_tpm[3] = {cross_tpm.Px(), cross_tpm.Py(), cross_tpm.Pz()};
         Double_t ct = cross_tpm.Dot(cross_tpp) / (TMath::Normalize(v_tpp) * TMath::Normalize(v_tpm));
         Double_t th = TMath::ACos(ct);
         // std::cout << "th = " << th << endl;
         marcin->Fill(th);


         p4Sum=piMinus+piPlus+nuTau+nuTauBar;     
         hMass->Fill(p4Sum.M());
      }

      pythia8->PrintStatistics();
      file->Write();
      delete file;
    
      TCanvas* c1 = new TCanvas("c1","Pythia8 test example",800,800);
      hMass->Draw();
      marcin->Draw();

    }
