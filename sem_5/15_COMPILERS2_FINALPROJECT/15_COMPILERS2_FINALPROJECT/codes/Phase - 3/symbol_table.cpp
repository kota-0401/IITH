#include <bits/stdc++.h>
using namespace std;

extern char* yytext;

struct typerec {
    string name;
    int scope;
    string type;
    string eletype;
    int no_of_dim;
};

struct param {
    string name;
    string eletype;
};

struct f_unique {
    string name;
    vector<param> param_list;
    string return_type;
    string return_eletype;
    bool is_defined;
};

vector<f_unique> function_table;
vector<vector<typerec>> local_symbol_table(1);

vector<param> param_list;
vector<string> call_arg_list;
int scope = 0;
int add_function_value = 0;
int no_of_dim = 0;
string eletype = "";
string type = "simple";

/*void insert_type() {
	type = yytext;
}*/

void create_symbol_table() {
    vector<typerec> new_table;  // Create a new empty symbol table
    local_symbol_table.push_back(new_table);  // Add the new symbol table to the vector
    scope++;
}

void delete_symbol_table() {
    local_symbol_table.pop_back();  // Remove the last symbol table
    scope--;
}

string check_type_eletype(string name) {
    for (int i = 0; i < function_table.size(); i++) {
        if (function_table[i].name == name) {
            if(function_table[i].param_list.size() == call_arg_list.size()) {
                for (int j = 0; j < call_arg_list.size(); j++) {
                    if (function_table[i].param_list[j].eletype != call_arg_list[j]) {
                        break;
                    }
                }
                return function_table[i].return_eletype;
            }
        }
    }
    return "not_found";
}


bool type_checking(string from_type, string to_type) {
    // Define a set of valid type conversions for each type
    unordered_set<string> valid_conversions;

    if (from_type == "int") {
        valid_conversions = {"int", "float", "bool","char"};
    } 
    else if (from_type == "float") {
        valid_conversions = {"float", "int", "bool", "char"};
    } 
    else if (from_type == "string") {
        valid_conversions = {"string"};
    } 
    else if (from_type == "bool") {
        valid_conversions = {"bool", "int", "float", "char"};
    }
    else if (from_type == "char") {
        valid_conversions = {"char", "bool", "int", "float"};
    }
    else if (from_type == "point") {
        valid_conversions = {"point"};
    }
    else if (from_type == "line") {
        valid_conversions = {"line", "line_circle"};
    }
    else if (from_type == "circle") {
        valid_conversions = {"circle", "line_circle"};
    }
    else if (from_type == "parabola") {
        valid_conversions = {"parabola"};
    }
    else if (from_type == "ellipse") {
        valid_conversions = {"ellipse", "ellipse_hyperbola"};
    }
    else if (from_type == "hyperbola") {
        valid_conversions = {"hyperbola", "ellipse_hyperbola"};
    } 
    return valid_conversions.find(to_type) != valid_conversions.end();
}

char* string_to_char(string str){
    char* c = new char(str.length() + 1);
    strcpy(c,str.c_str());
    return c;
}

string charPointerToString(const char* charPointer) {
    if (charPointer) {
        return string(charPointer);
    } else {
        // Handle the case where charPointer is nullptr
        return string();
    }
}

// Function to check if a function is in the function table
int is_function_declared(string name, string eletype) {
    for (int j = 0; j < local_symbol_table[0].size(); j++) {
            typerec entry = local_symbol_table[0][j];
            if (entry.name == name) {
                return 2;
            }
    }
    for (const auto& func : function_table) {
        if (func.name == name && func.param_list.size() == param_list.size()) {
            bool params_match = true;

            // Check if the types of parameters match
            for (int i = 0; i < param_list.size(); i++) {
                if (func.param_list[i].eletype != param_list[i].eletype) {
                    params_match = false;
                    break;
                }
            }

            if (params_match) {
                if (func.return_eletype == eletype) {
                    return 1; // Function found in table
                }
                return 2; // ambiguating new declaration of function
            }
        }
    }
    return 0; // Function not found in table
}

f_unique* is_function_defined(string name, string eletype) {
    f_unique* matching_function = NULL;  // Pointer to the matching function
    for (int j = 0; j < local_symbol_table[0].size(); j++) {
            typerec entry = local_symbol_table[0][j];
            if (entry.name == name) {
                matching_function->return_type = "complex";
                return matching_function;
            }
    }
    for (auto& func : function_table) {
        if (func.name == name && func.param_list.size() == param_list.size()) {
            bool params_match = true;

            // Check if the types of parameters match
            for (int i = 0; i < param_list.size(); i++) {
                if (func.param_list[i].eletype != param_list[i].eletype) {
                    params_match = false;
                    break;
                }
            }
            matching_function = &func;
            return matching_function;
        }
    }
    return NULL; // Function not found in table
}

// Function to add a function to the function table
int add_function(char* a, char* b) {
    // Check if the function is already in the table
    string name = a;
    string eletype = b;
    int c = is_function_declared(name, eletype);
    if (c == 0) {
        // If not in the table, add the function
        f_unique new_function;
        new_function.name = name;
        new_function.param_list = param_list;
        new_function.return_type = "simple";
        new_function.return_eletype = eletype;
        new_function.is_defined = 0;
        function_table.push_back(new_function);
        return 0;
    }
    else if (c == 1) {
        return 1;// ambiguating new declaration of function
    }
    return 0;
}

int add_function_body(string name, string eletype) {
    // Check if the function is already in the table
    f_unique* func = is_function_defined(name, eletype);
    if (func == NULL) {
        // If not in the table, add the function
        f_unique new_function;
        func->name = name;
        func->param_list = param_list;
        func->return_type = "simple";
        func->return_eletype = eletype;
        func->is_defined = 1;
        function_table.push_back(new_function);
    }
    else {
        if (func->return_eletype == eletype) {
            if (func->is_defined == 0) {
                func->is_defined = 1;
            }
            else {
                return 0; // ambiguating new definition of function
            }
        }
        return 2; // ambiguating new declaration of function
    }
    return 1;
}

bool is_variable_declared(char* v) {
    string variable = v;
    if (local_symbol_table.back().empty()) {
        return false;  // No symbol tables or last symbol table is empty
    }

    const vector<typerec>& last_table = local_symbol_table.back();
    for (const auto& entry : last_table) {
        if (entry.name == variable) {
            return true;  // Variable found in the last symbol table
        }
    }

    return false;  // Variable not found in the last symbol table
}

int add_variable(string name) {
    // Check if the variable is already in the table
    if (is_variable_declared(name) == 0) {
        // If not in the table, add the variable
        vector<typerec>& last_table = local_symbol_table.back();
        last_table.push_back({name, scope, type, eletype, no_of_dim});
        return 0;
    }
    return 1; // ambiguating new declaration of variable
}

int insert_param(string name, string eletype) {
    if (add_variable(name)) {
        return 0;
    }
    no_of_dim++;
    param_list.push_back({name, eletype});
    return 1;
}

string get_type(string id) {
    for (int i = local_symbol_table.size() - 1; i >= 0; i--) {
        for (int j = 0; j < local_symbol_table[i].size(); j++) {
            typerec entry = local_symbol_table[i][j];
            if (entry.name == id) {
                return entry.eletype;
            }
        }
    }
    // If the variable is not found, return an error type
    return "error_type";
}