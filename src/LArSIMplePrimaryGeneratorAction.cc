#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMplePrimaryGeneratorMessenger.hh"
#include "LArSIMpleRunAction.hh"
#include "LArSIMpleNeutrinoInputParser.hh"
#include "LArSIMpleTrueNeutrinoEvent.hh"
#include "LArSIMpleTrueParticle.hh"
#include "LArSIMpleDetectorConstruction.hh"

#include "Randomize.hh"

#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Event.hh"
#include "globals.hh"


LArSIMplePrimaryGeneratorAction::LArSIMplePrimaryGeneratorAction(const LArSIMpleDetectorConstruction *detCon) : fDetectorConstruction(detCon)
{  
  fParticleGun = new G4GeneralParticleSource();
  fMessenger = new LArSIMplePrimaryGeneratorMessenger(this);
  fNeutrinoEvent = nullptr;
}

LArSIMplePrimaryGeneratorAction::~LArSIMplePrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fMessenger;
}

void LArSIMplePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  const unsigned int eventNumber = static_cast<unsigned int>(anEvent->GetEventID());

  // Check if we are processing neutrinos
  if(this->UseNeutrinos())
  {
    if(fNeutrinoInputParser.GetNEvents() == 0)
    {
      // Check that the neutrino parser is ready if we request neutrino events
      fNeutrinoInputParser.ReadFile(fNeutrinoFileName, fNeutrinoFileType);
    }

    if(fNeutrinoInputParser.GetNEvents() >= eventNumber)
    {
      fNeutrinoEvent = new LArSIMpleTrueNeutrinoEvent(fNeutrinoInputParser.GetEvent(eventNumber));
    }
    else
    {
      std::cerr << "Requested event number is greater than the number of neutrinos. Processing the last one again." << std::endl;
      fNeutrinoEvent = new LArSIMpleTrueNeutrinoEvent(fNeutrinoInputParser.GetEvent(fNeutrinoInputParser.GetNEvents() - 1));
    }

    G4ThreeVector neutrinoVertex = fNeutrinoVertex;
    if(fUseRandomNeutrinoVertex)
      this->RandomiseVertex(neutrinoVertex); 
    const double vertexTime = 0.; // Hard code for now

    // Store the vertex positon
    fNeutrinoEvent->SetInteractionVertex(neutrinoVertex);

    // Use a particle gun for the neutrinos as it is more convenient
    G4ParticleGun *particleGun = new G4ParticleGun();
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();

    // We need to fire all of the final-state particles
    const std::vector<LArSIMpleTrueParticle> finalStateParticles = fNeutrinoEvent->GetFinalStateParticles();
    for(const LArSIMpleTrueParticle &part : finalStateParticles)
    {
      // Get the particle mass and hence kinetic energy
      // First, check if this particle exists in G4. If now, ignore for now
      G4ParticleDefinition *particleDef = particleTable->FindParticle(part.GetPDGCode());
      if(particleDef == nullptr)
      {
        std::cerr << "LArSimplePrimaryGeneratorAction :: unknown particle found: " << part.GetPDGCode() << std::endl; 
        continue;
      }
      particleGun->SetParticleDefinition(particleDef);
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

void LArSIMplePrimaryGeneratorAction::RandomiseVertex(G4ThreeVector &vtx) const
{
  // Choose a random position within the LAr volume (at least 1m from the edges)
  const float buffer(1000.f);
  const float minX = fDetectorConstruction->GetLArSizeMinX() + buffer;
  const float maxX = fDetectorConstruction->GetLArSizeMaxX() - buffer;
  const float minY = fDetectorConstruction->GetLArSizeMinY() + buffer;
  const float maxY = fDetectorConstruction->GetLArSizeMaxY() - buffer;
  const float minZ = fDetectorConstruction->GetLArSizeMinZ() + buffer;
  const float maxZ = fDetectorConstruction->GetLArSizeMaxZ() - buffer;
  
  vtx.setX(minX + G4UniformRand()*(maxX - minX));   
  vtx.setY(minY + G4UniformRand()*(maxY - minY));   
  vtx.setZ(minZ + G4UniformRand()*(maxZ - minZ));   
}
