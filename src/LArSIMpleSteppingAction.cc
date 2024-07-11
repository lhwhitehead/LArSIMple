#include "LArSIMpleSteppingAction.hh"
#include "G4SteppingManager.hh"
#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleEventAction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"

LArSIMpleSteppingAction::LArSIMpleSteppingAction(LArSIMpleEventAction *evt) :
    fEventAction(evt)
{
}

LArSIMpleSteppingAction::~LArSIMpleSteppingAction()
{
}

G4VProcess *LArSIMpleSteppingAction::GetCurrentProcess()
{
    return fpSteppingManager->GetfCurrentProcess();
}

void LArSIMpleSteppingAction::UserSteppingAction(const G4Step *aStep)
{
    if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName().contains("LArBox"))
    {
        LArSIMple3DEnergyDeposit energyDeposit;

        if ((aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit()) < fEventAction->GetHitThreshold())
            return;

        const G4StepPoint *stepPoint = aStep->GetPreStepPoint();
        energyDeposit.SetPositionAndTime((stepPoint->GetPosition() / CLHEP::cm), stepPoint->GetGlobalTime());

        const G4Track *track = aStep->GetTrack();
        int foldedTrackID{track->GetTrackID()};
        int foldedTrackPDG{track->GetParticleDefinition()->GetPDGEncoding()};
        int foldedTrackProcess{static_cast<int>(fEventAction->GetTrackDataFromTrackID(track->GetTrackID()).GetProcessCode())};

        if (fEventAction->FoldBackTruthInfo())
            this->GetFoldedTrackInfo(track, foldedTrackID, foldedTrackPDG, foldedTrackProcess);
        energyDeposit.SetParticleInfo(foldedTrackPDG, foldedTrackID, foldedTrackProcess);

        const double energy{aStep->GetTotalEnergyDeposit() - aStep->GetNonIonizingEnergyDeposit()};
        energyDeposit.SetEnergy(energy);
        energyDeposit.SetDeDx(energy / (aStep->GetDeltaPosition().mag() / CLHEP::cm));
        fEventAction->Add3DEnergyDeposit(energyDeposit);
    }
}

void LArSIMpleSteppingAction::GetFoldedTrackInfo(const G4Track *track, int &foldedTrackID, int &foldedTrackPDG, int &foldedTrackProcess)
{
    // Have we already folded this track before?
    const int foldedAssoc{fEventAction->GetFoldedTrackAssoc(track->GetTrackID())};
    if (foldedAssoc != -1)
    {
        const LArSIMpleTrackData &trkData = fEventAction->GetTrackDataFromTrackID(foldedAssoc);
        foldedTrackID = trkData.GetTrackID();
        foldedTrackPDG = trkData.GetPDG();
        foldedTrackProcess = static_cast<int>(trkData.GetProcessCode());
        return;
    }

    // We need to iterate through the parent links until we find a track that isn't foldable
    bool keepFolding{fEventAction->GetTrackDataFromTrackID(track->GetTrackID()).IsFoldable()};
    while (keepFolding)
    {
        const LArSIMpleTrackData trkData = fEventAction->GetTrackDataFromTrackID(foldedTrackID);
        if (trkData.IsFoldable())
        {
            foldedTrackID = trkData.GetParentID();
        }
        else
        {
            foldedTrackID = trkData.GetTrackID();
            foldedTrackPDG = trkData.GetPDG();
            foldedTrackProcess = static_cast<int>(fEventAction->GetTrackDataFromTrackID(foldedTrackID).GetProcessCode());
            fEventAction->AddFoldedTrackAssoc(track->GetTrackID(), foldedTrackID);
            keepFolding = false;
        }
    }
}
