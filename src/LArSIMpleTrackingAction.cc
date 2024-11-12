/**
 *  @file LArSIMple/src/LArSIMpleTrackingAction.cc
 * 
 *  @brief Implementation of the tracking action class.
 * 
 *  $Log: $
 */

#include "LArSIMpleTrackingAction.hh"
#include "LArSIMpleEventAction.hh"

#include "G4Track.hh"

LArSIMpleTrackingAction::LArSIMpleTrackingAction(LArSIMpleEventAction *pEventAction) :
    fEventAction(pEventAction)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleTrackingAction::~LArSIMpleTrackingAction()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleTrackingAction::PreUserTrackingAction(const G4Track *aTrack)
{
    fEventAction->AddTrack(aTrack);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleTrackingAction::PostUserTrackingAction(const G4Track *aTrack)
{
    fEventAction->UpdateTrackEndInfo(aTrack);
}
