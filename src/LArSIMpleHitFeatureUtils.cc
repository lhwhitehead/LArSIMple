#include "LArSIMpleHitFeatureUtils.hh"
#include "LArSIMple3DEnergyDeposit.hh"

#include "G4ThreeVector.hh"

#include <vector>
#include <map>

LArSIMpleHitFeatureUtils::LArSIMpleHitFeatureUtils(const std::vector<LArSIMple3DEnergyDeposit> &hits) :
  fHits(hits)
{
  this->FillNeighbourMap();
}

LArSIMpleHitFeatureUtils::~LArSIMpleHitFeatureUtils(){

}

void LArSIMpleHitFeatureUtils::FillNeighbourMap(){

  for(unsigned int idx1 = 0; idx1 < fHits.size(); ++idx1)
  {
    double minDist = std::numeric_limits<double>::max();
    double secondDist = std::numeric_limits<double>::max();
    unsigned int bestMatchIdx = 99999;
    unsigned int secondMatchIdx = 99999;
    for(unsigned int idx2 = 0; idx2 < fHits.size(); ++idx2)
    {
      if(idx1 == idx2)
        continue;
      const double dist = (fHits.at(idx2).GetPosition() - fHits.at(idx1).GetPosition()).mag();
      if(dist < minDist)
      {
        secondDist = minDist;
        secondMatchIdx = bestMatchIdx;
        minDist = dist;
        bestMatchIdx = idx2;
      }
      else if(dist < secondDist)
      {
        secondDist = dist;
        secondMatchIdx = idx2;
      }
    }
    fNearestNeighbourMap[idx1]=bestMatchIdx;
    fSecondNeighbourMap[idx1]=secondMatchIdx;
  }
}

double LArSIMpleHitFeatureUtils::GetAngleToNeighbours(const unsigned int idx) const
{
  const G4ThreeVector vec1 = fHits.at(fNearestNeighbourMap.at(idx)).GetPosition() - fHits.at(idx).GetPosition();
  const G4ThreeVector vec2 = fHits.at(fSecondNeighbourMap.at(idx)).GetPosition() - fHits.at(idx).GetPosition();
  return vec1.angle(vec2);
}

double LArSIMpleHitFeatureUtils::GetDotProductToNeighbours(const unsigned int idx) const
{
  const G4ThreeVector vec1 = fHits.at(fNearestNeighbourMap.at(idx)).GetPosition() - fHits.at(idx).GetPosition();
  const G4ThreeVector vec2 = fHits.at(fSecondNeighbourMap.at(idx)).GetPosition() - fHits.at(idx).GetPosition();
  return vec1.dot(vec2);
}

std::map<unsigned int, std::vector<unsigned int>> LArSIMpleHitFeatureUtils::GetNumberOfNeighboursWithinRadii(const std::vector<double> &rangeCuts) const
{
  std::map<unsigned int, std::vector<unsigned int>> result;

  for(unsigned int idx1 = 0; idx1 < fHits.size(); ++idx1)
  {
    std::vector<unsigned int> nNeighbours(rangeCuts.size());
    for(unsigned int idx2 = 0; idx2 < fHits.size(); ++idx2)
    {
      if(idx1 == idx2)
        continue;
      const double dist = (fHits.at(idx2).GetPosition() - fHits.at(idx1).GetPosition()).mag();
      for(unsigned int cut = 0; cut < rangeCuts.size(); ++cut)
      {
        if(dist < rangeCuts.at(cut))
          nNeighbours.at(cut) += 1;
      }
    }
    result[idx1] = nNeighbours;    
  }

  return result;
}

std::map<unsigned int, std::vector<double>> LArSIMpleHitFeatureUtils::GetChargeWithinRadii(const std::vector<double> &rangeCuts) const
{
  std::map<unsigned int, std::vector<double>> result;

  for(unsigned int idx1 = 0; idx1 < fHits.size(); ++idx1)
  {
    std::vector<double> charge(rangeCuts.size());
    for(unsigned int idx2 = 0; idx2 < fHits.size(); ++idx2)
    {
      if(idx1 == idx2)
        continue;
      const double dist = (fHits.at(idx2).GetPosition() - fHits.at(idx1).GetPosition()).mag();
      for(unsigned int cut = 0; cut < rangeCuts.size(); ++cut)
      {
        if(dist < rangeCuts.at(cut))
          charge.at(cut) += fHits.at(idx2).GetEnergy();
      }
    }
    result[idx1] = charge;
  }

  return result;
}

