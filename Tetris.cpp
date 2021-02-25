// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there. 

//TO-DOs: Implement next piece

#include <iostream>   //Standard Input / Output Stream Library
#include <thread>     //Gives us access to the this_thread class 
#include <Windows.h>  //Gives us access to the screen object
#include <random>     //Gives us access to the rand() function
#include <vector>     //Gives us access to the vector container 

using namespace std;


  //////////////////////////////////////
 //  G L O B A L  V A R I A B L E S  //
//////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
string tetromino[7];             //Array of wide strings for holding Tetromino pieces                   //
int nFieldWidth = 12;            //Width of the game field  (columns)                                   //
int nFieldHeight = 18;           //Height of the game field  (rows)                                     //
unsigned char *pField = nullptr; //Pointer to an unsigned char representing the playing field           //
                                                                                                        //
int nScreenWidth = 120;  //Console Screen Size X (columns)                                              //
int nScreenHeight = 30;  //Console Screen Size Y (rows)                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////
 //     S T A R T I N G  P O I N T       //
//////////////////////////////////////////
//
//
//A single dimensional array can be accessed like a multi-array if we arrange it as
//as an [n x m] array. Our tetromino arrays will be of length 16 [0, 1, 2,...., 15] 
//so we can arrange it as [4 x 4] multi-array like such:
//
//         Standard Rotation 
//              px = 0    1    2    3
//
//      py = 0       0    1    2    3
//
//           1       4    5    6    7        
//
//           2       8    9    10   11        
//
//           3       12   13   14   15                
//
//If we went to access a specific index we can use the formula index = py * 4 + px.
//
//For example in the standard rotation (r = 4), where px = 1 and py = 1
//the function will return py * 4 + px. So the index at (1, 1) -> 1 * 4 + 1 = 5. 
//
//What if we need to rotate the array containing our Tetris block aka rotate the Tetromino? 
//We rotate the Tetromino array representation and find an indexing formula for that representation.
//
//          90 Degree Rotation
//              px = 0    1    2    3
//
//      py = 0      12    8    4    0
//
//           1      13    9    5    1
//
//           2      14    10   6    2
//
//           3      15    11   7    3
//
//The formula for indexing is now i = 12 + py - (px * 4)
//
//Formula for 180 degrees: 15 - (py * 4) - px;
//
//Formula for 270 degrees: 3 - py + (px * 4);
//
//This becomes our base point for how we draw and manipulate our Tetromino blocks. We also use the
//general formula position.y * columnWidth + position.x as a way to index into our pField array 
//(containing the playing field) and our screen array (the entirety of the console screen). 



  //////////////////////////////////////
 //  H E L P E R  F U N C T I O N S  //
//////////////////////////////////////
//
//
//Function 'rotate' to specify rotation of Tetromino pieces. It takes three params:
//x coordinate, y coordinate, and a value specifying its rotation. 
//
//Returns (as an int) the index into the Tetromino array dependent on it's rotation
//and the x and y coordinates. It uses the remainder function '%' to determine
//the rotation value from parameter 'r'.
//
int rotate(int px, int py, int r)          
{
    switch (r % 4)
    {
    case 0: return py * 4 + px;           // 0 degrees
    case 1: return 12 + py - (px * 4);   // 90 degrees
    case 2: return 15 - (py * 4) - px;  // 180 degrees
    case 3: return 3 - py + (px * 4);  //  270 degrees
    }
    return 0;
}
//
//
//Function 'doesFit' checks whether a specific Tetromino can fit in a
//specific position. It takes four params:
//nTetromino - an int representing the Tetromino block
//nRotation - an int representing the rotation
//nPosx and nPosy - two ints representing its position within the pField
//
//Returns TRUE if Tetromino piece array can fit at the desired coordinates
//with the desired rotation. Returns FALSE otherwise.
bool doesFit(int nTetromino, int nRotation, int nPosX, int nPosY) {      
    
    //iterate through our tetromino array
    for (int px = 0; px < 4; px++)
        for (int py = 0; py < 4; py++) {
            
            //Get index into tetromino piece array
            int pi = rotate(px, py, nRotation);
            
            //get index into pField array 
            int fi = (nPosY + py) * nFieldWidth + (nPosX + px); 

            //Check to make sure we're not out of bounds on the X-axis
            if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
            {
            //Check to make sure we're not out of bounds on the Y-axis
                if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
                {
                    //If any part of the Tetromino array where we render a piece intersects
                    //with a portion of the pField that isn't empty, return FALSE. 
                    if (tetromino[nTetromino][pi] == 'X' && pField[fi] != 0)
                        return false;
                }
            }

        }
    //Always return TRUE otherwise 
    return true; 
}

int main()
{


  ////////////////////////////////////
 //    C R E A T E  A S S E T S    //
////////////////////////////////////
//
//
//Here we construct our '4 x 4' Tetromino arrays. We arrange the append 
//statements as a series to represent graphically what the Tetromino should
//display as in the final product. 
//We use the char '.' to denote a blank space in the Tetromino array and the
//char 'X' to denote where we should render part of the Tetromino block. 
//
//We will use the rotation function to transform the placement of the
//'.' character and'X' character into a rotated block instead of creating
//four assets for each block. 
//
//  S T R A I G H T  T E T R O M I N O
    tetromino[0].append("..X.");                  
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");
    tetromino[0].append("..X.");    

//  R I G H T  Z I G  T E T R O M I N O
    tetromino[1].append("..X.");                   
    tetromino[1].append(".XX.");
    tetromino[1].append(".X..");
    tetromino[1].append("....");

//  L E F T  Z I G  T E T R O M I N O
    tetromino[2].append(".X..");                   
    tetromino[2].append(".XX.");
    tetromino[2].append("..X.");
    tetromino[2].append("....");

//  B L O C K  T E T R O M I N O 
    tetromino[3].append("....");                   
    tetromino[3].append(".XX.");
    tetromino[3].append(".XX.");
    tetromino[3].append("....");

//  B I S E C T  T E T R O M I N O
    tetromino[4].append("..X.");                   
    tetromino[4].append("..XX");
    tetromino[4].append("..X.");
    tetromino[4].append("....");

//  L E F T  H O O K  T E T R O M I N O
    tetromino[5].append("....");                  
    tetromino[5].append(".XX.");
    tetromino[5].append("..X.");
    tetromino[5].append("..X.");

//  R I G H T  H O O K  T E T R O M I N O
    tetromino[6].append("....");                
    tetromino[6].append("..XX");
    tetromino[6].append("..X.");
    tetromino[6].append("..X.");
//
//
//
//                                              After the append statements we have an array 'tetromino' of 7 elements and each element has it's own array of 16 elements.
//
//                  tetromino                                             tetromino                                           tetromino                        ,......,                      tetromino 
//                     [0]                                                   [1]                                                 [2]                           ,......,                         [6]
//  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15        0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15      0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15    ,......,     0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
// [., ., X, ., ., ., X, ., ., ., X, ., ., ., X, .]      [., ., X, ., ., X, X, ., X, ., , ., ., ., ., .]    [., X, ., ., ., ., X, X, ., ., ., X, ., ., ., .]   ,......,    [., ., ., ., ., ., X, X, ., ., X, ., ., ., X, .]
//
//
//
//
//

//Create an array for the playing field and assign pField to point to it 
    pField = new unsigned char[nFieldWidth * nFieldHeight];         
    //Use a nested for loop to iterate through the X and Y axis
    for (int x = 0; x < nFieldWidth; x++) {                                                                    
        for (int y = 0; y < nFieldHeight; y++) {                                                               
            //Zero out game field and set value of boundaries of the field to 9 for rendering later
            //We see the general formula for indexing through a multi-array: i = py * columnWidth + px.
            pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;   
                                                                                                               
                                                                                                               
        }
    }
//Create a pointer to an array for the screen
    char* screen = new char[nScreenWidth * nScreenHeight];                                                       
    //Fill 'screen' full of Blank Space
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) {                                                             
        screen[i] = ' ';
    }              
    //Create a handle to the Console Buffer and set it as the Active Buffer.
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);   
    SetConsoleActiveScreenBuffer(hConsole);                                                                              
    DWORD dwBytesWritten = 0; 



//////////////////////////////////////////////////////////////////////
//                    G A M E  S T A T E                           //
////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    bool bGameOver = false;                     //Variable tracking Game Status                                 //    
    int nCurrentPiece = rand() % 7;             //Variable tracking current piece in play                       //
    int nNextPiece = rand() % 7;                //Variable to track next piece to be played                     //
    int nCurrentRotation{ 0 };                  //Variable tracking current rotation of piece in play           //
    int nCurrentX{ nFieldWidth / 3 };           //Variable of starting X position (middle of game field)        //
    int nCurrentY{ 0 };                         //Variable of starting Y position (top of game field)           //
                                                                                                                //
    bool bKey[4];                               //Array of Bool Variables for reading inputs                    //
    bool bRotateHold = false;                   //Variable to check if rotate button is held down               //
                                                                                                                //
    int nSpeed{ 20 };                                                                                           //
    int nSpeedCounter{ 0 };                                                                                     //
    bool bForceDown = false;                    //Variable to check forcing blocks down                         //
    int nPieceCount{ 0 };                       //Variable to track  # of pieces for difficulty                 //
    int nScore{ 0 };                            //Variable to track Score                                       //
                                                                                                                //
    vector<int> vLines;                         //Variable to store lines                                       //
    int nLines{ 0 };                            //Variable to store number of lines made                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////////////////////////////
        //                              G A M E  L O O P                               //
        ////////////////////////////////////////////////////////////////////////////////
        //
        //
        //Begin While Loop
    while (!bGameOver) {
        //
        //
        //
        //
        /////////////////////////////////////////////////////////////////////////////////
        //                                G A M E  T I M I N G                         //
        /////////////////////////////////////////////////////////////////////////////////
        //
        this_thread::sleep_for(50ms);           //Game Tick is 50ms
        nSpeedCounter++;                        //Every Game Tick add 1 to our speed counter
        bForceDown = (nSpeedCounter == nSpeed); //When speed counter = speed, set Force Block down to TRUE
                                                //occurs once every second | 50ms * 20 = 1000ms. 
        //
        /////////////////////////////////////////////////////////////////////////////////
        //                                G A M E  I N P U T                           //
        /////////////////////////////////////////////////////////////////////////////////
        //
        //When GetAsyncKeyState of (right, left, down, or z) has it's highest order bit set
        //(the key is down) set the corresponding index in the bKey array to true.
        //If the highest order bit isn't set it sets the index to false
        for (int k = 0; k < 4; k++)                               // R  L   D  Z
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;


        /////////////////////////////////////////////////////////////////////////////////
        //                                G A M E  L O G I C                           //
        /////////////////////////////////////////////////////////////////////////////////
        //
        //
        //                         P L A Y E R  M O V E M E N T
        //Uses the conditional operator '?:' and helper function 'doesFit' to change the position or rotation of the tetromino block
        //
        //
        //
        nCurrentX += (bKey[0] && doesFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;          //Moves Tetromino Block to the right    (add 1 to it's current x position) if it fits                                                                        
        nCurrentX -= (bKey[1] && doesFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;          //Moves Tetromino Block to the left     (subtract 1 from it's current x position) if it fits
        nCurrentY += (bKey[2] && doesFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;          //Moves Tetromino Block down            (add 1 to it's current x position) if it fits
        if (bKey[3])
        {

            nCurrentRotation += (!bRotateHold && doesFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0; //Rotates Tetromino if it fits and button isn't held down
            bRotateHold = true;                                                                                               //Sets hold condition to true
        }
        else {
            bRotateHold = false;                                                                                              //Sets hold condition to false
        }
        //
        //
        //
        //                      F A L L I N G  B L O C K  L O G I C
        //      Checks condidtion 'bForceDown' to see if 1 second has passed. If 1 second has passed, we start our falling block logic
        //
        if (bForceDown)
        {
            //Check to see if tetromino block can move down
            if (doesFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
                //If true, move block down (falling block)
                nCurrentY++;

            //If not possible then
            else
            {
                //Lock the current piece to the field
                //index through the tetromino array
                for (int px = 0; px < 4; px++)
                    for (int py = 0; py < 4; py++)
                        //Check for 'X' characters in our tetromino array so we know where to draw our piece
                        if (tetromino[nCurrentPiece][rotate(px, py, nCurrentRotation)] == 'X')
                            //draw the current piece to the field, add 1 so we index into the 'render array' correctly
                            pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

                //increment piece counter
                //every 10 pieces, lower threshold for
                //check to set bForceDown is TRUE (speedCounter = nSpeed); 
                nPieceCount++;
                if (nPieceCount % 10 == 0)
                    if (nSpeed >= 10) nSpeed--;

                //Check have we made any lines
                for (int py = 0; py < 4; py++)
                    if (nCurrentY + py < nFieldHeight - 1)
                    {
                        //set bLine to true and iterate through x-axis
                        bool bLine = true;
                        for (int px = 1; px < nFieldWidth - 1; px++)
                            //if index isn't empty (!= 0) at any point, bLine remains true, so we have a line
                            //if any index is empty, we do not have a line and bLine is false
                            bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;
                        //When we have a line, iterate through x-axis
                        if (bLine)
                        {
                            //set x indices to '=' char
                            for (int px = 1; px < nFieldWidth - 1; px++)
                                pField[(nCurrentY + py) * nFieldWidth + px] = 8;
                            //add y index to our vLines container
                            vLines.push_back(nCurrentY + py);
                            nLines++; 
                        }
                    }
                //increment score by 25 for every placed piece. 
                //if vLines container isn't empty and nLines > 4 Adds 50 * nLines * 2 ^ vLines size 
                //otherwise Adds 100 * 2 ^ vLines size
                //reward players for lasting longer and/or risky behavior 
                nScore += 25;
                if (!vLines.empty()) nScore += (nLines > 4) ? (nLines << vLines.size()) * 50 : (1 << vLines.size()) * 100;


                //Choose next piece
                nCurrentX = nFieldWidth / 3;
                nCurrentY = 0;
                nCurrentRotation = 0;
                nCurrentPiece = nNextPiece;
                nNextPiece = rand() % 7; 

                //if piece does not fit at the top of screen, GAME OVER
                bGameOver = !doesFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
            }
            nSpeedCounter = 0;          //Reset speed counter
        }

        //                              R  E  N  D  E  R    O  U  T  P  U  T
        //                            
        //
        //                                            
        //Draw Field 
        for (int x = 0; x < nFieldWidth; x++)
            for (int y = 0; y < nFieldHeight; y++)
                //render field to screen, pfield value is fed through a 'render array' to vary the representation of elements
                //'A' for tetromino 1, 'B' for tetromino 2, etc, '#' for boundaries, and '=' for line erasure
                screen[(y + 2) * nScreenWidth + (x + 2)] = " ABCDEFG=#"[pField[y * nFieldWidth + x]];

        //Draw Current Piece
        for (int px = 0; px < 4; px++)
            for (int py = 0; py < 4; py++)
                //Check if our Tetromino schema has an 'X' at these coordinates, if so
                //we write the to the screen (we add 65 to maintain the rendering scheme used for the initial render above)
                //0 + 65 = A, 1 + 65 = B, etc; 
                if (tetromino[nCurrentPiece][rotate(px, py, nCurrentRotation)] == 'X')
                    screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;

        //Draw SCORE, Lines
        //
        snprintf(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, "SCORE: %8d", nScore);
        snprintf(&screen[4 * nScreenWidth + nFieldWidth + 6], 16, "LINES: %8d", nLines);

        //Implement visual for next piece

        //Check if we have lines
        if (!vLines.empty())
        {
            //Display Frame for Line erasure
            WriteConsoleOutputCharacterA(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
            this_thread::sleep_for(300ms);  //Delay a bit
            
            //Loops backwards through vLines (y-indecies of lines)
            //copys the values in the line above the line to be cleared, into the line that was cleared
            //Zeroes out row;
            //Clears vLines container to start anew
            for (auto& v : vLines)
                for (int px = 1; px < nFieldWidth - 1; px++)
                {
                    for (int py = v; py > 0; py--)
                        pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
                    pField[px] = 0;
                }
            vLines.clear();
        }

        //Displays Frame
        WriteConsoleOutputCharacterA(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
    }

    //Game over
    CloseHandle(hConsole);
    cout << "Game Over!! Score: " << nScore << endl; 
    system("pause");

  return 0;


}
