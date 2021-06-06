#pragma once

#include <parse/ast.h>

namespace Frost::Gen{


class Executor {
public:
    virtual void exec() = 0;
private:
};

class ASTExecutor : public Executor, public Parse::ASTVisitor{};

class FIRExecutor : public Executor{};
}