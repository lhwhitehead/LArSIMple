/**
 *  @file   LArSIMple/include/LArSIMpleSteppingAction.hh
 * 
 *  @brief  Header file for the stepping action class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleSteppingAction_h
#define LArSIMpleSteppingAction_h

#include "G4UserSteppingAction.hh"

class LArSIMpleEventAction;
class G4Step;
class G4Track;
class G4VProcess;

/**
 *  @brief The stepping action class that determines what is done at each step of the simulation
 */
class LArSIMpleSteppingAction : public G4UserSteppingAction
{
public:
    /**
     *  @brief  Constructor
     *
     *  @param  pEventAction the pointer to the event action object
     */
    LArSIMpleSteppingAction(LArSIMpleEventAction *pEventAction);

    /**
     *  @brief  Destructor
     */
    virtual ~LArSIMpleSteppingAction();

    /**
     *  @brief  Action performed at each step
     *
     *  @param  aStep the current GEANT4 step
     */
    void UserSteppingAction(const G4Step *aStep);

    /**
     *  @brief  Get the current GEANT4 process for this step
     *
     *  @return The current process
     */
    G4VProcess *GetCurrentProcess();

private:
    LArSIMpleEventAction *fEventAction; ///< Pointer to the event action object
};

#endif
