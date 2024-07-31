/**
 *  @file   LArSIMple/include/LArSIMpleRunAction.hh
 * 
 *  @brief  Header file for the run action class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleRunAction_h
#define LArSIMpleRunAction_h

#include "G4UserRunAction.hh"

class G4Run;

/**
 *  @brief Run action class required by GEANT4
 */
class LArSIMpleRunAction : public G4UserRunAction
{
public:
    /**
     *  @brief  Default constructor
     */
    LArSIMpleRunAction();

    /**
     *  @brief  Destructor
     */
    ~LArSIMpleRunAction();

    /**
     *  @brief  Function that runs when the run starts
     *
     *  @param  aRun a pointer to the current GEANT4 run object
     */
    void BeginOfRunAction(const G4Run *aRun);

    /**
     *  @brief  Function that runs when the run ends
     *
     *  @param  aRun a pointer to the current GEANT4 run object
     */
    void EndOfRunAction(const G4Run *aRun);
};

#endif
