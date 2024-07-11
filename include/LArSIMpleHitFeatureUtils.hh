#ifndef LArSIMpleHitFeatureUtils_h
#define LArSIMpleHitFeatureUtils_h

#include "LArSIMple3DEnergyDeposit.hh"

#include <map>
#include <vector>

class LArSIMpleHitFeatureUtils
{
public:
    LArSIMpleHitFeatureUtils(const std::vector<LArSIMple3DEnergyDeposit> &hits);
    ~LArSIMpleHitFeatureUtils();

    std::vector<double> GetUVW(const double y, const double z, const double wireAngleU, const double wireAngleV, const double wireAngleW);

    std::map<unsigned int, unsigned int> GetNearestNeighbourMap() const
    {
        return fNearestNeighbourMap;
    };
    double GetAngleToNeighbours(const unsigned int idx) const;
    double GetDotProductToNeighbours(const unsigned int idx) const;

    std::map<unsigned int, std::vector<unsigned int>> GetNumberOfNeighboursWithinRadii(const std::vector<double> &rangeCuts) const;
    std::map<unsigned int, std::vector<double>> GetChargeWithinRadii(const std::vector<double> &rangeCuts) const;

private:
    void FillNeighbourMap();

    const std::vector<LArSIMple3DEnergyDeposit> &fHits;

    // Nearest neighbours, stored using the index of fHits
    std::map<unsigned int, unsigned int> fNearestNeighbourMap;
    // Second closest is useful for some features
    std::map<unsigned int, unsigned int> fSecondNeighbourMap;
};

#endif
