#include <fstream>

#include "LArSIMpleNeutrinoInputParser.hh"

#include "TChain.h"

LArSIMpleNeutrinoInputParser::LArSIMpleNeutrinoInputParser()
{

}

LArSIMpleNeutrinoInputParser::~LArSIMpleNeutrinoInputParser()
{

}

void LArSIMpleNeutrinoInputParser::ReadFile(const std::string &filename, const LArSIMpleNeutrinoInputType type)
{
  if (LArSIMpleNeutrinoInputType::kNuanceTracker == type)
    this->ReadFromNuanceTrackerFile(filename);
  else if (LArSIMpleNeutrinoInputType::kGENIETree == type)
    this->ReadFromGENIETreeFile(filename);
  else
    std::cerr << "Error: neutrino file format not set" << std::endl;
}

void LArSIMpleNeutrinoInputParser::ReadFromNuanceTrackerFile(const std::string &filename)
{

  std::cout << "Reading neutrino events from " << filename << std::endl;
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
      std::cout << "End of nuance vector file!" << std::endl;
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
  }

  if(inputFile.is_open())
    inputFile.close();
}

void LArSIMpleNeutrinoInputParser::ReadFromGENIETreeFile(const std::string &filename)
{
  TChain *input = new TChain("gst");
  input->Add(filename.c_str());

  // This isn't a full list of variables, just the key ones for now
  // Interaction details
  int nuanceCode;
  double vertexX;
  double vertexY;
  double vertexZ;
  input->SetBranchAddress("nuance_code",&nuanceCode);
  input->SetBranchAddress("vtxx",&vertexX);
  input->SetBranchAddress("vtxy",&vertexY);
  input->SetBranchAddress("vtxz",&vertexZ);

  // The neutrino
  int neutrinoPdg;
  double neutrinoEnergy;
  double neutrinoPx;
  double neutrinoPy;
  double neutrinoPz;
  input->SetBranchAddress("neu",&neutrinoPdg);
  input->SetBranchAddress("Ev",&neutrinoEnergy);
  input->SetBranchAddress("pxv",&neutrinoPx);
  input->SetBranchAddress("pyv",&neutrinoPy);
  input->SetBranchAddress("pzv",&neutrinoPz);

  // Target
  int targetPdg;
  double targetEnergy;
  double targetPx;
  double targetPy;
  double targetPz;
  input->SetBranchAddress("tgt",&targetPdg);
  input->SetBranchAddress("En", &targetEnergy);
  input->SetBranchAddress("pxn",&targetPx);
  input->SetBranchAddress("pyn",&targetPy);
  input->SetBranchAddress("pzn",&targetPz);

  // Leading lepton is considered separately
  int leptonPdg;
  double leptonEnergy;
  double leptonPx;
  double leptonPy;
  double leptonPz;
  input->SetBranchAddress("fspl",&leptonPdg);
  input->SetBranchAddress("El", &leptonEnergy);
  input->SetBranchAddress("pxl",&leptonPx);
  input->SetBranchAddress("pyl",&leptonPy);
  input->SetBranchAddress("pzl",&leptonPz);

  // Final-state hadrons with information stored as arrays. Set this to 100 elements to
  // hopefully avoid anything going wrong
  const unsigned int nAllowedFSPs = 100;
  int nFinalStates;
  int finalPdg[nAllowedFSPs];
  double finalEnergy[nAllowedFSPs];
  double finalPx[nAllowedFSPs];
  double finalPy[nAllowedFSPs];
  double finalPz[nAllowedFSPs];
  input->SetBranchAddress("nf",&nFinalStates);
  input->SetBranchAddress("pdgf",&finalPdg[0]);
  input->SetBranchAddress("Ef",&finalEnergy[0]);
  input->SetBranchAddress("pxf",&finalPx[0]);
  input->SetBranchAddress("pyf",&finalPy[0]);
  input->SetBranchAddress("pzf",&finalPz[0]);

  for (unsigned int e = 0; e < input->GetEntries(); ++e)
  {
    input->GetEntry(e);

    LArSIMpleTrueNeutrinoEvent newEvent;

    // Interaction first
    newEvent.SetInteractionType(this->ConvertNuanceCode(nuanceCode));
    G4ThreeVector vtx(vertexX,vertexY,vertexZ);
    newEvent.SetInteractionVertex(vtx);

    // Neutrino - NB: vtx doesn't make sense for the neutrino, but that's ok
    newEvent.AddNeutrino(vtx,G4ThreeVector(neutrinoPx,neutrinoPy,neutrinoPz).unit(),neutrinoEnergy*1000.,neutrinoPdg);

    // Target
    newEvent.AddTarget(vtx,G4ThreeVector(targetPx,targetPy,targetPz).unit(),targetEnergy*1000.,targetPdg);

    // Leading lepton - this is treated like any other final state particle by LArSIMple
    newEvent.AddFinalStateParticle(vtx,G4ThreeVector(leptonPx,leptonPy,leptonPz).unit(),leptonEnergy*1000.,leptonPdg);

    // Final state hadrons
    if (nFinalStates > static_cast<int>(nAllowedFSPs))
      nFinalStates = static_cast<int>(nAllowedFSPs);

    for (unsigned int h = 0; h < static_cast<unsigned int>(nFinalStates); ++h)
      newEvent.AddFinalStateParticle(vtx,G4ThreeVector(finalPx[h],finalPy[h],finalPz[h]).unit(),finalEnergy[h]*1000.,finalPdg[h]);

    fNeutrinoEvents.push_back(newEvent);
  }

  input->ResetBranchAddresses(); 
  delete input;
  input = nullptr;
}

