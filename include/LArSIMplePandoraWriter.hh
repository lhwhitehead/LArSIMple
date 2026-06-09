/**
 *  @file   LArSIMple/include/LArSIMplePandoraWriter.hh
 * 
 *  @brief  Header file for the messenger class.
 * 
 *  $Log: $
 */

#ifndef LArSIMplePandoraWriter_h
#define LArSIMplePandoraWriter_h

#include <vector>

#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleTrackData.hh"

#include "Pandora/Algorithm.h"
#include "Api/PandoraApi.h"
#include "Persistency/FileWriter.h"

#include "larpandoracontent/LArObjects/LArCaloHit.h"

/**
 *  @brief PandoraWriter class for writing Pandora input xml files 
 */

class LArSIMplePandoraWriter
{
public:
    /**
     *  @brief  Constructor
     */
    LArSIMplePandoraWriter(const LArSIMpleDetectorConstruction *const detector, const unsigned int eventNumber, const bool useXMLNotBinary);

    /**
     *  @brief  Destructor
     */
    ~LArSIMplePandoraWriter();

    void CreateCaloHits(const std::vector<LArSIMple3DEnergyDeposit> &hits);

    void CreateLArTPC();

    void CreateMCParticles(const std::vector<LArSIMpleTrackData> &mcParticles);

    void RunPandora();

private:
    void CreateCaloHitFrom3DEnergyDeposit(const unsigned int hitNumber, const LArSIMple3DEnergyDeposit &hit);

    void CreateMCParticle(const LArSIMpleTrackData &mcParticle);

    pandora::Pandora *fPandora;

    lar_content::LArCaloHitFactory fLArCaloHitFactory;

    const LArSIMpleDetectorConstruction *const fDetector;
    unsigned int fEventNumber;
    bool fUseXMLNotBinary;
    bool fWriteGeometry; 
};

#endif
