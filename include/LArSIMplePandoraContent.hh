/**
 *  @file   LArSIMple/include/LArSIMplePandoraContent.hh
 * 
 *  @brief  Header file for the messenger class.
 * 
 *  $Log: $
 */

#ifndef LArSIMplePandoraContent_h
#define LArSIMplePandoraContent_h

#include "Pandora/Pandora.h"

class LArSIMplePandoraContent
{
public:
    /**
     *  @brief  Register all the lar content algorithms and tools with pandora
     *
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterAlgorithms(const pandora::Pandora &pandora);

    /**
     *  @brief  Register the basic lar content plugins with pandora
     *
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterBasicPlugins(const pandora::Pandora &pandora);
};

#endif
