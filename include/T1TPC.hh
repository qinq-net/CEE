#ifndef T1TPC_h
#define T1TPC_h 1

#include "G4VUserDetectorConstruction.hh"


#include "G4LogicalVolume.hh"
//#include "G4SystemOfUnits.hh"
//#include "G4PhysicalConstants.hh"
#include "G4Transform3D.hh"
#include <G4PVPlacement.hh>


#include "globals.hh"



class T1TPC
{
  public:
    T1TPC();
    //~T1T0();
    G4LogicalVolume* logicTPC;
    G4Transform3D transTPC;

  private:
    G4LogicalVolume* logicTPCbox;
    G4LogicalVolume* logicTPCboxchamber;

    G4PVPlacement* physTPCbox;
    G4PVPlacement* physTPCchamber;
    std::vector<G4LogicalVolume*> SLVList;


  public:
    inline std::vector<G4LogicalVolume*> GetSensitiveLVs(G4int id=0)
    {
      return SLVList;
    }
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
