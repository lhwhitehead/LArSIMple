#ifndef LArSIMpleRunAction_h
#define LArSIMpleRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Run;

class LArSIMpleRunAction : public G4UserRunAction
{
  public:
    LArSIMpleRunAction();
   ~LArSIMpleRunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);


};

#endif

