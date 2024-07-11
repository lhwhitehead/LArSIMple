#include <fstream>
#include <sstream>
#include <vector>

#include "LArSIMple3DEnergyDeposit.hh"
#include "LArSIMpleOutputWriter.hh"
#include "LArSIMpleTrueNeutrinoEvent.hh"

#include "TFile.h"
#include "TTree.h"

#include "zlib.h"

LArSIMpleOutputWriter::LArSIMpleOutputWriter(unsigned int eventNumber) :
    fEventNumber(eventNumber)
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
        const std::vector<float> flatHit = hit.GetFlatRepresentation();
        flatVec.insert(flatVec.end(), flatHit.begin(), flatHit.end());
    }

    // Write the output using zlib
    const ulong src_len = flatVec.size() * sizeof(float); // pixelArray length
    ulong dest_len = compressBound(src_len);              // calculate size of the compressed data
    const char *outputStream = (char *)malloc(dest_len);  // allocate memory for the compressed data
    const int res = compress((Bytef *)outputStream, &dest_len, (Bytef *)&flatVec[0], src_len);

    // Buffer error

    if (res == Z_BUF_ERROR)
        std::cout << "Buffer too small!" << std::endl;
    // Memory error
    else if (res == Z_MEM_ERROR)
        std::cout << "Not enough memory for compression!" << std::endl;
    // Compression ok
    else
    {
        std::cout << "Hooray... writing files" << std::endl;

        // Create output files
        std::stringstream image_file_name;
        image_file_name << base << "_test_event_" << fEventNumber << ".gz";

        std::ofstream image_file(image_file_name.str(), std::ofstream::binary);
        if (image_file.is_open())
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
    if (infoFile.is_open())
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
void LArSIMpleOutputWriter::WriteRootFile(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits,
    const LArSIMpleTrueNeutrinoEvent *evt, const std::vector<double> &wireAngles) const
{
    std::stringstream rootFileName;
    rootFileName << base << "_event_" << fEventNumber << ".root";
    TTree *outputTree = new TTree("hits", "");
    outputTree->SetDirectory(0);

    // Each event consists the basic event information and a number of vectors

    // Event level information
    int eventNumber;
    outputTree->Branch("eventNumber", &eventNumber, "eventNumber/I");
    eventNumber = fEventNumber;

    // Specific for neutrino events
    int neutrinoPdg{0};
    float neutrinoEnergy{0.f};
    int nFinalStates{0};
    int interactionType{0};
    float interactionVtxX{0.f}, interactionVtxY{0.f}, interactionVtxZ{0.f};
    if (nullptr != evt)
    {
        outputTree->Branch("neutrinoPdg", &neutrinoPdg, "neutrinoPdg/I");
        outputTree->Branch("neutrinoEnergy", &neutrinoEnergy, "neutrinoEnergy/F");
        outputTree->Branch("nFinalStates", &nFinalStates, "nFinalStates/I");
        outputTree->Branch("interactionType", &interactionType, "interactionType/I");

        outputTree->Branch("interactionVtxX", &interactionVtxX, "interactionVtxX/F");
        outputTree->Branch("interactionVtxY", &interactionVtxY, "interactionVtxY/F");
        outputTree->Branch("interactionVtxZ", &interactionVtxZ, "interactionVtxZ/F");

        neutrinoPdg = evt->GetNeutrinoPDGCode();
        neutrinoEnergy = evt->GetNeutrinoEnergy();
        nFinalStates = evt->GetNumberOfFinalStateParticles();
        interactionType = static_cast<int>(evt->GetInteractionType());

        const G4ThreeVector interactionVtx = evt->GetInteractionVertex();
        interactionVtxX = interactionVtx.x();
        interactionVtxY = interactionVtx.y();
        interactionVtxZ = interactionVtx.z();
    }

    // Wire angles
    float wireAngleU = wireAngles.at(0);
    float wireAngleV = wireAngles.at(1);
    float wireAngleW = wireAngles.at(2);

    outputTree->Branch("wireAngleU", &wireAngleU, "wireAngleU/F");
    outputTree->Branch("wireAngleV", &wireAngleV, "wireAngleV/F");
    outputTree->Branch("wireAngleW", &wireAngleW, "wireAngleW/F");

    // Hit level information
    std::vector<float> posX, posY, posZ;
    std::vector<float> posU, posV, posW;
    std::vector<float> charge, dedx;
    std::vector<float> angle, dotProduct;
    std::vector<float> neighboursR1, neighboursR2, neighboursR3;
    std::vector<float> chargeR1, chargeR2, chargeR3;
    std::vector<int> pdg, trackid, process;

    outputTree->Branch("x", &posX);
    outputTree->Branch("y", &posY);
    outputTree->Branch("z", &posZ);
    outputTree->Branch("u", &posU);
    outputTree->Branch("v", &posV);
    outputTree->Branch("w", &posW);
    outputTree->Branch("charge", &charge);
    outputTree->Branch("dedx", &dedx);
    outputTree->Branch("angle", &angle);
    outputTree->Branch("dotProduct", &dotProduct);
    outputTree->Branch("neighboursR1", &neighboursR1);
    outputTree->Branch("neighboursR2", &neighboursR2);
    outputTree->Branch("neighboursR3", &neighboursR3);
    outputTree->Branch("chargeR1", &chargeR1);
    outputTree->Branch("chargeR2", &chargeR2);
    outputTree->Branch("chargeR3", &chargeR3);
    outputTree->Branch("pdg", &pdg);
    outputTree->Branch("trackid", &trackid);
    outputTree->Branch("process", &process);

    for (unsigned int h = 0; h < hits.size(); ++h)
    {
        const LArSIMple3DEnergyDeposit &hit = hits.at(h);

        const G4ThreeVector &xyzPos = hit.GetPosition();
        posX.emplace_back(xyzPos.getX());
        posY.emplace_back(xyzPos.getY());
        posZ.emplace_back(xyzPos.getZ());

        const G4ThreeVector &uvwPos = hit.GetUVWPosition();
        posU.emplace_back(uvwPos.getX());
        posV.emplace_back(uvwPos.getY());
        posW.emplace_back(uvwPos.getZ());

        const std::vector<float> features = hit.GetFeatures();
        charge.emplace_back(features.at(0));
        dedx.emplace_back(features.at(1));
        angle.emplace_back(features.at(2));
        dotProduct.emplace_back(features.at(3));
        neighboursR1.emplace_back(features.at(4));
        neighboursR2.emplace_back(features.at(5));
        neighboursR3.emplace_back(features.at(6));
        chargeR1.emplace_back(features.at(7));
        chargeR2.emplace_back(features.at(8));
        chargeR3.emplace_back(features.at(9));
        pdg.emplace_back(hit.GetParticlePDG());
        trackid.emplace_back(hit.GetParticleTrackID());
        process.emplace_back(hit.GetParticleProcess());
    }

    std::cout << "Creating ROOT TTree with " << posX.size() << " hits" << std::endl;
    outputTree->Fill();

    TFile *outputFile = new TFile(rootFileName.str().c_str(), "recreate");
    outputFile->cd();
    outputTree->Write();
    outputFile->Close();

    outputTree->ResetBranchAddresses();
    delete outputTree;
    delete outputFile;
}
