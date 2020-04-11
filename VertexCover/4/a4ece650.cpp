#include <iostream>
#include <vector>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <sstream>
#include <string>
#include <queue>
using namespace std;
using namespace Minisat;

// MARK: function declarations

int stringToInt(string str);
void ShowError(string messege);
vector<int> * ParseEdges(vector<int> * graphMatrix, int numberOfVertices, string operation);
void createGraph(vector <int> * graphMatrix, int numOfVertices, int sourceVertex,int destinationVertex);
void resetLiterals();
void initialiser(Solver *s, int temp_k, int numOfVertices);
void addClauses(Solver *s, int coverSize, int num_of_vertices, vector <int> * graphMatrix);
void minisatSolver(int numOfVertices, vector <int> * graphMatrix);

// MARK: global declarations

vector<int> coverVertices;
bool isParsed = false;
vector<vector<Lit> > minisatGraphLiterals;

// MARK: function definations

vector<int> * ParseEdges(vector<int> * graphMatrix, int numberOfVertices, string operation) {
    bool parathasisFlag = false;
    bool arrowFlag = false;
    bool sepratorFlag = false;
    char seperator = ',';
    string numberString = "";
    int edgeVertex1 = -1;
    int edgeVertex2 = -1;

// MARK: paranthesis matching logic

    for(unsigned int i=0;i<operation.length();i++) {
        if(operation[i] == '{') {
            parathasisFlag = true;
        }
        if(operation[i] == '<' && parathasisFlag == true) {
            arrowFlag = true;
        }
        if(arrowFlag == true && parathasisFlag == true && isdigit(operation[i]) && sepratorFlag == false) {
            numberString += operation[i];
        }
        if (operation[i] == seperator && arrowFlag == true ) {
            sepratorFlag = true;
            int num = stringToInt(numberString);
            edgeVertex1 = num;
            numberString = "";
        }
        if (arrowFlag == false && operation[i] == seperator ) {
            continue;
        }
        if(arrowFlag == true && parathasisFlag == true && isdigit(operation[i]) && sepratorFlag == true) {
            numberString += operation[i];
        }
        if(operation[i] == '>' && parathasisFlag == true && sepratorFlag == true && arrowFlag == true ) {
            int num = stringToInt(numberString);
            edgeVertex2 = num;

            if (edgeVertex1 >= numberOfVertices) {
                ShowError("Error: Invalid edge with vertex > " + to_string(edgeVertex1));
                delete[] graphMatrix;
                isParsed = false;    // parsing error
                graphMatrix =new vector<int>[numberOfVertices];
                break;
            } else if (edgeVertex2 >= numberOfVertices) {
                ShowError("Error: Invalid edge with vertex > " + to_string(edgeVertex2));
                delete[] graphMatrix;
                isParsed = false; // parsing error
                graphMatrix =new vector<int>[numberOfVertices];
                break;
            } else {
                graphMatrix[edgeVertex2].push_back(edgeVertex1);
                graphMatrix[edgeVertex1].push_back(edgeVertex2);
            }
            numberString = "";
            sepratorFlag = false;
            arrowFlag = false;
        }
        if(operation[i] == '}') {
            parathasisFlag = false;
            isParsed = true;   // no parsing error
        }
    }
    return graphMatrix;
}

// MARK: error localisation

void ShowError(string messege) {
    std::cerr<<messege<<endl;
}

void createGraph(vector <int> * graphMatrix, int numOfVertices, int sourceVertex,int destinationVertex) {

    int previousVertexArray[numOfVertices];
    int isTraversed[numOfVertices];
    bool isDestinationFound = false;
    queue <int> traversalQueue;

// MARK: initialisation of arrays

    for (int i = 0;i < numOfVertices;i++) {

        if (i == sourceVertex) {
            isTraversed[i] = 1;
        } else {
            isTraversed[i] = 0;
        }
        previousVertexArray[i] = -1;
    }
    traversalQueue.push(sourceVertex);

    while(!traversalQueue.empty()) {

        int currentNode = traversalQueue.front();
        traversalQueue.pop();
        unsigned long currentNodeSize = graphMatrix[currentNode].size();
        for (unsigned int index = 0 ; index < currentNodeSize ; index ++) {

            if (isTraversed[graphMatrix[currentNode][index]] == 0) {
                isTraversed[graphMatrix[currentNode][index]] = 1;
                previousVertexArray[graphMatrix[currentNode][index]] = currentNode;
                traversalQueue.push(graphMatrix[currentNode][index]);
            }
            if (graphMatrix[currentNode][index] == destinationVertex) {
                isDestinationFound = true;
            }
        }
    }
    if (isDestinationFound == true) {
        // MARK: create path vector
        vector<int> routeArray;
        int currentNode = destinationVertex;
        for(int index = 0 ; previousVertexArray[currentNode] != -1 ;) {
            routeArray.push_back(currentNode);
            currentNode = previousVertexArray[currentNode];
            index++;
        }
    } else {
        ShowError("Error: The vertices are not reachable");
    }
}

// MARK: reset literals

void resetLiterals() {
    minisatGraphLiterals.clear();
}

// MARK: initialise literals

void initialiser(Solver *s, int temp_k, int numOfVertices){
    for(int i = 0; i < numOfVertices; i++) {
        std::vector<Var> variables;
        std::vector<Lit> literals;
        for(int j = 0; j < temp_k; j++){
            variables.push_back(s->newVar());
            literals.push_back(mkLit(variables.back()));
        }
        minisatGraphLiterals.push_back(literals);
    }
}

// MARK: add clauses

void addClauses(Solver *s, int coverSize, int num_of_vertices, vector <int> * graphMatrix){
    assert(coverSize <= num_of_vertices);

    for (int index1 = 0; index1 < coverSize; index1++){
        vec<Lit> givenliterals;
        for (int index2 = 0; index2 < num_of_vertices; index2++){
            givenliterals.push(minisatGraphLiterals.at(index2).at(index1));
        }
        s->addClause(givenliterals);
    }

    for (int index1 = 0; index1 < num_of_vertices; index1++){
        for (int index2 = 0; index2 < coverSize; index2++){
            for (int index3 = index2 + 1; index3 < coverSize; index3++){
                Lit litral1 = minisatGraphLiterals.at(index1).at(index2);
                Lit litral2 = minisatGraphLiterals.at(index1).at(index3);
                s->addClause(~litral1, ~litral2);
            }
        }
    }

    for (int index1 = 0; index1 < coverSize; index1++){
        for (int index2 = 0; index2 < num_of_vertices; index2++){
            for (int index3 = index2 + 1; index3 < num_of_vertices; index3++) {
                Lit litral1 = minisatGraphLiterals.at(index2).at(index1);
                Lit litral2 = minisatGraphLiterals.at(index3).at(index1);
                s->addClause(~litral1, ~litral2);
            }
        }
    }

    for(int i = 0 ; i < num_of_vertices;i++) {
        for(long unsigned int j = 0;j<graphMatrix[i].size();j++ ) {
            int index1 = i;
            int index2 = graphMatrix[i].at(j);
            vec<Lit> literals;
            for (int k = 0; k < coverSize; k++){
                literals.push(minisatGraphLiterals.at(index1).at(k));
                literals.push(minisatGraphLiterals.at(index2).at(k));
            }
            s->addClause(literals);
        }
    }
}

// MARK: minisat solver

void minisatSolver(int numOfVertices, vector <int> * graphMatrix) {
    coverVertices.clear();
    bool isSolved = false;
    for (int i = 1; i <= numOfVertices; i++){
        Solver solver;
        resetLiterals();
        initialiser(&solver, i, numOfVertices);
        addClauses(&solver, i, numOfVertices, graphMatrix);
        isSolved = solver.solve();
        if (isSolved == true){
            lbool isModeled = l_False;
            for (long unsigned int index1 = 0; index1 < minisatGraphLiterals.size(); index1++){
                for (long unsigned int index2 = 0; index2 < minisatGraphLiterals.at(index1).size(); index2++){
                    isModeled = solver.modelValue(minisatGraphLiterals.at(index1).at(index2));
                    if (isModeled == l_True){
                        coverVertices.push_back(index1);
                    }
                }
            }
            for(long unsigned int i = 0; i < coverVertices.size(); i++) {
                std::cout << coverVertices.at(i) << " ";
            }
            std::cout<<std::endl;
            break;
        }
    }
}

// MARK: stoi extension

int stringToInt(string str) {
    try
    {
        int i = std::stoi(str);
        return i;
    }
    catch (std::invalid_argument const &e)
    {
        std::cout << "Error: invalid input" << '\n';
        return -1;
    }
    catch (std::out_of_range const &e)
    {
        std::cout << "Error: invalid input" << '\n';
        return -1;
    }
}
// MARK: empty graph check

bool EmptyGraphCheck(int numOfVertices, vector<int> * graphMatrix) {

    bool emptyGraphFlag = true;
    for (int i = 0 ; i<numOfVertices; i++) {
        if(!graphMatrix[i].empty()) {
            emptyGraphFlag = false;
            break;
        }
    }
    return emptyGraphFlag;
}

// MARK: main function

int main(int argc, char** argv) {

// MARK: variable declarations

    int numOfVertices = 0;
    char operation;
    vector<int> * graphMatrix = nullptr;
// MARK: input processing

    while (true) {

        std::string line;
        std::getline(std::cin, line);
        std::istringstream input(line);
        std::vector<unsigned> nums;

        if (std::cin.eof()) {
            if (graphMatrix != nullptr) {
                delete[] graphMatrix;
            }
            break;
        }

        while (!input.eof()) {
            string command;
            input >> command;
            operation = command[0];
// MARK: command switch case

            switch (operation) {
                case 'V':
                {
                    unsigned num;
                    input >> num;
                    if (input.fail()) {
                        ShowError("Error: invalid command format");
                        break;
                    }
                    if (num <= 0) {
                        ShowError("Error: invalid vertices length");
                        break;
                    }
                    else {
                        numOfVertices = num;
                        delete[] graphMatrix;
                        graphMatrix = new vector<int>[numOfVertices];
                    }
                    break;
                }
                case 'E':
                {
                    if (numOfVertices == 0) {
                        ShowError("Error: number of vertices not specified");
                        break;
                    }
                    string operation = "";
                    input >> operation;
                    graphMatrix = ParseEdges(graphMatrix, numOfVertices,operation);
                    if (isParsed) {    // if no error
                        minisatSolver(numOfVertices, graphMatrix);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
            // if eof bail out
//            if (input.eof())
//                break;
        }
    }
}

