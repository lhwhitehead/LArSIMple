#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class LArSIMpleDetectorMessenger;

class LArSIMpleDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  LArSIMpleDetectorConstruction();
  ~LArSIMpleDetectorConstruction();
  
public:
  
  G4VPhysicalVolume* Construct();
  void UpdateGeometry();
  
public:
  
  float GetWorldWidth() const {return fWorldSizeX;}; 
  float GetWorldHeight() const {return fWorldSizeY;}; 
  float GetWorldLength() const {return fWorldSizeZ;}; 

  void SetWorldWidth(double width)   {fWorldSizeX=width;};
  void SetWorldHeight(double height) {fWorldSizeY=height;};
  void SetWorldLength(double length) {fWorldSizeZ=length;};

  float GetWorldSizeMinX() const {return -1.f * fWorldSizeX / 2.f;}; 
  float GetWorldSizeMinY() const {return -1.f * fWorldSizeY / 2.f;}; 
  float GetWorldSizeMinZ() const {return -1.f * fWorldSizeZ / 2.f;}; 

  float GetWorldSizeMaxX() const {return fWorldSizeX / 2.f;}; 
  float GetWorldSizeMaxY() const {return fWorldSizeY / 2.f;}; 
  float GetWorldSizeMaxZ() const {return fWorldSizeZ / 2.f;}; 

  float GetLArWidth() const {return fLArSizeX;};
  float GetLArHeight() const {return fLArSizeY;}; 
  float GetLArLength() const {return fLArSizeZ;}; 

  void SetLArWidth(double width)   {fLArSizeX = width;};
  void SetLArHeight(double height) {fLArSizeY = height;};
  void SetLArLength(double length) {fLArSizeZ = length;};

  float GetLArSizeMinX() const {return -1.f * fLArSizeX / 2.f;}; 
  float GetLArSizeMinY() const {return -1.f * fLArSizeY / 2.f;}; 
  float GetLArSizeMinZ() const {return -1.f * fLArSizeZ / 2.f;}; 
  
  float GetLArSizeMaxX() const {return fLArSizeX / 2.f;}; 
  float GetLArSizeMaxY() const {return fLArSizeY / 2.f;}; 
  float GetLArSizeMaxZ() const {return fLArSizeZ / 2.f;}; 

  bool GetVoxeliseLAr() const {return fVoxeliseLAr;};
  void SetVoxeliseLAr(bool flag) {fVoxeliseLAr = flag;};

  bool GetCheckOverlaps() const {return fCheckOverlaps;};
  void SetCheckOverlaps(bool check) {fCheckOverlaps = check;};

  void PrintDetectorSummary() const;
//  void ReadInFile(std::string, G4double[], G4double[], G4double);
//  void ConvertToEnergy(G4int, G4double[], G4double[], G4double);

private:

  float  fWorldSizeX;
  float  fWorldSizeY;
  float  fWorldSizeZ;

  float  fLArSizeX;
  float  fLArSizeY;
  float  fLArSizeZ;
  
  bool fVoxeliseLAr;
  bool fCheckOverlaps;

  G4Material*        fMaterialLAr;
  G4Material*        fMaterialAir;
  
  G4Box*             fSolidWorld;
  G4LogicalVolume*   fLogicWorld;
  G4VPhysicalVolume* fPhysiWorld;
  
  G4Box*             fSolidLArBox;
  G4LogicalVolume*   fLogicLArBox;
  G4VPhysicalVolume* fPhysiLArBox;

  LArSIMpleDetectorMessenger* fDetectorMessenger;  //pointer to the Messenger

//  LArSIMpleSD*              DetectorSD;   //pointer to sensitive detectors

private:

  void DefineMaterials();  
  void ConstructWorld();
  void ConstructLArBox();
  void ConstructLArVoxels();
};

#endif

