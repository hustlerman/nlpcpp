#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <deque>

class MarkovChain {
    private:
    std::map<std::string, std::vector<std::string>*> ngrams;
    int order;

    public:
    MarkovChain(int order) {
        this->order = order;
    }

    void readText(std::ifstream& corpus) {
        std::string ngram;
        std::string qTemp;
        std::deque<std::string> q;
        corpus >> ngram;
        for(int i = 0; i < order - 1 && corpus >> qTemp; i++) {
            q.push_back(qTemp);
        }
        while(corpus >> qTemp) {
            q.push_back(qTemp);
            if(ngrams.find(ngram) != ngrams.end()) {
                std::vector<std::string>* follow = new std::vector<std::string>();
                for(int i = 0; i < order; i++) {
                    follow->push_back(q.at(i));
                }
                ngrams.insert(std::pair<std::string, std::vector<std::string>*>(ngram, follow));
            } else {
                std::vector<std::string>* follow = ngrams.at(ngram);
                for(int i = 0; i < order; i++) {
                    follow->push_back(q.at(i));
                }
            }
            ngram = q.front();
            q.pop_front();
        }
    }

    void printNGrams() {
        std::map<std::string, std::vector<std::string>*>::iterator it;
        for(it = ngrams.begin(); it != ngrams.end(); ++it) {
            std::cout << it->first << ':' << it->second << std::endl;
        }
    }
};

int main() {
    MarkovChain test(3);
    std::ifstream corpus("beemovie.txt");
    test.readText(corpus);
    test.printNGrams();
    return 0;
}