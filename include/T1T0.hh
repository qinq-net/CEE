#ifndef T1T0_h
#define T1T0_h 1

#include "G4VUserDetectorConstruction.hh"


#include "G4LogicalVolume.hh"
//#include "G4SystemOfUnits.hh"
//#include "G4PhysicalConstants.hh"
#include "G4Transform3D.hh"


#include "globals.hh"



class T1T0
{
  public:
    T1T0();
    //~T1T0();
    G4LogicalVolume* logicT0;
    G4Transform3D transT0;
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
