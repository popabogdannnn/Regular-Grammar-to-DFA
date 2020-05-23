#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

ifstream fin("input.in");
ofstream fout("output.out");

int cntT;

struct regGrammar
{
    map <char, set <pair <char, char> > > transitions;

    regGrammar getEpsilonFree() {
        regGrammar ret;
        
        //copy epsilon free transitions
        for (auto transition : transitions) {
            for (auto it : transition.second) {
                if (it.first != '$') {
                    ret.transitions[transition.first].insert(it);
                }
            }
        }

        set <char> symbolsWithEps;

        for (auto transition : transitions) {
            if (transition.second.find(make_pair('$', 0)) != transition.second.end()  // if epsilon transition is found
            ) { // and its not the starting symbol
                symbolsWithEps.insert(transition.first); // mark that is has epsilon transition
            }
        }

        for (auto transition : transitions) {
            for (auto it : transition.second) {
                if (symbolsWithEps.find(it.second) != symbolsWithEps.end()) { // if we find a transition with a symbol with eps
                    ret.transitions[transition.first].insert(make_pair(it.first, 0));
                    ret.transitions[transition.first].insert(it);
                }
            }
        }

        ret.transitions['['] = ret.transitions['S'];
        ret.transitions['S'].erase(make_pair('$', 0));
        return ret;
    }
};

regGrammar gram;

vector < tuple <char, char, char> > DFAT;
vector <char> finalStates;


int main()
{
    
    for (fin >> cntT, fin.get(); cntT; cntT--) {
        string rd;
        getline(fin, rd);
        if (rd.size() == 3) {
            gram.transitions[rd[0]].insert(make_pair(rd[2], 0));
        }
        else {
            gram.transitions[rd[0]].insert(make_pair(rd[2], rd[3]));
        }
    }

    gram = gram.getEpsilonFree();

    finalStates.push_back('D');
    
    for (auto transition : gram.transitions) {
        for (auto it : transition.second) {
            if (it.first == '$') {
                finalStates.push_back(transition.first);
            }
            else {
                if (it.second == 0) {
                    DFAT.push_back(make_tuple(transition.first, 'D', it.first));
                }
                else {
                    DFAT.push_back(make_tuple(transition.first, it.second, it.first));
                }
            }
        }
    }

    fout << gram.transitions.size() + 1 << " " << DFAT.size() << "\n";
    for (auto it : DFAT) {
        fout << get<0>(it) << " " << get<1>(it) << " " << get<2>(it) << "\n";
    }
    fout << "[\n";
    fout << finalStates.size() << "\n";
    for (auto it : finalStates) {
        fout << it << " ";
    }
    fout << "\n";
    
    return 0;
}

