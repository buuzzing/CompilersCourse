#include <bits/stdc++.h>
using namespace std;

string fileName = "ss1.1.in";
string fileOutputName = "ss1.1.out";

int main() {
	regex p0(R"(#[\s\S]*?\n)");
	regex p1(R"(/\*([\s\S]*?)\*/)");
	regex p2(R"(\n\s+)");
	regex p3(R"([[:blank:]]{2,})");
	ifstream fin(fileName);
	ofstream fout(fileOutputName);
	stringstream buffer;
	buffer << fin.rdbuf();
	string contents(buffer.str());
	cout << "----------------- BEFORE -----------------" << endl << endl;
	cout << contents << endl << endl;
	contents = regex_replace(contents, p0, "");
	contents = regex_replace(contents, p1, "");
	contents = regex_replace(contents, p2, "\n");
	contents = regex_replace(contents, p3, " ");
	cout << "----------------- AFTER -----------------" << endl << endl;
	fout << contents;
	cout << contents << endl << endl;
	return 0;
}