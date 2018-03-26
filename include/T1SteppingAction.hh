
#ifndef T1SteppingAction_h
#define T1SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class T1EventAction;

class G4LogicalVolume;

/// Stepping action class
///

class T1SteppingAction : public G4UserSteppingAction
{
  public:
    T1SteppingAction(T1EventAction* eventAction);
    virtual ~T1SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    T1EventAction*  fEventAction;
    G4LogicalVolume* fScoringVolume;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
