#ifndef CEEPSStep_H
#define CEEPSStep_H

#include <G4VPrimitiveScorer.hh>
#include <G4THitsMap.hh>
#include <G4ThreeVector.hh>

/* A struct used to save all valuable information during a major event */
typedef struct
{
	G4int trackID;
	G4int stepID;
	G4double time;
	G4ThreeVector position;
	G4double deltaEnergy;
	G4double energyDeposit;
} CEEStepData;

class CEEPSStep : public G4VPrimitiveScorer
{
	public:
	CEEPSStep(G4String name, G4int depth=0);
	CEEPSStep(G4String name, G4bool onlymain=TRUE, G4int depth=0);
	virtual ~CEEPSStep();

	protected:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

	public:
	virtual void Initialize(G4HCofThisEvent*);
	virtual void EndOfEvent(G4HCofThisEvent*);
	virtual void clear();
	virtual void DrawAll();
	virtual void PrintAll();
	G4THitsMap< std::vector<CEEStepData*> >* GetMap() { return EvtMap; }

	//virtual void SetUnit(const G4String& unit);

	private:
	G4int HCID;
	G4THitsMap< std::vector<CEEStepData*> >* EvtMap;
	G4bool onlymain;

};

#endif//CEEPSStep_H
