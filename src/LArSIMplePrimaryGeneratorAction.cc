#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMplePrimaryGeneratorMessenger.hh"
#include "LArSIMpleRunAction.hh"
#include "LArSIMpleNeutrinoInputParser.hh"
#include "LArSIMpleTrueNeutrinoEvent.hh"
#include "LArSIMpleTrueParticle.hh"

#include "Randomize.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Event.hh"
#include "globals.hh"


LArSIMplePrimaryGeneratorAction::LArSIMplePrimaryGeneratorAction()
{  
  fParticleGun = new G4GeneralParticleSource();
  fMessenger = new LArSIMplePrimaryGeneratorMessenger(this);
}


LArSIMplePrimaryGeneratorAction::~LArSIMplePrimaryGeneratorAction()
{
  delete fParticleGun;
}


void LArSIMplePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  const unsigned int eventNumber = static_cast<unsigned int>(anEvent->GetEventID());

  // Check if we are processing neutrinos
  if (this->UseNeutrinos())
  {
    if (fNeutrinoInputParser.GetNEvents() == 0)
    {
      // Check that the neutrino parser is ready if we request neutrino events
      fNeutrinoInputParser.ReadFile(fNeutrinoFileName, fNeutrinoFileType);
    }

    LArSIMpleTrueNeutrinoEvent neutrinoEvent;
    if (fNeutrinoInputParser.GetNEvents() >= eventNumber)
    {
      neutrinoEvent = fNeutrinoInputParser.GetEvent(eventNumber);
    }
    else
    {
      std::cerr << "Requested event number is greater than the number of neutrinos. Processing the last one again." << std::endl;
      neutrinoEvent = fNeutrinoInputParser.GetEvent(fNeutrinoInputParser.GetNEvents() - 1);  
    }

    const std::vector<LArSIMpleTrueParticle> finalStateParticles = neutrinoEvent.GetFinalStateParticles();
    const G4ThreeVector neutrinoVertex = neutrinoEvent.GetInteractionVertex();
    const double vertexTime = 0.; // Hard code for now

    // Use a particle gun for the neutrinos as it is more convenient
    G4ParticleGun *particleGun = new G4ParticleGun();
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();

    // We need to fire all of the final-state particles
    for (const LArSIMpleTrueParticle &part : finalStateParticles)
    {
      // Get the particle mass and hence kinetic energy
      particleGun->SetParticleDefinition(particleTable->FindParticle(part.GetPDGCode()));
      const G4double mass = particleGun->GetParticleDefinition()->GetPDGMass();
      const G4double ekin = part.GetEnergy() - mass;
    
      particleGun->SetParticleEnergy(ekin);
      particleGun->SetParticlePosition(neutrinoVertex);
      particleGun->SetParticleTime(vertexTime);
      particleGun->SetParticleMomentumDirection(part.GetDirection());
      particleGun->GeneratePrimaryVertex(anEvent);
    }
  }
  else
  {
    // Use the particle gun that was configured in the .mac file
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
}

