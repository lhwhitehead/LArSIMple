/**
 *  @file LArSIMple/src/LArSIMpleEventAction.cc
 * 
 *  @brief Implementation of the event action class.
 * 
 *  $Log: $
 */

#include "LArSIMpleEventAction.hh"
#include "LArSIMpleHitFeatureUtils.hh"
#include "LArSIMpleMessenger.hh"
#include "LArSIMpleOutputWriter.hh"
#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMpleTrueNeutrinoEvent.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4Track.hh"

LArSIMpleEventAction::LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction *genAction) :
    fGenAction(genAction),
    fOutputFileDirectory(""),
    fOutputFilePrefix("hits_3d"),
    fHitThreshold(0.0),
    fWireAngleU(35.9),
    fWireAngleV(-35.9),
    fWireAngleW(0.0),
    fWriteZipAndInfoFiles(true),
    fWriteRootFile(false),
    fFoldBackTruthInfo(true)
{
    fMessenger = new LArSIMpleMessenger(this);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleEventAction::~LArSIMpleEventAction()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleEventAction::BeginOfEventAction(const G4Event *evt)
{
    fEventID = evt->GetEventID();
    if (fEventID % 1000 == 0 || (fEventID % 100 == 0 && fEventID < 1000))
        std::cout << "\n---> Beginning of event: " << fEventID << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleEventAction::EndOfEventAction(const G4Event *)
{
    if (fEventID % 1000 == 0 || (fEventID % 100 == 0 && fEventID < 1000))
        std::cout << "\n---> Ending of event: " << fEventID << std::endl;

    std::cout << "Got " << fEnergyDeposits.size() << " 3D energy deposits" << std::endl;

    if (fEnergyDeposits.size() < 3)
    {
        std::cerr << "Need at least three 3D energy deposits - moving to next event" << std::endl;
        this->CleanUp();
        return;
    }

    for (unsigned int hitIdx = 0; hitIdx < fEnergyDeposits.size(); ++hitIdx)
    {
        LArSIMple3DEnergyDeposit &eDep = fEnergyDeposits.at(hitIdx);
        const G4ThreeVector &pos = eDep.GetPosition();
        eDep.CalculateUVW(fWireAngleU, fWireAngleV, fWireAngleW);
    }

    if (fUseHitFeatures)
    {
        LArSIMpleHitFeatureUtils hitUtils(fEnergyDeposits);
        // Get neighbours and charge for different radii
        std::vector<double> radii = {3, 10, 30}; // Measured in cm
        std::map<unsigned int, std::vector<unsigned int>> neighbours = hitUtils.GetNumberOfNeighboursWithinRadii(radii);
        std::map<unsigned int, std::vector<double>> charges = hitUtils.GetChargeWithinRadii(radii);
        for (unsigned int hitIdx = 0; hitIdx < fEnergyDeposits.size(); ++hitIdx)
        {
            LArSIMple3DEnergyDeposit &eDep = fEnergyDeposits.at(hitIdx);
            eDep.AddFeature(hitUtils.GetAngleToNeighbours(hitIdx));
            eDep.AddFeature(hitUtils.GetDotProductToNeighbours(hitIdx));
            for (unsigned int radius = 0; radius < radii.size(); ++radius)
                eDep.AddFeature(neighbours.at(hitIdx).at(radius));
            for (unsigned int radius = 0; radius < radii.size(); ++radius)
                eDep.AddFeature(charges.at(hitIdx).at(radius));
        }
    }

    this->UpdatePiZeroDecayPhotonTruth();

    LArSIMpleOutputWriter writer(fEventID);

    if (fWriteZipAndInfoFiles)
        writer.WriteOutputZipAndInfoFiles(fOutputFileDirectory + fOutputFilePrefix, fEnergyDeposits);

    if (fWriteRootFile)
    {
        const std::vector<double> wireAngles{fWireAngleU, fWireAngleV, fWireAngleW};
        writer.WriteRootFile(fOutputFileDirectory + fOutputFilePrefix, fEnergyDeposits, fGenAction->GetTrueNeutrinoEventPointer(),
            fTrackIDToTrackData, wireAngles);
    }
    this->CleanUp();
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleTrackData LArSIMpleEventAction::GetTrackDataFromTrackID(const int trackID) const
{
    if (fTrackIDToTrackData.count(trackID) != 0)
    {
        return fTrackIDToTrackData.at(trackID);
    }
    else
    {
        std::cout << "- Track " << trackID << " not found in the map... returning empty track data object" << std::endl;
        return LArSIMpleTrackData();
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleEventAction::AddTrack(const G4Track *track)
{
    if (fTrackIDToTrackData.count(track->GetTrackID()) == 0)
        fTrackIDToTrackData.insert(std::make_pair(track->GetTrackID(), LArSIMpleTrackData(track)));
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleEventAction::UpdateTrackEndInfo(const G4Track *track)
{
    if (fTrackIDToTrackData.count(track->GetTrackID()))
    {
        fTrackIDToTrackData[track->GetTrackID()].SetEndPointInfo(track);
    }
    else
    {
        std::cout << "LArSIMpleEventAction::UpdateTrackEndInfo: track does not exist" << std::endl;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleEventAction::UpdatePiZeroDecayPhotonTruth()
{
    std::vector<int> primaryPiZeroTrackIDs;

    for (const auto &pair : fTrackIDToTrackData)
    {
        if (pair.second.GetPDG() == 111 && pair.second.IsPrimary())
            primaryPiZeroTrackIDs.emplace_back(pair.second.GetTrackID());
    }

    if (primaryPiZeroTrackIDs.empty())
        return;

    // If we have pi zeros then look for their decay photons
    for (auto &pair : fTrackIDToTrackData)
    {
        if (pair.second.GetPDG() == 22)
        {
            if (std::find(primaryPiZeroTrackIDs.begin(), primaryPiZeroTrackIDs.end(), pair.second.GetParentID()) != primaryPiZeroTrackIDs.end())
                pair.second.SetPrimary();
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleEventAction::CleanUp()
{
    fEnergyDeposits.clear();
    fTrackIDToTrackData.clear();
    fTruthFolder.Reset();
}
