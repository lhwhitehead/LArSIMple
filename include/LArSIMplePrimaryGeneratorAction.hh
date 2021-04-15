#ifndef LArSIMplePrimaryGeneratorAction_h
#define LArSIMplePrimaryGeneratorAction_h 1

#include "LArSIMpleNeutrinoInputParser.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class LArSIMplePrimaryGeneratorMessenger;

class LArSIMplePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

public:
  LArSIMplePrimaryGeneratorAction();
  ~LArSIMplePrimaryGeneratorAction();
  
  void GeneratePrimaries(G4Event* anEvent);

  std::string GetNeutrinoFileName() const {return fNeutrinoFileName;};
  LArSIMpleNeutrinoInputType GetNeutrinoFileType() const {return fNeutrinoFileType;};

  void SetNeutrinoFileName(std::string val) {fNeutrinoFileName = val;};
  void SetNeutrinoFileType(LArSIMpleNeutrinoInputType val) {fNeutrinoFileType = val;};

  bool UseNeutrinos() const {return fUseNeutrinos;};
  void SetUseNeutrinos(bool val) {fUseNeutrinos = val;};
 
private:

  G4GeneralParticleSource* fParticleGun;
  LArSIMplePrimaryGeneratorMessenger* fMessenger;
  LArSIMpleNeutrinoInputParser fNeutrinoInputParser;

  bool fUseNeutrinos;
  std::string fNeutrinoFileName;
  LArSIMpleNeutrinoInputType fNeutrinoFileType;
};

#endif

