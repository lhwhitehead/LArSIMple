#ifndef LArSIMpleTrackData_h
#define LArSIMpleTrackData_h

#include <string>

class G4Track;

class LArSIMpleTrackData
{
  public:
  LArSIMpleTrackData();
  LArSIMpleTrackData(const G4Track* track);
  LArSIMpleTrackData(const LArSIMpleTrackData &rhs);
  LArSIMpleTrackData operator=(const LArSIMpleTrackData &rhs);

  ~LArSIMpleTrackData();

  int GetTrackID() const {return fTrackID;};
  int GetParentID() const {return fParentID;};
  int GetPDG() const {return fPDG;};
  std::string GetProcess() const {return fProcess;};
  bool IsFoldable() const {return fIsFoldable;};

  private:
  bool CanTrackBeFolded(const G4Track* track) const;

  int fTrackID;
  int fParentID;
  int fPDG;
  std::string fProcess;
  bool fIsFoldable;
};

#endif
