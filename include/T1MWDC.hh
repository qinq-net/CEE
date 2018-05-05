#ifndef T1MWDC_h
#define T1MWDC_h 1

#include "G4VUserDetectorConstruction.hh"


#include "G4LogicalVolume.hh"
//#include "G4SystemOfUnits.hh"
//#include "G4PhysicalConstants.hh"
#include "G4Transform3D.hh"


#include "globals.hh"



class T1MWDC
{
  public:
    T1MWDC();
    //~T1MWDC();
    void T1MWDC_0(int MWDC_id);
    void T1MWDC_exampledata(int MWDC_id);

    G4LogicalVolume* logicMWDC[6];
    G4Transform3D transMWDC[6];
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
