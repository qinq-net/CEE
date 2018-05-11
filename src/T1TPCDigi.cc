#include "T1TPCDigi.hh"
#include <G4VSensitiveDetector.hh>

#include <G4MultiFunctionalDetector.hh>
#include <G4SDParticleFilter.hh>
#include <G4VPrimitiveScorer.hh>
#include <G4PSEnergyDeposit.hh>
#include <G4PSNofSecondary.hh>
#include <G4PSMinKinEAtGeneration.hh>
#include <G4PSTrackLength.hh>
#include <G4PSNofStep.hh>

#include "CEEPSStartTime.hh"
#include "CEEPSStep.hh"

T1TPCDigi::T1TPCDigi(G4String name, G4int depth=0) : G4MultiFunctionalDetector(name)
{
	/* Register primitives on construction
	 *
	 * The second argument in each primitive means the "level" of geometrical hierarchy,
	 * the copy number of that level is used as the key of the G4THitsMap.
	 * For absorber (j = 0), the copy number of its own physical volume is used.
	 * For gap (j = 1), the copy number of its mother physical volume is used, since there
	 * is only one physical volume of gap is placed with respect to its mother. */
	G4VPrimitiveScorer* primitive;

	//primitive = new G4PSEnergyDeposit("eDep",depth);
	//this->RegisterPrimitive(primitive);
	primitive = new CEEPSStartTime("startTime",depth);
	this->RegisterPrimitive(primitive);
	primitive = new CEEPSStep("stepScorer", depth);
	this->RegisterPrimitive(primitive);

}

T1TPCDigi::~T1TPCDigi()
{
	;
}
