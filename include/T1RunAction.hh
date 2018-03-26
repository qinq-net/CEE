
#ifndef T1RunAction_h
#define T1RunAction_h 1

#include "G4UserRunAction.hh"
//#include "G4Parameter.hh"
#include "globals.hh"

class G4Run;    

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class T1RunAction : public G4UserRunAction
{
  public:
    T1RunAction();
    virtual ~T1RunAction();

    // virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};

#endif
