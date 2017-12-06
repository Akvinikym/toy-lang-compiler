#ifndef ILANG_DRIVER_H
#define ILANG_DRIVER_H

#include "../visitor.h"
#include <string.h>
#include "../AST/prototype.h"
#include "../AST/arraydecl.h"
#include "../AST/arrayref.h"
#include "../AST/assignment.h"
#include "../AST/binary.h"
#include "../AST/boolean.h"
#include "../AST/booleantype.h"
#include "../AST/cast.h"
#include "../AST/for.h"
#include "../AST/if.h"
#include "../AST/integer.h"
#include "../AST/integertype.h"
#include "../AST/namedref.h"
#include "../AST/real.h"
#include "../AST/realtype.h"
#include "../AST/recorddecl.h"
#include "../AST/recordref.h"
#include "../AST/return.h"
#include "../AST/routine.h"
#include "../AST/routinecall.h"
#include "../AST/program.h"
#include "../AST/statements.h"
#include "../AST/typedecl.h"
#include "../AST/unary.h"
#include "../AST/var.h"
#include "../AST/void.h"
#include "../AST/while.h"

class RoutineListVisitor : public Visitor
{
public:
    std::vector<Routine*> routines;

    void visit(Prototype&) {}
    void visit(ArrayDecl&) {}
    void visit(ArrayRef&) {}
    void visit(Assignment&) {}
    void visit(Binary&) {}
    void visit(Boolean&) {}
    void visit(BooleanType&) {}
    void visit(Cast&) {}
    void visit(For&) {}
    void visit(If&) {}
    void visit(Integer&) {}
    void visit(IntegerType&) {}
    void visit(NamedRef&) {}
    void visit(Program& node) {
        for (auto x : node.program) {
            x->accept(*this);
        }
    }
    void visit(Real&) {}
    void visit(RealType&) {}
    void visit(RecordDecl&) {}
    void visit(RecordRef&) {}
    void visit(Return&) {}
    void visit(Routine& node) {
        routines.push_back(&node);
    }
    void visit(RoutineCall&) {}
    void visit(Statements&) {}
    void visit(TypeDecl&) {}
    void visit(Unary&) {}
    void visit(Undefined&) {};
    void visit(Var&) {}
    void visit(Void&) {}
    void visit(While&) {}
};

class Driver {
public:
    std::string code;

    Driver(Program& program) {
        RoutineListVisitor list;
        program.accept(list);

        std::vector<Routine*> runnable;
        std::vector<Routine*> complex;
        for (auto f : list.routines) {
            bool argsOk = true;
            for (auto arg : f->proto->args) {
                argsOk = argsOk && Type::isPrimitive(*arg->var_decl.second);
            }
            bool retOk = Type::isPrimitive(*f->proto->type);
            if (argsOk && retOk) {
                runnable.push_back(f);
            } else {
                complex.push_back(f);
            }
        }

        std::stringstream s;

        s << "#include <string>\n";
        s << "#include <iostream>\n";
        s << "\n";

        for (auto f : runnable) {
            s << makePrototype(*f);
        }
        s << "\n";

        s << "int main() {\n";
        s << "    std::string name;\n";
        s << "    std::cin >> name;\n";
        s << "\n";

        for (auto f : runnable) {
            s << "    if (name == \"" << f->proto->name << "\") {\n";
            for (auto arg : f->proto->args) {
                s << "        " << makeType(*arg->var_decl.second) << " " << arg->var_decl.first << ";\n";
                s << "        std::cin >> " << arg->var_decl.first << ";\n";
            }
            s << "        std::cout << " << f->proto->name << "(";
            auto i = 0;
            for (auto arg : f->proto->args) {
                if (i != 0) {
                    s << ", ";
                }
                i++;
                s << arg->var_decl.first;
            }
            s << ") << \"\\n\";\n";
            s << "        return 0;\n";
            s << "    }\n";
        }
        s << "\n";

        if (!complex.empty()) {
            s << "    if (";
            auto i = 0;
            for (auto f : complex) {
                if (i != 0) {
                    s << " || ";
                }
                i++;
                s << "name == \"" << f->proto->name << "\"";
            }
            s << "    ) {\n";
            s << "        std::cout << \"function input or output is complex\";\n";
            s << "        return -1;\n";
            s << "    }\n";
            s << "\n";
        }

        s << "    std::cout << \"function does not exist\";\n";
        s << "    return -2;\n";
        s << "}\n";

        code = s.str();
    }
    std::string makePrototype(Routine& routine) {
        std::stringstream s;
        s << "extern \"C\" " << makeType(*routine.proto->type) << " " << routine.proto->name << "(";
        auto i = 0;
        for (auto arg : routine.proto->args) {
            if (i != 0) {
                s << ", ";
            }
            i++;
            s << makeType(*arg->var_decl.second) << " " << arg->var_decl.first;
        }
        s << ");\n";
        return s.str();
    }
    std::string makeType(Type& type) {
        if (type.type == types::Boolean) {
            return "bool";
        }
        if (type.type == types::Integer) {
            return "int";
        }
        if (type.type == types::Real) {
            return "double";
        }
        return "BAD_TYPE";
    }
};

#endif // !ILANG_DRIVER_H
