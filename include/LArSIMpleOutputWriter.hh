#ifndef LArSIMpleOutputWriter_h
#define LArSIMpleOutputWriter_h

#include "LArSIMple3DEnergyDeposit.hh"

class LArSIMpleTrueNeutrinoEvent;

class LArSIMpleOutputWriter
{  
  public:
  LArSIMpleOutputWriter(unsigned int eventNumber);
  virtual ~LArSIMpleOutputWriter();

  void WriteOutputZipAndInfoFiles(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits) const;
  void WriteRootFile(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits, const LArSIMpleTrueNeutrinoEvent *evt) const; 

  void SetEventNumber(unsigned int eventNumber) {fEventNumber = eventNumber;};

  private:

  unsigned int fEventNumber;
};

#endif

