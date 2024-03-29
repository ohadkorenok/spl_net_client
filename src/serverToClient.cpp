#include "../include/serverToClient.h"

using namespace std;

ServerToClient::ServerToClient(ConnectionHandler &connectionHandler, bool *isTerminated, bool *logoutNotsent) : handler(
        connectionHandler), isTermiated(isTerminated), logoutNotsent(logoutNotsent) {}

short ServerToClient::bytesToShort(char *bytesArr, int indexOfstart, int indexTofinish) {
    short result = (short) ((bytesArr[indexOfstart] & 0xff) << 8);
    result += (short) (bytesArr[indexTofinish] & 0xff);
    return result;
}

void ServerToClient::operator()() {
    while (!*isTermiated) {
        char bytes[2];
        handler.getBytes(bytes, 2);
        short opCode = ServerToClient::bytesToShort(bytes, 0, 1);
        switch (opCode) {
            case 9: { //Notifcation
                char byteStoGet[1];
                handler.getBytes(byteStoGet, 1);
                char type = byteStoGet[0];
                short notificationtype = (short) type;
                string postingUser;
                handler.getFrameAscii(postingUser, '\0');
                postingUser = postingUser.substr(0, postingUser.length() - 1);
                string content;
                handler.getFrameAscii(content, '\0');
                content = content.substr(0, content.length() - 1);
                if (notificationtype == 0) { // PM
                    cout << "NOTIFICATION PM " + postingUser + " " + content << endl;
                } else if (notificationtype == 1) { // Public
                    cout << "NOTIFICATION Public " + postingUser + " " + content << endl;
                }
                break;
            }
            case 10: { // ACK
                handler.getBytes(bytes, 2);
                short ackmessageOpcode = ServerToClient::bytesToShort(bytes, 0, 1);
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
                            toAdd = "";
                            handler.getFrameAscii(toAdd, '\0');
                            toAdd = toAdd.substr(0, toAdd.length() - 1);
                            collectingUsers.append(toAdd + " ");
                        }
                        toAdd = "";
                        handler.getFrameAscii(toAdd, '\0');
                        toAdd = toAdd.substr(0, toAdd.length() - 1);
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
                            toAdd = "";
                            handler.getFrameAscii(toAdd, '\0');
                            toAdd = toAdd.substr(0, toAdd.length() - 1);
                            collectingUsers.append(toAdd + " ");
                        }
                        toAdd = "";
                        handler.getFrameAscii(toAdd, '\0');
                        toAdd = toAdd.substr(0, toAdd.length() - 1);
                        collectingUsers.append(toAdd);
                        cout << "ACK " + std::string(std::to_string((int) ackmessageOpcode)) + std::string(" ") +
                                std::string(std::to_string((int) numofUsersToFollow)) + " " + collectingUsers
                             << endl;
                        break;
                    }
                    case 8: {
                        char moreBytes[6];
                        handler.getBytes(moreBytes, 6);
                        short numofPosts = ServerToClient::bytesToShort(moreBytes, 0, 1);
                        short numOfFollowers = ServerToClient::bytesToShort(moreBytes, 2, 3);
                        short numOfFollowing = ServerToClient::bytesToShort(moreBytes, 4, 5);
                        string str1 = (to_string((int) numofPosts));
                        string str2 = (to_string((int) numOfFollowers));
                        string str3 = (to_string((int) numOfFollowing));
                        cout << "ACK " + std::string(std::to_string((int) ackmessageOpcode)) + " " + str1 + " " + str2 +
                                " " + str3 << endl;
                        break;
                    }
                    default:
                        cout << "ACK " + std::string(std::to_string((int) ackmessageOpcode)) << endl;
                        break;
                }
                break;
            }
            case 11: { //Error
                handler.getBytes(bytes, 2);
                short errormessageOpcode = ServerToClient::bytesToShort(bytes, 0, 1);
                if (errormessageOpcode == 3) {
                    *logoutNotsent = true;
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
