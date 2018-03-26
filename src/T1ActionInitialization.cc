
#include "T1ActionInitialization.hh"
#include "T1PrimaryGeneratorAction.hh"
#include "T1RunAction.hh"
#include "T1EventAction.hh"
#include "T1SteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1ActionInitialization::T1ActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1ActionInitialization::~T1ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void T1ActionInitialization::BuildForMaster() const
{
  T1RunAction* runAction = new T1RunAction;
  SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void T1ActionInitialization::Build() const
{
  SetUserAction(new T1PrimaryGeneratorAction);

  T1RunAction* runAction = new T1RunAction;
  SetUserAction(runAction);

  T1EventAction* eventAction = new T1EventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new T1SteppingAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
