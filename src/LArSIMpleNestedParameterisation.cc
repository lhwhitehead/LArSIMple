/**
 *  @file LArSIMple/src/LArSIMpleNestedParameterisation.cc
 * 
 *  @brief Implementation of the nested parameterisation class.
 * 
 *  $Log: $
 */

#include "LArSIMpleNestedParameterisation.hh"

#include "G4Box.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4VNestedParameterisation.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"

LArSIMpleNestedParameterisation::LArSIMpleNestedParameterisation(
    std::vector<G4Material *> materials, const double halfSizeX, const double halfSizeY, const double halfSizeZ, const int nRepZ) :
    G4VNestedParameterisation(),
    fHalfSizeX(halfSizeX),
    fHalfSizeY(halfSizeY),
    fHalfSizeZ(halfSizeZ),
    fNRepZ(nRepZ),
    fMaterials(materials)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleNestedParameterisation::~LArSIMpleNestedParameterisation()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

G4Material *LArSIMpleNestedParameterisation::ComputeMaterial(G4VPhysicalVolume *, const G4int, const G4VTouchable *)
{
    // We only ever have argon so in all cases we do the same
    return fMaterials[0];
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleNestedParameterisation::ComputeTransformation(const G4int repNo, G4VPhysicalVolume *currentPV) const
{
    // Get the z coordinate for this replica, starting as -ve values
    const float zPos = (-fNRepZ / 2 + repNo + 0.5) * 2.0 * fHalfSizeZ;
    currentPV->SetTranslation(G4ThreeVector(0., 0., zPos));
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleNestedParameterisation::ComputeDimensions(G4Box &box, const G4int, const G4VPhysicalVolume *) const
{
    // All replicas have the same size
    box.SetXHalfLength(fHalfSizeX);
    box.SetYHalfLength(fHalfSizeY);
    box.SetZHalfLength(fHalfSizeZ);
}
