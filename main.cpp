#include<iostream> // cout, endl
#include<fstream> // ifstream, ofstream
#include<time.h> // clock_t, clock, CLOCKS_PER_SEC
#include<string> // string, getline
#include<queue>

using namespace std;

class Node {
    public:
    char letter;
    Node** childs;  // child pointer holder holder
    int rank;  // level in tree
    int childSize; // amount of children nodes
    int* combination;

    Node(char letter, int rank) {
        this->letter = letter;
        this->childs = new Node * [10 - rank];
        this->rank = rank;
        this->childSize = 0;
    }
    void createCombination(int* parentCombination, int uniqueLetterAmount, int newIndex) {
        int possibilities[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
        this->combination = new int[uniqueLetterAmount];

        for (int i = 0; i < uniqueLetterAmount; i++) {
            if (i < rank - 1) {
                this->combination[i] = parentCombination[i];
                possibilities[parentCombination[i]] = 0;
            } else {
                this->combination[i] = -1;
                if (i == rank - 1) {
                    int k = 0;
                    for (int j = 0; j < 10; j++) {
                        if (possibilities[j] == 1) {
                            if(k == newIndex) {
                                this->combination[i] = j;
                                break;
                            } else {
                                k++;
                            }
                            
                        }
                    }
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

    int checkSum(string st, char * uniqueLetters, int uniqueLetterAmount) {
        int sum = 0;
        for (int i = st.length() - 1, j = 0; i >= 0; i--, j++) {
            int pow = 1;
            for (int k = 0; k < j; k++) {
                pow *= 10;
            }
            int k = 0;
            int charIndex = -1;
            while(k < uniqueLetterAmount) {
                if (toupper(uniqueLetters[k]) == st.at(i)) {
                    charIndex = k;
                }
                k++;
            }
            if (this->combination[charIndex] == -1) {
                return 0;
            }
            sum += pow * this->combination[charIndex];
        }
        return sum;
    }

    int * checkConstraint(string augend, string addend, string sum, char * uniqueLetters, int uniqueLetterAmount) {
        /*
        int sumAugend = checkSum(augend, uniqueLetters, uniqueLetterAmount);
        int sumAddend = checkSum(addend, uniqueLetters, uniqueLetterAmount);
        int sumSum = checkSum(sum, uniqueLetters, uniqueLetterAmount);
        if (sumAugend == 0 || sumAddend == 0 || sumSum == 0) {
            return NULL;
        }
        if (sumAugend + sumAddend == sumSum) {
            return this->combination;
        }*/
        return NULL;
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
        ofstream outFile;
        recursiveCreate(this->root);
    }

    void recursiveCreate(Node* currentNode) {
        if (currentNode->rank == uniqueLetterAmount) {
            return;
        }
        for (int i = 0; i < 10 - currentNode->rank; i++) {
            currentNode->childs[i] = new Node(this->uniqueLetters[currentNode->rank+1], currentNode->rank+1);
            currentNode->childs[i]->createCombination(currentNode->combination, uniqueLetterAmount, i);
            //currentNode->childs[i]->print(uniqueLetterAmount);
            recursiveCreate(currentNode->childs[i]);
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
	inFile.open(augend + " " + addend + " " + sum + ".txt");

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

    cout << ( (float) clock() - runtime ) / CLOCKS_PER_SEC << endl;

    int visitedNodeAmount = 0;

    int * answer = new int[uniqueLetterAmount];

    if (search_type.compare("BFS") == 0) {
        queue<Node*> q;
        Node* traverser;
        q.push(mytree->root);
        while(q.empty() == false) {
            traverser = q.front();
            //traverser->print(uniqueLetterAmount);
            answer = traverser->checkConstraint(augend, addend, sum, uniqueLetters, uniqueLetterAmount);
            visitedNodeAmount++;
            q.pop();
            if (answer != NULL) {
                // found answer
                break;
            }
            if (traverser->rank != uniqueLetterAmount) {
                for (int i = 0; i < 10 - traverser->rank; i++) {
                    q.push(traverser->childs[i]);
                }
            }
        }
    } else if (search_type.compare("DFS") == 0) {

    } else {
        cout << "ERROR: Invalid search type. Please use either BFS or DFS as first argument." << endl;
        exit(1);
    }

    cout << "Algorithm: " << search_type << endl;
    cout << "Number of visited nodes: " << visitedNodeAmount << endl;
    cout << "Maximum number of nodes kept in the memory: " << endl;
    cout << "Solution: ";
    for (int i = 0; i < uniqueLetterAmount; i++) {
        cout << (char)toupper(uniqueLetters[i]) << ": " << answer[i];
        if (i + 1 != uniqueLetterAmount) {
            cout << ", ";
        } else {
            cout << endl;
        }
    }

    ofstream outFile;

    outFile.open(outputFileName);
	
    if (!outFile){
		cerr << "ERROR: Output file cannot be opened!" << endl;
		exit(1);
	}

    outFile << '\t'; // initial empty tab

    for (int i = 0; i < 10; i++) {
        outFile << i;
        if (i != 9) {
            outFile << '\t';
        } else {
            outFile << endl;
        }
    }

    for (int i = 0; i < uniqueLetterAmount; i++) {
        outFile << uniqueLetters[i] << '\t';
        for (int j = 0; j < 10; j++) {
            if (j == answer[i]) {
                outFile << 1;
            } else {
                outFile << '.';
            }
            if (j + 1 != 10) {
                outFile << '\t';
            } else {
                outFile << endl;
            }
        }
    }

    outFile.close();

	runtime = clock() - runtime; // calculate program execution time

	cout << "Running time: " << ( (float) runtime ) / CLOCKS_PER_SEC << " seconds" << endl;

	return 0;
}