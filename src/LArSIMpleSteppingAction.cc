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
  if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName().contains("LArBox"))
  {
    LArSIMple3DEnergyDeposit energyDeposit;

    if((aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit()) < fEventAction->GetHitThreshold())
      return;

    const G4StepPoint *stepPoint = aStep->GetPreStepPoint();
    energyDeposit.SetPositionAndTime((stepPoint->GetPosition() / CLHEP::cm),stepPoint->GetGlobalTime());

    const G4Track *track = aStep->GetTrack();
    int foldedTrackID = track->GetTrackID();
    int foldedTrackPDG = track->GetParticleDefinition()->GetPDGEncoding();
    int foldedTrackProcess = static_cast<int>(fEventAction->GetTrackDataFromTrackID(track->GetTrackID()).GetProcessCode());
    
    if(fEventAction->FoldBackTruthInfo())
    {
      this->GetFoldedTrackInfo(track,foldedTrackID,foldedTrackPDG,foldedTrackProcess);
    }
    energyDeposit.SetParticleInfo(foldedTrackPDG,foldedTrackID,foldedTrackProcess);

    // Allow for Birk's suppression when considering energy
//    G4EmSaturation* birksSup = G4LossTableManager::Instance()->EmSaturation();
//    const double eVis = birksSup->VisibleEnergyDeposition(track->GetParticleDefinition(), track->GetMaterialCutsCouple(), aStep->GetStepLength(), aStep->GetTotalEnergyDeposit(), aStep->GetNonIonizingEnergyDeposit());
//    energyDeposit.SetEnergy(eVis);
    const double energy = aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit();
    energyDeposit.SetEnergy(energy);
    energyDeposit.SetDeDx(energy / (aStep->GetDeltaPosition().mag() / CLHEP::cm));
    fEventAction->Add3DEnergyDeposit(energyDeposit);
  }
}

void LArSIMpleSteppingAction::GetFoldedTrackInfo(const G4Track *track, int &foldedTrackID, int &foldedTrackPDG, int &foldedTrackProcess)
{
  // Values for foldedTrackID and foldedTrackPDG set to those of the G4Track track already

  // We need to iterate through the parent links until we find a track that isn't foldable
  bool keepFolding = fEventAction->GetTrackDataFromTrackID(track->GetTrackID()).IsFoldable();
  while(keepFolding)
  {
    const LArSIMpleTrackData trkData = fEventAction->GetTrackDataFromTrackID(foldedTrackID);
    if(trkData.IsFoldable())
    {
      foldedTrackID = trkData.GetParentID();
      std::cout << "Folding track: " << trkData.GetPDG() << ", " << trkData.GetTrackID() << ", " << track->GetCreatorProcess()->GetProcessName() << " to parent " << trkData.GetParentID() << " with pdg " << fEventAction->GetTrackDataFromTrackID(trkData.GetParentID()).GetPDG() << std::endl;
    }
    else
    {
      std::cout << "Not folding track: " << trkData.GetPDG() << ", " << trkData.GetTrackID() << ", " << track->GetCreatorProcess()->GetProcessName() << " to parent " << trkData.GetParentID() << " with pdg " << fEventAction->GetTrackDataFromTrackID(foldedTrackID).GetPDG() << std::endl;
      foldedTrackID = trkData.GetTrackID();
      foldedTrackPDG = trkData.GetPDG();
      foldedTrackProcess = static_cast<int>(fEventAction->GetTrackDataFromTrackID(foldedTrackID).GetProcessCode());
      keepFolding = false;
    }
  }

}

