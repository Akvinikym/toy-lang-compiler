#include <memory>
#include <unordered_map>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"


#include "../visitor.h"
#include "../AST/prototype.h"
#include "../AST/integer.h"
#include "../AST/real.h"
#include "../AST/routine.h"
#include "../AST/binary.h"
#include "../AST/routinecall.h"
#include "../AST/return.h"
#include "../AST/namedref.h"
#include "../AST/var.h"
#include "../AST/program.h"
#include "../AST/boolean.h"
#include "../AST/if.h"
#include "../AST/unary.h"
#include "../AST/assignment.h"
#include "../AST/while.h"
#include "../AST/for.h"
#include "../AST/arraydecl.h"
#include "../AST/arrayref.h"
#include "../AST/recorddecl.h"
#include "../AST/recordref.h"
#include "../AST/typedecl.h"
#include "../AST/cast.h"

// using namespace llvm;

class CodegenVisitor : Visitor
{
private:
    llvm::LLVMContext TheContext;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> TheModule;
    llvm::TargetMachine* targetMachine;
    llvm::raw_fd_ostream output;
    std::error_code EC;

    Var *last_decl;
    llvm::Value *last_constant;
    llvm::Function *last_function;
    std::unordered_map<std::string, llvm::Value*> last_params;
    std::unordered_map<RecordDecl*, llvm::StructType*> structs;
    bool is_lvalue{false};
    // std::unordered_map<std::string, llvm::Value*> last_variables;

    llvm::Type* get_type(Type *type);
    llvm::ConstantInt* get_const_int(int value);

public:
    CodegenVisitor(const std::string& filename);
    CodegenVisitor() = delete;
    void visit(Prototype& node);
    void visit(ArrayDecl& node);
    void visit(Assignment& node);
    void visit(Binary& node);
    void visit(Boolean& node);
    void visit(BooleanType& node);
    void visit(For& node);
    void visit(If& node);
    void visit(Integer& node);
    void visit(IntegerType& node);
    void visit(Real& node);
    void visit(RealType& node);
    void visit(RecordDecl& node);
    void visit(Routine& node);
    void visit(RoutineCall& node);
    void visit(TypeDecl& node);
    void visit(Unary& node);
    void visit(Undefined& node);
    void visit(Var& node);
    void visit(NamedRef& node);
    void visit(While& node);
    void visit(Return& node);
    void visit(RecordRef& node);
    void visit(ArrayRef& node);
    void visit(Program& node);
    void visit(Statements& node);
    void visit(Void& node);
    void visit(Cast& node);
    void generate();
};
