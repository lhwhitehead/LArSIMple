#include "LArSIMpleSteppingAction.hh"
#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleEventAction.hh"
#include "G4SteppingManager.hh"
#include "G4EmSaturation.hh"
#include "G4LossTableManager.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"

//#include "CLHEP/Random/RandPoisson.h"
//#include "TRandom3.h"
//#include "TMath.h"

////#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LArSIMpleSteppingAction::LArSIMpleSteppingAction(LArSIMpleEventAction* evt) : fEventAction(evt)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LArSIMpleSteppingAction::~LArSIMpleSteppingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VProcess* LArSIMpleSteppingAction::GetCurrentProcess() {
  return fpSteppingManager->GetfCurrentProcess();
}

void LArSIMpleSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()=="LArBox")
  {
    LArSIMple3DEnergyDeposit energyDeposit;

    if((aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit()) < fEventAction->GetHitThreshold())
      return;

    const G4StepPoint *stepPoint = aStep->GetPreStepPoint();
    energyDeposit.SetPositionAndTime(stepPoint->GetPosition(),stepPoint->GetGlobalTime());

    const G4Track *track = aStep->GetTrack();
    int foldedTrackID = 0;
    int foldedTrackPDG = 0;
    this->GetFoldedTrackIDAndPDG(track,foldedTrackID,foldedTrackPDG);
    energyDeposit.SetParticleInfo(foldedTrackPDG,foldedTrackID);

    // Allow for Birk's suppression when considering energy
//    G4EmSaturation* birksSup = G4LossTableManager::Instance()->EmSaturation();
//    const double eVis = birksSup->VisibleEnergyDeposition(track->GetParticleDefinition(), track->GetMaterialCutsCouple(), aStep->GetStepLength(), aStep->GetTotalEnergyDeposit(), aStep->GetNonIonizingEnergyDeposit());
//    energyDeposit.SetEnergy(eVis);
    energyDeposit.SetEnergy(aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit());
    fEventAction->Add3DEnergyDeposit(energyDeposit);
  }
}

void LArSIMpleSteppingAction::GetFoldedTrackIDAndPDG(const G4Track *track, int &foldedTrackID, int &foldedTrackPDG)
{
  const int trackID = track->GetTrackID();
  const int trackPDG = track->GetParticleDefinition()->GetPDGEncoding();
  const int parentID = track->GetParentID();

  // Primary particle first
  if(parentID == 0)
  {
    foldedTrackID = trackID;
    foldedTrackPDG = trackPDG;
  }
  // Check if secondaries have processes we don't want to consider as particles
  else
  {
    std::string process = track->GetCreatorProcess()->GetProcessName();
    if(process.find("conv")           != std::string::npos
       || process.find("LowEnConversion") != std::string::npos
       || process.find("Pair")            != std::string::npos
       || process.find("compt")           != std::string::npos
       || process.find("Compt")           != std::string::npos
       || process.find("Brem")            != std::string::npos
       || process.find("phot")            != std::string::npos
       || process.find("Photo")           != std::string::npos
       || process.find("Ion")             != std::string::npos
       || process.find("annihil")         != std::string::npos)
    {
      foldedTrackID = parentID;
      foldedTrackPDG = fEventAction->GetPDGFromTrackID(parentID);
    }
    // Otherwise these are particles that we want to keep
    else
    {
      foldedTrackID = trackID;
      foldedTrackPDG = trackPDG;
    }
  }
}

