#include "CEEPSStartTime.hh"
#include <G4UnitsTable.hh>

CEEPSStartTime::CEEPSStartTime(G4String name, G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1),EvtMap(0),hitN(0)
{
  SetUnit("ns");
}

CEEPSStartTime::CEEPSStartTime(G4String name, const G4String& unit, 
				     G4int depth)
  :G4VPrimitiveScorer(name,depth),HCID(-1),EvtMap(0),hitN(0)
{
  SetUnit(unit);
}

CEEPSStartTime::~CEEPSStartTime()
{;}

G4bool CEEPSStartTime::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  //G4StepPoint* point1 = aStep->GetPreStepPoint();
  G4StepPoint* point2 = aStep->GetPostStepPoint();
  if(hitN++) return FALSE;
  G4double globalTime=point2->GetGlobalTime();
  G4int index = GetIndex(aStep);
  EvtMap->add(index, globalTime);
  return TRUE;
}

void CEEPSStartTime::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(),
				    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*)EvtMap);
  hitN=0;
}

void CEEPSStartTime::EndOfEvent(G4HCofThisEvent*)
{;}

void CEEPSStartTime::clear()
{
  EvtMap->clear();
  hitN=0;
}

void CEEPSStartTime::DrawAll()
{;}

void CEEPSStartTime::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  start time: " 
	   << *(itr->second)/GetUnitValue()
	   << " [" << GetUnit()<<"]"
	   << G4endl;
  }
}

void CEEPSStartTime::SetUnit(const G4String& unit)
{
	CheckAndSetUnit(unit,"Time");
}
