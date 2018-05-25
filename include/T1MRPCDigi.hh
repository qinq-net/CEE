#ifndef T1MRPCDigi_H
#define T1MRPCDigi_H

#include <G4MultiFunctionalDetector.hh>
#include "CEEPSMRPCScorer.hh"

class T1MRPCDigi: public G4MultiFunctionalDetector
{
	public:
	T1MRPCDigi(G4String name, CEEMRPCDirection direction, G4int depth);
	virtual ~T1MRPCDigi();
};

#endif//T1MRPCDigi_H
