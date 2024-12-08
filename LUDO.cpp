/*
LUDO using multithreading
*/

#include <iostream>
#include <cstring>
#include <string>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>

sem_t sema;  //for synchronization

using namespace std;

class tokens //class for gotis
//each player will get 4 tokens
{
    bool outside;
    int position;
    string color;
    int number;
    bool hasKilled;
    int startingPos, endingPos;    //for board
    int back; 
    bool isHome;     //checks if the player is in its home colmomns (winning colomn)
    bool Won;

public:
       //both default and paramterized consutructors are used
    tokens()
    {
        number = -1;
        hasKilled = 0;
        outside = 0;
        position = -1;
        startingPos = endingPos = 0;
        back = 0;
        isHome = false;
        Won = false;
    }
    tokens(string c, int n, int s, int b)
    {
        outside = 0;
        position = -1;
        color = c;
        number = n;
        hasKilled = 0;
        startingPos = s;
        back = b;
        isHome = false;
        Won = false;
    }
     //below are the setters and getters
    //and functions
    void setWon()
    {
        Won = true;
        isHome = false;
    }
    bool checkWon()
    {
        return Won;
    }
    bool checkHome()
    {
        return isHome;
    }
    void init(string c, int n, int s, int b, int e)
    {
        outside = 0;
        position = -1;
        color = c;
        number = n;
        hasKilled = 0;
        startingPos = s;
        back = b;
        endingPos = e;
    }
    void set(string* arr, int location)
    {
        position = location;
        if (position == startingPos || position == back)
            arr[location] = to_string(number) + "\033[0m";
        else
            arr[location] = color + to_string(number) + "\033[0m";
    }
    void moveToHomeLane(string** homeLane, int turn, int pos)
    {
        homeLane[turn][pos] = to_string(number);
        isHome = true;
        position = -10;
    }
    bool isAlive()
    {
        if (position != -1 && !isHome)
            return 1;
        return 0;
    }

    int getLocation()
    {
        return position;
    }
    void setKilled()
    {
        hasKilled = 1;
    }
    bool checkKilled()
    {
        return hasKilled;
    }
    void kill()
    {
        outside = 0;
        position = -1;
    }
    int getStartingPos()
    {
        return startingPos;
    }

    int getEndingPos()
    {
        return endingPos;
    }
};

class grid
{
    int sitting;
    int credintials[4][2];

public:
    grid()
    {
        sitting = 0;
    }
    void set(int p, int t)
    {
        sitting++;
        credintials[sitting - 1][0] = p;
        credintials[sitting - 1][1] = t;
    }
    void remove()
    {
        sitting--;
    }
    int* getCred()
    {
        int* arr = new int[2];
        arr[0] = credintials[0][0];
        arr[1] = credintials[0][1];
        return arr;
    }
    bool isSitting()
    {
        return sitting;
    }
};

struct parameters
{
    string* track;
    grid* locate;
    tokens** players;
    string** homeLane;
    int **counter;
    int turn, six, sixCount, killed, dice;
    void init(string* t, grid* l, tokens** p, string** h)
    {
        track = t;
        locate = l;
        players = p;
        homeLane = h;
    }
};
struct boardParameters
{
    string* track;
    string** homeLane;
    void init(string* t, string** h)
    {
        track = t;
        homeLane = h;
    }
};

void* displayBoard(void* args)          //// Function to display the current state of the board.
{ 
      // The function uses for colored output.
      // It handles the main game track and the home lanes for each player.
    boardParameters* b1 = (boardParameters*) args;
    string* track = b1->track;
    string** homeLane = b1->homeLane;
    
    const string RESET_COLOR = "\033[0m";
    const string RED_COLOR = "\033[31m";
    const string GREEN_COLOR = "\033[32m";
    const string YELLOW_COLOR = "\033[33m";
    const string BLUE_COLOR = "\033[34m";
    const string RED_BACKGROUND = "\033[41m";
    const string GREEN_BACKGROUND = "\033[42m";
    const string YELLOW_BACKGROUND = "\033[43m";
    const string BLUE_BACKGROUND = "\033[44m";

    cout << " _______________________________" << endl;
    cout << "|" << BLUE_BACKGROUND << "            " << RESET_COLOR << "|" << track[10] << "|" << track[11] << "|" << track[12] << "|" << YELLOW_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << BLUE_BACKGROUND << "            " << RESET_COLOR << "|" << track[9] << YELLOW_BACKGROUND << "|" << homeLane[1][0] << "|" << track[13] << YELLOW_BACKGROUND << "|" << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << BLUE_BACKGROUND << "            " << RESET_COLOR << YELLOW_BACKGROUND << "|" << track[8] << YELLOW_BACKGROUND << "|" << homeLane[1][1] << "|" << RESET_COLOR << track[14] << "|" << YELLOW_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << BLUE_BACKGROUND << "            " << RESET_COLOR << "|" << track[7] << YELLOW_BACKGROUND << "|" << homeLane[1][2] << "|" << RESET_COLOR << track[15] << "|" << YELLOW_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << BLUE_BACKGROUND << "            " << RESET_COLOR << "|" << track[6] << YELLOW_BACKGROUND << "|" << homeLane[1][3] << "|" << RESET_COLOR << track[16] << "|" << YELLOW_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << BLUE_BACKGROUND << "            " << RESET_COLOR << "|" << track[5] << YELLOW_BACKGROUND << "|" << homeLane[1][4] << "|" << RESET_COLOR << track[17] << "|" << YELLOW_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << track[51] << BLUE_BACKGROUND << "|" << track[0] << BLUE_BACKGROUND << "|" << RESET_COLOR << track[1] << "|" << track[2] << "|" << track[3] << "|" << track[4] << "|       |" << track[18] << "|" << track[19] << "|" << track[20] << GREEN_BACKGROUND << "|" << track[21] << GREEN_BACKGROUND << "|" << RESET_COLOR << track[22] << "|" << track[23] << "|" << endl;
    cout << "|" << track[50] << BLUE_BACKGROUND << "|" << homeLane[0][0] << "|" << homeLane[0][1] << "|" << homeLane[0][2] << "|" << homeLane[0][3] << "|" << homeLane[0][4] << "|" << RESET_COLOR << "       " << GREEN_BACKGROUND << "|" << homeLane[2][4] << "|" << homeLane[2][3] << "|" << homeLane[2][2] << "|" << homeLane[2][1] << "|" << homeLane[2][0] << "|" << RESET_COLOR << track[24] << "|" << endl;
    cout << "|" << track[49] << "|" << track[48] << BLUE_BACKGROUND << "|" << track[47] << BLUE_BACKGROUND << "|" << RESET_COLOR << track[46] << "|" << track[45] << "|" << track[44] << "|       |" << track[30] << "|" << track[29] << "|" << track[28] << "|" << track[27] << GREEN_BACKGROUND << "|" << track[26] << GREEN_BACKGROUND << "|" << RESET_COLOR << track[25] << "|" << endl;
    cout << "|" << RED_BACKGROUND << "            " << RESET_COLOR << "|" << track[43] << RED_BACKGROUND << "|" << homeLane[3][4] << "|" << RESET_COLOR << track[31] << "|" << GREEN_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << RED_BACKGROUND << "            " << RESET_COLOR << "|" << track[42] << RED_BACKGROUND << "|" << homeLane[3][3] << "|" << RESET_COLOR << track[32] << "|" << GREEN_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << RED_BACKGROUND << "            " << RESET_COLOR << "|" << track[41] << RED_BACKGROUND << "|" << homeLane[3][2] << "|" << RESET_COLOR << track[33] << "|" << GREEN_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << RED_BACKGROUND << "            " << RESET_COLOR << "|" << track[40] << RED_BACKGROUND << "|" << homeLane[3][1] << "|" << track[34] << RED_BACKGROUND << "|" << GREEN_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << RED_BACKGROUND << "            " << "|" << track[39] << RED_BACKGROUND << "|" << homeLane[3][0] << "|" << RESET_COLOR << track[35] << "|" << GREEN_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << "|" << RED_BACKGROUND << "            " << RESET_COLOR << "|" << track[38] << "|" << track[37] << "|" << track[36] << "|" << GREEN_BACKGROUND << "            " << RESET_COLOR << "|" << endl;
    cout << " -------------------------------" << endl;
    
    pthread_exit(NULL);
}

void* refresh(void* args)
{ 
      // Function to refresh the board state by updating the positions of tokens.
      // It iterates through all tokens and updates their positions on the board.
    parameters* p1 = (parameters*) args;
    string* track = p1->track;
    tokens** players = p1->players;
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if(players[i][j].getLocation() != -1 && players[i][j].getLocation() != -10)
                players[i][j].set(track, players[i][j].getLocation());
        }
    }
    pthread_exit(NULL);
}


void* rollDice(void* args)
{
      // Function to simulate rolling a dice.
    int* roll = new int;
    *roll = rand() % 6 + 1;    // It generates a random number between 1 and 6.
    pthread_exit((void*)roll); 
}

void* playMoves(void* args)       //all the logic of movement of token is implemented here.
{
      //  checks for various conditions like token availability, token kills, and moving to home lanes. 
    //as well the wining condition
	parameters* p1 = (parameters*) args;
    string* track = p1->track;
    grid* locate = p1->locate;
	tokens** players = p1->players;
   	string** homeLane = p1->homeLane;
   	int turn = p1->turn;
   	int six = p1->six;
   	int sixCount = p1->sixCount;
   	int killed = p1->killed;
   	int dice = p1->dice;
   	int** counter = p1->counter;

        bool available[4] = { 0 };
	int count = 0;
	int choice = 1;

	for (int i = 0; i < 4; i++)
    {
        if ((players[turn][i].isAlive() || players[turn][i].checkHome()) && !players[turn][i].checkWon())
        {
            available[i] = 1;
            count++;
        }
    }

    if (count == 0)
    {
        if (dice == 6)
        {
            for(int i=0; i<4; i++)
            {
                if(!available[i] && !players[turn][i].checkWon())
                {
                    players[turn][i].set(track, players[turn][i].getStartingPos());
                    locate[players[turn][i].getStartingPos()].set(turn, i);
                    six = turn;
                    sixCount++;
                    break;
                }
            }
        }
    }
    else
    {
        if (dice == 6)
        {
            cout << "Choose the piece to move:-" << endl;
            for (int i = 0; i < 4; i++)
            {
                cout << i + 1 << ", ";
            }
            cout << endl;
            cout << "Choice: ";
            cin >> choice;
            if (!players[turn][choice - 1].checkHome()) {

                if (players[turn][choice - 1].getLocation() == -1)
                {
                    players[turn][choice - 1].set(track, players[turn][choice - 1].getStartingPos());
                    locate[players[turn][choice - 1].getStartingPos()].set(turn, choice - 1);
                }
                else
                {
                    counter[turn][choice - 1] += dice;
                    if (counter[turn][choice - 1] > 50)
                    {
                        int temp = players[turn][choice - 1].getLocation();
                        track[temp] = " ";
                        locate[temp].remove();
                        // Move the token to its home lane
                        players[turn][choice - 1].moveToHomeLane(homeLane, turn, counter[turn][choice - 1] % 51);
                        // Reset movement count

                    }
                    else
                    {
                        int temp = players[turn][choice - 1].getLocation();
                        if (temp == -1)
                            temp = players[turn][choice - 1].getStartingPos();
                        else
                            track[temp] = " ";

                        locate[players[turn][choice - 1].getLocation()].remove();
                        temp = (temp + dice) % 52;

                        if (locate[temp].isSitting() && temp != 0 && temp != 13 && temp != 26 && temp != 39 && temp != 47 && temp != 8 && temp != 21 && temp != 34)
                        {
                            int* arr = locate[temp].getCred();
                            if (arr[0] != turn)
                            {
                                locate[temp].remove();
                                players[arr[0]][arr[1]].kill();
                                killed = 1;
                            }
                        }
                        players[turn][choice - 1].set(track, temp);
                        locate[temp].set(turn, choice - 1);
                    }
                }
                six = turn;
                sixCount++;
            }
            else
            {
                if((counter[turn][choice-1]+dice) <= 55)
                {
                    homeLane[turn][counter[turn][choice-1] % 51] = " ";
                    counter[turn][choice-1] += dice;
                    players[turn][choice-1].moveToHomeLane(homeLane, turn, counter[turn][choice-1] % 51);
                }
                else if((counter[turn][choice-1]+dice) == 56)
                {
                    players[turn][choice-1].setWon();
                    homeLane[turn][counter[turn][choice-1] % 51] = " ";
                }
            }
        }
        else
        {
            if (count == 1)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (available[i]&&!players[turn][i].checkHome())
                    {
                        counter[turn][i] += dice;
                        if (counter[turn][i] > 50)
                        {
                            int temp = players[turn][i].getLocation();
                            track[temp] = " ";
                            locate[temp].remove();
                            // Move the token to its home lane
                            players[turn][i].moveToHomeLane(homeLane, turn, counter[turn][i] % 51);
                            // Reset movement count
                        }
                        else
                        {
                            int temp = players[turn][i].getLocation();
                            if (temp == -1)
                                temp = players[turn][i].getStartingPos();
                            else
                                track[temp] = " ";

                            locate[players[turn][i].getLocation()].remove();
                            temp = (temp + dice) % 52;

                            if (locate[temp].isSitting() && temp != 0 && temp != 13 && temp != 26 && temp != 39 && temp != 47 && temp != 8 && temp != 21 && temp != 34)
                            {
                                int* arr = locate[temp].getCred();
                                if (arr[0] != turn)
                                {
                                    locate[temp].remove();
                                    players[arr[0]][arr[1]].kill();
                                    killed = 1;
                                }
                            }
                            players[turn][i].set(track, temp);
                            locate[temp].set(turn, i);
                        }
                    }
                    else if(available[i]&&players[turn][i].checkHome())
                    {
                        if(available[i])
                        {
                            // Move the token to its home lane
                            if((counter[turn][i]+dice) <= 55)
                            {
                                homeLane[turn][counter[turn][i] % 51] = " ";
                                counter[turn][i] += dice;
                                players[turn][i].moveToHomeLane(homeLane, turn, counter[turn][i] % 51);
                            }
                            else if((counter[turn][i]+dice) == 56)
                            {
                                players[turn][i].setWon();
                                homeLane[turn][counter[turn][i] % 51] = " ";
                            }
                            // Reset movement count
                            break;
                        }
                    }
                }
            }
            else
            {
                cout << "Choose the piece to move:-" << endl;
                for (int i = 0; i < 4; i++)
                {
                    if (available[i])
                        cout << i + 1 << ", ";
                }
                cout << endl;
                cout << "Choice: ";
                cin >> choice;

                if (available[choice - 1]&&!players[turn][choice - 1].checkHome())
                {
                    counter[turn][choice - 1] += dice;
                    if (counter[turn][choice - 1] > 50)
                    {

                        int temp = players[turn][choice - 1].getLocation();
                        track[temp] = " ";
                        locate[temp].remove();
                        players[turn][choice - 1].moveToHomeLane(homeLane, turn, counter[turn][choice - 1] % 51);
                    }
                    else
                    {
                        int temp = players[turn][choice - 1].getLocation();
                        if (temp == -1)
                            temp = players[turn][choice - 1].getStartingPos();
                        else
                            track[temp] = " ";

                        locate[players[turn][choice - 1].getLocation()].remove();
                        temp = (temp + dice) % 52;

                        if (locate[temp].isSitting() && temp != 0 && temp != 13 && temp != 26 && temp != 39 && temp != 47 && temp != 8 && temp != 21 && temp != 34)
                        {
                            int* arr = locate[temp].getCred();
                            locate[temp].remove();
                            players[arr[0]][arr[1]].kill();
                            killed = 1;
                        }
                        players[turn][choice - 1].set(track, temp);
                        locate[temp].set(turn, choice - 1);
                    }
                }
            }
            six = sixCount = 0;
        }
    }
    p1->turn = turn;
    p1->six = six;
    p1->sixCount = sixCount;
    p1->killed = killed;
    
    pthread_exit(NULL);
}

void* checkWin(void* args)
{
    parameters* p1 = (parameters*) args;
    tokens** players = p1->players;

    bool allDone = true;

    for(int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(!players[i][j].checkWon())
                allDone = false;
            if(allDone)
            {
                cout<<"Player # "<<i<<" won!!"<<endl;
                break;
            }
        }
        if(allDone)
        {
            break;
        }
    }
    pthread_exit((void*) allDone);
}

void* play(void* args)
{
       // Main game loop function.
    // It manages the game flow, including rolling the dice, moving tokens, and refreshing the board display.
    int** counter = new int*[4];
    for(int i=0; i<4; i++)
    {
    	counter[i] = new int[4];
    	for(int j=0; j<4; j++)
    	    counter[i][j] = 0;
    }
    

    parameters* p1 = (parameters*) args;
    string* track = p1->track;
    grid* locate = p1->locate;
    tokens** players = p1->players;
    string** homeLane = p1->homeLane;
    
    parameters p2;
    p2.init(track, locate, players, homeLane);
    
    boardParameters b1;
    b1.init(track, homeLane);
    
    pthread_t diceRoll, display, ref, move, winning;
    
    pthread_create(&display, NULL, displayBoard, &b1);
    pthread_join(display, NULL);
    bool killed = 0;
    int turn = -1;
    int dice = 0;
    int six = 0, sixCount = 0;
    while (1)
    {
    	sem_wait(&sema);
        if(!six || sixCount == 3)
            turn = rand() % 4;
        else
        {
            turn = six;
        }
        cout << "Player " << turn + 1 << "'s turn" << endl;
        cout << "Rolling the dice!" << endl;
        pthread_create(&diceRoll, NULL, rollDice, NULL);
        int* temp;
        pthread_join(diceRoll, (void**)&temp);
        dice = *temp;
        cout << "Dice = " << dice << endl;
        sleep(1);
        
        p2.turn = turn;
        p2.six = six;
        p2.sixCount = sixCount;
        p2.killed = killed;
        p2.dice = dice;
        p2.counter = counter;
        
        pthread_create(&move, NULL, playMoves, &p2);
        pthread_join(move, NULL);
        
        turn = p2.turn;
        six = p2.six;
        sixCount = p2.sixCount;
        killed = p2.killed;
        dice = p2.dice;
        counter = p2.counter;

        
        pthread_create(&ref, NULL, refresh, &p2);
        pthread_join(ref, NULL);
        //refresh(track, players);
        if(!killed)
        system("clear");
        pthread_create(&display, NULL, displayBoard, &b1);
        pthread_join(display, NULL);
        //displayBoard(track, homeLane);
        //displayBoard(locate);
        
        if (killed) {
            cout << "KILL DETETECTED!!" << endl;
            sem_post(&sema);
            //break;
            killed = 0;
        }
        pthread_create(&winning, NULL, checkWin, &p2);
        bool* cond;
        pthread_join(winning, (void**) cond);
        bool check = cond;
        if(cond)
            break;
        sem_post(&sema);
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t dice;
    
    sem_init(&sema, 0, 1);
    
    srand(time(0));
 //colors which we will use in creation of tokens and board
    const string RESET_COLOR = "\033[0m";
    const string RED_COLOR = "\033[31m";
    const string GREEN_COLOR = "\033[32m";
    const string YELLOW_COLOR = "\033[33m";
    const string BLUE_COLOR = "\033[34m";
    const string RED_BACKGROUND = "\033[41m";
    const string GREEN_BACKGROUND = "\033[42m";
    const string YELLOW_BACKGROUND = "\033[43m";
    const string BLUE_BACKGROUND = "\033[44m";


    string *track = new string[52];
    string **homeLane = new string*[4];
    for (int i = 0; i < 4; i++)
    {
        homeLane[i] = new string[5];
        for (int j = 0; j < 5; j++)
            homeLane[i][j] = " ";
    }


    grid *locate = new grid[52];

    for (int i = 0; i < 52; i++)
    {
        track[i] = ' ';
    }

    tokens **players = new tokens*[4];
    for (int i = 0; i < 4; i++)
    {
        players[i] = new tokens[4];
    }
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if (i == 0)
                players[i][j].init(BLUE_COLOR, j + 1, 0, 47, 50);
            else if (i == 1)
                players[i][j].init(YELLOW_COLOR, j + 1, 13, 8, 11);
            else if (i == 2)
                players[i][j].init(GREEN_COLOR, j + 1, 26, 21, 24);
            else
                players[i][j].init(RED_COLOR, j + 1, 39, 34, 37);
    }
        
    parameters p1;
    p1.init(track, locate, players, homeLane);

    pthread_create(&dice, NULL, play, &p1);
    pthread_join(dice, NULL);

    /*players[0][3].set(track, 0);
    displayBoard(track);*/

    return 0;
}
