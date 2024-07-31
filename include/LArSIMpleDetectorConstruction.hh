/**
 *  @file   LArSIMple/include/LArSIMpleDetectorConstruction.hh
 * 
 *  @brief  Header file for the detector construction class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleDetectorConstruction_h
#define LArSIMpleDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class LArSIMpleDetectorMessenger;

/**
 *   @brief Detector construction class
 */
class LArSIMpleDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    /**
     *  @brief  Default constructor
     */
    LArSIMpleDetectorConstruction();

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleDetectorConstruction();

    /**
     *  @brief  Construct the geometry
     */
    G4VPhysicalVolume *Construct();

    /**
     *  @brief  Update the geometry
     */
    void UpdateGeometry();

    /**
     *  @brief  Get the width (x) of the world volume
     *
     *  @return The world volume width
     */
    float GetWorldWidth() const;

    /**
     *  @brief  Get the height (y) of the world volume
     *
     *  @return The world volume height
     */
    float GetWorldHeight() const;

    /**
     *  @brief  Get the length (z) of the world volume
     *
     *  @return The world volume length
     */
    float GetWorldLength() const;

    /**
     *  @brief  Set the width (x) of the world volume
     *
     *  @param  width the world volume width
     */
    void SetWorldWidth(double width);

    /**
     *  @brief  Set the height (y) of the world volume
     *
     *  @param  height the world volume height
     */
    void SetWorldHeight(double height);

    /**
     *  @brief  Set the length (z) of the world volume
     *
     *  @return length the world volume length
     */
    void SetWorldLength(double length);

    /**
     *  @brief  Get the width (x) of the LAr volume
     *
     *  @return The LAr volume width
     */
    float GetLArWidth() const;

    /**
     *  @brief  Get the height (y) of the LAr volume
     *
     *  @return The LAr volume height
     */
    float GetLArHeight() const;

    /**
     *  @brief  Get the length (z) of the LAr volume
     *
     *  @return The LAr volume length
     */
    float GetLArLength() const;

    /**
     *  @brief  Set the width (x) of the LAr volume
     *
     *  @param  width the LAr volume width
     */
    void SetLArWidth(double width);

    /**
     *  @brief  Set the height (y) of the LAr volume
     *
     *  @param  height the LAr volume height
     */
    void SetLArHeight(double height);

    /**
     *  @brief  Set the length (z) of the LAr volume
     *
     *  @return length the LAr volume length
     */
    void SetLArLength(double length);

    /**
     *  @brief  Get the minimum x position of the LAr volume
     *
     *  @return The minimum x position of the LAr volume
     */
    float GetLArSizeMinX() const;

    /**
     *  @brief  Get the minimum y position of the LAr volume
     *
     *  @return The minimum y position of the LAr volume
     */
    float GetLArSizeMinY() const;

    /**
     *  @brief  Get the minimum z position of the LAr volume
     *
     *  @return The minimum z position of the LAr volume
     */
    float GetLArSizeMinZ() const;

    /**
     *  @brief  Get the maximum x position of the LAr volume
     *
     *  @return The maximum x position of the LAr volume
     */
    float GetLArSizeMaxX() const;

    /**
     *  @brief  Get the maximum y position of the LAr volume
     *
     *  @return The maximum y position of the LAr volume
     */
    float GetLArSizeMaxY() const;

    /**
     *  @brief  Get the maximum z position of the LAr volume
     *
     *  @return The maximum z position of the LAr volume
     */
    float GetLArSizeMaxZ() const;

    /**
     *  @brief  Is the LAr volume voxelised?
     *
     *  @return true if voxelised
     */
    bool GetVoxeliseLAr() const;

    /**
     *  @brief  Set if the LAr volume voxelised?
     *
     *  @param  Set if the LAr volume voxelised
     */
    void SetVoxeliseLAr(bool flag);

    /**
     *  @brief  Is Geant4 checking geometry overlaps?
     *
     *  @return true if it is checking
     */
    bool GetCheckOverlaps() const;

    /**
     *  @brief  Set whether to check for geometry overlaps
     *
     *  @param  check whether to check if voxelised
     */
    void SetCheckOverlaps(bool check)

    /**
     *  @brief  
     *
     *  @return true if voxelised
     */
    void PrintDetectorSummary() const;

private:
    /**
     *  @brief  Define the materials for the geometry
     */
    void DefineMaterials();

    /**
     *  @brief  Build the world volume
     */
    void ConstructWorld();

    /**
     *  @brief  Build the LAr volume
     */
    void ConstructLArBox();

    /**
     *  @brief  Divide the LAr volume into voxels if required
     */
    void ConstructLArVoxels();

    float fWorldSizeX;                  ///< Size of the world volume in x
    float fWorldSizeY;                  ///< Size of the world volume in y
    float fWorldSizeZ;                  ///< Size of the world volume in z

    float fLArSizeX;                    ///< Size of the LAr volume in x
    float fLArSizeY;                    ///< Size of the LAr volume in y
    float fLArSizeZ;                    ///< Size of the LAr volume in z

    bool fVoxeliseLAr;                  ///< Whether to voxelise the LAr volume
    bool fCheckOverlaps;                ///< Whether to check for geometry overlaps

    G4Material *fMaterialLAr;           ///< Geant4 material for air
    G4Material *fMaterialAir;           ///< Geant4 material for liquid argon

    G4Box *fSolidWorld;                 ///< World Geant4 solid volume
    G4LogicalVolume *fLogicWorld;       ///< World Geant4 logical volume
    G4VPhysicalVolume *fPhysiWorld;     ///< World Geant4 physical volume

    G4Box *fSolidLArBox;                ///< LAr Geant4 solid volume
    G4LogicalVolume *fLogicLArBox;      ///< LAr Geant4 logical volume
    G4VPhysicalVolume *fPhysiLArBox;    ///< LAr Geant4 physical volume

    LArSIMpleDetectorMessenger *fDetectorMessenger; ///< Pointer to the messenger class
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetWorldWidth() const
{
    return fWorldSizeX;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetWorldHeight() const
{
    return fWorldSizeY;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetWorldLength() const
{
    return fWorldSizeZ;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWorldWidth(double width)
{
    fWorldSizeX = width;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWorldHeight(double height)
{
    fWorldSizeY = height;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWorldLength(double length)
{
    fWorldSizeZ = length;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArWidth() const
{
    return fLArSizeX;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArHeight() const
{
    return fLArSizeY;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArLength() const
{
    return fLArSizeZ;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetLArWidth(double width)
{
    fLArSizeX = width;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetLArHeight(double height)
{
    fLArSizeY = height;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetLArLength(double length)
{
    fLArSizeZ = length;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArSizeMinX() const
{
    return -1.f * fLArSizeX / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArSizeMinY() const
{
    return -1.f * fLArSizeY / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArSizeMinZ() const
{
    return -1.f * fLArSizeZ / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArSizeMaxX() const
{
    return fLArSizeX / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArSizeMaxY() const
{
    return fLArSizeY / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArSizeMaxZ() const
{
    return fLArSizeZ / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline bool LArSIMpleDetectorConstruction::GetVoxeliseLAr() const
{
    return fVoxeliseLAr;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetVoxeliseLAr(bool flag)
{
    fVoxeliseLAr = flag;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline bool LArSIMpleDetectorConstruction::GetCheckOverlaps() const
{
    return fCheckOverlaps;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetCheckOverlaps(bool check)
{
    fCheckOverlaps = check;
}

#endif
