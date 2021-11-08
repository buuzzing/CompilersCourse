#include <bits/stdc++.h>
#define LOCAL
using namespace std;

struct grammarElement {
	// 我们假定产生式的左部和右部由一个空格分格
	// 右边由 '|' 分成若干部分
	grammarElement(string f = ""): formula(f) {}
	string formula;	// 产生式
};

vector<grammarElement> gramOldSet;	// 原始文法的产生式集
set<char> terSymbol;				// 终结符号
set<char> non_ter;					// 非终结符号
set<char> allSymbol;				// 所有符号
map<char, set<char>> firstSet;		// 各产生式右部的 FIRST 集
map<char, int> firstSetSize;		// 各产生式右部 FIRST 集大小
// map<char, bool> ifGetFirstSet;		// 标记一个符号是否已经求过 FIRST 集
map<char, set<char>> followSet;		// 各产生式左部的 FOLLOW 集
map<char, int> followSetSize;		// 各产生式左部 FOLLOW 集大小
// map<char, bool> ifGetFollowSet;		// 标记一个符号是否已经求过 FOLLOW 集
vector<vector<int>> M;				// 分析表

void INIT();				//* 已完成测试
void getFirstSet(char);		//* 已完成测试

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
	for (const auto& item : gramOldSet)
		if (item.formula[0] == x) {
			cc = getFormula(item);
			for (const auto& word : cc) {
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