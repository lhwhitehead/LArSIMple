#include "LArSIMpleTrackingAction.hh"
#include "LArSIMpleEventAction.hh"

#include "G4Track.hh"

LArSIMpleTrackingAction::LArSIMpleTrackingAction(LArSIMpleEventAction* evt) : fEventAction(evt)
{

}

LArSIMpleTrackingAction::~LArSIMpleTrackingAction()
{

}

void LArSIMpleTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  fEventAction->AddTrack(aTrack);
}

void LArSIMpleTrackingAction::PostUserTrackingAction(const G4Track*)
{

}

