#include "LArSIMpleEventAction.hh"
#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMpleMessenger.hh"
#include "LArSIMpleHitFeatureUtils.hh"
#include "LArSIMpleOutputWriter.hh"
#include "LArSIMpleTrueNeutrinoEvent.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4Track.hh"

#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4TrajectoryContainer.hh" 

#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include <fstream>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"

#include "zlib.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
LArSIMpleEventAction::LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction* genAction) 
  : fGenAction(genAction) { 
  fOutputFileDirectory="";
  fOutputFilePrefix="hits_3d";
  fHitThreshold = 0.;
  fWireAngleU=35.9;
  fWireAngleV=-35.9;
  fWireAngleW=0.0;
  fMessenger = new LArSIMpleMessenger(this);
  fWriteZipAndInfoFiles = true;
  fWriteRootFile = false;
  fFoldBackTruthInfo = true;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
LArSIMpleEventAction::~LArSIMpleEventAction() 
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void LArSIMpleEventAction::BeginOfEventAction(const G4Event* evt)
{
  fEventID = evt->GetEventID();
  if(fEventID%1000==0 || (fEventID%100==0 && fEventID<1000))
    std::cout << "\n---> Beginning of event: " << fEventID << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void LArSIMpleEventAction::EndOfEventAction(const G4Event*)
{
  if(fEventID%1000==0 || (fEventID%100==0 && fEventID<1000))
    std::cout << "\n---> Ending of event: " << fEventID << std::endl;

  std::cout << "Got " << fEnergyDeposits.size() << " 3D energy deposits" << std::endl;

  LArSIMpleHitFeatureUtils hitUtils(fEnergyDeposits);

  // Get neighbours and charge for different radii
  std::vector<double> radii = {3, 10, 30}; // Measured in cm
  std::map<unsigned int,std::vector<unsigned int>> neighbours = hitUtils.GetNumberOfNeighboursWithinRadii(radii);
  std::map<unsigned int,std::vector<double>> charges = hitUtils.GetChargeWithinRadii(radii);

  for(unsigned int hitIdx = 0; hitIdx < fEnergyDeposits.size(); ++hitIdx)
  {
    LArSIMple3DEnergyDeposit &eDep = fEnergyDeposits.at(hitIdx);
    // Get the UVW projections
    eDep.SetUVW(hitUtils.GetUVW(eDep.GetY(),eDep.GetZ(),fWireAngleU,fWireAngleV,fWireAngleW));
    eDep.AddFeature(hitUtils.GetAngleToNeighbours(hitIdx)); 
    eDep.AddFeature(hitUtils.GetDotProductToNeighbours(hitIdx));
    for(unsigned int radius = 0; radius < radii.size(); ++radius)
      eDep.AddFeature(neighbours.at(hitIdx).at(radius));
    for(unsigned int radius = 0; radius < radii.size(); ++radius)
      eDep.AddFeature(charges.at(hitIdx).at(radius));
  }

  LArSIMpleOutputWriter writer(fEventID);

  if(fWriteZipAndInfoFiles)
    writer.WriteOutputZipAndInfoFiles(fOutputFileDirectory+fOutputFilePrefix,fEnergyDeposits);

  if(fWriteRootFile)
    writer.WriteRootFile(fOutputFileDirectory+fOutputFilePrefix,fEnergyDeposits,fGenAction->GetTrueNeutrinoEventPointer());

  fTrackIDToTrackData.clear();
  fEnergyDeposits.clear();
}

LArSIMpleTrackData LArSIMpleEventAction::GetTrackDataFromTrackID(const int trackID) const{
  if(fTrackIDToTrackData.count(trackID) != 0)
  {
    return fTrackIDToTrackData.at(trackID);
  }
  else
  {
    std::cout << "- Track " << trackID << " not found in the map... returning empty track data object" << std::endl;
    return LArSIMpleTrackData();
  }
}

void LArSIMpleEventAction::AddTrack(const G4Track *track){
  if(fTrackIDToTrackData.count(track->GetTrackID()) == 0)
    fTrackIDToTrackData.insert(std::make_pair(track->GetTrackID(),LArSIMpleTrackData(track)));
}

