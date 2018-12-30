#include "clientToServer.h"
#include "regex"

ClientToServer::ClientToServer(ConnectionHandler &connectionHandler,bool *isTerminated,bool *logoutNotsent) : handler(connectionHandler) , isTerminated(isTerminated) ,logoutNotsent(logoutNotsent) {}

using namespace std;

static void shortToBytes(short num, char *bytesArr) {
    bytesArr[0] = ((num >> 8) & 0xFF);
    bytesArr[1] = (num & 0xFF);
}

void ClientToServer::operator()() {

    while (!*isTerminated) {
        const short bufsize = 1024;
        char buf[bufsize];
        if(*logoutNotsent) {
            std::cin.getline(buf, bufsize);
            std::string line(buf);
            string firstWord = line.substr(0, line.find(" "));

        map<string, regex> regexDict = {
                {"REGISTER", regex("REGISTER\\s* (\\w*)\\s*(\\w*)\\s*")},
                {"LOGIN",    regex("LOGIN\\s* (\\w*)\\s*(\\w*)\\s*")},
                {"LOGOUT",   regex("LOGOUT")},
                {"FOLLOW",   regex("FOLLOW\\s*(\\d)\\s*(\\d+)\\s*(.*)")},
                {"POST",     regex("POST\\s(\\.*)")},
                {"PM",       regex("PM\\s*(\\w*)\\s(.*)")},
                {"USERLIST", regex("USERLIST")},
                {"STAT",     regex("STAT \\s* (\\w*)")}
        };
        std::smatch smatch1;
        char *bytesArr = new char[2];
//        bytesArr= {'a','b','c','d'};
        regex_search(line, smatch1, regexDict[firstWord]);
        if (!smatch1.empty()) {
            if (firstWord == "FOLLOW") {
                /** opcode **/
                shortToBytes((short) 4, bytesArr);
                cout << "FOLLOW command. opcode is : "+4;

                handler.sendBytes(bytesArr, 2);
                /** Follow/Unfollow **/

                handler.sendBytes(smatch1[1].str().c_str(), (int) smatch1[1].str().length());
                cout << "FOLLOW command. after follow/unfollow: ";

                /** numberOfUsers **/
//                handler.sendBytes(smatch1[2].str().c_str(), (int) smatch1[2].str().length());
                cout << "FOLLOW command. after num of users: ";

                int numberOfUsers = stoi(smatch1[2].str());
                shortToBytes((short) numberOfUsers, bytesArr);
                handler.sendBytes(bytesArr, 2);

                string stringOfMatch = smatch1[3].str();
                cout << "FOLLOW command. string of match : "+stringOfMatch << endl;
                for (unsigned int i = 0; i < numberOfUsers - 1; ++i) {
                    int space = stringOfMatch.find(" ");
                    string userName = stringOfMatch.substr(0, space);
                    cout << " Username is : "+userName << endl;

                    handler.sendFrameAscii(userName, '\0');
                    stringOfMatch = stringOfMatch.substr(space + 1);
                }
                cout << " Username is : "+stringOfMatch << endl;

                handler.sendFrameAscii(stringOfMatch, '\0');
            }

            if (firstWord == "REGISTER") {
                shortToBytes((short) 1, bytesArr);
                handler.sendBytes(bytesArr, 2);
                string userName = smatch1[1].str();
                string password = smatch1[2].str();
                cout << "Register command. Username is : "+userName+" and Password is : "+password << endl;
                handler.sendFrameAscii(userName, '\0');
                handler.sendFrameAscii(password, '\0');
            }
            if (firstWord == "LOGIN") {
                shortToBytes((short) 2, bytesArr);
                handler.sendBytes(bytesArr, 2);
                string userName = smatch1[1].str();
                string password = smatch1[2].str();
                handler.sendFrameAscii(userName, '\0');
                handler.sendFrameAscii(password, '\0');
            }
            if (firstWord == "LOGOUT") {
                shortToBytes((short) 3, bytesArr);
                handler.sendBytes(bytesArr, 2);
            }
            if (firstWord == "POST") {
                shortToBytes((short) 5, bytesArr);
                handler.sendBytes(bytesArr, 2);
                string content = smatch1[1];
                handler.sendFrameAscii(content, '\0');
            }
            if (firstWord == "PM") {
                shortToBytes((short) 6, bytesArr);
                handler.sendBytes(bytesArr, 2);
                string userName = smatch1[1];
                string content = smatch1[2];
                handler.sendFrameAscii(userName, '\0');
                handler.sendFrameAscii(content, '\0');
            }
            if (firstWord == "USERLIST") {
                shortToBytes((short) 7, bytesArr);
                handler.sendBytes(bytesArr, 2);
            }
            if (firstWord == "STAT") {
                shortToBytes((short) 8, bytesArr);
                handler.sendBytes(bytesArr, 2);
                string userName = smatch1[1];
                handler.sendFrameAscii(userName, '\0');
            }
        } else {
            cout << "no match found!" << endl;
        }

    }


}
