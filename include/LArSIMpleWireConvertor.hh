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
    static LArSIMpleWireConvertor &Get()
    {
        static LArSIMpleWireConvertor wireConvertor;
        return wireConvertor;
    };
    LArSIMpleWireConvertor(LArSIMpleWireConvertor const &) = delete;
    LArSIMpleWireConvertor &operator=(LArSIMpleWireConvertor const &) = delete;

    void SetGeometryInformation(const float &xMin, const float &xMax, const float &yMin, const float &yMax, const float &zMin,
        const float &zMax, const float &wireAngleU, const float &wireAngleV, const float &wireAngleW);

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
    LArSIMpleWireConvertor();

    void GetWireGeometryInfo(const LArSIMpleReadoutView &view, float &min_wire, float &max_wire) const;

    bool fGeometrySetupComplete;
    float fXMin;
    float fXMax;
    float fYMin;
    float fYMax;
    float fZMin;
    float fZMax;
    float fWireAngleU;
    float fWireAngleV;
    float fWireAngleW;
};

inline void LArSIMpleWireConvertor::SetGeometryInformation(const float &xMin, const float &xMax, const float &yMin, const float &yMax,
    const float &zMin, const float &zMax, const float &wireAngleU, const float &wireAngleV, const float &wireAngleW)
{
    fXMin = xMin;
    fXMax = xMax;
    fYMin = yMin;
    fYMax = yMax;
    fZMin = zMin;
    fZMax = zMax;
    fWireAngleU = wireAngleU;
    fWireAngleV = wireAngleV;
    fWireAngleW = wireAngleW;
    fGeometrySetupComplete = true;
}

#endif
