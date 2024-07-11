#ifndef LArSIMpleRunAction_h
#define LArSIMpleRunAction_h

#include "G4UserRunAction.hh"

class G4Run;

class LArSIMpleRunAction : public G4UserRunAction
{
public:
    LArSIMpleRunAction();
    ~LArSIMpleRunAction();

    void BeginOfRunAction(const G4Run *);
    void EndOfRunAction(const G4Run *);
};

#endif
