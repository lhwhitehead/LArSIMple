/**
 *  @file   LArSIMple/include/LArSIMpleWireConvertor.hh
 * 
 *  @brief  Header file for the wire discretisation class
 * 
 *  $Log: $
 */

#ifndef LArSIMpleWireConvertor_h
#define LArSIMpleWireConvertor_h

#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleWireHit.hh"
#include <map>
#include <vector>

/**
 *  @brief Class that converts 3D energy deposits into discrete wire plane hits
 */
class LArSIMpleWireConvertor
{
public:
    /**
     *  @brief  Destructor
     */
    virtual ~LArSIMpleWireConvertor();

    /**
     *  @brief  Get the static instance to the class
     *
     *  @return The instance of the wire convertor
     */
    static LArSIMpleWireConvertor &Get(const LArSIMpleDetectorConstruction *const detector)
    {
        static LArSIMpleWireConvertor wireConvertor(detector);
        return wireConvertor;
    };
    LArSIMpleWireConvertor(LArSIMpleWireConvertor const &) = delete;
    LArSIMpleWireConvertor &operator=(LArSIMpleWireConvertor const &) = delete;

    /** 
     *  @brief  Convert 3D energy deposits to wire plane hits
     *
     *  @param  energyDeposits3D the vector of input LArSIMple3DEnergyDeposit objects
     *  @param  wireAngle the angle of the wire plane to the vertical
     *  @param  view the readout view of the detector
     *  
     *  @return a vector of wire hits
     */
    std::vector<LArSIMpleWireHit> Convert3DEnergyDepositsToWireHits(
        const std::vector<LArSIMple3DEnergyDeposit> &energyDeposits3D, const LArSIMpleReadoutView &view) const;

private:
    /**
     *  @brief  Constructor
     */
    LArSIMpleWireConvertor(const LArSIMpleDetectorConstruction *const detector);

    void GetWireGeometryInfo(const LArSIMpleReadoutView &view, float &min_wire, float &max_wire) const;

    const LArSIMpleDetectorConstruction *fDetector; ///< Pointer to the detector geometry object
};

#endif
