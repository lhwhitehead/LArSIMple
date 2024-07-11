#include "LArSIMpleNestedParameterisation.hh"

#include "G4Box.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4VNestedParameterisation.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"

LArSIMpleNestedParameterisation::LArSIMpleNestedParameterisation(
    std::vector<G4Material *> material, const double sizeX, const double sizeY, const double sizeZ, const int nRepZ) :
    G4VNestedParameterisation(),
    fSizeX(sizeX),
    fSizeY(sizeY),
    fSizeZ(sizeZ),
    fNRepZ(nRepZ),
    fMaterials(material)
{
}

LArSIMpleNestedParameterisation::~LArSIMpleNestedParameterisation()
{
}

G4Material *LArSIMpleNestedParameterisation::ComputeMaterial(G4VPhysicalVolume *, const G4int, const G4VTouchable *)
{
    // We only ever have argon so in all cases we do the same
    return fMaterials[0];
}

void LArSIMpleNestedParameterisation::ComputeTransformation(const G4int repNo, G4VPhysicalVolume *currentPV) const
{
    // Get the z coordinate for this replica, starting as -ve values
    const float zPos = (-fNRepZ / 2 + repNo + 0.5) * fSizeZ;
    currentPV->SetTranslation(G4ThreeVector(0., 0., zPos));
}

void LArSIMpleNestedParameterisation::ComputeDimensions(G4Box &box, const G4int, const G4VPhysicalVolume *) const
{
    box.SetXHalfLength(0.5 * fSizeX);
    box.SetYHalfLength(0.5 * fSizeY);
    box.SetZHalfLength(0.5 * fSizeZ);
}
