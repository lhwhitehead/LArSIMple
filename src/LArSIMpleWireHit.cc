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

LArSIMpleWireHit::LArSIMpleWireHit(const float &wireNumber, const float &driftBin, const LArSIMpleReadoutView &view,
    const int &trackId, const int &pdg, const float &charge)
{
    fWireNumber = wireNumber;
    fDriftBin = driftBin;
    fReadoutView = view;
    fTrackIds.emplace_back(trackId);
    fPDGs.emplace_back(pdg);
    fCharges.emplace_back(charge);
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleWireHit::LArSIMpleWireHit(const float &wireNumber, const float &driftBin, const LArSIMpleReadoutView &view,
    const std::vector<int> &trackIds, const std::vector<int> &pdgs, const std::vector<float> &charges)
{
    fWireNumber = wireNumber;
    fDriftBin = driftBin;
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
    fDriftBin = rhs.fDriftBin;
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
