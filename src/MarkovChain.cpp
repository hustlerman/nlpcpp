#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <deque>

class MarkovChain {
    private:
    std::map<std::string, std::vector<std::string*>*>* ngrams;
    std::map<std::string, std::string*>* dictionary;
    int order;

    public:
    MarkovChain(int order) {
        ngrams = new std::map<std::string, std::vector<std::string*>*>();
        dictionary = new std::map<std::string, std::string*>();
        this->order = order;
    }

    void readText(std::istream& corpus) {
        std::string temp;
        std::deque<std::string> currentNgram;
        std::string currentNgramString;
        //Initialize the first Ngram and beginning of the dictionary
        for(int i = 0; i < order && corpus >> temp; i++) {
            currentNgram.push_back(temp);
            if(dictionary->find(temp) == dictionary->end()) {
                dictionary->insert(std::pair<std::string, std::string*>(temp, new std::string(temp)));
            }
        }
        if(currentNgram.size() != order) {
            return;
        }
        while(corpus >> temp) {
            //Construct a string from the ngram deque
            currentNgramString = currentNgram.at(0);
            for(int i = 1; i < currentNgram.size(); i++) {
                currentNgramString += " " + currentNgram.at(i);
            }
            //Check to see if the next word is in the dictionary and get its pointer
            std::string* word;
            if(dictionary->find(temp) == dictionary->end()) {
                word = new std::string(temp);
                dictionary->insert(std::pair<std::string, std::string*>(temp, word));
            } else {
                word = dictionary->at(temp);
            }
            //Check to see if the current ngram already exists,
            //create if it doesn't, otherwise, append it to current list
            std::vector<std::string*>* follow;
            if(ngrams->find(currentNgramString) == ngrams->end()) {
                follow = new std::vector<std::string*>();
                ngrams->insert(std::pair<std::string, std::vector<std::string*>*>(currentNgramString, follow));
            } else {
                std::vector<std::string*>* follow = ngrams->at(currentNgramString);
            }
            follow->push_back(word);
            currentNgram.pop_front();
            currentNgram.push_back(temp);
        }
    }

    void printNGrams() {
        std::map<std::string, std::vector<std::string*>*>::iterator it;
        std::vector<std::string*>::iterator jt;
        for(it = ngrams->begin(); it != ngrams->end(); ++it) {
            std::cout << it->first << ':';
            jt = it->second->begin();
            std::cout << "[" << **jt;
            ++jt;
            for(jt; jt != it->second->end(); ++jt) {
                std::cout << ", " << **jt;
            }
            std::cout << "]" << std::endl;
        }
    }

    std::string generateText() {
        std::map<std::string, std::vector<std::string*>*>::iterator it;
        if(ngrams->size() == 0) {
            return "";
        }
        std::advance( it, rand()%ngrams->size() );
        std::string currentNgram = it->first;
        std::vector<std::string*> result;
        std::string resultString = currentNgram;
        for(int i = 0; i < 30; i++) {
            if (ngrams->find(currentNgram) == ngrams->end()) {
                break;
            }
            std::vector<std::string*>* possibilities = ngrams->at(currentNgram);
            if(possibilities->size() == 0) {
                break;
            }
            std::string* nextWord = possibilities->at(rand()%possibilities->size());
            result.push_back(nextWord);
            resultString += " " + *nextWord;
            currentNgram = "";
            for(int j = result.size()-order; j < result.size(); j++) {
                currentNgram += *result.at(j);
            }
        }
        return resultString;
    }
};

// int main() {
//     MarkovChain test(3);
//     std::ifstream corpus("beemovie.txt");
//     test.readText(corpus);
//     test.printNGrams();
//     return 0;
// }