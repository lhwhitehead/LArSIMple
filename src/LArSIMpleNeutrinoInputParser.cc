#include <fstream>

#include "LArSIMpleNeutrinoInputParser.hh"

#include "TChain.h"

LArSIMpleNeutrinoInputParser::LArSIMpleNeutrinoInputParser() : fNEventsToRead(0)
{

}

LArSIMpleNeutrinoInputParser::LArSIMpleNeutrinoInputParser(unsigned int nevents) : fNEventsToRead(nevents)
{

}

LArSIMpleNeutrinoInputParser::~LArSIMpleNeutrinoInputParser()
{

}

void LArSIMpleNeutrinoInputParser::ReadFromNuanceTrackerFile(const std::string &filename)
{

  if (fNEventsToRead == 0)
  {
    std::cerr << "You haven't requested any events to read from the tracker file. Reading them all." << std::endl;
  }

  std::ifstream inputFile(filename.c_str());

  // Keep going until we reach the stop command or we have the number of events that we want
  // Each iteration of this loops reads one entire event
  while (true)
  {    
    // Nuance tracker files have a very specific format for each event
    const int lineSize = 100;
    char inBuf[lineSize];
    std::vector<std::string> token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf);
 
    // Check we haven't reached the end of the input file 
    if (token.size() == 0 || token[0] == "stop")
    {
      std::cout << "end of nuance vector file!" << std::endl;
      break;
    }
    
    // The first line in the file says "begin"
    if (token[0] != "begin") {
      std::cout << "Unexpected first line begins with " << token[0] << std::endl;
      break;
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

    // We should now have read the "end" line, but let's make sure
    if (token[0] == "end")
    {
      fNeutrinoEvents.push_back(newEvent);
    }
    else
    {
      std::cout << "Unexpected line in file... exiting parser" << std::endl;
      return;
    }

    // If we have enough events then we can give up
    if (fNeutrinoEvents.size() == fNEventsToRead)
    {
      std::cout << "Successfully read " << fNEventsToRead << " from file " << filename << std::endl;
      break;
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

