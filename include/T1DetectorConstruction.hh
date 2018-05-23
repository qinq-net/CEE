#ifndef T1DetectorConstruction_h
#define T1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "T1TPC.hh"
#include "T1MWDC.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class T1DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    T1DetectorConstruction();
    virtual ~T1DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  protected:
    G4LogicalVolume*  fScoringVolume;

  private:
    T1TPC* CEE_TPC;
    T1MWDC* CEE_MWDC;
    virtual void SetupDetectors();
    virtual void SetupField();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
