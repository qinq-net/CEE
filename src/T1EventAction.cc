// vim: set nofoldenable :
#include "T1EventAction.hh"
#include "T1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include <G4SDManager.hh>
#include <G4VPrimitiveScorer.hh>
#include "T1TPCDigi.hh"
#include "T1MWDCDigi.hh"
#include "CEEEventLoggingSession.hh"
#include <G4UImanager.hh>

#include <fstream>
using namespace std;
//extern ofstream shuchu;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1EventAction::T1EventAction(T1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

T1EventAction::~T1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void T1EventAction::BeginOfEventAction(const G4Event*)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

template<class SDType> inline G4bool PrintMultiPrimitive(SDType* det)
{
	if(!det) { G4cerr << "### TODO:" << typeid(SDType).name() << " NOT Found!"; return FALSE;}
	else
	{
		G4cout << "### {{{ G4MultiFunctionalDetector " << det->GetName() << G4endl;
		for(G4int i=0; i<det->GetNumberOfPrimitives(); i++)
		{
			G4cout << "#### {{{ Primitive " << i << std::flush;
			G4cout << " " << det->GetPrimitive(i)->GetName() << G4endl;
			det->GetPrimitive(i)->PrintAll();
			G4cout << "#### }}}" << G4endl;
		}
		G4cout << "### }}}" << G4endl;
	}
	return TRUE;
}

void T1EventAction::EndOfEventAction(const G4Event* evt)
{
	G4cout << " >>> End of Event " << evt->GetEventID() << G4endl;
	{ //SD record
		G4SDManager* SDMan = G4SDManager::GetSDMpointer();
		if(!SDMan) { G4cerr << "No SDManager!" << G4endl;}
		else
		{
			G4String outFileName = "results/Event" + std::to_string(evt->GetEventID()) + "Out.txt";
			G4String errFileName = "results/Event" + std::to_string(evt->GetEventID()) + "Err.txt";
			
			G4cout << "Relocating G4cout to file: " << outFileName << G4endl;
			CEEEventLoggingSession* session = new CEEEventLoggingSession(
				outFileName, errFileName);
			G4UImanager* UI=G4UImanager::GetUIpointer();
			G4UIsession* oldSession = UI->GetSession();
			if(UI) UI->SetCoutDestination(session);
			// G4cout has been relocated.
			G4cout << "### Event " << evt->GetEventID() << G4endl;
			G4cout << "# vim: set foldmethod=marker :" << G4endl;

			{ //TPC
				T1TPCDigi* tpcDigi = dynamic_cast<T1TPCDigi*>(SDMan->FindSensitiveDetector("CEE_TPC_logic_det"));
				PrintMultiPrimitive<T1TPCDigi>(tpcDigi);
			}
			//MWDC
			for(int MWDC_id=1; MWDC_id<=6; MWDC_id++)
			{
				T1MWDCDigi* mwdcDigi = dynamic_cast<T1MWDCDigi*>(SDMan->FindSensitiveDetector("CEE_MWDC"+std::to_string(MWDC_id)+"_logic_det"));
				PrintMultiPrimitive<T1MWDCDigi>(mwdcDigi);
			}
			// relocating G4cout back to screen
			if(UI) UI->SetCoutDestination(oldSession);
			delete session;
		}
	}
	return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
