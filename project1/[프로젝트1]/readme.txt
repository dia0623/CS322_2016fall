CS322 Formal Languages and Automata

20150645 Sungmin Im

Platform: OS X El Capitan 10.11.6; Xcode: v8.0 (8A218a); Macbook Pro 13’’ Retina (Early 2015)
Language: C++
Source code: main.cpp
Input file: mealy.txt
Header file: linux_kbhit.h

How to Execute
1. Run in terminal with header file and input file in the same directory as main.cpp.
or
Run in Xcode with input file in the working directory of the project.

2. To use 받침우선, type 1 and to use 초성우선, type 2. The terminal will be cleared and you are ready to give your own input.

3. This program uses real time input-output. Your input will directly be showed on the terminal.

4. To erase, use <.

5. This program is not defensively programmed. Inappropriate input will terminate the program with an error.

More Descriptions
This program uses deterministic finite automata with 11 states, each representing 1 or more type of Hangeul character. It has std::vector<int> current_word that contains the last inputs. 3 integer variables, cho_ind, jung_ind, and jong_ind contains the index of last 초성, 중성, 종성 in current_word. These are some examples:
objective: 괡
input: r
current_word: <ㄱ>
input: h
current_word: <ㄱ, ㅗ>
input: o
current_word: <ㄱ, ㅗ, ㅙ>
input: f
current_word: <ㄱ, ㅗ, ㅙ, ㄹ>
input: r
current_word: <ㄱ, ㅗ, ㅙ, ㄹ, ㄹㄱ>

After each input, it prints out the current word by combining the characters at cho_ind, jung_ind, and jong_ind. When it is done with the current word(if more input comes in), it adds the current word to wchar_t* w, which is the final output string. This system is same for both 초성우선 and 받침우선, only the method of printing out characters are different.

To erase, if current_word is not empty, it simply erases the elements in current_word one by one. If current_word is empty, it erases the characters in w.