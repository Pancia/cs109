#include <iostream>
#include <locale>

#include "scanner.h"
#include "debug.h"

scanner::scanner() {
    seen_eof = false;
    advance();
}

void scanner::advance() {
    if (not seen_eof) {
        std::cin.get (lookahead);
        if (std::cin.eof()) seen_eof = true;
    }
}

token_t scanner::scan() {
    token_t result;
    while (not seen_eof and isspace (lookahead)) advance();
    if (seen_eof) {
        result.symbol = SCANEOF;
    } else if (lookahead == '_' or isdigit (lookahead)) {
        result.symbol = NUMBER;
        do {
            result.lexinfo += lookahead;
            advance();
        } while (not seen_eof and isdigit (lookahead));
    } else {
        result.symbol = OPERATOR;
        result.lexinfo += lookahead;
        advance();
    }
    DEBUGF ('S', result);
    return result;
}

std::ostream& operator<<(std::ostream& out,
        const terminal_symbol& symbol) {
    switch (symbol) {
        case NUMBER  : out << "NUMBER"  ; break;
        case OPERATOR: out << "OPERATOR"; break;
        case SCANEOF : out << "SCANEOF" ; break;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, const token_t& token) {
    out << token.symbol << ": \"" << token.lexinfo << "\"";
    return out;
}
