#include "codegen_visitor.h"
#include <iostream>
#include <iterator>

CodegenVisitor::CodegenVisitor(const std::string& filename): Builder{TheContext}, output{filename, EC, llvm::sys::fs::F_None}
{
    TheModule = llvm::make_unique<llvm::Module>("my cool jit", TheContext);
    auto TargetTriple = llvm::sys::getDefaultTargetTriple();
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();
    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
    if (!Target) {
        llvm::errs() << Error;
    }

    auto CPU = "generic";
    auto Features = "";
    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    targetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(targetMachine->createDataLayout());
    TheModule->setTargetTriple(TargetTriple);
}

void CodegenVisitor::generate()
{
    llvm::legacy::PassManager pass;
    auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

    targetMachine->addPassesToEmitFile(pass, output, FileType);
    std::cout << "Generating Object Code\n";
    pass.run(*TheModule);
    std::cout << "Printing IR\n";
    TheModule->print(llvm::errs(), nullptr);
    output.flush();
}

llvm::Type* CodegenVisitor::get_type(Type *type)
{
    switch (type->type) {
    case types::Integer:
        std::cout << "Generating integer type\n";
        return llvm::Type::getInt32Ty(TheContext);
    case types::Real:
        return llvm::Type::getDoubleTy(TheContext);
    case types::Boolean:
        return llvm::Type::getInt1Ty(TheContext);
    case types::Void:
        return llvm::Type::getVoidTy(TheContext);
    case types::Array: {
        auto elem_type = ((ArrayDecl *)type)->array_type;
        switch (elem_type->type) {
        case types::Integer:
            std::cout << "Generating pointer to integer\n";
            return llvm::Type::getInt32PtrTy(TheContext);
        case types::Real:
            std::cout << "Generating pointer to double\n";
            return llvm::Type::getDoublePtrTy(TheContext);
        case types::Boolean:
            return llvm::Type::getInt1PtrTy(TheContext);
        case types::Array:
            std::cout << "Array in Array\n";
            return llvm::PointerType::getUnqual(get_type(elem_type));
        case types::Record:
            std::cout << "Record in Array\n";
            auto rec = (RecordDecl*) elem_type;
            return llvm::PointerType::getUnqual(get_type(elem_type));
        }
    }
    case types::Record: {
        std::cout << "Record type\n";
        auto rec = (RecordDecl*)  type;
        std::cout << "Record Referenced " << rec << "\n";
        return llvm::PointerType::getUnqual(structs[rec]);
    }
    default:
        std::cout << "Unknown type " << (int)type->type << "\n";
        break;
    }
}

llvm::ConstantInt *CodegenVisitor::get_const_int(int value) {
    return llvm::ConstantInt::get(TheContext, llvm::APInt(32, value, true));
}

void CodegenVisitor::visit(Prototype& p)
{
    std::cout << "Parsing Prototype\n";
    last_params.clear();

    // Generate types of arguments
    std::vector<llvm::Type*> arg_types{};
    for (int i =0; i < p.args.size(); i++) {
        auto arg = (Var *) p.args[i];
        arg_types.push_back(get_type(arg->var_decl.second));
        std::cout << "Argument Type: " << (int)arg->var_decl.second->type <<"\n";
    }

    // function type generation
    auto ft = llvm::FunctionType::get(get_type(p.type), arg_types, false);
    last_function = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, p.getName(), TheModule.get());
}

void CodegenVisitor::visit(ArrayDecl& node)
{
    std::cout << "Generating ArrayDecl\n";
    llvm::Function *f = TheModule->getFunction("malloc");
    std::vector<llvm::Value*> ArgsV;
    node.expression->accept(*this);
    auto elem_type = node.array_type;
    auto num_of_elems = last_constant;
    auto ll_type = get_type(elem_type);
    auto size = TheModule->getDataLayout().getTypeAllocSize(ll_type);
    last_constant = Builder.CreateMul(
        get_const_int(size),
        last_constant
    );
    last_constant = Builder.CreateZExt(last_constant, llvm::Type::getInt64Ty(TheContext));
    ArgsV.push_back(last_constant);
    auto ptr = Builder.CreateCall(f, ArgsV);
    auto arr_ptr = Builder.CreateBitCast(ptr, get_type(&node));
    if (elem_type->type == types::Array || elem_type->type == types::Record) {
        // initialize every sub element
        auto elem_count = ((llvm::ConstantInt *) num_of_elems)->getSExtValue();
        for (int i = 0; i < elem_count; i++) {
            node.array_type->accept(*this);
            auto elem_address = Builder.CreateGEP(arr_ptr, get_const_int(i));
            last_constant->getType()->print(llvm::errs());
            elem_address->getType()->print(llvm::errs());
            Builder.CreateStore(last_constant, elem_address);
        }
    }
    last_constant = Builder.CreateBitCast(arr_ptr, get_type(&node));
}

void CodegenVisitor::visit(Assignment& node)
{
    std::cout << "Generating Assignment\n";

    if (!node.value) {
        std::cout << "No right side\n";
    }
    node.value->accept(*this);
    auto expr = last_constant;
    is_lvalue = true;
    node.ref->accept(*this);
    is_lvalue = false;
    Builder.CreateStore(expr, last_constant);

    std::cout << "Assignment generated\n";
}

void CodegenVisitor::visit(Binary& node)
{
    std::cout << "Parsing Binary\n";
    // node.type->type = types::Integer;
    node.lhs->accept(*this);
    auto L = last_constant;
    node.rhs->accept(*this);
    auto R = last_constant;

    switch (node.opchar) {
    case opchars::PLUS:
        if (node.type->type == types::Integer) {
            last_constant = Builder.CreateAdd(L, R, "tmp");
        } else if (node.type->type == types::Real){
            last_constant = Builder.CreateFAdd(L, R, "tmp");
        }
        break;
    case opchars::MINUS:
        if (node.type->type == types::Integer) {
            last_constant = Builder.CreateSub(L, R, "tmp");
        } else if (node.type->type == types::Real){
            last_constant = Builder.CreateFSub(L, R, "tmp");
        }
        break;
    case opchars::MUL:
        if (node.type->type == types::Integer) {
            last_constant = Builder.CreateMul(L, R, "tmp");
        } else if (node.type->type == types::Real){
            last_constant = Builder.CreateFMul(L, R, "tmp");
        }
        break;
    case opchars::DIV:
        if (node.type->type == types::Integer) {
            last_constant = Builder.CreateSDiv(L, R, "tmp");
        } else if (node.type->type == types::Real){
            last_constant = Builder.CreateFDiv(L, R, "tmp");
        }
        break;
    case opchars::MOD:
        if (node.type->type == types::Integer) {
            last_constant = Builder.CreateSRem(L, R, "tmp");
        } else if (node.type->type == types::Real){
            last_constant = Builder.CreateFRem(L, R, "tmp");
        }
        break;
    case opchars::NOTEQ:
        if (node.lhs->type->type == types::Integer) {
            last_constant = Builder.CreateICmpNE(L, R, "cond");
        } else if (node.lhs->type->type == types::Real){
           last_constant = Builder.CreateFCmpONE(L, R, "cond");
        }
        break;
    case opchars::EQUAL:
        if (node.lhs->type->type == types::Integer) {
            last_constant = Builder.CreateICmpEQ(L, R, "cond");
        } else if (node.lhs->type->type == types::Real){
           last_constant = Builder.CreateFCmpOEQ(L, R, "cond");
        }
        break;
    case opchars::HIGH:
        if (node.lhs->type->type == types::Integer) {
            last_constant = Builder.CreateICmpSGT(L, R, "cond");
        } else if (node.lhs->type->type == types::Real){
           last_constant = Builder.CreateFCmpOGT(L, R, "cond");
        }
        break;
    case opchars::LESS:
        std::cout << "Generating LESS\n";
        if (node.lhs->type->type == types::Integer) {
            last_constant = Builder.CreateICmpSLT(L, R, "cond");
        } else if (node.lhs->type->type == types::Real){
           last_constant = Builder.CreateFCmpOLT(L, R, "cond");
        }
        break;
    case opchars::HIGHEQ:
        if (node.lhs->type->type == types::Integer) {
            last_constant = Builder.CreateICmpSGE(L, R, "cond");
        } else if (node.lhs->type->type == types::Real){
           last_constant = Builder.CreateFCmpOGE(L, R, "cond");
        }
        break;
    case opchars::LESSEQ:
        if (node.lhs->type->type == types::Integer) {
            last_constant = Builder.CreateICmpSLE(L, R, "cond");
        } else if (node.lhs->type->type == types::Real){
           last_constant = Builder.CreateFCmpOLE(L, R, "cond");
        }
        break;
    case opchars::AND:
        std::cout << "Generating AND\n";
        last_constant = Builder.CreateAnd(L, R);
        break;
    case opchars::OR:
        last_constant = Builder.CreateOr(L, R);
        break;
    case opchars::XOR:
        last_constant = Builder.CreateXor(L, R);
        break;
    }
}

void CodegenVisitor::visit(Boolean& node)
{
    std::cout << "Generating Boolean\n";
    last_constant = llvm::ConstantInt::get(TheContext, llvm::APInt(1, node.value ? 1 : 0, false));
}
void CodegenVisitor::visit(BooleanType& node)
{
    std::cout << "Generating BooleanType\n";
}

void CodegenVisitor::visit(For& node)
{
    std::cout << "Generating For loop\n";
    auto cond = llvm::BasicBlock::Create(TheContext, "loopcond");
    auto loop = llvm::BasicBlock::Create(TheContext, "loop");
    last_function->getBasicBlockList().push_back(cond);
    auto end = llvm::BasicBlock::Create(TheContext, "loopend");

    node.name->accept(*this);
    auto iter = last_params[((Var *) node.name)->var_decl.first];
    node.start->accept(*this);
    Builder.CreateStore(last_constant, iter);
    node.end->accept(*this);
    auto endexpr = Builder.CreateAlloca(llvm::Type::getInt32Ty(TheContext), 0);
    Builder.CreateStore(last_constant, endexpr);

    Builder.CreateBr(cond);
    Builder.SetInsertPoint(cond);
    std::cout << "Generating For condition\n";
    // insert condition
    auto i = Builder.CreateLoad(iter, "i");
    auto pred = Builder.CreateLoad(endexpr, "pred");
    if (node.reverse) {
        last_constant = Builder.CreateICmpSGT(i, pred, "comp");
    } else {
        last_constant = Builder.CreateICmpSLT(i, pred, "comp");
    }
    Builder.CreateCondBr(last_constant, loop, end);
    // //
    std::cout << "Generating For body\n";
    last_function->getBasicBlockList().push_back(loop);
    Builder.SetInsertPoint(loop);
    node.body->accept(*this);
    i = Builder.CreateLoad(iter, "i");
    llvm::Value *incr;
    if (node.reverse) {
        incr = Builder.CreateSub(i, get_const_int(1));
    } else {
        incr = Builder.CreateAdd(i, get_const_int(1));
    }
    Builder.CreateStore(incr, iter);
    Builder.CreateBr(cond);
    last_function->getBasicBlockList().push_back(end);
    Builder.SetInsertPoint(end);
}

void CodegenVisitor::visit(If& node)
{
    std::cout << "Generating If Statement\n";
    auto then = llvm::BasicBlock::Create(TheContext, "then");
    auto elsecond = llvm::BasicBlock::Create(TheContext, "else");
    auto after_if = llvm::BasicBlock::Create(TheContext, "ifend");

    // insert condition
    node.condition->accept(*this);
    std::cout << "Condition Generated\n";
    if (node.else_body) {
        Builder.CreateCondBr(last_constant, then, elsecond);
    } else {
        Builder.CreateCondBr(last_constant, then, after_if);
    }
    // //
    last_function->getBasicBlockList().push_back(then);
    Builder.SetInsertPoint(then);
    node.then->accept(*this);
    Builder.CreateBr(after_if);
    if (node.else_body) {
        std::cout << "Else block\n";
        last_function->getBasicBlockList().push_back(elsecond);
        Builder.SetInsertPoint(elsecond);
        node.else_body->accept(*this);
        Builder.CreateBr(after_if);
    }
    last_function->getBasicBlockList().push_back(after_if);
    Builder.SetInsertPoint(after_if);
}

void CodegenVisitor::visit(Integer& node)
{
    std::cout << "Generating Integer\n";
    last_constant = get_const_int(node.value);
}

void CodegenVisitor::visit(IntegerType& node)
{
    std::cout << "Generating Integer Type\n";
}

void CodegenVisitor::visit(Real& node)
{
    std::cout << "Generating Real\n";
    last_constant = llvm::ConstantFP::get(TheContext, llvm::APFloat(node.value));
}

void CodegenVisitor::visit(RealType& node)
{
    std::cout << "Generating Real Type\n";
}
void CodegenVisitor::visit(RecordDecl& node)
{
    static int structN;
    if (structs.find(&node) != structs.end()) {
        // TODO: Finish record declaration initialization
        std::cout << "Record already generated\n";
        llvm::Function *f = TheModule->getFunction("malloc");
        std::vector<llvm::Value*> ArgsV;;
        uint64_t i = 0;
        for (auto& member : node.refs) {
            auto var = (Var *) member;
            auto ll_type = get_type(var->var_decl.second);
            i += TheModule->getDataLayout().getTypeAllocSize(ll_type);
        }
        std::cout << "Record size " << i << "\n";
        last_constant = get_const_int(i);
        last_constant = Builder.CreateZExt(last_constant, llvm::Type::getInt64Ty(TheContext));
        ArgsV.push_back(last_constant);
        auto ptr = Builder.CreateCall(f, ArgsV);
        auto record_ptr = Builder.CreateBitCast(ptr, get_type(&node));

        i = 0;
        for (auto& member : node.refs) {
            auto var = (Var *) member;
            if (var->var_decl.second->type == types::Record ||
                var->var_decl.second->type == types::Array) {
                var->var_decl.second->accept(*this);
                std::vector<llvm::Value *> indices{get_const_int(0), get_const_int(i)};
                auto elem_address = Builder.CreateGEP(
                    structs[&node],
                    record_ptr,
                    indices
                );
                last_constant->getType()->print(llvm::errs());
                elem_address->getType()->print(llvm::errs());
                Builder.CreateStore(last_constant, elem_address);
                i++;
            }
        }
        last_constant = record_ptr;
        return;
    }
    std::cout << "Generating Record Declaration\n";
    std::vector<llvm::Type*> members;
    for (auto& var : node.refs) {
        auto v = (Var *) var;
        members.push_back(get_type(v->var_decl.second));
    }
    // llvm::Function::Create(ft, llvm::Function::ExternalLinkage, p.getName(), TheModule.get());
    auto name = "struct" + std::to_string(structN);
    structN++;
    auto rec = llvm::StructType::create(TheContext, members, name, false);
    structs[&node] = rec;
    std::cout << "Record address " << &node << "\n";
}

void CodegenVisitor::visit(Routine& node)
{
    std::cout << "Parsing routine\n";
    node.proto->accept(*this);
    std::cout << "Parsed Prototype\n";

    if (!last_function) {
        std::cout << "No last function\n";
    }
    llvm::BasicBlock *BB = llvm::BasicBlock::Create(TheContext, "entry", last_function);
    Builder.SetInsertPoint(BB);
    std::cout << "Basic Block Created\n";
    // arguments generation
    unsigned Idx = 0;
    for (auto &Arg : last_function->args()) {
        auto var = ((Var *) node.proto->args[Idx++]);
        std::string name = var->var_decl.first;//((Var *) node.proto->args[Idx])->var_decl.first;
        llvm::AllocaInst *v = Builder.CreateAlloca(get_type(var->var_decl.second), 0, name);
        std::cout << "Argument Allocated: " << (int)var->var_decl.second->type <<"\n";
        Builder.CreateStore(&Arg, v);
        Arg.setName(name);
        last_params[name] = v;
    }

    if (node.body->statements.size() > 0) {
        std::cout << "Creating function body\n";
        node.body->accept(*this);
        std::cout << "Created function body\n";
        if (last_function->getReturnType() == llvm::Type::getVoidTy(TheContext)) {
            Builder.CreateRetVoid();
            return;
        }
    } else {
        std::cout << "Creating Return void\n";
        Builder.CreateRetVoid();
    }

    verifyFunction(*last_function);
}

void CodegenVisitor::visit(RoutineCall& node)
{
    std::cout << "Parsing call to" << node.callee << "\n";
    llvm::Function *f = TheModule->getFunction(node.callee->proto->name);
    if (!f) {
        std::cout << "no function\n";
    }
    std::vector<llvm::Value*> ArgsV;
    for (auto& arg : node.args) {
        arg->accept(*this);
        ArgsV.push_back(last_constant);
    }

    last_constant = Builder.CreateCall(f, ArgsV);
    std::cout << "Call Generated\n";
}

void CodegenVisitor::visit(TypeDecl& node)
{
    std::cout << "Generating Type Decl\n";
    node.ref_type->accept(*this);
}

void CodegenVisitor::visit(Unary& node)
{
    std::cout << "Generating Unary ";
    std::cout << node.opcode << "\n";

}

void CodegenVisitor::visit(Var& node)
{
    std::cout << "Creating Var declaration\n";
    auto name = node.var_decl.first;
    auto v = Builder.CreateAlloca(get_type(node.var_decl.second), 0, name);

    if (node.var_decl.second->type == types::Array) {
        node.var_decl.second->accept(*this);
        Builder.CreateStore(last_constant, v);
    } else if (node.var_decl.second->type == types::Record) {
        node.var_decl.second->accept(*this);
        Builder.CreateStore(last_constant, v);
    } else {
        std::cout << "Generating variable declaration\n";
        if (node.body) {
            std::cout << "Generating initialization of variable\n";
            node.body->accept(*this);
            std::cout << "Generated initialization of variable\n";
            Builder.CreateStore(last_constant, v);
        }
    }

    last_params[name] = v;
    std::cout << "Created Var declaration\n";
}

void CodegenVisitor::visit(NamedRef& node)
{
    std::cout << "Generating NamedRef " << (int)node.type->type << "\n";
    if (!node.var ) {
        std::cout << "var is 0\n";
    }
    last_decl = node.var;
    std::string name = node.var->var_decl.first;
    auto value = last_params[name];
    if (is_lvalue) {
        last_constant = value;
        std::cout << "Generated NamedRef address\n";
    } else {
        last_constant = Builder.CreateLoad(value, name.c_str());
        std::cout << "Generated NamedRef value\n";
    }
}

void CodegenVisitor::visit(Return& node) {
    std::cout << "Parsing Return\n";
    node.expression->accept(*this);
    Builder.CreateRet(last_constant);
}

void CodegenVisitor::visit(While& node) {
    std::cout << "Generating While loop\n";
    auto cond = llvm::BasicBlock::Create(TheContext, "loopcond");
    auto loop = llvm::BasicBlock::Create(TheContext, "loop");
    last_function->getBasicBlockList().push_back(cond);
    auto end = llvm::BasicBlock::Create(TheContext, "loopend");
    Builder.CreateBr(cond);
    Builder.SetInsertPoint(cond);

    // insert condition
    node.expression->accept(*this);
    Builder.CreateCondBr(last_constant, loop, end);
    // //
    last_function->getBasicBlockList().push_back(loop);
    Builder.SetInsertPoint(loop);
    node.body->accept(*this);
    Builder.CreateBr(cond);
    last_function->getBasicBlockList().push_back(end);
    Builder.SetInsertPoint(end);
}

void CodegenVisitor::visit(RecordRef& node) {
    std::cout << "Generating Record Reference\n";
    bool old_lvalue = is_lvalue;
    is_lvalue = false;
    node.prev->accept(*this);
    is_lvalue = old_lvalue;
    auto record_address = last_constant;
    auto decl = (RecordDecl *)(last_decl->var_decl.second);
    auto record = structs[decl];
    int i = 0;
    while (((Var*) decl->refs[i])->var_decl.first != node.ref && i < (decl->refs.size())) {
        i++;
    }
    std::vector<llvm::Value *> indices{get_const_int(0), get_const_int(i)};
    auto elem_address = Builder.CreateGEP(
        structs[decl],
        record_address,
        indices
    );
    if (is_lvalue) {
        last_constant = elem_address;
        std::cout << "Recordref Address Generated\n";
    } else {
        last_constant = Builder.CreateLoad(elem_address);
        std::cout << "Recordref Value Generated\n";
    }
}

void CodegenVisitor::visit(ArrayRef& node) {
    std::cout << "Generating ArrayRef\n";
    bool old_lvalue = is_lvalue;
    is_lvalue = false;
    node.prev->accept(*this);
    is_lvalue = old_lvalue;
    auto arr_address = last_constant;

    std::cout << "Generating Array Index\n";
    old_lvalue = is_lvalue;
    is_lvalue = false;
    node.pos->accept(*this);
    is_lvalue = old_lvalue;
    auto pos = Builder.CreateSub(last_constant, get_const_int(1));
    pos = Builder.CreateSExt(pos, llvm::Type::getInt64Ty(TheContext));
    std::cout << "Array Index Generated\n";
    auto elem_address = Builder.CreateGEP(arr_address, pos);
    if (is_lvalue) {
        last_constant = elem_address;
        elem_address->getType()->print(llvm::errs());
        std::cout << "ArrayRef Address Generated\n";
    } else {
        last_constant = Builder.CreateLoad(elem_address);
        std::cout << "ArrayRef Value Generated\n";
    }
}

void CodegenVisitor::visit(Program& node) {
    std::cout << "Generating Program\n";

    //built in functions
    std::vector<llvm::Type*> arg_types;
    arg_types.push_back(llvm::Type::getInt64Ty(TheContext));
    auto ft = llvm::FunctionType::get(llvm::Type::getInt8PtrTy(TheContext), arg_types, false);
    llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "malloc", TheModule.get());
    //
    std::vector<llvm::Type*> arg_types2;
    arg_types2.push_back(llvm::Type::getInt32Ty(TheContext));
    auto ft2 = llvm::FunctionType::get(llvm::Type::getInt32Ty(TheContext), arg_types2, false);
    llvm::Function::Create(ft2, llvm::Function::ExternalLinkage, "putchar", TheModule.get());
    //
    for (auto& n : node.program) {
        n->accept(*this);
    }
    std::cout << "Program Generated\n";
}

void CodegenVisitor::visit(Statements& node) {
    std::cout << "parsing statements\n";
    for (auto& n : node.statements) {
        if (!n) {
            std::cout << "Node is nullptr\n";
        }
        n->accept(*this);
    }
    std::cout << "Statements generated\n";
}

void CodegenVisitor::visit(Void& node) {
    std::cout << "Generating Void\n";
}

void CodegenVisitor::visit(Undefined& node) {
    std::cout << "Generating Undefined\n";
}

void CodegenVisitor::visit(Cast& node)
{
    std::cout << "Generating Cast\n";
    std::cout << "Cast to " << (int)node.type->type << "\n";
    if (node.type->type == types::Integer) {
        std::cout << "Casting to integer\n";
        node.value->accept(*this);
        if (node.value->type->type == types::Real) {
            std::cout << "Casting real to integer\n";
            last_constant = Builder.CreateFPToSI(last_constant, get_type(node.type));
        } else if (node.value->type->type == types::Boolean) {
            std::cout << "Casting boolean to integer\n";
            last_constant = Builder.CreateZExt(last_constant, get_type(node.type));
        }
    } else if (node.type->type == types::Real) {
        std::cout << "Casting to Real\n";
        node.value->accept(*this);
        if (node.value->type->type == types::Integer) {
            std::cout << "Casting integer to real\n";
            last_constant = Builder.CreateSIToFP(last_constant, get_type(node.type));
        } else if (node.value->type->type == types::Boolean) {
            std::cout << "Casting boolean to real\n";
            last_constant = Builder.CreateUIToFP(last_constant, get_type(node.type));
        }
    } else if (node.type->type == types::Boolean) {
        std::cout << "Casting to Boolean\n";
        node.value->accept(*this);
        if (node.value->type->type == types::Integer) {
            std::cout << "Casting integer to boolean\n";
            last_constant = Builder.CreateICmpNE(last_constant, get_const_int(0));
        }
    }
    std::cout << "Cast Generated\n";
}
