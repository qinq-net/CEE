#ifndef CEEEventLoggingSession_H
#define CEEEventLoggingSession_H

#include <G4UIsession.hh>
#include <iostream>
#include <fstream>

class CEEEventLoggingSession: public G4UIsession {
	public:
	CEEEventLoggingSession(G4String coutFile,G4String cerrFile);
	~CEEEventLoggingSession();

	G4UIsession* SessionStart();
	virtual G4int ReceiveG4cout(const G4String&);
	virtual G4int ReceiveG4cerr(const G4String&);

	private:
	std::ofstream coutStream;
	std::ofstream cerrStream;
	G4String coutFile;
	G4String cerrFile;
};
	

#endif//CEEEventLoggingSession_H
