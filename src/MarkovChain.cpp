#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <deque>

class MarkovChain {
    private:
    std::map<std::string*, std::vector<std::string*>*>* ngrams;
    std::vector<std::string*>* startingNgrams;
    std::map<std::string, std::string*>* dictionary;
    std::map<std::string, std::string*>* ngramDictionary;
    int order;

    public:
    MarkovChain(int order) {
        ngrams = new std::map<std::string*, std::vector<std::string*>*>();
        startingNgrams = new std::vector<std::string*>();
        dictionary = new std::map<std::string, std::string*>();
        ngramDictionary = new std::map<std::string, std::string*>();
        this->order = order;
    }

    void readText(std::istream& corpus) {
        std::string temp;
        std::deque<std::string> currentNgram;
        std::string currentNgramString;
        std::string* word;
        std::string* ngram;
        //Initialize the first Ngram and beginning of the dictionary
        for(int i = 0; i < order && corpus >> temp; i++) {
            currentNgram.push_back(temp);
            if(dictionary->find(temp) == dictionary->end()) {
                dictionary->insert(std::pair<std::string, std::string*>(temp, new std::string(temp)));
            }
        }
        if(currentNgram.size() != this->order) {
            return;
        }
        //Create first ngram and store it in starting Ngrams
        currentNgramString = currentNgram.front();
        for(int i = 1; i < currentNgram.size(); i++) {
            currentNgramString += " " + currentNgram.at(i);
        }
        ngram = new std::string(currentNgramString);
        startingNgrams->push_back(ngram);
        
        while(corpus >> temp) {
            //Check to see if the ngram exists in the ngramDictionary
            if(ngramDictionary->find(currentNgramString) == ngramDictionary->end()) {
                ngram = new std::string(currentNgramString);
                ngramDictionary->insert(std::pair<std::string, std::string*>(currentNgramString, ngram));
            } else {
                ngram = ngramDictionary->at(currentNgramString);
            }
            //Check to see if the next word is in the dictionary and get its pointer
            if(dictionary->find(temp) == dictionary->end()) {
                word = new std::string(temp);
                dictionary->insert(std::pair<std::string, std::string*>(temp, word));
            } else {
                word = dictionary->at(temp);
            }
            //Check to see if the current ngram already exists,
            //create if it doesn't, otherwise, append it to current list
            std::vector<std::string*>* follow;
            if(ngrams->find(ngram) == ngrams->end()) {
                follow = new std::vector<std::string*>();
                ngrams->insert(std::pair<std::string*, std::vector<std::string*>*>(ngram, follow));
            } else {
                std::vector<std::string*>* follow = ngrams->at(ngram);
            }
            follow->push_back(word);
            currentNgram.pop_front();
            currentNgram.push_back(temp);
            //Construct the next string from the ngram deque
            currentNgramString = currentNgram.front();
            for(int i = 1; i < currentNgram.size(); i++) {
                currentNgramString += " " + currentNgram.at(i);
            }
        }
    }

    void printNGrams() {
        std::map<std::string*, std::vector<std::string*>*>::iterator it;
        std::vector<std::string*>::iterator jt;
        for(it = ngrams->begin(); it != ngrams->end(); ++it) {
            std::cout << *it->first << ':';
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
        //Randomly select a starting ngram
        std::string currentNgram = *startingNgrams->at(rand()%startingNgrams->size());
        std::string tempNgram = currentNgram;
        std::string *ngramPointer;
        //Split starting ngram into the result vector on spaces
        std::vector<std::string*> result;
        bool split = true;
        while(split) {
            int index = tempNgram.find(' ');
            if(index != -1) {
                std::cout << tempNgram << std::endl;
                result.push_back(dictionary->at(tempNgram.substr(0, index)));
                tempNgram = tempNgram.substr(index+1, tempNgram.length());
                std::cout << tempNgram << std::endl;
            } else {
                split = false;
            }
        }
        std::cout << tempNgram << std::endl;
        std::cout << "Loading first ngram..." << std::endl;
        result.push_back(dictionary->at(tempNgram));
        std::cout << "Loaded first ngram!" << std::endl; 
        std::string resultString = currentNgram;
        for(int i = 0; i < 100; i++) {
            if(ngramDictionary->find(currentNgram) == ngramDictionary->end()) {
                break;
            }
            ngramPointer = ngramDictionary->at(currentNgram);
            if(ngrams->find(ngramPointer) == ngrams->end()) {
                break;
            }
            std::vector<std::string*>* possibilities = ngrams->at(ngramPointer);
            if(possibilities->size() == 0) {
                break;
            }
            std::string* nextWord = possibilities->at(rand()%possibilities->size());
            result.push_back(nextWord);
            resultString += " " + *nextWord;
            currentNgram = "";
            for(int j = result.size()-order; j < result.size(); j++) {
                currentNgram += *result.at(j) + " ";
            }
            //Remove the extra space
            currentNgram = currentNgram.substr(0, currentNgram.size() - 1);
        }
        return resultString;
    }
};

// int main() {
//     MarkovChain test(3);
//     std::ifstream corpus("beemovie.txt");
//     test.readText(corpus);
//     test.printNGrams();
//     std::cout << test.generateText() << std::endl;
//     return 0;
// }