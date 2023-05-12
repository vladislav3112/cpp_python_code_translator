
#include "Python3BaseVisitor.h"

antlrcpp::Any Python3BaseVisitor::visitSuite(Python3Parser::SuiteContext* ctx) {
    std::cout << "Suite text: " << ctx->getText() << std::endl;
    return visitChildren(ctx);
}

antlrcpp::Any Python3BaseVisitor::visitFuncdef(Python3Parser::FuncdefContext* ctx) {
    std::cout << "Funcdef text: " << ctx->getText() << std::endl;
    return visitChildren(ctx);
}

antlrcpp::Any Python3BaseVisitor::visitIf_stmt(Python3Parser::If_stmtContext* ctx) {
    std::cout << "If stmt text: " << ctx->getText() << std::endl;
    return visitChildren(ctx);
}
