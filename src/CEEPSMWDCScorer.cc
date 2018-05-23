#include "CEEPSMWDCScorer.hh"
#include <G4UnitsTable.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4RotationMatrix.hh>
#include <G4TouchableHistory.hh>


CEEPSMWDCScorer::CEEPSMWDCScorer(G4String name, G4double WireDistance_p, G4double LevelDistance_p, G4int depth)
	: G4VPrimitiveScorer(name, depth), HCID(-1), EvtMap(0),
	touchBox(0), sizeX(0.), sizeY(0.), sizeZ(0.),
	WireDistance(2.*mm), LevelDistance(40.*mm),
	GeometryParametersLoaded(FALSE)
{
	this->WireDistance = WireDistance_p;
	this->SensRadius = WireDistance_p/2;
	this->LevelDistance = LevelDistance_p;
	this->depth = depth;
}

CEEPSMWDCScorer::~CEEPSMWDCScorer()
{
	;
}

G4bool CEEPSMWDCScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	if(!GeometryParametersLoaded)
	{
		//std::cout << "Loading Geometry Parameters of MWDC..." << std::endl;
		//transTouch = aStep->GetPreStepPoint()->GetTouchable()->GetTranslation(depth);
		//std::cout << "Got Translation as " << transTouch.x() << "," << transTouch.y() << "," << transTouch.z();
		//rotTouch = aStep->GetPreStepPoint()->GetTouchable()->GetRotation(depth);
		//std::cout << "Got Rotation as " << rotTouch << std::endl;

		touchBox = dynamic_cast<G4Box*>(aStep->GetPreStepPoint()->GetTouchable()->GetSolid());
		sizeX = touchBox->GetXHalfLength();
		sizeY = touchBox->GetYHalfLength();
		sizeZ = touchBox->GetZHalfLength();
		//std::cout << "Got Box as " << sizeX << "," << sizeY << "z" << sizeZ << std::endl;

		nX = sizeX / WireDistance;
		nU = (sizeX + sizeY/sqrt(3)) / (WireDistance / sqrt(3) * 2.);
		nV = nU;

		GeometryParametersLoaded = TRUE;
	}
	
	G4int trackID = aStep->GetTrack()->GetTrackID();
	G4int stepID = aStep->GetTrack()->GetCurrentStepNumber();
	G4int index = GetIndex(aStep);
	if(!(*EvtMap)[index]) (*EvtMap->GetMap())[index]=new CEEMWDCDataMap({});
	CEEMWDCDataMap& dataMap = *(*EvtMap->GetMap())[index];


	//std::cout << "Getting information for Track " << trackID << "to map " << reinterpret_cast<std::size_t>(&dataMap) << std::endl;
	
	G4ThreeVector stepPosition = aStep->GetPreStepPoint()->GetPosition();
	//G4ThreeVector relativePosition = rotTouch->inverse() * ( stepPosition - transTouch );
	G4ThreeVector relativePosition = aStep->GetPreStepPoint()->GetTouchable()->GetHistory()->GetTopTransform().TransformPoint(stepPosition);
	G4double pX=relativePosition.x(), pY=relativePosition.y(), pZ=relativePosition.z();
	// Is An Electron And Has Not Been Recorded
	if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "e-"
		&& dataMap.count(trackID)==0)
	{
		// Exiting MWDC, Delete Entry
		/*
		if(aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary)
		{
			dataMap.erase(trackID);
		}
		else // Inside the MWDC, Create Entry
		{
			dataMap[trackID] =
			{
				{ pX, pZ }, // Cell of Step
				pY, // horizonal position of Step
				aStep->GetPreStepPoint()->GetGlobalTime()
			};
		}*/
		// if the track touches X layer
		if(-LevelDistance-SensRadius <= pZ && pZ <= -LevelDistance+SensRadius)
		{
			// if the track hits one wire
			for(G4int i=-nX; i<=nX; i++)
			{
				G4double Distance=sqrt(
					std::pow(pX-i*WireDistance,2)+
					std::pow(pZ+LevelDistance,2)
					);
				if(Distance<=SensRadius)
				{
					dataMap[trackID] =
					{
						CEEMWDCZLayer::X,//ZLayer
						i,//WireNumber
						Distance,
						aStep->GetPreStepPoint()->GetKineticEnergy(),
						aStep->GetPreStepPoint()->GetGlobalTime(),
						pX, pY, pZ,
						trackID, stepID
					};
				}
			}
		}
		// if the track touches U layer
		if(-WireDistance <= pZ && pZ <= WireDistance)
		{
			// if the track hits one wire
			for(G4int i=-nU; i<=nU; i++)
			{
				G4double Distance = sqrt(
					std::pow(sqrt(3)*pX-pY-2.*i*WireDistance,2)/4.+
					std::pow(pZ,2)
					);
				if(Distance<=SensRadius)
				{
					dataMap[trackID] =
					{
						CEEMWDCZLayer::U,//ZLayer
						i,//WireNumber
						Distance,
						aStep->GetPreStepPoint()->GetKineticEnergy(),
						aStep->GetPreStepPoint()->GetGlobalTime(),
						pX, pY, pZ,
						trackID, stepID
					};
				}
			}
		}
		// if the track touches V layer
		if(LevelDistance-SensRadius <= pZ && pZ <= LevelDistance+SensRadius)
		{
			// if the track hits one wire
			for(G4int i=-nV; i<=nV; i++)
			{
				G4double Distance = sqrt(
					std::pow(sqrt(3)*pX+pY-2.*i*WireDistance,2)/4.+
					std::pow(pZ-LevelDistance,2)
					);
				if(Distance<=SensRadius)
				{
					dataMap[trackID] =
					{
						CEEMWDCZLayer::V,//ZLayer
						i,//WireNumber
						Distance,
						aStep->GetPreStepPoint()->GetKineticEnergy(),
						aStep->GetPreStepPoint()->GetGlobalTime(),
						pX, pY, pZ,
						trackID, stepID
					};
				}
			}
		}
	}
	/*
	if(!dataMap.count(trackID))
	{
		dataMap[trackID] = 
		{
			relativePosition.x(), relativePosition.y(), relativePosition.z(),
			aStep->GetPreStepPoint()->GetKineticEnergy(),
			aStep->GetPreStepPoint()->GetGlobalTime()
		};
	}
	*/

	//std::cout << "data saved." << std::endl;

	return TRUE;
}

void CEEPSMWDCScorer::Initialize(G4HCofThisEvent* HCE)
{
	EvtMap = new G4THitsMap<CEEMWDCDataMap>(GetMultiFunctionalDetector()->GetName(), GetName());
	if(HCID < 0) {HCID = GetCollectionID(0);}
	HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void CEEPSMWDCScorer::EndOfEvent(G4HCofThisEvent*)
{
	;
}

void CEEPSMWDCScorer::clear()
{
	EvtMap->clear();
	GeometryParametersLoaded = FALSE;
}

void CEEPSMWDCScorer::DrawAll()
{
	;
}

void CEEPSMWDCScorer::PrintAll()
{
	G4cout << ">> MultiFunctionalDet  " << detector->GetName() << G4endl;
	G4cout << ">> PrimitiveScorer " << GetName() << G4endl;
	G4cout << ">> Number of copies " << EvtMap->entries() << G4endl;
	/*
	for(auto copyItr: *(EvtMap->GetMap()))
	{
		G4cout 	<< " >   copy no.: " << copyItr.first << G4endl;
		for(auto cellItr: *copyItr.second)
		{
			G4cout 	<< " >> Cell ix=" << cellItr.first.ix
				<< " iz=" << cellItr.first.iz
				<< " energyDeposit=" << cellItr.second.energyDeposit/MeV
				<< "MeV startTime=" << cellItr.second.startTime/ns
				<< "ns YPosition=" << cellItr.second.ZPosition/mm
				<< "mm" << G4endl;
		}
	}
	*/
	G4cout << ">> MWDC Size: sizeX=" << sizeX/mm
		<< "mm sizeY=" << sizeY/mm
		<< "mm sizeZ=" << sizeZ/mm
		<< "mm" << G4endl;
	G4cout << ">> WireDistance=" << WireDistance/mm
		<< "mm LayerDistance=" << LevelDistance/mm
		<< "mm" << G4endl;
	G4cout << ">> Wire Count: nX=" << nX 
		<< ", nU=" << nU << ", nV=" << nV << G4endl;
	for(auto copyItr: *(EvtMap->GetMap()))
	{
		G4cout << " >   copy no.: " << copyItr.first << G4endl;
		G4cout << " >   Number of entries " << copyItr.second->size()
			<< G4endl;
		for(auto trackItr: *copyItr.second)
		{
			G4int trackID=trackItr.first;
			G4int stepID=trackItr.second.stepID;
			G4double pX=trackItr.second.pX, pY=trackItr.second.pY, pZ=trackItr.second.pZ;
			G4double Distance=trackItr.second.Distance;
			G4double kinectEnergy=trackItr.second.kinectEnergy;
			G4double Time=trackItr.second.Time;
			CEEMWDCZLayer ZLayer=trackItr.second.ZLayer;
			G4int WireNumber = trackItr.second.WireNumber;
			G4cout << " >> TrackID=" << trackID
				<< " StepID=" << stepID
				<< " ZLayer:" << ZLayer
				<< " WireNumber:" << WireNumber
				<< " Distance=" << Distance/mm
				<< "mm kinectEnergy=" << kinectEnergy/MeV
				<< "MeV Time=" << Time/ns
				<< "ns pX=" << pX/mm
				<< "mm pY=" << pY/mm
				<< "mm pZ=" << pZ/mm
				<< "mm" << G4endl;
		}
	}
}
