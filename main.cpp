#include<iostream> // cout, endl
#include<fstream> // ifstream, ofstream
#include<time.h> // clock_t, clock, CLOCKS_PER_SEC
#include<string> // string, getline

using namespace std;

class Node {
    public:
    char letter;
    Node** childs;  // child pointer holder holder
    int rank;   // level in tree
    int childSize; // amount of children nodes
    int* combination;

    Node(char letter, int rank) {
        this->letter = letter;
        this->childs = new Node * [10];
        this->rank = rank;
        this->childSize = 0;
    }
    void createCombination(int* parentCombination, int uniqueLetterAmount, int newIndex) {
        this->combination = new int[uniqueLetterAmount];
        for (int i = 0; i < uniqueLetterAmount; i++) {
            if (i < rank) {
                this->combination[i] = parentCombination[i];
            } else {
                this->combination[i] = -1;
                if (i == rank) {
                    this->combination[i] = newIndex;
                }
            }
        }
    }
    void print(int size) {
        cout << this->rank << ": ";
        for (int i = 0; i < size; i++) {
            cout << combination[i] << " ";
        }
        cout << '\n';
    }
    void checkConstraint() {
        // chekku shite kadasai
    }
};

class Tree {
    public:
    Node * root;
    int uniqueLetterAmount;
    char * uniqueLetters;

    
    Tree(int uniqueLetterAmount, char* uniqueLetters) {
        this->root = new Node(' ', 0);
        this->root->combination = new int[uniqueLetterAmount];
        for (int i = 0; i < uniqueLetterAmount; i++) {
            this->root->combination[i] = -1;
        }
        this->uniqueLetterAmount = uniqueLetterAmount;
        this->uniqueLetters = uniqueLetters;
        
        recursiveCreate(this->root, 0);
        this->root->combination[0] = -1;
    }

    void recursiveCreate(Node* currentNode, int index) {
        if (index == uniqueLetterAmount) {
            return;
        }
        for (int i = 0; i < 10; i++) {
            currentNode->childs[i] = new Node(this->uniqueLetters[index], index);
            currentNode->childs[i]->createCombination(currentNode->combination, uniqueLetterAmount, i);
            //currentNode->childs[i]->print(uniqueLetterAmount);
            recursiveCreate(currentNode->childs[i], index+1);
        }
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

    char * uniqueLetters = new char[augend.length() + addend.length() + sum.length()]; // max possibility of unique letters

	while(inFile.peek() != EOF) {
        // read unique letters from input matrix
        getline(inFile, data, '\t'); // get unique letter
        uniqueLetters[uniqueLetterAmount++] = data.at(0);
		getline(inFile, data, '\n'); // move to next line
	}

    inFile.close();

    Tree * mytree = new Tree(uniqueLetterAmount, uniqueLetters);

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