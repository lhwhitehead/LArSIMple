#ifndef LArSIMpleHitFeatureUtils_h
#define LArSIMpleHitFeatureUtils_h

#include "LArSIMple3DEnergyDeposit.hh"

#include <vector>
#include <map>

class LArSIMpleHitFeatureUtils
 {
  public:
  LArSIMpleHitFeatureUtils(const std::vector<LArSIMple3DEnergyDeposit> &hits);
  ~LArSIMpleHitFeatureUtils();

  std::vector<double> GetUVW(const double y, const double z);

  std::map<unsigned int, unsigned int> GetNearestNeighbourMap() const {return fNearestNeighbourMap;};
  double GetAngleToNeighbours(const unsigned int idx) const;
  double GetDotProductToNeighbours(const unsigned int idx) const;

  std::map<unsigned int, std::vector<unsigned int>> GetNumberOfNeighboursWithinRadii(const std::vector<double> &rangeCuts) const;
  std::map<unsigned int, std::vector<double>> GetChargeWithinRadii(const std::vector<double> &rangeCuts) const;

  private:

  void FillNeighbourMap();

  const std::vector<LArSIMple3DEnergyDeposit> &fHits;

  // Angles of the wire planes w.r.t y
  double fWireAngleU;
  double fWireAngleV;
  double fWireAngleW;

  // Nearest neighbours, stored using the index of fHits
  std::map<unsigned int, unsigned int> fNearestNeighbourMap;
  // Second closest is useful for some features
  std::map<unsigned int, unsigned int> fSecondNeighbourMap;
};

#endif
