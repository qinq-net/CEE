#ifndef CEEPSMRPCScorer_H
#define CEEPSMRPCScorer_H

#include <G4VPrimitiveScorer.hh>
#include <G4THitsMap.hh>
#include <G4ThreeVector.hh>
#include <G4AffineTransform.hh>

typedef enum CEEMRPCDirection { X, Y, Z } CEEMRPCDirection;

typedef struct CEEMRPCData
{
	G4int trackID,stepID;
	G4double pX, pY, pZ;
	G4double Time;
	G4double ATime, BTime;
} CEEMRPCData;

typedef struct CEEMRPCGeomData
{
	G4double sizeX, sizeY, sizeZ;
	G4AffineTransform trans;
} CEEMRPCGeomData;

typedef std::vector<CEEMRPCData> CEEMRPCDataList;
typedef std::map<G4String, std::pair<CEEMRPCGeomData,CEEMRPCDataList>> CEEMRPCDataListMap;

class CEEPSMRPCScorer: public G4VPrimitiveScorer
{
	public:
	CEEPSMRPCScorer(G4String name, CEEMRPCDirection direction, G4int depth);
	virtual ~CEEPSMRPCScorer();

	protected:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

	public:
	virtual void Initialize(G4HCofThisEvent*);
	virtual void EndOfEvent(G4HCofThisEvent*);
	virtual void clear();
	virtual void DrawAll();
	virtual void PrintAll();
	G4THitsMap<CEEMRPCDataListMap>* GetMap() { return EvtMap; }

	private:
	G4int HCID;
	G4THitsMap<CEEMRPCDataListMap>* EvtMap;
	CEEMRPCDirection direction;
	G4int depth;
};
#endif
