#include "main.h"


#ifndef __MEMORY_H
#define __MEMORY_H


// Type of action
#define ACTION_START      0
#define ACTION_STRAIGHT   1
#define ACTION_MINOR_L    2
#define ACTION_MINOR_R    3
#define ACTION_MAJOR_L    4
#define ACTION_MAJOR_R    5
#define ACTION_RAMP_POS   6
#define ACTION_RAMP_NEG   7
#define ACTION_LOST       8
#define ACTION_OTHER      9

/**
 * Sturct used to represent an Action on the track
 */
typedef struct Action{
  uint8_t actionType;  // Type of action ACTION_*******
  float distance; // How long the action took
  
  struct Action * previous;  // Previous action in the link list
  struct Action * next;  // Next action in the link list
} Action;

/**
 * Memory handle
 */
typedef struct {
  uint32_t length;  // Number of actions in the memory
  Action * action; // Pointer ot the latest action in the memory
} __Memory, *Memory;

/**
 * Initialize and Action pointer
 *
 * @param actionType - ID of the action type ie. Start, Stop, Straight, Turn etc.
 * @param distance - distance the action took
 * @return Action pointer
 */
Action * Action_init(uint8_t actionType, float distance);

/**
 * Initialize Memory handel
 *
 * @return memory handle
 */
Memory Memory_init(void);

/**
 * Push and action into the memory stack
 *
 * @param memory - Memory handle
 * @param action - Action pointer to be pushed into the memory
 */
void Memory_push(Memory memory, Action * action);

/**
 * Creates a new action and pushes it into the memory stack
 *
 * @param memory - Memory handle
 * @param actionType - ID of the action type
 * @param distance - distance the action took
 */
void Memory_pushAction(Memory memory, uint8_t actionType, float distance);

/**
 * Copies the lates action without removing it from the memory
 *
 * @param memory -Memory handle
 * @param action - Pointer to empty Action sturcture used as return
 * @return status - 0: success 1: error
 */
int Memory_showPreviousAction(Memory memory, Action * action);

/**
 * Removes lates action from the memory stack
 *
 * @param memory - memory handle
 */
void Memory_popAction(Memory memory);


/**
 * Gets the number of actions in the memory
 *
 * @param memory - memory handle
 * @return number of action s in memory
 */
uint32_t Memory_getLength(Memory memory);

/**
 * Removes and discards all Actions in memory
 *
 * @param memory - Memory handle
 */
void Memory_clear(Memory memory);



#endif
