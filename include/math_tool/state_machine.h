#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <map>

namespace math_tool {
namespace state_machine {

enum LetterType { DELIM, DIGIT, CHARACTER, OPERATOR, OTHER };

LetterType JudgmentLetterType(char);

struct Token {
    void* value;
    enum type { NUMBER, OPERATOR };
};

class State {
    // Each class is a single state
public:
    struct ReceivedLetter {
        ReceivedLetter* next_letter;
        char letter;
        int letter_length;
    };

    State();
    virtual ~State();
    // When enter a state, produce possibel token only by current
    // state. Previous state should back to initialized status.
    virtual void Enter(const State& previous_state) = 0;
    // When has an event, detect wheter transition or discard.
    virtual State& HasLetter(char letter) = 0;

    // Clear the linked lists, and make the instance back to the initialized
    // state.
    void Clear();

    // Remove the last letter and return that letter
    char RemoveALetter();

    // Switch activate state to this state from previous state
    void SwitchState(const State& previous_state);

    void AddLetter(char letter);

protected:
    std::map<LetterType, State&> ConvertTable_;

private:
    ReceivedLetter* possible_token_;
};

class IntermediateState : public State {
    void Enter(const State& previous_state);
    State& HasLetter(char);
};

class TerminateState : public State {
    void Enter(const State& previous_state);
    virtual Token RecognizeToken(const State& last_state) = 0;
    State& HasLetter(char);
};

class StartState : public IntermediateState {};

// Recieved delime letter wait for next input
class DelimLetter : public IntermediateState {};

// Recognized delim as token but discard it
class DelimToken : public TerminateState {};

// Only recieved digits, thus may be a integer
class IntegerLetter : public IntermediateState {};

// Recognized interger as token
class IntegerToken : public State {};

// Recieved digits and decimal point, thus may be a float without E
class IntegerAndPointLetter : public IntermediateState {};

// Recieved digits, decimal point and other digit, thus may be a float without E
class PureFloatLetter : public IntermediateState {};

// Recognized pure float as token
class PureFloatToken : public TerminateState {};

// Recieved float or integer and 'E' or 'e', thus may be a float with 'E' or 'e'
// to indicate exponential
class FloatAndELetter : public IntermediateState {};

// Recieved float or integer, 'E' or 'e' and a sign, thus may be a float with
// 'E' or 'e' to indicate exponential
class FloatAndEAndSignLetter : public IntermediateState {};

// Recieved float interger, 'E' or 'e', sign or not and digit, thus may be a
// full float.
class FullFloatLetter : public IntermediateState {};

// Recognized full float as token
class FullFloatToken : public TerminateState {};

// Recieved characters, thus may be a basic elementary function
class CharacterLetter : public IntermediateState {};

// Recognized basic elementary function
class BasicElementaryToken : public TerminateState {};

// Recognized '+', '-', '*' or '/' as a operator token
class OperatorToken : public TerminateState {};

class StateMachine {
    // Finite State Machine
    //
};

}  // namespace state_machine
}  // namespace math_tool

#endif