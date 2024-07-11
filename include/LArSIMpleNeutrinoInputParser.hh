#ifndef LArSIMpleNeutrinoInputParser_h
#define LArSIMpleNeutrinoInputParser_h

#include "LArSIMpleTrueNeutrinoEvent.hh"
#include <fstream>
#include <string>
#include <vector>

enum class LArSIMpleNeutrinoInputType
{
    kNuanceTracker,
    kGENIETree,
    kGiBUUText,
    kNotSet = 999
};

class LArSIMpleNeutrinoInputParser
{

public:
    LArSIMpleNeutrinoInputParser();
    ~LArSIMpleNeutrinoInputParser();

    void ReadFile(const std::string &filename, const LArSIMpleNeutrinoInputType type);

    unsigned int GetNEvents() const;

    LArSIMpleTrueNeutrinoEvent GetEvent(unsigned int e);

private:
    void ReadFromNuanceTrackerFile(const std::string &filename);
    void ReadFromGENIETreeFile(const std::string &filename);
    void ReadFromGiBUUTextFile(const std::string &filename);
    LArSIMpleNeutrinoInteractionType ConvertNuanceCode(int code);
    LArSIMpleNeutrinoInteractionType ConvertGiBUUCode(int code, bool iscc);
    std::vector<std::string> ReadNuanceTrackerLine(std::ifstream &inFile, int lineSize, char *inBuf);
    std::vector<std::string> ReadGiBUUTextLine(std::ifstream &inFile, int lineSize, char *inBuf);
    std::vector<std::string> TokeniseString(std::string separators, std::string input);
    int GetPDGCodeFromGiBUU(const int pid, const int charge);

    std::vector<LArSIMpleTrueNeutrinoEvent> fNeutrinoEvents;
};

inline unsigned int LArSIMpleNeutrinoInputParser::GetNEvents() const
{
    return fNeutrinoEvents.size();
}

inline LArSIMpleNeutrinoInteractionType LArSIMpleNeutrinoInputParser::ConvertNuanceCode(int code)
{
    // Use simple, broad categories for now
    if (code == 1)
        return LArSIMpleNeutrinoInteractionType::kCCQE;
    if (code == 2)
        return LArSIMpleNeutrinoInteractionType::kNCQE;
    if (code == 91)
        return LArSIMpleNeutrinoInteractionType::kCCDIS;
    if (code == 92)
        return LArSIMpleNeutrinoInteractionType::kNCDIS;
    if (code == 96)
        return LArSIMpleNeutrinoInteractionType::kNCOther;
    if (code == 97)
        return LArSIMpleNeutrinoInteractionType::kCCOther;

    // The neutrino and antineutrino states are seperate, but we will combine here
    if ((code >= 3 && code <= 5) || (code >= 10 && code <= 12))
        return LArSIMpleNeutrinoInteractionType::kCCRES;
    if ((code >= 6 && code <= 9) || (code >= 13 && code <= 16))
        return LArSIMpleNeutrinoInteractionType::kNCRES;

    // There are a few other things, but we can ignore them for now
    return LArSIMpleNeutrinoInteractionType::kDummy;
}

inline LArSIMpleNeutrinoInteractionType LArSIMpleNeutrinoInputParser::ConvertGiBUUCode(int code, bool iscc)
{
    // GiBUU codes:
    // ! * Parameter determines the reaction mechanism and kind of final states
    // ! * Final states are numbered (often IP) by
    // ! * 1: nucleon (QE)
    // ! * 2-31: non-strange baryon resonance (as in IdTable)
    // ! * 32: pi neutron-background  (e.g. nu + n -> mu + pi+ + n)
    // ! * 33: pi proton-background   (e.g. nu + n -> mu + pi0 + p)
    // ! * 34: DIS
    // ! * 35: 2p2h QE
    // ! * 36: 2p2h Delta
    // ! * 37: two pion background

    // QE
    if (code == 1 && iscc)
        return LArSIMpleNeutrinoInteractionType::kCCQE;
    if (code == 1 && !iscc)
        return LArSIMpleNeutrinoInteractionType::kNCQE;
    // Res
    if (code > 1 && code < 34 && iscc)
        return LArSIMpleNeutrinoInteractionType::kCCRES;
    if (code > 1 && code < 34 && !iscc)
        return LArSIMpleNeutrinoInteractionType::kNCRES;
    // DIS
    if (code == 34 && iscc)
        return LArSIMpleNeutrinoInteractionType::kCCDIS;
    if (code == 34 && !iscc)
        return LArSIMpleNeutrinoInteractionType::kNCDIS;
    // Other
    if (code > 34 && code < 38 && iscc)
        return LArSIMpleNeutrinoInteractionType::kCCOther;
    if (code > 34 && code < 38 && !iscc)
        return LArSIMpleNeutrinoInteractionType::kNCOther;

    // There are a few other things, but we can ignore them for now
    return LArSIMpleNeutrinoInteractionType::kDummy;
}

inline std::vector<std::string> LArSIMpleNeutrinoInputParser::ReadNuanceTrackerLine(std::ifstream &inFile, int lineSize, char *inBuf)
{
    // Read in line break it up into tokens
    inFile.getline(inBuf, lineSize);
    return TokeniseString(" $", inBuf);
}

inline std::vector<std::string> LArSIMpleNeutrinoInputParser::ReadGiBUUTextLine(std::ifstream &inFile, int lineSize, char *inBuf)
{
    // Read in line break it up into tokens
    inFile.getline(inBuf, lineSize);
    return TokeniseString(" ", inBuf);
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

                tokens.emplace_back(input.substr(startToken, endToken - startToken));

                startToken = endToken;
            }
        }
    }
    return tokens;
}

inline LArSIMpleTrueNeutrinoEvent LArSIMpleNeutrinoInputParser::GetEvent(unsigned int e)
{
    if (e < fNeutrinoEvents.size())
        return fNeutrinoEvents.at(e);
    else
    {
        std::cerr << "Requested event number beyond the number of neutrino events. Returning empty event." << std::endl;
        return LArSIMpleTrueNeutrinoEvent();
    }
}

inline int LArSIMpleNeutrinoInputParser::GetPDGCodeFromGiBUU(const int pid, const int charge)
{

    // Nucleons
    if (pid == 1 && charge == 1)
        return 2212; // proton
    if (pid == 1 && charge == 0)
        return 2112; // neutron

    // Charged leptons
    if (pid == 901 && charge == -1)
        return 11; // electron
    if (pid == 901 && charge == 1)
        return -11; // positron
    if (pid == 902 && charge == -1)
        return 13; // muon
    if (pid == 902 && charge == 1)
        return -13; // anti muon
    if (pid == 903 && charge == -1)
        return 15; // tau
    if (pid == 903 && charge == 1)
        return -15; // anti tau

    // Neutrinos (neutrino or antineutrino doesn't matter for now)
    if (pid == 911)
        return 12; // electron neutrino
    if (pid == 912)
        return 14; // electron neutrino
    if (pid == 913)
        return 16; // electron neutrino

    // Pions
    if (pid == 101 && charge == 1)
        return 211; // pi plus
    if (pid == 101 && charge == 0)
        return 111; // pi zero
    if (pid == 101 && charge == -1)
        return -211; // pi minus

    // Kaons
    if (pid == 110 && charge == 1)
        return 321; // K plus
    if (pid == 110 && charge == 0)
        return 311; // K zero
    if (pid == 111 && charge == -1)
        return -321; // K minus
    if (pid == 111 && charge == 0)
        return -311; // K zero bar

    // Deltas
    if (pid == 2 && charge == 2)
        return 2224; // delta plus plus
    if (pid == 2 && charge == 1)
        return 2214; // delta plus
    if (pid == 2 && charge == 0)
        return 2114; // delta zero
    if (pid == 2 && charge == -1)
        return 1114; // delta minus

    // Lambda
    if (pid == 32)
        return 3122; // Lambda zero

    // Sigmas
    if (pid == 33 && charge == 1)
        return 3222; // Sigma plus
    if (pid == 33 && charge == 0)
        return 3212; // Sigma zero
    if (pid == 33 && charge == -1)
        return 3112; // Sigma minus

    // Other mesons
    if (pid == 102)
        return 221; // eta
    if (pid == 103 && charge == 1)
        return 213; // rho plus
    if (pid == 103 && charge == 0)
        return 113; // rho zero
    if (pid == 103 && charge == -1)
        return -213; // rho minus
    if (pid == 105)
        return 223; // omega

    return 0;
}

#endif
