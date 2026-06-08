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

#include "G4ThreeVector.hh"

#include "Api/PandoraApi.h"
#include "Api/PandoraContentApi.h"
#include "Objects/CartesianVector.h"
#include "Pandora/PandoraInternal.h"
#include "Persistency/BinaryFileWriter.h"
#include "Persistency/XmlFileWriter.h"
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

    if (fUseXMLNotBinary)
    {
        fEventWriter = new pandora::XmlFileWriter(*fPandora, "larsimple_event.xml", pandora::FileMode::OVERWRITE);
        if (fWriteGeometry)
            fGeomWriter = new pandora::XmlFileWriter(*fPandora, "larsimple_geom.xml", pandora::FileMode::OVERWRITE);
    }
    else
    {
        fEventWriter = new pandora::BinaryFileWriter(*fPandora, "larsimple_event.pndr", pandora::FileMode::OVERWRITE);
        if (fWriteGeometry)
            fGeomWriter = new pandora::BinaryFileWriter(*fPandora, "larsimple_geom.pndr", pandora::FileMode::OVERWRITE);
    }

    if (fWriteGeometry)
    {
        this->CreateLArTPC();
        PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, fGeomWriter->WriteGeometry());
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMplePandoraWriter::~LArSIMplePandoraWriter()
{
    delete fEventWriter;
    delete fGeomWriter;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateCaloHits(const std::vector<LArSIMple3DEnergyDeposit> &hits)
{
//    for (const LArSIMple3DEnergyDeposit hit : hits)
    for (unsigned int h = 0; h < hits.size(); ++h)
        this->CreateCaloHitFrom3DEnergyDeposit(h, hits.at(h));
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

void LArSIMplePandoraWriter::WriteEvent()
{
    const pandora::CaloHitList *pCaloHitList = nullptr;
    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));

    const pandora::TrackList *pTrackList = nullptr;
    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pTrackList));

    const pandora::MCParticleList *pMCParticleList = nullptr;
    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pMCParticleList));

    PANDORA_THROW_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=,
        fEventWriter->WriteEvent(*pCaloHitList, *pTrackList, *pMCParticleList, true, false));
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMplePandoraWriter::CreateCaloHitFrom3DEnergyDeposit(const unsigned int hitNumber, const LArSIMple3DEnergyDeposit &hit)
{
    lar_content::LArCaloHitFactory hitFactory;

    // Each 3D energy deposit actually gives us three hits - one in each view
    const G4ThreeVector pos3D{hit.GetPosition()};
    const G4ThreeVector posUVW{hit.GetUVWPosition()};

    const float voxelWidth{0.5f};
    const float MipE{0.00075}; // Pandora expects mips too?
    const float voxelE(hit.GetEnergy());
    const float voxelMipEquivalentE{voxelE / MipE};
   
    lar_content::LArCaloHitParameters hitParams;
    hitParams.m_positionVector = pandora::CartesianVector(pos3D.x() / 10.f, 0.f, posUVW.x() / 10.f);
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
    hitParams.m_hitType = pandora::TPC_VIEW_U;
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
        PandoraApi::SetCaloHitToMCParticleRelationship(*fPandora, (void *)((intptr_t)hitNumber), (void *)((intptr_t)hit.GetParticleTrackID()), 1.f);
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


