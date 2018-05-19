#ifndef CEEPSTPCScorer_H
#define CEEPSTPCScorer_H

#include <G4VPrimitiveScorer.hh>
#include <G4THitsMap.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include "T1TPC.hh"

/* A struct used to save energy deposit and other things */
typedef struct
{
	G4double energyDeposit;
	G4double startTime;
	G4double ZPosition;
} CEETPCData;

typedef struct CEETPCIndex
{
	G4int ix;
	G4int iz;
	inline bool operator<(const struct CEETPCIndex& right) const
	{
		return ((this->ix)<right.ix)||((this->iz)<right.iz);
	}
} CEETPCIndex;

typedef std::map<CEETPCIndex, CEETPCData*> CEETPCDataMap;

class CEEPSTPCScorer: public G4VPrimitiveScorer
{
	public:
	CEEPSTPCScorer(G4String name, G4int nX=16, G4int nZ=16, G4int depth=0);
	virtual ~CEEPSTPCScorer();

	protected:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

	public:
	virtual void Initialize(G4HCofThisEvent*);
	virtual void EndOfEvent(G4HCofThisEvent*);
	virtual void clear();
	virtual void DrawAll();
	virtual void PrintAll();
	G4THitsMap<CEETPCDataMap>* GetMap() { return EvtMap; }

	private:
	G4int HCID;
	G4THitsMap<CEETPCDataMap>* EvtMap;
	G4double depth;

	G4ThreeVector transTouch;
	const G4RotationMatrix* rotTouch;
	G4Box* touchBox;
	G4double sizeX, sizeY, sizeZ;
	G4int nX, nZ;
	G4double lengthX, lengthZ;

	G4bool GeometryParametersLoaded;

};
	
#endif//CEEPSTPCScorer_H
