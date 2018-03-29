#include "stdlib.h"
#include "stdio.h"


enum Action {
  Action_Start;
  Action_Forward;
  Action_RightTurn;
  Action_LeftTurn;
  Action_LineBreak;
  Action_END = 255;
}

typedef struct {
  enum Action action;
  float distance;
} __Movement, *Movement;

int init_Movements(Movement * movements){

  
}

int main(){
  Movement movements[] = calloc(sizeof(__Movement), MOVEMENT_LENGTH);
  realloc(movements, Length + 1);
  
  
  
  return 0;
}
