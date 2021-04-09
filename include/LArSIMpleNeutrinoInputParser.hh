#ifndef LArSIMpleNeutrinoInputParser_h
#define LArSIMpleNeutrinoInputParser_h

#include "LArSIMpleTrueNeutrinoEvent.hh"
#include <vector>
#include <string>

class LArSIMpleNeutrinoInputParser
{

  public:
  LArSIMpleNeutrinoInputParser();
  ~LArSIMpleNeutrinoInputParser();

  void ReadFromNuanceTrackerFile(const std::string &filename);
  void ReadFromGENIETreeFile(const std::string &filename);

  private:

  LArSIMpleNeutrinoInteractionType ConvertNuanceCode(int nuanceCode);

  std::vector<LArSIMpleTrueNeutrinoEvent> fNeutrinoEvents;
  
};

#endif

