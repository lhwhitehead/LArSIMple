/**
 *  @file LArSIMple/src/LArSIMpleSteppingAction.cc
 * 
 *  @brief Implementation of the stepping action class.
 * 
 *  $Log: $
 */

#include "LArSIMpleSteppingAction.hh"
#include "G4SteppingManager.hh"
#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleEventAction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"

LArSIMpleSteppingAction::LArSIMpleSteppingAction(LArSIMpleEventAction *pEventAction) :
    fEventAction(pEventAction)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleSteppingAction::~LArSIMpleSteppingAction()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

G4VProcess *LArSIMpleSteppingAction::GetCurrentProcess()
{
    return fpSteppingManager->GetfCurrentProcess();
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleSteppingAction::UserSteppingAction(const G4Step *aStep)
{
    if (G4StrUtil::contains(aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName(), "LArBox"))
    {
        LArSIMple3DEnergyDeposit energyDeposit;

        if ((aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit()) < fEventAction->GetHitThreshold())
            return;

        const G4StepPoint *stepPoint = aStep->GetPreStepPoint();
        energyDeposit.SetPositionAndTime((stepPoint->GetPosition()), stepPoint->GetGlobalTime());

        const G4Track *track = aStep->GetTrack();
        int foldedTrackID{track->GetTrackID()};
        int foldedTrackPDG{track->GetParticleDefinition()->GetPDGEncoding()};
        int foldedTrackProcess{static_cast<int>(fEventAction->GetTrackDataFromTrackID(track->GetTrackID()).GetProcessCode())};

        if (fEventAction->FoldBackTruthInfo())
        {
            LArSIMpleTruthFolder &truthFolder{fEventAction->GetTruthFolder()};
            const std::map<int, LArSIMpleTrackData> &trackIDToTrackDataMap{fEventAction->GetTrackIDToTrackDataMap()};
            truthFolder.PerformTrackFolding(track, trackIDToTrackDataMap, foldedTrackID, foldedTrackPDG, foldedTrackProcess);
        }
        energyDeposit.SetParticleInfo(foldedTrackPDG, foldedTrackID, foldedTrackProcess);

        const double energy{aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit()};
        energyDeposit.SetEnergy(energy);
        energyDeposit.SetDeDx(energy / (aStep->GetDeltaPosition().mag()));
        fEventAction->Add3DEnergyDeposit(energyDeposit);
    }
}
