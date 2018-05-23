#ifndef CEEPSMWDCScorer_H
#define CEEPSMWDCScorer_H

#include <G4VPrimitiveScorer.hh>
#include <G4THitsMap.hh>
#include <G4ThreeVector.hh>
#include <G4Box.hh>
#include <G4SystemOfUnits.hh>
#include "T1MWDC.hh"

typedef enum CEEMWDCZLayer
{ X, U, V } CEEMWDCZLayer;
typedef struct CEEMWDCData
{
	//TODO: Future Data
	CEEMWDCZLayer ZLayer; // should be 0(X), 1(U) or 2(V)
	G4int WireNumber; // on XY scale
	G4double Distance; //distance to wire
	G4double kinectEnergy;//first point kinect energy
	G4double Time;
	G4double pX, pY, pZ;
	G4int trackID, stepID;
} CEEMWDCData;

typedef G4int CEEMWDCIndex;// TrackID
typedef std::map<CEEMWDCIndex, CEEMWDCData> CEEMWDCDataMap;

class CEEPSMWDCScorer: public G4VPrimitiveScorer
{
	public:
	CEEPSMWDCScorer(G4String name, G4double WireDistance=2.*mm, G4double LevelDistance=40.*mm, G4int depth=2);
	virtual ~CEEPSMWDCScorer();

	protected:
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);

	public:
	virtual void Initialize(G4HCofThisEvent*);
	virtual void EndOfEvent(G4HCofThisEvent*);
	virtual void clear();
	virtual void DrawAll();
	virtual void PrintAll();
	G4THitsMap<CEEMWDCDataMap>* GetMap() { return EvtMap; }

	private:
	G4int HCID;
	G4THitsMap<CEEMWDCDataMap>* EvtMap;
	G4int depth;

	G4Box* touchBox;
	G4double sizeX, sizeY, sizeZ;
	G4int nX, nU, nV;//number of wires of each level
	G4double WireDistance, LevelDistance;
	G4double SensRadius;

	G4bool GeometryParametersLoaded;
};
	


#endif//CEEPSMWDCScorer
