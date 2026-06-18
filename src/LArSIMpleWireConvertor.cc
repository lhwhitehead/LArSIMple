/**
 *  @file LArSIMple/src/LArSIMpleWireConvertor.cc
 * 
 *  @brief Implementation of the wire discretisation class.
 * 
 *  $Log: $
 */

#include "LArSIMpleWireConvertor.hh"
#include "LArSIMpleWireHit.hh"

LArSIMpleWireConvertor::LArSIMpleWireConvertor(const LArSIMpleDetectorConstruction *const detector) :
    fDetector(detector)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleWireConvertor::~LArSIMpleWireConvertor()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

std::vector<LArSIMpleWireHit> LArSIMpleWireConvertor::Convert3DEnergyDepositsToWireHits(
    const std::vector<LArSIMple3DEnergyDeposit> &energyDeposits3D, const LArSIMpleReadoutView &view) const
{
    WireDriftGrid grid; // Grid of (w, x) points each containing a vector of LArSIMpleWireHit objects

    float min_wire, max_wire;
    GetWireGeometryInfo(view, min_wire, max_wire);

    const float wire_bin_width{fDetector->GetWirePitch(view)};
    const float drift_bin_width{fDetector->GetDriftEquivalentPitch()};
    const float min_drift{fDetector->GetLArVolumeMinX()}, max_drift{fDetector->GetLArVolumeMaxX()};
    const unsigned int nbins_wire = std::floor((max_wire - min_wire) / wire_bin_width);
    const unsigned int nbins_drift = std::floor((max_drift - min_drift) / drift_bin_width);

    // Add hits into a 2D grid in (wire, drift) coordinates
    for (const LArSIMple3DEnergyDeposit &depo : energyDeposits3D)
    {
        const G4ThreeVector uvw{depo.GetUVWPosition()};
        const double wirePos{view == LArSIMpleReadoutView::ViewU ? uvw.getX() : (view == LArSIMpleReadoutView::ViewV ? uvw.getY() : uvw.getZ())};
        const unsigned int wire = ((wirePos - min_wire) / (max_wire - min_wire)) * nbins_wire;
        const unsigned int drift = ((depo.GetPosition().getX() - min_drift) / (max_drift - min_drift)) * nbins_drift;

        std::pair<unsigned int, unsigned int> wireDrift{wire, drift};
        if (grid.count(wireDrift))
            grid.at(wireDrift).AddHitContribution(depo.GetParticleTrackID(), depo.GetParticlePDG(), depo.GetEnergy());
        else
        {
            // When creating the hit convert back to wire and x to remove binning
            const float w{min_wire + wire * wire_bin_width};
            const float x{min_drift + drift * drift_bin_width};
            grid.insert(std::make_pair(
                wireDrift, LArSIMpleWireHit(wire, w, drift, x, view, depo.GetParticleTrackID(), depo.GetParticlePDG(), depo.GetEnergy())));
        }
    }

    // Convert to a vector of LArSIMpleWireHits
    std::vector<LArSIMpleWireHit> viewHits;
    for (auto const &gridPair : grid)
        viewHits.emplace_back(gridPair.second);

    return viewHits;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleWireConvertor::GetWireGeometryInfo(const LArSIMpleReadoutView &view, float &min_wire, float &max_wire) const
{
    const float degreesToRadians{3.14159265358979323846 / 180.};
    const float wireAngle{view == LArSIMpleReadoutView::ViewU ? fDetector->GetWireAngle(LArSIMpleReadoutView::ViewU) : 
        (view == LArSIMpleReadoutView::ViewV ? fDetector->GetWireAngle(LArSIMpleReadoutView::ViewV) : fDetector->GetWireAngle(LArSIMpleReadoutView::ViewW))};
    if (wireAngle > 0)
    {
        min_wire = fDetector->GetLArVolumeMinZ() * std::cos(wireAngle * degreesToRadians) + fDetector->GetLArVolumeMinY() * std::sin(wireAngle * degreesToRadians);
        max_wire = fDetector->GetLArVolumeMaxZ() * std::cos(wireAngle * degreesToRadians) + fDetector->GetLArVolumeMaxY() * std::sin(wireAngle * degreesToRadians);
    }
    else
    {
        min_wire = fDetector->GetLArVolumeMinZ() * std::cos(wireAngle * degreesToRadians) + fDetector->GetLArVolumeMinY() * std::sin(wireAngle * degreesToRadians);
        max_wire = fDetector->GetLArVolumeMaxZ() * std::cos(wireAngle * degreesToRadians) + fDetector->GetLArVolumeMaxY() * std::sin(wireAngle * degreesToRadians);
    }
}
