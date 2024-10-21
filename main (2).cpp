#include <iostream>
#include <vector>
#include "stack.h"  
#include "queue.h"  
#include "linkedlist.h"  
#include <fstream> 
#include <cstdlib> 
#include <ctime>   

using namespace std;

const int BOARD_SIZE = 8;
const char JAIL = 'J';
const char RESOURCE = 'R';
const char SAFE_ZONE = 'S';
const char HAZARD = 'H';
const char VISITED = 'X'; // Mark squares that have been stepped on
bool gameEnd = false;
bool save = false;


// Player class to handle movement and scoring
class Player {
public:
    string name;
    int x, y; //location
    int health; // Player's health,
    int jail;
    Stack<pair<pair<int, int>, pair<char,int>>> moveStack; 


    Player(string playerName, int startX, int startY)
        : name(playerName), x(startX), y(startY), health(10), jail(0) {
            healthHistory.addNode(health);
    }


    void move(vector<vector<char>>& board) {
        char direction;
        bool validMove = false;

        while (!validMove) {
            if (jail > 0) 
            {
                cout<< name << " is still stuck because of weird creatures" << endl;
                jail = 0;
                moveStack.push({{x,y}, {board[x][y],health}}); 
                return;
            }
            cout << name << "(health:" <<health << "), make your move (w/a/s/d), undo (u), quit (q), or save (r): ";
            cin >> direction;

            if (direction == 'u' && !moveStack.empty()) {
                // Undo last move
                if (!moveStack.empty()) {
                    auto lastMove = moveStack.top();
                    x = lastMove.first.first;
                    y = lastMove.first.second;
                    board[x][y] = lastMove.second.first;
                    health = lastMove.second.second;
                    cout << name << " undid their last move!" << endl;
                    moveStack.pop();
                    jail = 0;
                    return;
                }
            }

            switch (direction) {
                case 'w':
                    if (x > 0 ) {
                        moveStack.push({{x,y}, {board[x][y],health}}); 
                        board[x][y] = VISITED; 
                        x--; 
                        validMove = true; 
                        updateHealth(board[x][y]);
                        }
                    else cout << "Invalid move! Can't move off the board." << endl;
                    break;
                case 's':
                    if (x < BOARD_SIZE - 1) {
                        moveStack.push({{x,y}, {board[x][y],health}}); 
                        board[x][y] = VISITED; 
                        x++; 
                        validMove = true; 
                        updateHealth(board[x][y]);
                        }
                    else cout << "Invalid move! Can't move off the board." << endl;
                    break;
                case 'a':
                    if (y > 0) {
                        moveStack.push({{x,y}, {board[x][y],health}}); 
                        board[x][y] = VISITED; 
                        y--; 
                        validMove = true; 
                        updateHealth(board[x][y]);
                        }
                    else cout << "Invalid move! Can't move off the board." << endl;
                    break;
                case 'd':
                    if (y < BOARD_SIZE - 1) {
                        moveStack.push({{x,y}, {board[x][y],health}}); 
                        board[x][y] = VISITED; 
                        y++; 
                        validMove = true; 
                        updateHealth(board[x][y]);
                        }
                    else cout << "Invalid move! Can't move off the board." << endl;
                    break;
                case 'q':
                    gameEnd = true;
                    cout << "So sad to see you go!!!" << endl;
                    return;
                case 'r':
                    save = true;
                    return;
                default:
                    cout << "Invalid input! Use w/a/s/d to move." << endl;
            }
        }
    }

    // Update player health based on the type of square
    void updateHealth(char square) {
        if (square == RESOURCE) {
             health += 5;
            cout << name << " found a Resource! health +5." << endl;
        } 
        else if (square == HAZARD) {
            health -= 5;
            cout << name << " hit a Hazard! health -5." << endl;
        }
        else if (square == VISITED) {
            health -= 1000000;
            cout << "Bye bye "<< name << "! Don't get corrupted next time!" << endl;
        }
        else if(square == JAIL)
        {
            jail += 1;
            cout << "You've been capture by weird creatures!! You cannot move for 1 round" << endl;
        }
    }


};

void saveGame(ofstream& file, vector<vector<char>>& board, Player* player1, Player* player2) {
    // Save board state
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            file << board[i][j] << " ";
        }
        file << endl;  // Newline after each row
    }

    // Save player states
    file << player1->name << " " << player1->x << " " << player1->y << " " << player1->health << " " << player1->jail << endl;
    file << player2->name << " " << player2->x << " " << player2->y << " " << player2->health << " " << player2->jail << endl;

    cout << "Game has been saved!" << endl;
}



// Display the game board
void displayBoard(const vector<vector<char>>& board, const Player& player1, const Player& player2) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (i == player1.x && j == player1.y)
                cout << '1' << " ";  // Player 1
            else if (i == player2.x && j == player2.y)
                cout << '2' << " ";  // Player 2
            else
                cout << board[i][j] << " "; // Display square type
        }
        cout << endl;
    }
}

// Function to check if the game has ended (goal condition)
bool checkGameEnd(Player& player) {
    return (player.health <= 0);
}

// Generate a random board with different types of squares
void generateBoard(vector<vector<char>>& board) {
    srand(static_cast<unsigned>(time(0))); // Seed for randomization
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            int randomValue = rand() % 4; // Generate random square types
            if (randomValue == 0) board[i][j] = RESOURCE;
            else if (randomValue == 1) board[i][j] = SAFE_ZONE;
            else if (randomValue == 2) board[i][j] = JAIL;
            else board[i][j] = HAZARD;
        }
    }
}

// Main game functiofunction
void game(bool load) {
    string p1Name, p2Name;
    vector<vector<char>> board(BOARD_SIZE, vector<char>(BOARD_SIZE));
    int p1X = 0, p2X = 7, p1Y = 0, p2Y= 7;
    generateBoard(board);
    if(!load){
    cout << "Enter Player 1 name: ";
    cin >> p1Name;
    cout << "Enter Player 2 name: ";
    cin >> p2Name;
    }
    Player player1(p1Name, p1X, p1Y);
    Player player2(p2Name, p2X, p2Y);

    if(load){
        ifstream inFile("game_state.txt");
        if (!inFile) {
            cout << "No saved game found!" << endl;
            return;
        }

        
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                inFile >> board[i][j];
            }
        }

        // Load player states
        int p1Health, p1Jail, p2X, p2Y, p2Health, p2Jail;
        inFile >> p1Name >> p1X >> p1Y >> p1Health >> p1Jail;
        inFile >> p2Name >> p2X >> p2Y >> p2Health >> p2Jail;

        player1.name = p1Name;
        player2.name = p2Name;
        player1.health = p1Health;
        player1.jail = p1Jail;
        player2.health = p2Health;
        player2.jail = p2Jail;
        player1.x= p1X;
        player1.y = p1Y;
        player2.x = p2X;
        player2.y = p2Y;

        cout << "Game loaded successfully!" << endl;        

        inFile.close();
    }



    Queue<Player*> turnQueue;  // Queue to manage player turns
    turnQueue.enqueue(&player1);
    turnQueue.enqueue(&player2);

    displayBoard(board, player1, player2);

    while (!gameEnd) {
        // Get the next player in the queue
        Player* currentPlayer = turnQueue.getFront();
        turnQueue.dequeue();  // Remove the player from the front of the queue

        currentPlayer->move(board);

        displayBoard(board, player1, player2);
        if(save)
        {
            ofstream outFile("game_state.txt");
            saveGame(outFile, board, &player1, &player2);
            save = false;
        }
        if (checkGameEnd(*currentPlayer)) {
            cout << currentPlayer->name << " has died!" << endl;
            gameEnd = true;
            break;
        }
        

        // Add the player back to the queue for the next turn
        turnQueue.enqueue(currentPlayer);

    }

    // Save game state
}


void tutorial() {
    cout << "Welcome to the Dead World Stranger!!!\n";
    cout << "The Goddess of Death have blessed you by giving you a chance to leave this dead world\n";
    cout << "In this world, you will move around a 8x8 land using the keys 'w', 'a', 's', and 'd' to go up, left, down, and right respectively.\n";
    cout << "Your goal is to the last one to survive by managing your health, avoiding hazards, and collecting resources.\n";
    cout << "If you are the last one surviving, you will be taken to a new world, where peace prevails\n";



    cout << "\nHere is the layout of the land: \n";
    cout << "R - Resource: You can plunder the life energy and increase your health by 5.\n";
    cout << "H - Hazard: The land here is particlularly toxic and decreases your health by 5.\n";
    cout << "J - Jail: A monster is waiting here, and will bind you for 1 turn.\n";
    cout << "X - Visited: You've already stepped on this square.\n";
    cout << "REMEMBER DONT STEP ON VISITED LAND. \n";
    cout << "Since you are not of this world, once you leave a plot of land, it turns corrputed, killing whatever steps into it.\n";
    cout << "You can undo your last move using 'u'.\n";
    cout << "To quit the game during your turn, press 'q'.\n";
    cout << "Good luck and have fun!\n";
}

int main() {
    char choice;
    bool stay = true;

    while (stay) {
        cout << "1. Start New Game\n2. Load Game\n3. Tutorial\n4. Exit\n";
        cin >> choice;

        switch (choice) {
            case '1':
                gameEnd = false;
                game(false);
                break;
            case '2':
                gameEnd = false;
                game(true);
                break;
            case '3':
                tutorial();
                break;
            case '4':
                stay = false;
                break;

            default:
                cout << "Invalid choice!" << endl;
                break;
        }
    }
}
