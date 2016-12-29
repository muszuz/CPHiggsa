//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 24 09:53:02 2013 by ROOT version 5.34/07
// from TTree Data/Pythia8 events
// found on file: Data.root
//////////////////////////////////////////////////////////

#ifndef Data_h
#define Data_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include <TAttLine.h>
#include <TParticle.h>

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxParticles = 3160;

class Data {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           Particles_;
   UInt_t          Particles_fUniqueID[kMaxParticles];   //[Particles_]
   UInt_t          Particles_fBits[kMaxParticles];   //[Particles_]
   Short_t         Particles_fLineColor[kMaxParticles];   //[Particles_]
   Short_t         Particles_fLineStyle[kMaxParticles];   //[Particles_]
   Short_t         Particles_fLineWidth[kMaxParticles];   //[Particles_]
   Int_t           Particles_fPdgCode[kMaxParticles];   //[Particles_]
   Int_t           Particles_fStatusCode[kMaxParticles];   //[Particles_]
   Int_t           Particles_fMother[kMaxParticles][2];   //[Particles_]
   Int_t           Particles_fDaughter[kMaxParticles][2];   //[Particles_]
   Float_t         Particles_fWeight[kMaxParticles];   //[Particles_]
   Double_t        Particles_fCalcMass[kMaxParticles];   //[Particles_]
   Double_t        Particles_fPx[kMaxParticles];   //[Particles_]
   Double_t        Particles_fPy[kMaxParticles];   //[Particles_]
   Double_t        Particles_fPz[kMaxParticles];   //[Particles_]
   Double_t        Particles_fE[kMaxParticles];   //[Particles_]
   Double_t        Particles_fVx[kMaxParticles];   //[Particles_]
   Double_t        Particles_fVy[kMaxParticles];   //[Particles_]
   Double_t        Particles_fVz[kMaxParticles];   //[Particles_]
   Double_t        Particles_fVt[kMaxParticles];   //[Particles_]
   Double_t        Particles_fPolarTheta[kMaxParticles];   //[Particles_]
   Double_t        Particles_fPolarPhi[kMaxParticles];   //[Particles_]

   // List of branches
   TBranch        *b_Particles_;   //!
   TBranch        *b_Particles_fUniqueID;   //!
   TBranch        *b_Particles_fBits;   //!
   TBranch        *b_Particles_fLineColor;   //!
   TBranch        *b_Particles_fLineStyle;   //!
   TBranch        *b_Particles_fLineWidth;   //!
   TBranch        *b_Particles_fPdgCode;   //!
   TBranch        *b_Particles_fStatusCode;   //!
   TBranch        *b_Particles_fMother;   //!
   TBranch        *b_Particles_fDaughter;   //!
   TBranch        *b_Particles_fWeight;   //!
   TBranch        *b_Particles_fCalcMass;   //!
   TBranch        *b_Particles_fPx;   //!
   TBranch        *b_Particles_fPy;   //!
   TBranch        *b_Particles_fPz;   //!
   TBranch        *b_Particles_fE;   //!
   TBranch        *b_Particles_fVx;   //!
   TBranch        *b_Particles_fVy;   //!
   TBranch        *b_Particles_fVz;   //!
   TBranch        *b_Particles_fVt;   //!
   TBranch        *b_Particles_fPolarTheta;   //!
   TBranch        *b_Particles_fPolarPhi;   //!

   Data(TTree *tree=0);
   virtual ~Data();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Data_cxx
Data::Data(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Data.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("Data.root");
      }
      f->GetObject("Data",tree);

   }
   Init(tree);
}

Data::~Data()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Data::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Data::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Data::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Particles", &Particles_, &b_Particles_);
   fChain->SetBranchAddress("Particles.fUniqueID", Particles_fUniqueID, &b_Particles_fUniqueID);
   fChain->SetBranchAddress("Particles.fBits", Particles_fBits, &b_Particles_fBits);
   fChain->SetBranchAddress("Particles.fLineColor", Particles_fLineColor, &b_Particles_fLineColor);
   fChain->SetBranchAddress("Particles.fLineStyle", Particles_fLineStyle, &b_Particles_fLineStyle);
   fChain->SetBranchAddress("Particles.fLineWidth", Particles_fLineWidth, &b_Particles_fLineWidth);
   fChain->SetBranchAddress("Particles.fPdgCode", Particles_fPdgCode, &b_Particles_fPdgCode);
   fChain->SetBranchAddress("Particles.fStatusCode", Particles_fStatusCode, &b_Particles_fStatusCode);
   fChain->SetBranchAddress("Particles.fMother[2]", Particles_fMother, &b_Particles_fMother);
   fChain->SetBranchAddress("Particles.fDaughter[2]", Particles_fDaughter, &b_Particles_fDaughter);
   fChain->SetBranchAddress("Particles.fWeight", Particles_fWeight, &b_Particles_fWeight);
   fChain->SetBranchAddress("Particles.fCalcMass", Particles_fCalcMass, &b_Particles_fCalcMass);
   fChain->SetBranchAddress("Particles.fPx", Particles_fPx, &b_Particles_fPx);
   fChain->SetBranchAddress("Particles.fPy", Particles_fPy, &b_Particles_fPy);
   fChain->SetBranchAddress("Particles.fPz", Particles_fPz, &b_Particles_fPz);
   fChain->SetBranchAddress("Particles.fE", Particles_fE, &b_Particles_fE);
   fChain->SetBranchAddress("Particles.fVx", Particles_fVx, &b_Particles_fVx);
   fChain->SetBranchAddress("Particles.fVy", Particles_fVy, &b_Particles_fVy);
   fChain->SetBranchAddress("Particles.fVz", Particles_fVz, &b_Particles_fVz);
   fChain->SetBranchAddress("Particles.fVt", Particles_fVt, &b_Particles_fVt);
   fChain->SetBranchAddress("Particles.fPolarTheta", Particles_fPolarTheta, &b_Particles_fPolarTheta);
   fChain->SetBranchAddress("Particles.fPolarPhi", Particles_fPolarPhi, &b_Particles_fPolarPhi);
   Notify();
}

Bool_t Data::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Data::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Data::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Data_cxx
