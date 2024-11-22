/**
 *  @file LArSIMple/src/LArSIMpleWireConvertor.cc
 * 
 *  @brief Implementation of the wire discretisation class.
 * 
 *  $Log: $
 */

#include "LArSIMpleWireConvertor.hh"
#include "LArSIMpleWireHit.hh"

LArSIMpleWireConvertor::LArSIMpleWireConvertor() :
    fGeometrySetupComplete{false}
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

    float bin_width = 5.f;
    float min_drift{fXMin}, max_drift{fXMax};
    const unsigned int nbins_wire = std::floor((max_wire - min_wire) / bin_width);
    const unsigned int nbins_drift = std::floor((max_drift - min_drift) / bin_width);

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
            grid.insert(std::make_pair(
                wireDrift, LArSIMpleWireHit(wire, drift, view, depo.GetParticleTrackID(), depo.GetParticlePDG(), depo.GetEnergy())));
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
    const float wireAngle{view == LArSIMpleReadoutView::ViewU ? fWireAngleU : (view == LArSIMpleReadoutView::ViewV ? fWireAngleV : fWireAngleW)};
    if (wireAngle > 0)
    {
        min_wire = fZMin * std::cos(wireAngle * degreesToRadians) + fYMin * std::sin(wireAngle * degreesToRadians);
        max_wire = fZMax * std::cos(wireAngle * degreesToRadians) + fYMax * std::sin(wireAngle * degreesToRadians);
    }
    else
    {
        min_wire = fZMin * std::cos(wireAngle * degreesToRadians) + fYMax * std::sin(wireAngle * degreesToRadians);
        max_wire = fZMax * std::cos(wireAngle * degreesToRadians) + fYMin * std::sin(wireAngle * degreesToRadians);
    }
}
