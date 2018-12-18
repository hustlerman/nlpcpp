#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "MarkovChain.cpp"

int main() {
    std::map<std::string, MarkovChain*> users;
    std::vector<std::string> names;
    users.insert(std::pair<std::string, MarkovChain*>("Sierra LaPlante", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Kristen Lovell", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Brendan Micciche", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Sam Rappl", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Angela Hoyt", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Megan Hood", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Katie Hazel", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Jeremy Kelly", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Kayla Oh", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Chris B", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Chloe Campbell", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Ryan Gramzay", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Emi G", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Nick Latz", new MarkovChain(1)));
    users.insert(std::pair<std::string, MarkovChain*>("Matt Wyant", new MarkovChain(1)));
    names.push_back("Sierra LaPlante");
    names.push_back("Kristen Lovell");
    names.push_back("Brendan Micciche");
    names.push_back("Sam Rappl");
    names.push_back("Angela Hoyt");
    names.push_back("Megan Hood");
    names.push_back("Katie Hazel");
    names.push_back("Jeremy Kelly");
    names.push_back("Kayla Oh");
    names.push_back("Chris B");
    names.push_back("Chloe Campbell");
    names.push_back("Ryan Gramzay");
    names.push_back("Emi G");
    names.push_back("Nick Latz");
    names.push_back("Matt Wyant");
    std::ifstream groupchat("groupchat.txt");
    std::string line;
    std::getline(groupchat, line);
    std::fstream message;
    //Load training data from all of the users
    while(users.find(line) != users.end()) {
        MarkovChain* user = users.at(line);
        std::cout << "Reading text for " << line << std::endl;
	    message.open("temp.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
        while(std::getline(groupchat, line) && !line.empty() && users.find(line) == users.end()) {
            message << line << std::endl;
            std::cout << line << std::endl;
        }
        message.seekg(0, std::ios::beg);
        user->readText(message);
        message.close();
    }

    for(std::map<std::string, MarkovChain*>::iterator it = users.begin(); it != users.end(); ++it) {
        it->second->printNGrams();
    }

    //Generate a group-chat!
    std::ofstream generatedchat("lemon.txt");
    for(int i = 0; i < 100; i++) {
        std::string name = names.at(rand()%names.size());
        MarkovChain* currUser = users.at(name);
        currUser->printNGrams();
        std::cout << "\n" << std::endl;
        std::cout << "Beginning to generate text for " << name << "!" << std::endl;
        generatedchat << name << ": \n" << currUser->generateText() << std::endl;
    }
    return 0;
}
