#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<string> states;
vector<string> final_states;
vector<char> symbols;
vector< vector<string> > table;

//result of delta(state, symbol)
string f(string state, char symbol){
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

    table.at(i).at(j) = new_state;
    return;
}

bool exists(string curr) {
	vector<string>::iterator iter;

	for (iter = final_states.begin(); iter != final_states.end(); iter ++) {
		if (curr.compare(*iter) == 0) return true;
	}
	return false;
}

int main(){
    ifstream dfa, input;
    ofstream output;
    string state, symbol, func, ini, fin;
	string input_string;
	string current_state;
    int i, j, k;

    dfa.open("m-dfa.txt");
    input.open("input.txt");
    output.open("output.txt");

    //read states
    dfa>>state;
    dfa>>state;

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
    dfa>>symbol;
    dfa>>symbol;
    dfa>>symbol;
    i = 0;
    while(i < symbol.size()){
        if (i % 2 == 0)
            symbols.push_back(symbol.at(i));
        i ++;
    }

    //read state transition function
    table.assign(states.size(), vector<string>(symbols.size(), "."));

    dfa >> func; dfa >> func; dfa >> func;
    while(true){
        dfa>>func;
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
    dfa >> func;

    //initial state
    dfa >> ini;
    //final state
    dfa >> fin; dfa >> fin;
    dfa >> fin;

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

	getline(input, input_string);	
	while(true){
		current_state = ini;
	
		string nextline;
		getline(input, nextline);

		//if(nextline.compare("") == 0) k = input_string.size();
		//else k = input_string.size() - 1;

		for (i = 0; i < input_string.size() - 1; i ++){
			current_state = f(current_state, input_string.at(i));
			if (current_state.compare(".") == 0){
				break;
			}
		}
		if (exists(current_state)){
			output << "네" << endl;
		}
		else{
			output << "아니오" << endl;
		}

		input_string = nextline;
		if(input_string.compare("") == 0) break;
	}

    dfa.close();
    input.close();
    output.close();

    return 0;
}
