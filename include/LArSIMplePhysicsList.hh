/**
 *  @file   LArSIMple/include/LArSIMplePhysicsList.hh
 * 
 *  @brief  Header file for the physics list class
 * 
 *  $Log: $
 */

#ifndef LArSIMplePhysicsList_h
#define LArSIMplePhysicsList_h 1

#include "QGSP_BIC_HP.hh"

/**
 *  @brief The physics list class
 */
class LArSIMplePhysicsList : public QGSP_BIC_HP
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
