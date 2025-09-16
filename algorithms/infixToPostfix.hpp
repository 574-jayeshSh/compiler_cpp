#ifndef __INFIX_TO_POSTFIX__
#define __INFIX_TO_POSTFIX__

#include "../headers/parser.hpp"
#include <bits/stdc++.h>
using namespace std;

unordered_map<enum NODE_TYPE , int> operatorPrecedence = {
    {NODE_OP_ADD,0},
    {NODE_OP_SUB,0},
    {NODE_OP_DIV,1},
    {NODE_OP_MUL,1},
    {NODE_OP_MOD,2},
};

stack<AST_NODE*> operatorStack;

vector<AST_NODE*> infixToPostfix(vector<AST_NODE*> &operations){
    vector<AST_NODE*> resultant;

    for(AST_NODE* CURRENT : operations){
        if(CURRENT->TYPE == NODE_INT || CURRENT->TYPE == NODE_VARIABLE){
            resultant.push_back(CURRENT);
        }
        else{
            while(true){
                if(operatorStack.size() == 0){
                    operatorStack.push(CURRENT);
                    break;
                }
                else if(operatorPrecedence[operatorStack.top()->TYPE] >= operatorPrecedence[CURRENT->TYPE]){
                    resultant.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                else{
                    operatorStack.push(CURRENT);
                    break;
                }
            }
        }
    }

    while(operatorStack.size() != 0){
        resultant.push_back(operatorStack.top());
        operatorStack.pop();
    }

    return resultant;
}

#endif