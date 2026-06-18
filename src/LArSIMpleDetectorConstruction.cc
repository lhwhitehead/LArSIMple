/**
 *  @file   LArSIMple/src/LArSIMpleDetectorConstruction.cc
 * 
 *  @brief  Implementation for the detector construction class.
 * 
 *  $Log: $
 */

#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleDetectorMessenger.hh"
#include "LArSIMpleNestedParameterisation.hh"
#include "LArSIMpleWireConvertor.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVParameterised.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include <iostream>

LArSIMpleDetectorConstruction::LArSIMpleDetectorConstruction() :
    fSolidWorld(nullptr),
    fLogicWorld(nullptr),
    fPhysiWorld(nullptr),
    fSolidLArBox(nullptr),
    fLogicLArBox(nullptr),
    fPhysiLArBox(nullptr)
{
    fDetectorMessenger = new LArSIMpleDetectorMessenger(this);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleDetectorConstruction::~LArSIMpleDetectorConstruction()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

G4VPhysicalVolume *LArSIMpleDetectorConstruction::Construct()
{
    this->DefineMaterials();
    this->ConstructWorld();
    this->ConstructLArBox();

    // Set up the geometry information needed by the WireConvertor class
    LArSIMpleWireConvertor &wireConvertor = LArSIMpleWireConvertor::Get(this);

    return fPhysiWorld;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleDetectorConstruction::DefineMaterials()
{
    // Try using the NIST database for our materials
    G4NistManager *manager = G4NistManager::Instance();
    fMaterialAir = manager->FindOrBuildMaterial("G4_AIR");
    fMaterialLAr = manager->FindOrBuildMaterial("G4_lAr");
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleDetectorConstruction::ConstructWorld()
{
    // World - remember we need half-lengths to define the box
    fSolidWorld = new G4Box("World", fWorldSizeX / 2.f, fWorldSizeY / 2.f, fWorldSizeZ / 2.f);
    fLogicWorld = new G4LogicalVolume(fSolidWorld, fMaterialAir, "World");
    fPhysiWorld = new G4PVPlacement(0, G4ThreeVector(), fLogicWorld, "World", 0, false, 0, fCheckOverlaps);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleDetectorConstruction::ConstructLArBox()
{
    // LAr volume
    fSolidLArBox = new G4Box("LArBox", fLArSizeX / 2.f, fLArSizeY / 2.f, fLArSizeZ / 2.f);
    fLogicLArBox = new G4LogicalVolume(fSolidLArBox, fMaterialLAr, "LArBox");
    fPhysiLArBox = new G4PVPlacement(0, G4ThreeVector(), fLogicLArBox, "LArBox", fLogicWorld, false, 0, fCheckOverlaps);

    if (fVoxeliseLAr)
        this->ConstructLArVoxels();

    fLogicLArBox->SetUserLimits(new G4UserLimits(1.0 / CLHEP::mm));
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleDetectorConstruction::ConstructLArVoxels()
{
    const double voxelSize = 5.0;
    const int nCellsX = static_cast<int>(fLArSizeX / voxelSize);
    const int nCellsY = static_cast<int>(fLArSizeY / voxelSize);
    const int nCellsZ = static_cast<int>(fLArSizeZ / voxelSize);

    std::cout << fLArSizeY << ", " << voxelSize << ", " << nCellsY << std::endl;

    // Firstly we build detector slices in Y
    G4VSolid *solYRep = new G4Box("LArBox_ySlices", fLArSizeX / 2., voxelSize / 2., fLArSizeZ / 2.);
    G4LogicalVolume *logYRep = new G4LogicalVolume(solYRep, fMaterialLAr, "LArBox_ySlices");
    new G4PVReplica("LArBox_ySlices", logYRep, fLogicLArBox, kYAxis, nCellsY, voxelSize);

    std::cout << "Constructed y slices" << std::endl;

    // Now for X
    G4VSolid *solXRep = new G4Box("LArBox_xSlices", voxelSize / 2., voxelSize / 2., fLArSizeZ / 2.);
    G4LogicalVolume *logXRep = new G4LogicalVolume(solXRep, fMaterialLAr, "LArBox_xSlices");
    new G4PVReplica("LArBox_xSlices", logXRep, logYRep, kXAxis, nCellsX, voxelSize);

    std::cout << "Constructed x slices" << std::endl;

    // We use our custom nested parameterisation for Z
    G4VSolid *solVoxel = new G4Box("LArBox_zSlices", voxelSize / 2., voxelSize / 2., voxelSize / 2.);
    G4LogicalVolume *logVoxel = new G4LogicalVolume(solVoxel, fMaterialLAr, "LArBox_zSlices");
    std::vector<G4Material *> materials({fMaterialLAr});
    // Parameterisation for transformation of voxels.
    //  (voxel size is fixed in this example.
    //  e.g. nested parameterisation handles material and transfomation of voxels.)
    LArSIMpleNestedParameterisation *paramVoxels =
        new LArSIMpleNestedParameterisation(materials, voxelSize / 2., voxelSize / 2., voxelSize / 2., nCellsZ);

    new G4PVParameterised("LArBox_zSlices", // their name
        logVoxel,                           // their logical volume
        logXRep,                            // Mother logical volume
        kUndefined,                         // Are placed along this axis
        nCellsZ,                            // Number of cells
        paramVoxels);                       // Parameterisation.
    std::cout << "Constructed voxels" << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleDetectorConstruction::PrintDetectorSummary() const
{
    std::cout << "Constructed LAr volume of size" << fLArSizeX << " X " << fLArSizeY << " X " << fLArSizeZ << std::endl;
}
