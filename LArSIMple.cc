#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
//#include "G4VSteppingVerbose.hh"

#ifdef G4UI_USE_XM
#include "G4UIXm.hh"
#endif

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMplePhysicsList.hh" // include for Geant Version 9
#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMpleRunAction.hh"
#include "LArSIMpleEventAction.hh"
#include "LArSIMpleTrackingAction.hh"
#include "LArSIMpleSteppingAction.hh"
//#include "LArSIMpleSteppingVerbose.hh"
#include "LArSIMpleMessenger.hh"
#include "LArSIMpleDetectorMessenger.hh"
#include "G4PhysListFactory.hh"

#include "FTFP_BERT.hh"
#include "QGSP_BERT_HP.hh"
#include "G4EmDNAPhysics.hh"
#include "QGSP_BIC_HP.hh"

#include <vector>

int main(int argc,char** argv) {

  // choose the Random engine
  //HepRandom::setTheEngine(new RanecuEngine); //Geant 4.9
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

//  LArSIMpleSteppingVerbose* stepping_verbose = new LArSIMpleSteppingVerbose();
  //G4VSteppingVerbose::SetInstance(new LArSIMpleSteppingVerbose);
//  G4VSteppingVerbose::SetInstance(stepping_verbose);

  // Construct the default run manager
  G4RunManager * runManager = new G4RunManager;


  // set mandatory initialization classes
  LArSIMpleDetectorConstruction* detector = new LArSIMpleDetectorConstruction();
  runManager->SetUserInitialization(detector);
  //LArSIMpleDetectorMessenger* detectorMessenger = new LArSIMpleDetectorMessenger(detector);
  //get the pointer to the User Interface manager 
  
  //Geant Version 9 (user defined physics list)
  runManager->SetUserInitialization(new LArSIMplePhysicsList);

  //Geant Version 10 (predefined physics list, as from example B4a)
  //G4VModularPhysicsList* physicsList = new FTFP_BERT;
  //G4VModularPhysicsList* physicsList = new QGSP_BERT_HP;
  //G4VModularPhysicsList* physicsList = new G4EmDNAPhysics;
  //G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
  //runManager->SetUserInitialization(physicsList);

 G4UIsession* session=nullptr;
  
  if (argc==1)   // Define UI session for interactive mode.
  {
      // G4UIterminal is a (dumb) terminal.
#ifdef G4UI_USE_XM
      session = new G4UIXm(argc,argv);
#else           
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);      
#else
      session = new G4UIterminal();
#endif
#endif
  }
  
#ifdef G4VIS_USE
  // visualization manager
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
    
  // set user action classes
  LArSIMplePrimaryGeneratorAction* pga = new LArSIMplePrimaryGeneratorAction;
  runManager->SetUserAction(pga);
  //runManager->SetUserAction(new LArSIMpleRunAction);
  //runManager->SetUserAction(new LArSIMpleEventAction(pga));

  LArSIMpleRunAction* run_action = new LArSIMpleRunAction;  
  runManager->SetUserAction(run_action);

  LArSIMpleEventAction* event_action = new LArSIMpleEventAction(pga);
  runManager->SetUserAction(event_action);

  LArSIMpleTrackingAction* tracking_action = new LArSIMpleTrackingAction(event_action);
  runManager->SetUserAction(tracking_action);
//  LArSIMpleMessenger* messenger = new LArSIMpleMessenger(event_action);
 
  LArSIMpleSteppingAction* stepping_action = new LArSIMpleSteppingAction(event_action);
  runManager->SetUserAction(stepping_action);

  // Read the detector and physics configuration
  G4UImanager* UI = G4UImanager::GetUIpointer();
  G4String command, fileName;
  if (argc==3) {
    command = "/control/execute ";
    fileName = argv[2];
    UI->ApplyCommand(command+fileName);
  }

  
  //Initialize G4 kernel
  runManager->Initialize();

  // Define UI session for interactive mode.
  if(session) {

      // G4UIterminal is a (dumb) terminal.
      UI->ApplyCommand("/control/execute inter.mac");       
      session->SessionStart();
      delete session;
    }
  // Batch mode
  else
    { 
      command = "/control/execute ";
      fileName = argv[1];
      UI->ApplyCommand(command+fileName);
    }


  // job termination
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}



