/* prog5Wumpus.cpp

 Description: Navigate a maze to kill the Wumpus.
 Implementation of a 1972 game by Gregory Yob.

 Results of a program run are shown below:
https://codio.com/dreed/prog5-hunt-the-wumpus/filetree#

 */
#include <iostream>
#include <iomanip>   // for setw()
#include <cctype>    // for toupper()
#include <cstdlib>   // for rand() and srand()
#include <ctime>     // to seed random number generator with time(0)
using namespace std;

// global constants
#define MAX_LINE_LENGTH 81
#define NUMBER_OF_ROOMS 20

int AllConnections[20][3] = {
        {1,4,7}, {0,2,9}, {1,3,11}, {2,4,13}, {0,3,5},
        {4,6,14}, {5,7,16}, {0,6,8}, {7,9,17}, {1,8,10},
        {9,11,18}, {2,10,12}, {11,13,19}, {3,12,14}, {5,13,15},
        {14,16,19}, {6,15,17}, {8,16,18}, {10,17,19}, {12,15,18}
};
//--------------------------------------------------------------------------------

class Room{ // Room Class - all Room Objects come from here
private:
    int roomNumber;
public:
    int connections[3] = {0}; // Contains the room numbers that a room is connected to
    Room() = default;
    Room(int number); // Constructor
    void setConnection(int connections[3]); // Sets up the connections array
    bool isConnected(int number); // Checks if a room is connected to another room
};

/// Room Constructor
/// \param number Room number of object
Room::Room(int number) { // Constructor
    this->roomNumber = number;
}

/// Sets the connections
/// \param connections array of connected room numbers
void Room::setConnection(int connections[3]) { // Sets up connections
    this->connections[0] = connections[0];
    this->connections[1] = connections[1];
    this->connections[2] = connections[2];
}

/// Finds out if a room is connected to another room
/// \param number number of room we're comparing to
/// \return whether the room is connected or not
bool Room::isConnected(int number){ // Checks if a room is connected to a given room number
    for(int i = 0; i<3; i++){ // Loops through connections array
        if (number == this->connections[i]){ // Checks if the number we're checking is in hte array
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------
Room ROOMS[20];
//--------------------------------------------------------------------------------
class Wumpus{ // Wumpus object class
private:
    int room;
public:
    Wumpus(int number);
    int getRoom(){return this->room;};
    void setRoom(int number){this->room = number-1;};
    void arrowShotMove();
    void moveRoom(int number);
};

/// Wumpus Constructor
/// \param number room number of wumpus
Wumpus::Wumpus(int number){
    this->room = number;
}

/// Moves Wumpus after an Arrow has bene shot
void Wumpus::arrowShotMove() {
    int lowest = 21;
    for (int i=0; i<3; i++){ // Finds the smallest room connected
        if (ROOMS[this->room].connections[i] < lowest){
            lowest = ROOMS[this->room].connections[i];
        }
    }
    this->moveRoom(lowest); // Moves to smallest adjacent room
}

/// Moves the Wumpus
/// \param number The room number we're moving the Wumpus too
void Wumpus::moveRoom(int number) {
    if (ROOMS[this->room].isConnected(number)){
        this->room = number;
    }
}
//--------------------------------------------------------------------------------
class Bats{ // Bats Class
private:
    int room;
public:
    Bats(int number){this->room = number;};
    void setRoom(int number){this->room = number-1;};
    int getRoom(){return this->room;};
};
//--------------------------------------------------------------------------------
class Pits{ // Pits Class
private:
    int room;
public:
    Pits(int number){this->room = number;};
    void setRoom(int number){this->room = number-1;};
    int getRoom(){return this->room;};
};
//--------------------------------------------------------------------------------
class Arrow{ // Arrow Class
    int room;
public:
    bool withPlayer= true;
    Arrow(int room){this->room = room;};
    void setRoom(int number){this->room=number-1;};
    int getRoom(){return this->room;};
    void shootArrow(int numRooms, Wumpus &wumpus);
    bool moveRoom(int number, int startingPoint, Wumpus wumpus);
    void moveRoom(int number);
    bool checkHitWumpus(Wumpus wumpus);
};

/// Checks if the Arrow hit the Wumpus
/// \param wumpus Wumpus Object
/// \return Returns false if the Wumpus was not hit
bool Arrow::checkHitWumpus(Wumpus wumpus) {
    if (this->getRoom() == wumpus.getRoom()){
        cout << "Wumpus has just been pierced by your deadly arrow!\n"
                "Congratulations on your victory, you awesome hunter you." << endl;
        cout << "\nExiting Program ..." << endl;
        exit(-1);
    }
    return false;
}

/// Shoots the Arrow
/// \param numRooms The number of rooms we're sending the arrow through
/// \param wumpus The Wumpus Object
void Arrow::shootArrow(int numRooms, Wumpus &wumpus) {
    int startingPoint = this->room; // Starting point of the Arrow
    for (int i=0; i<numRooms; i++){ // Loops through the rooms the arrow will go through
        int arrowMove;
        cin >> arrowMove;
        if (this->moveRoom(arrowMove-1, startingPoint, wumpus)){ // Calls the moveRoom function
            ;
        }
        else{
            break;
        }

    }
    this->withPlayer=false; // The Player no longer has the Arrow
    if (this->room == startingPoint){ // If the final room is back where the arrow started, you died
        cout << "You just shot yourself.\n"
                "Maybe Darwin was right.  You're dead." << endl << endl;
        cout << "Exiting Program ...";
        exit(-1);
    }
    if (!this->checkHitWumpus(wumpus)){ // Checks if you hit the Wumpus. If not, move the Wumpus.
        wumpus.arrowShotMove();
    }
}

/// Moves the Arrow to another room with the Player
/// \param number The room we're moving to
void Arrow::moveRoom(int number) {
    if (ROOMS[this->room].isConnected(number)){ // Checks if connected
        this->room = number;
    }
}

/// Moves the Arrow to other rooms after shot
/// \param number Room we're going to
/// \param startingPoint Starting point of the arrow
/// \param wumpus Wumpus Object
/// \return Returns if the arrow was moved succesfully
bool Arrow::moveRoom(int number, int startingPoint, Wumpus wumpus) {
    if (ROOMS[this->room].isConnected(number)){ // Checks if connected
        this->room = number;
        return true;
    }
    else{
        cout << "Room " << number+1 << " is not adjacent.  Arrow ricochets..." << endl; // Arrow Ricochets
        int lowest = 21;
        for (int i=0; i<3; i++){ // Finds the lowest room number adjacent
            if (ROOMS[this->room].connections[i] < lowest){
                lowest = ROOMS[this->room].connections[i];
            }
        }
        this->room = lowest;
        if (this->room == wumpus.getRoom()){ // Checks if the arrow ricochetted and killed the wumpus
            cout << "Your arrow ricochet killed the Wumpus, you lucky dog!\n"
                    "Accidental victory, but still you win!" << endl << endl;
            cout << "Exiting Program ...";
            exit(-1);
        }
        if (this->room == startingPoint){ // Checks if you just killed yourself with a Ricochet
            cout << endl;
            cout << "You just shot yourself, and are dying.\n"
                    "It didn't take long, you're dead."<< endl << endl;
            cout << "Exiting Program ...";

            exit(-1);
        }
        return false;

    }
}

//--------------------------------------------------------------------------------
class Player{ // Player object
private:
    int room;
public:
    Player(int number){this->room = number;};
    int getRoom(){return this->room;};
    void setRoom(int number){this->room = number-1;};
    bool moveRoom(int number);
    bool nearThreat(Wumpus wumpus);
    bool nearThreat(Bats bat);
    bool nearThreat(Pits pit);
};

/// Checks if the Player is near the Wumpus
/// \param wumpus Wumpus Object
/// \return Near the Wumpus or not
bool Player::nearThreat(Wumpus wumpus) {
    return ROOMS[this->room].isConnected(wumpus.getRoom());
}

/// Checks if the Player is near a Bat
/// \param bat Bat Object
/// \return Near a Bat or not
bool Player::nearThreat(Bats bat) {
    return ROOMS[this->room].isConnected(bat.getRoom());
}

/// Checks if the Player is near a Pit
/// \param pit Pit Object
/// \return Near a Pit or not
bool Player::nearThreat(Pits pit) {
    return ROOMS[this->room].isConnected(pit.getRoom());
}

/// Moves the Player between rooms
/// \param number Room number we're going to
/// \return True if it's a valid move
bool Player::moveRoom(int number) {
    if (ROOMS[this->room].isConnected(number)){ // Checks if the room is connected
        this->room = number;
        return true;
    }
    else{
        cout << "Invalid move.  Please retry."<<endl;
        return false;
    }
}
//--------------------------------------------------------------------------------
void displayCave()
{
    cout<< "       ______18______             \n"
        << "      /      |       \\           \n"
        << "     /      _9__      \\          \n"
        << "    /      /    \\      \\        \n"
        << "   /      /      \\      \\       \n"
        << "  17     8        10     19       \n"
        << "  | \\   / \\      /  \\   / |    \n"
        << "  |  \\ /   \\    /    \\ /  |    \n"
        << "  |   7     1---2     11  |       \n"
        << "  |   |    /     \\    |   |      \n"
        << "  |   6----5     3---12   |       \n"
        << "  |   |     \\   /     |   |      \n"
        << "  |   \\       4      /    |      \n"
        << "  |    \\      |     /     |      \n"
        << "  \\     15---14---13     /       \n"
        << "   \\   /            \\   /       \n"
        << "    \\ /              \\ /        \n"
        << "    16---------------20           \n"
        << endl;
}//end displayCave()

//--------------------------------------------------------------------------------
void displayInstructions()
{
    cout<< "Hunt the Wumpus:                                             \n"
        << "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
        << "room has 3 tunnels leading to other rooms.                   \n"
        << "                                                             \n"
        << "Hazards:                                                     \n"
        << "1. Two rooms have bottomless pits in them.  If you go there you fall and die.  \n"
        << "2. Two other rooms have super-bats.  If you go there, the bats grab you and    \n"
        << "   fly you to some random room, which could be troublesome.  Then those bats go\n"
        << "   to a new room different from where they came from and from the other bats.  \n"
        << "3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and   \n"
        << "   is too heavy for bats to lift.  Usually he is asleep.  Two things wake      \n"
        << "   him up: Anytime you shoot an arrow, or you entering his room.  When he      \n"
        << "   wakes he moves if he is in an odd-numbered room, but stays still otherwise. \n"
        << "   After that, if he is in your room, he eats you and you die!                 \n"
        << "                                                                               \n"
        << "Moves:                                                                         \n"
        << "On each move you can do the following, where input can be upper or lower-case: \n"
        << "1. Move into an adjacent room.  To move enter 'M' followed by a space and      \n"
        << "   then a room number.                                                         \n"
        << "2. Shoot your guided arrow through a list of up to three adjacent rooms, which \n"
        << "   you specify.  Your arrow ends up in the final room.                         \n"
        << "   To shoot enter 'S' followed by the number of rooms (1..3), and then the     \n"
        << "   list of the desired number (up to 3) of adjacent room numbers, separated    \n"
        << "   by spaces. If an arrow can't go a direction because there is no connecting  \n"
        << "   tunnel, it ricochets and moves to the lowest-numbered adjacent room and     \n"
        << "   continues doing this until it has traveled the designated number of rooms.  \n"
        << "   If the arrow hits the Wumpus, you win! If the arrow hits you, you lose. You \n"
        << "   automatically pick up the arrow if you go through a room with the arrow in  \n"
        << "   it.                                                                         \n"
        << "3. Enter 'R' to reset the person and hazard locations, useful for testing.     \n"
        << "4. Enter 'C' to cheat and display current board positions.                     \n"
        << "5. Enter 'D' to display this set of instructions.                              \n"
        << "6. Enter 'P' to print the maze room layout.                                    \n"
        << "7. Enter 'X' to exit the game.                                                 \n"
        << "                                                                               \n"
        << "Good luck!                                                                     \n"
        << " \n"
        << endl;
}//end displayInstructions()

//--------------------------------------------------------------------------------

/// Sets the random variables and makes sure they're unique
/// \param randArray Array of Random Numbers to store data in
/// \param index Index value we're dumping stuff into the Array from
/// \return The random number we generated
int setRandom(int* randArray, int index){
    bool done = false;
    while(!done) {
        cnt:;
        int randNum = rand() % 20; // Generates Random number
        for (int i = 0; i < 6; i++) { // Checks if the random number already exists in the array
            if (randNum == randArray[i]) {
                goto cnt; // Goes back to the top of the loop
            }
        }
        randArray[index] = randNum; // Stores random number in the array
        return randNum;
    }
}
//--------------------------------------------------------------------------------


int main()
{
    int turn = 1;
    // Seed the random number generator.  Change seed to time(0) to change output each time.
    // srand(time(0));
    srand(1);   // Use this version in what you turn in

    // Setup
    int bat1num,bat2num,pit1num,pit2num,wumpusnum,playernum,arrownum;
    int randArray[6];
    bool finished = false;

    // Object Initializations
    Bats bats1 = Bats(setRandom(randArray,0));
    Bats bats2 = Bats(setRandom(randArray,1));
    Pits pit1 = Pits(setRandom(randArray,2));
    Pits pit2 = Pits(setRandom(randArray,3));
    Wumpus wumpus = Wumpus(setRandom(randArray,4));
    Player player = Player(setRandom(randArray,5));
    Arrow arrow = Arrow(player.getRoom());


    //Initialize Rooms
    for (int x = 0; x<20; x++){
        ROOMS[x] = Room(x);
        ROOMS[x].setConnection(AllConnections[x]);
    }


    // Runtime Loop
    while (!finished) {
        if (player.getRoom() == wumpus.getRoom()) { // Checks if you went into the Wumpus Room
            if(player.getRoom() % 2 != 0) { // Checks if the room is divisible by 2
                cout << "You briefly feel a slimy tentacled arm as your neck is snapped.\n"
                        "It is over.";
                break;
            }
            else{
                wumpus.arrowShotMove();
                cout << "You hear a slithering sound, as the Wumpus slips away.\n"
                        "Whew, that was close!" << endl;
            }
        }
        if (player.getRoom() == pit1.getRoom() || player.getRoom() == pit2.getRoom()){ // Checks if you fell in a pit
            cout << "Aaaaaaaaahhhhhh.... \n"
                    "     You fall into a pit and die.";
            break;
        }
        if (player.getRoom() == bats1.getRoom()){ // Checks if you went into a Bat room.
            int newRoom = rand()%20 + 1;
            int newBatRoom = rand()%20 + 1;
            player.setRoom(newRoom); // Moves Player randomly
            bats1.setRoom(newBatRoom); // Moves Bat randomly
            cout << "Woah... you're flying!\n"
                    "You've just been transported by bats to room " << newRoom << endl;
            continue;
        } // Checks if you went in the other bat room
        if (player.getRoom() == bats2.getRoom()){
            int newRoom = rand()%20 + 1;
            int newBatRoom = rand()%20 + 1;
            player.setRoom(newRoom); // Moves Player randomly
            bats2.setRoom(newBatRoom); // Moves Bat Randomly
            cout << "Woah... you're flying!\n"
                    "You've just been transported by bats to room " << newRoom << endl;
            continue;
        }



        cout << "You are in room " << player.getRoom() + 1 << ".";
        if (player.nearThreat(wumpus)) { // Checks if near Wumpus
            cout << " You smell a stench.";
        }
        if(player.nearThreat(pit1) || player.nearThreat(pit2)){ // Checks if near a Pit
            cout << " You feel a draft.";
        }
        if(player.nearThreat(bats1) || player.nearThreat(bats2)){ //Checks if near a Bat
            cout << " You hear rustling of bat wings.";
        }

        cout << endl << endl;
        cout << turn << ".";
        cout << " Enter your move (or 'D' for directions): " << endl;

        char choice;
        cin >> choice;
        switch (std::toupper(choice)) { // Checks what the User is going to do
            case 'C': // Cheating
                cout << "Cheating! Game elements are in the following rooms: \n"
                     << "\nPlayer Wumpus Bats1 Bats2 Pit1 Pit2 Arrow\n"
                     << setw(4) << player.getRoom()+1 << setw(7) << wumpus.getRoom()+1 << setw(7)
                     << bats1.getRoom()+1 << setw(6) << bats2.getRoom()+1 << setw(5)
                     << pit1.getRoom()+1 << setw(5) << pit2.getRoom()+1 <<  setw(6)
                     << arrow.getRoom()+1 << endl << endl;
                break;
            case 'S': // Shooting Arrow
                if (!arrow.withPlayer){ // Checks if Player has the Arrow
                    cout << "Sorry, you can't shoot an arrow you don't have.  Go find it." << endl;
                }
                else { // Shootes the Arrow
                    arrow.setRoom(player.getRoom()+1);
                    cout << "Enter the number of rooms (1..3) into which you want to shoot, followed by the rooms themselves:" << endl;
                    int numRooms;
                    cin >> numRooms;
                    arrow.shootArrow(numRooms, wumpus);

                }
                turn++;
                break;
            case 'R': // Resetting
                cout << "(Remember arrow value of -1 means it is with the person.)" << endl;
                cout << "Enter the 7 room locations (1..20) for player, wumpus, bats1, bats2, pit1, pit2, and arrow:" << endl << endl << endl;
                cin >> playernum;
                cin >> wumpusnum;
                cin >> bat1num;
                cin >> bat2num;
                cin >> pit1num;
                cin >> pit2num;
                cin >> arrownum;

                player.setRoom(playernum);
                wumpus.setRoom(wumpusnum);
                bats1.setRoom(bat1num);
                bats2.setRoom(bat2num);
                pit1.setRoom(pit1num);
                pit2.setRoom(pit2num);
                (arrownum == -1) ? arrow.setRoom(player.getRoom()+1) : arrow.setRoom(arrownum);
                break;
            case 'D': // Display the Instructions
                displayInstructions();
                break;
            case 'P': // Display the Map
                displayCave();
                break;
            case 'X': // Exit game
                finished = true;
                break;
            case 'M': // Move
                int move;
                cin >> move;
                if (arrow.withPlayer) { // Moves arrow with player
                    arrow.moveRoom(move - 1);
                    (player.moveRoom(move - 1)) ? turn++ : turn=turn;
                }
                else{ // Moves player without arrow.
                    (player.moveRoom(move - 1)) ? turn++ : turn=turn;
                    if (arrow.getRoom() == player.getRoom()){ // Picks up arrow if in same room
                        cout << "Congratulations, you found the arrow and can once again shoot." << endl;
                        arrow.withPlayer=true;
                        arrow.setRoom(-1);
                    }
                }
                break;
        }

    }
    cout << "\nExiting Program ..." << endl;
}//end main()
