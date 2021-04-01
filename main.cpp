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

    int getNum(char letterToCheck, char * uniqueLetters, int uniqueLetterAmount) {
        for (int i = 0; i < uniqueLetterAmount; i++) {
            if (toupper(uniqueLetters[i]) == letterToCheck) {
                return this->combination[i];
            }
        }
        return -1;
    }

    bool checkConstraint(string augend, string addend, string sum, char * uniqueLetters, int uniqueLetterAmount) {
        int carryOver = 0;
        bool unknownCurrent = false;
        int augendNum = 0, addendNum = 0, sumNum = 0;
        int i = augend.length()-1, j = addend.length()-1, k = sum.length()-1;
        while (i >= 0 || j >= 0 || k >= 0) {
            augendNum = 0, addendNum = 0, sumNum = 0;
            if (i >= 0) {
                augendNum = getNum(augend.at(i), uniqueLetters, uniqueLetterAmount);
                if (augendNum == -1) {
                    unknownCurrent = true;
                    i--, j--, k--;
                    continue;
                } else if (i == 0) {
                    if (getNum(augend.at(i), uniqueLetters, uniqueLetterAmount) == 0) {
                        return false;
                    }
                }
            }
            if (j >= 0) {
                addendNum = getNum(addend.at(j), uniqueLetters, uniqueLetterAmount);
                if (addendNum == -1) {
                    unknownCurrent = true;
                    i--, j--, k--;
                    continue;
                } else if (j == 0) {
                    if (getNum(addend.at(j), uniqueLetters, uniqueLetterAmount) == 0) {
                        return false;
                    }
                }
            }
            if (k >= 0) {
                sumNum = getNum(sum.at(k), uniqueLetters, uniqueLetterAmount);
                if (sumNum == -1) {
                    unknownCurrent = true;
                    i--, j--, k--;
                    continue;
                } else if (k == 0) {
                    if (getNum(sum.at(k), uniqueLetters, uniqueLetterAmount) == 0) {
                        return false;
                    }
                }
            }
            //cout << augend.at(i) << augendNum << " + " << addend.at(j) << addendNum << " = " << sum.at(k) << sumNum << endl;
            
            if (unknownCurrent == true) {
                
                unknownCurrent = false;
                if (augendNum + addendNum + 0 == sumNum) {
                    carryOver = 0;
                } else if (augendNum + addendNum + 0 == sumNum + 10) {
                    carryOver = 1;
                } else if (augendNum + addendNum + 1 == sumNum) {
                    carryOver = 0;
                } else if (augendNum + addendNum + 1 == sumNum + 10) {
                    carryOver = 1;
                } else {
                    return false;
                }
            } else {
                if (augendNum + addendNum + carryOver == sumNum) {
                    carryOver = 0;
                } else if (augendNum + addendNum + carryOver == sumNum + 10) {
                    carryOver = 1;
                } else {
                    return false;
                }
            }
            i--, j--, k--;
        }
        return true;
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
        cerr << "ERROR: Correct amount of arguments must be supplied. Correct form:" << endl;
        cerr << "./hw1 searchType augend addend sum outputFileName" << endl;
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

    //cout << "Tree construction time: " << ( (float) clock() - runtime ) / CLOCKS_PER_SEC << endl;

    int visitedNodeAmount = 0;
    int maxNodes = 0;
    int * answer = NULL;

    if (search_type.compare("BFS") == 0) {
        queue<Node*> q;
        Node* traverser;
        q.push(mytree->root);
        while(q.empty() == false) {
            if (q.size() > maxNodes) {
                maxNodes = q.size();
            }
            traverser = q.front();
            q.pop();
            visitedNodeAmount++;
            //traverser->print(uniqueLetterAmount);
            if (traverser->checkConstraint(augend, addend, sum, uniqueLetters, uniqueLetterAmount) == true) {
                if (traverser->rank == uniqueLetterAmount) {
                    answer = traverser->combination;
                    break;
                }
                if (traverser->rank != uniqueLetterAmount) {
                    for (int i = 0; i < 10 - traverser->rank; i++) {
                        q.push(traverser->childs[i]);
                    }
                }
            }
        }
    } else if (search_type.compare("DFS") == 0) {

    } else {
        cerr << "ERROR: Invalid search type. Please use either BFS or DFS as first argument." << endl;
        exit(1);
    }

    bool answerExists = false;
    if (answer != NULL) {
        answerExists = true;
    } else {
        answerExists = false;
        answer = new int[uniqueLetterAmount];
        for (int i = 0; i < uniqueLetterAmount; i++) {
            answer[i] = -1;
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
                if (i + 1 != uniqueLetterAmount) {
                    outFile << endl;
                }
            }
        }
    }

    outFile.close();

    cout << "Algorithm: " << search_type << endl;
    cout << "Number of visited nodes: " << visitedNodeAmount << endl;
    cout << "Maximum number of nodes kept in the memory: " << maxNodes << endl;

	runtime = clock() - runtime; // calculate program execution time

	cout << "Running time: " << ( (float) runtime ) / CLOCKS_PER_SEC << " seconds" << endl;

    cout << "Solution: ";

    if (answerExists == false) {
        cout << "Answer could not be found" << endl;
    } else {
        for (int i = 0; i < uniqueLetterAmount; i++) {
            cout << (char)toupper(uniqueLetters[i]) << ": " << answer[i];
            if (i + 1 != uniqueLetterAmount) {
                cout << ", ";
            } else {
                cout << endl;
            }
        }
    }

	return 0;
}