#include "forPostfix.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include "forPythonGraphs.h"

using namespace std;

int main() {
    ifstream archivo("../datos.txt");
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo." << endl;
        return 1;
    }

    string linea;
    stack<string> postfixTxt;

    while (getline(archivo, linea)) {
        if(verify(linea)){
            linea = replaceUnicodeCharacters(linea);
            cout << "Expresión Regular en Infix: " << linea << endl;
            string postfix = replaceUnicodeCharacters3(infixToPostfix(linea));
            cout << "Expresión Regular en Postfix: " << postfix << endl;
            cout << "-------------------------" << endl;

            postfixTxt.push(postfix);
        }

    }


    archivo.close();

    while (!postfixTxt.empty()) {
        cout<<postfixTxt.top()<<endl;
        calculateSpace(postfixTxt.top());
        postfixTxt.pop();
    }

    return 0;
}
