
#ifndef T1EventAction_h
#define T1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class T1RunAction;

/// Event action class
///

class T1EventAction : public G4UserEventAction
{
  public:
    T1EventAction(T1RunAction* runAction);
    virtual ~T1EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

  private:
    T1RunAction* fRunAction;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
