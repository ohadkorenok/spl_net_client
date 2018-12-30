#ifndef CLIENTTOSERVER
#define CLIENTTOSERVER

#include "connectionHandler.h"

class ClientToServer{
public:
    ClientToServer(ConnectionHandler &connectionHandler, bool *isTerminated, bool *logoutNotsent);
    void operator()();

private:
    ConnectionHandler &handler;
    std::vector<std::string> parseFollow(std::string match);
    bool *isTerminated;
    bool *logoutNotsent;
};



#endif
