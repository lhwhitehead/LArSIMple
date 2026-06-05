/**
 *  @file LArSIMple/src/LArSIMpleWireHit.cc
 * 
 *  @brief Implementation of the wire hit class.
 * 
 *  $Log: $
 */

#include <algorithm>

#include "LArSIMpleWireHit.hh"

LArSIMpleWireHit::LArSIMpleWireHit()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleWireHit::LArSIMpleWireHit(const unsigned int &wireNumber, const float &wireCoordinate,  const unsigned int &driftBin, const float &xCoordinate, const LArSIMpleReadoutView &view,
    const int &trackId, const int &pdg, const float &charge)
{
    fWireNumber = wireNumber;
    fWireCoordinate = wireCoordinate;
    fDriftBin = driftBin;
    fXCoordinate = xCoordinate;
    fReadoutView = view;
    fTrackIds.emplace_back(trackId);
    fPDGs.emplace_back(pdg);
    fCharges.emplace_back(charge);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleWireHit::LArSIMpleWireHit(const unsigned int &wireNumber, const float &wireCoordinate, const unsigned int &driftBin, const float &xCoordinate, const LArSIMpleReadoutView &view,
    const std::vector<int> &trackIds, const std::vector<int> &pdgs, const std::vector<float> &charges)
{
    fWireNumber = wireNumber;
    fWireCoordinate = wireCoordinate;
    fDriftBin = driftBin;
    fXCoordinate = xCoordinate;
    fReadoutView = view;
    fTrackIds = trackIds;
    fPDGs = pdgs;
    fCharges = charges;
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleWireHit::LArSIMpleWireHit(const LArSIMpleWireHit &rhs)
{
    fReadoutView = rhs.fReadoutView;
    fWireNumber = rhs.fWireNumber;
    fWireCoordinate = rhs.fWireCoordinate;
    fDriftBin = rhs.fDriftBin;
    fXCoordinate = rhs.fXCoordinate;
    fTrackIds = rhs.fTrackIds;
    fPDGs = rhs.fPDGs;
    fCharges = rhs.fCharges;
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleWireHit::~LArSIMpleWireHit()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

int LArSIMpleWireHit::GetLargestContributingTrackId() const
{
    const unsigned int maxIndex = std::distance(fCharges.begin(), std::max_element(fCharges.begin(), fCharges.end()));
    return fTrackIds.at(maxIndex);
}

//------------------------------------------------------------------------------------------------------------------------------------------

int LArSIMpleWireHit::GetLargestContributingPDG() const
{
    const unsigned int maxIndex = std::distance(fCharges.begin(), std::max_element(fCharges.begin(), fCharges.end()));
    return fPDGs.at(maxIndex);
}
