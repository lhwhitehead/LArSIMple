/**
 *  @file   LArSIMple/include/LArSIMpleWireHit.hh
 * 
 *  @brief  Header file for the wire hit class
 * 
 *  $Log: $
 */

#ifndef LArSIMpleWireHit_h
#define LArSIMpleWireHit_h

#include <map>
#include <numeric>
#include <vector>

enum class LArSIMpleReadoutView
{
    ViewU = 0,
    ViewV = 1,
    ViewW = 2
};

/**
 *  @brief Class to represent wire hits
 */
class LArSIMpleWireHit
{
public:
    /**
     *  @brief  Constructor
     */
    LArSIMpleWireHit();

    /**
     *  @brief  Constructor
     *
     *  @param  wireNumber the number of the wire that the hit would have been registered on
     *  @param  driftBin the value of the discretised drift direction
     *  @param  view the readout view in question (U, V or W)
     *  @param  trackId the contributing track id to this hit
     *  @param  pdg the contributing pdg code to this hit 
     *  @param  charge the contributing energy deposit to this hit
     */
    LArSIMpleWireHit(const unsigned int wireNumber, const float wireCoordinate, const unsigned int driftBin, const float driftCoordinate,
        const LArSIMpleReadoutView view, const int trackId, const int pdg, const float charge);

    /**
     *  @brief  Constructor
     *
     *  @param  wireNumber the number of the wire that the hit would have been registered on
     *  @param  driftBin the value of the discretised drift direction
     *  @param  view the readout view in question (U, V or W)
     *  @param  trackIds the vector of contributing track ids to this hit 
     *  @param  pdgs the vector of contributing pdg codes to this hit 
     *  @param  charges the vector of contributing energy deposits to this hit
     */
    LArSIMpleWireHit(const unsigned int wireNumber, const float wireCoordinate, const unsigned int driftBin, const float driftCoordinate,
        const LArSIMpleReadoutView view, const std::vector<int> &trackIds, const std::vector<int> &pdgs, const std::vector<float> &charges);

    /**
     *  @brief  Copy constructor
     */
    LArSIMpleWireHit(const LArSIMpleWireHit &rhs);

    /**
     *  @brief  Destructor
     */
    virtual ~LArSIMpleWireHit();

    /**
     *  @brief  Returns the wire number of this hit
     *
     *  @return the wire number
     */
    unsigned int GetWireNumber() const;

    /**
     *  @brief  Returns the drift bin of this hit
     *
     *  @return the drift bin
     */
    unsigned int GetDriftBin() const;

    /**
     *  @brief  Returns the wire coordinate of this hit
     *
     *  @return the wire coordinate
     */
    float GetWireCoordinate() const;

    /**
     *  @brief  Returns the x coordinate of this hit
     *
     *  @return the x coordinate
     */
    float GetDriftCoordinate() const;

    /**
     *  @brief  Returns the readout view number of this hit
     *
     *  @return the readout view
     */
    LArSIMpleReadoutView GetView() const;

    /**
     *  @brief  Returns the G4 track id of the largest contributor to this hit
     *
     *  @return the G4 track id
     */
    int GetLargestContributingTrackId() const;

    /**
     *  @brief  Returns the PDG code of the largest contributor to this hit
     *
     *  @return the PDG code
     */
    int GetLargestContributingPDG() const;

    /**
     *  @brief  Returns the charge of this hit
     *
     *  @return the charge
     */
    float GetCharge() const;

    /**
     *  @brief  Add a particle's energy deposit to this hit
     *
     *  @param  trackId the G4 track id of the particle
     *  @param  pdg the PDG code of the particle
     *  @param  charge the amount of deposited energy
     */
    void AddHitContribution(const int &trackId, const int &pdg, const float &charge);

private:
    LArSIMpleReadoutView fReadoutView; ///< The readout plane
    unsigned int fWireNumber;             ///< The wire coordinate that the hit would have been registered on
    float fWireCoordinate;             ///< The wire coordinate that the hit would have been registered on
    unsigned int fDriftBin;            ///< The drift coordinate after discretisation
    float fDriftCoordinate;            ///< The drift coordinate after discretisation
    std::vector<int> fTrackIds;        ///< The track ids responsible for this hit
    std::vector<int> fPDGs;            ///< The PDG codes responsible for this hit
    std::vector<float> fCharges;       ///< The charge contributions to this hit
};

typedef std::map<std::pair<unsigned int, unsigned int>, LArSIMpleWireHit> WireDriftGrid;

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleWireHit::GetWireNumber() const
{
    return fWireNumber;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleWireHit::GetWireCoordinate() const
{
    return fWireCoordinate;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline unsigned int LArSIMpleWireHit::GetDriftBin() const
{
    return fDriftBin;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleWireHit::GetDriftCoordinate() const
{
    return fDriftCoordinate;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline LArSIMpleReadoutView LArSIMpleWireHit::GetView() const
{
    return fReadoutView;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline float LArSIMpleWireHit::GetCharge() const
{
    return std::accumulate(fCharges.begin(), fCharges.end(), 0.f);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleWireHit::AddHitContribution(const int &trackId, const int &pdg, const float &charge)
{
    fTrackIds.emplace_back(trackId);
    fPDGs.emplace_back(pdg);
    fCharges.emplace_back(charge);
}

#endif
