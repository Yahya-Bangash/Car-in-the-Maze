// Section G

#include <iostream>
#include "header.h"

using namespace std;

int validation(int upperBound, int lowerBound, int x)
{
    while (cin.fail() || x > upperBound || x < lowerBound)
    {
        cin.clear();  // clear the error flag
        cin.ignore(); // discard invalid input
        cout << "Invalid Input, Enter again: ";
        cin >> x;
    }
    return x;
}

int main()
{
    Game g;
    int scoreRanking;
    int type; // 1 for manual, 2 for semi-Automatic, 3 for automatic
    char Exit;
    string name;
    g.createMap(0, 0);
    g.convertMaptoAdjacency();
    g.dijkstra(0, 400, 378);
    g.place_Obstacles_Powerups();
    system("cls");
    int choice;

    while (1)
    {
        cout << "*************************************************************************" << endl;
        cout << "*                                                                       *" << endl;
        cout << "*                             MENU                                      *" << endl;
        cout << "*                          1- Manual                                    *" << endl;
        cout << "*                          2- Automatic                                 *" << endl;
        cout << "*                          3- Semi-Automatic                            *" << endl;
        cout << "*                          4- Resume                                    *" << endl;
        cout << "*                          5- Scoreboard                                *" << endl;
        cout << "*                          6- Exit                                      *" << endl;
        cout << "*************************************************************************" << endl;
        cin >> choice;
        choice = validation(6, 1, choice);
        switch (choice)
        {
        case 1:
            cout << "Enter Player Name" << endl;
            cin >> name;
            g = Game();
            g.createMap(0, 0);
            g.convertMaptoAdjacency();
            g.dijkstra(0, 400, 378);
            g.place_Obstacles_Powerups();
            g.play(1, name);
            type = 1; // So it remembers what we were doing in case players wants to resume
            break;

        case 2:
            // g = Game();
            g.createMap(0, 0);
            g.convertMaptoAdjacency();
            g.dijkstra(0, 400, 378);
            g.place_Obstacles_Powerups();
            g.play(3, "Automatic");
            type = 3; // Type 3 for automatic
            break;

        case 3:
            cout << "Enter Player Name" << endl;
            cin >> name;
            g = Game();
            g.createMap(0, 0);
            g.convertMaptoAdjacency();
            g.dijkstra(0, 400, 378);
            g.place_Obstacles_Powerups();
            g.play(2, name);
            type = 2; // Type 2 for semi automatic
            break;

        case 4:
            g.play(type, name);
            break;

        case 5:
            system("cls");
            cout << "*************************************************************************" << endl;
            cout << "*                                                                       *" << endl;
            cout << "*                            SCOREBOARD                                 *" << endl;
            cout << "*                                                                       *" << endl;
            cout << "*************************************************************************" << endl;
            cout << endl;
            g.highestScores.readFromFile();
            scoreRanking = 1;
            g.highestScores.displayTopScores(g.highestScores.root, scoreRanking);
            cout << endl;
            cout << "Enter 'e' to Exit" << endl;
            Exit = _getch();
            while (Exit != 'e')
            {
                Exit = _getch();
            }
            system("cls");
            break;

        case 6:
            exit(1);
            break;
        }
    }

    // g.displayMapWithPath("displayWithPath");

    // for (int i = 0; i < 400; i++)
    // {
    //     for (int j = 0; j < 400; j++)
    //     {
    //         if (g.adjacencyMatrix[i][j] == 1)
    //             cout << "( " << i << " and " << j << " )";
    //     }
    // }

    // for (int i = 0; i < 20; i++)
    // {
    //     for (int j = 0; j < 20; j++)
    //     {
    //         for (int k = 0; k < 400; k++)
    //         {
    //             if (g.referenceMatrix[i][j] == g.shortestPath[k])
    //                 cout << "*  ";
    //             else
    //                 cout << g.referenceMatrix[i][j] << " ";
    //         }
    //     }
    //     cout << endl;
    // }
    // g.displayMapMatrix();
    // g.play();
    // g.createMap(0, 0);

    // g.displayMap();
    // g.createMap(0, 0);

    // g.displayMap();
}