#ifndef LArSIMpleOutputWriter_h
#define LArSIMpleOutputWriter_h 1

#include "LArSIMple3DEnergyDeposit.hh"

class LArSIMpleOutputWriter
{  
  public:
  LArSIMpleOutputWriter(unsigned int eventNumber);
  virtual ~LArSIMpleOutputWriter();

  void WriteOutputZipAndInfoFiles(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits) const;
  void WriteRootFile(const std::string &base, const std::vector<LArSIMple3DEnergyDeposit> &hits) const; 

  void SetEventNumber(unsigned int eventNumber) {fEventNumber = eventNumber;};

  private:

  unsigned int fEventNumber;
};

#endif

