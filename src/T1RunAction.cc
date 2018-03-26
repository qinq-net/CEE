#include "T1RunAction.hh"
//#include "T1PrimaryGeneratorAction.hh"
#include "T1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
//#include "G4ParameterManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1RunAction::T1RunAction()
: G4UserRunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1RunAction::~T1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void T1RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void T1RunAction::EndOfRunAction(const G4Run*)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
