#include "CEEPSMRPCScorer.hh"
#include <G4UnitsTable.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4RotationMatrix.hh>
#include <G4TouchableHistory.hh>
#include <G4Box.hh>
const G4double SpeedOfLight=299792458.*m/s;

CEEPSMRPCScorer::CEEPSMRPCScorer(G4String name, CEEMRPCDirection direction, G4int depth)
	: G4VPrimitiveScorer(name, depth), HCID(-1), EvtMap(0)
{
	this->direction = direction;
	this->depth = depth;
}

CEEPSMRPCScorer::~CEEPSMRPCScorer()
{
	;
}

G4bool CEEPSMRPCScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	G4int index=GetIndex(aStep);
	G4String MRPCName = aStep->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName();

	G4int trackID = aStep->GetTrack()->GetTrackID();
	G4int stepID = aStep->GetTrack()->GetCurrentStepNumber();

	if(!(*EvtMap)[index]) (*EvtMap->GetMap())[index]=new CEEMRPCDataListMap({});
	CEEMRPCDataListMap& dataListMap = *(*EvtMap->GetMap())[index];

	// read geometry info
	G4Box* touchBox = dynamic_cast<G4Box*>(aStep->GetPreStepPoint()->GetTouchable()->GetSolid());
	G4double sizeX = touchBox->GetXHalfLength();
	G4double sizeY = touchBox->GetYHalfLength();
	G4double sizeZ = touchBox->GetZHalfLength();
	G4AffineTransform trans = aStep->GetPreStepPoint()->GetTouchable()->GetHistory()->GetTopTransform();
	G4ThreeVector position = aStep->GetPreStepPoint()->GetPosition();
	G4ThreeVector relativePosition = trans.TransformPoint(position);
	G4double pX=relativePosition.x(), pY=relativePosition.y(), pZ=relativePosition.z();
	G4double Time = aStep->GetPreStepPoint()->GetGlobalTime();
	G4double ATime,BTime;
	const G4double Speed = 2./3.*SpeedOfLight;
	switch(direction) {
		case CEEMRPCDirection::X:
			ATime = Time + (sizeX-pX) / Speed;
			BTime = Time + (sizeX+pX) / Speed;
			break;
		case CEEMRPCDirection::Y:
			ATime = Time + (sizeY-pY) / Speed;
			BTime = Time + (sizeY+pY) / Speed;
			break;
		case CEEMRPCDirection::Z:
			ATime = Time + (sizeZ-pZ) / Speed;
			BTime = Time + (sizeZ+pZ) / Speed;
			break;
		G4cerr << "No direction for " << this->GetName() << G4endl;
		return FALSE;
	}
	if(!dataListMap.count(MRPCName))
	{
		dataListMap[MRPCName]=
		{
			{// Geometry Data
				sizeX, sizeY, sizeZ,
				trans
			},
			{{// Digitized Data
				trackID,stepID,
				pX,pY,pZ,
				Time, ATime, BTime
			}}
		};
	}
	else
	{
		dataListMap[MRPCName].second.push_back(
		{
			trackID,stepID,
			pX,pY,pZ,
			Time, ATime, BTime
		});
	}
	return TRUE;
}

void CEEPSMRPCScorer::Initialize(G4HCofThisEvent* HCE)
{
	EvtMap = new G4THitsMap<CEEMRPCDataListMap>(GetMultiFunctionalDetector()->GetName(), GetName());
	if(HCID < 0) {HCID = GetCollectionID(0);}
	HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void CEEPSMRPCScorer::EndOfEvent(G4HCofThisEvent*)
{
	;
}

void CEEPSMRPCScorer::clear()
{
	EvtMap->clear();
}

void CEEPSMRPCScorer::DrawAll()
{
	;
}

void CEEPSMRPCScorer::PrintAll()
{
	G4cout << ">> MultiFunctionalDet  " << detector->GetName() << G4endl;
	G4cout << ">> PrimitiveScorer " << GetName() << G4endl;
	G4cout << ">> Number of copies " << EvtMap->entries() << G4endl;
	G4cout << ">> Direction:" << std::flush;
	switch(direction)
	{
		case CEEMRPCDirection::X: G4cout << "X" << G4endl; break;
		case CEEMRPCDirection::Y: G4cout << "Y" << G4endl; break;
		case CEEMRPCDirection::Z: G4cout << "Z" << G4endl; break;
	}
	for(auto copyItr: *(EvtMap->GetMap()))
	{
		G4cout << " >  copy no.: " << copyItr.first << G4endl;
		G4cout << " >  Number of entries " << copyItr.second->size() << G4endl;
		for(auto mrpcItr: *copyItr.second)
		{
			if(!mrpcItr.second.second.size()) break;
			G4cout << " >>> MRPC: " << mrpcItr.first << G4endl;
			G4cout << "   > sizeX=" << mrpcItr.second.first.sizeX/mm
				<< "mm sizeY=" << mrpcItr.second.first.sizeY/mm
				<< "mm sizeZ=" << mrpcItr.second.first.sizeZ/mm
				<< "mm " << G4endl;
			G4double Time=mrpcItr.second.second[0].Time;
			G4double ATime=mrpcItr.second.second[0].ATime;
			G4double BTime=mrpcItr.second.second[0].BTime;
			for(auto stepItr: mrpcItr.second.second)
			{
				if(stepItr.Time<Time) Time=stepItr.Time;
				if(stepItr.ATime<ATime) ATime=stepItr.ATime;
				if(stepItr.BTime<BTime) BTime=stepItr.BTime;
			}
			G4cout << " >>> Time=" << Time/ns
				<< "ns ATime=" << ATime/ns
				<< "ns BTime=" << BTime/ns
				<< "ns" << G4endl;
			for(auto stepItr: mrpcItr.second.second)
			{
				G4cout << " >>>> tID=" << stepItr.trackID
					<< " sID=" << stepItr.stepID
					<< " pX=" << stepItr.pX/mm
					<< "mm pY=" << stepItr.pY/mm
					<< "mm pZ=" << stepItr.pZ/mm
					<< "mm Time=" << stepItr.Time/ns
					<< "ns ATime=" << stepItr.ATime/ns
					<< "ns BTime=" << stepItr.BTime/ns
					<< "ns" << G4endl;
			}
		}
	}
}

