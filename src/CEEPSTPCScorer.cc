#include "CEEPSTPCScorer.hh"
#include <G4UnitsTable.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>
#include <G4RotationMatrix.hh>
#include <G4TouchableHistory.hh>


CEEPSTPCScorer::CEEPSTPCScorer(G4String name, G4int nX_p, G4int nZ_p, G4int depth)
	: G4VPrimitiveScorer(name, depth), HCID(-1), EvtMap(0),
	touchBox(0), sizeX(0.), sizeY(0.), sizeZ(0.),
	lengthX(0.), lengthZ(0.), GeometryParametersLoaded(FALSE)
{
	this->nX=nX_p;
	this->nZ=nZ_p;
	this->depth=depth;
}

CEEPSTPCScorer::~CEEPSTPCScorer()
{
	;
}

G4bool CEEPSTPCScorer::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	if(!GeometryParametersLoaded)
	{
		//std::cout << "Loading Geometry Parameters of TPC..." << std::endl;
		//transTouch = aStep->GetPreStepPoint()->GetTouchable()->GetTranslation(depth);
		//std::cout << "Got Translation as " << transTouch.x() << "," << transTouch.y() << "," << transTouch.z();
		//rotTouch = aStep->GetPreStepPoint()->GetTouchable()->GetRotation(depth);
		//std::cout << "Got Rotation as " << rotTouch << std::endl;

		touchBox = dynamic_cast<G4Box*>(aStep->GetPreStepPoint()->GetTouchable()->GetSolid());
		sizeX = touchBox->GetXHalfLength();
		sizeY = touchBox->GetYHalfLength();
		sizeZ = touchBox->GetZHalfLength();
		//std::cout << "Got Box as " << sizeX << "," << sizeY << "z" << sizeZ << std::endl;

		lengthX = 2. * sizeX / (G4double)nX;
		lengthZ = 2. * sizeZ / (G4double)nZ;
		//std::cout << "Got length as " << lengthX << "," << lengthZ << std::endl;
		GeometryParametersLoaded = TRUE;
	}
	
	G4int trackID = aStep->GetTrack()->GetTrackID();
	G4int index = GetIndex(aStep);
	if(!(*EvtMap)[index]) (*EvtMap->GetMap())[index]=new CEETPCDataMap({});
	CEETPCDataMap& dataMap = *(*EvtMap->GetMap())[index];


	//std::cout << "Getting information for Track " << trackID << "to map " << reinterpret_cast<std::size_t>(&dataMap) << std::endl;
	
	G4ThreeVector stepPosition = aStep->GetPreStepPoint()->GetPosition();
	//G4ThreeVector relativePosition = rotTouch->inverse() * ( stepPosition - transTouch );
	G4ThreeVector relativePosition = aStep->GetPreStepPoint()->GetTouchable()->GetHistory()->GetTopTransform().TransformPoint(stepPosition);
	// 分入不同的小室内
	G4int pX = relativePosition.x()/lengthX;
	G4double pY = relativePosition.y();
	G4int pZ = relativePosition.z()/lengthZ;

	//std::cout << "Position got: " << pX << " " << pZ << std::endl;

	/*
	if(!dataMap.count({pX,pZ}))
	{
		dataMap[{pX,pZ}] = {
			aStep->GetTotalEnergyDeposit(),
			aStep->GetPreStepPoint()->GetGlobalTime(),
			relativePosition.y()	};
	}
	else
	{
		dataMap[{pX,pZ}].energyDeposit += aStep->GetTotalEnergyDeposit();
	}
	*/
	// Is An Electron
	if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName() == "e-")
	{
		// Exiting TPC, Delete Entry
		if(aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary)
		{
			dataMap.erase(trackID);
		}
		else // Inside the TPC, Create Entry
		{
			dataMap[trackID] =
			{
				{ pX, pZ }, // Cell of Step
				pY, // horizonal position of Step
				aStep->GetPreStepPoint()->GetGlobalTime()
			};
		}
	}

	//std::cout << "data saved." << std::endl;

	return TRUE;
}

void CEEPSTPCScorer::Initialize(G4HCofThisEvent* HCE)
{
	EvtMap = new G4THitsMap<CEETPCDataMap>(GetMultiFunctionalDetector()->GetName(), GetName());
	if(HCID < 0) {HCID = GetCollectionID(0);}
	HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void CEEPSTPCScorer::EndOfEvent(G4HCofThisEvent*)
{
	;
}

void CEEPSTPCScorer::clear()
{
	EvtMap->clear();
	GeometryParametersLoaded = FALSE;
}

void CEEPSTPCScorer::DrawAll()
{
	;
}

void CEEPSTPCScorer::PrintAll()
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
	G4cout << ">> TPC Size: sizeX=" << sizeX/mm
		<< "mm sizeY=" << sizeY/mm
		<< "mm sizeZ=" << sizeZ/mm
		<< "mm" << G4endl;
	G4cout << ">> Cell Length: lengthX=" << lengthX/mm
		<< "mm, lengthZ=" << lengthZ/mm
		<< "mm" << G4endl;
	for(auto copyItr: *(EvtMap->GetMap()))
	{
		G4cout << " >   copy no.: " << copyItr.first << G4endl;
		G4cout << " >   Number of entries " << copyItr.second->size()
			<< G4endl;
		std::map<std::pair<G4int,G4int>, std::pair<G4int,G4double>> cellMap;
		for(auto trackItr: *copyItr.second)
		{
			G4int trackID=trackItr.first;
			G4int cellX=trackItr.second.XZPosition.first;
			G4int cellZ=trackItr.second.XZPosition.second;
			G4double positionY=trackItr.second.YPosition;
			G4double Time=trackItr.second.Time;
			G4double SignalTime = Time + (sizeY - positionY)/(5*cm/us);
			G4cout << " >> TrackID=" << trackID
				<< " CellXZ={" << cellX
				<< "," << cellZ
				<< "} positionY=" << positionY/mm
				<< "mm Time=" << Time/ns
				<< "ns" << G4endl;
			// modify cellMap entry
			if(!cellMap.count({cellX,cellZ}))
				cellMap[{cellX,cellZ}].second=SignalTime;
			cellMap[{cellX,cellZ}].first+=1;
			if(SignalTime<cellMap[{cellX,cellZ}].second)
				cellMap[{cellX,cellZ}].second=SignalTime;
		}
		for(auto cellItr: cellMap)
		{
			G4cout << " >>> Cell:{" << cellItr.first.first
				<< "," << cellItr.first.second
				<< "} ElectronCount=" << cellItr.second.first
				<< " SignalTime=" << cellItr.second.second/ns
				<< "ns" << G4endl;
		}
	}
}
