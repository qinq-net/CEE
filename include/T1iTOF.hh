#ifndef T1iTOF_h
#define T1iTOF_h 1

#include "G4VUserDetectorConstruction.hh"


#include "G4LogicalVolume.hh"
//#include "G4SystemOfUnits.hh"
//#include "G4PhysicalConstants.hh"
#include "G4Transform3D.hh"


#include "globals.hh"



class T1iTOF
{
  public:
    T1iTOF();
    //~T1iTOF();
    void T1iTOF_0(int iTOF_id);
    void T1iTOF_exampledata(int iTOF_id);

    G4LogicalVolume* logiciTOF[2];
    G4Transform3D transiTOF[2];

  private:
    std::vector<G4LogicalVolume*> SLVList;

  public:
    inline std::vector<G4LogicalVolume*> GetSensitiveLVs()
    {
      return SLVList;
    }
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
