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

#include "Api/PandoraApi.h"
#include "Persistency/BinaryFileWriter.h"

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
    LArSIMplePandoraWriter();

    /**
     *  @brief  Destructor
     */
    ~LArSIMplePandoraWriter();

    void CreateCaloHits(const std::vector<LArSIMple3DEnergyDeposit> &hits);

private:

    void CreateCaloHitFrom3DEnergyDeposit(const LArSIMple3DEnergyDeposit &hit, pandora::CaloHitList &caloHitList);

    pandora::Pandora *fPandora;
    pandora::BinaryFileWriter *fWriter;

    lar_content::LArCaloHitFactory fLArCaloHitFactory;
};

#endif
