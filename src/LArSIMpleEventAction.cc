#include "LArSIMpleEventAction.hh"

//#include "LArSIMpleHit.hh"
#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMpleMessenger.hh"
#include "LArSIMpleHitFeatureUtils.hh"

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
LArSIMpleEventAction::LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction* LArSIMplePGA) 
  : fGenAction(LArSIMplePGA) {
  
  fOutputFileBase="hits_3d";
  fHitThreshold = 0.;
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
void LArSIMpleEventAction::BeginOfEventAction(const G4Event* evt) {

  fEventID = evt->GetEventID();
  if(fEventID%1000==0 || (fEventID%100==0 && fEventID<1000))
    std::cout << "\n---> Beginning of event: " << fEventID << std::endl;

  // get ID for hits collection
//  if (detectorCollID==-1) {
//    G4SDManager *SDman = G4SDManager::GetSDMpointer();
//    detectorCollID = SDman->GetCollectionID("detectorCollection");
//  }
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void LArSIMpleEventAction::EndOfEventAction(const G4Event* evt) {

  fEventID = evt->GetEventID();
  if(fEventID%1000==0 || (fEventID%100==0 && fEventID<1000))
    std::cout << "\n---> Ending of event: " << fEventID << std::endl;

  std::cout << "Got " << fEnergyDeposits.size() << " 3D energy deposits" << std::endl;

  LArSIMpleHitFeatureUtils hitUtils(fEnergyDeposits);

  // Get neighbours and charge for different radii
  std::vector<double> radii = {3, 10, 30}; // Measured in cm
  std::map<unsigned int,std::vector<unsigned int>> neighbours = hitUtils.GetNumberOfNeighboursWithinRadii(radii);
  std::map<unsigned int,std::vector<double>> charges = hitUtils.GetChargeWithinRadii(radii);

  std::vector<float> flatVector;
  for(unsigned int hitIdx = 0; hitIdx < fEnergyDeposits.size(); ++hitIdx)
  {
    fEnergyDeposits.at(hitIdx).AddFeature(hitUtils.GetAngleToNeighbours(hitIdx)); 
    fEnergyDeposits.at(hitIdx).AddFeature(hitUtils.GetDotProductToNeighbours(hitIdx));
    for(unsigned int radius = 0; radius < radii.size(); ++radius)
      fEnergyDeposits.at(hitIdx).AddFeature(neighbours.at(hitIdx).at(radius));
    for(unsigned int radius = 0; radius < radii.size(); ++radius)
      fEnergyDeposits.at(hitIdx).AddFeature(charges.at(hitIdx).at(radius));
    std::vector flatHit = fEnergyDeposits.at(hitIdx).GetFlatRepresentation();
    flatVector.insert(flatVector.end(),flatHit.begin(),flatHit.end());
  }

  std::cout << "Final flat vector has " << flatVector.size() << " elements" << std::endl;

  if(fWriteZipAndInfoFiles)
    this->WriteOutputZipAndInfoFiles(flatVector);

  if(fWriteRootFile)
    this->WriteRootFile();

  fTrackIDToTrackData.clear();
  fEnergyDeposits.clear();
//
//  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
//
//  G4int n_trajectories = 0;
//  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
//  std::cout << "There were " << n_trajectories << " trajectories" << std::endl;
//
//
//
//  // check that hits collections has been defined and address it
//  if(detectorCollID<0) return;
//  LArSIMpleHitsCollection* LArSIMpleHC = NULL;
//  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
//  if(HCE)
//    LArSIMpleHC = (LArSIMpleHitsCollection*)(HCE->GetHC(detectorCollID));
//
//  G4int    LArSIMple_hits = 0;
//  G4double LArSIMple_edep = 0.;
//
//  // total energy
//  
//  if(LArSIMpleHC) {
//    LArSIMple_hits = LArSIMpleHC->entries();
//    for (G4int i=0; i<LArSIMple_hits; i++)
//      LArSIMple_edep  += (*LArSIMpleHC)[i]->GetEdep();
//    
//    if(event_id%1000==0 || (event_id%100==0 && event_id<1000)) {
//      G4cout << " Size of hit collection: " << LArSIMple_hits << G4endl;
//      G4cout << "     Edep LArSIMple: " << LArSIMple_edep/CLHEP::keV << " keV" << G4endl;    
//    }
//  }
//  
//  
//  //G4double stoppingDistance = ->GetPosition().z();
//
//  // write to file
//  //  if(LArSIMpleHC && LArSIMple_hits) {
//  //G4String filename="hits.out";
//  std::ofstream hitsfile(filename, std::ios::app);
//  if(hitsfile.is_open()) {
//    hitsfile << std::setiosflags(std::ios::fixed)
//	     << std::setprecision(4)
//	     << std::setiosflags(std::ios::left)
//	     << std::setw(6)
//	     << event_id              << "\t"
//	     << primEnergy            << "\t"
//	     << LArSIMple_edep/CLHEP::keV    << "\t"
//	     << LArSIMple_hits               << "\t"
//	     << visibleEnergy         << "\t"
//	     << smearedVisibleE       << "\t";
//    
//    std::cout << "Total energy deposited: " << fEDeposit << " MeV" << std::endl;
//    std::cout << "Total visible energy: " << visibleEnergy << " MeV" << std::endl;
//    std::cout << "Total smeared visible energy: " << smearedVisibleE << " MeV" << std::endl;
//    
//    //hitsfile.close();
//    //}
//    
//  }
//  
//
//  // draw tracks
//  // draw tracks
//  G4VVisManager* pVisManager = G4VVisManager::GetConcreteInstance();
//  if (pVisManager) {
//    G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
//    G4int n_trajectories = 0;
//    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
//    for (G4int i=0; i<n_trajectories; i++) { 
//      G4VTrajectory* trj = ((*(evt->GetTrajectoryContainer()))[i]);
//      if (trj->GetParticleName() == "proton") {
//      G4double stoppingDistance;
//      stoppingDistance = trj->GetPoint(trj->GetPointEntries()-1)->GetPosition().z();
//      std::cout << "Z position: " << stoppingDistance << " mm" << std::endl;
//      //pVisManager->Draw(*trj,1000); // geant v9
//      pVisManager->Draw(*trj); // geant v10
//
//      //Write stopping distance to file
//      hitsfile << stoppingDistance << G4endl;
//      hitsfile.close();
//      }
//    }
//  }
}

const LArSIMpleTrackData LArSIMpleEventAction::GetTrackDataFromTrackID(const int trackID) const{
  if(fTrackIDToTrackData.count(trackID) != 0)
  {
    return fTrackIDToTrackData.at(trackID);
  }
  else
  {
    std::cerr << "- Track " << trackID << " not found in the map... returning empty track data object" << std::endl;
    return LArSIMpleTrackData();
  }
}

void LArSIMpleEventAction::AddTrack(const G4Track *track){
  const int trackID = track->GetTrackID();
  if(fTrackIDToTrackData.count(trackID) == 0)
  {
    fTrackIDToTrackData[trackID] = LArSIMpleTrackData(track);
  }
  else
  {
    std::cerr << "- Track " << trackID << " already exists in the map... doing nothing" << std::endl;
  }
}

void LArSIMpleEventAction::WriteOutputZipAndInfoFiles(const std::vector<float> &flatVec) const
{
  // Write the output using zlib
  ulong src_len = flatVec.size() * sizeof(float); // pixelArray length
  ulong dest_len = compressBound(src_len);     // calculate size of the compressed data               
  char* outputStream = (char *) malloc(dest_len);  // allocate memory for the compressed data

  int res = compress((Bytef *) outputStream, &dest_len, (Bytef *) &flatVec[0], src_len);

  // Buffer error

  if (res == Z_BUF_ERROR)
    std::cout << "Buffer too small!" << std::endl;
  // Memory error
  else if (res ==  Z_MEM_ERROR)
    std::cout << "Not enough memory for compression!" << std::endl;
  // Compression ok 
  else
  {
    std::cout << "Hooray... writing files" << std::endl;

    // Create output files 
    std::stringstream image_file_name;
    image_file_name << fOutputFileBase << "_test_event_" << fEventID << ".gz";

    std::ofstream image_file (image_file_name.str(), std::ofstream::binary);
    if(image_file.is_open())
    {
      // Write the graph to the file and close it
      image_file.write(outputStream, dest_len);
      image_file.close();
    }
  }

  // Write info file
  std::stringstream infoFileName;
  infoFileName << fOutputFileBase << "_test_event_" << fEventID << ".info";
  std::ofstream infoFile(infoFileName.str());
  if(infoFile.is_open())
  {
    // Number of nodes
    infoFile << fEnergyDeposits.size() << std::endl;
    // Number of positions
    infoFile << fEnergyDeposits.at(0).GetNCoordinates() << std::endl;
    // Number of features
    infoFile << fEnergyDeposits.at(0).GetNFeatures() << std::endl;
    // Truth information
    infoFile << fEnergyDeposits.at(0).GetNTruths() << std::endl;

    infoFile.close();
  }

}

// This is very specific and hardcoded
void LArSIMpleEventAction::WriteRootFile() const
{
  std::stringstream rootFileName;
  rootFileName << fOutputFileBase << "_test_event_" << fEventID << ".root";
  TTree *outputTree = new TTree("hits","");
  outputTree->SetDirectory(0);

  float posX, posY, posZ;
  float charge;
  float angle, dotProduct;
  float neighboursR1, neighboursR2, neighboursR3;
  float chargeR1, chargeR2, chargeR3;
  int pdg, trackid;
  outputTree->Branch("x",&posX,"x/F");
  outputTree->Branch("y",&posY,"y/F");
  outputTree->Branch("z",&posZ,"z/F");
  outputTree->Branch("charge",&charge,"charge/F");
  outputTree->Branch("angle",&angle,"angle/F");
  outputTree->Branch("dotProduct",&dotProduct,"dotProduct/F");
  outputTree->Branch("neighboursR1",&neighboursR1,"neighboursR1/F");
  outputTree->Branch("neighboursR2",&neighboursR2,"neighboursR2/F");
  outputTree->Branch("neighboursR3",&neighboursR3,"neighboursR3/F");
  outputTree->Branch("chargeR1",&chargeR1,"chargeR1/F");
  outputTree->Branch("chargeR2",&chargeR2,"chargeR2/F");
  outputTree->Branch("chargeR3",&chargeR3,"chargeR3/F");
  outputTree->Branch("pdg",&pdg,"pdg/I");
  outputTree->Branch("trackid",&trackid,"trackid/I");

  for(unsigned int h = 0; h < fEnergyDeposits.size(); ++h)
  {
    const LArSIMple3DEnergyDeposit &hit = fEnergyDeposits.at(h);
    posX = hit.GetX();
    posY = hit.GetY();
    posZ = hit.GetZ();
    const std::vector<float> features = hit.GetFeatures();
    charge = features.at(0);
    angle = features.at(1);
    dotProduct = features.at(2);
    neighboursR1 = features.at(3);
    neighboursR2 = features.at(4);
    neighboursR3 = features.at(5);
    chargeR1 = features.at(6);
    chargeR2 = features.at(7);
    chargeR3 = features.at(8);
    pdg = hit.GetParticlePDG();
    trackid = hit.GetParticleTrackID();
    outputTree->Fill();
  }

  TFile *outputFile = new TFile(rootFileName.str().c_str(),"recreate");
  outputFile->cd();
  outputTree->Write();
  outputFile->Close();

  outputTree->ResetBranchAddresses();
  delete outputTree;
  delete outputFile;
}

