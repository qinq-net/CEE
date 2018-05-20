#include "CEEPSStep.hh"
#include <G4UnitsTable.hh>
#include <G4Step.hh>
#include <G4SystemOfUnits.hh>

CEEPSStep::CEEPSStep(G4String name, G4int depth)
	: G4VPrimitiveScorer(name, depth), HCID(-1), EvtMap(0), onlymain(FALSE)
{
	;
}

CEEPSStep::CEEPSStep(G4String name, G4bool onlymain, G4int depth)
	: G4VPrimitiveScorer(name, depth), HCID(-1), EvtMap(0)
{
	this->onlymain = onlymain;
}

CEEPSStep::~CEEPSStep()
{
	;
}

G4bool CEEPSStep::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	G4int trackID = aStep->GetTrack()->GetTrackID();
	G4int stepID = aStep->GetTrack()->GetCurrentStepNumber();
	if(!(onlymain) || aStep->GetTrack()->GetTrackID() == 1)
	{
		//G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
		//G4StepPoint* postStepPoint = aStep->GetPostStepPoint();
		G4double time = aStep->GetPostStepPoint()->GetGlobalTime();
		G4ThreeVector position = aStep->GetPostStepPoint()->GetPosition();
		G4double deltaEnergy = aStep->GetDeltaEnergy();
		G4double energyDeposit = aStep->GetTotalEnergyDeposit();
		CEEStepData* stepData = new CEEStepData({
			trackID,
			stepID,
			//preStepPoint,
			//postStepPoint,
			time,
			position,
			deltaEnergy,
			energyDeposit
		});
		
		G4int index = GetIndex(aStep);
		if(!(*EvtMap)[index]) EvtMap->set(index, *(new std::vector<CEEStepData*>));
		(*EvtMap)[index]->push_back(stepData);
		return TRUE;
	}
	return FALSE;
}

void CEEPSStep::Initialize(G4HCofThisEvent* HCE)
{
	EvtMap = new G4THitsMap< std::vector<CEEStepData*> >(GetMultiFunctionalDetector()->GetName(), GetName());
	if(HCID < 0) {HCID = GetCollectionID(0);}
	HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
}

void CEEPSStep::EndOfEvent(G4HCofThisEvent*)
{
	;
}

void CEEPSStep::clear()
{
	EvtMap->clear();
}

void CEEPSStep::DrawAll()
{
	/* TODO: draw all points */
	;
}

void CEEPSStep::PrintAll()
{
	G4cout << ">> MultiFunctionalDet  " << detector->GetName() << G4endl;
	G4cout << ">> PrimitiveScorer " << GetName() << G4endl;
	G4cout << ">> Number of entries " << EvtMap->entries() << G4endl;
	for(auto copyItr: *(EvtMap->GetMap()))
	{
		G4cout 	<< " >   copy no.: " << copyItr.first << G4endl;
		for(auto trackItr: *copyItr.second)
		{
			G4cout 	<< " >> tID=" << trackItr->trackID
				<< " sID=" << trackItr->stepID
				<< " t=" << trackItr->time/ns
				<< "ns x=" << trackItr->position.x()/mm
				<< "mm y=" << trackItr->position.y()/mm
				<< "mm z=" << trackItr->position.z()/mm
				<< "mm dE=" << trackItr->deltaEnergy/MeV
				<< "MeV eD=" << trackItr->energyDeposit/MeV << "MeV" << G4endl;
		}
	}
}
		
		
	
