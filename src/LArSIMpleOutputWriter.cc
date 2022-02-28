#include <vector>
#include <sstream>
#include <fstream>

#include "LArSIMpleOutputWriter.hh"
#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleTrueNeutrinoEvent.hh"

#include "TFile.h"
#include "TTree.h"

#include "zlib.h"

LArSIMpleOutputWriter::LArSIMpleOutputWriter(unsigned int eventNumber) : fEventNumber(eventNumber) 
{

}


LArSIMpleOutputWriter::~LArSIMpleOutputWriter() 
{

}

void LArSIMpleOutputWriter::WriteOutputZipAndInfoFiles(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits) const
{

  std::vector<float> flatVec;
  // Flatten the hits
  for (const LArSIMple3DEnergyDeposit &hit : hits)
  {
    std::vector<float> flatHit = hit.GetFlatRepresentation();
    flatVec.insert(flatVec.end(),flatHit.begin(),flatHit.end());
  }

  // Write the output using zlib
  const ulong src_len = flatVec.size() * sizeof(float); // pixelArray length
  ulong dest_len = compressBound(src_len);     // calculate size of the compressed data               
  const char* outputStream = (char *) malloc(dest_len);  // allocate memory for the compressed data
  const int res = compress((Bytef *) outputStream, &dest_len, (Bytef *) &flatVec[0], src_len);

  // Buffer error

  if (res == Z_BUF_ERROR)
    std::cout << "Buffer too small!" << std::endl;
  // Memory error
  else if (res ==  Z_MEM_ERROR)
    std::cout << "Not enough memory for compression!" << std::endl;
  // Compression ok 
  else
  {
    std::cout << "Hooray... writing files" << std::endl;

    // Create output files 
    std::stringstream image_file_name;
    image_file_name << base << "_test_event_" << fEventNumber << ".gz";

    std::ofstream image_file (image_file_name.str(), std::ofstream::binary);
    if(image_file.is_open())
    {
      // Write the graph to the file and close it
      image_file.write(outputStream, dest_len);
      image_file.close();
    }
  }

  // Write info file
  std::stringstream infoFileName;
  infoFileName << base << "_test_event_" << fEventNumber << ".info";
  std::ofstream infoFile(infoFileName.str());
  if(infoFile.is_open())
  {
    // Number of nodes
    infoFile << hits.size() << std::endl;
    // Number of positions
    infoFile << hits.at(0).GetNCoordinates() << std::endl;
    // Number of features
    infoFile << hits.at(0).GetNFeatures() << std::endl;
    // Truth information
    infoFile << hits.at(0).GetNTruths() << std::endl;

    infoFile.close();
  }

}

// This is very specific and hardcoded
void LArSIMpleOutputWriter::WriteRootFile(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits, const LArSIMpleTrueNeutrinoEvent *evt) const
{
  std::stringstream rootFileName;
  rootFileName << base << "_test_event_" << fEventNumber << ".root";
  TTree *outputTree = new TTree("hits","");
  outputTree->SetDirectory(0);

  // Each event consists the basic event information and a number of vectors

  // Event level information
  int eventNumber;
  outputTree->Branch("eventNumber",&eventNumber,"eventNumber/I");
  eventNumber = fEventNumber;

  // Specific for neutrino events
  int neutrinoPdg;
  float neutrinoEnergy;
  int nFinalStates;
  int interactionType;
  if (nullptr != evt)
  {
    outputTree->Branch("neutrinoPdg",&neutrinoPdg,"neutrinoPdg/I");
    outputTree->Branch("neutrinoEnergy",&neutrinoEnergy,"neutrinoEnergy/F");
    outputTree->Branch("nFinalStates",&nFinalStates,"nFinalStates/I");
    outputTree->Branch("interactionType",&interactionType,"interactionType/I");

    neutrinoPdg = evt->GetNeutrinoPDGCode();
    neutrinoEnergy = evt->GetNeutrinoEnergy();
    nFinalStates = evt->GetNumberOfFinalStateParticles();
    interactionType = static_cast<int>(evt->GetInteractionType());
  }
  
  // Hit level information
  std::vector<float> posX, posY, posZ;
  std::vector<float> posU, posV, posW;
  std::vector<float> charge, dedx;
  std::vector<float> angle, dotProduct;
  std::vector<float> neighboursR1, neighboursR2, neighboursR3;
  std::vector<float> chargeR1, chargeR2, chargeR3;
  std::vector<int> pdg, trackid, process;

  outputTree->Branch("x",&posX);
  outputTree->Branch("y",&posY);
  outputTree->Branch("z",&posZ);
  outputTree->Branch("u",&posU);
  outputTree->Branch("v",&posV);
  outputTree->Branch("w",&posW);
  outputTree->Branch("charge",&charge);
  outputTree->Branch("dedx",&dedx);
  outputTree->Branch("angle",&angle);
  outputTree->Branch("dotProduct",&dotProduct);
  outputTree->Branch("neighboursR1",&neighboursR1);
  outputTree->Branch("neighboursR2",&neighboursR2);
  outputTree->Branch("neighboursR3",&neighboursR3);
  outputTree->Branch("chargeR1",&chargeR1);
  outputTree->Branch("chargeR2",&chargeR2);
  outputTree->Branch("chargeR3",&chargeR3);
  outputTree->Branch("pdg",&pdg);
  outputTree->Branch("trackid",&trackid);
  outputTree->Branch("process",&process);

  for(unsigned int h = 0; h < hits.size(); ++h)
  {
    const LArSIMple3DEnergyDeposit &hit = hits.at(h);
    posX.push_back(hit.GetX());
    posY.push_back(hit.GetY());
    posZ.push_back(hit.GetZ());
    posU.push_back(hit.GetU());
    posV.push_back(hit.GetV());
    posW.push_back(hit.GetW());
    const std::vector<float> features = hit.GetFeatures();
    charge.push_back(features.at(0));
    dedx.push_back(features.at(1));
    angle.push_back(features.at(2));
    dotProduct.push_back(features.at(3));
    neighboursR1.push_back(features.at(4));
    neighboursR2.push_back(features.at(5));
    neighboursR3.push_back(features.at(6));
    chargeR1.push_back(features.at(7));
    chargeR2.push_back(features.at(8));
    chargeR3.push_back(features.at(9));
    pdg.push_back(hit.GetParticlePDG());
    trackid.push_back(hit.GetParticleTrackID());
    process.push_back(hit.GetParticleProcess());
  }

  std::cout << "Creating ROOT TTree with " << posX.size() << " hits" << std::endl; 
  outputTree->Fill();

  TFile *outputFile = new TFile(rootFileName.str().c_str(),"recreate");
  outputFile->cd();
  outputTree->Write();
  outputFile->Close();

  outputTree->ResetBranchAddresses();
  delete outputTree;
  delete outputFile;
}

