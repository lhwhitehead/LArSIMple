#include "LArSIMple3DEnergyDeposit.hh"

#include "G4ThreeVector.hh"

LArSIMple3DEnergyDeposit::LArSIMple3DEnergyDeposit() :
    fPosX(0.),
    fPosY(0.),
    fPosZ(0.),
    fTime(0.),
    fPosU(0.),
    fPosV(0.),
    fPosW(0.),
    fEnergy(0.),
    fDeDx(0.),
    fParticlePDG(0),
    fParticleTrackID(0),
    fParticleProcess(0)
{
}

LArSIMple3DEnergyDeposit::LArSIMple3DEnergyDeposit(const LArSIMple3DEnergyDeposit &rhs)
{
    this->SetPositionAndTime(rhs.GetPosition(), rhs.GetTime());
    this->SetEnergy(rhs.GetEnergy());
    this->SetDeDx(rhs.GetDeDx());
    this->SetParticleInfo(rhs.GetParticlePDG(), rhs.GetParticleTrackID(), rhs.GetParticleProcess());
}

LArSIMple3DEnergyDeposit::~LArSIMple3DEnergyDeposit()
{
}

void LArSIMple3DEnergyDeposit::CalculateUVW(const double wireAngleU, const double wireAngleV, const double wireAngleW)
{
    const double degreesToRadians{3.14159265358979323846 / 180.};
    fPosU = fPosZ * std::cos(wireAngleU * degreesToRadians) - fPosY * std::sin(wireAngleU * degreesToRadians);
    fPosV = fPosZ * std::cos(wireAngleV * degreesToRadians) - fPosY * std::sin(wireAngleV * degreesToRadians);
    fPosW = fPosZ * std::cos(wireAngleW * degreesToRadians) - fPosY * std::sin(wireAngleW * degreesToRadians);
}

void LArSIMple3DEnergyDeposit::PrintSummary() const
{
    std::cout << "3D Energy Deposit: " << fPosX << " " << fPosY << " " << fPosZ << " " << fTime << " " << fEnergy << " " << fParticlePDG
              << " " << fParticleTrackID << std::endl;
}
