#include "LArSIMplePhysicsList.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"

#include "G4ios.hh"
#include "globals.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4RegionStore.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4EmSaturation.hh"
#include "G4LossTableManager.hh"

#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh"

#include "G4eMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

#include "G4OpBoundaryProcess.hh"

#include "G4PhysicsListHelper.hh"


// Constructor /////////////////////////////////////////////////////////////
LArSIMplePhysicsList::LArSIMplePhysicsList() : QGSP_BIC_HP() {

  this->defaultCutValue = 0.05*CLHEP::mm; //standard = 0.5 mm
  //this->defaultCutValue     = 0.001*CLHEP::mm; //for mylar
  //defaultCutValue     = 1*CLHEP::mm;
  cutForGamma         = defaultCutValue;
  cutForElectron      = defaultCutValue;
  cutForPositron      = defaultCutValue;
  cutForProton        = defaultCutValue; 

  //ConstructEM();
  
  this->RemovePhysics(new G4EmStandardPhysics());
  this->RemovePhysics(new G4EmExtraPhysics());

  this->RegisterPhysics( new G4EmLivermorePhysics() );

}


// Destructor //////////////////////////////////////////////////////////////
LArSIMplePhysicsList::~LArSIMplePhysicsList() 
{}

/*void LArSIMplePhysicsList::ConstructEM() {

  G4PhysicsListHelper *ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // processes
  G4PhotoElectricEffect* lowePhot = new G4PhotoElectricEffect();
  lowePhot->SetEmModel(new G4LivermorePhotoElectricModel());

  G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
  theComptonScattering->SetEmModel(new G4LivermoreComptonModel());

  G4GammaConversion* theGammaConversion = new G4GammaConversion();
  theGammaConversion->SetEmModel(new G4LivermoreGammaConversionModel());
  
  G4RayleighScattering* theRayleigh = new G4RayleighScattering();
  theRayleigh->SetEmModel(new G4LivermoreRayleighModel());
    
  G4eIonisation* loweIon = new G4eIonisation();
  loweIon->SetEmModel(new G4LivermoreIonisationModel());
  loweIon->SetFluctModel(new G4UniversalFluctuation());

  G4eBremsstrahlung* loweBrem = new G4eBremsstrahlung();
  loweBrem->SetEmModel(new G4LivermoreBremsstrahlungModel());					  
  
  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName      = particle->GetParticleName();
    G4String particleType      = particle->GetParticleType();
    G4double particleCharge    = particle->GetPDGCharge();
    
    if (particleName == "e-") {
      // electron

      //std::cout << "Particle: " << particleName << ", " << particleType << std::endl;

      G4eMultipleScattering* aMultipleScattering = new G4eMultipleScattering("MultipleScattering");
      ph->RegisterProcess(aMultipleScattering, particle);
      ph->RegisterProcess(loweIon, particle);
      ph->RegisterProcess(loweBrem, particle);
    }
  }
}
*/
/////////////////////////////////////////////////////////////////////////////
// Cuts /////////////////////////////////////////////////////////////////////
void LArSIMplePhysicsList::SetCuts() {

  QGSP_BIC_HP::SetCuts();

  /*
  //To specify the production threshold in energy or go below 990eV.  
  G4double lowlimit=1*CLHEP::keV;
  //G4double lowlimit=0.1*CLHEP::keV;
  G4ProductionCutsTable::GetProductionCutsTable()
    ->SetEnergyRange(lowlimit, 100.*CLHEP::GeV);
  */

  /*
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  SetCutValue(cutForProton, "proton");
  */

  SetCutsWithDefault();
  
  //Set different cuts for different volumes/regions

  //G4double MylarCutValue = 0.001*CLHEP::mm;
  /*
  G4Region* region;
  G4String regName;
  G4ProductionCuts* cuts;
  
  regName = "mylar";
  region = G4RegionStore::GetInstance()->GetRegion(regName);
  cuts = new G4ProductionCuts;
  cuts->SetProductionCut(0.01*CLHEP::mm);
  region->SetProductionCuts(cuts);
  */
}


/////////////////////////////////////////////////////////////////////////////
