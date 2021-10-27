#include <bits/stdc++.h>
using namespace std;

const string inputFileName = "task1.txt";
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

vector<string> split_by_line(string);

int isKeyword(const string&);
int isOp(const string&);
bool isOp(char);

void print(int, const string&);
void work(const vector<string>&);

int main() {
    regex p0(R"(//[\s\S]*?\n)");
    regex p1(R"(/\*([\s\S]*?)\*/)");
    regex p2(R"(\n\s+)");
    regex p3(R"([[:blank:]]{2,})");

    ifstream fin(inputFileName);
    stringstream buffer;
    buffer << fin.rdbuf();
    string contents(buffer.str());

    contents = regex_replace(contents, p0, "");
    contents = regex_replace(contents, p1, "");
    contents = regex_replace(contents, p2, "\n");
    contents = regex_replace(contents, p3, " ");

    vector<string> line = split_by_line(contents);

    work(line);
    return 0;
}

vector<string> split_by_line(string contents) {
    vector<string> v;
    string tmp;
    for (const auto & ch : contents) 
        if (ch != '\n') tmp += ch;
        else {
            v.emplace_back(tmp);
            tmp = "";
        }
    return v;
}

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

bool isOp(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
    ch == ':' || ch == '=' || ch == '<' || ch == '>' ||
    ch == ';' || ch == '(' || ch == ')' || ch == '#';
}

void work(const vector<string> &contents) {
    string line;
    string token;
    // 0: illegal char, 1: alpha after number, 2: unpaired brackets
    vector<int> err;
    int lineNum = 0, totErrors = 0;
    for (const auto & l : contents) {
        err.clear();
        line = l;
        ++lineNum;
        cout << "> Line: " << lineNum << endl << line << endl;
        cout << "-----------------------------" << endl;
        token = "";
        for (unsigned i = 0; i < line.length(); ) {
            if (isblank(line[i])) {
                ++i;
            }
            else if (isalpha(line[i])) {
                token = "";
                while (i < line.length() && 
                    (isalpha(line[i]) || isdigit(line[i])))
                    token += line[i++];
                int state = isKeyword(token);
                print(state, token);
            } else if (isdigit(line[i])) {
                token = "";
                while (i < line.length() && isdigit(line[i]))
                    token += line[i++];
                if (i < line.length() && isalpha(line[i])) {
                    err.emplace_back(i);
                    while (i < line.length() 
                        && (isalpha(line[i]) || isdigit(line[i]))) ++i;
                    continue;
                }
                print(26, token);
            } else if (isOp(line[i])) {
                token = "";
                if (i + 1 < line.length() 
                    && line[i] == '/' && line[i + 1] == '*') {
                    err.emplace_back(i);
                    i += 2;
                    continue;
                }
                while (i < line.length() && isOp(line[i]) 
                    && isOp(token + line[i]) != -1)
                    token += line[i++];
                int state = isOp(token);
                print(state, token);
            } else {
                err.emplace_back(i);
                ++i;
            }
        }
        cout << "-----------------------------" << endl;
        cout << err.size() << " Error(s)" << endl;
        if (!err.empty()) {
            totErrors += err.size();    
            cout << line << endl;
            int pre_error_pos = 0;
            for (const auto & pos : err) {
                for (int i = 0; i < pos - pre_error_pos; ++i) cout << ' ';
                cout << '^';
                pre_error_pos = pos + 1;
            }
            cout << endl;
        }
        totErrors += err.size();
        cout << endl;
    }
    cout << "Total Line(s): " << lineNum << endl;
    cout << "Total Error(s): " << totErrors << endl;
}

void print(int syn, const string &token) {
    if (~syn) 
        cout << "< " << syn << ", " << token << " >" << endl;
    else
        cout << "< " << 25 << ", " << token << " >" << endl;
}