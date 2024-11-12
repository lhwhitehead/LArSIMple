/**
 *  @file   LArSIMple/include/LArSIMpleNestedParameterisation
 * 
 *  @brief  Header file for the 3D energy deposit class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleNestedParameterisation_h
#define LArSIMpleNestedParameterisation_h

#include "G4Material.hh"
#include "G4VNestedParameterisation.hh"

class G4VTouchable;
class G4VPhysicalVolume;

/**
 *  @brief Class required to voxelise the LAr volume
 */

class LArSIMpleNestedParameterisation : public G4VNestedParameterisation
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  materials the vector of materials making up the voxelised volume (always LAr in this case)
     *  @param  halfSizeX the half-size of the voxels in the x direction 
     *  @param  halfSizeY the half-size of the voxels in the y direction 
     *  @param  halfSizeZ the half-size of the voxels in the z direction 
     *  @param  nRepZ the number of voxels to create in the z direction 
     */
    LArSIMpleNestedParameterisation(
        std::vector<G4Material *> materials, const double halfSizeX, const double halfSizeY, const double halfSizeZ, const int nRepZ);

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleNestedParameterisation();

    using G4VNestedParameterisation::ComputeMaterial;
    /**
     *  @brief  Method to access the material of a given physics volume replica
     *
     *  @param  currentVol the physical volume
     *  @param  repNo the replica number
     *  @param  parentTouch the touchable volume
     *
     *  @return The material of the associated physical volume replica
     */
    G4Material *ComputeMaterial(G4VPhysicalVolume *currentVol, const G4int repNo, const G4VTouchable *parentTouch = 0);

    /**
     *  @brief  Get the number of materials
     *
     *  @return The number of materials
     */
    G4int GetNumberOfMaterials() const override;

    /**
     *  @brief  Get the material for a given index in the material vector
     *
     *  @param  idx the index of the material
     */
    G4Material *GetMaterial(G4int idx) const override;

    /**
     *  @brief  Calculate the z position of the given replica
     *
     *  @param  repNo the replica number of the physical volume
     *  @param  currentPV the current physical volume
     */
    void ComputeTransformation(const G4int repNo, G4VPhysicalVolume *currentPV) const override;

    using G4VNestedParameterisation::ComputeDimensions;
    /**
     *  @brief  Calculate the dimensions of the replica
     *
     *  @param  box the bounding box around the replica volume
     *  @param  repNo the replica number of the physical volume
     *  @param  currentPV the current physical volume
     */
    void ComputeDimensions(G4Box &box, const G4int repNo, const G4VPhysicalVolume *currentPV) const override;

private:
    double fHalfSizeX;                    ///< Half size of the voxel in the x direction
    double fHalfSizeY;                    ///< Half size of the voxel in the y direction
    double fHalfSizeZ;                    ///< Half size of the voxel in the z direction
    int fNRepZ;                           ///< The number of replicas (voxels) in the z dimension
    std::vector<G4Material *> fMaterials; ///< Vector of materials, in this case just LAr
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4Material *LArSIMpleNestedParameterisation::GetMaterial(G4int idx) const
{
    if (static_cast<unsigned int>(idx) < fMaterials.size())
        return fMaterials.at(idx);
    else
        return nullptr;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4int LArSIMpleNestedParameterisation::GetNumberOfMaterials() const
{
    return fMaterials.size();
};

#endif
