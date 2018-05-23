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
  private:
    // key:boxID
    std::map<G4String,G4LogicalVolume*> logicMWDCbox[6];
    // key:boxID+":"+chamberID
    std::map<G4String,G4LogicalVolume*> logicMWDCboxChamber[6];
  public:
    inline std::vector<G4LogicalVolume*> GetSensitiveLVs(G4int MWDC_id)
    {
        if(MWDC_id<1 || MWDC_id>6) return {};
        std::vector<G4LogicalVolume*> sensList = {};
        for(auto chamberItr: logicMWDCboxChamber[MWDC_id-1])
        {
            sensList.push_back(chamberItr.second);
        }
        return sensList;
    }
    
};



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
