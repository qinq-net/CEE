#ifndef T1TPCDigi_H
#define T1TPCDigi_H

#include <G4MultiFunctionalDetector.hh>

class T1TPCDigi: public G4MultiFunctionalDetector
{
	public:
	T1TPCDigi(G4String name, G4int depth);
	virtual ~T1TPCDigi();
};

#endif//T1TPCDigi_H
