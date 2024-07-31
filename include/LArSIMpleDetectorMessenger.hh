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
    LArSIMpleDetectorConstruction *fDetectorConstruction;  ///< Pointer to the DetectorConstruction class

    G4UIdirectory *fLArSIMpleDir;                          ///< Pointer to the G4 config directory class

    G4UIcmdWithADoubleAndUnit *fWorldWidth;                ///< Command for changing the world volume width
    G4UIcmdWithADoubleAndUnit *fWorldHeight;               ///< Command for changing the world volume height
    G4UIcmdWithADoubleAndUnit *fWorldLength;               ///< Command for changing the world volume length

    G4UIcmdWithADoubleAndUnit *fDetectorWidth;             ///< Command for changing the LAr volume width
    G4UIcmdWithADoubleAndUnit *fDetectorHeight;            ///< Command for changing the LAr volume height
    G4UIcmdWithADoubleAndUnit *fDetectorLength;            ///< Command for changing the LAr volume length

    G4UIcmdWithABool *fVoxeliseLAr;                        ///< Command for whether to voxelise the LAr volume
    G4UIcmdWithABool *fCheckOverlaps;                      ///< Command for whether to check geometry overlaps
};

#endif
