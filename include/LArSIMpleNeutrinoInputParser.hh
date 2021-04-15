#ifndef LArSIMpleNeutrinoInputParser_h
#define LArSIMpleNeutrinoInputParser_h

#include "LArSIMpleTrueNeutrinoEvent.hh"
#include <vector>
#include <string>
#include <fstream>

class LArSIMpleNeutrinoInputParser
{

  public:
  LArSIMpleNeutrinoInputParser();
  LArSIMpleNeutrinoInputParser(unsigned int nevents);
  ~LArSIMpleNeutrinoInputParser();

  void ReadFromNuanceTrackerFile(const std::string &filename);
  void ReadFromGENIETreeFile(const std::string &filename);

  void SetNEventsToRead(unsigned int val) {fNEventsToRead = val;};
  unsigned int GetNEvents() const {return fNeutrinoEvents.size();};

  private:

  LArSIMpleNeutrinoInteractionType ConvertNuanceCode(int code);
  std::vector<std::string> ReadNuanceTrackerLine(std::ifstream& inFile, int lineSize, char* inBuf);
  std::vector<std::string> TokeniseString(std::string separators, std::string input);

  unsigned int fNEventsToRead;
  std::vector<LArSIMpleTrueNeutrinoEvent> fNeutrinoEvents;
  
};

inline LArSIMpleNeutrinoInteractionType LArSIMpleNeutrinoInputParser::ConvertNuanceCode(int code)
{
  // Use simple, broad categories for now

  if(code == 1) return LArSIMpleNeutrinoInteractionType::kCCQE;
  if(code == 2) return LArSIMpleNeutrinoInteractionType::kNCQE;
  if(code == 91) return LArSIMpleNeutrinoInteractionType::kCCDIS;
  if(code == 92) return LArSIMpleNeutrinoInteractionType::kNCDIS;
  if(code == 96) return LArSIMpleNeutrinoInteractionType::kNCOther;
  if(code == 97) return LArSIMpleNeutrinoInteractionType::kCCOther;

  // The neutrino and antineutrino states are seperate, but we will combine here
  if((code >= 3 && code <= 5) || (code >= 10 && code <= 12)) return LArSIMpleNeutrinoInteractionType::kCCRES;
  if((code >= 6 && code <= 9) || (code >= 13 && code <= 16)) return LArSIMpleNeutrinoInteractionType::kNCRES;

  // There are a few other things, but we can ignore them for now
  return LArSIMpleNeutrinoInteractionType::kDummy;
}

inline std::vector<std::string> LArSIMpleNeutrinoInputParser::ReadNuanceTrackerLine(std::ifstream& inFile, int lineSize, char* inBuf)
{
  // Read in line break it up into tokens
  inFile.getline(inBuf, lineSize);
  return TokeniseString(" $", inBuf);
}

// Returns a vector with the tokens
inline std::vector<std::string> LArSIMpleNeutrinoInputParser::TokeniseString(std::string separators, std::string input)
{
  std::size_t startToken = 0, endToken;
  std::vector<std::string> tokens;

  if (separators.size() > 0 && input.size() > 0)
  {
    while (startToken < input.size())
    {
      startToken = input.find_first_not_of(separators, startToken);

      if (startToken != input.npos)
      {
        endToken = input.find_first_of(separators, startToken);
        if (endToken == input.npos)
          endToken = input.size();

        tokens.push_back(input.substr(startToken, endToken - startToken));

        startToken = endToken;
      }
    }
  }

  return tokens;
}

#endif

