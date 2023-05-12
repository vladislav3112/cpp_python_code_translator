#include "Python3BaseListener.h"
#include<vector>
#include <string>

void Python3BaseListener::enterFor_stmt(Python3Parser::For_stmtContext* forCtx) {


	Python3Parser::ExprContext* exprCtx = forCtx->testlist()->test()[0]->or_test()[0]->and_test()[0]->not_test()[0]->comparison()->expr()[0];
	auto childs = forCtx->children;

	std::vector<std::string> childrens_text_vec;

	for (auto child : childs) {
		childrens_text_vec.push_back(child->getText());
	}
	//std::cout << "End of childrens list: " << std::endl;
	
	std::string converted_str = "for(auto ";
	std::string range_num;
	int start_pos, end_pos;
	

	//Trying to convert foreach or range loop: 
	if (childs.size() > 4) {
		if (childrens_text_vec[3].find("range") == -1) {
			converted_str += childrens_text_vec[1];
			converted_str += ": ";
			converted_str += childrens_text_vec[3];
			converted_str += ")";
			converted_str += "{\n";
		}
		else {
			start_pos = childrens_text_vec[3].find("(") + 1;
			end_pos = childrens_text_vec[3].find(")");
			range_num = childrens_text_vec[3].substr(start_pos, end_pos - start_pos);
			converted_str += childrens_text_vec[1];//i = 0; i < 5; i++)
			converted_str += "= 0; ";
			converted_str += childrens_text_vec[1];
			converted_str += " < ";
			converted_str += range_num;
			converted_str += "; ";
			converted_str += childrens_text_vec[1];
			converted_str += "++)";
			converted_str += "{\n";
		}
		std::cout << "Converted loop: " << converted_str << std::endl;
		outputfilestr += converted_str;
	}
	else
	{
		std::cout << "Cannot convert loop: " << std::endl;
	}

}
void Python3BaseListener::exitFor_stmt(Python3Parser::For_stmtContext* forCtx) {
	outputfilestr += "\n}\n";
}

void Python3BaseListener::enterWhile_stmt(Python3Parser::While_stmtContext* forCtx) {
	std::cout << "While statement text: " << forCtx->getText() << std::endl;

	// Display text of expr context
	//Python3Parser::ExprContext* exprCtx = forCtx->test()[0]->or_test()[0]->and_test()[0]->not_test()[0]->comparison()->expr()[0];
	//std::cout << "Expr text: " << exprCtx->getText() << std::endl;

	// Display text of atom context
	//Python3Parser::AtomContext* atomCtx = exprCtx->xor_expr()[0]->and_expr()[0]->shift_expr()[0]->arith_expr()[0]->term()[0]->factor()[0]->power()->atom_expr()->atom();
	//std::cout << "Atom text: " << atomCtx->getText() << std::endl;
}


//main body of loop or func
void Python3BaseListener::enterSuite(Python3Parser::SuiteContext* ctx) {
	//std::cout << "Suite context: " << ctx->getText() << std::endl;
	//std::cout << "Childrens: " << std::endl;
	auto childs = ctx->children;

	std::vector<std::string> childrens_text_vec;
	std::string converted_str, child_text;
	int start_pos;

	for (auto child : childs) {
		child_text = child->getText();
		//std::cout << child_text << std::endl;
		if (child_text.find("print") == 0) {
			start_pos = child_text.find("print") + 1;
			// found print satatement, convert to cout
			converted_str += "std::cout <<";
			converted_str += child_text.substr(start_pos + 5, child_text.size() - 8); //word "print" have len=5+2 for \n and ")" symblos 
			converted_str += "<< std::endl;\n";
			outputfilestr += converted_str;
		}
		childrens_text_vec.push_back(child->getText());
	}
	//std::cout << "End of childrens list: " << std::endl;
}

void Python3BaseListener::enterExpr(Python3Parser::ExprContext* ctx) {
	if (!is_augassigment_was_before) {
		buffered_expression = ctx->getText();
	}
	else {
		outputfilestr += buffered_expression;
		outputfilestr += ctx->getText();
		outputfilestr += ";";
		is_augassigment_was_before = false;
	}
}
void Python3BaseListener::enterStmt(Python3Parser::StmtContext* ctx) {
	//std::cout << "Statement context: " << ctx->getText() << std::endl;
	//std::cout << "Expr text: " << exprCtx->getText() << std::endl;
}

void Python3BaseListener::enterAnnassign(Python3Parser::AnnassignContext* ctx) {
	std::cout << "Annassign context: " << ctx->getText() << std::endl;
	
	std::cout << "Buffered context: " << buffered_expression << std::endl;
	auto childs = ctx->children;

	std::vector<std::string> childrens_text_vec;
	std::string converted_str, child_text;
	int start_pos;

	for (auto child : childs) {
		child_text = child->getText();
		childrens_text_vec.push_back(child_text);
		std::cout << child_text << std::endl;
	}
	
	//different types handler
	std::string list_elems_type;
	if (childrens_text_vec[1] == "list") {
		converted_str += "std::vector<";
		if ((childrens_text_vec[3].find("'") != -1) or (childrens_text_vec[3].find('"') != -1)) {
			list_elems_type = "std::string";
		}
		else if (childrens_text_vec[3].find(".") != -1) {
			list_elems_type = "double";
		}
		else {
			list_elems_type = "int";
		}
		converted_str += list_elems_type;
		converted_str += "> ";
		converted_str += buffered_expression;
		converted_str += "= {";
		std::string list_elems = childrens_text_vec[3].substr(1, childrens_text_vec[3].size() - 2);
		converted_str += list_elems;
		converted_str += "};\n";
	
	}
	else{
		converted_str += childrens_text_vec[1];
		converted_str += " ";
		converted_str += buffered_expression;
		converted_str += "= ";	
		converted_str += childrens_text_vec[3];
		converted_str += ";\n";
	}
	outputfilestr += converted_str;
	std::cout << "Statement context end: " << std::endl;
}

void Python3BaseListener::enterAugassign(Python3Parser::AugassignContext* ctx) {
	std::cout << "Augassign context: " << ctx->getText() << std::endl;

	std::cout << "buffered_expression: " << buffered_expression << std::endl;

	auto childs = ctx->children;
	std::vector<std::string> childrens_text_vec;
	std::string converted_str, child_text;

	for (auto child : childs) {
		child_text = child->getText();
		childrens_text_vec.push_back(child_text);
		std::cout << child_text << std::endl;
	}

	std::cout << "Statement context end: " << std::endl;
	buffered_expression += childrens_text_vec[0];
	is_augassigment_was_before = true;
}

void Python3BaseListener::enterIf_stmt(Python3Parser::If_stmtContext* ctx) {
	std::cout << "If context: " << ctx->getText() << std::endl;
	//std::cout << "Childrens: " << std::endl;
	auto childs = ctx->children;

	std::vector<std::string> childrens_text_vec;
	std::string converted_str, child_text;
	int start_pos;
	
	converted_str += "if (";
	int i = 0;
	for (auto child : childs) {
		if (i == 0) { i += 1; continue; }
		child_text = child->getText();
		if (child_text == ":") {
			converted_str += "){\n";
			break;
		}
		else if ((child_text.find("print") == 0)or(child_text.find("\n    print") == 0)) {
			continue;
		}
		else {
			converted_str += child_text;
		}
		child_text = child->getText();
		//std::cout << child_text << std::endl;
		// found print satatement, convert to cout
		
		 //word "print" have len=5+2 for \n and ")" symblos 
		childrens_text_vec.push_back(child->getText());
	}
	outputfilestr += converted_str;

}
void Python3BaseListener::exitIf_stmt(Python3Parser::If_stmtContext* ctx) {
	outputfilestr += "\n}\n";
}