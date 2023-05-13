#include "Python3BaseListener.h"
#include<vector>
#include <string>
#include <sstream>

void Python3BaseListener::enterFor_stmt(Python3Parser::For_stmtContext* forCtx) {


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
		if (child_text.find("print(") == 0) {
			start_pos = child_text.find("print") + 1;
			// found print satatement, convert to cout
			converted_str += "std::cout <<";
			converted_str += child_text.substr(start_pos + 5, child_text.size() - 8); //word "print" have len=5+2 for \n and ")" symblos 
			converted_str += "<< std::endl;\n";
			outputfilestr += converted_str;
		}
		childrens_text_vec.push_back(child->getText());
	}

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
	std::string ctx_txt = ctx->getText();
	std::string str_to_find;

	for (std::string method : declareted_methods_names) {
		str_to_find = method;
		str_to_find += "(";
		if (ctx_txt.find(str_to_find) != -1 and ctx_txt.find("defmain(") == -1 and ctx_txt != "main()" and ctx_txt != "main()\n") {
			if(ctx_txt.substr(ctx_txt.size() - 1,1)=="\n") ctx_txt.pop_back();// do not drop last symbol if not end of line
			outputfilestr += ctx_txt;
			outputfilestr += ";\n";
			break;
		}
	}
	//std::cout << "Statement context: " << ctx->getText() << std::endl;
	//std::cout << "Expr text: " << exprCtx->getText() << std::endl;
}

void Python3BaseListener::enterAnnassign(Python3Parser::AnnassignContext* ctx) {
	//std::cout << "Annassign context: " << ctx->getText() << std::endl;
	
	//std::cout << "Buffered context: " << buffered_expression << std::endl;
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
	//std::cout << "Statement context end: " << std::endl;
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
	//std::cout << "If context: " << ctx->getText() << std::endl;
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
		else if ((child_text.find("print(") == 0)or(child_text.find("\n    print(") == 0)) {
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


void Python3BaseListener::enterFuncdef(Python3Parser::FuncdefContext* ctx) {
	auto childs = ctx->children;
	std::vector<std::string> childrens_text_vec;
	std::string converted_str, child_text;
	int start_pos;

	for (auto child : childs) {
		child_text = child->getText();
		childrens_text_vec.push_back(child_text);
	}

	//construct c++ func type
	//python : def _func_name_ (_args_list_) -> _return_type_ :
	//c++: _return_type_ _func_name_ (_args_list_){

	if (childrens_text_vec[4] == "str") converted_str += "std::string ";
	else if (childrens_text_vec[4] == "None") converted_str += "void ";
	else if (childrens_text_vec[4] == "list") converted_str += "auto ";
	else converted_str += childrens_text_vec[4];

	converted_str += " ";
	converted_str += childrens_text_vec[1];
	converted_str += " (";

	//remove ( brackers from arglist, then split args by comma
	std::string raw_list = childrens_text_vec[2].substr(1, childrens_text_vec[2].size() - 2);;
	
	std::vector<std::string> arg_list;
	std::string arg;
	std::stringstream ss(raw_list);
	while (std::getline(ss, arg, ','))
	{
		arg_list.push_back(arg);
	}
	std::string arg_name, arg_type;
	
	for (std::string curr_arg : arg_list) {
		arg_name = curr_arg.substr(0, curr_arg.find(":"));
		arg_type = curr_arg.substr(curr_arg.find(":") + 1, curr_arg.size() - arg_name.size() - 1);
		if (arg_type == "str") converted_str += "std::string ";
		else if (arg_type == "list") converted_str += "auto ";
		else converted_str += arg_type;
		converted_str += " ";
		converted_str += arg_name;
		converted_str += ", ";
	}
	if (childrens_text_vec[2] != "()") {
		converted_str.pop_back();
		converted_str.pop_back();
	}
	converted_str += "){\n";
	outputfilestr += converted_str;
	declareted_methods_names.insert(childrens_text_vec[1]);
}
void Python3BaseListener::exitFuncdef(Python3Parser::FuncdefContext* ctx) {
	outputfilestr += "\n}\n";
}
void Python3BaseListener::enterReturn_stmt(Python3Parser::Return_stmtContext* ctx) {
	std::vector<std::string> childrens_text_vec;
	std::string converted_str, child_text;
	int start_pos;
	auto childs = ctx->children;

	for (auto child : childs) {
		child_text = child->getText();
		converted_str += child_text;
		converted_str += " ";
		std::cout << child_text << std::endl;
	}
	std::cout << "enter funcdef end" << std::endl;
	converted_str.pop_back();
	outputfilestr += converted_str;
	outputfilestr += ";";
}