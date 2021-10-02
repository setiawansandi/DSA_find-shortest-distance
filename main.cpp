#include <iostream>
#include <stdlib.h>             // abs (absolute value)
#include <vector>
#include <algorithm>            // std::next_permutation
#include <string>
#include <fstream>

using namespace std;

class coordinate {
public:
    int getX();
    void setX(int);
    int getY();
    void setY(int);
    int getN();
    void setN(int);
    coordinate();
    coordinate(int, int, int);

private:
    int x;
    int y;
    int nodes;

    /* coordinate() {  //origin
         x = 0;
         y = 0;
         nodes = 0;
     }

     coordinate(int, int, int);*/
};
coordinate::coordinate()
{
    x = 0;
    y = 0;
    nodes = 0;
}
int coordinate::getX()
{
    return x;
}
void coordinate::setX(int x1)
{
    x = x1;
}
int coordinate::getY()
{
    return y;
}
void coordinate::setY(int y2)
{
    y = y2;
}
int coordinate::getN()
{
    return nodes;
}
void coordinate::setN(int n3)
{
    nodes = n3;
}
coordinate::coordinate(int s, int w, int e)
{
    x = s;
    y = w;
    nodes = e;
}

// Permutation condition
struct compareNodes {
    bool operator()(coordinate& a, coordinate& b) const {

        return  a.getN() < b.getN();
    }
};


int calculate_distance(coordinate a, coordinate b) {

    return (abs(a.getX() - b.getX()) + abs(a.getY() - b.getY()));
}

void print_coord(string** output_table, int racksize[], vector <coordinate> graph, int j) {
    for (int i = racksize[1]; i >= 0; i--) {
        for (int j = 0; j <= racksize[0]; j++) {
            cout << " " << output_table[i][j];              // print out the content stored in the 2d array
        }
        cout << endl;
    }
    cout << "(" << graph[j].getX() << "," << graph[j].getY() << ") " << "retrieved" << endl;      // display which coordinate has been retrieved
    cout << endl;
}

void display_coordinates(vector <coordinate> graph, int racksize[]) {
    // create a 2d matrix to store the coordinates that need to be printed out
    string** output_table = new string * [racksize[1] + 1];      // row

    for (int y = racksize[1]; y >= 0; y--) {
        output_table[y] = new string[racksize[0] + 1];           // col

        for (int x = 0; x <= racksize[0]; ++x) {

            for (int k = 1; k < static_cast<int>(graph.size()); k++) {        // variable "k" to navigate the vector
                if (y == graph[0].getY() && x == graph[0].getX()) {   // origin
                    output_table[y][x] = "H";
                    break;
                }
                else {
                    if (y == graph[k].getY() && x == graph[k].getX()) { // if x and y match the coordinates stored in vector, store 'X'
                        output_table[y][x] = "X";
                        break;
                    }
                    else  output_table[y][x] = "O";
                    // else (k == graph.size() - 1)
                }
            }
        }
    }

    print_coord(output_table, racksize, graph, 0); // 0 : the first object stored by the vector always points to origin

    for (int i = 1; i < static_cast<int>(graph.size()); i++) {
        output_table[graph[i].getY()][graph[i].getX()] = to_string(graph[i].getN()); // use to_string to convert int to string (otherwise it will be converted to char symbol)
        print_coord(output_table, racksize, graph, i);
    }

    //delete matrix
    for (int y = racksize[1]; y >= 0; y--) {
        delete[] output_table[y];               // delete rows
    }
    delete[] output_table;                      // delete pointers that point to rows
    output_table = NULL;                        // remove pointer (that point to pointers) from stack
}


void findDistance() {
    // ======== File Reader ========
    vector <coordinate> position;
    string dataX, dataY, ignore, file_name;
    int racksize[2];
    int numOfpoint = 0, x, y, node;

    coordinate origin;
    position.push_back(origin);

    cout << "Enter the filename: ";
    cin >> file_name;

    ifstream myfile(file_name);
    if (myfile.is_open())
    {
        getline(myfile, ignore);                  // ignore "size: "
        // get rack size
        getline(myfile, dataX, ',');
        racksize[0] = stoi(dataX);                // stoi = string to integer
        getline(myfile, dataY, '\n');
        racksize[1] = stoi(dataY);

        getline(myfile, ignore);                  // ignore "x, y"

        while (!myfile.eof())                     // while not the end of file
        {
 
            getline(myfile, dataX, ',');
            x = stoi(dataX);
            getline(myfile, dataY, '\n');
            y = stoi(dataY);
           
            if (x < racksize[0] && y < racksize[1]) {
                ++numOfpoint;
                node = numOfpoint;
                position.push_back(coordinate(x, y, node));
            }
            else
                cout << "Coordinates not valid." << endl;
        }
        myfile.close();
    }
    else {
        cout << "Unable to open file" << endl;
        return;
    }
    // ======== //file reader =========


    // make a 2d matrix that store distance between two point for all the points
    int** matrix = new int* [numOfpoint + 1];   // row,     (numOfpoint+1) because + origin(0, 0)

    // loop to compute all the distance between every pair of points in the 'position' vector
    for (int i = 0; i < numOfpoint + 1; i++) {
        matrix[i] = new int[numOfpoint + 1];    // col

        for (int j = 0; j < numOfpoint + 1; j++) {
            matrix[i][j] = calculate_distance(position[i], position[j]);
        }

    }


    // ============= compare all distance =================
    int min_distance = INT_MAX; // initialize min_distance with maximum value
    vector <coordinate> graph;  // to store the sequence that has minimum distance

    // Loop to compute the distance for every possible permutation
    do {
        int distance = 0;
        int prev = 0; // start from 0 (origin)

        //cout << "Sequence: ";

        // compute total distance covered for every co-ordinate points
        for (int i = 1; i <= numOfpoint; i++) {
            distance = distance + matrix[prev][position[i].getN()];
            prev = position[i].getN();
            //cout << position[i].getN() << " ";
            //cout << "(" << position[i].getX() << "," << position[i].getY() << ")  ";   // display x and y coordinates linked to the respective node
        }
        //cout << "\tdistance: " << distance;

        // if distance is less than minimum value then update it
        if (distance < min_distance) {
            min_distance = distance;
            graph.assign(position.begin(), position.end());
        }

        //cout << endl;

    } while (next_permutation(position.begin() + 1, position.end(), compareNodes()));
    // (position.begin + 1) as we don't want to include origin(0, 0) in the permutation

    cout << "\nminimum distance: " << min_distance << endl << endl;

    // ============= //compare all distance =================

    display_coordinates(graph, racksize);

    // delete matrix
    for (int i = 0; i < numOfpoint + 1; i++) {
        delete[] matrix[i];               // delete rows
    }
    delete[] matrix;                      // delete pointers that point to rows
    matrix = NULL;                        // remove pointer (that point to pointers) from stack
}


int main() {
    findDistance();

    return 0;
}