// author: willnationsdev (Will Nations)
// MIT License

#include "ink.h"

using namespace ink;

Error Ink::parse_text(const string_t &p_text) {
    return OK;
}

Error Ink::parse_file(const string_t &p_path) {
    return OK;
}

// generation
string_t Ink::generate_text(Error *r_err) const {
    return string_t("");
}

// status operations
void Ink::set_file(const string_t &p_path) {

}

const string_t &Ink::get_file() const {
    return string_t("");
}

// load operations
Error Ink::add_data(DataType p_type, const string_t &p_data, size_t line) {
    return OK;
}

// story operations
Error Ink::start() {
    return OK;
}

Error Ink::quit() {
    return OK;
}

Error Ink::pause() {
    return OK;
}

Error Ink::resume() {
    return OK;
}

Error Ink::reset() {
    return OK;
}

// navigation
Error Ink::next() {
    return OK;
}

Error Ink::previous() {
    return OK;
}

bool Ink::is_at_choice() const {
    return false;
}

Error Ink::choose(uint8_t p_option) {
    return OK;
}

const string_t &Ink::get_text() const {
    return string_t("");
}