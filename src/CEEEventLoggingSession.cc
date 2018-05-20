#include "CEEEventLoggingSession.hh"

CEEEventLoggingSession::CEEEventLoggingSession(G4String coutFile_p, G4String cerrFile_p)
{
	this->coutFile=coutFile_p;
	this->cerrFile=cerrFile_p;
	coutStream.open(coutFile, std::ofstream::out|std::ofstream::trunc);
	cerrStream.open(cerrFile, std::ofstream::out|std::ofstream::app);
}

G4UIsession* CEEEventLoggingSession::SessionStart()
{
	return NULL;
}

G4int CEEEventLoggingSession::ReceiveG4cout(const G4String& output)
{
	coutStream << output << std::flush;
	return 0;
}

G4int CEEEventLoggingSession::ReceiveG4cerr(const G4String& output)
{
	cerrStream << output << std::flush;
	return 0;
}

CEEEventLoggingSession::~CEEEventLoggingSession()
{
	coutStream.close();
	cerrStream.close();
}
