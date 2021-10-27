#include <bits/stdc++.h>
using namespace std;

struct grammarElement {
	// 我们假定产生式的左部和右部由一个空格分格
	// 右边由 '|' 分成若干部分
	string formula;	// 产生式
};

vector<grammarElement> gramOldSet;	// 原始文法的产生式集
set<char> terSymbol;				// 终结符号
set<char> non_ter;					// 非终结符号
set<char> allSymbol;				// 所有符号
map<char, set<char>> firstSet;		// 各产生式右部的 FIRST 集
map<char, int> ifGetFirstSet;		// 标记一个符号是否已经求过 FIRST 集
map<char, set<char>> followSet;		// 各产生式左部的 FOLLOW 集
map<char, int> ifGetFollowSet;		// 标记一个符号是否已经求过 FOLLOW 集
vector<vector<int>> M;				// 分析表

// 将一个产生式的右边根据 '|' 拆解
vector<string> getFormula(grammarElement t) {
	int pos = 0;
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

void getFirstSet(char);











void getFirstSet(char x) {
	string ff;
	vector<string> cc;
	if (terSymbol.find(x) == -1) return;
	for (const auto& item : gramOldSet) {
		if (item.formula[0] == x) {
			cc = getFormula(item);
			for (const auto& word : cc) {
				if (word[0] == '?' || terSymbol.find(word[0]) != -1)
					firstSet[x].insert(word[0]);
				if (non_ter.find(word[0]) != -1) {
					if (word[0] == x) continue;

				}

			}
		}
	}
}