#include <fstream>

#include "LArSIMpleNeutrinoInputParser.hh"

#include "TChain.h"

LArSIMpleNeutrinoInputParser::LArSIMpleNeutrinoInputParser()
{

}

LArSIMpleNeutrinoInputParser::~LArSIMpleNeutrinoInputParser()
{

}

void LArSIMpleNeutrinoInputParser::ReadFromNuanceTrackerFile(const std::string &filename)
{

  std::ifstream inputFile(filename.c_str());

  // Nuance tracker files have a very specific format for each event
  const int lineSize = 100;
  char inBuf[lineSize];
  std::vector<std::string> token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf);

  if (token.size() == 0)
  {
    std::cout << "end of nuance vector file!" << std::endl;
    return;
  }
  
  // The first line in the file says "begin"
  if (token[0] != "begin") {
    std::cout << "Unexpected first line begins with " << token[0] << std::endl;
  } 

  LArSIMpleTrueNeutrinoEvent newEvent;

  // Read the nuance line - the second token is the interaction type
  token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf);
  // The nuance line contains the interaction mode. Bag it and tag it
  newEvent.SetInteractionType(this->ConvertNuanceCode(std::atoi(token[1].c_str())));

  // Read the interaction vertex line
  token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf);
  newEvent.SetInteractionVertex(G4ThreeVector(atof(token[1].c_str()), atof(token[2].c_str()), std::atof(token[3].c_str())));

  // Now, the neutrino line
  token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf);
  int pdg(std::atoi(token[1].c_str()));
  double energy(std::atof(token[2].c_str()));
  G4ThreeVector pos; // Default vector since the production point of the neutrino doesn't make sense
  G4ThreeVector dir(std::atof(token[3].c_str()), std::atof(token[4].c_str()), std::atof(token[5].c_str()));
  newEvent.AddNeutrino(pos,dir,energy,pdg);

  // Now read the target line 
  token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf);
  pdg = std::atoi(token[1].c_str());
  energy = std::atof(token[2].c_str());
  dir = G4ThreeVector(std::atof(token[3].c_str()), std::atof(token[4].c_str()), std::atof(token[5].c_str()));
  newEvent.AddTarget(pos,dir,energy,pdg);

  // Now read the outgoing particles
  while (token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf), token[0] == "track") {
    // We are only interested in the particles
    // that leave the nucleus, tagged by "0" 
    if (token[6] == "0") {
      pdg = std::atoi(token[1].c_str());
      energy = std::atof(token[2].c_str());
      dir = G4ThreeVector(std::atof(token[3].c_str()), std::atof(token[4].c_str()), std::atof(token[5].c_str()));
      pos = newEvent.GetInteractionVertex();
      newEvent.AddFinalStateParticle(pos,dir,energy,pdg); 
    }
  }

  if(inputFile.is_open())
    inputFile.close();
}

void LArSIMpleNeutrinoInputParser::ReadFromGENIETreeFile(const std::string &filename)
{
  TChain *input = new TChain("some_name");
  input->Add(filename.c_str());

  delete input;
  input = nullptr;
}

