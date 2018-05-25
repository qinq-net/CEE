#ifndef T1eTOF_h
#define T1eTOF_h 1

#include "G4VUserDetectorConstruction.hh"


#include "G4LogicalVolume.hh"
//#include "G4SystemOfUnits.hh"
//#include "G4PhysicalConstants.hh"
#include "G4Transform3D.hh"


#include "globals.hh"



class T1eTOF
{
  public:
    T1eTOF();
    //~T1eTOF();
    void T1eTOF_0(int eTOF_id);
    void T1eTOF_exampledata(int eTOF_id);

    G4LogicalVolume* logiceTOF[2];
    G4Transform3D transeTOF[2];

  private:
    std::vector<G4LogicalVolume*> SLVList[2];

  public:
    inline std::vector<G4LogicalVolume*> GetSensitiveLVs(G4int id=0)
    {
      return SLVList[id];
    }
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
