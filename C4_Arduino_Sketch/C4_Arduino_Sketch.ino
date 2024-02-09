#include <SoftwareSerial.h>
SoftwareSerial mySerial(3,2);

//Constants
#define columnControl1A 2
#define columnControl1B 3
#define columnControl2A 4
#define columnControl2B 5
#define rowControl1A 6
#define rowControl1B 7
#define columnControl1ShutOff 8
#define button 9
#define columnControl2ShutOff 10
#define columnColour1 11
#define rowControl2 12
#define columnColour2 13
#define selectionDelay 1000

//Variables
int boardState[7][6] = {{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0},{0,0,0,0,0,0}};
int stackTops[7] = {0,0,0,0,0,0,0};
int columnSelect = 0;
int tick = 0;
int lastSelect = 0;
int playerTurn = 1;
bool gameOn = true;
bool inMove = false;
bool validSelect = true;
int lastSelectTop = 0;
int turn = 0;
bool endWait = false;

void setup(){
//Init Serial USB
Serial.begin(9600);
Serial.println(F("Initialize System"));

  pinMode(columnControl1A, OUTPUT);
  pinMode(columnControl1B, OUTPUT);
  pinMode(columnControl2A, OUTPUT);
  pinMode(columnControl2B, OUTPUT);
  pinMode(rowControl1A, OUTPUT);
  pinMode(rowControl1B, OUTPUT);
  pinMode(columnControl1ShutOff, OUTPUT);
  pinMode(button, INPUT);
  pinMode(columnControl2ShutOff, OUTPUT);
  pinMode(columnColour1, OUTPUT);
  pinMode(rowControl2, OUTPUT);
  pinMode(columnColour2, OUTPUT);
  
  digitalWrite(columnControl1ShutOff, HIGH);
  digitalWrite(columnControl2ShutOff, HIGH);
  digitalWrite(columnControl1A, LOW);
  digitalWrite(columnControl1B, LOW);
  digitalWrite(columnControl2A, LOW);
  digitalWrite(columnControl2B, LOW);
  digitalWrite(rowControl1A, LOW);
  digitalWrite(rowControl1B, LOW);
  digitalWrite(columnColour1, LOW);
  digitalWrite(rowControl2, LOW);
  digitalWrite(columnColour2, LOW);

}

void displayBoard(int boardState[7][6]){
  for (int row = 0; row < 6; row += 1){
    digitalWrite(columnControl1ShutOff, HIGH);
    digitalWrite(columnControl2ShutOff, HIGH);
    if (row < 4){
      digitalWrite(rowControl2, LOW);
      switch (row){
        case 0:
          digitalWrite(rowControl1A, LOW);
          digitalWrite(rowControl1B, LOW);
          break;
        case 1:
          digitalWrite(rowControl1A, LOW);
          digitalWrite(rowControl1B, HIGH);
          break;
        case 2:
          digitalWrite(rowControl1A, HIGH);
          digitalWrite(rowControl1B, LOW);
          break;
        case 3:
          digitalWrite(rowControl1A, HIGH);
          digitalWrite(rowControl1B, HIGH);
          break;
      }
    }
    else{
      digitalWrite(rowControl2, HIGH);
      switch (row){
        case 4:
          digitalWrite(rowControl1A, LOW);
          digitalWrite(rowControl1B, LOW);
          break;
        case 5:
          digitalWrite(rowControl1A, LOW);
          digitalWrite(rowControl1B, HIGH);
          break;
      }
    }
    for (int column = 0; column < 7; column += 1){
      switch (boardState[column][row]){
        case 0:
          digitalWrite(columnControl1ShutOff, HIGH);
          digitalWrite(columnControl2ShutOff, HIGH);
          break;
        case 1:
          digitalWrite(columnControl1ShutOff, HIGH);
          digitalWrite(columnControl2ShutOff, HIGH);
          // Set colour to RED
          digitalWrite(columnColour1, HIGH);
          digitalWrite(columnColour2, HIGH);

          if (column < 3){

            switch (column){
              case 0:
                digitalWrite(columnControl1A, LOW);
                digitalWrite(columnControl1B, LOW);
                break;
              case 1:
                digitalWrite(columnControl1A, LOW);
                digitalWrite(columnControl1B, HIGH);
                break;
              case 2:
                digitalWrite(columnControl1A, HIGH);
                digitalWrite(columnControl1B, LOW);
                break;
            }
            digitalWrite(columnControl1ShutOff, LOW);
          }
          else{

            switch (column){
              case 3:
                digitalWrite(columnControl2A, LOW);
                digitalWrite(columnControl2B, LOW);
                break;
              case 4:
                digitalWrite(columnControl2A, LOW);
                digitalWrite(columnControl2B, HIGH);
                break;
              case 5:
                digitalWrite(columnControl2A, HIGH);
                digitalWrite(columnControl2B, LOW);
                break;
              case 6:
                digitalWrite(columnControl2A, HIGH);
                digitalWrite(columnControl2B, HIGH);
                break;
            }
            digitalWrite(columnControl2ShutOff, LOW);
          }
          //delay(1);
          break;
        case 2:
          digitalWrite(columnControl1ShutOff, HIGH);
          digitalWrite(columnControl2ShutOff, HIGH);

          // Set colour to BLUE
          digitalWrite(columnColour1, LOW);
          digitalWrite(columnColour2, LOW);

          if (column < 4){

            switch (column){
              case 0:
                digitalWrite(columnControl1A, LOW);
                digitalWrite(columnControl1B, LOW);
                break;
              case 1:
                digitalWrite(columnControl1A, LOW);
                digitalWrite(columnControl1B, HIGH);
                break;
              case 2:
                digitalWrite(columnControl1A, HIGH);
                digitalWrite(columnControl1B, LOW);
                break;
              case 3:
                digitalWrite(columnControl1A, HIGH);
                digitalWrite(columnControl1B, HIGH);
                break;
            }
            digitalWrite(columnControl1ShutOff, LOW);
          }
          else{
            switch (column){
              case 4:
                digitalWrite(columnControl2A, LOW);
                digitalWrite(columnControl2B, LOW);
                break;
              case 5:
                digitalWrite(columnControl2A, LOW);
                digitalWrite(columnControl2B, HIGH);
                break;
              case 6:
                digitalWrite(columnControl2A, HIGH);
                digitalWrite(columnControl2B, LOW);
                break;
            }
            digitalWrite(columnControl2ShutOff, LOW);
          }
          //delay(1);
          break;
      }
    }
  }
}

void playMove(int column){
  inMove = true;
  int fall = 0;
  int fallSpeed = 100;
  int prev = 5;
  int cur = 5;
  while(cur > stackTops[column]){
    fall += 1;
    cur = 5 - (fall / fallSpeed);
    boardState[column][cur] = playerTurn;
    if (prev != cur){
      boardState[column][cur + 1] = 0;
      prev = cur;
    }
    displayBoard(boardState);
  }
  stackTops[column] = stackTops[column] + 1;
  inMove = false;
}

bool checkWin(int column, int row, int player){
  int inARow = 0;
  //vertical
  for (int i = 0; i < 6; i+=1){
    if (boardState[column][i] == player){
      inARow += 1;
      if (inARow > 3){
        return true;
      }
    }
    else{
      inARow = 0;
    }
  }

  //horizontal
  inARow = 0;
  for (int i = 0; i < 7; i+=1){
    if (boardState[i][row] == player){
      inARow += 1;
      if (inARow > 3){
        return true;
      }
    }
    else{
      inARow = 0;
    }
  }

  //positive slope
  inARow = 0;
  int horizontalShift = 0;
  int verticalShift = 0;
  if (column - row >= 0){
    horizontalShift = column - row;
  }
  else{
    verticalShift = row - column;
  }
  for (int i = 0; i < 6; i+=1){
    if (i + horizontalShift > 6 || i + verticalShift > 5){
      break;
    }
    if (boardState[i + horizontalShift][i + verticalShift] == player){
      inARow += 1;
      if (inARow > 3){
        return true;
      }
    }
    else{
      inARow = 0;
    }
  }

  //negative slope
  inARow = 0;
  horizontalShift = 0;
  verticalShift = 0;
  if (column - (5-row) >= 0){
    horizontalShift = column - (5-row);
  }
  else{
    verticalShift = (5-row) - column;
  }
  for (int i = 0; i < 6; i+=1){
    if (i + horizontalShift > 6 || 5 - (i + verticalShift) < 0){
      break;
    }
    if (boardState[i + horizontalShift][5 - (i + verticalShift)] == player){
      inARow += 1;
      if (inARow > 3){
        return true;
      }
    }
    else{
      inARow = 0;
    }
  }
  return false;
}

void resetGame(){
  for (int i = 0; i < 7; i += 1){
    stackTops[i] = 0;
    for (int j = 0; j < 6; j += 1){
      boardState[i][j] = 0;
    }
  }
  columnSelect = 0;
  tick = 0;
  lastSelect = 0;
  playerTurn = 1;
  gameOn = true;
  inMove = false;
  validSelect = true;
  lastSelectTop = 0;
  turn = 0;
  endWait = false;
}

int generateMove(){
  int move = random(7);
  while (stackTops[move] > 5){
    move = random(7);
  }
  return move;
}

void loop(){
  if (gameOn){
    if (turn >= 42){
      gameOn = false;
    }
    displayBoard(boardState);
    tick += 1;
    columnSelect = tick / selectionDelay;

    if (columnSelect > 6 || playerTurn == 1){
      tick = 0;
    }
    else{
      if(stackTops[columnSelect] < 6){
        boardState[lastSelect][5] = lastSelectTop;
        boardState[columnSelect][5] = 2;
        lastSelect = columnSelect;
        validSelect = true;
        lastSelectTop = 0;
      }
      else{
        tick += selectionDelay;
        validSelect = false;
      }
    }
    if(! inMove && validSelect){
      if (playerTurn == 1){
        int columnSelction = generateMove();
        playMove(columnSelction);
        if(checkWin(columnSelction, stackTops[columnSelect] - 1, playerTurn)){
          gameOn = false;
          tick = 0;
        }
        else{
          playerTurn = 2;
          turn += 1;
        }
      }
      else{
        if (digitalRead(button) == HIGH){
          playMove(columnSelect);
          if(checkWin(columnSelect, stackTops[columnSelect] - 1, playerTurn)){
            gameOn = false;
            tick = 0;
          }
          else{
            lastSelectTop = boardState[0][5];
            lastSelect = 0;
            playerTurn = 1;
            turn += 1;
          }
        }
      }
    }
  }
  else{
    tick += 1;
    int winScreen[7][6] = {{0,0,0,0,0,0},{0,0,playerTurn,0,0,0},{0,playerTurn,0,0,playerTurn,0},{0,playerTurn,0,0,0,0},{0,playerTurn,0,0,playerTurn,0},{0,0,playerTurn,0,0,0},{0,0,0,0,0,0}};
    if(turn < 42){
      int endScreenState = tick / selectionDelay;
      switch (endScreenState){
        case 0:
          displayBoard(winScreen);
          break;
        case 1:
          displayBoard(boardState);
          break;
        default:
          tick = 0;
          endWait = true;
          break;
      }
    }
    else{
      displayBoard(boardState);
    }
    if (digitalRead(button) == HIGH && endWait){
      resetGame();
    }
  }
}

