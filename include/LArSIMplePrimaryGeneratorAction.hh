/**
 *  @file   LArSIMple/include/LArSIMplePrimaryGeneratorAction.hh
 * 
 *  @brief  Header file for the primary generator action class.
 * 
 *  $Log: $
 */

#ifndef LArSIMplePrimaryGeneratorAction_h
#define LArSIMplePrimaryGeneratorAction_h 1

#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "LArSIMpleNeutrinoInputParser.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class LArSIMplePrimaryGeneratorMessenger;
class LArSIMpleDetectorConstruction;
class LArSIMpleTrueNeutrinoEvent;

/**
 *  @brief The primary generator action class that generates the GEANT4 events
 */
class LArSIMplePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  detCon the detector construction object that creates the geometry
     */
    LArSIMplePrimaryGeneratorAction(const LArSIMpleDetectorConstruction *detCon);

    /**
     *  @brief  Destructor
     */
    ~LArSIMplePrimaryGeneratorAction();

    /**
     *  @brief  Generate the primary particles from the true events
     *
     *  @param  anEvent the pointer to a GEANT4 event
     */
    void GeneratePrimaries(G4Event *anEvent);

    /**
     *  @brief  Generate the primary particles from the true neutrino events
     *
     *  @param  anEvent the pointer to a GEANT4 event
     *  @param  particleGun the pointer to the G4 particle gun
     */
    void GenerateNeutrinoPrimaries(G4Event *anEvent, G4ParticleGun *particleGun);

    /**
     *  @brief  Generate the primary particles from the particle bomb events
     *
     *  @param  anEvent the pointer to a GEANT4 event
     *  @param  particleGun the pointer to the G4 particle gun
     */
    void GenerateParticleBombPrimaries(G4Event *anEvent, G4ParticleGun *particleGun);

    /**   
     *  @brief  Set the input neutrino input file (including path)
     *
     *  @param  inputFilePath the input file path
     */
    void SetNeutrinoFileName(std::string inputFilePath);

    /**
     *  @brief  Set the input neutrino input file type
     *
     *  @param  inputFileType the input file type
     */
    void SetNeutrinoFileType(LArSIMpleNeutrinoInputType inputFileType);

    /**
     *  @brief  Set whether to use neutrino events as opposed to a particle gun
     *
     *  @param  val whether to use neutrinos or not
     */
    void SetUseNeutrinos(bool val);

    /**
     *  @brief  Set whether to randomise the neutrino vertex position
     *
     *  @param  val whether or not to randomise the vertex
     */
    void SetUseRandomNeutrinoVertex(bool val);

    /**
     *  @brief  Set the position of the neutrino events to a fixed value
     *
     *  @param  vec the position of the vertex
     */
    void SetNeutrinoVertex(G4ThreeVector vec);

    /**
     *  @brief  Get the true neutrino event 
     *
     *  @return The pointer to the true neutrino event
     */
    const LArSIMpleTrueNeutrinoEvent *GetTrueNeutrinoEventPointer() const;

    /**
     *  @brief  Set whether to use particle bomb events as opposed to a particle gun
     *
     *  @param  val whether to use particle bombs or not
     */
    void SetUseParticleBombs(bool val);

    /**
     *  @brief  Set the lepton pdg code for particle bomb events
     *
     *  @param  pdg the pdg code of the lepton
     */
    void SetParticleBombLeptonPdg(int pdg);

    /**
     *  @brief  Set the number of protons for particle bomb events
     *
     *  @param  n the number of protons
     */
    void SetParticleBombNProton(unsigned int n);

    /**
     *  @brief  Set the number of neutrons for particle bomb events
     *
     *  @param  n the number of neutrons
     */
    void SetParticleBombNNeutron(unsigned int n);

    /**
     *  @brief  Set the number of pi0 for particle bomb events
     *
     *  @param  n the number of pi0
     */
    void SetParticleBombNPiZero(unsigned int n);

    /**
     *  @brief  Set the number of pi+ for particle bomb events
     *
     *  @param  n the number of pi+
     */
    void SetParticleBombNPiPlus(unsigned int n);

    /**
     *  @brief  Set the number of pi- for particle bomb events
     *
     *  @param  n the number of pi-
     */
    void SetParticleBombNPiMinus(unsigned int n);

    /**
     *  @brief  Set the number of K0 for particle bomb events
     *
     *  @param  n the number of K0
     */
    void SetParticleBombNKZero(unsigned int n);

    /**
     *  @brief  Set the number of K+ for particle bomb events
     *
     *  @param  n the number of K+
     */
    void SetParticleBombNKPlus(unsigned int n);

    /**
     *  @brief  Set the number of K- for particle bomb events
     *
     *  @param  n the number of K-
     */
    void SetParticleBombNKMinus(unsigned int n);

    /**
     *  @brief  Set the minimum kinetic energy for particle bomb particles
     *
     *  @param  minKE the minimum kinetic energy
     */
    void SetParticleBombMinKE(double minKE);

    /**
     *  @brief  Set the maximum kinetic energy for particle bomb particles
     *
     *  @param  maxKE the maximum kinetic energy
     */
    void SetParticleBombMaxKE(double maxKE);

private:
    /**
     *  @brief  Generate a random vertex position
     *
     *  @param  vtx the three vector to take the random position
     */
    void GenerateRandomVertex(G4ThreeVector &vtx) const;

    /**
     *  @brief  Generate a direction from an isotropic distribution
     *
     *  @return A direction from an isotropic distribution
     */
    G4ThreeVector GenerateIsotropicDirection() const;

    /**
     *  @brief  Generate a random vertex position
     *
     *  @param  pdg the PDG code to add to the map
     *  @param  n the number of particles with PDG code pdg
     */
    void AddToParticleBombMap(int pdg, unsigned int n);

    G4GeneralParticleSource *fParticleGun;                ///< Pointer to the particle gun object
    LArSIMplePrimaryGeneratorMessenger *fMessenger;       ///< Pointer to the configuration object
    LArSIMpleNeutrinoInputParser fNeutrinoInputParser;    ///< The neutrino input parser object

    LArSIMpleTrueNeutrinoEvent *fNeutrinoEvent;           ///< The true neutrino event
    bool fUseNeutrinos;                                   ///< Whether to use neutrinos or not
    std::string fNeutrinoFileName;                        ///< File name and path of the input neutrinos
    LArSIMpleNeutrinoInputType fNeutrinoFileType;         ///< Type of neutrino input file
    bool fUseRandomNeutrinoVertex;                        ///< Whether to randomise the neutrino vertex position
    G4ThreeVector fNeutrinoVertex;                        ///< The neutrino vertex position

    bool fUseParticleBombs;                               ///< Whether to use particle bombs
    std::map<int, unsigned int> fParticleBombParticles;   ///< Map of pdg code to number of particles for particle bomb events     
    double fParticleBombMinKE;                            ///< Minimum kinetic energy for particle bomb particles
    double fParticleBombMaxKE;                            ///< Maximum kinetic energy for particle bomb particles

    const LArSIMpleDetectorConstruction *fDetectorConstruction;   ///< Pointer to the detector construction (geometry) object
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetNeutrinoFileName(std::string inputFilePath)
{
    fNeutrinoFileName = inputFilePath;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetNeutrinoFileType(LArSIMpleNeutrinoInputType inputFileType)
{
    fNeutrinoFileType = inputFileType;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetUseNeutrinos(bool val)
{
    fUseNeutrinos = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetUseRandomNeutrinoVertex(bool val)
{
    fUseRandomNeutrinoVertex = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetNeutrinoVertex(G4ThreeVector vec)
{
    fNeutrinoVertex = vec;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline const LArSIMpleTrueNeutrinoEvent *LArSIMplePrimaryGeneratorAction::GetTrueNeutrinoEventPointer() const
{
    return fNeutrinoEvent;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetUseParticleBombs(bool val)
{
    fUseParticleBombs = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombLeptonPdg(int pdg)
{
    const unsigned int absPdg = std::abs(pdg);
    if (absPdg == 11 || absPdg == 13 || absPdg == 15)
        this->AddToParticleBombMap(pdg, 1);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombNProton(unsigned int n)
{
    this->AddToParticleBombMap(2212, n);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombNNeutron(unsigned int n)
{
    this->AddToParticleBombMap(2112, n);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombNPiZero(unsigned int n)
{
    this->AddToParticleBombMap(111, n);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombNPiPlus(unsigned int n)
{
    this->AddToParticleBombMap(211, n);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombNPiMinus(unsigned int n)
{
    this->AddToParticleBombMap(-211, n);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombNKZero(unsigned int n)
{
    this->AddToParticleBombMap(311, n);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombNKPlus(unsigned int n)
{
    this->AddToParticleBombMap(321, n);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombNKMinus(unsigned int n)
{
    this->AddToParticleBombMap(-321, n);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombMinKE(double minKE)
{
    fParticleBombMinKE = minKE;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetParticleBombMaxKE(double maxKE)
{
    fParticleBombMaxKE = maxKE;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::AddToParticleBombMap(int pdg, unsigned int n)
{
    if (!fParticleBombParticles.count(pdg))
        fParticleBombParticles[pdg] = n;
    else
        std::cout << "Particles with PDG code " << pdg << " already in the particle bomb particle list" << std::endl;
}

#endif
