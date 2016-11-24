//
//  main.cpp
//  pre2-1
//
//  Created by Sungmin Im on 2016. 10. 31..
//  Copyright © 2016년 Sungmin Im. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

vector<string> states;
vector<string> final_states;
vector<char> symbols;
vector< vector< vector<string> > > table;
vector< vector<string> > eclosure;
string ini;

//result of delta(state, symbol)
vector<string> delta(string state, char symbol){
    vector<string>::iterator iter;
    vector<char>::iterator iter2;
    int i = 0, j = 0;
    
    for (iter = states.begin(); iter != states.end(); iter ++){
        if (state.compare(*iter) == 0) break;
        else i ++;
    }
    for (iter2 = symbols.begin(); iter2 != symbols.end(); iter2 ++){
        if (*iter2 == symbol) break;
        else j ++;
    }
    
    return table.at(i).at(j);
}

//put element delta(old_state, symbol) = new_state
void put(string old_state, char symbol, string new_state){
    vector<string>::iterator iter;
    vector<char>::iterator iter2;
    int i = 0, j = 0;
    
    for (iter = states.begin(); iter != states.end(); iter ++){
        if (old_state.compare(*iter) == 0){
            break;
        }
        else i ++;
    }
    for (iter2 = symbols.begin(); iter2 != symbols.end(); iter2 ++){
        if (*iter2 == symbol) break;
        else j ++;
    }
    
    table.at(i).at(j).push_back(new_state);
    return;
}

int indexof(vector<string> list, string target){
    int i;
    
    for(i = 0; i < list.size(); i ++){
        if (list.at(i).compare(target) == 0)
            return i;
    }
    return -1;
}

int indexofsym(vector<char> list, char c){
    int i;
    
    for (i = 0; i < list.size(); i ++){
        if (list.at(i) == c)
            return i;
    }
    return -1;
}

int exists(vector< vector<string> > v_set, vector<string> v) {
    //vector< vector<string> >::iterator iter;
    int iter;
    vector<string>::iterator iter2, iter3;
    
    for (iter = 0; iter < v_set.size(); iter ++){
        vector<string> cmp = v_set.at(iter);
        
        if(cmp.size() == v.size()){
            iter3 = cmp.begin();
            for (iter2 = v.begin(); iter2 < v.end(); iter2 ++){
                if ((*iter2).compare(*iter3))
                    break;
                else{
                    iter3 ++;
                }
            }
            if (iter2 == v.end()) return iter;
        }
    }
    return -1;
}

void readText(string filename){
    ifstream nfa;
    string state, symbol, func, fin;
    string input_string;
    string current_state;
    int i, j;
    
    nfa.open(filename);
    
    //read states
    nfa>>state;
    nfa>>state;
    
    i = 0;
    j = 0;
    while(j < state.size()){
        if (state.at(j) != ',')
            j ++;
        else{
            string subs = state.substr(i, j - i);
            states.push_back(subs);
            //cout << i << j << subs << endl;
            i = j + 1;
            j = i;
        }
    }
    string subs = state.substr(i, j - i);
    states.push_back(subs);
    
    //read symbols
    nfa>>symbol;
    nfa>>symbol;
    nfa>>symbol;
    i = 0;
    if (symbol.compare("State")){
        while(i < symbol.size()){
            if (i % 2 == 0)
                symbols.push_back(symbol.at(i));
            i ++;
        }
        nfa >> func;
        nfa >> func;
        nfa >> func;
    }
    else{
        nfa >> func;
        nfa >> func;
    }
    symbols.push_back('E');
    
    //read state transition function
    table.assign(states.size(), vector< vector<string> >(symbols.size()));
    
    while(true){
        nfa>>func;
        if (func == "Initial") break;
        
        string new_state, old_state;
        char sym;
        
        i = 0; j = 0;
        while(func.at(j) != ','){
            j ++;
        }
        old_state = func.substr(i, j - i);
        j ++;
        sym = func.at(j ++);
        new_state = func.substr(j + 1, func.size() - j);
        
        put(old_state, sym, new_state);
    }
    nfa >> func;
    
    //initial state
    nfa >> ini;
    //final state
    nfa >> fin; nfa >> fin;
    nfa >> fin;
    
    i = 0;
    j = 0;
    while(j < fin.size()){
        if (fin.at(j) != ',')
            j ++;
        else{
            string subs = fin.substr(i, j - i);
            final_states.push_back(subs);
            
            i = j + 1;
            j = i;
        }
    }
    subs = fin.substr(i, j - i);
    final_states.push_back(subs);
    
    nfa.close();
}

vector<string> mk_eclosure(int k){
    bool *visited = new bool[states.size()];
    vector<string> eclo;
    for(int i = 0; i < states.size(); i ++)
        visited[i] = false;
    
    list<int> queue;
    
    visited[k] = true;
    queue.push_back(k);
    
    list<int>::iterator i;
    
    while(!queue.empty()){
        k = queue.front();
        eclo.push_back(states.at(k));
        queue.pop_front();
        
        for(int j = 0; j < states.size(); j ++){
            vector<string>::iterator p;
            for(p = table.at(k).at(symbols.size() -1).begin(); p != table.at(k).at(symbols.size() - 1).end(); p ++){
                if((*p).compare(states.at(j)) == 0){
                    if (!visited[j]){
                        visited[j] = true;
                        queue.push_back(j);
                    }
                }
            }
        }
    }
    return eclo;
}

bool is_final(char x, vector<char> fin){
    vector<char>::iterator i;
    
    for(i = fin.begin(); i != fin.end(); i ++){
        if ((*i) == x)
            return true;
    }
    return false;
}

vector<char> non_final(vector<char> list, vector<char> fin){
    vector<char> result;
    
    vector<char>::iterator i;
    
    for(i = list.begin(); i != list.end(); i ++){
        if(!is_final(*i, fin))
            result.push_back(*i);
    }
    
    return result;
}

bool contains(vector<string> list, vector<string> elem){
    vector<string>::iterator i, j;
    
    for(i = list.begin(); i != list.end(); i ++){
        for (j = elem.begin(); j != elem.end(); j ++){
            if((*i).compare(*j) == 0)
                return true;
        }
    }

    return false;
}

vector<char> intersect(vector<char> x, vector<char> y){
    vector<char> result;
    vector<char>::iterator i, j;
    
    for (i = x.begin(); i != x.end(); i ++){
        for (j = y.begin(); j != y.end(); j ++){
            if((*i) == (*j)){
                result.push_back(*i);
                break;
            }
        }
    }
    
    return result;
}

vector<char> complement(vector<char> x, vector<char> y){
    vector<char> result;
    vector<char>::iterator i, j;
    
    for (i = x.begin(); i != x.end(); i ++){
        for(j = y.begin(); j != y.end(); j ++){
            if ((*i) == (*j)) break;
        }
        if (j == y.end()) result.push_back(*i);
    }
    
    return result;
}

bool same(vector<char> x, vector<char> y){
    if(x.size() == y.size())
        if(intersect(x, y).size() == x.size())
            return true;
    return false;
}

char find(list< vector<char> > P, char k){
    list< vector<char> >::iterator i;
    
    for (i = P.begin(); i != P.end(); i ++){
        vector<char>::iterator j;
        
        for(j = i->begin(); j != i->end(); j ++){
            if (k == (*j))
                return (*i).at(0);
        }
    }
    return '.';
}

int main() {
    ofstream mdfa;
    vector<string> current_closure;
    vector< vector<string> > v_set;
    vector<char> dfa_state;
    vector<char> dfa_final;
    vector< vector<char> > dfa_table;
    char s = 'A';
    int i, j;
    
    readText("e-nfa.txt");
    mdfa.open("m-dfa.txt");
    
    //make e-closure for all states
    for (i = 0; i < states.size(); i ++){
        eclosure.push_back(mk_eclosure(i));
    }
    
    current_closure = eclosure.at(indexof(states, ini)); //vector<string>
    sort(current_closure.begin(), current_closure.end());
    v_set.push_back(current_closure);
    dfa_state.push_back(s);
    if(contains(current_closure, final_states)){
        dfa_final.push_back(s);
    }
    s ++;
    dfa_table.push_back(vector<char>(symbols.size() - 1, '.'));
    int dfa_ind = 0;
    
    while(dfa_ind < dfa_state.size()){
        vector<string>::iterator iter;
        vector<char>::iterator iter2;
        for (iter2 = symbols.begin(); iter2 != symbols.end() - 1; iter2 ++){ // for each symbol
            char sym = *iter2;
            vector<string> v;
            for (iter = v_set.at(dfa_ind).begin(); iter != v_set.at(dfa_ind).end(); iter ++){ // for each state in the closure
                vector<string> f = delta(*iter, sym);
                vector<string>::iterator iter3;
                
                for(iter3 = f.begin(); iter3 != f.end(); iter3 ++){ // for each result of delta(state, symbol)
                    vector<string> e = eclosure.at(indexof(states, *iter3));
                    v.insert(v.end(), e.begin(), e.end());
                }
            }
            //remove duplicate in v and sort
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            if (v.size() == 0) continue;
            int ind = exists(v_set, v);
            if(ind < 0){
                v_set.push_back(v);
                dfa_state.push_back(s);
                
                if(contains(v, final_states)){
                    dfa_final.push_back(s);
                }
                
                dfa_table.push_back(vector<char>(symbols.size() - 1, '.')); //for state s
                dfa_table.at(dfa_ind).at(indexofsym(symbols, sym)) = s ++;
            }
            else{
                char c = 'A' + ind;
                dfa_table.at(dfa_ind).at(indexofsym(symbols, sym)) = c;
            }
        }
        dfa_ind ++;
    }
    
    //make m-dfa
    list< vector<char> > P, W;
    P.push_back(dfa_final);
    P.push_back(non_final(dfa_state, dfa_final));
    W.push_back(dfa_final);
    
    list< vector<char> >::iterator it;
    vector<char>::iterator itt;
    
    while(!W.empty()){
        vector<char> a = W.front();
        W.pop_front();
        
        for (i = 0; i < symbols.size() - 1; i ++){ //for each symbol
            vector<char> x;
            vector<char>::iterator iter;
            
            for(iter = a.begin(); iter != a.end(); iter ++){ //for each element in A
                for(j = 0; j < dfa_state.size(); j ++){
                    if (dfa_table.at(j).at(i) == (*iter))
                        x.push_back(dfa_state.at(j));
                }
            }
            
            list< vector<char> >::iterator it;
            for(it = P.begin(); it != P.end(); it ++){
                vector<char> y = *it;
                
                vector<char> inter = intersect(x, y);
                vector<char> comple = complement(y, x);
                
                if(inter.size() != 0 && comple.size() != 0){
                    P.erase(it);
                    P.push_back(inter);
                    P.push_back(comple);
                    
                    list< vector<char> >::iterator k;
                    for(k = W.begin(); k != W.end(); k ++){
                        if(same((*k), y)){
                            W.erase(k);
                            W.push_back(inter);
                            W.push_back(comple);
                            break;
                        }
                    }
                    if(k == W.end()){
                        if(inter.size() <= comple.size())
                            W.push_back(inter);
                        else
                            W.push_back(comple);
                    }
                }
            }
        }
    }
    
    //write text
    mdfa << "State" << endl;
    
    for (it = P.begin(); it != P.end(); it ++){
        if ((*it).size() == 0)
            P.erase(it);
    }
    
    for (it = P.begin(); it != P.end(); it ++){
        if (it == P.begin()){
            mdfa << (*it).at(0);
            continue;
        }
        
        mdfa << "," << (*it).at(0);
    }
    mdfa << endl;
    
    mdfa << "Input Symbol" << endl;
    vector<char>::iterator it1;
    
    symbols.pop_back();
    for (it1 = symbols.begin(); it1 != symbols.end(); it1 ++){
        if (it1 == symbols.begin()){
            mdfa << (*it1);
            continue;
        }
        
        mdfa << "," << (*it1);
    }
    mdfa << endl;
    
    mdfa << "State transition function" << endl;
    int cnt1 = 0;
    for (it = P.begin(); it != P.end(); it ++){
        char curr_state = (*it).at(0);
        
        for(i = 0; i < dfa_state.size(); i ++){
            if (dfa_state.at(i) == curr_state){
                vector<char>::iterator k;
                int ind = 0;
                for (k = dfa_table.at(i).begin(); k != dfa_table.at(i).end(); k ++){
                    if ((*k) != '.'){
                        mdfa << curr_state << ",";
                        mdfa << symbols.at(ind) << ",";
                        
                        //find where (*k) is contained
                        char next_state = find(P, (*k));
                        mdfa << next_state << endl;
                        cnt1 ++;
                    }
                    ind ++;
                }
                break;
            }
        }
    }
    if (cnt1 == 0) mdfa << endl;
    
    mdfa << "Initial state" << endl;
    for (it = P.begin(); it != P.end(); it ++){
        vector<char>::iterator k;
        for (k = it->begin(); k != it->end(); k ++){
            if ((*k) == 'A'){
                mdfa << (*it).at(0) << endl;
                break;
            }
        }
        if (k != it->end()) break;
    }
    
    mdfa << "Final state" << endl;
    int cnt = 0;
    cnt1 = 0;
    for (it = P.begin(); it != P.end(); it ++){
        vector<char>::iterator k, l;
        for (k = it->begin(); k != it->end(); k ++){
            for (l = dfa_final.begin(); l != dfa_final.end(); l ++){
                if ((*k) == (*l)){
                    if (cnt != 0)
                        mdfa << ",";
                    mdfa << (*it).at(0);
                    cnt1 ++;
                    cnt ++;
                    break;
                }
            }
            if (l != dfa_final.end()) break;
        }
    }
    if (cnt1 == 0) mdfa << 'A';
    mdfa << endl;
    
    /*vector< vector<char> >::iterator it11;
    for (it11 = dfa_table.begin(); it11 != dfa_table.end(); it11 ++){
        vector<char>::iterator it2;
        for (it2 = it11->begin(); it2 != it11->end(); it2 ++){
            cout << *it2 << " ";
        }
        cout << endl;
    }*/
    
    mdfa.close();
    
    return 0;
}
