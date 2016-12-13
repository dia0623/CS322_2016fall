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

5. This program is not defensively programmed. Inappropriate input will terminate the program with an error. This includes typing z(획) on ㅊ.

6. This program uses KT나랏글 as following
1	2	3
ㄱ	ㄴ	ㅏㅓ
Q	W	E
ㄹ	ㅁ	ㅗㅜ
A	S	D
ㅅ	ㅇ	ㅣ
Z	X	C
획	ㅡ	쌍

More Descriptions
This program uses deterministic finite automata with 46 states, each representing 1 or more type of Hangeul character. It has std::vector<int> current_word which is a buffer that contains the last inputs. 3 integer variables, cho_ind, jung_ind, and jong_ind contains the index of last 초성, 중성, 종성 in current_word. These are some examples:

<contents>(cho_ind, jung_ind, jong_ind)

input: 1
current_word: <ㄱ>(0,0,0)
input: z
current_word: <ㄱ, ㄲ>(1,0,0)
input: e
current_word: <ㄱ, ㄲ, ㅗ>(1,2,0)
input: e
current_word: <ㄱ, ㅗ, ㅜ>(1,2,0)
input: 3
current_word: <ㄱ, ㅗ, ㅜ, ㅝ>(1,3,0)
input: d
current_word: <ㄱ, ㅗ, ㅜ, ㅞ>(1,3,0)
input: q
current_word: <ㄱ, ㅗ, ㅜ, ㅞ, ㄹ>(1,3,4)
input: 1
current_word: <ㄱ, ㅗ, ㅜ, ㅞ, ㄹ, ㄹㄱ>(1,3,5)

In cases where making 겹받침 results in a problem, such as ㄹㄴ->ㄹㄷ->ㄹㅌ and ㄹㅌ is the only accepted 받침. In this case, I used temp_word buffer, which contains the 초성 of next word. If the next input is <ㅏ, ㅗ, ㅣ, ㅡ>, it becomes 초성. If next input is <ㄱ, ㄴ, ㄹ, ㅁ, ㅅ, ㅇ>, it is an error. 

After each input, it prints out the current word by combining the characters at cho_ind, jung_ind, and jong_ind. When it is done with the current word(if more input comes in), it adds the current word to wchar_t* w, which is the final output string. This system is same for both 초성우선 and 받침우선, only the method of printing out characters are different.

To erase, if current_word is not empty, it simply erases the elements in current_word one by one. If current_word is empty, it erases the characters in w. Then, it changes the current state to the appropriate state according to the current and previous character.