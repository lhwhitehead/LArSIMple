/**
 *  @file LArSIMple/src/LArSIMpleNeutrinoInputParser.cc
 * 
 *  @brief Implementation of the neutrino event reading class.
 * 
 *  $Log: $
 */

#include <fstream>

#include "LArSIMpleNeutrinoInputParser.hh"
#include "TChain.h"
#include "TDatabasePDG.h"
#include "TMath.h"

LArSIMpleNeutrinoInputParser::LArSIMpleNeutrinoInputParser()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

LArSIMpleNeutrinoInputParser::~LArSIMpleNeutrinoInputParser()
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleNeutrinoInputParser::ReadFile(const std::string &filename, const LArSIMpleNeutrinoInputType inputType)
{
    if (LArSIMpleNeutrinoInputType::kNuanceTracker == inputType)
        this->ReadFromNuanceTrackerFile(filename);
    else if (LArSIMpleNeutrinoInputType::kGENIETree == inputType)
        this->ReadFromGENIETreeFile(filename);
    else if (LArSIMpleNeutrinoInputType::kGiBUUText == inputType)
        this->ReadFromGiBUUTextFile(filename);
    else
    {
        std::cerr << "Error: neutrino file format not set - " << static_cast<int>(inputType) << std::endl;
        return;
    }
    std::cout << "Read " << fNeutrinoEvents.size() << " from file " << filename << std::endl;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleNeutrinoInputParser::ReadFromNuanceTrackerFile(const std::string &filename)
{

    std::cout << "Reading neutrino events from " << filename << std::endl;
    std::ifstream inputFile(filename.c_str());

    if (!inputFile.is_open())
    {
        std::cerr << "Could not open neutrino input file " << filename << ", returning no neutrino events" << std::endl;
        return;
    }

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
        if (token[0] != "begin")
        {
            std::cout << "Unexpected first line begins with " << token[0] << std::endl;
            break;
        }

        LArSIMpleTrueNeutrinoEvent newEvent;

        // Read the nuance line - the second token is the interaction type
        token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf);
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
        newEvent.AddNeutrino(pos, dir, energy, pdg);

        // Now read the target line
        token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf);
        pdg = std::atoi(token[1].c_str());
        energy = std::atof(token[2].c_str());
        dir = G4ThreeVector(std::atof(token[3].c_str()), std::atof(token[4].c_str()), std::atof(token[5].c_str()));
        newEvent.AddTarget(pos, dir, energy, pdg);

        // Now read the outgoing particles
        while (token = this->ReadNuanceTrackerLine(inputFile, lineSize, inBuf), token[0] == "track")
        {
            // We are only interested in the particles that leave the nucleus, tagged by "0"
            if (token[6] == "0")
            {
                pdg = std::atoi(token[1].c_str());
                energy = std::atof(token[2].c_str());
                dir = G4ThreeVector(std::atof(token[3].c_str()), std::atof(token[4].c_str()), std::atof(token[5].c_str()));
                pos = newEvent.GetInteractionVertex();
                newEvent.AddFinalStateParticle(pos, dir, energy, pdg);
            }
        }

        // We should now have read the "end" line, but let's make sure
        if (token[0] == "end")
        {
            fNeutrinoEvents.emplace_back(newEvent);
        }
        else
        {
            std::cout << "Unexpected line in file... exiting parser" << std::endl;
            return;
        }
    }

    if (inputFile.is_open())
        inputFile.close();
}

//------------------------------------------------------------------------------------------------------------------------------------------

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
    input->SetBranchAddress("nuance_code", &nuanceCode);
    input->SetBranchAddress("vtxx", &vertexX);
    input->SetBranchAddress("vtxy", &vertexY);
    input->SetBranchAddress("vtxz", &vertexZ);

    // The neutrino
    int neutrinoPdg;
    double neutrinoEnergy;
    double neutrinoPx;
    double neutrinoPy;
    double neutrinoPz;
    input->SetBranchAddress("neu", &neutrinoPdg);
    input->SetBranchAddress("Ev", &neutrinoEnergy);
    input->SetBranchAddress("pxv", &neutrinoPx);
    input->SetBranchAddress("pyv", &neutrinoPy);
    input->SetBranchAddress("pzv", &neutrinoPz);

    // Target
    int targetPdg;
    double targetEnergy;
    double targetPx;
    double targetPy;
    double targetPz;
    input->SetBranchAddress("tgt", &targetPdg);
    input->SetBranchAddress("En", &targetEnergy);
    input->SetBranchAddress("pxn", &targetPx);
    input->SetBranchAddress("pyn", &targetPy);
    input->SetBranchAddress("pzn", &targetPz);

    // Leading lepton is considered separately
    int leptonPdg;
    double leptonEnergy;
    double leptonPx;
    double leptonPy;
    double leptonPz;
    input->SetBranchAddress("fspl", &leptonPdg);
    input->SetBranchAddress("El", &leptonEnergy);
    input->SetBranchAddress("pxl", &leptonPx);
    input->SetBranchAddress("pyl", &leptonPy);
    input->SetBranchAddress("pzl", &leptonPz);

    // Final-state hadrons with information stored as arrays. Set this to 100 elements to
    // hopefully avoid anything going wrong
    const unsigned int nAllowedFSPs = 100;
    int nFinalStates;
    int finalPdg[nAllowedFSPs];
    double finalEnergy[nAllowedFSPs];
    double finalPx[nAllowedFSPs];
    double finalPy[nAllowedFSPs];
    double finalPz[nAllowedFSPs];
    input->SetBranchAddress("nf", &nFinalStates);
    input->SetBranchAddress("pdgf", &finalPdg[0]);
    input->SetBranchAddress("Ef", &finalEnergy[0]);
    input->SetBranchAddress("pxf", &finalPx[0]);
    input->SetBranchAddress("pyf", &finalPy[0]);
    input->SetBranchAddress("pzf", &finalPz[0]);

    for (unsigned int e = 0; e < input->GetEntries(); ++e)
    {
        input->GetEntry(e);

        LArSIMpleTrueNeutrinoEvent newEvent;

        // Interaction first
        newEvent.SetInteractionType(this->ConvertNuanceCode(nuanceCode));
        G4ThreeVector vtx(vertexX, vertexY, vertexZ);
        newEvent.SetInteractionVertex(vtx);

        // Neutrino - NB: vtx doesn't make sense for the neutrino, but that's ok
        newEvent.AddNeutrino(vtx, G4ThreeVector(neutrinoPx, neutrinoPy, neutrinoPz).unit(), neutrinoEnergy * 1000., neutrinoPdg);

        // Target
        newEvent.AddTarget(vtx, G4ThreeVector(targetPx, targetPy, targetPz).unit(), targetEnergy * 1000., targetPdg);

        // Leading lepton - this is treated like any other final state particle by LArSIMple
        newEvent.AddFinalStateParticle(vtx, G4ThreeVector(leptonPx, leptonPy, leptonPz).unit(), leptonEnergy * 1000., leptonPdg);

        // Final state hadrons
        if (nFinalStates > static_cast<int>(nAllowedFSPs))
            nFinalStates = static_cast<int>(nAllowedFSPs);

        for (unsigned int h = 0; h < static_cast<unsigned int>(nFinalStates); ++h)
            newEvent.AddFinalStateParticle(vtx, G4ThreeVector(finalPx[h], finalPy[h], finalPz[h]).unit(), finalEnergy[h] * 1000., finalPdg[h]);

        fNeutrinoEvents.emplace_back(newEvent);
    }

    input->ResetBranchAddresses();
    delete input;
    input = nullptr;
}

//------------------------------------------------------------------------------------------------------------------------------------------

void LArSIMpleNeutrinoInputParser::ReadFromGiBUUTextFile(const std::string &filename)
{
    TDatabasePDG *database = new TDatabasePDG();

    std::cout << "Reading GiBUU neutrino events from " << filename << std::endl;
    std::ifstream inputFile(filename.c_str());

    if (!inputFile.is_open())
    {
        std::cerr << "Could not open neutrino input file " << filename << ", returning no neutrino events" << std::endl;
        return;
    }

    const int lineSize = 250;
    char inBuf[lineSize];
    std::vector<std::string> token = this->ReadGiBUUTextLine(inputFile, lineSize, inBuf);

    // Check we haven't reached the end of the input file
    if (token.size() == 0)
    {
        std::cout << "Error in GiBUU text file!" << std::endl;
        return;
    }

    // The first line in the file contains a comment starting with #
    if (token[0] != "#")
    {
        std::cout << "Unexpected first line begins with " << token[0] << std::endl;
        return;
    }

    // We want to keep reading until we reach the end of the file. There is no delimiter between events, so we have to keep track
    // of the event number (token two)
    int currentEvent{1};

    // Event loop
    while (true)
    {
        //    std::cout << "Processing new event: " << fNeutrinoEvents.size() << std::endl;
        // Create a new neutrino event ready for filling
        LArSIMpleTrueNeutrinoEvent newEvent;
        int lineNumber = 1;
        bool finishedProcessing{false};

        // Particle loop
        // Line format: run   event   type   charge   weight   x   y   z   E   px   py   pz   history   interactionType   E_nu
        while (true)
        {
            token = this->ReadGiBUUTextLine(inputFile, lineSize, inBuf);
            // If this line is empty then we need to stop processing
            if (token.size() == 0)
            {
                if (lineNumber == 1)
                {
                    finishedProcessing = true;
                    break;
                }
                else
                {
                    fNeutrinoEvents.emplace_back(newEvent);
                    break;
                }
            }

            // If the event changes then we need to store the event and move on
            if (std::atoi(token.at(1).c_str()) != currentEvent)
            {
                //        std::cout << "Finished reading event " << currentEvent << " with " << lineNumber-1 << " particles" << std::endl;
                fNeutrinoEvents.emplace_back(newEvent);
                ++currentEvent;
                lineNumber = 1;
                newEvent = LArSIMpleTrueNeutrinoEvent();
            }

            // First line is the leading lepton
            if (lineNumber == 1)
            {
                // Interaction first
                const int leptonPdg = GetPDGCodeFromGiBUU(std::atoi(token.at(2).c_str()), std::atoi(token.at(3).c_str()));
                newEvent.SetInteractionType(this->ConvertGiBUUCode(std::atoi(token.at(13).c_str()), leptonPdg % 2));

                const G4ThreeVector vtx(std::atof(token.at(5).c_str()), std::atof(token.at(6).c_str()), std::atof(token.at(7).c_str()));
                newEvent.SetInteractionVertex(vtx);

                // Neutrino - we don't have a line for this, but make a dummy momentum vector using E_nu
                const G4ThreeVector nuMom(0.0, 0.0, std::atof(token.at(14).c_str()) * 1000.);
                // GiBUU doesn't give us the neutrino flavour, but we can get it using the leading lepton
                const int neutrinoPdg = leptonPdg % 2 == 0 ? leptonPdg : (leptonPdg > 0 ? leptonPdg + 1 : leptonPdg - 1);
                newEvent.AddNeutrino(vtx, nuMom.unit(), std::atof(token.at(14).c_str()) * 1000., neutrinoPdg);

                // Now for the leading lepton
                const G4ThreeVector leptonMom(std::atof(token.at(9).c_str()), std::atof(token.at(10).c_str()), std::atof(token.at(11).c_str()));
                newEvent.AddFinalStateParticle(vtx, leptonMom.unit(), std::atof(token.at(8).c_str()) * 1000., leptonPdg);
            }
            // Second line is the target nucleon
            else if (lineNumber == 2)
            {
                const G4ThreeVector vtx(newEvent.GetInteractionVertex());
                const int targetPdg = GetPDGCodeFromGiBUU(std::atoi(token.at(2).c_str()), std::atoi(token.at(3).c_str()));
                const G4ThreeVector targetMom(std::atof(token.at(9).c_str()), std::atof(token.at(10).c_str()), std::atof(token.at(11).c_str()));

                newEvent.AddTarget(vtx, targetMom.unit(), std::atof(token.at(8).c_str()) * 1000., targetPdg);
                //        std::cout << "Target momentum = " << targetMom << std::endl;
            }
            // All other lines are just final state particles that we want to keep
            else
            {
                const G4ThreeVector vtx(newEvent.GetInteractionVertex());
                const int fspPdg = GetPDGCodeFromGiBUU(std::atoi(token.at(2).c_str()), std::atoi(token.at(3).c_str()));
                const G4ThreeVector fspMom(std::atof(token.at(9).c_str()), std::atof(token.at(10).c_str()), std::atof(token.at(11).c_str()));

                // Occasionally find neutrons with energy slightly less that their rest mass? Try fixing it
                float energy = std::atof(token.at(8).c_str());
                if (fspPdg == 2112)
                {
                    const float mass = database->GetParticle(fspPdg)->Mass();
                    if (energy < mass)
                    {
                        std::cout << "Energy less than rest mass... calculating using momentum vector" << std::endl;
                        energy = TMath::Sqrt(fspMom.mag2() + mass * mass);
                    }
                }
                newEvent.AddFinalStateParticle(vtx, fspMom.unit(), energy * 1000., fspPdg);
                //        std::cout << "Final state particle momentum = " << fspMom << std::endl;
            }
            ++lineNumber;
        } // End particle loop

        if (finishedProcessing)
            break;
    } // End event loop
}
