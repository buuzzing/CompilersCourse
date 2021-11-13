#include "ss2.1.cpp"

void TEST_INIT() {
    // INIT();
	cout << "beginSym: " << beginSym << endl;
    cout << "terSymbol: ";
	for (const auto & ch : terSymbol)
		cout << ch << ' ';
	cout << "\nnon_ter: ";
	for (const auto & ch : non_ter)
		cout << ch << ' ';
	cout << "\nallSymbol: ";
	for (const auto & ch : allSymbol)
		cout << ch << ' ';
	cout << endl;
	cout << endl;
	for (const auto & f : gramOldSet)
		cout << f.formula << endl;
	cout << endl;
}

void TEST_getFirstSet() {
	// 获取所有符号的 First 集
	while(true) {
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
	// 删去可能被计算的 ? 集
	firstSet.erase('?');
	cout << "FIRST SET" << endl;
	for (const auto & item : firstSet) {
		cout << item.first << ":\t";
		for (const auto & ch : item.second)
			cout << ch << ' ';
		cout << endl;
	}
}

void TEST_getFollowSet() {
	// 获取非终结符的 Follow 集
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
	cout << "FOLLOW SET" << endl;
	for (const auto & item : followSet) {
		cout << item.first << ":\t";
		for (const auto & ch : item.second)
			cout << ch << ' ';
		cout << endl;
	}
}

void TEST_getM() {
	getM();
	char n_ter = '\0';
	for (const auto & item : M)
		cout << "<" << item.first.first << ", " << item.first.second
			<< "> " << item.second << endl;
}