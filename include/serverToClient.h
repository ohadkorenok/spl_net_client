//
// Created by koreo@wincs.cs.bgu.ac.il on 12/28/18.
//

#ifndef serverToClient
#define serverToClient

#include "connectionHandler.h"

class ServerToClient{
public:
    ServerToClient(ConnectionHandler &connectionHandler);
    void operator()();

private:
    ConnectionHandler &handler;

};
#endif
