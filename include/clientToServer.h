#ifndef CLIENTTOSERVER
#define CLIENTTOSERVER

#include "connectionHandler.h"

class ClientToServer{
public:
    ClientToServer(ConnectionHandler &connectionHandler);
    void operator()();

private:
    ConnectionHandler &handler;
};



#endif
