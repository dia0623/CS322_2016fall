CS322 Formal Languages and Automata

20150645 Sungmin Im

Platform: OS X El Capitan 10.11.6; Xcode: v8.0 (8A218a); Macbook Pro 13’’ Retina (Early 2015)
Language: C++
Source code: main.cpp
Input file: e-nfa.txt
Output file: m-dfa.txt

How to Execute
Run in terminal with input file in the same directory as main.cpp.
or
Run in Xcode with input file in the working directory of the project.

Algorithm
A) To make dfa from e-nfa
1. find epsilon-closure for all states
2. in std::vector<std::vector<std::string>> v_set, put the e-closure of initial state
3. while there is no unchecked closure in v_set, do the following:
	1) for each symbol, and for each state in the closure, calculate delta(state, symbol)
	2) merge the e-closure of resulting state of state transition into std::vector<std::string> v
	3) if v is in v_set, just update dfa_table
	4) if v is not in v_set, make a new dfa state and update dfa_table

B) To make m-dfa from dfa
1. make a partition of all states - final and non-final
2. if states in a set in partition each leads to different set in partition, divide the set
3. continue until there’s nothing wrong!

** In A-3) and A-4) of my algorithm, if v is empty, I did not assign new state to my dfa. Therefore, my dfa is partial function. In my dfa_table, such states with no assigned transition function for certain input symbol is filled with ‘.’. If the element of the table is ‘.’, state transition function is not printed in m-dfa.txt.


Example
Homework 2 Question 2
Textbook p.106 Example 3.8 Figure 3.18(c)
Regular Expression for this e-NFA: (0 + 1)* 1 (0 + 1)
(** in my dfa, 0 -> a 1 -> b, so (a + b)* b (a + b)

e-nfa:
State
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
Input symbol
a,b
State transition function
0,E,1
0,E,7
1,E,2
1,E,3
2,a,4
3,b,5
4,E,6
5,E,6
6,E,1
6,E,7
7,E,8
8,b,9
9,E,10
10,E,11
10,E,12
11,a,13
12,b,14
13,E,15
14,E,15
Initial state
0
Final state
15

resulting m-dfa:
State
E,D,C,A
Input Symbol
a,b
State transition funtion
E,a,D
E,b,E
D,a,A
D,b,C
C,a,D
C,b,E
A,a,A
A,b,C
Initial state
A
Final state
E,D

test case:
aaba
baaba
baaabaab

b
ba
bab
bbbaaa

expected result:
네
네
아니요
아니요
아니요
네
아니요
아니요

the test case input files and output files are in the same folder.