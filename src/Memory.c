#include "Memory.h"
#include <stdlib.h>

Action * Action_init(uint8_t actionType, float distance){
  Action * action = malloc(sizeof(Action)); // Allocate space for action on the Heap
  
  // Set Action Values
  action->actionType = actionType;
  action->distance = distance;
  action->previous = NULL;
  action->next = NULL:
  
  return action;
}


Memory Memory_init(void){
  Memory mem = malloc(sizeof(__Memory)); // Initialize space for the memory handle on heap
  
  // Initialzie memory to 0
  mem->length = 0;
  action = NULL;
  
  return mem;
}

void Memory_push(Memory memory, Action * action){
  
  // Update current action to point to previous action
  action->previous = memory->action;
  action->next = NULL;
  
  // Add action to stack
  memory->action = action;
  memory->length += 1; // Increment length
}


void Memory_pushAction(Memory memory, uint8_t actionType, float distance){
  // Create action pointer
  Action * action = Action_init(actionType, distance);
  
  // Push this action to memory stack
  Memory_push(memory, action);
  
}

int Memory_showPreviousAction(Memory memory, Action * action){
  if(memory->action != NULL && memory->length != 0)
    memcpy(action, memory->action, sizeof(Action)); // Copy action to return action pointer
  else
    return 1; // No actions in memory
  return 0; // SUccess
}

void Memory_popAction(Memory memory){
  Action * action = memory->action; // Current action
  memory->action = action->previous; // Set current action to previous one
  memory->length -= 1; // Decrement memory length
  free(action); // Free popped action
}

uint32_t Memory_getLength(Memory memory){
  return memory->length; // Get lenght
}

void Memory_clear(Memory memory){
  while(memory->action != NULL){ // While we have actions in the memory
    Memory_popAction(memory); // Remove the top action
  }
}
