#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#endif

struct Token {
    void* value;
    enum type { NUMBER, OPERATOR };
};

class State {
    // Each class is a single state
public:
    virtual ~State();
    // When enter a state, record the pointer to
    // former state and maintain a counter.
    virtual void Enter() = 0;
    // When has an event, detect wheter transition or discard.
    virtual void HasEvent() = 0;
    // For the intermidiate state, exit means discard.
    // For the end state, exit means recognize a token.
    virtual Token Exit() = 0;
    // TODO: Detect how many object should be created for a single state.

private:
    char current_letter_;
};

class StateMachine {
    // Finite State Machine
    //
};