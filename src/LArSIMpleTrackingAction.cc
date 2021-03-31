#include "LArSIMpleTrackingAction.hh"
#include "LArSIMpleEventAction.hh"

#include "G4Track.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LArSIMpleTrackingAction::LArSIMpleTrackingAction(LArSIMpleEventAction* evt) : fEventAction(evt)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LArSIMpleTrackingAction::~LArSIMpleTrackingAction()
{

}

void LArSIMpleTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  fEventAction->AddTrack(aTrack);
}

void LArSIMpleTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{

}

