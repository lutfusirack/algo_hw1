/*
Name: Lütfü Sirac Küçükarabacıoğlu
ID: 504211521
Project1-BLG336E
*/

#include <iostream> 
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <queue>
#include <chrono>    

#define DFS 0
#define BFS 1

using namespace std;

class node {

    public:
        int x;
        int y;
        bool isFull;
        bool isVisited;
        node * right;
        node * left;
        node * up;
        node * down;

        node(int X,int Y){
            x = X;
            y = Y;
            isFull = false;
            right = NULL;
            left = NULL;
            up = NULL;
            down = NULL;
            isVisited = false;
        };

};

class map{

    public:

        node * head;
        int numberofFullNode;
        map(){
            head = NULL;
            numberofFullNode = 0;
        }
};

class ship{
    public:
        int xbegin;
        int xend;
        int ybegin;
        int yend;
        int size;
        char axis;

        ship(int Xbegin,int Ybegin,int Xend,int Yend){
            xbegin = Xbegin;
            xend = Xend;
            ybegin = Ybegin;
            yend = Yend;
            if(Xbegin != Xend){
                size = abs(Xbegin - Xend)+1; // Added bcs we need size of the ship also in the xend point
                axis = 'x';
            }
            else if(Ybegin != Yend){
                size = abs(Ybegin - Yend)+1;
                axis = 'y';
            }
            else{
                size = 1;
                axis = 'x';
            }
        }
};

class player{
    public:
        string algo_s;
        int algorithm; 
        int x;  //  initial point of player1
        int y;  //  initial point of player1
        int sizeofMap;
        int numberofShips;
        vector<ship*> ships;
        map Map; // Player's playground map is  linked list of node 
        int numberofNodesKeptInMemory;
        int numberofVisitedNode;
};


player P1,P2;

void read_players(string splayer1,string splayer2);
node * buildgraph(int size);
void filltheMap(player * p_player);
void placetheShip(player * p_player,ship * p_ship);
int startTheGame(void);
void DFSrec(node * p_node_p1,node * p_node_p2,int turn,int * winner);
int DFSGame();
int BFSGame();
void log(int winner,int time);

int main(int argc, char* argv[]) {
    // std::chrono::time_point<std::chrono::steady_clock> start, end;
    // start = std::chrono::steady_clock::now();
    auto start = std::chrono::steady_clock::now();
    system("clear");// make this line as comment if you are compiling on Linux or Mac
	//system("cls"); // make this line as comment if you are compiling on Windows
    string dirOfplayer1 = string(argv[1]);
    string dirOfplayer2 = string(argv[2]);

    read_players(dirOfplayer1,dirOfplayer2);
    // initMaps();

    P1.Map.head = buildgraph(P1.sizeofMap);    
    P2.Map.head = buildgraph(P2.sizeofMap);    

    P1.numberofNodesKeptInMemory = P1.sizeofMap*P1.sizeofMap;
    P2.numberofNodesKeptInMemory = P2.sizeofMap*P2.sizeofMap;
    
    filltheMap(&P1);
    filltheMap(&P2);

    int winner = startTheGame();
    // end = std::chrono::steady_clock::now();
    // std::chrono::duration<std::chrono::milliseconds> elapsed_seconds = end - start;
    // cout<<elapsed_seconds.count()<<endl;
    int time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
    log(winner,time);

    // read_players(dirOfplayer1,dirOfplayer2);
return 0;
}
void log(int winner,int time){

    cout<<"The algorithm: Player1 "<<P1.algo_s<<", Player2: "<<P2.algo_s<<endl;
    cout<<"The number of visited nodes: Player1: "<<P1.numberofVisitedNode<<", Player2: "<<P2.numberofVisitedNode<<endl;
    cout<<"The number of nodes kept in the memory: Player1: "<<P1.numberofNodesKeptInMemory<<", Player2: "<<P2.numberofNodesKeptInMemory<<endl;
    cout<<"The result: Player"<<winner<<" won!"<<endl;
    cout<<"The running time: "<<time<<"ms"<<endl;
}

int startTheGame(){

    int winner=0;

    if (P1.algorithm == BFS)
    {
        winner = BFSGame();
    }
    else if (P1.algorithm == DFS)
    {
        winner = DFSGame();
    }
    // cout<<"winner of the game "<<winner<<endl;
    return winner;
}

void filltheMap(player * p_player){

    // cout<<p_player->ships.size()<<endl;
    vector<ship*> temp_q = p_player->ships;
    for (size_t i = 0; i < temp_q.size(); i++)
    {
        placetheShip(p_player,temp_q[i]);
    }

    

}

void placetheShip(player * p_player,ship * p_ship){

    node * temp = p_player->Map.head;
    
    while (temp->x < p_ship->xbegin)
    {
        temp = temp->down;
    }
    while (temp->y < p_ship->ybegin)
    {
        temp = temp->right;
    }    

    if(p_ship->axis == 'x'){

        for (int i = 0; i < p_ship->size; i++)
        {
            temp->isFull = true;
            temp = temp->down;
            p_player->Map.numberofFullNode++;
        }
    
    }
    else if(p_ship->axis == 'y'){

        for (int i = 0; i < p_ship->size; i++)
        {
            temp->isFull = true;
            temp = temp->right;
            p_player->Map.numberofFullNode++;
        }
        
    }

}

node * buildgraph(int size){

    node * nodeList[size][size];

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            nodeList[i][j] = new node(i,j);
            if(j != 0){
                nodeList[i][j-1]->right = nodeList[i][j];
                nodeList[i][j]->left = nodeList[i][j-1];
            }
            if(i != 0){
                nodeList[i-1][j]->down = nodeList[i][j];
                nodeList[i][j]->up = nodeList[i-1][j];

            }
        }
        
    }
    return nodeList[0][0];

}

void read_players(string splayer1,string splayer2){

    string temp_char;
    ifstream InputReadFile1(splayer1);
    ifstream InputReadFile2(splayer2);
    int counter = 0;

    while (getline(InputReadFile1, temp_char))
    {
        // cout<<temp_char<<endl;
        if(counter==0 && memcmp(temp_char.c_str(),"DFS",3) == 0){
            P1.algo_s = "DFS";
            P1.algorithm = DFS;
        }
        else if(counter==0 && memcmp(temp_char.c_str(),"BFS",3) == 0){
            P1.algorithm = BFS;
            P1.algo_s = "BFS";
        }
        else if(counter == 1){
            // cout<<temp_char[1]<<endl;
            P1.x = (int)temp_char[0] - '0';
            P1.y = (int)temp_char[2] - '0';
        }
        else if(counter == 2)
        {
            P1.sizeofMap = (int)temp_char[0] - '0';
            P1.numberofShips = (int)temp_char[2] - '0';
        }
        else{

           ship * temp = new ship((int)temp_char[0] - '0',(int)temp_char[2] - '0',(int)temp_char[4] - '0',(int)temp_char[6] - '0');
           P1.ships.push_back(temp);
        }

        // s_player1.push_back(temp_char);
        counter++;
    }
    counter = 0;

    while (getline(InputReadFile2, temp_char))
    {
        if(counter==0 && memcmp(temp_char.c_str(),"DFS",3) == 0){
            P2.algo_s = "DFS";
            P2.algorithm = DFS;
        }
        else if(counter==0 && memcmp(temp_char.c_str(),"BFS",3) == 0){
            P2.algo_s = "BFS";
            P2.algorithm = BFS;
        }
        else if(counter == 1){
            // cout<<temp_char[1]<<endl;
            P2.x = (int)temp_char[0] - '0';
            P2.y = (int)temp_char[2] - '0';
        }
        else if(counter == 2)
        {
            P2.sizeofMap = (int)temp_char[0] - '0';
            P2.numberofShips = (int)temp_char[2] - '0';
        }
        else{

           ship * temp = new ship((int)temp_char[0] - '0',(int)temp_char[2] - '0',(int)temp_char[4] - '0',(int)temp_char[6] - '0');
           P2.ships.push_back(temp);
        }
    counter++;   
    }

}

int BFSGame(){

    node * tempP2 = P2.Map.head;
    node * tempP1 = P1.Map.head;

    for (int i = 0; i < P1.x; i++)
    {
        tempP2 = tempP2->down;
    }
    for (int i = 0; i < P1.y; i++)
    {
        tempP2 = tempP2->right;
    }
        
    for (int i = 0; i < P2.x; i++)
    {
        tempP1 = tempP1->down;
    }
    for (int i = 0; i < P2.y; i++)
    {
        tempP1 = tempP1->right;
    }

    queue<node*> P1_q;  // Player1 node queue
    queue<node*> P2_q;  // Player2 node queue

    P1_q.push(tempP2);
    P2_q.push(tempP1);

    P1.numberofNodesKeptInMemory++;
    P2.numberofNodesKeptInMemory++;

    while (!P1_q.empty() && !P2_q.empty())
    {
        // Player1 turn
        node * temp = P1_q.front();
        P1_q.pop();
        P1.numberofVisitedNode++;
        temp->isVisited = true;
        if(temp->isFull){
            temp->isFull = false;
            P2.Map.numberofFullNode--;
        }
        if(temp->up != NULL && !temp->up->isVisited){
            P1_q.push(temp->up);
            P1.numberofNodesKeptInMemory++;
            temp->up->isVisited = true;
        }
        if(temp->left != NULL && !temp->left->isVisited){
            P1_q.push(temp->left);
            P1.numberofNodesKeptInMemory++;
            temp->left->isVisited = true;
        }
        if(temp->down != NULL && !temp->down->isVisited){
            P1_q.push(temp->down);
            P1.numberofNodesKeptInMemory++;
            temp->down->isVisited = true;

        }
        if(temp->right != NULL && !temp->right->isVisited){
            P1_q.push(temp->right);
            P1.numberofNodesKeptInMemory++;
            temp->right->isVisited = true;
        }

        if(P2.Map.numberofFullNode == 0){
            return 1;
        }
    
        // Player2 turn
        temp = P2_q.front();
        P2_q.pop();
        P2.numberofVisitedNode++;
        if(temp->isFull){
            temp->isFull = false;
            P1.Map.numberofFullNode--;
        }
        if(temp->up != NULL && !temp->up->isVisited){
            P2_q.push(temp->up);
            P2.numberofNodesKeptInMemory++;
            temp->up->isVisited = true;
        }
        if(temp->left != NULL && !temp->left->isVisited){
            P2_q.push(temp->left);
            P2.numberofNodesKeptInMemory++;
            temp->left->isVisited = true;
        }
        if(temp->down != NULL && !temp->down->isVisited){
            P2_q.push(temp->down);
            P2.numberofNodesKeptInMemory++;
            temp->down->isVisited = true;
        }
        if(temp->right != NULL && !temp->right->isVisited){
            P2_q.push(temp->right);
            P2.numberofNodesKeptInMemory++;
            temp->right->isVisited = true;

        }
        
        if(P1.Map.numberofFullNode == 0){
            return 2;
        }

    }
    

return -1;
}

int DFSGame(){

    node * tempP2 = P2.Map.head;    // starting point for Player1 at Player2's map
    node * tempP1 = P1.Map.head;    // starting point for Player2 at Player1's map

    for (int i = 0; i < P1.x; i++)
    {
        tempP2 = tempP2->down;
    }
    for (int i = 0; i < P1.y; i++)
    {
        tempP2 = tempP2->right;
    }
        
    for (int i = 0; i < P2.x; i++)
    {
        tempP1 = tempP1->down;
    }
    for (int i = 0; i < P2.y; i++)
    {
        tempP1 = tempP1->right;
    }

    int winner = 0;
    DFSrec(tempP2,tempP1,1,&winner);
    return winner;
}

void DFSrec(node * p_node_p1,node * p_node_p2,int turn,int * winner){

    if (*winner != 0)
        return;

    if (turn == 1) // player1 turn
    {   
        P1.numberofVisitedNode++;
        // check is there any unisVisited close node
        bool empty_room = false;
        if(p_node_p1->up != NULL && !p_node_p1->up->isFull)
            empty_room = true;
        if(p_node_p1->left != NULL && !p_node_p1->left->isFull)
            empty_room = true;
        if(p_node_p1->down != NULL && !p_node_p1->down->isFull)
            empty_room = true;
        if(p_node_p1->right != NULL && !p_node_p1->right->isFull)
            empty_room = true;
        if(empty_room == false)
            return;

        if (p_node_p1->isFull)
        {
            p_node_p1->isFull = false;
            P2.Map.numberofFullNode--;
        }

        if(P2.Map.numberofFullNode == 0){
            *winner = 1;
            return;
        }


        p_node_p1->isVisited = true;
        if(p_node_p1->up != NULL && !p_node_p1->up->isVisited && *winner == 0){
            DFSrec(p_node_p1->up,p_node_p2,2,winner);
            P1.numberofNodesKeptInMemory++;
            // p_node_p1->up->isVisited = true;
        }
        if(p_node_p1->left != NULL && !p_node_p1->left->isVisited && *winner == 0){
            DFSrec(p_node_p1->left,p_node_p2,2,winner);
            P1.numberofNodesKeptInMemory++;
            // p_node_p1->left->isVisited = true;
        }
        if(p_node_p1->down != NULL && !p_node_p1->down->isVisited && *winner == 0){
            DFSrec(p_node_p1->down,p_node_p2,2,winner);
            P1.numberofNodesKeptInMemory++;
            // p_node_p1->down->isVisited = true;
        }
        if(p_node_p1->right != NULL && !p_node_p1->right->isVisited && *winner == 0){
            DFSrec(p_node_p1->right,p_node_p2,2,winner);
            P1.numberofNodesKeptInMemory++;
            // p_node_p1->right->isVisited = true;
        }
        

    }
    
    if (turn == 2) // player2 turn
    {   
        P2.numberofVisitedNode++;
        // check is there any unisVisited close node
        bool empty_room = false;
        if(p_node_p2->up != NULL && !p_node_p2->up->isFull)
            empty_room = true;
        if(p_node_p2->left != NULL && !p_node_p2->left->isFull)
            empty_room = true;
        if(p_node_p2->down != NULL && !p_node_p2->down->isFull)
            empty_room = true;
        if(p_node_p2->right != NULL && !p_node_p2->right->isFull)
            empty_room = true;
        if(empty_room == false)
            return;

        if (p_node_p2->isFull)
        {
            p_node_p2->isFull = false;
            P1.Map.numberofFullNode--;
        }

        if(P1.Map.numberofFullNode == 0){ // check is there any ship node
            *winner = 2;
            return;
        }


        p_node_p2->isVisited = true;

        if(p_node_p2->up != NULL && !p_node_p2->up->isVisited && *winner == 0){
            DFSrec(p_node_p1,p_node_p2->up,1,winner);
            P2.numberofNodesKeptInMemory++;
        }
        if(p_node_p2->left != NULL && !p_node_p2->left->isVisited && *winner == 0){
            DFSrec(p_node_p1,p_node_p2->left,1,winner);
            P2.numberofNodesKeptInMemory++;
        }
        if(p_node_p2->down != NULL && !p_node_p2->down->isVisited && *winner == 0){
            DFSrec(p_node_p1,p_node_p2->down,1,winner);
            P2.numberofNodesKeptInMemory++;
        }
        if(p_node_p2->right != NULL && !p_node_p2->right->isVisited && *winner == 0){
            DFSrec(p_node_p1,p_node_p2->right,1,winner);
            P2.numberofNodesKeptInMemory++;
        }
        

    }
    





}