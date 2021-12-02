/*
 * File:        ReversiPartB.c
 * Author:      Nelson Lee
 *
 * Date: April 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "project_reversi_skeleton.h" 
#include <string.h>
#include "ReversiPart2Lib.h"

/*
 * Function:  printBoard 
 * --------------------
 * Lays out the Format for the Axis Titles and then fills out the Board Array.
 */
void printBoard(char board[][26], int n) 
{
    int num; //Initialize Variable
    printf("  ");

    //Title Letters
    for (int i=0;i<n;i++)
    {
        num = 97+i;
        printf("%c", num);
    }
    printf("\n");

    //Prints Actual Board
    for (int j=0;j<n;j++) //Row
    {
        num = 97+j;
        printf("%c ", num);

        for (int k=0;k<n;k++) //Column
        {
            printf("%c", board[j][k]);
        }
        printf("\n");
    }
}

/*
 * Function:  positionInBounds 
 * --------------------
 * Checks whether the specified (row, col) lies within the board dimensions.
 */
bool positionInBounds(int n, int row, int col) 
{
    bool legalCheck = true; //Sets Basic Boolean
    //Checks if Rows and Columns are Within Bounds
    if (row>n-1 || row<0)
        legalCheck=false;
    if (col>n-1 || col<0)
        legalCheck=false;
    return legalCheck; //Return
}

/*
 * Function:  moveTiles 
 * --------------------
 * Function to Flip over Tiles once Legal Checks are Done.
 */
void moveTiles(char board[26][26], int n, int row, int col, char colour) 
{
    for (int deltaRow = -1; deltaRow <= 1; deltaRow++) //Sets up initial Delta Values for Movement in Rows
    {
        for (int deltaCol = -1; deltaCol <= 1; deltaCol++) //Sets up initial Delta Values for Movement in Columns
        {
            if (deltaRow != 0 || deltaCol != 0) //Checks if the Value if Not 0 
            {
                //Check Legality
                if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol) == true) 
                {
                    //Create a counter variable
                    int counter = 1; 
                    //Makes Flips in Between Colours
                    while (board[row+counter*deltaRow][col+counter*deltaCol]!=colour) 
                    {
                        board[row+counter*deltaRow][col+counter*deltaCol]=colour;
                        counter++; //Adds
                    }
                }
            }
        }
    }
    return;
}

/*
 * Function:  nextTileCheck 
 * --------------------
 * Function to Check the Next Tiles for the Board
 */
bool nextTileCheck(char board[][26], int n, int row, int col, int deltaRow, int deltaCol, char colour) 
{
    //Check if the Values are in Bound
    if (positionInBounds(n,row+deltaRow,col+deltaCol)==false) 
        return false;   
    //Check  if the next Value has the same colour as the input
    if ((int)board[row+deltaRow][col+deltaCol]==(int)colour)
        return true; 
    //Check if the Following Value is Occupied
    if ((int)board[row+deltaRow][col+deltaCol]==85)
        return false;

    //If White
    if ((int)colour == 87) 
    {
        if ((int)board[row+deltaRow][col+deltaCol] == 66) //Checks if Opposite Colour
        {
            if (nextTileCheck(board,n,row+deltaRow,col+deltaCol,deltaRow,deltaCol,colour) == true)
                return true;
            else
                return false;
        }
    }
    //If Black
    if ((int)colour == 66) 
    {
        if ((int)board[row+deltaRow][col+deltaCol] == 87) //Checks if Opposite Colour
        {
            if (nextTileCheck(board,n,row+deltaRow,col+deltaCol,deltaRow,deltaCol,colour) == true)
                return true;
            else
                return false;
        }
    }
    return false;
}

/*
 * Function:  checkLegalInDirection 
 * --------------------
 * Checks whether (row, col) is a legal position for a tile of colour by “looking” in the direction 
 * specified by deltaRow and deltaCol.
 */
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol) 
{  
    //Check if Tile is occupied
    if ((int)board[row][col] != 85)
        return false;
    //Check if Additions in Values are in Bounds
    if (positionInBounds(n,row+deltaRow,col+deltaCol) == false)
        return false;
    //Check if Value is in Bound
    if (positionInBounds(n, row, col) == false) 
        return false;
    //Check if it is White
    if ((int)colour == 87) 
    {
        if ((int)board[row+deltaRow][col+deltaCol]==66) 
        {
            if (nextTileCheck(board,n,row+deltaRow,col+deltaCol,deltaRow,deltaCol,colour) == true)
                return true;
        }
    }
    //Check if it is Black
    if ((int)colour == 66) 
    {
        if ((int)board[row+deltaRow][col+deltaCol]==87) 
        {
            if (nextTileCheck(board,n,row+deltaRow,col+deltaCol,deltaRow,deltaCol,colour) == true)
                return true;
        }
    }
    return false;             
}

/*
 * Function:  directionalChecks 
 * --------------------
 * Assigns the Delta for Row and Column respectively for the Counter
 */
bool directionalChecks (char board [][26],int n, int row, int col, char colour, int delta)
{
    bool legalCheck = false;
    int deltaArray[2][8]={{1,1,0,-1,-1,-1,0,1},{0,1,1,1,0,-1,-1,-1}};
    int i = deltaArray[0][delta-1];
    int j = deltaArray[1][delta-1];

    if(checkLegalInDirection(board,n,row,col,colour,i,j)==true) //Checks for Every Possible 1 Step Increment Move
        legalCheck=true;  

    return legalCheck;
}

/*
 * Function:  whoTurn 
 * --------------------
 * Determine which Player's Turn based on overall Turn Count
 */
int whoTurn(char whoTurn, int turn) 
{
    int turnTurn = turn%2;
    if (whoTurn==66&&turnTurn==1)
        return 1; //Returns that it is CPU Move
    if (whoTurn==66&&turnTurn==0)
        return 0; //Returns that it is Player Move
    if (whoTurn==87&&turnTurn==0)
        return 1; //Returns CPU Move
    if (whoTurn==87&&turnTurn==1)
        return 0; //Returns that it is Player Move
    else 
        return 69;
}

/*
 * Function:  findScore 
 * --------------------
 * Checks to see how many B/W are on a BOARD
 */
int findScore(char board[][26], int n, int row, int col, char colour)
{
    int finalScore=0, insideCounter=0, edgesCounter=0, cornerCounter=0, opponentAvailableMoves=0;
    
    //Inside Board, Excluding Edges and Corners
    for (int i=1;i<n-1;i++)
    {
        for (int j=1;j<n-1;j++)
        {
            if (board[i][j]==colour)
                insideCounter++;
        }
    }
   
    //Row Edges
    for (int j=1;j<n-1;j++)
    {
        if (board[0][j]==colour)
            edgesCounter++;
        if (board[n-1][j]==colour)
            edgesCounter++;
        if (board[j][0]==colour)
            edgesCounter++;
        if (board[j][n-1]==colour)
            edgesCounter++;
    }

    //Corner Cases
    if (board[0][0]==colour)
            cornerCounter++;
    if (board[0][n-1]==colour)
            cornerCounter++;
    if (board[n-1][0]==colour)
            cornerCounter++;
    if (board[n-1][n-1]==colour)
            cornerCounter++;

    char copyBoard[26][26]; //Creates Copy of Board
    for (int ii=0;ii<n;ii++)
    {
        for (int jj=0;jj<n;jj++)
        {
            copyBoard[ii][jj]=board[ii][jj];
        }
    }

    char oppositeColour;
    if (colour==66)
        oppositeColour=87;
    else
        oppositeColour=66;

    //Makes Moves on Copied Board
    moveTiles(copyBoard, n, row, col, colour);
    copyBoard[row][col]=colour;

    //Finds how many Possible Opponent Moves are on Copied Board
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            int legalCheck=0, counter=1;
            do
            { 
                if (directionalChecks(board,n,i,j,oppositeColour,counter)==true)
                {
                    legalCheck=1;
                    opponentAvailableMoves++;
                }
                counter++;
            }while (legalCheck==0 && counter<9);
        }
    }

    //Composite Scoring
    finalScore= 1*insideCounter+3*edgesCounter+5*cornerCounter-0.5*opponentAvailableMoves;  
    return finalScore;
}

/*
 * Function:  makeMove 
 * --------------------
 * The player "turn" makes a move at (row, col).
 * Note: This function MUST NOT change board, only row and col can be changed to reflect the move.
 */
int makeMove(const char board[26][26], int n, char turn, int *row, int *col) 
{
    int highestScore=0;

    for (int i=0;i<n;i++) //Starts Going Through Every Tile
    {
        for (int j=0;j<n;j++)
        {
            int legalCheck=0, counter=1, temporaryScore=0;
            do //Checks for Legality
            { 
                if (directionalChecks(*board,n,i,j,turn,counter)==true)
                    legalCheck=1;
                counter++;
            } while (legalCheck==0 && counter<9);

            if (legalCheck==1)
            {   
                //Make Copy of Board
                char copyBoard[26][26];
                for (int ii=0;ii<n;ii++)
                {
                    for (int jj=0;jj<n;jj++)
                    {
                        copyBoard[ii][jj]=board[ii][jj];
                    }
                }

                //Makes Moves on Copied Board
                moveTiles(copyBoard, n, i, j, turn);
                copyBoard[i][j]=turn;

                temporaryScore=findScore(copyBoard,n, i, j, turn); //Finds Composite Score

                if (temporaryScore>highestScore) //If Composite Score if Higher than Any Previous Ones, it's new BEST MOVE
                {
                    highestScore=temporaryScore;
                    *row=i; //Sets to Pointers Back to Main
                    *col=j; //Sets to Pointers Back to Main
                }
            }
        }
    }   
}

/*
 * Function:  nextmoveChecks 
 * --------------------
 * Checks to see if the specified Move is Possible for a Colour
 */
int nextmoveChecks (char board[][26], int n, int whosTurn)
{
    int checksNextMove=0;

    for (int i=0;i<n;i++) //Checks Every Tile for Any Possible Move with Legality Check
    {
        for (int j=0;j<n;j++)
        {
            int counter=1;
            do
            { 
                if (directionalChecks(board,n,i,j,whosTurn,counter)==true)
                    checksNextMove=1;
                counter++;
            }while (counter<9);
        }
    }

    return checksNextMove;
}

#ifndef COMPETE_MODE 
int main(int argc, char **argv) 
{
    //Obtain Board Size
    int n;
    printf("Enter the board dimension: ");
    scanf("%d", &n);

    //Initialize Full Empty Board
    char board[26][26];
    for (int i=0;i<n;i++)
    {
        for (int j=0;j<n;j++)
        {
            board[i][j]=85;
        }
    }

    //Initialize Centre of Board
    board[(n/2)-1][(n/2)-1]=87;
    board[(n/2)-1][n/2]=66;
    board[n/2][(n/2)-1]=66;
    board[n/2][n/2]=87;

    //Ask for Computer Colour
    char computerColour, humanColour;
    printf("Computer plays (B/W) : ");
    scanf(" %c", &computerColour);
    
    //Gives Human Their Colour
    if (computerColour==66)
        humanColour=87;
    else
        humanColour=66;

    printBoard(board,n); //Initial Print Board
    
    //Computer/Human Inputs
    int gameNotOver=0, gameTurn=1;
    while (gameNotOver==0)
    {
        //Computer Move
        if (whoTurn(computerColour,gameTurn)==1)
        {
            int bestRow=97,bestColumn=97;
            makeMove(board, n, computerColour,&bestRow,&bestColumn); //Call makeMove Function to Find Row, Col for Computer
            
            //Make Moves
            moveTiles(board, n, bestRow, bestColumn, computerColour);
            board[bestRow][bestColumn]=computerColour; //Sets The Inputted Value as New Digit
            printf("Computer places %c at %c%c.\n", computerColour, bestRow+97, bestColumn+97);
            printBoard(board,n); 
        }
    
        //HumanMove
        if (whoTurn(computerColour,gameTurn)==0)
        {
            int row, col;
            findSmarterMove(board, n, humanColour, &row, &col);
            printf("Testing AI move (row, col): %c%c\n", row + 'a', col + 'a');

            //Checks for LEGALITY
            int legalMoveCheck=0, counterMove=1;
            do
            { 
                if (directionalChecks(board,n,row,col,humanColour,counterMove)==true)
                    legalMoveCheck=1;
                counterMove++;
            }while (legalMoveCheck==0 && counterMove<9);

            if (legalMoveCheck==1)
            {
                moveTiles(board, n, row, col, humanColour);
                board[row-'a'][col-'a']=humanColour; //Sets The Inputted Value as New Digit
                printBoard(board,n);
            }

            if (legalMoveCheck==0)
            {
                printf("Invalid move.\n");
                printf("%c player wins.", computerColour);
                gameNotOver=1;
            }
        }

        if (gameNotOver==0)
        {
            int temporaryTurn = gameTurn+1, computerChecks=0, humanChecks=0, sameChecks=0; //Declare Variables

            //Check if Computer Next Move is Possible
            if (whoTurn(computerColour,temporaryTurn)==1)
                computerChecks=nextmoveChecks(board,n,computerColour);

            //Check if Human Next Move is Possible
            if (whoTurn(computerColour,temporaryTurn)==0)
                humanChecks=nextmoveChecks(board,n,humanColour);

            //If Next Move not Available, check if Same Move is Possible
            if (whoTurn(computerColour,gameTurn)==1)
                sameChecks=nextmoveChecks(board,n,computerColour);

            if (whoTurn(computerColour,gameTurn)==0)
                sameChecks=nextmoveChecks(board,n,humanColour);

            //Checks to See if we should increase Turns
            if (computerChecks || humanChecks == 1)
                gameTurn=gameTurn+1;

            if (whoTurn(computerColour,temporaryTurn)==0)
                if (humanChecks==0 && sameChecks !=0)
                    printf("%c player has no valid move.\n", humanColour);
            if (whoTurn(computerColour,temporaryTurn)==1) 
                if (computerChecks==0 && sameChecks !=0)
                    printf("%c player has no valid move.\n", computerColour);

            if (computerChecks ==0 && humanChecks == 0 && sameChecks ==0)
                gameNotOver = 1;

            //If Game is Over, Find Winner
            if (gameNotOver==1)
            {
                int finalComputerScore = 0, finalHumanScore = 0;
                //Find Computer Score
                finalComputerScore=findScore(board,n,0,0,computerColour);

                //Find Human Score
                finalHumanScore=findScore(board,n,0,0,humanColour);

                //Final Outputs to see WHO WINS!
                if (finalHumanScore==finalComputerScore)
                    printf("Draw!\n");
                if (finalHumanScore>finalComputerScore)
                    printf("%c player wins.\n", humanColour);
                if (finalHumanScore<finalComputerScore)
                    printf("%c player wins.\n", computerColour);
            }
        }    
    }
    return 0;
}

#endif 
