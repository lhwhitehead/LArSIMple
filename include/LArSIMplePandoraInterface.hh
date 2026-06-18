/**
 *  @file   LArSIMple/include/LArSIMplePandoraInterface.hh
 * 
 *  @brief  Header file for the pandora interface class.
 * 
 *  $Log: $
 */

#ifndef LArSIMplePandoraInterface_h
#define LArSIMplePandoraInterface_h

#include <vector>

#include "LArSIMpleWireHit.hh"
#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleTrackData.hh"

#include "Pandora/Algorithm.h"
#include "Api/PandoraApi.h"
#include "Persistency/FileWriter.h"

#include "larpandoracontent/LArObjects/LArCaloHit.h"

/**
 *  @brief PandoraInterface class for writing Pandora input xml files 
 */

class LArSIMplePandoraMessenger;

class LArSIMplePandoraInterface
{
public:
    /**
     *  @brief  Constructor
     */
    LArSIMplePandoraInterface(const LArSIMpleDetectorConstruction *const detector);

    /**
     *  @brief  Destructor
     */
    ~LArSIMplePandoraInterface();

    /**
     *  @brief  Create Pandora CaloHits from LArSIMpleWireHit objects
     *
     *  @param  hits the vector of LArSIMpleWireHit objects
     */   
    void CreateCaloHits(const std::vector<LArSIMpleWireHit> &hits);

    /**
     *  @brief  Create Pandora LArTPCs using the geometry
     */
    void CreateLArTPC();

    /**
     *  @brief  Create Pandora MCParticles from LArSIMpleTrackData objects
     *
     *  @param  mcParticles the vector of LArSIMpleTrackData objects
     */  
    void CreateMCParticles(const std::vector<LArSIMpleTrackData> &mcParticles);

    /**
     *  @brief  Run the Pandora instance
     */
    void RunPandora();

    /**
     *  @brief  Set whether to apply the calo hit charge threshold
     *
     *  @param  value whether to apply the threshold
     */
    void SetApplyCaloHitThreshold(const bool value);   

    /**
     *  @brief  Set the calo hit charge threshold
     *
     *  @param  value the value of the threshold
     */
    void SetCaloHitThreshold(const float value);

private:

    /**
     *  @brief  Create Pandora CaloHit from LArSIMpleWireHit object
     *
     *  @param  hitNumber the number of the hit in the hit vector
     *  @param  hit the LArSIMpleWireHit object
     */   
    void CreateCaloHitFromWireHit(const unsigned int hitNumber, const LArSIMpleWireHit &hit);

    /**
     *  @brief  Create Pandora MCParticle from LArSIMpleTrackData object
     *
     *  @param  mcParticle the LArSIMpleTrackData object
     */  
    void CreateMCParticle(const LArSIMpleTrackData &mcParticle);

    pandora::Pandora *fPandora;                           ///< The Pandora instance
    const LArSIMpleDetectorConstruction *const fDetector; ///< The detector object
    float fEnergyScale;                                   ///< Energy conversion factor for GEANT -> Pandora
    float fPositionScale;                                 ///< Position conversion factor for GEANT -> Pandora
    bool fBuiltGeometry;                                  ///< Check whether we have made the geometry

    LArSIMplePandoraMessenger *fMessenger;                ///< Pointer to the messenger object
    bool fApplyCaloHitThreshold;                          ///< Whether to apply the calo hit charge threshold
    float fCaloHitThreshold;                              ///< Value of the calo hit charge threshold
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePandoraInterface::SetApplyCaloHitThreshold(const bool value)
{
    fApplyCaloHitThreshold = value;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMplePandoraInterface::SetCaloHitThreshold(const float value)
{
    fCaloHitThreshold = value;
}

#endif
