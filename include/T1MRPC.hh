#ifndef T1MRPC_h
#define T1MRPC_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


class T1MRPC
{
  public:
    T1MRPC(G4int qixishu,G4double d1,G4double,d2);
    ~T1MRPC();
    G4LogicalVolume* logicMRPC;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
