/**
 *  @file LArSIMple/src/LArSIMpleEventAction.cc
 * 
 *  @brief Implementation of the event action class.
 * 
 *  $Log: $
 */

#include "LArSIMpleEventAction.hh"
#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleHitFeatureUtils.hh"
#include "LArSIMpleMessenger.hh"
#include "LArSIMpleOutputWriter.hh"
#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMpleTrueNeutrinoEvent.hh"
#include "LArSIMpleWireConvertor.hh"
#include "LArSIMpleWireHit.hh"

#ifdef USE_PANDORA
#include "LArSIMplePandoraWriter.hh"
#endif

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4Track.hh"

LArSIMpleEventAction::LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction *genAction, LArSIMpleDetectorConstruction *detector) :
    fGenAction(genAction),
    fDetector(detector),
    fOutputFileDirectory(""),
    fOutputFilePrefix("hits_3d"),
    fHitThreshold(0.0),
    fWriteZipAndInfoFiles(true),
    fWriteRootFile(false),
    fFoldBackTruthInfo(true)
{
    fMessenger = new LArSIMpleMessenger(this);
#ifdef USE_PANDORA
    fPandoraWriter = std::make_unique<LArSIMplePandoraWriter>(detector);
#endif
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
        eDep.CalculateUVW(fDetector->GetWireAngle(LArSIMpleReadoutView::ViewU), fDetector->GetWireAngle(LArSIMpleReadoutView::ViewV), fDetector->GetWireAngle(LArSIMpleReadoutView::ViewW));
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

    LArSIMpleWireConvertor &wireConvertor = LArSIMpleWireConvertor::Get(fDetector);
    const std::vector<LArSIMpleWireHit> uHits = wireConvertor.Convert3DEnergyDepositsToWireHits(fEnergyDeposits, LArSIMpleReadoutView::ViewU);
    const std::vector<LArSIMpleWireHit> vHits = wireConvertor.Convert3DEnergyDepositsToWireHits(fEnergyDeposits, LArSIMpleReadoutView::ViewV);
    const std::vector<LArSIMpleWireHit> wHits = wireConvertor.Convert3DEnergyDepositsToWireHits(fEnergyDeposits, LArSIMpleReadoutView::ViewW);

    if (fWriteRootFile)
    {
        const std::vector<double> wireAngles{fDetector->GetWireAngle(LArSIMpleReadoutView::ViewU), fDetector->GetWireAngle(LArSIMpleReadoutView::ViewV), fDetector->GetWireAngle(LArSIMpleReadoutView::ViewW)};
        writer.WriteRootFile(fOutputFileDirectory + fOutputFilePrefix, fEnergyDeposits, uHits, vHits, wHits,
            fGenAction->GetTrueNeutrinoEventPointer(), fTrackIDToTrackData, wireAngles);
    }

#ifdef USE_PANDORA
//    LArSIMplePandoraWriter pandoraWriter(fDetector);
    std::cout << "Creating LArTPC" << std::endl;
    fPandoraWriter->CreateLArTPC();

    // Convert TrackData map into a vector
    std::cout << "Creating MC particles" << std::endl;
    std::vector<LArSIMpleTrackData> mcParticles;
    for (auto [_, mcParticle] : fTrackIDToTrackData)
        mcParticles.emplace_back(mcParticle);
    fPandoraWriter->CreateMCParticles(mcParticles);

    // Add all wire hits into a single vector
    std::vector<LArSIMpleWireHit> allWireHits;
    allWireHits.insert(allWireHits.end(), uHits.begin(), uHits.end());
    allWireHits.insert(allWireHits.end(), vHits.begin(), vHits.end());
    allWireHits.insert(allWireHits.end(), wHits.begin(), wHits.end());
    std::cout << "Creating calo hits" << std::endl;
    fPandoraWriter->CreateCaloHits(allWireHits);

    std::cout << "Running Pandora" << std::endl;
    fPandoraWriter->RunPandora();
#endif


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
