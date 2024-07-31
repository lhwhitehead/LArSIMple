/**
 *  @file LArSIMple/src/LArSIMple3DEnergyDeposit.cc
 * 
 *  @brief Implementation of the 3D energy deposit class.
 * 
 *  $Log: $
 */

#include "LArSIMple3DEnergyDeposit.hh"

#include "G4ThreeVector.hh"

LArSIMple3DEnergyDeposit::LArSIMple3DEnergyDeposit() :
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

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMple3DEnergyDeposit::LArSIMple3DEnergyDeposit(const LArSIMple3DEnergyDeposit &rhs)
{
    this->SetPositionAndTime(rhs.GetPosition(), rhs.GetTime());
    this->SetEnergy(rhs.GetEnergy());
    this->SetDeDx(rhs.GetDeDx());
    this->SetParticleInfo(rhs.GetParticlePDG(), rhs.GetParticleTrackID(), rhs.GetParticleProcess());
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMple3DEnergyDeposit::~LArSIMple3DEnergyDeposit()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMple3DEnergyDeposit::CalculateUVW(const double wireAngleU, const double wireAngleV, const double wireAngleW)
{
    const double degreesToRadians{3.14159265358979323846 / 180.};
    const double posY{fPosition.getY()};
    const double posZ{fPosition.getZ()};
    fPosU = posZ * std::cos(wireAngleU * degreesToRadians) - posY * std::sin(wireAngleU * degreesToRadians);
    fPosV = posZ * std::cos(wireAngleV * degreesToRadians) - posY * std::sin(wireAngleV * degreesToRadians);
    fPosW = posZ * std::cos(wireAngleW * degreesToRadians) - posY * std::sin(wireAngleW * degreesToRadians);
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMple3DEnergyDeposit::PrintSummary() const
{
    std::cout << "3D Energy Deposit: " << fPosition.getX() << " " << fPosition.getY() << " " << fPosition.getZ()
              << " " << fTime << " " << fEnergy << " " << fParticlePDG
              << " " << fParticleTrackID << std::endl;
}
