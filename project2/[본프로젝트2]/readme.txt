CS322 Formal Languages and Automata

20150645 Sungmin Im

Platform: OS X El Capitan 10.11.6; Xcode: v8.0 (8A218a); Macbook Pro 13’’ Retina (Early 2015)
Language: C++, Python2.7
Source code: main.cpp; ply_enfa.py
Input file: re.txt
Output file: e-nfa.txt, m-dfa.txt

How to Execute
Run ply_enfa.py with Python2.7 in terminal with input file re.txt in the same directory as the source file. This will create e-nfa.txt file in the directory. Then, run main.cpp in the same directory. This creates m-dfa.txt.

*ply directory should be in the same directory as main.cpp, ply_enfa.py, and input files.

——————————————————————————————————————————————————————————————————————————————————

The program takes 5 different types of symbols : (numbers and alphabet), +, *, (, )

According to the rule
expression is :
1) symbol
2) (expression)
3) expression + expression
4) ()
5) expression*
the code parses the given regular expression and makes an AST.

Then, the python code interprets the AST to make an e-NFA.
There are 4 functions, eNFA_OR, eNFA_CONCAT, eNFA_STAR, eNFA_SYMBOL.
These functions form e-NFA following the rules in textbook TP 3.
e.g.)
If a symbols is given, two states are connected by the given symbol.
If CONCAT is given, two e-NFA is connected by epsilon.
. . .

The program makes an output file e-nfa.txt using the e-NFA made from the previous algorithm.

Then, main.cpp gets the e-nfa.txt file, and makes m-dfa.txt.
main.cpp is same as the previous project.
If there is no symbol or no transition function, etc, there is extra empty line in m-dfa.txt file.



