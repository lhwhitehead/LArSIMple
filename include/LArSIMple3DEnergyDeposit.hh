/**
 *  @file   LArSIMple/include/LArSIMple3DEnergyDeposit.hh
 * 
 *  @brief  Header file for the 3D energy deposit class.
 * 
 *  $Log: $
 */

#ifndef LArSIMple3DEnergyDeposit_h
#define LArSIMple3DEnergyDeposit_h

#include "G4ThreeVector.hh"
#include <vector>

/**
 *  @brief Class representing a true 3D energy deposit
 */
class LArSIMple3DEnergyDeposit
{
public:
    /**
     *  @brief  Default constructor
     */
    LArSIMple3DEnergyDeposit();

    /**
     *  @brief  Copy constructor
     *
     *  @param  rhs LArSIMple3DEnergyDeposit object to copy
     */
    LArSIMple3DEnergyDeposit(const LArSIMple3DEnergyDeposit &rhs);

    /**
     *  @brief  Destructor
     */
    ~LArSIMple3DEnergyDeposit();

    /**
     *  @brief  Get the position vector of the energy deposit
     *
     *  @return The xyz position of the energy deposit
     */
    G4ThreeVector GetPosition() const;

    /**
     *  @brief  Get the position vector of the energy deposit in U, V and W readout planes
     *
     *  @return The UVW position of the energy deposit
     */
    G4ThreeVector GetUVWPosition() const;

    /**
     *  @brief  Get the time of the energy deposit
     *
     *  @return The time of the energy deposit
     */
    double GetTime() const;

    /**
     *  @brief  Get the deposited energy
     *
     *  @return The deposited energy
     */
    double GetEnergy() const;

    /**
     *  @brief  Get the deposited energy per unit length
     *
     *  @return The dE/dx
     */
    double GetDeDx() const;

    /**
     *  @brief  Get the true particle code according to the PDG
     *
     *  @return The PDG code
     */
    int GetParticlePDG() const;

    /**
     *  @brief  Get the Geant4 id of the true particle that deposited the energy
     *
     *  @return The Geant4 track id
     */
    int GetParticleTrackID() const;

    /**
     *  @brief  Get the process that caused the energy deposition
     *
     *  @return The process as an integer
     */
    int GetParticleProcess() const;

    /**
     *  @brief  Set position and time using (x,y,z,t)
     *
     *  @param  x the x position coordinate
     *  @param  y the y position coordinate
     *  @param  z the z position coordinate
     *  @param  t the time of the energy deposit
     */
    void SetPositionAndTime(const double x, const double y, const double z, const double time);

    /**
     *  @brief  Set position and time using (x,y,z,t)
     *
     *  @param  pos the 3D position
     *  @param  t the time of the energy deposit
     */
    void SetPositionAndTime(const G4ThreeVector &pos, const double time);

    /**
     *  @brief  Calculate the U, V and W positions of the energy deposit
     *
     *  @param  wireAngleU the angle of the U plane to the z axis
     *  @param  wireAngleV the angle of the V plane to the z axis
     *  @param  wireAngleW the angle of the W plane to the z axis
     */
    void CalculateUVW(const double wireAngleU, const double wireAngleV, const double wireAngleW);

    /**
     *  @brief  Set the energy of the deposit
     *
     *  @param  energy the deposited energy
     */
    void SetEnergy(const double energy);

    /**
     *  @brief  Set the energy per unit length of the deposit
     *
     *  @param  dedx the dE/dx
     */
    void SetDeDx(const double dedx);

    /**
     *  @brief  Set the true particle information
     *
     *  @param  energy the deposited energy
     */
    void SetParticleInfo(const int pdg, const int trackID, const int processCode);

    /**
     *  @brief  Add a feature to the energy deposit, such as number of neighbours
     *
     *  @param  val the value of the feature to add
     */
    void AddFeature(const float val);

    /**
     *  @brief  Get the vector of features associated to the energy deposit
     *
     *  @return The vector of feature values
     */
    std::vector<float> GetFeatures() const;

    /**
     *  @brief  Get the flat vector representation of the energy deposit
     *
     *  @return The vector of position, time, energy, dE/dx, features and truth information values
     */
    std::vector<float> GetFlatRepresentation() const;

    /**
     *  @brief  Get the number of position coordinates
     *
     *  @return The number of position coordinates
     */
    unsigned int GetNCoordinates() const;

    /**
     *  @brief  Get the number of wire position coordinates
     *
     *  @return The number of wire position coordinates
     */
    unsigned int GetNWireCoordinates() const;

    /**
     *  @brief  Get the number of features
     *
     *  @return The number of features
     */
    unsigned int GetNFeatures() const;

    /**
     *  @brief  Get the number of bits of truth information
     *
     *  @return The number of truth information values
     */
    unsigned int GetNTruths() const;

    /**
     *  @brief  Print a summary of the class to the screen
     */
    void PrintSummary() const;

private:
    G4ThreeVector fPosition;      ///< 3D position of the energy deposit
    double fTime;                 ///< Time of the energy deposit

    double fPosU;                 ///< Position in the U wire plane projection 
    double fPosV;                 ///< Position in the V wire plane projection
    double fPosW;                 ///< Position in the W wire plane projection 

    double fEnergy;               ///< The energy deposited
    double fDeDx;                 ///< The dE/dx

    int fParticlePDG;             ///< The true particle type that produced the energy deposit
    int fParticleTrackID;         ///< The Geant4 track id of the particle that produced the energy deposit
    int fParticleProcess;         ///< The process by which the energy was deposited

    std::vector<float> fFeatures; ///< The vector of the associated features
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMple3DEnergyDeposit::GetPosition() const
{
    return fPosition;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline G4ThreeVector LArSIMple3DEnergyDeposit::GetUVWPosition() const
{
    return G4ThreeVector(fPosU, fPosV, fPosW);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMple3DEnergyDeposit::GetTime() const
{
    return fTime;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMple3DEnergyDeposit::GetEnergy() const
{
    return fEnergy;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMple3DEnergyDeposit::GetDeDx() const
{
    return fDeDx;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMple3DEnergyDeposit::GetParticlePDG() const
{
    return fParticlePDG;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMple3DEnergyDeposit::GetParticleTrackID() const
{
    return fParticleTrackID;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMple3DEnergyDeposit::GetParticleProcess() const
{
    return fParticleProcess;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMple3DEnergyDeposit::SetPositionAndTime(const double x, const double y, const double z, const double time)
{
    return this->SetPositionAndTime(G4ThreeVector(x, y, z), time);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMple3DEnergyDeposit::SetPositionAndTime(const G4ThreeVector &pos, const double time)
{
    fPosition = pos;
    fTime = time;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMple3DEnergyDeposit::SetEnergy(const double energy)
{
    fEnergy = energy;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMple3DEnergyDeposit::SetDeDx(const double dedx)
{
    fDeDx = dedx;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMple3DEnergyDeposit::SetParticleInfo(const int pdg, const int trackID, const int processCode)
{
    fParticlePDG = pdg;
    fParticleTrackID = trackID;
    fParticleProcess = processCode;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMple3DEnergyDeposit::AddFeature(const float val)
{
    fFeatures.emplace_back(val);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::vector<float> LArSIMple3DEnergyDeposit::GetFeatures() const
{
    return fFeatures;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMple3DEnergyDeposit::GetNCoordinates() const
{
    return 3;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMple3DEnergyDeposit::GetNWireCoordinates() const
{
    return 3;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMple3DEnergyDeposit::GetNFeatures() const
{
    return fFeatures.size();
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMple3DEnergyDeposit::GetNTruths() const
{
    return 3;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline std::vector<float> LArSIMple3DEnergyDeposit::GetFlatRepresentation() const
{
    std::vector<float> flatOutput;
    // Three position coordinates
    flatOutput.emplace_back(fPosition.getX());
    flatOutput.emplace_back(fPosition.getY());
    flatOutput.emplace_back(fPosition.getZ());
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
