#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

vector<string> readFile(ifstream &infile) {
        vector<string> v;
	if(infile.is_open()) {
                while(infile.good()) {
                        string line = "";
                        getline(infile, line);
                        v.push_back(line);
                }
        } else {
                cout << "ERROR\n";
        }
        return v;
}

vector<string> readFile(const string &filename) {
	ifstream infile{filename};
	return readFile(infile);
}

string toTitleCase (const string &input) {
	if(input.size() > 0) {
		string in = input;
		transform(in.begin(), in.end(), in.begin(), ::toupper);
		transform(in.begin()+1, in.end(), in.begin()+1, ::tolower);
		return in;
	} else {
		return input;
	}
}

int main(int argc, char* argv[]) {
	//initialize the random number generator with the current time
	srand(time(NULL));
	//read firstnames.txt and lastnames.txt and randomly generate a full name;

	vector<string> first_names = readFile("FirstNames.txt");
	vector<string> last_names = readFile("LastNames.txt");

	string random_first = toTitleCase(first_names[rand() % first_names.size()]);
	string random_last = toTitleCase(last_names[rand() % last_names.size()]);
	transform(random_first.begin()+1, random_first.end(), random_first.begin()+1, ::tolower);
	cout << random_first + " " + random_last << endl;

	return 0;
}


//WRITE TITLE CASE FUNCTION
