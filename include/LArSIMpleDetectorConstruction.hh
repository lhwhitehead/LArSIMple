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

enum class LArSIMpleReadoutView
{
    ViewU = 0,
    ViewV = 1,
    ViewW = 2
};

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
    float GetLArVolumeMinX() const;

    /**
     *  @brief  Get the minimum y position of the LAr volume
     *
     *  @return The minimum y position of the LAr volume
     */
    float GetLArVolumeMinY() const;

    /**
     *  @brief  Get the minimum z position of the LAr volume
     *
     *  @return The minimum z position of the LAr volume
     */
    float GetLArVolumeMinZ() const;

    /**
     *  @brief  Get the maximum x position of the LAr volume
     *
     *  @return The maximum x position of the LAr volume
     */
    float GetLArVolumeMaxX() const;

    /**
     *  @brief  Get the maximum y position of the LAr volume
     *
     *  @return The maximum y position of the LAr volume
     */
    float GetLArVolumeMaxY() const;

    /**
     *  @brief  Get the maximum z position of the LAr volume
     *
     *  @return The maximum z position of the LAr volume
     */
    float GetLArVolumeMaxZ() const;

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
    void SetCheckOverlaps(bool check);

    /**
     *  @brief  Get the angle of the wires w.r.t the y-axis
     *
     *  @param  view the readout view
     *
     *  @return the angle to the y-axis
     */
    float GetWireAngle(const LArSIMpleReadoutView view) const;

    /**
     *  @brief  Set the angle of the U wires w.r.t the y-axis
     *
     *  @param  angle the angle to the y-axis
     */
    void SetWireAngleU(float angle);

    /**
     *  @brief  Set the angle of the V wires w.r.t the y-axis
     *
     *  @param  angle the angle to the y-axis
     */
    void SetWireAngleV(float angle);

    /**
     *  @brief  Set the angle of the W wires w.r.t the y-axis
     *
     *  @param  angle the angle to the y-axis
     */
    void SetWireAngleW(float angle);

    /**
     *  @brief  Get the wire pitch
     *
     *  @param  view the readout view
     *  @return the wire pitch
     */
    float GetWirePitch(const LArSIMpleReadoutView view) const;

    /**
     *  @brief  Set the pitch of the U wires
     *
     *  @param  pitch the wire pitch
     */
    void SetWirePitchU(const float pitch);

    /**
     *  @brief  Set the pitch of the V wires
     *
     *  @param  pitch the wire pitch
     */
    void SetWirePitchV(const float pitch);

    /**
     *  @brief  Set the pitch of the W wires
     *
     *  @param  pitch the wire pitch
     */
    void SetWirePitchW(const float pitch);

    /**
     *  @brief  Get the drift equivalent pitch
     *
     *  @return the drift equivalent pitch 
     */
    float GetDriftEquivalentPitch() const;

    /**
     *  @brief  Set the drift equivalent pitch
     *
     *  @param  pitch the drift equivalent pitch
     */
    void SetDriftEquivalentPitch(const float pitch);

    /**
     *  @brief  Print a summary of the detector geometry
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

    float fWorldSizeX; ///< Size of the world volume in x
    float fWorldSizeY; ///< Size of the world volume in y
    float fWorldSizeZ; ///< Size of the world volume in z

    float fLArSizeX; ///< Size of the LAr volume in x
    float fLArSizeY; ///< Size of the LAr volume in y
    float fLArSizeZ; ///< Size of the LAr volume in z

    bool fVoxeliseLAr;   ///< Whether to voxelise the LAr volume
    bool fCheckOverlaps; ///< Whether to check for geometry overlaps

    std::map<LArSIMpleReadoutView, float> fWireAngles; ///< Wire angles in degrees
    std::map<LArSIMpleReadoutView, float> fWirePitches; ///< Wire pitches in cm
    float fDriftEquivalentPitch; ///< Equivalent pitch of the drift coordinate

    G4Material *fMaterialLAr; ///< Geant4 material for air
    G4Material *fMaterialAir; ///< Geant4 material for liquid argon

    G4Box *fSolidWorld;             ///< World Geant4 solid volume
    G4LogicalVolume *fLogicWorld;   ///< World Geant4 logical volume
    G4VPhysicalVolume *fPhysiWorld; ///< World Geant4 physical volume

    G4Box *fSolidLArBox;             ///< LAr Geant4 solid volume
    G4LogicalVolume *fLogicLArBox;   ///< LAr Geant4 logical volume
    G4VPhysicalVolume *fPhysiLArBox; ///< LAr Geant4 physical volume

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

inline float LArSIMpleDetectorConstruction::GetLArVolumeMinX() const
{
    return -1.f * fLArSizeX / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArVolumeMinY() const
{
    return -1.f * fLArSizeY / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArVolumeMinZ() const
{
    return -1.f * fLArSizeZ / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArVolumeMaxX() const
{
    return fLArSizeX / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArVolumeMaxY() const
{
    return fLArSizeY / 2.f;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetLArVolumeMaxZ() const
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

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetWireAngle(const LArSIMpleReadoutView view) const
{
    return fWireAngles.at(view);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWireAngleU(const float angle)
{
    fWireAngles[LArSIMpleReadoutView::ViewU] = angle;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWireAngleV(const float angle)
{
    fWireAngles[LArSIMpleReadoutView::ViewV] = angle;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWireAngleW(const float angle)
{
    fWireAngles[LArSIMpleReadoutView::ViewW] = angle;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetWirePitch(const LArSIMpleReadoutView view) const
{
    return fWirePitches.at(view);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWirePitchU(const float pitch)
{
    fWirePitches[LArSIMpleReadoutView::ViewU] = pitch;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWirePitchV(const float pitch)
{
    fWirePitches[LArSIMpleReadoutView::ViewV] = pitch;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetWirePitchW(const float pitch)
{
    fWirePitches[LArSIMpleReadoutView::ViewW] = pitch;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleDetectorConstruction::GetDriftEquivalentPitch() const
{
    return fDriftEquivalentPitch;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleDetectorConstruction::SetDriftEquivalentPitch(const float pitch)
{
    fDriftEquivalentPitch = pitch;
}

#endif
