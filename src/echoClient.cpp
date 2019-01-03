#include <stdlib.h>
#include <iostream>
#include <thread>
#include "../include/clientToServer.h"
#include "../include/serverToClient.h"
#include "../include/connectionHandler.h"


/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);

    ConnectionHandler connectionHandler(host, port);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }
    bool *isTerminated = new bool;
    *isTerminated = false;
    bool *logoutNotsent = new bool;
    *logoutNotsent = true;
    ClientToServer cts = ClientToServer(connectionHandler, isTerminated, logoutNotsent);
    ServerToClient stc = ServerToClient(connectionHandler, isTerminated, logoutNotsent);
    std::thread nitzanWork(std::ref(stc));
    std::thread ohadWork(std::ref(cts));
//
    nitzanWork.join();
    ohadWork.join();

    delete (isTerminated);
    delete (logoutNotsent);
    return 0;
}
