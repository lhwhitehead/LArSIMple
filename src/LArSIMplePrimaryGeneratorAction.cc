/**
 *  @file LArSIMple/src/LArSIMplePrimaryGeneratorAction.cc
 * 
 *  @brief Implementation of the primary generator action class.
 * 
 *  $Log: $
 */

#include <cmath>

#include "LArSIMplePrimaryGeneratorAction.hh"
#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleNeutrinoInputParser.hh"
#include "LArSIMplePrimaryGeneratorMessenger.hh"
#include "LArSIMpleRunAction.hh"
#include "LArSIMpleTrueNeutrinoEvent.hh"
#include "LArSIMpleTrueParticle.hh"

#include "Randomize.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "globals.hh"

LArSIMplePrimaryGeneratorAction::LArSIMplePrimaryGeneratorAction(const LArSIMpleDetectorConstruction *detCon) :
    fDetectorConstruction(detCon)
{
    fParticleGun = new G4GeneralParticleSource();
    fMessenger = new LArSIMplePrimaryGeneratorMessenger(this);
    fNeutrinoEvent = nullptr;
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMplePrimaryGeneratorAction::~LArSIMplePrimaryGeneratorAction()
{
    delete fParticleGun;
    delete fMessenger;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
    const unsigned int eventNumber{static_cast<unsigned int>(anEvent->GetEventID())};

    // Check if we are processing neutrinos
    if (fUseNeutrinos)
    {
        if (fNeutrinoInputParser.GetNEvents() == 0)
        {
            // Check that the neutrino parser is ready if we request neutrino events
            fNeutrinoInputParser.ReadFile(fNeutrinoFileName, fNeutrinoFileType);
        }

        if (fNeutrinoInputParser.GetNEvents() >= eventNumber)
        {
            fNeutrinoEvent = new LArSIMpleTrueNeutrinoEvent(fNeutrinoInputParser.GetEvent(eventNumber));
        }
        else
        {
            std::cerr << "Requested event number is greater than the number of neutrinos. Processing the last one again." << std::endl;
            fNeutrinoEvent = new LArSIMpleTrueNeutrinoEvent(fNeutrinoInputParser.GetEvent(fNeutrinoInputParser.GetNEvents() - 1));
        }

        G4ThreeVector neutrinoVertex = fNeutrinoVertex;
        if (fUseRandomNeutrinoVertex)
            this->GenerateRandomVertex(neutrinoVertex);

        // Store the vertex positon
        fNeutrinoEvent->SetInteractionVertex(neutrinoVertex);

        // Use a particle gun for the neutrinos as it is more convenient
        G4ParticleGun *particleGun = new G4ParticleGun();
        this->GenerateNeutrinoPrimaries(anEvent, particleGun);
    }
    else if (fUseParticleBombs)
    {
        G4ParticleGun *particleGun = new G4ParticleGun();
        this->GenerateParticleBombPrimaries(anEvent, particleGun);
    }
    else
    {
        // Use the particle gun that was configured in the .mac file
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePrimaryGeneratorAction::GenerateNeutrinoPrimaries(G4Event *anEvent, G4ParticleGun *particleGun)
{
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    
    // We need to fire all of the final-state particles
    const std::vector<LArSIMpleTrueParticle> finalStateParticles = fNeutrinoEvent->GetFinalStateParticles();
    for (const LArSIMpleTrueParticle &part : finalStateParticles)
    {
        // Get the particle mass and hence kinetic energy
        // First, check if this particle exists in G4. If now, ignore for now
        G4ParticleDefinition *particleDef = particleTable->FindParticle(part.GetPDGCode());
        if (particleDef == nullptr)
        {
            std::cerr << "LArSimplePrimaryGeneratorAction :: unknown particle found: " << part.GetPDGCode() << std::endl;
            continue;
        }
        particleGun->SetParticleDefinition(particleDef);
        const G4double mass{particleGun->GetParticleDefinition()->GetPDGMass()};
        const G4double ekin{part.GetEnergy() - mass};
    
        particleGun->SetParticleEnergy(ekin);
        particleGun->SetParticlePosition(fNeutrinoVertex);
        particleGun->SetParticleTime(0.0); // Hard-code for now
        particleGun->SetParticleMomentumDirection(part.GetDirection());
        particleGun->GeneratePrimaryVertex(anEvent);
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePrimaryGeneratorAction::GenerateParticleBombPrimaries(G4Event *anEvent, G4ParticleGun *particleGun)
{
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    
    unsigned int nParticles{0};
    G4ThreeVector firstParticleDir;

    for (const std::pair<int, unsigned int> &particlePair : fParticleBombParticles)
    {        
        for (unsigned int p = 0; p < particlePair.second; ++p)
        {
            G4ThreeVector direction(this->GenerateIsotropicDirection());
            // Special treatment if we want other particles generated in a cone around the first
            if (fParticleBombUseCone)
            {
                if (nParticles == 0)
                    firstParticleDir = direction;
                else
                    direction = this->GenerateDirectionWithinCone(firstParticleDir);
            }

            G4ParticleDefinition *particleDef = particleTable->FindParticle(particlePair.first);
            if (particleDef == nullptr)
            {
                std::cerr << "LArSimplePrimaryGeneratorAction :: unknown particle found: " << particlePair.first << std::endl;
                continue;
            }
            particleGun->SetParticleDefinition(particleDef);
            const G4double ekin{fParticleBombMinKE + G4UniformRand() * (fParticleBombMaxKE - fParticleBombMinKE)};

            particleGun->SetParticleEnergy(ekin);
            particleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
            particleGun->SetParticleTime(0.0); 
            particleGun->SetParticleMomentumDirection(direction);
            particleGun->GeneratePrimaryVertex(anEvent);

            ++nParticles;
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePrimaryGeneratorAction::GenerateRandomVertex(G4ThreeVector &vtx) const
{
    // Choose a random position within the LAr volume (at least 1m from the edges)
    const float buffer{1000.f};
    const float minX{fDetectorConstruction->GetLArSizeMinX() + buffer};
    const float maxX{fDetectorConstruction->GetLArSizeMaxX() - buffer};
    const float minY{fDetectorConstruction->GetLArSizeMinY() + buffer};
    const float maxY{fDetectorConstruction->GetLArSizeMaxY() - buffer};
    const float minZ{fDetectorConstruction->GetLArSizeMinZ() + buffer};
    const float maxZ{fDetectorConstruction->GetLArSizeMaxZ() - buffer};

    vtx.setX(minX + G4UniformRand() * (maxX - minX));
    vtx.setY(minY + G4UniformRand() * (maxY - minY));
    vtx.setZ(minZ + G4UniformRand() * (maxZ - minZ));
}

//------------------------------------------------------------------------------------------------------------------------------------------

G4ThreeVector LArSIMplePrimaryGeneratorAction::GenerateIsotropicDirection() const
{
    // This is the same as picking a random point on a unit sphere
    const double a{2.0 * G4UniformRand() - 1};
    const double b{2.0 * std::acos(-1.0) * G4UniformRand()};

    const double x = std::sqrt(1 - a * a) * std::cos(b);
    const double y = std::sqrt(1 - a * a) * std::sin(b);
    const double z = a;

    G4ThreeVector dir(x, y, z);
    return dir;
}

//------------------------------------------------------------------------------------------------------------------------------------------

G4ThreeVector LArSIMplePrimaryGeneratorAction::GenerateDirectionWithinCone(const G4ThreeVector &baseDir) const
{
    // Step 1: Generate a random angle between 0 and theta_max
    const double theta{fParticleBombConeAngle * CLHEP::deg * G4UniformRand()};

    // Step 2: Find a random perpendicular vector to A
    // We can do this by taking the cross product with an arbitrary vector
    G4ThreeVector randomVector(1.0, 0.0, 0.0);  // Arbitrary vector not collinear with A
    if (baseDir.isParallel(randomVector, 0.01)) {  // Ensure it's not collinear with A
        randomVector.set(0.0, 1.0, 0.0);
    }

    // Cross product to find a perpendicular vector
    G4ThreeVector perpVec = baseDir.cross(randomVector);
    perpVec = perpVec.unit();  // Normalize to get a unit vector

    // Step 3: Calculate the new vector B
    // B = cos(theta) * A + sin(theta) * R
    G4ThreeVector dirInCone = baseDir.unit() * std::cos(theta) + perpVec * std::sin(theta);

    return dirInCone.unit();
}

