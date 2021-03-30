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

    const G4StepPoint *stepPoint = aStep->GetPreStepPoint();
    energyDeposit.SetPositionAndTime(stepPoint->GetPosition(),stepPoint->GetGlobalTime());

    const G4Track *track = aStep->GetTrack();
    energyDeposit.SetParticleInfo(track->GetParticleDefinition()->GetPDGEncoding(),track->GetTrackID());

    // Allow for Birk's suppression when considering energy
//    G4EmSaturation* birksSup = G4LossTableManager::Instance()->EmSaturation();
//    const double eVis = birksSup->VisibleEnergyDeposition(track->GetParticleDefinition(), track->GetMaterialCutsCouple(), aStep->GetStepLength(), aStep->GetTotalEnergyDeposit(), aStep->GetNonIonizingEnergyDeposit());
//    energyDeposit.SetEnergy(eVis);
    energyDeposit.SetEnergy(aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit());
    fEventAction->Add3DEnergyDeposit(energyDeposit);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


