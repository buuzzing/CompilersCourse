#include <bits/stdc++.h>
#define LOCAL
using namespace std;

struct grammarElement {
	// 我们假定产生式的左右两边由一个空格分格
	// 右边由 '|' 分成若干部分
	grammarElement(string f = ""): formula(f) {}
	string formula;	// 产生式
};

char beginSym;						//* 文法开始符号
vector<grammarElement> gramOldSet;	//* 原始文法的产生式集
set<char> terSymbol;				//* 终结符号
set<char> non_ter;					//* 非终结符号
set<char> allSymbol;				//* 所有符号
map<char, set<char>> firstSet;		//* 各产生式右部的 FIRST 集
map<char, int> firstSetSize;		//* 各产生式右部 FIRST 集大小
// map<char, bool> ifGetFirstSet;		// 标记一个符号是否已经求过 FIRST 集
map<char, set<char>> followSet;		//* 各产生式左部的 FOLLOW 集
map<char, int> followSetSize;		//* 各产生式左部 FOLLOW 集大小
// map<char, bool> ifGetFollowSet;		// 标记一个符号是否已经求过 FOLLOW 集
map<pair<char, char>, string> M;	//* 分析表

void INIT();				//* 已完成测试
void getFirstSet(char);		//* 已完成测试
void getFollowSet(char);	//* 已完成测试
void getM();				//* 已完成测试
void printM();				//* 已完成测试

/***** TESTPART *****/

/********************/


int main() {
	INIT();
	while(true) {	// 计算 First 集
		for (const auto & ch : allSymbol)
			getFirstSet(ch);
		bool OK = true;
		for (const auto & item : firstSet) {
			if (firstSetSize[item.first] != item.second.size())
				OK = false;
			firstSetSize[item.first] = item.second.size();
		}
		if (OK) break;
	}
	while(true) {	// 计算 Follow 集
		for (const auto & ch : non_ter)
			getFollowSet(ch);
		bool OK = true;
		for (const auto & item : followSet) {
			if (followSetSize[item.first] != item.second.size())
				OK = false;
			followSetSize[item.first] = item.second.size();
		}
		if (OK) break;
	}
	getM();
	printM();
	return 0;
}

// 读入 N 条产生式，并将其存入 gramOldSet
// 同时生成 terSymbol, non_ter 和 allSymbol 集合
// 并将 first 集、follow 集的初始大小置 0
void INIT() {
#ifdef LOCAL
	freopen("a.in" , "r", stdin);
#endif
	string tmp;
	while(getline(cin, tmp)) {
		if (beginSym == '\0') beginSym = tmp[0]; 
		gramOldSet.emplace_back(grammarElement(tmp));
		for (const auto & ch : tmp) {
			if (ch == ' ' || ch == '|' || ch == '?') continue;
			if ('A' <= ch && ch <= 'Z') non_ter.insert(ch);
			else terSymbol.insert(ch);
			allSymbol.insert(ch);
		}
	}
	for (const auto & ch : allSymbol) {
		firstSetSize[ch] = 0;
		followSetSize[ch] = 0;
	}
}

// 将一个产生式的右边根据 '|' 拆解
vector<string> getFormula(grammarElement t) {
	size_t pos = 0;
	while (t.formula[pos] != ' ') ++pos;
	while (t.formula[pos] == ' ') ++pos;
	string tmp;
	vector<string> cc;
	while (pos <= t.formula.length()) {
		if (pos == t.formula.length() || t.formula[pos] == '|') {
			cc.emplace_back(tmp);
			tmp = "";
		} else {
			tmp += t.formula[pos];
		}
		++pos;
	}
	return cc;
}

void getFirstSet(char x) {
	vector<string> cc;
	if (terSymbol.find(x) != terSymbol.end())
		return (void)firstSet[x].insert(x);
	bool hasVoid = false;
	for (const auto & item : gramOldSet)
		if (item.formula[0] == x) {
			cc = getFormula(item);
			for (const auto & word : cc) {
				if (word[0] == '?') {
					firstSet[x].insert(word[0]);	// X -> ? 是产生式
					hasVoid = true;
				} else if (terSymbol.find(word[0]) != terSymbol.end())
					firstSet[x].insert(word[0]);	// X -> a 是产生式
				for (size_t i = 0; i <= word.length(); ++i) {
					if (i == word.length()) {
						// 所有 First(Yj) 都含有 ?，则将 ? 加入 First(X)
						hasVoid = true;
						firstSet[x].insert('?');
						break;
					}
					char ch = word[i];
                    // X -> ...a... 且 a 前的非终结符均可推出 ?
					if (terSymbol.find(ch) != terSymbol.end()) {
                        firstSet[x].insert(ch);
                        break;
                    }
					// X -> Y... 且 Y 是非终结符
					// 则 First(Y) 属于 First(X)
					for (const auto & u : firstSet[ch])
						firstSet[x].insert(u);
					// X -> Y1Y2...Yi-1 均可推出 ? 即继续
					if (firstSet[ch].find('?') == firstSet[ch].end()) break;
				}
			}
		}
	if (!hasVoid) firstSet[x].erase('?');
}

void getFollowSet(char x) {
	vector<string> cc;
	// 对于文法的开始符号 S,置 # 于 FOLLOW(S) 中
	if (x == beginSym) followSet[x].insert('#');
	for (const auto & item : gramOldSet) {
		cc = getFormula(item);
		for (const auto & word : cc) {
			for (int i = 0; i < word.length() - 1; ++i) {
				// 若 A -> xBy
				if (non_ter.find(word[i]) != non_ter.end()) {
					// 则把 First(y) 中的非 ? 符号加入 Follow(B)
					for (const auto & tt : firstSet[word[i + 1]])
						if (tt != '?') followSet[word[i]].insert(tt);
				}
			}
			if (item.formula[0] == x) {
				for (int i = word.length() - 1; i >= 0; --i) {
					// 若 A -> xB 是产生式
					// 则把 FOLLOW(A) 加至 FOLLOW(B)
					for (const auto & tt : followSet[x])
						followSet[word[i]].insert(tt);
					// 若 A -> xBy 是产生式且 y 可星号推出 ?
					// 则把 FOLLOW(A) 加至 FOLLOW(B)
					if (firstSet[word[i]].find('?') == firstSet[word[i]].end())
						break;
				}
			}
		}
	}
}

void getM() {
	for (const auto & n_ter : non_ter)
		for (const auto & ter : terSymbol)
			M[make_pair(n_ter, ter)] = "";
	vector<string> cc;
	char begin;
	for (const auto & item : gramOldSet) {
		cc = getFormula(item);
		begin = item.formula[0];
		for (const auto & word : cc) {
			auto fs = firstSet[word[0]];
			for (const auto & ch : fs)
				M[make_pair(begin, ch)] = (begin + ("->" + word));
			if (word[0] == '?' || fs.find('?') != fs.end())
				for (const auto & ch : followSet[begin])
					M[make_pair(begin, ch)] = (begin + ("->" + word));
		}
	}
}

// 输出预测分析表
void printM() {
	function<void(int)> printLine = [](int num) -> void {
		cout << endl;
		for (int i = 0; i < 70; ++i) cout << '-';
	};
	printLine(70);
	vector<char> t;
	for (const auto & ter : terSymbol)
		t.emplace_back(ter);
	t.emplace_back('#');
	sort(t.begin(), t.end());
	cout << "\n\t";
	for (const auto & ter : t)
		cout << ter << "\t\t\t";
	printLine(70);
	char be = '\0';
	int i, mx = t.size();
	for (const auto & item : M) {
		if (item.first.first != be) {
			be = item.first.first;
			i = 0;
			cout << endl << be << "|\t";
		}
		while (t[i] != item.first.second) {
			i = (i + 1) % mx;
			cout << "\t\t\t";
		}
		cout << item.second;
		if (item.second.length() <= 3) cout << "\t\t\t";
		else if (item.second.length() <= 7) cout << "\t\t";
		else cout << "\t";
		++i;
	}
	cout << endl;
	for (int i = 0; i < 70; ++i) cout << '-';
	cout << endl;
}