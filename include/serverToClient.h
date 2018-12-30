//
// Created by koreo@wincs.cs.bgu.ac.il on 12/28/18.
//

#ifndef serverToClient
#define serverToClient

#include "connectionHandler.h"

class ServerToClient{
public:
    ServerToClient(ConnectionHandler &connectionHandler,bool *isTermiatedbool ,bool *logoutNotsent);
    void operator()();
    short bytesToShort(char* bytesArr,int indexOfstart,int indexTofinish);

private:
    ConnectionHandler &handler;
    bool *isTermiated;
    bool *logoutNotsent;


};
#endif
