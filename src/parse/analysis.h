#pragma once

#include <unit.h>
#include <types.h>
#include <type.h>
#include <parse/ast.h>
#include <symtable.h>

namespace Frost::Parse{

class AnalysisContext{};
class Analyser {
public:
    static Analyser create(Unit* unit, AST* ast){
        Analyser a;
        a.m_unit = unit;
        a.m_ast = ast;
        return a;
    }
    void analyse();
    Optional<Type> visit(AST* ast, AnalysisContext);
    Optional<Type> visit(ProgramAST* program_ast, AnalysisContext);
    Optional<Type> visit(DeclAST* decl_ast, AnalysisContext);
    Optional<Type> visit(BinOpAST* bin_op_ast, AnalysisContext);
    Optional<Type> visit(VariableAST* variable_ast, AnalysisContext);
    Optional<Type> visit(LiteralAST* literal_ast, AnalysisContext);
private:
    Unit* m_unit;
    AST* m_ast;
    SymTable<Type> m_sym_table;
};

}