/**
 *  @file LArSIMple/src/LArSIMplePandoraWriter.cc
 * 
 *  @brief Implementation of the messenger class. 
 * 
 *  $Log: $
 */

#include <iostream>

#include "LArSIMplePandoraWriter.hh"

#include "G4ThreeVector.hh"

#include "Api/PandoraApi.h"
#include "Persistency/BinaryFileWriter.h"
#include "larpandoracontent/LArObjects/LArCaloHit.h"

LArSIMplePandoraWriter::LArSIMplePandoraWriter()
{
    std::cout << "Creating Pandora instance" << std::endl;
    fPandora = new pandora::Pandora();

    fWriter = new pandora::BinaryFileWriter(*fPandora, "event.pndr", pandora::FileMode::OVERWRITE);

}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMplePandoraWriter::~LArSIMplePandoraWriter()
{

}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateCaloHits(const std::vector<LArSIMple3DEnergyDeposit> &hits)
{
    pandora::CaloHitList caloHitList;

    for (const LArSIMple3DEnergyDeposit hit : hits)
        this->CreateCaloHitFrom3DEnergyDeposit(hit, caloHitList);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateCaloHitFrom3DEnergyDeposit(const LArSIMple3DEnergyDeposit &hit, pandora::CaloHitList &caloHitList)
{
    lar_content::LArCaloHitFactory hitFactory;

    // Each 3D energy deposit actually gives us three hits - one in each view
    const G4ThreeVector pos3D{hit.GetPosition()};
    const G4ThreeVector posUVW{hit.GetUVWPosition()};

    const float voxelWidth{0.5f};
   
    lar_content::LArCaloHitParameters hitParams;
    hitParams.m_positionVector = pandora::CartesianVector(pos3D.x() / 10.f, 0.f, posUVW.x() / 10.f);
    hitParams.m_expectedDirection = pandora::CartesianVector(0.f, 0.f, 1.f);
    hitParams.m_cellNormalVector = pandora::CartesianVector(0.f, 0.f, 1.f);
    hitParams.m_cellGeometry = pandora::RECTANGULAR;
    hitParams.m_cellSize0 = voxelWidth;
    hitParams.m_cellSize1 = voxelWidth;
    hitParams.m_cellThickness = voxelWidth;
//    hitParams.m_nCellRadiationLengths = 1.f;
//    hitParams.m_nCellInteractionLengths = 1.f;
//    hitParams.m_time = 0.f;
//    hitParams.m_inputEnergy = voxelE;
//    hitParams.m_mipEquivalentEnergy = voxelMipEquivalentE;
//    hitParams.m_electromagneticEnergy = voxelE;
//    hitParams.m_hadronicEnergy = voxelE;
//    hitParams.m_isDigital = false;
//    hitParams.m_hitType = pandora::TPC_3D;
//    hitParams.m_hitRegion = pandora::SINGLE_REGION;
//    hitParams.m_layer = 0;
//    hitParams.m_isInOuterSamplingLayer = false;
//    hitParams.m_pParentAddress = (void *)(static_cast<uintptr_t>(++hitCounter));
//    hitParams.m_larTPCVolumeId = tpcID < 0 ? 0 : tpcID;
//    hitParams.m_daughterVolumeId = 0;

}

