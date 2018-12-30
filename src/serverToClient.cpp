#include "serverToClient.h"
using namespace std;

ServerToClient:: ServerToClient(ConnectionHandler &connectionHandler,bool *isTerminated,bool *logoutNotsent): handler(connectionHandler) , isTermiated(isTerminated), logoutNotsent(logoutNotsent) {}

short ServerToClient::bytesToShort(char *bytesArr, int indexOfstart, int indexTofinish){
    short result = (short)((bytesArr[indexOfstart] & 0xff) << 8);
    result += (short)(bytesArr[indexTofinish] & 0xff);
    return result;
}

void ServerToClient::operator()() {
    while (!*isTermiated) {
        char bytes[4];
        handler.getBytes(bytes, 4);
        short opCode = ServerToClient::bytesToShort(bytes, 0, 1);
        switch (opCode) {
            case 9: { //Notifcation

                short notificationtype = ServerToClient::bytesToShort(bytes, 2, 3);
                string postingUser;
                handler.getFrameAscii(postingUser, '\0');
                string content;
                handler.getFrameAscii(content, '\0');
                if (notificationtype == 0) { // PM
                    cout << "NOTIFICATION PM " + postingUser + " " + content << endl;
                } else if (notificationtype == 1) { // Public
                    cout << "NOTIFICATION Public " + postingUser + " " + content << endl;
                }
                break;
            }
            case 10: { // ACK
                short ackmessageOpcode = ServerToClient::bytesToShort(bytes, 2, 3);
                switch (ackmessageOpcode) {
                    case 3: {
                        *isTermiated = true;
                        cout << "ACK " + std::string(std::to_string((int) ackmessageOpcode)) << endl;
                        break;
                    }
                    case 4: {
                        char moreBytes[2];
                        handler.getBytes(moreBytes, 2);
                        short numofUsersToFollow = ServerToClient::bytesToShort(moreBytes, 0, 1);
                        string collectingUsers = "";
                        string toAdd = "";
                        for (int i = 0; i < numofUsersToFollow - 1; i++) {
                            handler.getFrameAscii(toAdd, '\0');
                            collectingUsers.append(toAdd);
                        }
                        toAdd = "";
                        handler.getFrameAscii(toAdd, '\0');
                        collectingUsers.append(toAdd);
                        cout << "ACK " + std::string(std::to_string((int) ackmessageOpcode)) + std::string(" ") +
                                std::string(std::to_string((int) numofUsersToFollow)) + " " + collectingUsers << endl;
                        break;
                    }
                    case 7: {
                        char moreBytes[2];
                        handler.getBytes(moreBytes, 2);
                        short numofUsersToFollow = ServerToClient::bytesToShort(moreBytes, 0, 1);
                        string collectingUsers = "";
                        string toAdd = "";
                        for (int i = 0; i < numofUsersToFollow - 1; i++) {
                            handler.getFrameAscii(toAdd, '\0');
                            collectingUsers.append(toAdd);
                        }
                        toAdd = "";
                        handler.getFrameAscii(toAdd, '\0');
                        collectingUsers.append(toAdd);
                        cout << "ACK " + std::string(std::to_string((int) ackmessageOpcode)) + std::string(" ") +
                                std::string(std::to_string((int) numofUsersToFollow)) + " " + collectingUsers
                             << endl;
                        break;
                    }
                    default:
                        cout << "ACK " + std::string(std::to_string((int) ackmessageOpcode)) << endl;
                        break;
                }
                break;
            }
                case 11: { //Error
                    short errormessageOpcode = ServerToClient::bytesToShort(bytes, 2, 3);
                    if(errormessageOpcode==3){
                        *logoutNotsent=true;
                    }
                    cout << "ERROR " + std::string(std::to_string((int) errormessageOpcode)) << endl;
                    break;
                }
                default: {
                    std::cout << "Didn't recieve any recongizable message from socket" << std::endl;
                    break;
                }
            }
        }
    }
