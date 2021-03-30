#include "LArSIMpleDetectorConstruction.hh"
#include "LArSIMpleDetectorMessenger.hh"
//#include "LArSIMpleSensitiveDetector.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"
#include "G4Tubs.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4EmSaturation.hh"
#include "G4LossTableManager.hh"
#include "G4Region.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include <iostream>
#include <fstream>
#include <sstream>

LArSIMpleDetectorConstruction::LArSIMpleDetectorConstruction() :
   fSolidWorld(nullptr),fLogicWorld(nullptr),fPhysiWorld(nullptr),
   fSolidLArBox(nullptr),fLogicLArBox(nullptr),fPhysiLArBox(nullptr)
{
  fDetectorMessenger = new LArSIMpleDetectorMessenger(this);

}


LArSIMpleDetectorConstruction::~LArSIMpleDetectorConstruction()
{}
  
  
G4VPhysicalVolume* LArSIMpleDetectorConstruction::Construct()
{
  this->DefineMaterials(); 
  this->ConstructWorld();
  this->ConstructLArBox(); 

  return fPhysiWorld;
}

void LArSIMpleDetectorConstruction::DefineMaterials()
{
  // Try using the NIST database for our materials
  G4NistManager* manager = G4NistManager::Instance();

  // Define the elements that we need
//  const G4Element* nitrogen = manager->FindOrBuildElement(7,true);
//  const G4Element* oxygen = manager->FindOrBuildElement(8,true);
//  const G4Element* argon = manager->FindOrBuildElement(18,true);

//  fMaterialAir = manager->ConstructNewMaterial("Air", std::vector<G4String>({nitrogen->GetName(),oxygen->GetName()}),
//                                               std::vector<double>({0.7,0.3}),1.290*CLHEP::mg/CLHEP::cm3, true);
//  fMaterialLAr = manager->ConstructNewMaterial("LAr",std::vector<G4String>({argon->GetName()}),
//                                               std::vector<double>({1.0}),1400*CLHEP::kg/CLHEP::m3,true);
  fMaterialAir = manager->FindOrBuildMaterial("G4_AIR");
  fMaterialLAr = manager->FindOrBuildMaterial("G4_lAr");
  manager->PrintG4Material("G4_lAr");
//  fMaterialAir = new G4Material("Air"  , density=1.290*CLHEP::mg/CLHEP::cm3, ncomponents=2);
//  fMaterialAir->AddElement(nitrogen, 0.7);
//  fMaterialAir->AddElement(oxygen, 0.3);
//
//  fMaterialLAr = new G4Material("LAr"  , density=1400*CLHEP::kg/CLHEP::m3, ncomponents=1);
//  fMaterialLAr->AddElement(argon, 1.0);
}

void LArSIMpleDetectorConstruction::ConstructWorld()
{
  // World - remember we need half-lengths to define the box
  fSolidWorld = new G4Box("World", fWorldSizeX/2.f, fWorldSizeY/2.f, fWorldSizeZ/2.f);
  fLogicWorld = new G4LogicalVolume(fSolidWorld, fMaterialAir, "World");
  fPhysiWorld = new G4PVPlacement(0, G4ThreeVector(), fLogicWorld, "World", 0, false, 0, fCheckOverlaps);
}
  
void LArSIMpleDetectorConstruction::ConstructLArBox()
{
  // LAr volume
  fSolidLArBox = new G4Box("LArBox", fLArSizeX/2.f, fLArSizeY/2.f, fLArSizeZ/2.f);
  fLogicLArBox = new G4LogicalVolume(fSolidLArBox, fMaterialLAr, "LArBox");
  fPhysiLArBox = new G4PVPlacement(0, G4ThreeVector(), fLogicLArBox, "LArBox", fLogicWorld, false, 0, fCheckOverlaps);
}

void LArSIMpleDetectorConstruction::PrintDetectorSummary() const
{
  std::cout << "Constructed LAr volume of size" << fLArSizeX << " X " << fLArSizeY << " X " << fLArSizeZ << std::endl;
}

//void LArSIMpleDetectorConstruction::DefineScintillatorDimensions()
//{
//  // Define scintillator and plate parameters (half lengths)
//
//  //ScintX = 128.*CLHEP::mm; //standard
//  //ScintY = 128.*CLHEP::mm; //standard
//
//  ScintX = 500.*CLHEP::mm; //for 250 MeV beam simulations, lateral profile
//  ScintY = 500.*CLHEP::mm;
//
//  //ScintZ = 60.*CLHEP::mm; //for comparison with Sarah for 3.5 MeV neutrons
//  //ScintZ = 50.*CLHEP::mm;
//  ScintZ = 50.*CLHEP::cm; //for 300MeV protons
//
//  PlateZ = 7.7*CLHEP::mm; // aluminium plate, 7.7 mm to fully stop 60 MeV protons
//  //PMMAZ = 0.*CLHEP::mm; // PMMA absorber, 13.7 mm to fully stop 60 MeV protons //use when defining PMMA in here (rather than control file)
//  StainlessSteelZ = 3.0*CLHEP::mm; // stailness steel collimator
//  //MylarZ = 0.006*CLHEP::mm; //mylar on front face of scintillator, total thickness = 12 micro metres  
//  MylarZ = 10.*0.006*CLHEP::mm; //increase mylar thickness by 10 (so that GEANT actually interacts) Remember to decrease density by 10!
//  //MylarZ = 10.*0.5*CLHEP::mm; //To test whether increasing size/decreasing density is acceptable! Use 1mm (total) with original density and 10mm with /10 density.
//
//  HardboardX = 152.4*CLHEP::mm; //hardboard x = 304.8mm;
//  HardboardY = 192.9*CLHEP::mm; //hardboard y = 385.8mm;
//  HardboardZ = 2.5*CLHEP::mm; //hardboard thickness = 5mm;
//
//  BrassZ = 50*CLHEP::mm; // brass collimator thickness = 10 cm
//
//  /*
//  // Define scintillator and plate parameters (full lengths)
//  ScintX = 256.*CLHEP::mm;
//  ScintY = 256.*CLHEP::mm;
//  ScintZ = 120.*CLHEP::mm;
//
//  PlateZ = 15.4*CLHEP::mm; // aluminium plate, 15.4 mm to fully stop 60 MeV protons
//  PMMAZ = 27.4*CLHEP::mm; // PMMA absorber, 27.4 mm to fully stop 60 MeV protons
//  StainlessSteelZ = 6.0*CLHEP::mm; // stailness steel collimator, 6.0 mm to full stop 60 MeV protons
//  */
//  ComputeCalorParameters();
//}
//
//
//void LArSIMpleDetectorConstruction::DefineMaterials()
//{ 
//  //Define materials
//  
//  G4String symbol;             //a=mass of a mole;
//  G4double a, z, density;      //z=mean number of protons;  
//  G4int iz, n;                 //iz=number of protons  in an isotope; 
//  // n=number of nucleons in an isotope;
//  
//  G4int ncomponents, natoms;
//  G4double abundance, fractionmass;
//  
//  //
//  // define Elements
//  //
//  
//  G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*CLHEP::g/CLHEP::mole);
//  G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*CLHEP::g/CLHEP::mole);
//  G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*CLHEP::g/CLHEP::mole);
//  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*CLHEP::g/CLHEP::mole);
//  G4Element* Si = new G4Element("Silicon" ,symbol="Si" , z= 14., a= 28.09*CLHEP::g/CLHEP::mole);
//  G4Element* Cr = new G4Element("Chromium",symbol="Cr", z= 24., a= 52.00*CLHEP::g/CLHEP::mole);
//  G4Element* Mn = new G4Element("Manganese",symbol="Mn",z= 25., a= 54.94*CLHEP::g/CLHEP::mole);
//  G4Element* Fe = new G4Element("Iron"    ,symbol="Fe", z= 26., a= 55.85*CLHEP::g/CLHEP::mole);
//  G4Element* Ni = new G4Element("Nickel"  ,symbol="Ni", z= 28., a= 58.69*CLHEP::g/CLHEP::mole);
//  G4Element* B = new G4Element("Boron"    , symbol="B", z = 5., a = 10.811*CLHEP::g/CLHEP::mole);
//  G4Element* Na = new G4Element("Sodium"  , symbol="Na",z = 11., a = 22.99*CLHEP::g/CLHEP::mole);
//  G4Element* Al = new G4Element("Aluminium",symbol="Al",z = 13., a = 26.98*CLHEP::g/CLHEP::mole);
//  G4Element* Cu = new G4Element("Copper", symbol = "Cu",z = 29., a = 63.546*CLHEP::g/CLHEP::mole);
//  G4Element* Zn = new G4Element("Zinc", symbol = "Zn", z = 30., a = 65.38*CLHEP::g/CLHEP::mole);
//  
//  //
//  // define simple materials
//  //
//  
//  Aluminium = new G4Material("Aluminium", z=13., a=26.98*CLHEP::g/CLHEP::mole, density=2.700*CLHEP::g/CLHEP::cm3);
//  
//  //
//  // define a material from elements
//  //
//  
//  H2O = 
//    new G4Material("Water", density= 1.000*CLHEP::g/CLHEP::cm3, ncomponents=2);
//  H2O->AddElement(H, natoms=2);
//  H2O->AddElement(O, natoms=1);
//  // overwrite computed meanExcitationEnergy with ICRU recommended value 
//  //H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*CLHEP::eV);
//  
//  Brass = 
//    new G4Material("Brass", density = 8.57*CLHEP::g/CLHEP::cm3, ncomponents = 2);
//  Brass->AddElement(Cu, natoms = 3);
//  Brass->AddElement(Zn, natoms = 2);
//
//  scintillatorMaterial = 
//    new G4Material("Scintillator", density= 1.023*CLHEP::g/CLHEP::cm3, ncomponents=2);
//  scintillatorMaterial->AddElement(C, natoms=9);
//  scintillatorMaterial->AddElement(H, natoms=10);
//  
//  // Scintillator constants definitions
//
//  // Definitions for wavelength shifting  (full_popop_emission.cfg)
//  const G4int wlsEmEntries = 501;
//  
//  G4double wlsWavelength[wlsEmEntries];
//  G4double wlsEnergy[wlsEmEntries];
//  G4double wlsEmit[wlsEmEntries];
//
//  ReadInFile("simulation_input_files/full_popop_emission.cfg", wlsWavelength, wlsEmit, CLHEP::mm);
//  ConvertToEnergy(wlsEmEntries, wlsWavelength, wlsEnergy, CLHEP::eV);
// 
//  // Definitions for the WLS absorption spectrum
//  const G4int wlsAbEntries = 501; 
// 
//  G4double wlsAbsorb[wlsAbEntries];
//
//  ReadInFile("simulation_input_files/full_popop_emission.cfg", wlsWavelength, wlsEmit, CLHEP::mm);
//  ConvertToEnergy(wlsEmEntries, wlsWavelength, wlsEnergy, CLHEP::eV);
//
//
//  // Definitions for refraction index (PST_ref_index.dat)
//  const G4int ref_index_Entries = 11;
//
//  G4double ref_index_Wavelength[ref_index_Entries];
//  G4double ref_index_Energy[ref_index_Entries];
//  G4double ref_index_value[ref_index_Entries];
//
//  ReadInFile("simulation_input_files/PST_ref_index.dat", ref_index_Wavelength, ref_index_value, 1.);
//  ConvertToEnergy(ref_index_Entries, ref_index_Wavelength, ref_index_Energy, CLHEP::eV);
// 
//  //Definitions for bulk absorption spectrum (PSTBulkAbsorb.cfg)
//  const G4int absorbEntries = 501;
//  
//  G4double absorbWavelength[absorbEntries];
//  G4double absorbEnergy[absorbEntries];
//  G4double Absorb[absorbEntries];
//
//  ReadInFile("simulation_input_files/PSTBulkAbsorb.cfg", absorbWavelength, Absorb, CLHEP::m);
//  ConvertToEnergy(absorbEntries, absorbWavelength, absorbEnergy, CLHEP::eV);
// 
//  // Other definitions
//  G4double lightyield = 10000;
//  G4double scintRes = 1;
//  G4double scintFastconst = 0.9*CLHEP::ns;
//  G4double scintSlowconst = 2.1*CLHEP::ns;
//
//  const G4int nEntries = 32;
//  
//  G4double photonEnergy [nEntries] =
//    {2.034*CLHEP::eV, 2.068*CLHEP::eV, 2.103*CLHEP::eV, 2.139*CLHEP::eV,
//     2.177*CLHEP::eV, 2.216*CLHEP::eV, 2.256*CLHEP::eV, 2.298*CLHEP::eV,
//     2.341*CLHEP::eV, 2.286*CLHEP::eV, 2.433*CLHEP::eV, 2.481*CLHEP::eV,
//     2.532*CLHEP::eV, 2.585*CLHEP::eV, 2.640*CLHEP::eV, 2.697*CLHEP::eV,
//     2.757*CLHEP::eV, 2.820*CLHEP::eV, 2.885*CLHEP::eV, 2.954*CLHEP::eV,
//     3.026*CLHEP::eV, 3.102*CLHEP::eV, 3.181*CLHEP::eV, 3.265*CLHEP::eV,
//     3.353*CLHEP::eV, 3.446*CLHEP::eV, 3.545*CLHEP::eV, 3.649*CLHEP::eV,
//     3.760*CLHEP::eV, 3.877*CLHEP::eV, 4.002*CLHEP::eV, 4.136*CLHEP::eV};
//
//  G4double scintFast [nEntries] = 
//    {1., 1., 1., 1., 1.,
//     1., 1., 1., 1., 1., 
//     1., 1., 1., 1., 1., 
//     1., 1., 1., 1., 1., 
//     1., 1., 1., 1., 1., 
//     1., 1., 1., 1., 1., 1., 1.};
//
//  G4double scintSlow [nEntries] =
//    {0.01, 1.00, 2.00, 3.00, 4.00, 
//     5.00, 6.00, 7.00, 8.00, 9.00, 
//     8.00, 7.00, 6.00, 4.00, 3.00, 
//     2.00, 1.00, 0.01, 1.00, 2.00, 
//     3.00, 4.00, 5.00, 6.00, 7.00, 
//     8.00, 9.00, 8.00, 7.00, 6.00, 5.00, 4.00};
//
//  G4MaterialPropertiesTable* scintMLArSIMple = new G4MaterialPropertiesTable();
//
//  //scintMLArSIMple->AddProperty("FASTCOMPONENT", photonEnergy, scintFast, nEntries);
//  //scintMLArSIMple->AddProperty("SLOWCOMPONENT", photonEnergy, scintSlow, nEntries);
//
//  scintMLArSIMple->AddProperty("WLSCOMPONENT",wlsEnergy,wlsEmit,wlsEmEntries);
//  scintMLArSIMple->AddProperty("WLSABSLENGTH",wlsEnergy,wlsAbsorb,wlsAbEntries);   // the WLS absorption spectrum
//  scintMLArSIMple->AddConstProperty("WLSTIMECONSTANT",12*CLHEP::ns);
//  scintMLArSIMple->AddProperty("RINDEX",ref_index_Energy,ref_index_value,ref_index_Entries);
//
//  scintMLArSIMple->AddProperty("ABSLENGTH",absorbEnergy,Absorb,absorbEntries);    // the bulk absorption spectrum
//  scintMLArSIMple->AddConstProperty("SCINTILLATIONYIELD",lightyield/CLHEP::MeV);
//  scintMLArSIMple->AddConstProperty("PROTONSCINTILLATIONYIELD",lightyield/CLHEP::MeV);
//  scintMLArSIMple->AddConstProperty("RESOLUTIONSCALE",scintRes);
//  scintMLArSIMple->AddConstProperty("FASTTIMECONSTANT",scintFastconst);
//  scintMLArSIMple->AddConstProperty("SLOWTIMECONSTANT",scintSlowconst);
//  scintMLArSIMple->AddConstProperty("YIELDRATIO",0.8);
//
//  scintillatorMaterial->SetMaterialPropertiesTable(scintMLArSIMple); 
//  
//  // Set Birks Constant and create an instance of Birks Law
//  scintillatorMaterial->GetIonisation()->SetBirksConstant(0.207*CLHEP::mm/CLHEP::MeV);
//  G4EmSaturation* EmSaturation = G4LossTableManager::Instance()->EmSaturation();
//  G4double birksConstant = EmSaturation->FindG4BirksCoefficient(scintillatorMaterial);
//  std::cout << "Birk's constant: " << birksConstant << std::endl;
//  
//  //Define the mylar
//  /*
//    G4int mylar_entries = 0;
//    G4double mylar_energy[500];
//    G4double mylar_reflect[500];
//    G4double realzero[500];
//    G4double one[500];
//    
//    std::ifstream Read_mylar;
//    G4String mylar_file = "simulation_input_files/mylar.dat";
//    
//    Read_mylar.open(mylar_file);
//    if (Read_mylar.is_open()){
//      while(!Read_mylar.eof()){
//	G4String filler;
//	G4double wavelength;
//	G4double mylar_ref_coeff;
//	Read_mylar >> wavelength >> filler >> mylar_ref_coeff;
//	mylar_energy[mylar_entries] = (1240/wavelength)*CLHEP::eV;
//	mylar_reflect[mylar_entries] = mylar_ref_coeff;
//	realzero[mylar_entries] = 0.0;
//	one[mylar_entries] = 1.0;
//	mylar_entries++;
//      }
//    }
//    else
//      G4cout << "Error opening file: " << mylar_file << G4endl; 
//    Read_mylar.close();
//    
//    op_mylar = new G4OpticalSurface("op_mylar");
//    op_mylar->SetModel(glisur);
//    op_mylar->SetFinish(polished);
//    op_mylar->SetType(dielectric_metal);
//    
//    mylarMLArSIMple = new G4MaterialPropertiesTable();
//    mylarMLArSIMple->AddProperty("REFLECTIVITY",mylar_energy,mylar_reflect,mylar_entries);
//    mylarMLArSIMple->AddProperty("SPECULARLOBECONSTANT",mylar_energy,one,mylar_entries);
//    mylarMLArSIMple->AddProperty("SPECULARSPIKECONSTANT",mylar_energy,one,mylar_entries);
//    mylarMLArSIMple->AddProperty("DIFFUSELOBECONSTANT",mylar_energy,realzero,mylar_entries);
//    mylarMLArSIMple->AddProperty("BACKSCATTERCONSTANT",mylar_energy,realzero,mylar_entries);
//    
//    op_mylar->SetMaterialPropertiesTable(mylarMLArSIMple);
//    b_mylar = new G4LogicalSkinSurface("b_mylar",logical_mylar,op_mylar);
//  */
//
//  Myl =  
//    new G4Material("Mylar", density= 1.397/10.*CLHEP::g/CLHEP::cm3, ncomponents=3); //increase mylar thickness by 10 (so that mylar actually interacts) and decrease mylar density by 10!
//  Myl->AddElement(C, natoms=10);
//  Myl->AddElement(H, natoms= 8);
//  Myl->AddElement(O, natoms= 4);
//  
//  Hardboard =
//    new G4Material("Hardboard", density = 0.205*CLHEP::g/CLHEP::cm3, ncomponents = 2);
//  Hardboard->AddElement(C, natoms = 8);
//  Hardboard->AddElement(H, natoms = 8);
//  
//  PMMA =
//    new G4Material("PMMA", density = 1.18*CLHEP::g/CLHEP::cm3, ncomponents = 3);
//  PMMA->AddElement(C, natoms = 5);
//  PMMA->AddElement(O, natoms = 2);
//  PMMA->AddElement(H, natoms = 8);
//  
//  //
//  // define a material from elements.   case 2: mixture by fractional mass
//  //
//  
//  Air = 
//    new G4Material("Air"  , density= 1.290*CLHEP::mg/CLHEP::cm3, ncomponents=2);
//  Air->AddElement(N, fractionmass=0.7);
//  Air->AddElement(O, fractionmass=0.3);
//  
//
//  StainlessSteel =
//    new G4Material("StainlessSteel", density = 8.06*CLHEP::g/CLHEP::cm3, ncomponents=6);
//  StainlessSteel->AddElement(C, fractionmass=0.001);
//  StainlessSteel->AddElement(Si, fractionmass=0.007);
//  StainlessSteel->AddElement(Cr, fractionmass=0.18);
//  StainlessSteel->AddElement(Mn, fractionmass=0.01);
//  StainlessSteel->AddElement(Fe, fractionmass=0.712);
//  StainlessSteel->AddElement(Ni, fractionmass=0.09);
//  
//  //
//  // examples of vacuum
//  //
//  
//  G4Material* Vacuum =
//    new G4Material("Galactic", z=1., a=1.01*CLHEP::g/CLHEP::mole,density= CLHEP::universe_mean_density,
//		   kStateGas, 2.73*CLHEP::kelvin, 3.e-18*CLHEP::pascal);
//  
//  // print table
//  //
//  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
//
//  //PMTs
//
//  const G4int PMTarray = 3;
//
//  // Glass
//  G4Material* glass = new G4Material("glass", 2.21*CLHEP::g/CLHEP::cm3, ncomponents = 2);
//  glass->AddElement(Si, 1);
//  glass->AddElement(O, 2);
//
//  G4double glass_PP[PMTarray] = {1.0*CLHEP::eV, 6.69*CLHEP::eV, 7.50*CLHEP::eV};
//  G4double glass_RIND[PMTarray] = {1.69, 1.69, 1.69};
//  G4double glass_ABSL[PMTarray] = {999.0*CLHEP::cm, 999.0*CLHEP::cm, 999.0*CLHEP::cm};
//
//  G4MaterialPropertiesTable* glassMLArSIMple = new G4MaterialPropertiesTable();
//  glassMLArSIMple->AddProperty("RINDEX", glass_PP, glass_RIND, PMTarray);
//  glassMLArSIMple->AddProperty("ABSLENGTH", glass_PP, glass_ABSL, PMTarray);
//  glass->SetMaterialPropertiesTable(glassMLArSIMple);
//
//  // Borosilicate glass (bglass)
//  G4Material* bglass = new G4Material("bglass", 2.21*CLHEP::g/CLHEP::cm3, ncomponents = 5);
//  bglass->AddElement(O, 0.54);
//  bglass->AddElement(Si, 0.38);
//  bglass->AddElement(B, 0.04);
//  bglass->AddElement(Na, 0.03);
//  bglass->AddElement(Al, 0.01);
//
//  G4double bglass_PP[PMTarray] = {1.0*CLHEP::eV, 6.69*CLHEP::eV, 7.50*CLHEP::eV};
//  G4double bglass_RIND[PMTarray] = {1.49, 1.49, 1.49};
//  G4double bglass_ABSL[PMTarray] = {999.0*CLHEP::cm, 999.0*CLHEP::cm, 999.0*CLHEP::cm};
//
//  G4MaterialPropertiesTable* bglassMLArSIMple = new G4MaterialPropertiesTable();
//  bglassMLArSIMple->AddProperty("RINDEX",bglass_PP, bglass_RIND, PMTarray);
//  bglassMLArSIMple->AddProperty("ABSLENGTH", bglass_PP, bglass_ABSL, PMTarray);
//  bglass->SetMaterialPropertiesTable(bglassMLArSIMple);
//
//  // Photocathode
//  // 94% absorption, 6% reflection
//  G4Material* photocathode = new G4Material("photocathode", 1., 1.*CLHEP::g/CLHEP::mole, 1.e-20*CLHEP::g/CLHEP::cm3, kStateSolid);
//  
//  G4double nf = 1.;
//  G4double phc_RIND[PMTarray] = {glass_RIND[0]*nf, glass_RIND[1]*nf, glass_RIND[2]*nf};
//  G4double phc_ABSL[PMTarray] = {1.e-20*CLHEP::m, 1.e-20*CLHEP::m, 1.e-20*CLHEP::m};
//  G4double phc_REFL[PMTarray] = {0.06, 0.06, 0.06};
//  G4double phc_EFF[PMTarray] = {1.00, 1.00, 1.00};
//
//  G4MaterialPropertiesTable* phcMLArSIMple = new G4MaterialPropertiesTable();
//  phcMLArSIMple->AddProperty("RINDEX", glass_PP, phc_RIND, PMTarray);
//  phcMLArSIMple->AddProperty("ABSLENGTH", glass_PP, phc_ABSL, PMTarray);
//  phcMLArSIMple->AddProperty("REFLECTIVITY", glass_PP, phc_REFL, PMTarray);
//  phcMLArSIMple->AddProperty("EFFICIENCY", glass_PP, phc_EFF, PMTarray);
//  photocathode->SetMaterialPropertiesTable(phcMLArSIMple);
//
//  // Optical Grease (BC630, n = 1.46)
//  G4Material* grease = new G4Material("grease", 1.06*CLHEP::g/CLHEP::cm3, ncomponents = 2);
//  grease->AddElement(H, 1);
//  grease->AddElement(C, 1);
//
//  const G4int greaseArray = 3;
//
//  G4double grease_PP[greaseArray] = {1.0*CLHEP::eV, 6.69*CLHEP::eV, 7.50*CLHEP::eV};
//  G4double grease_RIND[greaseArray] = {1.46, 1.46, 1.46};
//  G4double grease_ABSL[greaseArray] = {999.0*CLHEP::cm, 999.0*CLHEP::cm, 999.0*CLHEP::cm};
//
//  G4MaterialPropertiesTable* greaseMLArSIMple = new G4MaterialPropertiesTable();
//  greaseMLArSIMple->AddProperty("RINDEX", grease_PP, grease_RIND, greaseArray);
//  greaseMLArSIMple->AddProperty("ABSLENGHT", grease_PP, grease_ABSL, greaseArray);
//  grease->SetMaterialPropertiesTable(greaseMLArSIMple);
//
//  //Assign materials to global detector component material names
//  
//  defaultMaterial = Vacuum;
//  pmtMaterial = bglass;
//  photocathodeMaterial = photocathode;
//  greaseMaterial = grease;
//}
//
//G4VPhysicalVolume* LArSIMpleDetectorConstruction::ConstructCalorimeter()
//{
//  
//  // Clean old geometry, if any
//  //
//  G4GeometryManager::GetInstance()->OpenGeometry();
//  G4PhysicalVolumeStore::GetInstance()->Clean();
//  G4LogicalVolumeStore::GetInstance()->Clean();
//  G4SolidStore::GetInstance()->Clean();
//  
//  
//  // Bool for checking volume overlaps
//
//  G4bool checkOverlaps = true;
//
//  //     
//  // World
//  //
//  solidWorld = new G4Box("World",				//its name
//  			 fWorldSizeX/2,fWorldSizeY/2,fWorldSizeZ/2);	//its size
//
//  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
//                                   //defaultMaterial,	//its material
//                                   Air,
//				   "World");		//its name
//  
//  physiWorld = new G4PVPlacement(0,			//no rotation
//  				 G4ThreeVector(),	//at (0,0,0)
//                                 logicWorld,		//its logical volume				 
//                                 "World",		//its name
//                                 0,			//its mother  volume
//                                 false,			//no boolean operation
//				 0,                     //copy number
//				 checkOverlaps);        //overlaps checking
// 
//  G4cout << "\n------------------------------------------------------------"
//         << "\n---> The size of the World is: "
//         << fWorldSizeX << " x " << fWorldSizeY << " x " <<  fWorldSizeZ<< " mm." 
//         << "\n------------------------------------------------------------\n";
//
//  //
//  // Aluminium plate
//  //
//  /*
//  G4double alZPlace = -(ScintZ + PlateZ + 0.1*CLHEP::mm);
//  G4cout << alZPlace << " mm, z placement of aluminium plate" << G4endl; 
//
//  solidPlate = 0; logicPlate = 0; physiPlate = 0;
//
//  solidPlate = new G4Box("Plate",
//			 ScintX, ScintY, PlateZ);
//
//  logicPlate = new G4LogicalVolume(solidPlate,
//				   Aluminium,
//				   "Plate");
//
//  physiPlate = new G4PVPlacement(0,
//				 //G4ThreeVector(0,0,-65.*CLHEP::mm),
//				 G4ThreeVector(0,0,alZPlace*CLHEP::mm),
//				 logicPlate,
//				 "Plate",
//				 logicWorld,
//				 false,
//				 0,
//				 checkOverlaps);
//  */
//  //
//  // PMMA plate
//  //
//  /*
//  //G4double pmmaZPlace = -(ScintZ + MylarZ + PMMAZ + 0.1*CLHEP::mm); //with Mylar
//  G4double pmmaZPlace = -(ScintZ + PMMAZ + 0.1*CLHEP::mm); // no Mylar
//  //G4double pmmaZPlace = -1800.*CLHEP::mm; // to reproduce PMMA plate being 1.8 m upstream of the calorimeter module
//  G4cout << "PMMA Z (1/2 thickness) = " << PMMAZ << G4endl;
//  G4cout << pmmaZPlace << " mm, z placement of PMMA plate" << G4endl; 
//  
//  solidPMMA = 0; logicPMMA = 0; physiPMMA = 0;
//  
//  solidPMMA = new G4Box("PMMAPlate",
//			ScintX, ScintY, PMMAZ);
//
//  logicPMMA = new G4LogicalVolume(solidPMMA,
//				  PMMA,
//				  "PMMAPlate");
//
//  physiPMMA = new G4PVPlacement(0,
//				//G4ThreeVector(0,0,-0.9*CLHEP::m),
//				G4ThreeVector(0,0,pmmaZPlace*CLHEP::mm),
//				logicPMMA,
//				"PMMAPlate",
//				logicWorld,
//				false,
//				0,
//				checkOverlaps);
//  */
//  // Stainless Steel plate
//  //
//  /*
//  G4double ssZPlace = -(ScintZ + StainlessSteelZ + 0.1*CLHEP::mm);
//  G4cout << ssZPlace << " mm, z placement of stainless steel collimator" << G4endl; 
//
//  solidSSPlate = 0; logicSSPlate = 0; physiSSPlate = 0;
//
//  solidSSPlate = new G4Box("SSPlate",
//			 ScintX, ScintY, StainlessSteelZ);
//
//  logicSSPlate = new G4LogicalVolume(solidSSPlate,
//                                     StainlessSteel,
//				     "StainlessSteelPlate");
//
//  physiSSPlate = new G4PVPlacement(0,
//				 G4ThreeVector(0,0,ssZPlace*CLHEP::mm),
//				 logicSSPlate,
//				 "StainlessSteelPlate",
//				 logicWorld,
//				 false,
//				 0,
//				 checkOverlaps);
//  */
//  //
//  			  
//  //                               
//  // Scintillator 
//  //  
//  solidScint=0; logicScint=0; physiScint=0;
//  
//  solidScint = new G4Box("Scintillator",       //its name
//			 ScintX,ScintY,ScintZ);//size
//  
//  logicScint = new G4LogicalVolume(solidScint,	        //its solid
//				   scintillatorMaterial,//its material
//				   "Scintillator");	//its name
//  
//  physiScint = new G4PVPlacement(0,			//no rotation
//				 G4ThreeVector(),	//at (0,0,0)
//				 logicScint,	//its logical volume
//				 "Scintillator",//its name
//				 logicWorld,	//its mother  volume
//				 false,		//no boolean operation
//				 0,             //copy number
//				 checkOverlaps);//check overlaps
//  /*
//  //
//  //Water Phantom
//  //
//  solidWater=0; logicWater=0; physiWater=0;
//
//  solidWater = new G4Box("WaterPhantom",       //its name
//			 ScintX,ScintY,ScintZ);//size
//  
//  logicWater = new G4LogicalVolume(solidWater,	        //its solid
//				   H2O,                 //its material
//				   "WaterPhantom");	//its name
//  
//  physiWater = new G4PVPlacement(0,			//no rotation
//				 G4ThreeVector(),	//at (0,0,0)
//				 logicWater,	//its logical volume
//				 "WaterPhantom",//its name
//				 logicWorld,	//its mother  volume
//				 false,		//no boolean operation
//				 0,             //copy number
//				 checkOverlaps);//check overlaps
//
//  */
//  //
//  // Mylar
//  //
//  /*
//  G4double mylarZPlace = -(ScintZ + MylarZ + 0.05*CLHEP::mm);
//  G4cout << mylarZPlace << " mm, z placement of Mylar" << G4endl; 
//  
//  solidMylar = 0; logicMylar = 0; physiMylar = 0;
//  
//  solidMylar = new G4Box("AlMylar",
//			ScintX, ScintY, MylarZ);
//
//  logicMylar = new G4LogicalVolume(solidMylar,
//				  Myl,
//				  "AlMylar");
//
//  physiMylar = new G4PVPlacement(0,
//				G4ThreeVector(0,0,mylarZPlace*CLHEP::mm),
//				logicMylar,
//				"AlMylar",
//				logicWorld,
//				false,
//				0,
//				checkOverlaps);
//  */
//  //
//  // Black hardboard
//  //
//  /*
//  G4double boardZPlace = -(ScintZ + HardboardZ + 50.*CLHEP::mm);
//  G4cout << boardZPlace << " mm, z placement of Hardboard" << G4endl;
//
//  solidBoard = 0; logicBoard = 0; physiBoard = 0;
//  
//  solidBoard = new G4Box("BlackHardboard", 
//			 HardboardX, HardboardY, HardboardZ);
//  
//  logicBoard = new G4LogicalVolume(solidBoard,
//				   Hardboard,
//				   "BlackHardboard");
//
//  physiBoard = new G4PVPlacement(0,
//			     G4ThreeVector(0,0,boardZPlace*CLHEP::mm),
//			     logicBoard,
//			     "BlackHardboard",
//			     logicWorld,
//			     false,
//			     0,
//			     checkOverlaps);
//  
//  */
//
//  // Brass collimator/plate
//  /*
//  //G4double brassZPlace = -(ScintZ + MylarZ + BrassZ + 0.1*CLHEP::mm); //with Mylar
//  G4double brassZPlace = -(ScintZ + BrassZ + 0.1*CLHEP::mm); // no Mylar
//  //G4double pmmaZPlace = -1800.*CLHEP::mm; // to reproduce PMMA plate being 1.8 m upstream of the calorimeter module
//  G4cout  << brassZPlace << " mm, z placement of brass collimator" << G4endl;
//  
//  solidBrass = 0; logicBrass = 0; physiBrass = 0;
//  
//  solidBrass = new G4Box("BrassCollimator",
//			ScintX, ScintY, BrassZ);
//
//  logicBrass = new G4LogicalVolume(solidBrass,
//				  Brass,
//				  "BrassCollimator");
//
//  physiBrass = new G4PVPlacement(0,
//				 //G4ThreeVector(0,0,-0.9*CLHEP::m),
//				 G4ThreeVector(0,0,brassZPlace*CLHEP::mm),
//				 logicBrass,
//				 "BrassCollimator",
//				 logicWorld,
//				 false,
//				 0,
//				 checkOverlaps);
//  */
//  //
//  // PMT (future PMT)
//  //
//  // PMT tube (glass)
//  /*
//  solidPMT=0; logicPMT=0; physiPMT=0;
//
//  G4double innerRadius = 0.*CLHEP::mm;
//  G4double outerRadius = 101.6*CLHEP::mm; //8 inch PMT diameter = 20.32 cm, radius = 10.16 cm
//  G4double pmtZ = 250.*CLHEP::mm; //defined as full length (unlike ScintZ)
//  G4double startAngle = 0.*CLHEP::deg;
//  G4double spanningAngle = 360.*CLHEP::deg; 
//
//  G4double greaseThickness = 0.50*CLHEP::mm; //required here for placement of PMT, defined as full length
//
//  //G4double PMTZPlace = (ScintZ + pmtZ/2 + 0.1*CLHEP::mm); //with air gap
//  G4double PMTZPlace = (ScintZ + pmtZ/2 + greaseThickness); //no air gap
//  G4cout << "Z position of PMT tube within the world volume: " << PMTZPlace << " mm" << G4endl;
//  
//  solidPMT = new G4Tubs("pmtTube", innerRadius/2., outerRadius/2., pmtZ/2., startAngle, spanningAngle);
//
//  G4RotationMatrix* PMTrotate = new G4RotationMatrix;
//  PMTrotate->rotateX(90*CLHEP::deg);
//
//  logicPMT = new G4LogicalVolume(solidPMT,
//				 pmtMaterial,
//				 "pmtTube");
//
//  physiPMT = new G4PVPlacement(0,
//			       G4ThreeVector(0.*CLHEP::mm, 0.*CLHEP::mm, PMTZPlace),
//			       logicPMT,
//			       "pmtTube",
//			       logicWorld,
//			       false,
//			       0,
//			       checkOverlaps);
//  
//
//  // PMT photocathode
//  
//  solidCathode = 0; logicCathode = 0; physiCathode = 0;
//
//  G4double phcRadius = 98.*CLHEP::mm; //full length
//  G4double glassThickness = 4.0*CLHEP::mm; //full length
//  G4double phcThickness = 0.10*CLHEP::mm; //full length
//  G4double phcZPlace = -((pmtZ/2.) - glassThickness - phcThickness/2.);
//
//  //G4double phcZPlace = (ScintZ + phcThickness/2 + 0.2*CLHEP::mm); // 0.1mm for air gap between scintillator and PMT, 
//                                                                   //0.1mm to ensure photocathode is behind the PMT glass
//  
//  G4cout << "Z position of photocathode within PMT tube: " << phcZPlace << " mm" << G4endl;
//
//  solidCathode = new G4Tubs("pmtCathode", innerRadius/2., phcRadius/2., phcThickness/2., startAngle, spanningAngle);
//
//  logicCathode = new G4LogicalVolume(solidCathode,
//				    photocathodeMaterial,
//				    "pmtCathode");
//
//  physiCathode = new G4PVPlacement(0,
//				   G4ThreeVector(0.*CLHEP::mm, 0.*CLHEP::mm, phcZPlace),
//				   logicCathode,
//				   "pmtCathode",
//				   logicPMT,
//				   false,
//				   0,
//				   checkOverlaps);
//  
//  // PMT vacuum
//  
//  solidPMTVac = 0; logicPMTVac = 0; physiPMTVac = 0;
//
//  G4double vacuumZPlace = (glassThickness + phcThickness)/2.;
//  G4cout << "Z position of vacuum within PMT tube: " << vacuumZPlace << " mm" << G4endl;
//
//  solidPMTVac = new G4Tubs("pmtVacuum", innerRadius/2., (outerRadius/2. - glassThickness), (pmtZ/2. - glassThickness - phcThickness), startAngle, spanningAngle);
//
//  logicPMTVac = new G4LogicalVolume(solidPMTVac,
//				    defaultMaterial,
//				    "pmtVacuum");
//
//  physiPMTVac = new G4PVPlacement(0,
//				  G4ThreeVector(0.*CLHEP::mm, 0.*CLHEP::mm, vacuumZPlace),
//				  logicPMTVac,
//				  "pmtVacuum",
//				  logicPMT,
//				  false,
//				  0,
//				  checkOverlaps);
//  
//  // Optical grease
//  
//  solidGrease = 0; logicGrease = 0; physiGrease = 0;
//
//  G4double greaseZPlace = (ScintZ + greaseThickness/2.); //no air gap
//  G4cout << "Z position of grease within the world volume: " << greaseZPlace << " mm" << G4endl;
//
//  solidGrease = new G4Tubs("grease", innerRadius/2., outerRadius/2., greaseThickness/2., startAngle, spanningAngle);
//
//  logicGrease = new G4LogicalVolume(solidGrease,
//				    greaseMaterial,
//				    "grease");
//
//  physiGrease = new G4PVPlacement(0,
//				  G4ThreeVector(0.*CLHEP::mm, 0*CLHEP::mm, greaseZPlace),
//				  logicGrease,
//				  "grease",
//				  logicWorld,
//				  false,
//				  0,
//				  checkOverlaps);
//  */	
//
//  //
//  // Define regions (to set different cuts for different regions)
//  //
//  
//  //mylar = new G4Region("mylar");
//  //mylar->AddRootLogicalVolume(logicMylar);
//		    
//  //
//  // Define sensitive detector 
//  //
//  G4SDManager* SDman = G4SDManager::GetSDMpointer();
//  G4String name="SD";
//  DetectorSD = new LArSIMpleSensitiveDetector(name);
//  SDman->AddNewDetector(DetectorSD);
//  //logicScint->SetSensitiveDetector(DetectorSD);
//  //logicPMT->SetSensitiveDetector(DetectorSD);
//  //logicCathode->SetSensitiveDetector(DetectorSD);
//  
//  PrintCalorParameters();     
//  
//  return physiWorld;
//}
//
//void LArSIMpleDetectorConstruction::PrintCalorParameters()
//{
//  G4cout << "\n------------------------------------------------------------"
//         << "\n---> The calorimeter consists of a scintillator block of "
//         << ScintX*2 << " x " << ScintY*2 << " x " << ScintZ*2 << " mm." 
//         << "\n------------------------------------------------------------\n";
//}
//
//
//#include "G4RunManager.hh"
//
//void LArSIMpleDetectorConstruction::UpdateGeometry()
//{
//  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
//}
//
//void LArSIMpleDetectorConstruction::ReadInFile(std::string filename, G4double wavelength[], G4double value[], G4double scaleFactor){
//
//  // Load the file
//  std::ifstream infile(filename.c_str(),std::ifstream::in);
//  
//  // now define a string into which we temporarily read the file.
//  std::string tempString = "";
//
//  // Jut need a counter for the array index
//  G4int counter = 0;
//
//  while(getline(infile,tempString)){
//    
//    std::stringstream tempStream;
//    
//    std::cout << "Read: " << tempString << std::endl;	
//    
//    tempStream << tempString;
//    tempStream >> wavelength[counter] >> value[counter];
//    value[counter]*=scaleFactor;
//    
//    std::cout << "Filled arrays: " << wavelength[counter] << ", " << value[counter] << std::endl;
//    
//    ++counter;
//  }
//  infile.close();
//}
//
//void LArSIMpleDetectorConstruction::ConvertToEnergy(G4int entries, G4double wavelength[], G4double energy[], G4double scaleFactor){
//  
//  G4double metres = 1e-9;           // to convert nm to metres
//  G4double h = 6.62606957e-34;      // Planck's constant, m^2.kg/s
//  G4double c = 2.99792458e8;        // Speed of light, m/s
//  G4double joules = 1.60217657e-19; // To convert J to eV
//
//  for (G4int i = 0; i < entries; i++) {
//    energy[i] = (((h*c)/(wavelength[i]*metres))/joules)*scaleFactor;
//  
//    std::cout << i << ": Filled energy array with " << energy[i] << " MeV for " << wavelength[i] << " nm." << std::endl;
//  }
//}
