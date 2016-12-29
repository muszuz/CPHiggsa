void readData(){

  // Array of particles
  TClonesArray* particles = new TClonesArray("TParticle", 1000);
  
  TFile *file = new TFile("Data.root","READ");
  TTree *tree = (TTree*)file->Get("Data");
  TBranch        *bParticles;  
  tree->SetBranchAddress("Particles", &particles_, &bParticles);

  return;

// Histograms
   TH1F* hMass = new TH1F("hMass", "Mass of #tau#tau",200,0,200);
    
    
// Event loop
   for (Int_t iev = 0; iev < nev; iev++) {
      pythia8->GenerateEvent();
      if (iev < ndeb) pythia8->EventListing();
      pythia8->ImportParticles(particles,"All");
      Int_t np = particles->GetEntriesFast();
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

      p4Sum=piMinus+piPlus+nuTau+nuTauBar;     
      hMass->Fill(p4Sum.M());
   }

   pythia8->PrintStatistics();
    
   TCanvas* c1 = new TCanvas("c1","Pythia8 test example",800,800);
   hMass->Draw();
 }
