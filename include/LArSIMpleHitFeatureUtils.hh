/**
 *  @file   LArSIMple/include/LArSIMpleHitFeatureUtils.hh
 * 
 *  @brief  Header file for the hit feature utility class
 * 
 *  $Log: $
 */

#ifndef LArSIMpleHitFeatureUtils_h
#define LArSIMpleHitFeatureUtils_h

#include "LArSIMple3DEnergyDeposit.hh"

#include <map>
#include <vector>

/**
 *  @brief Class for calculating hit features
 */
class LArSIMpleHitFeatureUtils
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  hits the collection of energy deposits
     */
    LArSIMpleHitFeatureUtils(const std::vector<LArSIMple3DEnergyDeposit> &hits);

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleHitFeatureUtils();

    /**
     *  @brief  Get the angle between the directions to neighbouring energy deposits
     *
     *  @return The angle between directions to neighbouring energy deposits
     */
    double GetAngleToNeighbours(const unsigned int idx) const;

    /**
     *  @brief  Get the dot product between the directions to neighbouring energy deposits
     *
     *  @return The dot product between directions to neighbouring energy deposits
     */
    double GetDotProductToNeighbours(const unsigned int idx) const;

    /**
     *  @brief  Get the map of the number of neighbours within given distances of each hit
     *
     *  @return The map of the number of neighbours within given distances of each hit
     */
    std::map<unsigned int, std::vector<unsigned int>> GetNumberOfNeighboursWithinRadii(const std::vector<double> &rangeCuts) const;

    /**
     *  @brief  Get the map of the charge deposited within given distances of each hit
     *
     *  @return The map of the charge deposited within given distances of each hit
     */
    std::map<unsigned int, std::vector<double>> GetChargeWithinRadii(const std::vector<double> &rangeCuts) const;

private:
    /**
     *  @brief  Fill the map of nearest and second nearest neighbours for each hit
     */
    void FillNeighbourMap();

    const std::vector<LArSIMple3DEnergyDeposit> &fHits;         ///< Vector of hits
    std::map<unsigned int, unsigned int> fNearestNeighbourMap;  ///< Map of the nearest neighbour hit index to a given hit index
    std::map<unsigned int, unsigned int> fSecondNeighbourMap;   ///< Map of the second nearest neighbour hit index to a given hit index
};

#endif
