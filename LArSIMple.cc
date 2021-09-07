#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "Randomize.hh"

#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMplePhysicsList.hh"
#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMpleRunAction.hh"
#include "LArSIMpleEventAction.hh"
#include "LArSIMpleTrackingAction.hh"
#include "LArSIMpleSteppingAction.hh"
#include "LArSIMpleMessenger.hh"
#include "LArSIMpleDetectorMessenger.hh"
#include "G4PhysListFactory.hh"

int main(int argc,char** argv) {

  if (argc != 3)
  {
    std::cerr << "LArSIMple error: insufficient arguments" << std::endl;
    std::cerr << " -- usage: LArSIMple /path/to/main/mac_file /path/to/settings/mac_file" << std::endl;
    return 1;
  }

  // choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;

  // Need the detector geometry before everything else
  LArSIMpleDetectorConstruction* detector = new LArSIMpleDetectorConstruction();
  runManager->SetUserInitialization(detector);
  
  // Physics List
  runManager->SetUserInitialization(new LArSIMplePhysicsList);

  LArSIMpleRunAction* run_action = new LArSIMpleRunAction;  
  runManager->SetUserAction(run_action);

  LArSIMplePrimaryGeneratorAction* pga = new LArSIMplePrimaryGeneratorAction(detector);
  runManager->SetUserAction(pga);

  LArSIMpleEventAction* event_action = new LArSIMpleEventAction(pga);
  runManager->SetUserAction(event_action);

  LArSIMpleTrackingAction* tracking_action = new LArSIMpleTrackingAction(event_action);
  runManager->SetUserAction(tracking_action);
 
  LArSIMpleSteppingAction* stepping_action = new LArSIMpleSteppingAction(event_action);
  runManager->SetUserAction(stepping_action);


  // Read the detector and physics configuration
  G4UImanager* UI = G4UImanager::GetUIpointer();
  G4String command, fileName;

  // Run the generator config first  
  command = "/control/execute ";
  fileName = argv[2];
  UI->ApplyCommand(command+fileName);

  // Now initialise the run manager and run the main config to launch the simulation
  runManager->Initialize();
  fileName = argv[1];
  UI->ApplyCommand(command+fileName);

  delete runManager;

  return 0;
}



