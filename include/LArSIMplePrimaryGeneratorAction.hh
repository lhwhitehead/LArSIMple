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

private:
    /**
     *  @brief  Generate a random vertex position
     *
     *  @param  vtx the three vector to take the random position
     */
    void GenerateRandomVertex(G4ThreeVector &vtx) const;

    G4GeneralParticleSource *fParticleGun;                ///< Pointer to the particle gun object
    LArSIMplePrimaryGeneratorMessenger *fMessenger;       ///< Pointer to the configuration object
    LArSIMpleNeutrinoInputParser fNeutrinoInputParser;    ///< The neutrino input parser object

    LArSIMpleTrueNeutrinoEvent *fNeutrinoEvent;           ///< The true neutrino event

    bool fUseNeutrinos;                                   ///< Whether to use neutrinos or not
    std::string fNeutrinoFileName;                        ///< File name and path of the input neutrinos
    LArSIMpleNeutrinoInputType fNeutrinoFileType;         ///< Type of neutrino input file

    bool fUseRandomNeutrinoVertex;                        ///< Whether to randomise the neutrino vertex position
    G4ThreeVector fNeutrinoVertex;                        ///< The neutrino vertex position

    const LArSIMpleDetectorConstruction *fDetectorConstruction;   ///< Pointer to the detector construction (geometry) object
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetNeutrinoFileName(std::string inputFilePath)
{
    fNeutrinoFileName = val;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetNeutrinoFileType(LArSIMpleNeutrinoInputType inputFileType)
{
    fNeutrinoFileType = val;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetUseNeutrinos(bool val)
{
    fUseNeutrinos = val;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetUseRandomNeutrinoVertex(bool val)
{
    fUseRandomNeutrinoVertex = val;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePrimaryGeneratorAction::SetNeutrinoVertex(G4ThreeVector vec)
{
    fNeutrinoVertex = vec;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline const LArSIMplePrimaryGeneratorAction::LArSIMpleTrueNeutrinoEvent *GetTrueNeutrinoEventPointer() const
{
    return fNeutrinoEvent;
};

#endif
