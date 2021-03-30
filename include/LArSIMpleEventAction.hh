#ifndef LArSIMpleEventAction_h
#define LArSIMpleEventAction_h 1

#include "LArSIMple3DEnergyDeposit.hh"

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ios.hh"

class LArSIMplePrimaryGeneratorAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
class LArSIMpleEventAction : public G4UserEventAction {
  
public:
  LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction*);
  virtual ~LArSIMpleEventAction();
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
 
  void Add3DEnergyDeposit(LArSIMple3DEnergyDeposit edep);
  void SetOutputFileBase(std::string base) {fOutputFileBase = base;};
 
private:
  
  LArSIMplePrimaryGeneratorAction* fGenAction;

  unsigned int fEventID;

  std::string fOutputFileBase;

  std::vector<LArSIMple3DEnergyDeposit> fEnergyDeposits;

//  G4int detectorCollID;                // hits collection
  
  void WriteOutputFiles() const;
  
};

inline void LArSIMpleEventAction::Add3DEnergyDeposit(LArSIMple3DEnergyDeposit edep)
{
  fEnergyDeposits.push_back(edep);
}

#endif

