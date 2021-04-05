#include<iostream> // cout, endl
#include<fstream> // ifstream, ofstream
#include<time.h> // clock_t, clock, CLOCKS_PER_SEC
#include<string> // string, getline
#include<list> // STL list (for BFS and DFS)

using namespace std;

class Node {
    public:
    Node** childs;  // child pointer holder holder
    int rank;  // level in tree
    int* combination; // letter to number arrangement

    Node(char letter, int rank) {
        this->childs = new Node * [10 - rank]; // amount of childs in the tree
        this->rank = rank; // level of this node in the tree
    }
    void createCombination(int* parentCombination, int uniqueLetterAmount, int newIndex) {
        int possibilities[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // possible number assignments (from 0 to 9)
        this->combination = new int[uniqueLetterAmount];

        for (int i = 0; i < uniqueLetterAmount; i++) {
            if (i < rank - 1) {
                this->combination[i] = parentCombination[i]; // copy already assigned letters from parent
                possibilities[parentCombination[i]] = 0; // remove the numbers from possibilities
            } else {
                this->combination[i] = -1; // assign the non-assigned letters as -1
                if (i == rank - 1) {
                    // assign the current node's letter
                    int k = 0;
                    for (int j = 0; j < 10; j++) {
                        if (possibilities[j] == 1) {
                            if(k == newIndex) {
                                // assign according to this node's position according to parent
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
        // function returns the assigned number for the requested letter
        for (int i = 0; i < uniqueLetterAmount; i++) {
            if (toupper(uniqueLetters[i]) == letterToCheck) {
                return this->combination[i];
            }
        }
        return -1;
    }

    bool checkConstraint(string augend, string addend, string sum, char * uniqueLetters, int uniqueLetterAmount) {
        // function returns whether the letter to number assignment follows the constraints
        int carryOver = 0; // carry out from previous equation (this will be next equation's carry in)
        bool unknownCurrent = false; // stores whether constraints were checked for previous equation
        int augendNum = 0, addendNum = 0, sumNum = 0; // holds assigned number of letter
        int i = augend.length()-1, j = addend.length()-1, k = sum.length()-1; // iterators for augend, addend, sum
        while (i >= 0 || j >= 0 || k >= 0) {
            augendNum = 0, addendNum = 0, sumNum = 0; // reset number assignments
            if (i >= 0) {
                augendNum = getNum(augend.at(i), uniqueLetters, uniqueLetterAmount);
                if (augendNum == -1) {
                    // if augend letter is unassigned, skip equation
                    unknownCurrent = true;
                    i--, j--, k--;
                    continue;
                } else if (i == 0) {
                    if (getNum(augend.at(i), uniqueLetters, uniqueLetterAmount) == 0) {
                        // if this is the leftmost letter and it is 0
                        return false;
                    }
                }
            }
            if (j >= 0) {
                addendNum = getNum(addend.at(j), uniqueLetters, uniqueLetterAmount);
                if (addendNum == -1) {
                    // if addend letter is unassigned, skip equation
                    unknownCurrent = true;
                    i--, j--, k--;
                    continue;
                } else if (j == 0) {
                    if (getNum(addend.at(j), uniqueLetters, uniqueLetterAmount) == 0) {
                        // if this is the leftmost letter and it is 0
                        return false;
                    }
                }
            }
            if (k >= 0) {
                sumNum = getNum(sum.at(k), uniqueLetters, uniqueLetterAmount);
                if (sumNum == -1) {
                    // if sum letter is unassigned, skip equation
                    unknownCurrent = true;
                    i--, j--, k--;
                    continue;
                } else if (k == 0) {
                    if (getNum(sum.at(k), uniqueLetters, uniqueLetterAmount) == 0) {
                        // if this is the leftmost letter and it is 0
                        return false;
                    }
                }
            }
            
            if (unknownCurrent == true) {
                // if previous equation was not checked (carry in of current eq is unknown)
                unknownCurrent = false; // reset var as current eq is being checked
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
                // if previous equation was checked (carry in of current eq is known)
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
        return true; // if everything passes, return true for now
    }
};

class Tree {
    public:
    Node * root;
    int uniqueLetterAmount;
    char * uniqueLetters;
    
    Tree(int uniqueLetterAmount, char* uniqueLetters) {
        this->root = new Node(' ', 0); // manually create root node and its combination
        this->root->combination = new int[uniqueLetterAmount];
        for (int i = 0; i < uniqueLetterAmount; i++) {
            this->root->combination[i] = -1;
        }
        this->uniqueLetterAmount = uniqueLetterAmount;
        this->uniqueLetters = uniqueLetters;
        recursiveCreate(this->root);
    }

    void recursiveCreate(Node* currentNode) {
        // recursively creates the tree
        if (currentNode->rank == uniqueLetterAmount) {
            // if leaf node is reached, return back
            return;
        }
        for (int i = 0; i < 10 - currentNode->rank; i++) {
            currentNode->childs[i] = new Node(this->uniqueLetters[currentNode->rank+1], currentNode->rank+1);
            currentNode->childs[i]->createCombination(currentNode->combination, uniqueLetterAmount, i);
            recursiveCreate(currentNode->childs[i]);
        }
    }
};

int main (int argc, char** argv) {
    
	clock_t runtime; // used for running time of program
	runtime = clock(); // get start time

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
	
    /* read unique letters from file */

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

    /* execute the requested search */

    int visitedNodeAmount = 0;
    int maxNodes = 0; // max nodes kept in memory
    int * answer = NULL;

    if (search_type.compare("BFS") == 0) {
        list<Node*> q;
        Node* traverser;
        q.push_back(mytree->root); // push root to queue
        while(q.empty() == false) {
            // execute until queue is empty
            if (q.size() > maxNodes) {
                // check for maximum amount of nodes kept in memory
                maxNodes = q.size();
            }
            traverser = q.front(); // get front-most node from queue
            q.pop_front(); // remove this node from queue
            visitedNodeAmount++;
            if (traverser->checkConstraint(augend, addend, sum, uniqueLetters, uniqueLetterAmount) == true) {
                if (traverser->rank == uniqueLetterAmount) {
                    // if constraint holds for node and it is a leaf node, we have found our answer
                    answer = traverser->combination;
                    break;
                }
                if (traverser->rank != uniqueLetterAmount) {
                    // if constraint holds for node and it is not a leaf node, we add its children to the queue
                    for (int i = 0; i < 10 - traverser->rank; i++) {
                        q.push_back(traverser->childs[i]);
                    }
                }
            }
        }
    } else if (search_type.compare("DFS") == 0) {
        list<Node*> s; // stack
        Node* traverser;
        s.push_back(mytree->root);
        while(s.empty() == false) {
            // execute until stack is empty
            if (s.size() > maxNodes) {
                // check for maximum amount of nodes kept in memory
                maxNodes = s.size();
            }
            traverser = s.back(); // get top-most node from stack
            s.pop_back(); // remove this node from queue
            visitedNodeAmount++;
            if (traverser->checkConstraint(augend, addend, sum, uniqueLetters, uniqueLetterAmount) == true) {
                if (traverser->rank == uniqueLetterAmount) {
                    // if constraint holds for node and it is a leaf node, we have found our answer
                    answer = traverser->combination;
                    break;
                }
                if (traverser->rank != uniqueLetterAmount) {
                    // if constraint holds for node and it is not a leaf node, we add its children to the queue
                    for (int i = 0; i < 10 - traverser->rank; i++) {
                        s.push_back(traverser->childs[i]);
                    }
                }
            }
        }
    } else {
        cerr << "ERROR: Invalid search type. Please use either BFS or DFS as first argument." << endl;
        exit(1);
    }

    /* check whether answer was found */

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

    /* write to file */

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

    /* write to console */

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