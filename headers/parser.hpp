#ifndef __PARSER_H
#define __PARSER_H

#include <bits/stdc++.h>
#include "lexer.hpp"
using namespace std;

enum NODE_TYPE {
    NODE_ROOT,
    NODE_VARIABLE,
    NODE_RETURN,
    NODE_PRINT,
    NODE_INT,
    NODE_STRING,
};

string nodeToString(enum NODE_TYPE type) {
    switch (type) {
        case NODE_ROOT: return "NODE_ROOT";
        case NODE_VARIABLE: return "NODE_VARIABLE";
        case NODE_RETURN: return "NODE_RETURN";
        case NODE_PRINT: return "NODE_PRINT";
        case NODE_INT: return "NODE_INT";
        case NODE_STRING: return "NODE_STRING";
        default: return "UNKNOWN_NODE";
    }
}

struct AST_NODE {
    enum NODE_TYPE TYPE;
    string* VALUE = nullptr;
    AST_NODE* CHILD = nullptr;
    vector<AST_NODE*> SUB_STATEMENTS;
};

class Parser {
public:
    Parser(vector<Token*> tokens) {
        parserTokens = tokens;
        index = 0;
        limit = parserTokens.size();
        current = parserTokens.at(index);
    }

    Token* proceed(enum TokenType TYPE) {
        if (current->TYPE != TYPE) {
            cout << "[!] Syntax error: Expected " << typeToStr(TYPE)
                 << " but found " << typeToStr(current->TYPE) << endl;
            exit(1);
        } else {
            index++;
            if (index < limit)
                current = parserTokens.at(index);
            return current;
        }
    }

    AST_NODE* parseINT() {
        if (current->TYPE != TOKEN_INT) {
            cout << "[!] Syntax error: Expected INT but found "
                 << typeToStr(current->TYPE) << endl;
            exit(1);
        }
        AST_NODE* node = new AST_NODE();
        node->TYPE = NODE_INT;
        node->VALUE = &current->VALUE;
        proceed(TOKEN_INT);
        return node;
    }

    AST_NODE* parseID() {
        string* buffer = &current->VALUE;
        proceed(TOKEN_ID);
        proceed(TOKEN_EQUAL);
        AST_NODE* node = new AST_NODE();
        node->TYPE = NODE_VARIABLE;
        node->VALUE = buffer;
        node->CHILD = parseINT();
        return node;
    }

    AST_NODE* parseRETURN() {
        proceed(TOKEN_KEYWORD);
        AST_NODE* node = new AST_NODE();
        node->TYPE = NODE_RETURN;
        node->CHILD = parseINT();
        return node;
    }

    AST_NODE* parseSTRING(){
        if(current -> TYPE != TOKEN_STRING){
            cout << "[!] Syntax error: Expected STRING but found "
                 << typeToStr(current->TYPE) << endl;
            exit(1);
        }
        AST_NODE* node = new AST_NODE();
        node->TYPE = NODE_STRING;
        node->VALUE = &current->VALUE;
        proceed(TOKEN_STRING);
        return node;
    }

    AST_NODE* parsePRINT() {
        proceed(TOKEN_KEYWORD);
        proceed(TOKEN_LPAREN);
        proceed(TOKEN_QUOTE);
        // parse 
        AST_NODE* newNode = new AST_NODE();
        newNode -> TYPE = NODE_PRINT;
        newNode->CHILD = parseSTRING();
        proceed(TOKEN_QUOTE);
        proceed(TOKEN_RPAREN);

        return newNode;
    }

    AST_NODE* parseKEYWORD() {
        if (current->VALUE == "return") {
            return parseRETURN();
        } else if (current->VALUE == "print") {
            return parsePRINT();
        } else {
            cout << "[!] Syntax error: Unexpected keyword " << current->VALUE << endl;
            exit(1);
        }
    }

    AST_NODE* parse() {
        AST_NODE* ROOT = new AST_NODE();
        ROOT->TYPE = NODE_ROOT;

        while (current->TYPE != TOKEN_EOF) {
            switch (current->TYPE) {
                case TOKEN_ID: {
                    ROOT->SUB_STATEMENTS.push_back(parseID());
                    break;
                }
                case TOKEN_KEYWORD: {
                    ROOT->SUB_STATEMENTS.push_back(parseKEYWORD());
                    break;
                }
                default: {
                    cout << "[!] Syntax error: Unexpected token "
                         << typeToStr(current->TYPE) << endl;
                    exit(1);
                }
            }
            proceed(TOKEN_SEMICOLON);
        }
        return ROOT;
    }

private:
    int limit;
    int index;
    Token* current;
    vector<Token*> parserTokens;
};

#endif
