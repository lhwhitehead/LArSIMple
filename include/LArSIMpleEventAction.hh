/**
 *  @file   LArSIMple/include/LArSIMpleEventAction.hh
 * 
 *  @brief  Header file for the event action class.
 * 
 *  $Log: $
 */

#ifndef LArSIMpleEventAction_h
#define LArSIMpleEventAction_h

#include <map>

#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleTrackData.hh"
#include "LArSIMpleTruthFolder.hh"

#include "G4UserEventAction.hh"
#include "G4ios.hh"
#include "globals.hh"

class LArSIMplePrimaryGeneratorAction;
class LArSIMpleMessenger;
class G4Track;

/**
 *  @brief Class controlling the event actions in Geant4
 */
class LArSIMpleEventAction : public G4UserEventAction
{

public:
    LArSIMpleEventAction(LArSIMplePrimaryGeneratorAction *);
    virtual ~LArSIMpleEventAction();
    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);

    /**
     *  @brief  Add a 3D energy deposit (called by the SteppingAction class)
     *
     *  @param  edep the energy deposit to add
     */
    void Add3DEnergyDeposit(LArSIMple3DEnergyDeposit edep);

    /**
     *  @brief  Get the LArSIMpleTrackData object associated to a Geant4 track id
     *
     *  @param  trackID the Geant4 track id
     *
     *  @return The track data object associated to a Geant4 track id
     */
    LArSIMpleTrackData GetTrackDataFromTrackID(const int trackID) const;

    /**
     *  @brief  Get the map of G4 track id to LArSIMpleTrackData objects
     *
     *  @return A reference to the map of the G4 track id to LArSIMpleTrackData objects
     */
    const std::map<int, LArSIMpleTrackData>& GetTrackIDToTrackDataMap() const;

    /**
     *  @brief  Use a Geant4 track to populate a new LArSIMpleTrackData object
     *
     *  @param  track the Geant4 track to add
     */
    void AddTrack(const G4Track *track);

    /**
     *  @brief  Fill the end point information for an existing track
     *
     *  @param  track the Geant4 track required for updating the existin track
    */
    void UpdateTrackEndInfo(const G4Track *track);

    /**
     *  @brief  Check if we are folding back truth info to group together particles in showers
     *
     *  @return Whether folding back truth or not
     */
    bool FoldBackTruthInfo() const;

    /**
     *  @brief  Set whether or not to fold back truth info to group together particles in showers
     *
     *  @param val whether or not to do the truth folding
     */
    void SetFoldBackTruthInfo(const bool val);

    /**
     *  @brief  Set whether or not to fold back delta rays
     *
     *  @param val whether or not to do the truth folding for delta rays
     */
    void SetFoldBackDeltaRays(const bool val);

    /**
     *  @brief  Get a reference to the truth folding object
     *
     *  @return A reference to the truth folding object
     */
    LArSIMpleTruthFolder& GetTruthFolder();

    /**
     *  @brief  Associate the Geant4 track id with that of its folded ancestor
     *
     *  @param  trackID the Geant4 track id of the track
     *  @param  foldedTrackID the Geant4 track id of its folded ancestor
     */
    void AddFoldedTrackAssoc(const int trackID, const int foldedTrackID);

    /**
     *  @brief  Get the folded track id associated with the track
     *
     *  @param  trackID the Geant4 track id of the track
     *
     *  @return The Geant4 track id of its folded ancestor
     */
    int GetFoldedTrackID(const int trackID) const;

    /**
     *  @brief  Get the energy threshold for creating a 3DEnergyDeposit
     *
     *  @return The value of the energy threshold
     */
    double GetHitThreshold() const;

    /**
     *  @brief  Set the energy threshold for creating a 3DEnergyDeposit
     *
     *  @param  threshold the value of the energy threshold
     */
    void SetHitThreshold(const double threshold);

    /**
     *  @brief  Whether to calculate and use neighbourhood features for each 3DEnergyDeposit
     *
     *  @param  val whether or not to use the features
     */
    void SetUseHitFeatures(const bool val);

    /**
     *  @brief  Set the path to a directory for the output file(s)
     *
     *  @param  val the path to the directory
     */
    void SetOutputFileDirectory(std::string val);
 
    /**
     *  @brief  Set the prefix for the output file names
     *
     *  @param  val the prefix for the file names
     */
    void SetOutputFilePrefix(std::string val);

    /**
     *  @brief  Set whether to output zlib files with truth information
     *
     *  @param  val whether to output zlib files or not
     */
    void SetWriteZipAndInfoFiles(const bool val);

    /**
     *  @brief  Set whether to save output ROOT files or not
     *
     *  @param  val whether to output ROOT files or not
     */
    void SetWriteRootFile(const bool val);

    /**
     *  @brief  Set the angle of the U wire plane w.r.t the z-axis
     *
     *  @param  angle the angle to the z-axis
     */
    void SetWireAngleU(double angle);

    /**
     *  @brief  Set the angle of the V wire plane w.r.t the z-axis
     *
     *  @param  angle the angle to the z-axis
     */
    void SetWireAngleV(double angle);

    /**
     *  @brief  Set the angle of the W wire plane w.r.t the z-axis
     *
     *  @param  angle the angle to the z-axis
     */
    void SetWireAngleW(double angle);

private:
    /**
     *  @brief  We want the decay photons of primary pi0 mesons to be primaries
     */
    void UpdatePiZeroDecayPhotonTruth();

    /**
     *  @brief  Clean up vectors and maps for the next event
     */
    void CleanUp();

    LArSIMplePrimaryGeneratorAction *fGenAction;  ///< Pointer to the primary generator action class
    LArSIMpleMessenger *fMessenger;               ///< Pointer to the messenger config class

    unsigned int fEventID;                        ///< Event ID for the current event
    double fHitThreshold;                         ///< Energy threshold for 3DEnergyDeposit creation
    bool fUseHitFeatures;                         ///< Whether to use and calculate hit features from local neighbourhood

    std::string fOutputFileDirectory;             ///< Path to the output directory
    std::string fOutputFilePrefix;                ///< Prefix for output file names

    bool fWriteZipAndInfoFiles;                   ///< Whether to write zlib files
    bool fWriteRootFile;                          ///< Whether to write ROOT files
    bool fFoldBackTruthInfo;                      ///< Whether to fold back truth information
    LArSIMpleTruthFolder fTruthFolder;            ///< Truth folding object

    double fWireAngleU;                           ///< Wire angle for the U plane
    double fWireAngleV;                           ///< Wire angle for the V plane
    double fWireAngleW;                           ///< Wire angle for the W plane

    std::vector<LArSIMple3DEnergyDeposit> fEnergyDeposits; ///< The 3DEnergyDeposits of the current event
    std::map<int, LArSIMpleTrackData> fTrackIDToTrackData; ///< Map of Geant4 track IDs to TrackData objects
    std::map<int, int> fTrackIDToFoldedTrackID;            ///< Map of Geant4 track IDs to folded track IDs
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::Add3DEnergyDeposit(LArSIMple3DEnergyDeposit edep)
{
    fEnergyDeposits.emplace_back(edep);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline const std::map<int, LArSIMpleTrackData>& LArSIMpleEventAction::GetTrackIDToTrackDataMap() const
{
    return fTrackIDToTrackData;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline bool LArSIMpleEventAction::FoldBackTruthInfo() const
{
    return fFoldBackTruthInfo;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetFoldBackTruthInfo(const bool val)
{
    fFoldBackTruthInfo = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetFoldBackDeltaRays(const bool val)
{
    fTruthFolder.SetFoldDeltaRays(val);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline LArSIMpleTruthFolder& LArSIMpleEventAction::GetTruthFolder()
{
    return fTruthFolder;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::AddFoldedTrackAssoc(const int trackID, const int foldedTrackID)
{
    fTruthFolder.AddFoldedTrackAssoc(trackID, foldedTrackID);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline int LArSIMpleEventAction::GetFoldedTrackID(const int trackID) const
{
    return fTruthFolder.GetFoldedTrackID(trackID);
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline double LArSIMpleEventAction::GetHitThreshold() const
{
    return fHitThreshold;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetHitThreshold(const double threshold)
{
    fHitThreshold = threshold;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetUseHitFeatures(const bool val)
{
    fUseHitFeatures = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetOutputFileDirectory(std::string val)
{
    fOutputFileDirectory = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetOutputFilePrefix(std::string val)
{
    fOutputFilePrefix = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetWriteZipAndInfoFiles(const bool val)
{
    fWriteZipAndInfoFiles = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetWriteRootFile(const bool val)
{
    fWriteRootFile = val;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetWireAngleU(double angle)
{
    fWireAngleU = angle;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetWireAngleV(double angle)
{
    fWireAngleV = angle;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline void LArSIMpleEventAction::SetWireAngleW(double angle)
{
    fWireAngleW = angle;
}

#endif
