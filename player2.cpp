#include <stdio.h> 
#include <iostream>
#include <fstream>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include "files.h"

using namespace std;

pthread_t countScore;

string p = "Player2: ";

int main()
{
    initBoard();

    mkfifo("board", 0666);
    ifstream f("time.txt", ios::in);

    int pipe;

    while (1)
    {
        displayBoard();
        cout << "Waiting for your opponent move...." << endl;
        pipe = open("board", O_RDONLY);
        read(pipe, boardArr, 9);
        if (boardArr[0][0] == 'L'){
            system("clear");
            displayBoard();
            cout << "HARD LUCK! You Lost!" << endl;
            close(pipe);
            pthread_create(&countScore, NULL, writeHighScore, &p);
            pthread_join(countScore, NULL);
            return 0;
        }
        if (boardArr[0][0] == 'D'){
            system("clear");
            displayBoard();
            cout << "DRAW! No one won!" << endl;
            close(pipe);
            return 0;
        }
        system("clear");
        close(pipe);
        
        displayBoard();
        cout << "Your move... " << endl;
        placeToken('O', p);
        pipe = open("board", O_WRONLY);
        if (checkWinningCond()) {
            system("clear");
            displayBoard();
            write(pipe, "LLLLLLLLL", 9);
            cout << "CONGRATULATIONS! You won!" << endl;
            close(pipe);
            return 0;
        }
        if (occupiedPlaces <= 0){
            system("clear");
            displayBoard();
            write(pipe, "DDDDDDDD", 9);
            cout << "DRAW! No one won!" << endl;
            close(pipe);
            return 0;
        }
        write(pipe, boardArr, 9);
        close(pipe);
        system("clear");
    }
}