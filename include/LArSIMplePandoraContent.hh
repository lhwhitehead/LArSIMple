/**
 *  @file   LArSIMple/include/LArSIMplePandoraContent.hh
 * 
 *  @brief  Header file for the pandora content class.
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
     *  @brief  Register all the content algorithms with pandora
     *
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterAlgorithms(const pandora::Pandora &pandora);
};

#endif
