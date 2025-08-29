#ifndef __GENERATOR_H
#define __GENERATOR_H

#include "parser.hpp"
#include <bits/stdc++.h>
using namespace std;

class Generator {
public: 
    Generator(AST_NODE* ROOT, string fileName) {
        AST_ROOT = ROOT;
        assemblyFile.open(fileName + ".asm");
        
    }

    void writeSections() {
        assemblyFile << sectionData.str() << endl;
        assemblyFile << sectionText.str() << endl;
        assemblyFile.close();
    }

    string generateReturn(AST_NODE* stmt) {
        if (!stmt->CHILD || !stmt->CHILD->VALUE) {
            cout << "[!] Generation error: Return statement is missing a value." << endl;
            exit(1);
        }

        stringstream codeBuffer;
        codeBuffer << "mov rax, 60\n";
        codeBuffer << "mov rdi, " << *stmt->CHILD->VALUE << "\n";
        codeBuffer << "syscall\n";
        return codeBuffer.str();
    }

    int lookup(string* DATA){
        int cnter = 0;
        for(string *temp : stringReferences){
            if(*temp == *DATA){
                return cnter;
            }
            cnter++;
        }
        return -1;  // Not found
    }

    void sectionDataDefine( string *data, int refNumber) {
        sectionData << "SRef" + to_string(refNumber) + " :\n";
        sectionData << "db \"" + *data + "\" \n";
        sectionData << "SRef"+ to_string(refNumber) + "_L : equ $ - SRef" + to_string(refNumber) + "\n\n\n";
    }

    void generatePRINT(AST_NODE* stmt) {
        if (!stmt->CHILD || !stmt->CHILD->VALUE) {
            cout << "[!] Generation error: the print statement is missing a value." << endl;
            exit(1);
        }
        int refNumber = lookup(stmt->CHILD->VALUE);
        if(refNumber == -1){
            refNumber = stringRefCounter++;
            sectionDataDefine(stmt->CHILD->VALUE , refNumber);
            stringReferences.push_back(stmt->CHILD->VALUE);
        }

        sectionText << "\n\nmov rax, 1\n"; // syscall: write
        sectionText << "mov rdi, 1\n"; // file descriptor: stdout
        sectionText << "mov rsi, SRef" + to_string(refNumber) + "\n"; // pointer to string
        sectionText << "mov rdx, SRef" + to_string(refNumber) + "_L\n"; // length of string
        sectionText << "syscall\n\n\n";
    }

    void generate() {

        stringRefCounter = 0;

        sectionData << "section .data\n\n";
        sectionText << "section .text\n"
                    << "global _start\n\n"
                    << "_start:\n";

        for (AST_NODE* CURRENT : AST_ROOT->SUB_STATEMENTS) {
            switch (CURRENT->TYPE) {
                case NODE_RETURN:
                    sectionText << generateReturn(CURRENT) << endl;
                    break;

                case NODE_PRINT:
                    generatePRINT(CURRENT);
                    break;

                default:
                    cout << "unrecognized AST node type: " 
                         << nodeToString(CURRENT->TYPE) << endl;
                    break;
            }
        }
        writeSections();
    }

private:
    stringstream sectionData;
    stringstream sectionText;
    AST_NODE* AST_ROOT;
    ofstream assemblyFile;
    int stringRefCounter;
    vector<string *> stringReferences;
};

#endif
