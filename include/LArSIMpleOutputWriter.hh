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
    void WriteRootFile(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits, const LArSIMpleTrueNeutrinoEvent *trueEvt,
        const std::vector<double> &wireAngles) const;

private:
    unsigned int fEventNumber;    ///< The event number
};

#endif
