/**
 *  @file   LArSIMple/include/LArSIMpleTrackingAction.hh
 * 
 *  @brief  Header file for the tracking action class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleTrackingAction_h
#define LArSIMpleTrackingAction_h

#include "G4UserTrackingAction.hh"

class LArSIMpleEventAction;
class G4Track;

/**
 *  @brief Tracking action class required by GEANT4
 */
class LArSIMpleTrackingAction : public G4UserTrackingAction
{
public:
    /**
     *  @brief  Constructor using the event action object
     *
     *  @param  pEventAction the pointer to the event action object
     */
    LArSIMpleTrackingAction(LArSIMpleEventAction *pEventAction);

    /**
     *  @brief  Destructor
     */
    virtual ~LArSIMpleTrackingAction();

    /**
     *  @brief  Perform operations before the tracking action step
     *
     *  @param  pTrack the pointer to the track object
     */
    void PreUserTrackingAction(const G4Track *pTrack);

    /**
     *  @brief  Perform operations after the tracking action step
     *
     *  @param  pTrack the pointer to the track object
     */
    void PostUserTrackingAction(const G4Track *pTrack);

private:
    LArSIMpleEventAction *fEventAction; ///< Pointer to the event action object
};

#endif
