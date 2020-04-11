//
//  Created by Madhurdeep Singh on 29/01/20.
//  Copyright © 2020 mdsStudio. All rights reserved.
//
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
using namespace std;


// MARK: function declarations

int stringToInt(string str);
void ShowError(string messege);
vector<int> * ParseEdges(vector<int> * graphMatrix, int numberOfVertices, string operation);
void PrintPath(vector<int> path , int sourceVertex);
bool EmptyGraphCheck(int numOfVertices, vector<int> * graphMatrix);
void createGraph(vector <int> * graphMatrix, int numOfVertices, int sourceVertex,int destinationVertex);

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
                graphMatrix =new vector<int>[numberOfVertices];
                break;
            } else if (edgeVertex2 >= numberOfVertices) {
                ShowError("Error: Invalid edge with vertex > " + to_string(edgeVertex2));
                delete[] graphMatrix;
                graphMatrix =new vector<int>[numberOfVertices];
                break;
            } else {
                graphMatrix[edgeVertex2].push_back(edgeVertex1);
                graphMatrix[edgeVertex1].push_back(edgeVertex2);
            }
            //            edgeVertex2 = -1;
            //            edgeVertex1 = -1;
            numberString = "";
            sepratorFlag = false;
            arrowFlag = false;
        }
        if(operation[i] == '}') {
            parathasisFlag = false;
        }
    }
    return graphMatrix;
}

void PrintPath(vector<int> path , int sourceVertex) {
    cout<<sourceVertex;
    for (long index = path.size() - 1; index >=0; index--) {
        cout<<"-"<<path[index];
    }
    cout<<endl;
}

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
        PrintPath(routeArray,sourceVertex);
    } else {
        ShowError("Error: The vertices are not reachable");
//        return -1;
        exit(0);
    }
}

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

int AddVertex(int input) {
    return 0;
}

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
    int sourceVertex = -1;
    int destinationVertex = -1;
    char operation;
    vector<int> * graphMatrix = nullptr;
    
    // MARK: input processing
    
    while (!std::cin.eof()) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream input(line);
        std::vector<unsigned> nums;
        
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
                        return -1;
                        break;
                    }
                    if (num <= 0) {
                        ShowError("Error: invalid vertices length");
                        return -1;
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
                        return -1;
                        break;
                    }
                    string operation = "";
                    input >> operation;
                    
                    if(operation == "Invalid" || NULL) {
                        
                    } else {
//                        if(command.size() > 0){
                            cout<<"V "<<numOfVertices<<endl;
                            cout<<command[0]<<" "<<operation<<endl;
//                        }
                    }
                    graphMatrix = ParseEdges(graphMatrix, numOfVertices,operation);
                    
                    //                    if (!graphMatrix[0].empty()) {
                    //                        cout<<"V "<<numOfVertices<<endl;
                    //                        cout<<command[0]<<" "<<operation<<endl;
                    //                    }
                    break;
                }
                case 's':
                {
                    if (EmptyGraphCheck(numOfVertices,graphMatrix)) {
                        ShowError("Error: Graph does not exist");
                        return -1;
                        break;
                    }
                    int vertex1;
                    input >> vertex1;
                    if (!input.fail())
                        sourceVertex = vertex1;
                    if (input.fail()) {
                        ShowError("Error: invalid command format");
                        return -1;
                    }
                    int vertex2;
                    input >> vertex2;
                    if (!input.fail())
                        destinationVertex = vertex2;
                    if (input.fail()) {
                        ShowError("Error: invalid command format");
                        return -1;
                    }
                    createGraph(graphMatrix, numOfVertices, sourceVertex, destinationVertex);
                    break;
                }
                default:
                {
                    break;
                }
            }
            // if eof bail out
            if (input.eof())
                break;
        }
    }
    return 0;
}

