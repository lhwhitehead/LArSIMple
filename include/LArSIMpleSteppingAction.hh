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
    /**
     *  @brief  Get the folded truth information: the information of the folded ancestor
     *
     *  @param  track the current true particle track
     *  @param  foldedTrackID the address to store the ID of the folded ancestor track
     *  @param  foldedTrackPDG the address to store the PDG code of the folded ancestor track
     *  @param  foldedTrackProcess the address to store the process code of the folded ancestor track
     */
    //    void GetFoldedTrackInfo(const G4Track *track, int &foldedTrackID, int &foldedTrackPDG, int &foldedTrackProcess);

    LArSIMpleEventAction *fEventAction; ///< Pointer to the event action object
};

#endif
