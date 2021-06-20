#pragma once

#include <unit.h>
#include <types.h>
#include <type.h>
#include <parse/ast.h>
#include <symtable.h>

namespace Frost::Parse{

class AnalysisCtx{
public:
    enum class Scope{
        GLOBAL,
        FN,
        BLOCK
    };
    Scope& scope(){
        return m_scope;
    }
    Frost::Type& expecting_type(){
        return m_expecting_type;
    }
private:
    Scope m_scope;
    Frost::Type m_expecting_type; // used for type inference
};


class Analyser {
public:
    static Analyser create(Unit* unit, AST* ast){
        Analyser a;
        a.m_unit = unit;
        a.m_ast = ast;
        return a;
    }
    void analyse();
    Optional<Type> visit(AST*, AnalysisCtx);
    Optional<Type> visit(ProgramAST*, AnalysisCtx);
    Optional<Type> visit(BlockAST*, AnalysisCtx);
    Optional<Type> visit(DeclAST*, AnalysisCtx);
    Optional<Type> visit(BinOpAST*, AnalysisCtx);
    Optional<Type> visit(VariableAST*, AnalysisCtx);
    Optional<Type> visit(LiteralAST*, AnalysisCtx);
    Optional<Type> visit(FnAST*, AnalysisCtx);
    Optional<Type> visit(StructAST*, AnalysisCtx);
private:
    Unit* m_unit;
    AST* m_ast;
    SymTable<Type> m_sym_table;
};

}