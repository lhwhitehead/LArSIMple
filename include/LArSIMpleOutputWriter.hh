/**
 *  @file   LArSIMple/include/LArSIMpleOutputWriter.hh
 * 
 *  @brief  Header file for the output file writing class
 * 
 *  $Log: $
 */

#ifndef LArSIMpleOutputWriter_h
#define LArSIMpleOutputWriter_h

#include <vector>

#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleTrackData.hh"
#include "LArSIMpleWireHit.hh"

class LArSIMpleTrueNeutrinoEvent;

/**
 *  @brief Class that writes the output files from the simulation
 */
class LArSIMpleOutputWriter
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  eventNumber the event to write out
     */
    LArSIMpleOutputWriter(unsigned int eventNumber);

    /**
     *  @brief  Destructor
     */
    virtual ~LArSIMpleOutputWriter();

    /**
     *  @brief  Write out zlib-zipped files and a text summary (can be used for CNN approaches)
     *
     *  @param  base the output file directory
     *  @param  hits the vector of 3D energy deposits 
     */
    void WriteOutputZipAndInfoFiles(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits) const;

    /**
     *  @brief  Write out zlib-zipped files and a text summary (can be used for CNN approaches)
     *
     *  @param  base the output file directory
     *  @param  hits the vector of 3D energy deposits
     *  @param  trueEvt the true neutrino event
     *  @param  wireAngles the angles of the three wire readout planes
     */
    void WriteRootFile(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits, const std::vector<LArSIMpleWireHit> &uHits,
        const std::vector<LArSIMpleWireHit> &vHits, const std::vector<LArSIMpleWireHit> &wHits, const LArSIMpleTrueNeutrinoEvent *trueEvt,
        std::map<int, LArSIMpleTrackData> &trueTracks, const std::vector<double> &wireAngles) const;

private:
    /**
     *  @brief  Convert YZ coordinates into those from wire planes
     *
     *  @param  y the y-coordinate
     *  @param  z the z-coordinate
     *  @param  wireAngle the angle of the wire plane to the z axis
     *
     *  @return The coordinate in the wire plane
     */
    double ConvertYZToWireCoordinate(const double y, const double z, const double wireAngle) const;

    unsigned int fEventNumber; ///< The event number
};

inline double LArSIMpleOutputWriter::ConvertYZToWireCoordinate(const double y, const double z, const double wireAngle) const
{
    const double degreesToRadians{3.14159265358979323846 / 180.};
    return z * std::cos(wireAngle * degreesToRadians) - y * std::sin(wireAngle * degreesToRadians);
}

#endif
