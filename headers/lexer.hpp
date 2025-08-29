#ifndef __LEXER_HPP
#define __LEXER_HPP

#include <bits/stdc++.h>
using namespace std;


enum TokenType{
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_EQUAL,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_QUOTE,
    TOKEN_EOF,

};

struct Token{
    enum TokenType TYPE;
    string VALUE;
};

string typeToStr(enum TokenType type) {
    switch (type) {
        case TOKEN_ID: return "TOKEN_ID";
        case TOKEN_INT: return "TOKEN_INT";
        case TOKEN_EQUAL: return "TOKEN_EQUAL";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_QUOTE: return "TOKEN_QUOTE";
        case TOKEN_EOF: return "TOKEN_EOF";
        default: return "UNKNOWN";
    }
}

class Lexer{
private:
    string source;
    int cursor;
    int size;
    char current;
    int line;
    int column;

public:
    Lexer(string sourceCode) {
        source = sourceCode;
        cursor = 0;
        size = sourceCode.length();
        current = sourceCode.at(cursor);
        line = 1;
        column = 1;
    }

    char advance(){
        if(cursor < size){
            char temp = current;
            cursor++;
            column++;
            current = (cursor < size) ? source[cursor] : '\0';
            return temp;
        }
        else {
            return '\0';
        }
        
    }

    void checkAndSkip(){
        while(current ==  ' ' || current == '\n' || current == '\t' || current == '\r'){

            if(current == '\n'){
                line++;
                column = 0; 
            }
            advance();

        }
    }

    vector<string> keywords = {"return","print"};

    Token* tokenizeID_KEYWORD(){
        stringstream buffer;
        buffer << advance();
        
        while(isalnum(current) || current == '_'){
            buffer << advance();
        }

        Token * newToken = new Token();

        newToken->TYPE = (find(keywords.begin(), keywords.end(), buffer.str()) != keywords.end()) ? TOKEN_KEYWORD : TOKEN_ID;
        newToken->VALUE = buffer.str();

        return newToken;
    }

    Token* tokenizeINT(){
        stringstream buffer;
        

        while(isdigit(current)){
            buffer << advance();
        }

        Token * newToken = new Token();

        newToken->TYPE = TOKEN_INT;
        newToken->VALUE = buffer.str();

        return newToken;
    }

    Token* tokenizeSPECIAL(enum TokenType TYPE){
        Token * newToken = new Token();
        newToken->TYPE = TYPE;
        newToken->VALUE = string(1, advance());
        
        return newToken;
    }

    Token* tokenizeSTRING(){
        
        stringstream buffer;
       

        while( current !=  '"' ){
           
            if(current == '\0'){
                cout << "[!] Lexer Error: Unclosed string literal" << endl;
                cout << "[!] Error at line: " << line << ", column: " << column << endl;
                exit(1);
            }
            buffer << advance();
        }

        Token* newToken = new Token();
        newToken->TYPE = TOKEN_STRING;
        newToken->VALUE = buffer.str();

        return newToken;
    }

    vector<Token*> tokenize(){

        vector<Token*> tokens;
        Token* token;
        bool notEOF = true;
        while(cursor < size && notEOF){

            checkAndSkip();
            /// logic for ids
            if(isalpha(current) || current == '_'){
                tokens.push_back(tokenizeID_KEYWORD());
                continue;
            }
            // logic for integer literals
            if(isdigit(current)){
                tokens.push_back(tokenizeINT());
                continue;
            }

            switch (current)
            {
            case ';':
                tokens.push_back(tokenizeSPECIAL(TOKEN_SEMICOLON));
                break;
            
            case '=':
                tokens.push_back(tokenizeSPECIAL(TOKEN_EQUAL));
                break;
            
            case '"':{
                tokens.push_back(tokenizeSPECIAL(TOKEN_QUOTE));
                tokens.push_back(tokenizeSTRING());
                tokens.push_back(tokenizeSPECIAL(TOKEN_QUOTE));

                break;
            }

            case '(':
                tokens.push_back(tokenizeSPECIAL(TOKEN_LPAREN));
                break;

            case ')':
                tokens.push_back(tokenizeSPECIAL(TOKEN_RPAREN));
                break;
            case 0:
                tokens.push_back(tokenizeSPECIAL(TOKEN_EOF));
                break;
            
            default:
                cout << "[!] Lexer Error: Unexpected character: " << current << endl;
                cout << "[!] Error at line: " << line << ", column: " << column << endl;
                exit(1);
            }
            
        }
       return tokens;
    }

    char peek(int offset = 0){
        if(cursor + offset < size){
            return source[cursor + offset];
        }
        return '\0';
    }

};
#endif 