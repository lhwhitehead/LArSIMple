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

  std::ifstream input(filename.c_str());

  if(input.is_open())
    input.close();
}

void LArSIMpleNeutrinoInputParser::ReadFromGENIETreeFile(const std::string &filename)
{
  TChain *input = new TChain("some_name");
  input->Add(filename.c_str());

  delete input;
  input = nullptr;
}

