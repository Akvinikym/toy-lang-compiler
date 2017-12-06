#include <fstream>
#include <iostream>
#include <string>
#include "AST/ast.h"
#include "codegen/codegen_visitor.h"
#include "semantical_analysis/semantic_visitor.h"
#include "lib/argparse-nortti.hpp"
#include "parsing/Lexer.h"
#include "parsing/Parser.h"
#include "lib/cxxopts.hpp"
#include "codegen/driver.h"

class CmdArgsParser {
public:
    std::string input;
    std::string output;

    bool printHelp;
    std::string helpText;

    bool error;

public:
    CmdArgsParser(int argc, char *argv[]) {
        ArgumentParser parser;
        parser.useExceptions(true);
        parser.appName(argv[0]);
        parser.addArgument("-h", "--help");
        parser.addArgument("-o", "--output", 1);
        parser.addFinalArgument("input");

        this->helpText = parser.usage();

        this->error = true;
        try {
            parser.parse(argc, (const char**)argv);
        }
        catch (std::exception &e) {
            return;
        }

        this->error = false;

        this->printHelp = parser.count("help");
        if (this->printHelp) {
            return;
        }

        if (!parser.count("input")) {
            this->error = true;
            return;
        }
        this->input = parser.retrieve<std::string>("input");

        if (!parser.count("output")) {
            this->error = true;
            return;
        }
        this->output = parser.retrieve<std::string>("output");
    }
};

int main(int argc, char *argv[]) {

    CmdArgsParser args(argc, argv);
    if (args.error || args.printHelp) {
        std::cout << args.helpText << std::endl;
        return 0;
    }

    std::ifstream fs(args.input);
    Lexer lexer(&fs);
    Parser parser(&lexer);
    auto program = parser.parse();
    std::cout << "Program parsed\n";

    AliasUnwrapVisitor().visit(*program);
    std::cout << "alias-unwrap: done\n";
    TypeDeduceVisitor().visit(*program);
    std::cout << "type-deduce: done\n";
    ConstEvalVisitor().visit(*program);
    std::cout << "const-eval: done\n";
    TypeCheckingVisitor().visit(*program);
    std::cout << "type-checking: done\n";

    CodegenVisitor v{args.output};
    v.visit(*program);
    v.generate();
    std::cout << "codegen: done\n";

    Driver driver(*program);
    std::ofstream(args.output + ".boot.cpp") << driver.code;
    std::cout << "driver: done\n";

    return 0;
}
