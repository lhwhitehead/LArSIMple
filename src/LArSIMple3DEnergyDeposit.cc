#include "LArSIMple3DEnergyDeposit.hh"

#include "G4ThreeVector.hh"

LArSIMple3DEnergyDeposit::LArSIMple3DEnergyDeposit() :
  fPosX(0.), fPosY(0.), fPosZ(0.), fTime(0.), fEnergy(0.),
  fParticlePDG(0), fParticleTrackID(0)
{

}

LArSIMple3DEnergyDeposit::LArSIMple3DEnergyDeposit(const LArSIMple3DEnergyDeposit &rhs)
{
  this->SetPositionAndTime(rhs.GetPosition(), rhs.GetTime());
  this->SetEnergy(rhs.GetEnergy());
  this->SetParticleInfo(rhs.GetParticlePDG(), rhs.GetParticleTrackID());
}

LArSIMple3DEnergyDeposit::~LArSIMple3DEnergyDeposit()
{

}

void LArSIMple3DEnergyDeposit::PrintSummary() const
{
  std::cout << "3D Energy Deposit: " << fPosX << " " << fPosY << " " << fPosZ << " " << fTime << " " << fEnergy << " " << fParticlePDG << " " << fParticleTrackID << std::endl;

}
