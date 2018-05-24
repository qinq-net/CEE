#ifndef T1ZDC_h
#define T1ZDC_h 1

#include "G4VUserDetectorConstruction.hh"


#include "G4LogicalVolume.hh"
//#include "G4SystemOfUnits.hh"
//#include "G4PhysicalConstants.hh"
#include "G4Transform3D.hh"


#include "globals.hh"



class T1ZDC
{
  public:
    T1ZDC();
    //~T1ZDC();
    //void T1ZDC_0(int ZDC_id);
    void T1ZDC_exampledata();

    G4LogicalVolume* logicZDC;
    G4Transform3D transZDC;
  private:
    std::map<G4String, std::vector<G4LogicalVolume*>> sensLVs;
  public:
    inline std::vector<G4LogicalVolume*> GetSensitiveLVs(G4String towerID)
    { return sensLVs[towerID]; }
    inline std::map<G4String, std::vector<G4LogicalVolume*>> GetSensitiveLVs()
    { return sensLVs; }
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
