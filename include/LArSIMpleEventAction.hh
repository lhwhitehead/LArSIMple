#ifndef LArSIMpleEventAction_h
#define LArSIMpleEventAction_h 1

#include <map>

#include "LArSIMple3DEnergyDeposit.hh"

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ios.hh"

class LArSIMplePrimaryGeneratorAction;
class LArSIMpleMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
class LArSIMpleEventAction : public G4UserEventAction {
  
public:
  LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction*);
  virtual ~LArSIMpleEventAction();
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
 
  void Add3DEnergyDeposit(LArSIMple3DEnergyDeposit edep);
  void SetOutputFileBase(std::string base) {fOutputFileBase = base;};
 
  int GetPDGFromTrackID(const int trackID) const;
  void AddTrackIDAndPDG(const int trackID, const int pdg);

  double GetHitThreshold() const {return fHitThreshold;};
  void SetHitThreshold(const double threshold) {fHitThreshold = threshold;};

private:
  
  LArSIMplePrimaryGeneratorAction* fGenAction;
  LArSIMpleMessenger* fMessenger;

  unsigned int fEventID;
  double fHitThreshold;
  std::string fOutputFileBase;

  std::vector<LArSIMple3DEnergyDeposit> fEnergyDeposits;

  // Tracks can be transient so keep track of the type
  std::map<int,int> fTrackIDToPDG;
  
  void WriteOutputFiles() const;
  
};

inline void LArSIMpleEventAction::Add3DEnergyDeposit(LArSIMple3DEnergyDeposit edep)
{
  fEnergyDeposits.push_back(edep);
}

#endif

