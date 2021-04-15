#include <vector>
#include <sstream>
#include <fstream>

#include "LArSIMpleOutputWriter.hh"
#include "LArSIMple3DEnergyDeposit.hh"

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
void LArSIMpleOutputWriter::WriteRootFile(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits) const
{
  std::stringstream rootFileName;
  rootFileName << base << "_test_event_" << fEventNumber << ".root";
  TTree *outputTree = new TTree("hits","");
  outputTree->SetDirectory(0);

  float posX, posY, posZ;
  float charge;
  float angle, dotProduct;
  float neighboursR1, neighboursR2, neighboursR3;
  float chargeR1, chargeR2, chargeR3;
  int pdg, trackid;
  outputTree->Branch("x",&posX,"x/F");
  outputTree->Branch("y",&posY,"y/F");
  outputTree->Branch("z",&posZ,"z/F");
  outputTree->Branch("charge",&charge,"charge/F");
  outputTree->Branch("angle",&angle,"angle/F");
  outputTree->Branch("dotProduct",&dotProduct,"dotProduct/F");
  outputTree->Branch("neighboursR1",&neighboursR1,"neighboursR1/F");
  outputTree->Branch("neighboursR2",&neighboursR2,"neighboursR2/F");
  outputTree->Branch("neighboursR3",&neighboursR3,"neighboursR3/F");
  outputTree->Branch("chargeR1",&chargeR1,"chargeR1/F");
  outputTree->Branch("chargeR2",&chargeR2,"chargeR2/F");
  outputTree->Branch("chargeR3",&chargeR3,"chargeR3/F");
  outputTree->Branch("pdg",&pdg,"pdg/I");
  outputTree->Branch("trackid",&trackid,"trackid/I");

  for(unsigned int h = 0; h < hits.size(); ++h)
  {
    const LArSIMple3DEnergyDeposit &hit = hits.at(h);
    posX = hit.GetX();
    posY = hit.GetY();
    posZ = hit.GetZ();
    const std::vector<float> features = hit.GetFeatures();
    charge = features.at(0);
    angle = features.at(1);
    dotProduct = features.at(2);
    neighboursR1 = features.at(3);
    neighboursR2 = features.at(4);
    neighboursR3 = features.at(5);
    chargeR1 = features.at(6);
    chargeR2 = features.at(7);
    chargeR3 = features.at(8);
    pdg = hit.GetParticlePDG();
    trackid = hit.GetParticleTrackID();
    outputTree->Fill();
  }

  TFile *outputFile = new TFile(rootFileName.str().c_str(),"recreate");
  outputFile->cd();
  outputTree->Write();
  outputFile->Close();

  outputTree->ResetBranchAddresses();
  delete outputTree;
  delete outputFile;
}

