#include "math_tool/state_machine.h"

math_tool::state_machine::LetterType
math_tool::state_machine::JudgmentLetterType(char letter)
{
    if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'z')) {
        return CHARACTER;
    }
    if (letter >= '0' && letter <= '9') {
        return DIGIT;
    }
    if ((letter == ' ') || (letter == '\n') || (letter == '\t')) {
        return DELIM;
    }
    if ((letter == '+') || (letter == '-') || (letter == '*') ||
        (letter == '/')) {
        return OPERATOR;
    }
    return OTHER;
}

// Implemetn State
math_tool::state_machine::State::State()
{
    // Only need to initialize head node
    // convert table should be determined util all state
    // be create.
    possible_token_ = new ReceivedLetter;
    possible_token_->next_letter = nullptr;
    possible_token_->letter = 0;
    possible_token_->letter_length = 0;
}

void math_tool::state_machine::State::Clear()
{
    // Clear all node except head node
    ReceivedLetter* current_node = possible_token_->next_letter;
    ReceivedLetter* next_node = current_node->next_letter;
    while (next_node != nullptr) {
        delete current_node;
        current_node = next_node;
        next_node = next_node->next_letter;
    }
    possible_token_->letter = 0;
    possible_token_->letter_length = 0;
}

void math_tool::state_machine::State::AddLetter(char letter)
{
    ReceivedLetter* new_letter = new ReceivedLetter;
    new_letter->letter = letter;
    new_letter->next_letter = possible_token_->next_letter;
    possible_token_->next_letter = new_letter;
}

void math_tool::state_machine::State::SwitchState(
    const math_tool::state_machine::State& previous_state)
{
    // Deep copy possible token of previous state to this state's possible
    // token, and set previous state's possible token to zero
    possible_token_->letter_length =
        previous_state.possible_token_->letter_length;
    possible_token_->next_letter = previous_state.possible_token_->next_letter;
    previous_state.possible_token_->letter_length = 0;
    previous_state.possible_token_->next_letter = nullptr;
}

char math_tool::state_machine::State::RemoveALetter()
{
    ReceivedLetter* tmp_pointer = possible_token_->next_letter;
    char letter = tmp_pointer->letter;
    possible_token_->next_letter = tmp_pointer->next_letter;
    delete tmp_pointer;

    return letter;
}

// Implement IntermediateState & TerminateState

void math_tool::state_machine::IntermediateState::Enter(
    const state_machine::State& previous_state)
{
    // For intermediate states, enter are merely equal to switch state
    SwitchState(previous_state);
}

math_tool::state_machine::State&
math_tool::state_machine::IntermediateState::HasLetter(char letter)
{
    LetterType letter_type;
    letter_type = JudgmentLetterType(letter);
    auto new_state = ConvertTable_.find(letter_type);
    AddLetter(letter);
    return new_state->second;
}

void math_tool::state_machine::TerminateState::Enter(
    const math_tool::state_machine::State& previous_state)
{
    // For terminate states, enter are equal to regconize token
    RecognizeToken(previous_state);
}

math_tool::state_machine::State&
math_tool::state_machine::TerminateState::HasLetter(char letter)
{
    return *this;
}

// Implement other sub class
