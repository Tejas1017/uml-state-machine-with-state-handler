

#include <Arduino.h>

// Define enums and structs
//typedef enum {
//  idle,
//  incPress,
//  decPress,
//  statePress
//} state_t;

typedef enum {
  zero,
  incCount,
  decCount,
  resetCount,
  countVariable,
  ENTRY,
  EXIT
} event_t;

typedef enum {
  eventHandled,
  eventTransition,
  eventIgnored
} event_status_t;
typedef event_status_t (*state_t)();
struct p1 {
  int count;
  state_t activeState;
};

// Global variables
void eventDispatch();
event_status_t statMachine();
event_status_t idlerHandler();
event_status_t incHandler();
event_status_t decHandler();
event_status_t stateHandler();
event_t ee = zero;
#define idle &idlerHandler
#define incPress &incHandler
#define decPress &decHandler
#define statePress &stateHandler
p1 c = {.count = 0, .activeState = idlerHandler};

// Function prototypes


void setup() {
  Serial.begin(9600);
}

void loop() {
  char s = 0;
  if (Serial.available() > 0) {
    s = Serial.read();
  }

  // Handling serial input to trigger events
  switch (s) {
    case '1':
      ee = incCount;
      break;
    case '2':
      ee = decCount;
      break;
    case '3':
      ee = resetCount;
      break;
    case '4':
      ee =countVariable;
      break;
    default:
      // If no valid input received, do nothing
      return;
  }

  eventDispatch();
  //rial.println(c.count);
}

void eventDispatch() {
  event_status_t status;
  state_t curr = c.activeState;
  state_t target;
  status =c.activeState();
  if (status == eventTransition) {
    target = c.activeState;

    ee = EXIT;
    c.activeState = curr;
    c.activeState();
    ee = ENTRY;
    c.activeState = target;
    c.activeState();
  }
}

//event_status_t statMachine() {
//  switch (c.activeState) {
//    case idle:
//      return idlerHandler();
//    case incPress:
//      return incHandler();
//    case decPress:
//      return decHandler();
//    case statePress:
//      return stateHandler();
//  }
//}

event_status_t idlerHandler() {
  switch (ee) {
    case ENTRY:
      Serial.println("1) to increment count 2) decrement count 3) reset count");
      return eventHandled;
    case EXIT:
      return eventHandled;
    case incCount:
      Serial.println("Incrementing count");
      c.count++;
      c.activeState = incPress;
      return eventTransition;
    case decCount:
      // Decrementing count is not allowed in idle state
      Serial.println("Decrementing count is not allowed in idle state");
      return eventIgnored;
     
    case resetCount:
      // Resetting count in idle state
      Serial.println("Resetting count");
      c.count = 0;
      return eventHandled;
    case countVariable:
      Serial.println(c.count);
      c.activeState=statePress;
      return eventTransition;
    default:
      return eventIgnored;
  }
}
event_status_t incHandler() {
  switch (ee) {
    case ENTRY:
      Serial.println("You are in increment state");
      return eventHandled;
    case EXIT:
      return eventHandled;
    case incCount:
      // Incrementing count further
      Serial.println("Incrementing count");
      c.count++;
      return eventHandled;
    case decCount:
      Serial.println("Decrementing inc count");
      if (c.count > 0) {
        c.count--;
        c.activeState=decPress;
        return eventTransition;
      } else {
        // If count is already 0, decrement is not allowed
        Serial.println("Count is already 0, decrement not allowed");
        return eventIgnored;
      }
    case resetCount:
      // Resetting count and transitioning back to idle state
      Serial.println("Resetting inc count");
      c.count = 0;
      c.activeState = idle;
      return eventTransition;
    case countVariable:
      Serial.println(c.count);
      c.activeState=statePress;
      return eventTransition;
    default:
      return eventIgnored;
  }
}

event_status_t decHandler() {
  switch (ee) {
    case ENTRY:
      Serial.println("You are in decrement state");
      return eventHandled;
    case EXIT:
      return eventHandled;
    case incCount:
      Serial.println("Incrementing in dec state");
      c.count++;
      c.activeState=incPress;
      return eventTransition;
    case decCount:
      Serial.println("Decrementing count");
      if (c.count > 0) {
        c.count--;
        return eventHandled;
      } else {
       
        Serial.println("Count is already 0, decrement not allowed");
        return eventIgnored;
      }
    case resetCount:
      // Resetting count and transitioning back to idle state
      Serial.println("Resetting count");
      c.count = 0;
      c.activeState = idle;
      return eventTransition;
    case countVariable:
      Serial.println(c.count);
      c.activeState=statePress;
      return eventTransition;
    default:
      return eventIgnored;
  }
}
event_status_t stateHandler(){
    switch (ee) {
    case ENTRY:
    
    Serial.println(c.count);
      return eventHandled;
    case EXIT:
      return eventHandled;
    case incCount:
      Serial.println("Incrementing count");
      c.count++;
      c.activeState=incPress;
      return eventTransition;
    case decCount:
      Serial.println("Decrementing count");
      if (c.count > 0) {
        c.count--;
        c.activeState=decPress;
        return eventTransition;
      } else {
        
        Serial.println("Count is already 0, decrement not allowed");
        return eventIgnored;
      }
    case resetCount:
      // Resetting count and transitioning back to idle state
      Serial.println("Resetting count");
      c.count = 0;
      c.activeState = idle;
      return eventTransition;
   case countVariable:
    Serial.println(c.count);
    return eventHandled;
    default:
      return eventIgnored;
  }
}
