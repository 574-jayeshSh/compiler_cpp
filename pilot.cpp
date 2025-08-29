#include<bits/stdc++.h>
#include "headers/lexer.hpp"
#include "headers/parser.hpp"
#include "headers/generator.hpp"
using namespace std;

int main(int argc , char **argv){
    if(argc < 2){
        cout << "No input file specified!";
        exit(1);
    }
    // reading from input file
    cout << "Compiling: " << argv[1] << endl;
    ifstream sourceFileStream(argv[1]);
    stringstream buffer;

    char temp;

    while(sourceFileStream.get(temp)){
        buffer << temp;
    }

    string sourceCode = buffer.str();

    // cout << "Source code read from file:" << endl;
    // cout << endl << sourceCode << endl;
    sourceCode.append("\0");


    Lexer lexer(sourceCode);
    vector<Token*> tokens = lexer.tokenize();

    int counter = 0; 
    cout << endl << "[*] Tokenized successfully:" << endl; 
    
    if(tokens.back() ->TYPE != TOKEN_EOF){
        Token* EOFNode = new Token();
        EOFNode->TYPE = TOKEN_EOF;
        tokens.push_back(EOFNode);
    }
    
    for(Token * temp : tokens){
        counter++;
        // cout << counter << ")" << typeToStr(temp->TYPE)  << ": " << (temp->VALUE) << endl;
    }


    Parser parser(tokens);
    AST_NODE* ROOT = parser.parse();
    // cout << "Number of Statements: " << ROOT->SUB_STATEMENTS.size() << endl;

    Generator codeGen(ROOT, argv[1]);
    codeGen.generate();

    string fileName = argv[1];
    fileName.pop_back();
    fileName.pop_back();
    fileName.pop_back();

    stringstream assemblerInstruction;
    assemblerInstruction << "nasm -f elf64 " + fileName + ".pi.asm";

    system(assemblerInstruction.str().c_str());

    stringstream linkerInstruction;
    linkerInstruction << "ld -o " + fileName + " " + fileName + ".pi.o";

    system(linkerInstruction.str().c_str());

    // cout << "End of file reached" << endl;
    return 0;
} 