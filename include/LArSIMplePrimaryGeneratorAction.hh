#ifndef LArSIMplePrimaryGeneratorAction_h
#define LArSIMplePrimaryGeneratorAction_h 1

#include "G4ThreeVector.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "LArSIMpleNeutrinoInputParser.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class LArSIMplePrimaryGeneratorMessenger;
class LArSIMpleDetectorConstruction;
class LArSIMpleTrueNeutrinoEvent;

class LArSIMplePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    LArSIMplePrimaryGeneratorAction(const LArSIMpleDetectorConstruction *detCon);
    ~LArSIMplePrimaryGeneratorAction();

    void GeneratePrimaries(G4Event *anEvent);

    std::string GetNeutrinoFileName() const
    {
        return fNeutrinoFileName;
    };
    LArSIMpleNeutrinoInputType GetNeutrinoFileType() const
    {
        return fNeutrinoFileType;
    };

    void SetNeutrinoFileName(std::string val)
    {
        fNeutrinoFileName = val;
    };
    void SetNeutrinoFileType(LArSIMpleNeutrinoInputType val)
    {
        fNeutrinoFileType = val;
    };

    bool UseNeutrinos() const
    {
        return fUseNeutrinos;
    };
    void SetUseNeutrinos(bool val)
    {
        fUseNeutrinos = val;
    };

    void SetUseRandomNeutrinoVertex(bool val)
    {
        fUseRandomNeutrinoVertex = val;
    };
    void SetNeutrinoVertex(G4ThreeVector vec)
    {
        fNeutrinoVertex = vec;
    };

    const LArSIMpleTrueNeutrinoEvent *GetTrueNeutrinoEventPointer()
    {
        return fNeutrinoEvent;
    };

private:
    G4GeneralParticleSource *fParticleGun;
    LArSIMplePrimaryGeneratorMessenger *fMessenger;
    LArSIMpleNeutrinoInputParser fNeutrinoInputParser;

    // Pointer such that it can be null
    LArSIMpleTrueNeutrinoEvent *fNeutrinoEvent;

    bool fUseNeutrinos;
    std::string fNeutrinoFileName;
    LArSIMpleNeutrinoInputType fNeutrinoFileType;

    bool fUseRandomNeutrinoVertex;
    G4ThreeVector fNeutrinoVertex;

    const LArSIMpleDetectorConstruction *fDetectorConstruction;

    void RandomiseVertex(G4ThreeVector &vtx) const;
};

#endif
