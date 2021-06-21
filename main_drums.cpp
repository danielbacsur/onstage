#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <math.h>
using namespace std;

// Time between elements of final array (in seconds)
const double arrayTime = 0.25;
// Input array = {midi value, start time}

const int midi_length = 7;
const int midi[midi_length] = {38, 50, 51, 36, 42, 52, 53};
//snare, tom1, tom2, tom3, hh, ride, crash

void editFile(string str_replace)		//NEEDS EDITING
{
	ostringstream text;
	ifstream in_file("Drums/Melody.cpp");

	text << in_file.rdbuf();
	string str = text.str();
	size_t pos = str.find("STARTMARK");
	size_t pos2 = str.find("//ENDMARK");
	str.replace(pos+1, pos2 - (pos+1), str_replace);
	in_file.close();

	ofstream out_file("Drums/Melody.cpp");
	out_file << str;
}

void writeInteract(int arr[], int as)
{
	string drumsw = to_string(as) + "\n";
	string pianow = "";
	for (int i=0; i<as; i++) {
		drumsw += to_string(arr[i]) + "\n";
		if (arr[i] == 136) {
			pianow += i*arrayTime;
			if (i != as-1) pianow += "\n";
		}
	}
	drumsw += to_string(arrayTime);
	
	//Drums
	fstream dout;
	dout.open("Controller/toDrums.txt", ios::out | ios::trunc);
	dout << drumsw;
	dout.close();
	
	//Piano
	fstream pout;
	pout.open("Controller/toPiano.txt", ios::out | ios::trunc);
	pout << pianow;
	pout.close();
}

int main()
{
	fstream dread;
	dread.open("DrumsRead.txt", ios::in);
	string tp;
	getline(dread, tp)
	int dSize = stoi(tp);
	double d[dSize];
	int counter = 0;
	while (getline(dread, tp)) if (counter != 0) d[counter-1] = stod(tp);
	
	//Generate empty array
	int size = (d[dSize-1])/arrayTime +1; //length of final array: length of track / arrayTime
    
    	string f = "\nconst int d[" + to_string(size+1) + "] = {";
	string uf;
	int of[size];
	for (int t=0;t<size;t++) { // t+1th atTime in g
		uf = "10000000";
		// loop through d
		for (int i = 1; i<dSize; i+=2) {
			if (t*arrayTime == d[i]) {
				for (int j = 0; j<midi_length; j++) {
					if (midi[j] == d[i-1]) {
						uf[j+1] = '1';
					}
				}
			}
			// Is true if at time t p[i-1] is held down at time t
		}
		of[t] = stoi(uf, 0, 2);
		uf = to_string(of[t]) + ", ";
		f += uf;
	}
	f += "128};\nconst int arrSize = " + to_string(size+1) + ";\nconst double arrayTime = " + to_string(arrayTime) + ";\n\n";
	editFile(f);
	writeInteract(of, size);
	//cout << f;
}
