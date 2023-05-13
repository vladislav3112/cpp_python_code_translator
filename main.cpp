#include <iostream>

#include "antlr4-runtime.h"
#include "Python3Lexer.h"
#include "Python3Parser.h"
#include "Python3BaseListener.h"
#include "Python3BaseVisitor.h"

#include "tree/ParseTreeWalker.h"

int main(int argc, const char* argv[]) {
    
    //variable that specifies should output be saved as main() func only or not:
    //you should call main func in .py as main() otherwise there won't be main func unless SAVE_AS_MAIN_ONLY=true

    bool SAVE_AS_MAIN_ONLY = true;

    //Create an input file stream
    //test2 for SAVE_AS_MAIN_ONLY=true
    //test for SAVE_AS_MAIN_ONLY=fasle

    std::ifstream infile("test2.py");



    // Create an ANTLR stream from the file stream
    antlr4::ANTLRInputStream input(infile);

    // Create a lexer from the ANTLR stream
    Python3Lexer lexer(&input);

    // Create a token stream from the lexer
    antlr4::CommonTokenStream tokens(&lexer);

    // Create a parser from the token stream
    Python3Parser parser(&tokens);    

    std::cout << "Listener output" << std::endl;
    antlr4::tree::ParseTreeWalker walker;
    antlr4::ParserRuleContext* fileInput = parser.file_input();
    Python3BaseListener* listener = new Python3BaseListener();
    walker.walk(listener, fileInput);

    std::cout << "Converted output" << std::endl;
    std::cout << listener->outputfilestr << std::endl;
    

    std::ofstream myfile("generated_code.txt");
    std::string main_head_start = "#include <iostream>\n#include <vector>\n\n int main () {\n";
    if (SAVE_AS_MAIN_ONLY) {
        if (myfile.is_open())
        {
            myfile << main_head_start;
            myfile << listener->outputfilestr;
            myfile << "}\n";
            myfile.close();
        }
    }
    else {
        if (myfile.is_open())
        {
            myfile << "#include <iostream>\n#include <vector>\n\n";
            myfile << listener->outputfilestr;
            myfile.close();
        }
    }
    return 0;
}