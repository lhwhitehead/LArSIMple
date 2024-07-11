#ifndef LArSIMpleTrueParticle_h
#define LArSIMpleTrueParticle_h

#include "G4ThreeVector.hh"

class LArSIMpleTrueParticle
{
public:
    LArSIMpleTrueParticle();
    LArSIMpleTrueParticle(const G4ThreeVector &pos, const G4ThreeVector &dir, const double energy, const int pdg);
    ~LArSIMpleTrueParticle();

    G4ThreeVector GetPosition() const
    {
        return fPosition;
    };
    G4ThreeVector GetDirection() const
    {
        return fDirection;
    };
    double GetEnergy() const
    {
        return fEnergy;
    };
    int GetPDGCode() const
    {
        return fPDGCode;
    };

    double GetX() const
    {
        return fPosition.x();
    };
    double GetY() const
    {
        return fPosition.y();
    };
    double GetZ() const
    {
        return fPosition.z();
    };

    double GetDirX() const
    {
        return fDirection.x();
    };
    double GetDirY() const
    {
        return fDirection.y();
    };
    double GetDirZ() const
    {
        return fDirection.z();
    };

private:
    G4ThreeVector fPosition;
    G4ThreeVector fDirection;
    double fEnergy;
    int fPDGCode;
};

#endif
