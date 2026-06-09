/**
 *  @file LArSIMple/src/LArSIMplePandoraWriter.cc
 * 
 *  @brief Implementation of the messenger class. 
 * 
 *  $Log: $
 */

#include <iostream>

#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleTrackData.hh"
#include "LArSIMplePandoraWriter.hh"
#include "LArSIMplePandoraContent.hh"

#include "G4ThreeVector.hh"

#include "Api/PandoraApi.h"
#include "Api/PandoraContentApi.h"
#include "Objects/CartesianVector.h"
#include "Pandora/PandoraInternal.h"
#include "larpandoracontent/LArObjects/LArCaloHit.h"
#include "larpandoracontent/LArObjects/LArMCParticle.h"

LArSIMplePandoraWriter::LArSIMplePandoraWriter(const LArSIMpleDetectorConstruction *const detector, const unsigned int eventNumber, const bool useXMLNotBinary) :
fDetector(detector),
fEventNumber(eventNumber),
fUseXMLNotBinary(useXMLNotBinary),
fWriteGeometry(true)
{
    std::cout << "Creating Pandora instance" << std::endl;
    fPandora = new pandora::Pandora();

}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMplePandoraWriter::~LArSIMplePandoraWriter()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateCaloHits(const std::vector<LArSIMpleWireHit> &hits)
{
//    for (const LArSIMpleWireHit hit : hits)
    for (unsigned int h = 0; h < hits.size(); ++h)
        this->CreateCaloHitFromWireHit(h, hits.at(h));
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateMCParticles(const std::vector<LArSIMpleTrackData> &mcParticles)
{
    for (const LArSIMpleTrackData &mcParticle : mcParticles)
        this->CreateMCParticle(mcParticle);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateLArTPC()
{
    PandoraApi::Geometry::LArTPC::Parameters tpcParams;

    // Currently assumes detector centre is at (0,0,0)
    tpcParams.m_centerX = 0.f;
    tpcParams.m_centerY = 0.f;
    tpcParams.m_centerZ = 0.f;
    tpcParams.m_widthX = fDetector->GetLArWidth();
    tpcParams.m_widthY = fDetector->GetLArHeight();
    tpcParams.m_widthZ = fDetector->GetLArLength();
    tpcParams.m_larTPCVolumeId = 0;
    tpcParams.m_wirePitchU = 0.5f; // Currently hard coded as 5mm
    tpcParams.m_wirePitchV = 0.5f;
    tpcParams.m_wirePitchW = 0.5f;
    tpcParams.m_wireAngleU = fDetector->GetWireAngleU();
    tpcParams.m_wireAngleV = fDetector->GetWireAngleV();
    tpcParams.m_wireAngleW = fDetector->GetWireAngleW();
    tpcParams.m_sigmaUVW = 1;
    tpcParams.m_isDriftInPositiveX = 1;

    try
    {
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::Geometry::LArTPC::Create(*fPandora, tpcParams));
    }
    catch (const pandora::StatusCodeException &)
    {
        std::cerr << "Failed to create Pandora LArTPC" << std::endl;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::RunPandora()
{
    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, LArSIMplePandoraContent::RegisterAlgorithms(*fPandora));

    std::string fullConfigFileName = "config/PandoraSettings.xml";
    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS,
                            !=,
                            PandoraApi::ReadSettings(*fPandora, fullConfigFileName));

    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::ProcessEvent(*fPandora));
    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::Reset(*fPandora));
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateCaloHitFromWireHit(const unsigned int hitNumber, const LArSIMpleWireHit &hit)
{
    lar_content::LArCaloHitFactory hitFactory;

    const float voxelWidth{0.5f};
    const float MipE{0.00075}; // Pandora expects mips too?
    const float voxelE(hit.GetCharge());
    const float voxelMipEquivalentE{voxelE / MipE};
   
    lar_content::LArCaloHitParameters hitParams;
    hitParams.m_positionVector = pandora::CartesianVector(hit.GetWireNumber() * 0.1f, 0.f, hit.GetDriftBin() * 0.1f);
    hitParams.m_expectedDirection = pandora::CartesianVector(0.f, 0.f, 1.f);
    hitParams.m_cellNormalVector = pandora::CartesianVector(0.f, 0.f, 1.f);
    hitParams.m_cellGeometry = pandora::RECTANGULAR;
    hitParams.m_cellSize0 = voxelWidth;
    hitParams.m_cellSize1 = voxelWidth;
    hitParams.m_cellThickness = voxelWidth;
    hitParams.m_nCellRadiationLengths = 1.f;
    hitParams.m_nCellInteractionLengths = 1.f;
    hitParams.m_time = 0.f;
    hitParams.m_inputEnergy = voxelE;
    hitParams.m_mipEquivalentEnergy = voxelMipEquivalentE;
    hitParams.m_electromagneticEnergy = voxelE;
    hitParams.m_hadronicEnergy = voxelE;
    hitParams.m_isDigital = false;
    hitParams.m_hitType = static_cast<pandora::HitType>(static_cast<unsigned short>(hit.GetView()) + 4); // Enums offset by 4
    hitParams.m_hitRegion = pandora::SINGLE_REGION;
    hitParams.m_layer = 0;
    hitParams.m_isInOuterSamplingLayer = false;
    hitParams.m_pParentAddress = (void *)(static_cast<uintptr_t>(hitNumber));
    hitParams.m_larTPCVolumeId = 0;
    hitParams.m_daughterVolumeId = 0;

    try
    {
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=,PandoraApi::CaloHit::Create(*fPandora, hitParams, hitFactory));
    }
    catch(...)
    {
        std::cerr << "Failed to create LArCaloHit" << std::endl;
    }

    try
    {
        PandoraApi::SetCaloHitToMCParticleRelationship(*fPandora, (void *)((intptr_t)hitNumber), (void *)((intptr_t)hit.GetLargestContributingTrackId()), 1.f);
    }
    catch(...)
    {
        std::cerr << "Failed to create LArCaloHit to LArMCParticle link" << std::endl;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateMCParticle(const LArSIMpleTrackData &mcParticle)
{
        lar_content::LArMCParticleFactory mcParticleFactory;
        lar_content::LArMCParticleParameters mcParticleParams;

        // Initial momentum and energy in GeV
        const float px = mcParticle.GetVertexDirection().getX() * mcParticle.GetVertexMomentum();
        const float py = mcParticle.GetVertexDirection().getY() * mcParticle.GetVertexMomentum();
        const float pz = mcParticle.GetVertexDirection().getZ() * mcParticle.GetVertexMomentum();
        const float energySq = mcParticle.GetVertexMomentum() * mcParticle.GetVertexMomentum() + mcParticle.GetMass() * mcParticle.GetMass();
        mcParticleParams.m_energy = sqrt(energySq);
        mcParticleParams.m_momentum = pandora::CartesianVector(px, py, pz);

        // Particle codes
        mcParticleParams.m_particleId = mcParticle.GetPDG();
        mcParticleParams.m_mcParticleType = pandora::MC_3D;
        mcParticleParams.m_nuanceCode = mcParticle.IsPrimary() ? 2001 : 0;

        // Unique file-based ID for this MC particle
        mcParticleParams.m_pParentAddress = (void *)((intptr_t)mcParticle.GetTrackID());

        // Start and end points in cm
        const float startx = mcParticle.GetVertexPosition().getX() * 0.1f;
        const float starty = mcParticle.GetVertexPosition().getY() * 0.1f;
        const float startz = mcParticle.GetVertexPosition().getZ() * 0.1f;
        mcParticleParams.m_vertex = pandora::CartesianVector(startx, starty, startz);

        const float endx = mcParticle.GetEndPosition().getX() * 0.1f;
        const float endy = mcParticle.GetEndPosition().getY() * 0.1f;
        const float endz = mcParticle.GetEndPosition().getZ() * 0.1f;
        mcParticleParams.m_endpoint = pandora::CartesianVector(endx, endy, endz);

        // Process ID
        mcParticleParams.m_process = mcParticle.GetProcessCode();

        // Create MCParticle
        try
        {
            PANDORA_THROW_RESULT_IF(
                pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::MCParticle::Create(*fPandora, mcParticleParams, mcParticleFactory));
        }
        catch (const pandora::StatusCodeException &)
        {
            std::cerr << "Unable to create MCParticle " << std::endl;
        }

        // Create Parent Links
        try
        {
            PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=,
                PandoraApi::SetMCParentDaughterRelationship(*fPandora, (void *)((intptr_t)mcParticle.GetParentID()), (void *)((intptr_t)mcParticle.GetTrackID())));
        }
        catch (const pandora::StatusCodeException &)
        {
            std::cerr << "Unable to create MCParticle parent and child links" << std::endl;
        }
        
}

