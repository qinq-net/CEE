
#include "T1SteppingAction.hh"
#include "T1EventAction.hh"
#include "T1DetectorConstruction.hh"

#include "G4Step.hh"
//#include "G4StepPoint.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1SteppingAction::T1SteppingAction(T1EventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1SteppingAction::~T1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void T1SteppingAction::UserSteppingAction(const G4Step* aStep)
{



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
