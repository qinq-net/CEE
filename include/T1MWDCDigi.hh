#ifndef T1MWDCDigi_H
#define T1MWDCDigi_H

#include <G4MultiFunctionalDetector.hh>

class T1MWDCDigi: public G4MultiFunctionalDetector
{
	public:
	T1MWDCDigi(G4String name, G4int depth);
	virtual ~T1MWDCDigi();
};

#endif//T1MWDCDigi_H
