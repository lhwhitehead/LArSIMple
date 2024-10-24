/**
 *  @file LArSIMple/src/LArSIMpleOutputWriter.cc
 * 
 *  @brief Implementation of the output writer class.
 * 
 *  $Log: $
 */

#include <fstream>
#include <sstream>
#include <vector>
#include <set>

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

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleOutputWriter::~LArSIMpleOutputWriter()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleOutputWriter::WriteRootFile(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits,
    const LArSIMpleTrueNeutrinoEvent *evt, std::map<int, LArSIMpleTrackData> &trueTracks, const std::vector<double> &wireAngles) const
{
    // This is very specific and hardcoded
    TTree *outputTree = new TTree("hits", "");
    outputTree->SetDirectory(0);

    const bool useHitFeatures{!(hits.at(0).GetFeatures().empty())};

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

    if (useHitFeatures)
    {
        outputTree->Branch("angle", &angle);
        outputTree->Branch("dotProduct", &dotProduct);
        outputTree->Branch("neighboursR1", &neighboursR1);
        outputTree->Branch("neighboursR2", &neighboursR2);
        outputTree->Branch("neighboursR3", &neighboursR3);
        outputTree->Branch("chargeR1", &chargeR1);
        outputTree->Branch("chargeR2", &chargeR2);
        outputTree->Branch("chargeR3", &chargeR3);
    }

    outputTree->Branch("pdg", &pdg);
    outputTree->Branch("trackid", &trackid);
    outputTree->Branch("process", &process);

    std::set<int> hitTrackIDs;
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

        charge.emplace_back(hit.GetEnergy());
        dedx.emplace_back(hit.GetDeDx());

        if (useHitFeatures)
        {
            const std::vector<float> features = hit.GetFeatures();
            angle.emplace_back(features.at(0));
            dotProduct.emplace_back(features.at(1));
            neighboursR1.emplace_back(features.at(2));
            neighboursR2.emplace_back(features.at(3));
            neighboursR3.emplace_back(features.at(4));
            chargeR1.emplace_back(features.at(5));
            chargeR2.emplace_back(features.at(6));
            chargeR3.emplace_back(features.at(7));
        }

        pdg.emplace_back(hit.GetParticlePDG());
        trackid.emplace_back(hit.GetParticleTrackID());
        process.emplace_back(hit.GetParticleProcess());

        if(!hitTrackIDs.count(hit.GetParticleTrackID()))
            hitTrackIDs.insert(hit.GetParticleTrackID());
    }

    // True track information
    std::vector<float> trackVtxPosX, trackVtxPosY, trackVtxPosZ, trackVtxPosU, trackVtxPosV, trackVtxPosW;
    std::vector<float> trackVtxDirX, trackVtxDirY, trackVtxDirZ, trackVtxDirU, trackVtxDirV, trackVtxDirW;
    std::vector<float> trackVtxKE, trackVtxMomentum;

    std::vector<float> trackEndPosX, trackEndPosY, trackEndPosZ, trackEndPosU, trackEndPosV, trackEndPosW;
    std::vector<float> trackEndDirX, trackEndDirY, trackEndDirZ, trackEndDirU, trackEndDirV, trackEndDirW;
    std::vector<float> trackEndKE, trackEndMomentum;

    std::vector<int> trackPDG;
    std::vector<int> trackID;
    std::vector<float> trackMass;
    std::vector<int> trackIsPrimary;

    outputTree->Branch("trueTrackVtxPosX", &trackVtxPosX);
    outputTree->Branch("trueTrackVtxPosY", &trackVtxPosY);
    outputTree->Branch("trueTrackVtxPosZ", &trackVtxPosZ);
    outputTree->Branch("trueTrackVtxPosU", &trackVtxPosU);
    outputTree->Branch("trueTrackVtxPosV", &trackVtxPosV);
    outputTree->Branch("trueTrackVtxPosW", &trackVtxPosW);
    outputTree->Branch("trueTrackVtxDirX", &trackVtxDirX);
    outputTree->Branch("trueTrackVtxDirY", &trackVtxDirY);
    outputTree->Branch("trueTrackVtxDirZ", &trackVtxDirZ);
    outputTree->Branch("trueTrackVtxDirU", &trackVtxDirU);
    outputTree->Branch("trueTrackVtxDirV", &trackVtxDirV);
    outputTree->Branch("trueTrackVtxDirW", &trackVtxDirW);
    outputTree->Branch("trueTrackVtxKE", &trackVtxKE);
    outputTree->Branch("trueTrackVtxMomentum", &trackVtxMomentum);

    outputTree->Branch("trueTrackEndPosX", &trackEndPosX);
    outputTree->Branch("trueTrackEndPosY", &trackEndPosY);
    outputTree->Branch("trueTrackEndPosZ", &trackEndPosZ);
    outputTree->Branch("trueTrackEndPosU", &trackEndPosU);
    outputTree->Branch("trueTrackEndPosV", &trackEndPosV);
    outputTree->Branch("trueTrackEndPosW", &trackEndPosW);
    outputTree->Branch("trueTrackEndDirX", &trackEndDirX);
    outputTree->Branch("trueTrackEndDirY", &trackEndDirY);
    outputTree->Branch("trueTrackEndDirZ", &trackEndDirZ);
    outputTree->Branch("trueTrackEndDirU", &trackEndDirU);
    outputTree->Branch("trueTrackEndDirV", &trackEndDirV);
    outputTree->Branch("trueTrackEndDirW", &trackEndDirW);
    outputTree->Branch("trueTrackEndKE", &trackEndKE);
    outputTree->Branch("trueTrackEndMomentum", &trackEndMomentum);

    outputTree->Branch("trueTrackPDG", &trackPDG);
    outputTree->Branch("trueTrackID", &trackID);
    outputTree->Branch("trueTrackMass", &trackMass);
    outputTree->Branch("trueTrackIsPrimary", &trackIsPrimary);

    for (const auto &trackPair : trueTracks)
    {
        if (!hitTrackIDs.count(trackPair.first))
            continue;

        G4ThreeVector tempVec{trackPair.second.GetVertexPosition()};
        trackVtxPosX.emplace_back(tempVec.getX());
        trackVtxPosY.emplace_back(tempVec.getY());
        trackVtxPosZ.emplace_back(tempVec.getZ());
        trackVtxPosU.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleU));
        trackVtxPosV.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleV));
        trackVtxPosW.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleW));

        tempVec = trackPair.second.GetVertexDirection();
        trackVtxDirX.emplace_back(tempVec.getX());
        trackVtxDirY.emplace_back(tempVec.getY());
        trackVtxDirZ.emplace_back(tempVec.getZ());
        trackVtxDirU.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleU));
        trackVtxDirV.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleV));
        trackVtxDirW.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleW));

        tempVec = trackPair.second.GetEndPosition();
        trackEndPosX.emplace_back(tempVec.getX());
        trackEndPosY.emplace_back(tempVec.getY());
        trackEndPosZ.emplace_back(tempVec.getZ());
        trackEndPosU.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleU));
        trackEndPosV.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleV));
        trackEndPosW.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleW));

        tempVec = trackPair.second.GetEndDirection();
        trackEndDirX.emplace_back(tempVec.getX());
        trackEndDirY.emplace_back(tempVec.getY());
        trackEndDirZ.emplace_back(tempVec.getZ());
        trackEndDirU.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleU));
        trackEndDirV.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleV));
        trackEndDirW.emplace_back(this->ConvertYZToWireCoordinate(tempVec.getY(), tempVec.getZ(), wireAngleW));

        trackVtxKE.emplace_back(trackPair.second.GetVertexKineticEnergy());
        trackVtxMomentum.emplace_back(trackPair.second.GetVertexMomentum());
        trackEndKE.emplace_back(trackPair.second.GetEndKineticEnergy());
        trackEndMomentum.emplace_back(trackPair.second.GetEndMomentum());

        trackPDG.emplace_back(trackPair.second.GetPDG());
        trackID.emplace_back(trackPair.first);
        trackMass.emplace_back(trackPair.second.GetMass());
        trackIsPrimary.emplace_back(trackPair.second.IsPrimary());
    }

    std::cout << "Creating ROOT TTree with " << posX.size() << " hits" << std::endl;
    outputTree->Fill();

    std::stringstream rootFileName;
    rootFileName << base << "_event_" << fEventNumber << ".root";
    TFile *outputFile = new TFile(rootFileName.str().c_str(), "recreate");
    outputFile->cd();
    outputTree->Write();
    outputFile->Close();

    outputTree->ResetBranchAddresses();
    delete outputTree;
    delete outputFile;
}
