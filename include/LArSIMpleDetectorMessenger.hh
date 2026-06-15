/**
 *  @file   LArSIMple/include/LArSIMpleDetectorMessenger.hh
 * 
 *  @brief  Header file for the detector messenger class
 * 
 *  $Log: $
 */

#ifndef LArSIMpleDetectorMessenger_h
#define LArSIMpleDetectorMessenger_h

#include "G4UImessenger.hh"

class LArSIMpleDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;

/**
 *  @brief Detector messenger class for configuring the detector construction class
 */
class LArSIMpleDetectorMessenger : public G4UImessenger
{
public:
    /**
     *  @brief  constructor
     *
     *  @param  pDetector the LArSIMpleDetectorConstruction object
     */
    LArSIMpleDetectorMessenger(LArSIMpleDetectorConstruction *pDetector);

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleDetectorMessenger();

    /**
     *  @brief  Set a value in the DetectorConstruction class
     *
     *  @param  command the variable name
     *  @param  newValues the new value(s) for the variable
     */
    void SetNewValue(G4UIcommand *command, G4String newValues);

private:
    LArSIMpleDetectorConstruction *fDetectorConstruction; ///< Pointer to the DetectorConstruction class

    G4UIdirectory *fLArSIMpleDir; ///< Pointer to the G4 config directory class

    G4UIcmdWithADoubleAndUnit *fWorldWidth;  ///< Command for changing the world volume width
    G4UIcmdWithADoubleAndUnit *fWorldHeight; ///< Command for changing the world volume height
    G4UIcmdWithADoubleAndUnit *fWorldLength; ///< Command for changing the world volume length

    G4UIcmdWithADoubleAndUnit *fDetectorWidth;  ///< Command for changing the LAr volume width
    G4UIcmdWithADoubleAndUnit *fDetectorHeight; ///< Command for changing the LAr volume height
    G4UIcmdWithADoubleAndUnit *fDetectorLength; ///< Command for changing the LAr volume length

    G4UIcmdWithABool *fVoxeliseLAr;   ///< Command for whether to voxelise the LAr volume
    G4UIcmdWithABool *fCheckOverlaps; ///< Command for whether to check geometry overlaps

    G4UIcmdWithADouble *fWireAngleU; ///< Command to set the U plane wire angle in degrees
    G4UIcmdWithADouble *fWireAngleV; ///< Command to set the V plane wire angle in degrees
    G4UIcmdWithADouble *fWireAngleW; ///< Command to set the W plane wire angle in degrees

    G4UIcmdWithADouble *fWirePitchU; ///< Command to set the U plane wire pitch in cm
    G4UIcmdWithADouble *fWirePitchV; ///< Command to set the V plane wire pitch in cm
    G4UIcmdWithADouble *fWirePitchW; ///< Command to set the W plane wire pitch in cm

    G4UIcmdWithADouble *fDriftEquivalentPitch; ///< Command to set the equivalent pitch in the drift direction in cm
};

#endif
