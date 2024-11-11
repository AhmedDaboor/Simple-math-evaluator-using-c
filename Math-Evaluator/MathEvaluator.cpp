#include <iostream>
#include <cctype>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

const string FileName = "inputExpression.txt";

void displayError(string txt = "Unvalid expression ...!") {
    cout << txt << endl;
    exit(EXIT_FAILURE);
}

bool isArithmeticOperator(char c) {
    
    return c == '*' || c == '/' || c == '+' || c == '-';
}

double ArithmeticOperator(char c, double Number1, double Number2) {
    
    switch (c) {
    case '*': return Number1 * Number2;
    case '/': return Number1 / Number2;
    case '+': return Number1 + Number2;
    case '-': return Number1 - Number2;
    default: return 0;
    }
}

int findIndex(vector<string>& ExpressionElement, string target) {
    
    vector<string>::iterator it = find(ExpressionElement.begin(), ExpressionElement.end(), target);
    
    if (it != ExpressionElement.end())
        return distance(ExpressionElement.begin(), it);
    
    return -1;
}

double PerformCalculation(vector<string>& ExpressionElement) {  
    

    while (findIndex(ExpressionElement, "*") != -1 || findIndex(ExpressionElement, "/") != -1) {
        
        int i = findIndex(ExpressionElement, "*"); 
        
        if (i == -1) 
            i = findIndex(ExpressionElement, "/");
        
        double temp = ArithmeticOperator(ExpressionElement[i][0], stod(ExpressionElement[i - 1]), stod(ExpressionElement[i + 1]));
        
        ExpressionElement.erase(ExpressionElement.begin() + (i - 1), ExpressionElement.begin() + (i + 2));
        ExpressionElement.insert(ExpressionElement.begin() + (i - 1), to_string(temp));
    }

    while (findIndex(ExpressionElement, "+") != -1 || findIndex(ExpressionElement, "-") != -1) {
        
        int i = findIndex(ExpressionElement, "+");
        
        if (i == -1) 
            i = findIndex(ExpressionElement, "-");
        
        double temp = ArithmeticOperator(ExpressionElement[i][0], stod(ExpressionElement[i - 1]), stod(ExpressionElement[i + 1]));
        
        ExpressionElement.erase(ExpressionElement.begin() + (i - 1), ExpressionElement.begin() + (i + 2));
        ExpressionElement.insert(ExpressionElement.begin() + (i - 1), to_string(temp));
    }

    return stod(ExpressionElement[0]);
}

double EvaluateParentheses(string Expression) {
    
    while (Expression.find('(') != string::npos) {
        
        size_t Begin = Expression.rfind('(');    //   "1 + ((2*3)/2 ) + (3*4) / 6".
        size_t End = Expression.find(')', Begin);
        
        if (Begin == string::npos || End == string::npos) {
            displayError();
        }


        string subExp = Expression.substr(Begin + 1, End - Begin - 1);
        
        vector<string> ExpressionElement;
        string number = "";
        
        for (char& c : subExp) {
            
            if (c == ' ') continue;
            
            else if (isdigit(c) || c == '.') {
                
                number += c;
            }
            
            else if (isArithmeticOperator(c)) {
                
                if (!number.empty()) {
                    
                    ExpressionElement.push_back(number);
                    number.clear();
                }
                
                else {
                    displayError();
                }

                ExpressionElement.push_back(string(1, c));
            }
            
            else {
                displayError();
            }
        }
        
        if (!number.empty()) 
            ExpressionElement.push_back(number);
        
        else {
            displayError();
        }

        double subResult = PerformCalculation(ExpressionElement);
        Expression.replace(Begin, End - Begin + 1, to_string(subResult));
    }
    
    vector<string> ExpressionElement;
    string number = "";
    
    for (char& c : Expression) {
        
        if (c == ' ') continue;
        
        else if (isdigit(c) || c == '.') {
            
            number += c;
        }
        
        else if (isArithmeticOperator(c)) {
            
            if (!number.empty()) {
                
                ExpressionElement.push_back(number);
                number.clear();
            }
            
            else {
                displayError();
            }
            ExpressionElement.push_back(string(1, c));
        }
        
        else {
            displayError();
        }
    }
    
    if (!number.empty()) 
        ExpressionElement.push_back(number);
    
    else {
        displayError();
    }

    return PerformCalculation(ExpressionElement);
}

vector<string> ReadTheFunctionFromFile() {
    
    fstream MyFile;
    string Text = "";
    vector<string> Expressions;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        while (getline(MyFile, Text))
            Expressions.push_back(Text);

        MyFile.close();
    }
    return Expressions;
}

void start() {

    vector<string> expressions = ReadTheFunctionFromFile();
    double result = 0;

    for (const string& exp : expressions) {

        result = EvaluateParentheses(exp);

            cout << "Result: " << result << endl;
    }

    cin.get();

}

int main() {
    
    start();

    return 0;
}
