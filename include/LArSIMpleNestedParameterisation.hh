#ifndef LArSIMpleNestedParameterisation_h
#define LArSIMpleNestedParameterisation_h

#include "G4VNestedParameterisation.hh"
#include "G4Material.hh"

class LArSIMpleNestedParameterisation : public G4VNestedParameterisation
{
  public:
  LArSIMpleNestedParameterisation(std::vector<G4Material*> material, const double sizeX, const double sizeY, const double sizeZ, const int nRepZ);
  ~LArSIMpleNestedParameterisation();

  G4Material* ComputeMaterial(G4VPhysicalVolume* currentVol,
                                      const G4int repNo, 
                                      const G4VTouchable* parentTouch=0);
    // Required method, as it is the reason for this class.
    // Must cope with parentTouch=0 for navigator's SetupHierarchy.

  G4int       GetNumberOfMaterials() const {return fMaterials.size();};
  G4Material* GetMaterial(G4int idx) const;
    // Needed to define materials for instances of Nested Parameterisation 
    // Current convention: each call should return the materials 
    // of all instances with the same mother/ancestor volume.

  void ComputeTransformation(const G4int no, G4VPhysicalVolume* currentPV) const;

  void ComputeDimensions(G4Box &, const G4int, const G4VPhysicalVolume *) const;

  private:

  double fSizeX;
  double fSizeY;
  double fSizeZ;
  int fNRepZ;
  std::vector<G4Material*> fMaterials;

};

inline G4Material* LArSIMpleNestedParameterisation::GetMaterial(G4int idx) const
{
  if(idx < fMaterials.size())
    return fMaterials.at(idx);
  else
    return nullptr;
}

#endif
