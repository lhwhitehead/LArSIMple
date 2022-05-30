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
  double GetU() const {return fPosU;};
  double GetV() const {return fPosV;};
  double GetW() const {return fPosW;};
  double GetTime() const {return fTime;};
  double GetEnergy() const {return fEnergy;};
  double GetDeDx() const {return fDeDx;};
  int GetParticlePDG() const {return fParticlePDG;};
  int GetParticleTrackID() const {return fParticleTrackID;};
  int GetParticleProcess() const {return fParticleProcess;};

  void SetPositionAndTime(const double x, const double y, const double z, const double time);
  void SetPositionAndTime(const G4ThreeVector &pos, const double time);
  void SetUVW(const std::vector<double> &uvw);
  void SetEnergy(const double energy);
  void SetDeDx(const double dedx);
  void SetParticleInfo(const int pdg, const int trackID, const int processCode);

  void AddFeature(const float val) {fFeatures.push_back(val);};
  std::vector<float> GetFeatures() const {return fFeatures;};

  std::vector<float> GetFlatRepresentation() const;

  unsigned int GetNCoordinates() const {return 3;}; // Hardcoded for now
  unsigned int GetNFeatures() const {return fFeatures.size();};
  unsigned int GetNTruths() const {return 2;}; // Hardcoded for now
  void PrintSummary() const;

  private:

  double fPosX;
  double fPosY;
  double fPosZ;
  double fTime;

  // Wire planes
  double fPosU;
  double fPosV;
  double fPosW;

  double fEnergy;
  double fDeDx;

  // Information about the particle that created the energy
  int fParticlePDG;
  int fParticleTrackID;
  int fParticleProcess;

  std::vector<float> fFeatures;
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
  // Add the energy as the first feature... we should also allow for calibration later
  fFeatures.push_back(fEnergy);
}

inline void LArSIMple3DEnergyDeposit::SetDeDx(const double dedx)
{
  fDeDx = dedx;
  fFeatures.push_back(fDeDx);
}

inline void LArSIMple3DEnergyDeposit::SetParticleInfo(const int pdg, const int trackID, const int processCode)
{
  fParticlePDG = pdg;
  fParticleTrackID = trackID;
  fParticleProcess = processCode;
}

inline std::vector<float> LArSIMple3DEnergyDeposit::GetFlatRepresentation() const
{
  std::vector<float> flatOutput;
  // Three position coordinates
  flatOutput.push_back(fPosX);
  flatOutput.push_back(fPosY);
  flatOutput.push_back(fPosZ);
  // Now for the projections
  flatOutput.push_back(fPosU);
  flatOutput.push_back(fPosV);
  flatOutput.push_back(fPosW);
  // Now add any features
  for(const float &feature : fFeatures)
    flatOutput.push_back(feature);
  // Truth information
  flatOutput.push_back(fParticlePDG);
  flatOutput.push_back(fParticleTrackID);
  flatOutput.push_back(fParticleProcess);
  return flatOutput;
}

#endif
