// Yahya Hassan (22i-1549)
// M. Zunair    (22i-2729)
// Section G

#include <iostream>
#include <conio.h>
#include <ctime>
#include <queue>
#include <windows.h>
#include <fstream>

using namespace std;

// class NodeQ
// {
// public:
//     char data;
//     NodeQ *next;

//     NodeQ()
//     {
//         next = nullptr;
//     }

//     NodeQ(char x)
//     {
//         data = x;
//         next = nullptr;
//     }
// };

// class Queue
// {
// public:
//     NodeQ *front;

//     void push(char x)
//     {
//         NodeQ *newNode = new NodeQ(x);
//         if (!front)
//         {
//             front = newNode;
//         }
//         else
//         {
//             newNode->next = front;
//             front = newNode;
//         }
//     }

//     char front()
//     {
//         return front->data;
//     }

//     void pop()
//     {
//         if (!front)
//         {
//             return;
//         }
//         if (!front->next)
//         {
//             front = nullptr;
//             return;
//         }

//         front = front->next;
//         return;
//     }

//     bool empty()
//     {
//         return (front == nullptr);
//     }
// };

class Node
{
public:
    Node *left;
    int data;
    string playerName;
    Node *right;

    Node()
    {
        left = right = nullptr;
    }

    Node(string name, int score)
    {
        playerName = name;
        data = score;
        left = right = nullptr;
    }
};

class BinarysearchTree
{
public:
    Node *root;
    int nodeCount;

    BinarysearchTree()
    {
        root = nullptr;
        nodeCount = 0;
    }

    Node *insertR(Node *temp, string name, int score)
    {
        if (!temp)
        {
            temp = new Node(name, score);
            if (!root)
            {
                root = temp;
            }
            nodeCount++;
            writeToFile(); // Call function to write to file whenever new Node is inserted
            return temp;
        }
        else if (score > temp->data)
        {
            temp->right = insertR(temp->right, name, score);
        }
        else if (score < temp->data)
        {
            temp->left = insertR(temp->left, name, score);
        }

        writeToFile(); // Call function to write to file
        // Default return
        return temp;
    }

    void writeToFile()
    {
        ofstream outFile("scoreRecord.txt");
        if (outFile.is_open())
        {
            // Write scores and names of the top 5 nodes to the file
            writeToFileRecursive(root, outFile);
            outFile.close();
        }
        else
        {
            cout << "Unable to open file for writing." << endl;
        }
    }

    void writeToFileRecursive(Node *temp, ofstream &outFile)
    {
        if (temp != nullptr)
        {
            // Write scores and names in an in-order traversal
            writeToFileRecursive(temp->left, outFile);
            outFile << temp->playerName << " " << temp->data << endl;
            writeToFileRecursive(temp->right, outFile);
        }
    }

    void readFromFile()
    {
        ifstream inFile("scoreRecord.txt");
        if (inFile.is_open())
        {
            string name;
            int score;
            // Read scores and names from the file and insert them into the tree
            while (inFile >> name >> score)
            {
                root = insertR(root, name, score);
            }
            inFile.close();
        }
        else
        {
            cout << "Unable to open file for reading." << endl;
        }
    }

    void displayTopScores(Node *temp, int &ranking)
    {
        if (temp != nullptr)
        {
            // Perform in-order traversal
            displayTopScores(temp->right, ranking);

            // Display the player name and score for each node
            cout << ranking++ << "- " << temp->playerName << " : " << temp->data << endl;

            // Continue in-order traversal
            displayTopScores(temp->left, ranking);
        }
    }
};

class Game
{
public:
    int shortestPath[400];
    int count;

    // Queue for powerups and obstacles
    queue<char> obstacles;
    queue<char> powerUps;

    // For highest Scores
    BinarysearchTree highestScores;

    // X for x-axis, Y for y-axis and score for coins used in play() function, in case someone wants to resume.
    int x;
    int y;
    int score;
    int playerName;
    // To store map design
    char mapDesign[20][20];
    // To store edges info
    int adjacencyMatrix[400][400];
    // For refernece of each node
    int referenceMatrix[20][20];

    Game()
    {
        srand(time(0));
        x = 0;
        y = 0;
        score = 1000;

        int count = 0;

        // Enqueuing 10 obstacles and powerUps
        for (int i = 0; i < 10; i++)
        {
            obstacles.push('#');
            powerUps.push('*');
        }

        // Initializing mapDesign Matrix
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                mapDesign[i][j] = '0';
            }
        }

        // Initializing  adjacencyMatrix
        for (int i = 0; i < 400; i++)
        {
            for (int j = 0; j < 400; j++)
            {
                adjacencyMatrix[i][j] = 0;
            }
        }

        // Initializing reference Matrix
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                referenceMatrix[i][j] = count;
                count++;
            }
        }

        // Initializing shortest path array
        for (int i = 0; i < 200; i++)
        {
            shortestPath[i] = 0;
        }
    }

    // CreateMap Function 1: Function to check if a given position is valid in the maze
    bool isValid(int x, int y)
    {
        return x >= 0 && x < 20 && y >= 0 && y < 20;
    }

    // CreateMap Function 2: Shuffles [0,1,2,3] directions
    void shuffleDirection(int arr[], int size)
    {

        for (int i = size - 1; i > 0; --i)
        {
            int j = rand() % (i + 1);
            // we would swap the direction on current index i with some direction on random index j. s
            swap(arr[i], arr[j]);
        }
    }

    // To create a random design map everytime
    void createMap(int currentX, int currentY)
    {
        mapDesign[currentX][currentY] = '1';

        int dirX[] = {0, 0, 1, -1};
        int dirY[] = {1, -1, 0, 0};
        int directions[] = {0, 1, 2, 3};
        shuffleDirection(directions, 4);

        for (int i = 0; i < 4; ++i)
        {
            int newX = currentX + dirX[directions[i]] * 2;
            int newY = currentY + dirY[directions[i]] * 2;

            if (isValid(newX, newY) && mapDesign[newX][newY] == '0')
            {
                // If the neighbor is valid and unvisited, remove the wall
                mapDesign[currentX + dirX[directions[i]]][currentY + dirY[directions[i]]] = '1';
                createMap(newX, newY);
            }
        }
    }

    // To check if the current key is present in shortest path found by dijsktra
    bool isInShortestPath(int key)
    {
        for (int i = 0; i < 400; i++)
        {
            // Checking for shortest path but without the last node i-e Finish Node
            if (shortestPath[i] == key)
                return true;
        }
        return false;
    }

    // returns the next vertice of the shortest path
    int nextVerticeInPath(int currentVertice)
    {
        for (int i = 0; i < 400; i++)
        {
            if (shortestPath[i] == currentVertice)
            {
                return shortestPath[i + 1];
            }
        }
        return -1;
    }

    // Display the map with options to either diaply with shortest path or not
    void displayMapWithPath(string pathChoice)
    {
        // For a line on top
        for (int i = 0; i < 21; i++)
        {
            cout << char(254) << " ";
        }
        cout << endl;

        for (int i = 0; i < 20; i++)
        {

            for (int j = 0; j < 20; j++)
            {

                // for statring box
                if (j == 0)
                {
                    cout << char(254) << " ";
                }
                // For main content
                if (mapDesign[i][j] == '1')
                {
                    if (pathChoice == "displayWithPath")
                    {
                        if (isInShortestPath(referenceMatrix[i][j]))
                        {
                            cout << ". ";
                        }
                        else
                            cout << "  ";
                    }
                    else if (pathChoice == "DonotdisplayWithPath")
                    {
                        cout << "  ";
                    }
                }
                else if (mapDesign[i][j] == 'C')
                    cout << "C ";
                else if (mapDesign[i][j] == '#')
                    cout << "# ";
                else if (mapDesign[i][j] == '*')
                    cout << "* ";
                else if (mapDesign[i][j] == 'F')
                    cout << "F ";
                else if (mapDesign[i][j] == 'S')
                    cout << "S ";
                else
                    cout << char(254) << " ";
            }
            cout << endl;
        }

        cout << endl;
    }

    // Display the mapDesign matrix
    void displayMapMatrix()
    {
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                cout << mapDesign[i][j] << " ";
            }
            cout << endl;
        }
    }

    // Converts the char mapDesign matrix into int adjacency matrix in order to Dijkstra to be applicable to it
    void convertMaptoAdjacency()
    {
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                if (mapDesign[i][j] == '1')
                {
                    if (mapDesign[i - 1][j] == '1')
                    {
                        // creating edge between current vertice and vertice in above row with same col
                        adjacencyMatrix[referenceMatrix[i][j]][referenceMatrix[i - 1][j]] = 1;
                        // cout << referenceMatrix[i][j] << " and " << referenceMatrix[i - 1][j] << " marked" << endl;
                        // cout << "Adcency value: " << adjacencyMatrix[referenceMatrix[i][j]][referenceMatrix[i - 1][j]] << endl;
                    }
                    if (mapDesign[i + 1][j] == '1')
                    {
                        // edge between current vertice and down vertice
                        adjacencyMatrix[referenceMatrix[i][j]][referenceMatrix[i + 1][j]] = 1;
                    }
                    if (mapDesign[i][j - 1] == '1')
                    {
                        // edge between current vertice and left vertice
                        adjacencyMatrix[referenceMatrix[i][j]][referenceMatrix[i][j - 1]] = 1;
                    }
                    if (mapDesign[i][j + 1] == '1')
                    {
                        // edge between current vertice and right vertice
                        adjacencyMatrix[referenceMatrix[i][j]][referenceMatrix[i][j + 1]] = 1;
                    }
                }
            }
        }
    }

    // Dijkstra Part 2
    int minDistance(int distances[], bool visited[], int numVertices)
    {
        int minDistance = INT32_MAX;
        int minDistanceIndex;

        for (int i = 0; i < numVertices; i++)
        {
            if (!visited[i] && distances[i] <= minDistance)
            {
                minDistance = distances[i];
                minDistanceIndex = i;
            }
        }

        return minDistanceIndex;
    }

    // Dijkstra Part 3
    void storeShortestPath(int parent[], int j)
    {
        if (parent[j] == -1)
        {
            shortestPath[count] = j;
            count++;
            return;
        }
        storeShortestPath(parent, parent[j]);
        shortestPath[count] = j;
        count++;
    }

    // Dijkstra for finding and storing the shortest distance
    //  Dijkstra Part 1
    void dijkstra(int source, int numVertices, int destination)
    {
        count = 0;
        int distances[numVertices];
        bool visited[numVertices];
        int parent[numVertices];

        for (int i = 0; i < numVertices; i++)
        {
            distances[i] = INT32_MAX;
            visited[i] = false;
            parent[i] = -1;
        }

        distances[source] = 0;

        for (int i = 0; i < numVertices; i++)
        {

            // cout << "Distances first Node: " << distances[0] << endl;
            // cout << "Distance 399 node: " << distances[399] << endl;

            int parentNode = minDistance(distances, visited, numVertices);
            visited[parentNode] = 1;

            // cout << "Parent Node " << parentNode << endl;

            for (int child = 0; child < numVertices; child++)
            {

                // cout << "Child : " << child << endl;
                // cout << "!visited[child]: " << !visited[child] << endl;
                // cout << "adjacencyMatrix[parentNode][child] == 1: " << adjacencyMatrix[parentNode][child] << endl;
                // cout << "distances[parentNode] != INT32_MAX: " << (distances[parentNode] != INT32_MAX) << endl;
                // cout << "distances[parentNode] + adjacencyMatrix[parentNode][child]: " << (distances[parentNode] + adjacencyMatrix[parentNode][child] < distances[child]) << endl;

                if (!visited[child] && adjacencyMatrix[parentNode][child] == 1 &&
                    distances[parentNode] + adjacencyMatrix[parentNode][child] < distances[child])
                {

                    distances[child] = distances[parentNode] + adjacencyMatrix[parentNode][child];
                    parent[child] = parentNode;
                }
            }
        }
        // cout << "Parent Array" << endl;
        // for (int i = 0; i < numVertices; i++)
        // {
        //     cout << parent[i] << " ";
        // }

        // cout << "388 path: " << endl;
        storeShortestPath(parent, destination);
    }

    void place_Obstacles_Powerups()
    {

        // 4 obstacles and 4 powerups can be placed on the shortest path
        int shortestPathObstacles = 4;
        int shortestPathPowerUp = 4;
        while (!obstacles.empty())
        {
            int randomX = rand() % 20;
            int randomY = rand() % 20;

            if (mapDesign[randomX][randomY] == '1')
            {
                if (isInShortestPath(referenceMatrix[randomX][randomY]) && shortestPathObstacles > 0)
                {
                    mapDesign[randomX][randomY] = obstacles.front();
                    shortestPathObstacles--;
                    obstacles.pop();
                }
                else if (!isInShortestPath(referenceMatrix[randomX][randomY]))
                {
                    mapDesign[randomX][randomY] = obstacles.front();
                    obstacles.pop();
                }
            }
        }

        while (!powerUps.empty())
        {
            int randomX = rand() % 20;
            int randomY = rand() % 20;

            if (mapDesign[randomX][randomY] == '1')
            {
                if (isInShortestPath(referenceMatrix[randomX][randomY]) && shortestPathPowerUp)
                {
                    mapDesign[randomX][randomY] = powerUps.front();
                    shortestPathPowerUp--;
                    powerUps.pop();
                }
                else if (!isInShortestPath(referenceMatrix[randomX][randomY]))
                {
                    mapDesign[randomX][randomY] = powerUps.front();
                    powerUps.pop();
                }
            }
        }
    }

    void play(int type, string name)
    {
        // int x = 0;
        // int y = 0;
        // int score = 1000;
        char input;
        int dummyMap[20][20];
        int nextVertice;

        // Setting start and finsih
        mapDesign[0][0] = 'S';
        mapDesign[18][18] = 'F';

        // Copying mapDesign to keep track of what was what in the map
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                dummyMap[i][j] = mapDesign[i][j];
            }
        }

        // Main loop for playing
        while (input != 'e' && score > 0 && mapDesign[x][y] != 'F')
        {
            // Clearing everthing else
            system("cls");

            // Instructions
            cout << "-> CONTROLS: 'w'(up)  's'(down)  'a'(left)  'd'(right)" << endl;
            cout << "-> Abbreviations: C-Car S-Start F-Finish" << endl;
            cout << "-> Power ups (+40): *" << endl;
            cout << "-> Obstacles (-50): #" << endl;
            cout << "-> Walls: " << char(254) << endl;
            cout << "-> Path: Empty Spaces" << endl;
            cout << "-> Enter E to exit" << endl;

            cout << endl
                 << endl;
            cout << "*************************" << endl;
            cout << "      SCORE: " << score << endl;
            cout << "*************************" << endl;
            cout << endl;

            // Displaying Grid
            mapDesign[x][y] = 'C';
            // if its type 1 i-e manual donot display shortest path
            if (type == 1)
            {
                displayMapWithPath("DonotdisplayWithPath");
            }
            // For type 2 and 3 I-e semi-automatic and automatic
            else if (type == 2 || type == 3)
            {
                // applying dijsktra to update shortest path from the current path
                dijkstra(referenceMatrix[x][y], 400, 378);
                displayMapWithPath("displayWithPath");
                Sleep(1000);
            }

            // Taking Input and doing processing
            if (type == 1 || type == 2)
            {
                // for manual and semi automatic we take input
                input = _getch();
            }
            else if (type == 3)
            {
                // for automatic input is automatically selected
                nextVertice = nextVerticeInPath(referenceMatrix[x][y]);
                if (referenceMatrix[x - 1][y] == nextVertice)
                    input = 'w';
                else if (referenceMatrix[x + 1][y] == nextVertice)
                    input = 's';
                else if (referenceMatrix[x][y + 1] == nextVertice)
                    input = 'd';
                else if (referenceMatrix[x][y - 1] == nextVertice)
                    input = 'a';
            }

            if (input == 'w')
            {
                if (mapDesign[x - 1][y] == '1' || mapDesign[x - 1][y] == '#' || mapDesign[x - 1][y] == '*' || mapDesign[x - 1][y] == 'F')
                {
                    mapDesign[x][y] = dummyMap[x][y];
                    x = x - 1;

                    // Score Reduction For every step
                    if (mapDesign[x][y] == '1')
                        score = score - 5;

                    // Score Reduction for obstacles
                    if (mapDesign[x][y] == '#')
                    {
                        score = score - 50;
                    }
                    // Score addition for power ups
                    if (mapDesign[x][y] == '*' && (score + 40 <= 1000))
                    {
                        score = score + 40;
                    }
                }
            }
            if (input == 's')
            {
                if (mapDesign[x + 1][y] == '1' || mapDesign[x + 1][y] == '#' || mapDesign[x + 1][y] == '*' || mapDesign[x + 1][y] == 'F')
                {
                    mapDesign[x][y] = dummyMap[x][y];
                    x = x + 1;

                    // Score Reduction For every step
                    if (mapDesign[x][y] == '1')
                        score = score - 5;

                    // Score Reduction for obstacles
                    if (mapDesign[x][y] == '#')
                    {
                        score = score - 50;
                    }
                    // Score addition for power ups
                    if (mapDesign[x][y] == '*' && (score + 40 <= 1000))
                    {
                        score = score + 40;
                    }
                }
            }
            if (input == 'd')
            {
                if (mapDesign[x][y + 1] == '1' || mapDesign[x][y + 1] == '#' || mapDesign[x][y + 1] == '*' || mapDesign[x][y + 1] == 'F')
                {
                    mapDesign[x][y] = dummyMap[x][y];
                    y = y + 1;

                    // Score Reduction For every step
                    if (mapDesign[x][y] == '1')
                        score = score - 5;

                    // Score Reduction for obstacles
                    if (mapDesign[x][y] == '#')
                    {
                        score = score - 50;
                    }
                    // Score addition for power ups
                    if (mapDesign[x][y] == '*' && (score + 40 <= 1000))
                    {
                        score = score + 40;
                    }
                }
            }
            if (input == 'a')
            {
                if (mapDesign[x][y - 1] == '1' || mapDesign[x][y - 1] == '#' || mapDesign[x][y - 1] == '*' || mapDesign[x][y - 1] == 'F')
                {
                    mapDesign[x][y] = dummyMap[x][y];
                    y = y - 1;

                    // Score Reduction For every step
                    if (mapDesign[x][y] == '1')
                        score = score - 5;

                    // Score Reduction for obstacles
                    if (mapDesign[x][y] == '#')
                    {
                        score = score - 50;
                    }
                    // Score addition for power ups
                    if (mapDesign[x][y] == '*' && (score + 40 <= 1000))
                    {
                        score = score + 40;
                    }
                }
            }
        }

        system("cls");
        mapDesign[x][y] = dummyMap[x][y];
        if (score <= 0 && mapDesign[x][y] != 'F' && input != 'e')
        {
            cout << "********************************************************************************" << endl;
            cout << "*                                                                              *" << endl;
            cout << "*                  Oh no! You ran out of Coins. Try Again!!                    *" << endl;
            cout << "*                                                                              *" << endl;
            cout << "********************************************************************************" << endl;
            Sleep(4000);
            system("cls");
        }
        else if (score >= 0 && mapDesign[x][y] == 'F' && input != 'e')
        {
            cout << "********************************************************************************" << endl;
            cout << "*                                                                              *" << endl;
            cout << "*                 Congratulations!! You Have scored " << score << "                        *" << endl;
            cout << "*                                                                              *" << endl;
            cout << "********************************************************************************" << endl;

            // To get previous score first then insert
            highestScores.readFromFile();
            highestScores.insertR(highestScores.root, name, score);
            Sleep(4000);
            system("cls");
        }
    }
};