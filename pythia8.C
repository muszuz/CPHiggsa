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


      TVector3 cross_P1 = TVector3(piPlus.Px(),piPlus.Py(),piPlus.Pz()).Cross(TVector3(nuTauBar.Px(),nuTauBar.Py(),nuTauBar.Pz()));
      TVector3 cross_P2 = TVector3(piMinus.Px(),piMinus.Py(),piMinus.Pz()).Cross(TVector3(nuTau.Px(),nuTau.Py(),nuTau.Pz()));
      using std::cout;
      using std::endl;
      cout << "piPlus.Px = " << piPlus.Px() << ", piPlus.Py = " << piPlus.Py() << " piPlus.Pz = " << piPlus.Pz() << endl;
      cout << "nuTauBar.Px = " << nuTauBar.Px() << ", nuTauBar.Py = " << nuTauBar.Py() << " nuTauBar.Pz = " << nuTauBar.Pz() << endl;
      cout << "piMinus.Px = " << piMinus.Px() << ", piMinus.Py = " << piMinus.Py() << " piMinus.Pz = " << piMinus.Pz() << endl;
      cout << "nuTau.Px = " << nuTau.Px() << ", nuTau.Py = " << nuTau.Py() << " nuTau.Pz = " << nuTau.Pz() << endl;
      cout << "cross_P1.Px = " << cross_P1.Px() << ", cross_P1.Py = " << cross_P1.Py() << " cross_P1.Pz = " << cross_P1.Pz() << endl;
      cout << "cross_P2.Px = " << cross_P2.Px() << ", cross_P2.Py = " << cross_P2.Py() << " cross_P2.Pz = " << cross_P2.Pz() << endl;


      Double_t cth = cross_P1.Dot(cross_P2) / ( TMath::Normalize({cross_P1.Px(), cross_P1.Py(),cross_P1.Pz()}) * TMath::Normalize({cross_P2.Px(), cross_P2.Py(),cross_P2.Pz()}) );
      cout << "cth = " << cth << endl;
      Double_t th = TMath::ACos(cth);
      std::cout << "th = " << th << endl << endl;

      p4Sum=piMinus+piPlus+nuTau+nuTauBar;     
      hMass->Fill(p4Sum.M());
   }

   pythia8->PrintStatistics();
   file->Write();
   delete file;
 
   TCanvas* c1 = new TCanvas("c1","Pythia8 test example",800,800);
   hMass->Draw();
 }
