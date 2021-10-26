#include <bits/stdc++.h>
// #define PRINT_HELP
using namespace std;

const string inputFileName = "ss1.1.out";
const string keyword[] = {
	"main", "if", "then", "while", "do", "static",
	"int", "double", "struct", "break", "else", "long",
	"switch", "case", "typedef", "char", "return", "const",
	"float", "short", "continue", "for", "void", "sizeof", "default"
};
const string op[] = {
	"+", "-", "*", "/", ":", ":=",
	"<", "<>", "<=", ">", ">=", "=",
	";", "(", ")", "#"
};

int isKeyword(const string &t) {
	if (t == "default") return 39;
	for (int i = 0; i < 25; ++i)
		if (keyword[i] == t) return i + 1;
	return -1;
}

int isOp(const string &t) {
	if (t == "#") return 0;
	for (int i = 0; i < 12; ++i)
		if (op[i] == t) return i + 27;
	for (int i = 12; i < 15; ++i)
		if (op[i] == t) return i + 29;
	return -1;
}

bool isChar(char ch) {
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

bool isNum(char ch) {
	return isdigit(ch);
}

bool isBlack(char ch) {
	return ch == ' ' || ch == '\t';
}

bool isOp(char ch) {
	return ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
	ch == ':' || ch == '=' || ch == '<' || ch == '>' ||
	ch == ';' || ch == '(' || ch == ')' || ch == '#';
}

void print(int state, const string &token, bool isN) {
	unsigned l = token.length();
	if (isN)
		cout << 26 << "\t\t" << '-' << "\t\t\t" << token << endl;
	else {
		cout << (state == -1 ? 25 : state) << "\t\t" << token
			<< (l >= 8 ? "\t" : (l >= 4 ? "\t\t" : "\t\t\t"))
			<< '-' << endl;
	}
}

void work() {
    ifstream fin(inputFileName);
    string line;
    string token;
    vector<pair<int, char>> err;
    int lineNum = 0;
    while (getline(fin, line)) {
        ++lineNum;
        cout << "> Line: " << lineNum << endl << line << endl;
        cout << "-----------------------------" << endl;
        token = "";
        for (int i = 0; i < line.length(); ) {
            if (isBlack(line[i]))
                ++i;
            else if (isChar(line[i])) {
                token = "";
                while (i < line.length() && (isChar(line[i]) || isNum(line[i])))
                    token += line[i++];
                int state = isKeyword(token);
                print(state, token, false);
                // if (state != -1)
                //     cout << state << '\t' << token << '\t' << '-' << endl;
                // else
                //     cout << 25 << '\t' << token << '\t' << '-' << endl;
            } else if (isNum(line[i])) {
                token = "";
                while (i < line.length() && isNum(line[i]))
                    token += line[i++];
                print(0, token, true);
                // cout << 26 << '\t' << '-' << '\t' << token << endl;
            } else if (isOp(line[i])) {
                token = "";
                while (i < line.length() && isOp(line[i]) && isOp(token + line[i]) != -1)
                    token += line[i++];
                int state = isOp(token);
                print(state, token, false);
                // if (state != -1)
                //     cout << state << '\t' << token << '\t' << '-' << endl;
            } else {
                err.emplace_back(lineNum, line[i]);
                ++i;
            }
        }
        cout << endl;
    }
    cout << "Total Line(s): " << lineNum << endl;
    cout << "Total Error(s): " << err.size() << endl;
    if (err.size())
        for (const auto & e : err)
            cout << "Illegal char '" << e.second
            << "' at Line: " << e.first << endl;
    fin.close();
}

#ifdef PRINT_HELP
const string help = "Output would be in this format:\n\n"
                    "> Line: LINENUM\n"
                    "ORIGINAL_LINE_DATA\n"
                    "-----------------------------\n"
                    "syn\t\ttoken\t\tsum\n\n"
                    "When and only when the word is a number, "
                    "the \"sum\" part would not be a '-'.\n"
                    "And when and only when the word is a number, "
                    "the \"token\" part would be a '-'.\n";
#endif

int main(int argc, char* argv[]) {
#ifdef PRINT_HELP
    cout << help << endl;
#endif
	work();
	return 0;
}