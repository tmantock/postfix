#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using std::cout;
using std::endl;
using std::stack;
using std::string;
using std::vector;
using std::istringstream;
using std::getline;
using std::find_if;
using std::stoi;
using std::to_string;

int precedence(char);
bool is_operator(string&);
bool is_number(const string&);
bool string_is_char(string&, char);
bool valid_parenthesis(string&);
vector<string> string_split(string&, char);
string infix_to_postfix(string&);
string evaluate_postfix_expression(vector<string>&);

int main() {
    string input = "5 + (6 * 3)";

    if (!valid_parenthesis(input)) {
        cout << "Invalid expression passed. Please check parenthesis order." << endl;

        return 1;
    }

    string postfix = infix_to_postfix(input);
    vector<string> split_postfix = string_split(postfix, ' ');

    cout << "The result of " << input << " is: " << evaluate_postfix_expression(split_postfix) << endl;
}

bool valid_parenthesis(string& input) {
    stack<char> stk;

    for (char c : input) {
        if (c == '(' || c == '{' || c == '[') {
            stk.push(c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (stk.empty()) return false;

            if ((c == ')' && stk.top() == '(') || (c == '}' && stk.top() == '{') || (c == '}' && stk.top() == '{')) {
                stk.pop();
            } else {
                return false;
            }
        }
    }

    return stk.empty();
}

vector<string> string_split(string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream token_stream(str);

    while (getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

string infix_to_postfix(string& input) {
    string postfix;
    stack<char> stk;

    for (char c : input) {
        if (isspace(c)) {
            stk.push(c);
            postfix += ' ';
        } else if (c >= '0' && c <= '9') {
            postfix += c;
        } else if (c == '(') {
            stk.push(c);
        } else if (c == ')') {
            while (!stk.empty() && stk.top() != '(') {
                char n = stk.top();
                stk.pop();
                postfix += n;
            }

            if (!stk.empty() && stk.top() == '(') {
                stk.pop();
            }
        } else {
            while (!stk.empty() && precedence(c) <= precedence(stk.top())) {
                char n = stk.top();
                stk.pop();
                postfix += n;
            }

            stk.push(c);
        }
    }

    while (!stk.empty()) {
        char n = stk.top();
        stk.pop();
        postfix += n;
    }

    return postfix;
}

string evaluate_postfix_expression(vector<string>& postfix) {
    stack<string> stk;
    int operation = 0;

    for (string str : postfix) {
        if (is_number(str)) {
            stk.push(str);
        } else if (is_operator(str)) {
            if (stk.empty()) {
                throw "Invalid Mathematical expression";
            }

            int operand_two = stoi(stk.top());
            stk.pop();
            int operand_one = stoi(stk.top());
            stk.pop();

            if (string_is_char(str, '-')) {
                operation = operand_one - operand_two;
            } else if (string_is_char(str, '+')) {
                operation = operand_one + operand_two;
            } else if (string_is_char(str, '*')) {
                operation = operand_one * operand_two;
            } else if (string_is_char(str, '/')) {
                operation = operand_one / operand_two;
            }

            stk.push(to_string(operation));
        }
    }

    return stk.top();
}

int precedence(char c) {
    if (c == '^') return 3;
    else if (c == '*' || c == '/') return 2;
    else if (c == '+' || c == '-') return 1;

    return -1;
}

bool is_operator(string& s) {
    return string_is_char(s, '+') || string_is_char(s, '-') || string_is_char(s, '*') || string_is_char(s, '/');
}

bool string_is_char(string& s, char c) {
    return !s.empty() && s.size() == 1 && s.find(c) != string::npos;
}

bool is_number(const string& s) {
    return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}