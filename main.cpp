#include<iostream> // cout, endl
#include<fstream> // ifstream, ofstream
#include<time.h> // clock_t, clock, CLOCKS_PER_SEC
#include<string> // string, getline

using namespace std;

class Node {
    char letter;
    Node** childs;  // child pointer holder holder
    int rank;   // level in tree
    int childSize; // amount of children nodes

    public:
    Node(char letter, int rank) {
        this->letter = letter;
        this->childs = new Node * [10 - rank];
        this->rank = rank;
        this->childSize = 0;
    }

    void createMatrix(char * uniqueLetters, int uniqueLetterAmount) {

    }

};

int main (int argc, char** argv) {
    
	clock_t runtime; // used for running time
	runtime = clock();

    string search_type;
    string augend;
    string addend;
    string sum;
    string outputFileName;

	if (argc == 6) {
        search_type = argv[1];
        augend = argv[2];
        addend = argv[3];
        sum = argv[4];
        outputFileName = argv[5];
	} else {
        cout << "ERROR: Correct amount of arguments must be supplied. Correct form:" << endl;
        cout << "./hw1 searchType augend addend sum outputFileName" << endl;
        exit(1);
	}
	
	ifstream inFile;
	inFile.open(augend.append(" ").append(addend).append(" ").append(sum).append(".txt"));

	if (!inFile){
		cerr << "ERROR: Input file cannot be opened!" << endl;
		exit(1);
	}

	string data; // input file iterator
	
	getline(inFile, data, '\n'); // this is the first line of input

	int uniqueLetterAmount = 0;

    char * uniqueLetters = new char[augend.length() + addend.length() + sum.length()];

    Node * root = new Node(' ', 0); // create node of empty matrix

	while(inFile.peek() != EOF) {
        // read unique letters from input matrix
        getline(inFile, data, '\t'); // get unique letter
        uniqueLetters[uniqueLetterAmount++] = data.at(0);
        cout << uniqueLetters[uniqueLetterAmount - 1] << endl;
		getline(inFile, data, '\n'); // move to next line
	}

    inFile.close();

    if (search_type.compare("BFS") == 0) {

    } else if (search_type.compare("DFS") == 0) {

    } else {
        cout << "ERROR: Invalid search type. Please use either BFS or DFS as first argument." << endl;
        exit(1);
    }

    cout << "Algorithm: " << search_type << endl;
    cout << "Number of visited nodes: " << endl;
    cout << "Maximum number of nodes kept in the memory: " << endl;
    cout << "Solution: " << endl;

    ofstream outFile;

    outFile.open(outputFileName);
	
    if (!outFile){
		cerr << "ERROR: Output file cannot be opened!" << endl;
		exit(1);
	}

    outFile.close();

	runtime = clock() - runtime; // calculate program execution time

	cout << "Running time: " << ( (float) runtime ) / CLOCKS_PER_SEC << " seconds" << endl;

	return 0;
}