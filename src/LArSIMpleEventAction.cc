#include "LArSIMpleEventAction.hh"

//#include "LArSIMpleHit.hh"
#include "LArSIMplePrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"

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

#include "zlib.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
LArSIMpleEventAction::LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction* LArSIMplePGA) 
  : fGenAction(LArSIMplePGA) {
  
  fOutputFileBase="hits_3d";

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
LArSIMpleEventAction::~LArSIMpleEventAction() 
{;}


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

  std::cout << "Got " << fEnergyDeposits.size() << std::endl;
  for(auto const &edep : fEnergyDeposits)
    edep.PrintSummary();

  this->WriteOutputFiles();

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

void LArSIMpleEventAction::WriteOutputFiles() const
{
  // Write the output using zlib
  std::vector<double> pixel_array{0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8};
  ulong src_len = pixel_array.size(); // pixelArray length
  ulong dest_len = compressBound(src_len);     // calculate size of the compressed data               
  char* ostream = (char *) malloc(dest_len);  // allocate memory for the compressed data

  int res = compress((Bytef *) ostream, &dest_len, (Bytef *) &pixel_array[0], src_len);

  // Buffer error

  if (res == Z_BUF_ERROR)
    std::cout << "Buffer too small!" << std::endl;
  // Memory error
  else if (res ==  Z_MEM_ERROR)
    std::cout << "Not enough memory for compression!" << std::endl;
  // Compression ok 
  else { 
    std::cout << "Hooray... writing files" << std::endl;
  }
}

