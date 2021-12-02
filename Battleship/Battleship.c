#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define BOARDROWS 9
#define BOARDCOLS 11

bool dumpComputer = true; //Causes Dump of Computer Board before Game if True (for Debugging)

int playerBoard[BOARDROWS+1][BOARDCOLS+1];
int computerBoard[BOARDROWS+1][BOARDCOLS+1];
int compShotBoard[BOARDROWS+1][BOARDCOLS+1];
int invalidInputs();
int outsideBoard();

int getRand(int lowval,int highval)
{
    return (rand()%(highval+1-lowval) + lowval);
}

int getShot(bool fromUser, int board[BOARDROWS+1][BOARDCOLS+1])
{
    int row,col;
    
    while(1)
    {
        if(fromUser)
        {
            printf("Give a shot (row, col):");
            scanf("%d%d",&row,&col);
            if(col<1 || col > BOARDCOLS || row < 1 || row > BOARDROWS)
                printf("Invalid input\n");
            else  
                break;
        }
        //Computer Generated
        else 
        { 
            row=getRand(1,BOARDROWS);
            col=getRand(1,BOARDCOLS);
            if(compShotBoard[row][col]==0) 
            { 
                compShotBoard[row][col]=1; 
                break;
            }
        }
    } //Will Leave this Loop with Valid Input
    if(board[row][col] != 0)
    {
        if(board[row][col]>0)
            board[row][col]= -1*board[row][col]; 
        return -1*board[row][col]; 
    }
    return 0; 
}
            
bool allShipsNotHit(int board[BOARDROWS+1][BOARDCOLS+1])
{
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>0) //Ship and Not Hit
                return(true);
        }
    }
    return (false); //No Ships Found Not All Hit
}

bool noneLeft(int valueToFind,int board[BOARDROWS+1][BOARDCOLS+1])
{
    for(int i=1;i<=BOARDROWS;i++){
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]==valueToFind) //Ship and Not Hit
                return(false);
        }
    }
    return (true); //No Ships Found, All Hit
}

void populateBoard(bool getUserInput, int board[BOARDROWS+1][BOARDCOLS+1]) //Populate Board Function to Set Up Initial Battleships
{
    if (getUserInput==true) //For Player Input
    {
        int startingRow, startingColumn, orientation, legalPermission=1, k=5; //Initialize Variables
        printf("Rows are 1 - 9, Columns are 1 - 11\n");
        printf("Orientation is 0 for across, 1 for down\n");

        while(legalPermission==1&&k>=1) //While Loop to Continue for Each Size of Battleship
        {
            do //Checks for Legality, If not, Keep Requesting Input
            {
                printf("Give starting row, starting column and orientation (3 inputs) for ship of size=%d:", k);
                scanf("%d %d %d", &startingRow, &startingColumn, &orientation); //User Input
                legalPermission=1; //Legality Check for Invalid Input, 1 for True
                int legalPermissionOne=1; //Legality Check for Overlap
                if (invalidInputs(startingRow,startingColumn,orientation)==1) //Calls invalidInputs Function to check if Inputted Number is within Parametres
                    {
                        legalPermission=0; //Sets to False
                    }
                if (legalPermission==1) //Skips Section if Initial Input if Off
                {
                    if (orientation==0) //Horizontal Orientation
                    {
                        for (int i=0; i<k;i++) //Fop Loop to Check for 0, versus previous Ship
                        {
                            if (board[startingRow][startingColumn+i]!=0) //Change Legality Checks to False
                            {
                                legalPermission=0;
                                legalPermissionOne=0;
                            }
                        }
                        if (legalPermissionOne==0) //Output for Conflicting Ships
                            printf("Conflicts with ship already placed\n");
                    }
                    if (orientation==1) //Vertical Orientation
                    {
                        for (int i=0; i<k;i++) //For Loop to Check for Overlap with Previous Ships
                        {
                            if ((board[startingRow+i][startingColumn]!=0)) //Change Legalities
                            {    
                                legalPermission=0;
                                legalPermissionOne=0;
                            }
                        }
                        if (legalPermissionOne==0) //Output if Wrong
                            printf("Conflicts with ship already placed\n");
                    }
                    if (outsideBoard(k,startingRow,startingColumn,orientation)==1) //Checks if Ship goes Outside Board with outsideBoard Function
                        {
                            legalPermission=0; //Changes Legality Check
                        }
            }
            if (legalPermission==0 && legalPermissionOne==1) //Outputs Invalid Input and Prompts Repeat Input
                {
                    printf("Invalid Input\n");
                }
            }while(legalPermission==0);

            for (int i=0; i<k;i++) //Fills the Board
            {
                if (orientation==0)
                    board[startingRow][startingColumn+i]=k;
                if (orientation==1)
                    board[startingRow+i][startingColumn]=k;
            }
            legalPermission=1; //Resets Permission
            k--; //Decrease of Ship Size
        }
    }

    if (getUserInput==false) //Computer Input
    {
        int startingRow, startingColumn, orientation, legalPermission=1, k=5; //Initialize Variables
        while(legalPermission==1&&k>=1) //Legality Check and Ship Size Check for While Loop
        {
            do //Loop that Goes On Until Proper Input, Does not Overlap, or Over Board
            {
                //Seeds for Random Values 
                startingRow=getRand(1,9);
                startingColumn=getRand(1,11);
                orientation=getRand(0,1);

                legalPermission=1;
                if (invalidInputs(startingRow,startingColumn,orientation)==1) //Checks for Proper Input with Function
                    {
                        legalPermission=0;
                    }
                if (orientation==0) //For Horizontal, Checks if we Overlap
                {
                    for (int i=0; i<k;i++)
                    {
                        if (board[startingRow][startingColumn+i]!=0)
                            {
                                legalPermission=0;
                            }
                    }
                }
                if (orientation==1) //For Vertical,  Check if Ships Overlap
                {
                    for (int i=0; i<k;i++)
                    {
                        if ((board[startingRow+i][startingColumn]!=0))
                        {    
                            legalPermission=0;
                        }
                    }
                }
                if (outsideBoard(k,startingRow,startingColumn,orientation)==1) //Checks if Ship goes Overboard with Function
                    {
                        legalPermission=0;
                    }

            }while(legalPermission==0); //Repeats Input if Does not Work

            for (int i=0; i<k;i++) //Fills the Board with said information
            {
                if (orientation==0)
                    board[startingRow][startingColumn+i]=k;
                if (orientation==1)
                    board[startingRow+i][startingColumn]=k;
            }
            legalPermission=1;
            k--; //Decreases Ship Size
        }
    }
    
}

int outsideBoard (int k, int startingRow, int startingColumn, int orientation) //Outside Board Function
{
    int outputs = 0; //Sets Checker
    if (orientation==0) //Horizontal
    {
        if (startingColumn+k-1>11) //If the Ship is Outside Perscribed Column Value, Changes Checker
            outputs = 1;
    }
    if (orientation==1) //Vertical
    {
        if (startingRow+k-1>9) //If the Ship is Outside Perscribed Row Value, Changes Checker
            outputs = 1;
    }
    return outputs; //Return Checker Value
}

int invalidInputs (int startingRow, int startingColumn, int orientation) //Invalid Inputs Function
{
    int outputs = 0; //Sets Checker 
    if (startingRow<1 || startingRow >9) //If Row is not in 1-9, Change Checker
        outputs = 1;
    if (startingColumn<1 || startingColumn>11) //If Column is not in 1-11, Change Checker
        outputs = 1;
    if (orientation<0 || orientation >1) //If Orientation is not in 0-1, Change Checker
        outputs = 1;
    return outputs; //Return Checker
}


void showBoard(int board[BOARDROWS+1][BOARDCOLS+1]){
    printf("   Cols\n");
    printf("    1  2  3  4  5  6  7  8  9 10 11\n");
    printf("     ________________________________\n");
    for(int i=1;i<=BOARDROWS;i++){
        printf("R%d|",i);
        for(int j=1;j<=BOARDCOLS;j++){
            if(board[i][j]>=0)
                printf(" ");
            printf("%d ",board[i][j]);
        }
        printf("|\n");
    }
    printf("  __________________________________\n");
}

int main(int argc, char **argv)
{
    bool playerBoardOK,computerBoardOK;
    
    // initialize the seed randomly. 

        srand(time(NULL)); 

    //init the boards
    for(int i=0;i<BOARDROWS;i++){
        for(int j=0;j<BOARDCOLS;j++){
            playerBoard[i][j]=0;
            computerBoard[i][j]=0;
            compShotBoard[i][j]=0;
        }
    }

    populateBoard(true,playerBoard);
    populateBoard(false,computerBoard);
    printf("Your board is\n");
    showBoard(playerBoard);
    if(dumpComputer){
        printf("\n\nComputer board is\n");
        showBoard(computerBoard);
    }

    // play starts here
    playerBoardOK=true;
    computerBoardOK=true;
    while(playerBoardOK && computerBoardOK){
        int hit=getShot(true,computerBoard); //shot from user
        if(hit){
            printf("HIT on size %d\n",hit);
            if(noneLeft(hit,computerBoard))
                printf("Sunk computer's %d\n",hit);
        }
        else
            printf("MISS!!\n");
        hit=getShot(false,playerBoard);
        if(hit){
            printf("Computer hits! Size=%d\n", hit);
            if(noneLeft(hit,playerBoard))
                printf("Sunk your %d\n",hit);
        }
        else
            printf("Computer miss!\n");
        playerBoardOK = allShipsNotHit(playerBoard);
        computerBoardOK = allShipsNotHit(computerBoard);
//    showBoard(playerBoard); //debug
//    showBoard(computerBoard); //debug
   }
   if(playerBoardOK)
       printf("You win!\n");
    else
        printf("You lose.\n");
    showBoard(computerBoard);

    return 0;
}
