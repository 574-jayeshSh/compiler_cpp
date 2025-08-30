#ifndef __LEXER_HPP
#define __LEXER_HPP

#include <bits/stdc++.h>
using namespace std;


enum TokenType{
    TOKEN_ID,
    TOKEN_INT,
    TOKEN_ARE,
    TOKEN_EQUAL,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LSQR,
    TOKEN_RSQR,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_QUOTE,
    TOKEN_COMMA,
    TOKEN_FUNCTION,
    TOKEN_CALL,
    TOKEN_ARGUMENTS,
    TOKEN_REL_EQUAL,
    TOKEN_REL_NOTEQUAL,
    TOKEN_REL_LESSTHAN,
    TOKEN_REL_LESSTHANEQUAL,
    TOKEN_REL_GREATERTHAN,
    TOKEN_REL_GREATERTHANEQUAL,
    TOKEN_MATH,
    TOKEN_RANGE,
    TOKEN_TILL,
    TOKEN_TO,
    TOKEN_AS,
    TOKEN_REFERENCE,
    TOKEN_INDENT,
    TOKEN_DOT,
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
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_FUNCTION: return "TOKEN_FUNCTION";
        case TOKEN_CALL: return "TOKEN_CALL";
        case TOKEN_ARGUMENTS: return "TOKEN_ARGUMENTS";
        case TOKEN_REL_EQUAL: return "TOKEN_REL_EQUAL";
        case TOKEN_REL_NOTEQUAL: return "TOKEN_REL_NOTEQUAL";
        case TOKEN_REL_LESSTHAN: return "TOKEN_REL_LESSTHAN";
        case TOKEN_REL_LESSTHANEQUAL: return "TOKEN_REL_LESSTHANEQUAL";
        case TOKEN_REL_GREATERTHAN: return "TOKEN_REL_GREATERTHAN";
        case TOKEN_REL_GREATERTHANEQUAL: return "TOKEN_REL_GREATERTHANEQUAL";
        case TOKEN_MATH: return "TOKEN_MATH";
        case TOKEN_RANGE: return "TOKEN_RANGE";
        case TOKEN_TILL: return "TOKEN_TILL";
        case TOKEN_TO: return "TOKEN_TO";
        case TOKEN_AS: return "TOKEN_AS";
        case TOKEN_REFERENCE: return "TOKEN_REFERENCE";
        case TOKEN_INDENT: return "TOKEN_INDENT";
        case TOKEN_DOT: return "TOKEN_DOT";
        case TOKEN_ARE: return "TOKEN_ARE";
        case TOKEN_LSQR: return "TOKEN_LSQR";
        case TOKEN_RSQR: return "TOKEN_RSQR";
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
    bool newLine;

public:
    Lexer(string sourceCode) {
        source = sourceCode;
        cursor = 0;
        size = sourceCode.length();
        current = sourceCode.at(cursor);
        line = 1;
        column = 1;
    }
    
    char seek(int offset){
        if(cursor + offset < size){
            return source[cursor + offset];
        }
        return '\0';
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

    void skipNew(){
        while(current == '\n'){
            line++;
            column = 0;
            advance();
        }
    }

    void checkAndSkip(){
        while(current ==  ' ' || current == '\t' || current == '\r'){
            advance();
        }
    }

    bool isAlphaWrapper(){
        if(isalpha(current)){
            return true;
        }
        else if((int)current < 0)
            return true;
        
        else return false;
    }

    bool isAlnumWrapper(){

        if(isalnum(current)){
            return true;
        }
        else if((int)current < 0)
            return true;

        else return false;
    }


    vector<string> keywords = {"return", "print", "if", "else", "get", "clear", "sleep"};

    unordered_map<string ,string> translatables = {
        {"display" , "print"}
    }

    unordered_map<string , enum TokenType> convertibles = {
        {"is", TOKEN_EQUAL},
        {"plus", TOKEN_MATH},
        {"minus", TOKEN_MATH},
        {"times", TOKEN_MATH},
        {"into", TOKEN_MATH},
        {"by", TOKEN_MATH},
        {"mod", TOKEN_MATH},
        {"range", TOKEN_RANGE},
        {"till", TOKEN_TILL},
        {"to", TOKEN_TO},
        {"as", TOKEN_AS},
        {"are", TOKEN_ARE},
        {"function", TOKEN_FUNCTION},
        {"call" , TOKEN_CALL},
    };

    Token* tokenizeID_KEYWORD(){
        stringstream buffer;
        buffer << advance();

        while(isAlnumWrapper() || current == '_'){
            buffer << advance();
        }

        Token * newToken = new Token();
        newToken->VALUE = buffer.str();

        if(translatables.find(newToken->VALUE) != translatables.end()){
            newToken->VALUE = translatables[newToken->VALUE];
        }
        if(convertibles.find(newToken->VALUE) != convertibles.end()){
            newToken->TYPE = convertibles[newToken->VALUE];
        }
        else{
            newToken->TYPE = (find(keywords.begin(), keywords.end(),newToken->VALUE) != keywords.end()) ? TOKEN_KEYWORD : TOKEN_ID;
        } 

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
        
         if (newToken->VALUE == "\n")
		    newToken->VALUE = "\\n";

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

    Token* tokenizeSEMICOLON(int scope){
        Token* newToken = new Token();
        newToken->TYPE = TOKEN_SEMICOLON;
        newToken->VALUE = to_string(scope);

        return newToken;
    }

    vector<Token*> tokenize(){

        vector<Token*> tokens;
    
        bool notEOF = true;
        bool newLine = true;
        int scopeCounter;


        while(cursor < size && notEOF){

            checkAndSkip();
            /// logic for ids
             if(isAlphaWrapper() || current == '_') 
            {
                tokens.push_back(tokenizeID_KEYWORD());
                newLine = false;
                continue;
            }
            if(isdigit (current)) 
            {
                tokens.push_back(tokenizeINT());
                continue;
            }

            switch (current)
            {

            case '\n': 
                if(newLine){
                    skipNew();
                }
                else{
                    advance();
                    scopeCounter = 0;

                    while(current == '\n' || current == ' '){
                        if(current == 0){
                            scopeCounter = 0;
                        }
                        else{
                            scopeCounter++;
                        }
                        advance();
                    }

                    tokens.push_back(tokenizeSEMICOLON(scopeCounter));
                    line++;
                    column = 0;
                    newLine = true;
                }
                break;


            case ',':
                tokens.push_back(tokenizeSPECIAL(TOKEN_COMMA));
                break;
            
            case '=' :{
                if (seek(1) == '=')
                {
                    advance();
                    tokens.push_back(tokenizeSPECIAL(TOKEN_REL_EQUAL));
                }
                else
                    tokens.push_back(tokenizeSPECIAL(TOKEN_EQUAL));
                break;
            }

            case '!' :{
                if (seek(1) == '=')
                {
                    advance();
                    tokens.push_back(tokenizeSPECIAL(TOKEN_REL_NOTEQUAL));
                    break;
                }
                cout << "[!] Syntax Error : unexpected symbol : " << seek(1) << " expected  : = " << endl;
			    exit(1);
            }

            
            
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