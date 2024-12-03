/**
 *  @file   LArSIMple/include/LArSIMplePhysicsList.hh
 * 
 *  @brief  Header file for the physics list class
 * 
 *  $Log: $
 */

#ifndef LArSIMplePhysicsList_h
#define LArSIMplePhysicsList_h 1

#include "QGSP_BERT.hh"

/**
 *  @brief The physics list class
 */
class LArSIMplePhysicsList : public QGSP_BERT
{
public:
    /**
     *  @brief  Default constructor
     */
    LArSIMplePhysicsList();

    /**
     *  @brief  Destructor
     */
    virtual ~LArSIMplePhysicsList();

    /**
     *  @brief  Set the cuts for particle tracking
     */
    virtual void SetCuts();
};

#endif
