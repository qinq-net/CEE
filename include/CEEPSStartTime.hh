#ifndef CEEPSStartTime_H
#define CEEPSStartTime_H

#include <G4VPrimitiveScorer.hh>
#include <G4THitsMap.hh>

class CEEPSStartTime : public G4VPrimitiveScorer
{
	public:
	CEEPSStartTime(G4String name, G4int depth=0);
	CEEPSStartTime(G4String name, const G4String& unit, G4int depth=0);
	virtual ~CEEPSStartTime();

	protected:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

	public:
	virtual void Initialize(G4HCofThisEvent*);
	virtual void EndOfEvent(G4HCofThisEvent*);
	virtual void clear();
	virtual void DrawAll();
	virtual void PrintAll();

	virtual void SetUnit(const G4String& unit);

	private:
	G4int HCID;
	G4THitsMap<G4double>* EvtMap;
	G4bool hitN;

};

#endif//CEEPSStartTime_H
