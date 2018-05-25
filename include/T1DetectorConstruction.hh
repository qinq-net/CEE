#ifndef T1DetectorConstruction_h
#define T1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "T1TPC.hh"
#include "T1MWDC.hh"
#include "T1ZDC.hh"
#include "T1T0.hh"
#include "T1iTOF.hh"
#include "T1eTOF.hh"

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
    T1ZDC* CEE_ZDC;
    T1T0* CEE_T0;
    T1iTOF* CEE_iTOF;
    T1eTOF* CEE_eTOF;
    virtual void SetupDetectors();
    virtual void SetupField();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
