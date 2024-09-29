#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>

enum TokenType { NUMBER, OPERATOR, PAREN };

struct Token {
    TokenType type;
    double value; // For numbers
    char op;      // For operators and parentheses
};

std::vector<Token> tokenize(const std::string& expr) {
    std::vector<Token> tokens;
    size_t i = 0;
    size_t n = expr.length();

    while (i < n) {
        if (isspace(expr[i])) {
            i++;
            continue;
        }

        if (isdigit(expr[i]) || expr[i] == '.') {
            size_t j = i;
            while (j < n && (isdigit(expr[j]) || expr[j] == '.')) j++;
            double num = std::stod(expr.substr(i, j - i));
            tokens.push_back({ NUMBER, num, '\0' });
            i = j;
        } else if (expr[i] == '+' || expr[i] == '-' ||
                   expr[i] == '*' || expr[i] == '/') {
            tokens.push_back({ OPERATOR, 0, expr[i] });
            i++;
        } else if (expr[i] == '(' || expr[i] == ')') {
            tokens.push_back({ PAREN, 0, expr[i] });
            i++;
        } else {
            throw std::runtime_error(std::string("Invalid character: ") + expr[i]);
        }

        // Handle implied multiplication
        if (!tokens.empty() && i < n) {
            Token& lastToken = tokens.back();
            char nextChar = expr[i];

            if ((lastToken.type == NUMBER || (lastToken.type == PAREN && lastToken.op == ')')) &&
                (nextChar == '(' || isdigit(nextChar))) {
                tokens.push_back({ OPERATOR, 0, '*' });
            }
        }
    }
    return tokens;
}

class Parser {
    std::vector<Token> tokens;
    size_t pos;

    double parseExpression() {
        double value = parseTerm();
        while (pos < tokens.size() &&
               (tokens[pos].type == OPERATOR && (tokens[pos].op == '+' || tokens[pos].op == '-'))) {
            char op = tokens[pos++].op;
            double rhs = parseTerm();
            if (op == '+') value += rhs;
            else value -= rhs;
        }
        return value;
    }

    double parseTerm() {
        double value = parseFactor();
        while (pos < tokens.size() &&
               (tokens[pos].type == OPERATOR && (tokens[pos].op == '*' || tokens[pos].op == '/'))) {
            char op = tokens[pos++].op;
            double rhs = parseFactor();
            if (op == '*') value *= rhs;
            else value /= rhs;
        }
        return value;
    }

    double parseFactor() {
        if (pos >= tokens.size()) {
            throw std::runtime_error("Unexpected end of expression");
        }

        Token token = tokens[pos++];

        if (token.type == NUMBER) {
            return token.value;
        } else if (token.type == PAREN && token.op == '(') {
            double value = parseExpression();
            if (pos >= tokens.size() || tokens[pos].type != PAREN || tokens[pos].op != ')') {
                throw std::runtime_error("Missing closing parenthesis");
            }
            pos++; // Consume ')'
            return value;
        } else if (token.type == OPERATOR && token.op == '-') {
            // Unary minus
            return -parseFactor();
        } else {
            throw std::runtime_error("Invalid factor");
        }
    }

public:
    Parser(const std::vector<Token>& tokens_) : tokens(tokens_), pos(0) {}

    double parse() {
        double value = parseExpression();
        if (pos != tokens.size()) {
            throw std::runtime_error("Unexpected tokens at end of expression");
        }
        return value;
    }
};

int main() {
    std::string expr;
    std::cout << "Enter a mathematical expression:\n";
    std::getline(std::cin, expr);

    try {
        std::vector<Token> tokens = tokenize(expr);
        Parser parser(tokens);
        double result = parser.parse();
        std::cout << "Result: " << result << '\n';
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
    return 0;
}
