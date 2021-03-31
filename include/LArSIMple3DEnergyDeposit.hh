#ifndef LArSIMple3DEnergyDeposit_h
#define LArSIMple3DEnergyDeposit_h

#include <vector>
#include "G4ThreeVector.hh"

class LArSIMple3DEnergyDeposit
{
  public:
  LArSIMple3DEnergyDeposit();
  LArSIMple3DEnergyDeposit(const LArSIMple3DEnergyDeposit &rhs);
  ~LArSIMple3DEnergyDeposit();

  G4ThreeVector GetPosition() const;
  std::vector<double> GetPositionStd() const;
  double GetX() const {return fPosX;};
  double GetY() const {return fPosY;};
  double GetZ() const {return fPosZ;};
  double GetTime() const {return fTime;};
  double GetEnergy() const {return fEnergy;};
  int GetParticlePDG() const {return fParticlePDG;};
  int GetParticleTrackID() const {return fParticleTrackID;};

  void SetPositionAndTime(const double x, const double y, const double z, const double time);
  void SetPositionAndTime(const G4ThreeVector &pos, const double time);
  void SetEnergy(const double energy);
  void SetParticleInfo(const int pdg, const int trackID);

  void AddFeature(const float val) {fFeatures.push_back(val);};
  std::vector<float> GetFlatRepresentation() const;

  void PrintSummary() const;

  private:

  double fPosX;
  double fPosY;
  double fPosZ;
  double fTime;

  double fEnergy;
  std::vector<float> fFeatures;

  // Information about the particle that created the energy
  int fParticlePDG;
  int fParticleTrackID;

};

inline G4ThreeVector LArSIMple3DEnergyDeposit::GetPosition() const
{
  return G4ThreeVector(fPosX, fPosY, fPosZ);
}

inline std::vector<double> LArSIMple3DEnergyDeposit::GetPositionStd() const
{
  return std::vector<double>({fPosX, fPosY, fPosZ});
}

inline void LArSIMple3DEnergyDeposit::SetPositionAndTime(const double x, const double y, const double z, const double time)
{
  fPosX = x;
  fPosY = y;
  fPosZ = z;
  fTime = time;
}

inline void LArSIMple3DEnergyDeposit::SetPositionAndTime(const G4ThreeVector &pos, const double time)
{
  this->SetPositionAndTime(pos.x(), pos.y(), pos.z(), time);
}

inline void LArSIMple3DEnergyDeposit::SetEnergy(const double energy)
{
  fEnergy = energy;
}

inline void LArSIMple3DEnergyDeposit::SetParticleInfo(const int pdg, const int trackID)
{
  fParticlePDG = pdg;
  fParticleTrackID = trackID;
}

inline std::vector<float> LArSIMple3DEnergyDeposit::GetFlatRepresentation() const
{
  std::vector<float> flatOutput;
  // Three position coordinates
  flatOutput.push_back(fPosX);
  flatOutput.push_back(fPosY);
  flatOutput.push_back(fPosZ);
  // Charge (energy)
  flatOutput.push_back(fEnergy);
  // Now add any features
  for(const float &feature : fFeatures)
    flatOutput.push_back(feature);

  return flatOutput;
}

#endif
