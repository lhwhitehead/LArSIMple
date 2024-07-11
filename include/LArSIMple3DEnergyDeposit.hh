#ifndef LArSIMple3DEnergyDeposit_h
#define LArSIMple3DEnergyDeposit_h

#include "G4ThreeVector.hh"
#include <vector>

class LArSIMple3DEnergyDeposit
{
public:
    LArSIMple3DEnergyDeposit();
    LArSIMple3DEnergyDeposit(const LArSIMple3DEnergyDeposit &rhs);
    ~LArSIMple3DEnergyDeposit();

    G4ThreeVector GetPosition() const;
    G4ThreeVector GetUVWPosition() const;
    double GetTime() const
    {
        return fTime;
    };
    double GetEnergy() const
    {
        return fEnergy;
    };
    double GetDeDx() const
    {
        return fDeDx;
    };
    int GetParticlePDG() const
    {
        return fParticlePDG;
    };
    int GetParticleTrackID() const
    {
        return fParticleTrackID;
    };
    int GetParticleProcess() const
    {
        return fParticleProcess;
    };

    void SetPositionAndTime(const double x, const double y, const double z, const double time);
    void SetPositionAndTime(const G4ThreeVector &pos, const double time);
    void CalculateUVW(const double wireAngleU, const double wireAngleV, const double wireAngleW);
    void SetEnergy(const double energy);
    void SetDeDx(const double dedx);
    void SetParticleInfo(const int pdg, const int trackID, const int processCode);

    void AddFeature(const float val)
    {
        fFeatures.emplace_back(val);
    };
    std::vector<float> GetFeatures() const
    {
        return fFeatures;
    };

    std::vector<float> GetFlatRepresentation() const;

    unsigned int GetNCoordinates() const
    {
        return 3;
    }; // Hardcoded for now
    unsigned int GetNFeatures() const
    {
        return fFeatures.size();
    };
    unsigned int GetNTruths() const
    {
        return 2;
    }; // Hardcoded for now
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

inline G4ThreeVector LArSIMple3DEnergyDeposit::GetUVWPosition() const
{
    return G4ThreeVector(fPosU, fPosV, fPosW);
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
    fFeatures.emplace_back(fEnergy);
}

inline void LArSIMple3DEnergyDeposit::SetDeDx(const double dedx)
{
    fDeDx = dedx;
    fFeatures.emplace_back(fDeDx);
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
    flatOutput.emplace_back(fPosX);
    flatOutput.emplace_back(fPosY);
    flatOutput.emplace_back(fPosZ);
    // Now for the projections
    flatOutput.emplace_back(fPosU);
    flatOutput.emplace_back(fPosV);
    flatOutput.emplace_back(fPosW);
    // Now add any features
    for (const float &feature : fFeatures)
        flatOutput.emplace_back(feature);
    // Truth information
    flatOutput.emplace_back(fParticlePDG);
    flatOutput.emplace_back(fParticleTrackID);
    flatOutput.emplace_back(fParticleProcess);
    return flatOutput;
}

#endif
