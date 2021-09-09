#ifndef LArSIMpleEventAction_h
#define LArSIMpleEventAction_h

#include <map>

#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleTrackData.hh"

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ios.hh"

class LArSIMplePrimaryGeneratorAction;
class LArSIMpleMessenger;
class G4Track;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
class LArSIMpleEventAction : public G4UserEventAction {
  
  public:
  LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction*);
  virtual ~LArSIMpleEventAction();
  virtual void BeginOfEventAction(const G4Event*);
  virtual void EndOfEventAction(const G4Event*);
 
  void Add3DEnergyDeposit(LArSIMple3DEnergyDeposit edep);

  void SetOutputFileDirectory(std::string val) {fOutputFileDirectory = val;};
  void SetOutputFilePrefix(std::string val) {fOutputFilePrefix = val;};
 
  const LArSIMpleTrackData GetTrackDataFromTrackID(const int trackID) const;
  void AddTrack(const G4Track *track);

  double GetHitThreshold() const {return fHitThreshold;};
  void SetHitThreshold(const double threshold) {fHitThreshold = threshold;};

  void SetWriteZipAndInfoFiles(const bool val) {fWriteZipAndInfoFiles = val;};
  void SetWriteRootFile(const bool val) {fWriteRootFile = val;};

  bool FoldBackTruthInfo() const {return fFoldBackTruthInfo;};
  void SetFoldBackTruthInfo(const bool val) {fFoldBackTruthInfo = val;}; 

  private:  
  LArSIMplePrimaryGeneratorAction* fGenAction;
  LArSIMpleMessenger* fMessenger;

  unsigned int fEventID;
  double fHitThreshold;
  std::string fOutputFileDirectory;
  std::string fOutputFilePrefix;

  bool fWriteZipAndInfoFiles;
  bool fWriteRootFile;
  bool fFoldBackTruthInfo;

  std::vector<LArSIMple3DEnergyDeposit> fEnergyDeposits;

  // Tracks can be transient so keep track of the type
  std::map<int,LArSIMpleTrackData> fTrackIDToTrackData;
 
};

inline void LArSIMpleEventAction::Add3DEnergyDeposit(LArSIMple3DEnergyDeposit edep)
{
  fEnergyDeposits.push_back(edep);
}

#endif

