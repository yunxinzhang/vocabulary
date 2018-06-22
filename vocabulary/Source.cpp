// vs2015 win10 64bit
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <windows.h> 
#include "pystring.h"
#include <random>
#include <ctime>
#include <random>
#include <algorithm>
//windows.h 必须在 pystring.h 的前面，否则Sleep函数找不到
using namespace std;

map<int, string> id2word;
map<string, int> word2id;
map<int, string> id2meaning;
map<string, pair<int,int>> allWord2info;
map<string, pair<int, int>> testWord2info;
int numberOfTest  = 10;
void readWordList() {
	ifstream fin("vocabulary.txt");
	ifstream fin2("interference.txt");
	if (!fin)cout << "read error!" << endl;
	if (!fin2)cout << "read error2!" << endl;
	char buff[2001];
	string str;
	vector<string> wm;
	int cnt = 0;
	while (!fin.eof()) {
		//cout << 1 << endl;
		fin.getline(buff, 2000);
		str = buff;
		if (str.find("=") == string::npos)continue;
		pystring::split(str, wm, "=");
		id2word.insert(pair<int, string>(cnt, pystring::strip(wm[0])));
		word2id.insert(pair<string, int>(pystring::strip(wm[0]), cnt));
		id2meaning.insert(pair<int, string>(cnt, pystring::strip(wm[1])));
		++cnt;
	}
	while (!fin2.eof()) {
		//cout << 2 << endl;
		fin2.getline(buff, 2000);
		str = buff;
		id2meaning.insert(pair<int, string>(cnt, str));
		++cnt;
	}
	fin.close();
	fin2.close();
	//cout << "read end." << endl;
}
void readInfo() {
	ifstream fin("info.txt");
	if (!fin) {
		//cout << "no info" << endl;
		return;
	}
	else {
		string word;
		int select, wrong;
		while (!fin.eof()) {
// 不支持短语
			fin >> word >> select >> wrong;
			allWord2info.insert(pair<string, pair<int, int>>(word, pair<int, int>(select, wrong)));
			if (word2id.find(word) != word2id.end()) {
				testWord2info.insert(pair<string, pair<int, int>>(word, pair<int, int>(select, wrong)));
			}
		}
	}
	fin.close();
	
	//cout << "read info end." << endl;
}
void getAns(string &str) {
	cin >> str;
}
void initQuestion() {
	for (auto it : word2id) {
		if (allWord2info.find(it.first) == allWord2info.end()) {
			allWord2info.insert(pair<string, pair<int, int>>(it.first, pair<int, int>(0, 0)));
			testWord2info.insert(pair<string, pair<int, int>>(it.first, pair<int, int>(0, 0)));
		}
	}
	vector<pair<string, pair<int, int>>> testWord(testWord2info.begin(), testWord2info.end());
	//cout << testWord.size() << endl;
	//换排序方法
	//********************************************************************
	srand(clock());
	std::random_shuffle(testWord.begin(), testWord.end());
	//********************************************************************
	/*for (auto it : testWord) {
		cout << it.first << " >> " << it.second.first << "\t" << it.second.second << endl;
	}*/
	int testNum = numberOfTest > testWord.size() ? testWord.size() : numberOfTest;
	vector<int> ans;
	for (auto i = 0; i < id2meaning.size(); ++i) {
		ans.push_back(i);
	}
	for (int i = 0; i < testNum; ++i) {
		
		cout << "============" << endl;
		cout << testWord[i].first<< endl;
		int id = word2id.at(testWord[i].first);
		cout << "============" << endl;
		int rightAnswer;
		std::random_shuffle(ans.begin(), ans.end());
		//cout <<">>"<< ans[0]<<"," << ans[1]<<"," << ans[2]<<"," << ans[3] << endl;
		if (ans[0] != id && ans[1] != id && ans[2] != id && ans[3] != id) {
			rightAnswer = rand()%4;
			ans[rightAnswer] = id;
		}
		else {
			if (ans[0] == id) rightAnswer = 0;
			else if (ans[1] == id)rightAnswer = 1;
			else if (ans[2] == id)rightAnswer = 2;
			else if(ans[3]== id)rightAnswer = 3;
		}
		for (int j = 0; j < 4; ++j) {
			cout << j+1 << " >> ";
			cout << id2meaning.at(ans[j]) << endl;
		}
		string outinput;
		cout << "your answer>>";
		cin >> outinput;
		if (outinput[0] > '0' && outinput[0] < '9') {
			cout <<"right answer>>"<< rightAnswer+1 << endl;
			if (rightAnswer+1 == atoi(outinput.substr(0, 1).c_str())) {
				testWord[i].second.first++;
				testWord[i].second.second++;
			}
		}
		else {
			testWord[i].second.first++;
		}
	}
	/*for (auto it : testWord) {
		cout << it.first << "\t" << it.second.first << "\t" << it.second.second << endl;
	}*/
	ofstream out("info.txt");
	for (auto it : testWord) {
		out << it.first << "\t" << it.second.first << "\t" << it.second.second << "\n";
	}
	out.close();
}

int main() {
	cout << "请输入一组测试多少个单词?" << endl;
	cin >> numberOfTest;
	readWordList();
	
	cout << "vocabulary test" << endl;
	string commend;
	do {
		readInfo();
		initQuestion();
		cout << "tips : 记住发音单词就差不多记住了。" << endl;
		cout << "tips : 回答问题快一点记忆效果可能会不错哦。" << endl;
		cout << "在线牛津词典		https://www.oxfordlearnersdictionaries.com" << endl;
		cout << "在线韦伯词典		https://www.merriam-webster.com/" << endl;
		cout << "停止测试请输入：quit, 继续请输入其它字符。" << endl;
		cin >> commend;
	} while (commend != "quit");

	
	system("pause");
}