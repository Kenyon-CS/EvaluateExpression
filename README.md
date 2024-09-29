# EvaluateExpression
C++ program to evaluate an numeric expression

1. Tokenizer (`tokenize` function):
  - Converts the input string into a `vector` of tokens.
  - Handles multi-digit numbers and decimal points.
  - Recognizes operators `(+, -, *, /)` and parentheses `((, ))`.
  - Implements implied multiplication by inserting a * operator when necessary (e.g., between a number and ().
2. Parser (`Parser` class):
  - Uses recursive descent parsing to evaluate the expression.
  - Functions:
    - `parseExpression`: Handles addition and subtraction.
    - `parseTerm`: Handles multiplication and division.
    - `parseFactor`: Handles numbers, parentheses, and unary minus.
  - Throws exceptions for invalid syntax or unexpected tokens.
3. Main Function:
  - Reads the input expression from the user.
  - Calls the tokenizer and parser to compute the result.
  - Outputs the result or an error message if an exception occurs.

## Note:
  - The program assumes that division by zero and other mathematical errors are handled by C++'s default behavior.
  - The tokenizer and parser are designed to be robust and handle a variety of valid mathematical expressions.
  - Error messages are provided for invalid input to aid in debugging.
