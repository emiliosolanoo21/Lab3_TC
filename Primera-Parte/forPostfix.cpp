#include <iostream>
#include <fstream>
#include <stack>
#include <map>
#include <string>
#include "forPostfix.h"
#include <regex>

using namespace std;

char contraParte(const char caracter){
    switch (caracter) {
        case '(':
            return ')';
            break;
        case '[':
            return ']';
            break;
        case '{':
            return '}';
        case ')':
            return '(';
            break;
        case ']':
            return '[';
            break;
        case '}':
            return '{';
        default:
            return ' ';
    }
}

void mostrarPila(const stack<char>& pila) {
    // Creamos una pila auxiliar para almacenar temporalmente los elementos
    stack<char> pilaAux = pila;
    cout<< "Datos en la pila: [";
    // Mostramos los elementos de la pila auxiliar
    while (!pilaAux.empty()) {


        cout << pilaAux.top();
        pilaAux.pop();
    }

    cout<<"]"<< endl;
}

void mostrarPila(const stack<string>& pila) {
    // Creamos una pila auxiliar para almacenar temporalmente los elementos
    stack<string> pilaAux = pila;
    cout<< "Datos en la pila: [";
    // Mostramos los elementos de la pila auxiliar
    while (!pilaAux.empty()) {


        cout << pilaAux.top()<<',';
        pilaAux.pop();
    }

    cout<<"]"<< endl;
}

// Función para determinar si un token es un operador
bool isOperator(const char& token) {

    static const string operators = "|?,+*^";
    return operators.find(token) != string::npos;
}

bool isBinary(const char& token) {
    static const string operators = "^|";
    return operators.find(token) != string::npos;
}

// Función para asignar prioridades a los operadores
// Función para asignar prioridades a los operadores
int precedence(const char& op) {
    static const map<char, int> prec = {
        {'(', 1}, {')', 1},{'[', 1}, {']', 1},{'{', 1}, {'}', 1}, {'|', 2}, {',', 3}, {'?', 4}, {'*', 4}, {'+', 4}, {'^', 5}, {'∗', 4}
    };
    auto it = prec.find(op);
    if (it != prec.end()) {
        return it->second;
    }
    // Si el operador no se encuentra en el mapa, devolvemos una prioridad por defecto
    return -1;
}
// Formatear la expresión regular para incluir concatenaciones explícitas
string formatRegEx(const string linea) {
    string resultado;
    string vresultado;
    bool completed= false;
    stack<string> group;
    string grouped ="";
    bool ingroup = false;
    bool isescaped =false;


    for (size_t i = 0; i < linea.length(); ++i) {
        char c1 = linea[i];
        if (i + 1 < linea.length()) {
            char c2 = linea[i + 1];

            if(c1!='?' && c1 != '+'){
                resultado += c1;
            }


            // Verificar si el caracter actual es '\' (carácter de escape)
            if (c1 == '\\') {
                resultado += c2; // Agregar el siguiente caracter sin formato
                ++i; // Avanzar al siguiente caracter
                resultado += ',';
                isescaped = true;
                continue;
            }else if(isescaped){
                isescaped = false;
            }

            if(c1 == '['){
                completed = true;
            }

            if((c1 == '(' || c1 =='[' || c1=='{' )&& !isescaped){

                if(ingroup){
                    resultado.erase(resultado.size()-1);
                    group.push(resultado);
                    resultado="";
                    resultado+=c1;
                }else {
                    ingroup = true;
                }
            }

            if(!ingroup && !isescaped){
                if(c2=='?'){
                    resultado= "("+resultado+"|ε)";
                }
                if(c2=='+'){
                    resultado= resultado +','+resultado+'*';
                }
            }

            if((c1 == ')' || c1 ==']' || c1=='}' ) && !isescaped){

                if(c2=='?'){
                    resultado= "("+resultado+"|ε)";
                }

                if(c2=='+'){
                    resultado= resultado +','+resultado+'*';
                }

                ingroup = !group.empty();

                if(!group.empty()){
                    resultado = group.top()+resultado;
                    group.pop();
                }
            }


            if ((c1 != '(' && c2 != ')' && !isOperator(c2) && !isBinary(c1)) && !completed) {
                resultado += ',';
            }
            if(c2==']'){
                completed =false;
            }

            if(!ingroup){
                vresultado+=resultado;
                resultado="";
            }
        }
    }

    if(linea[linea.length()-1]!='?' && linea[linea.length()-1]!='+'){
        vresultado += linea[linea.length() - 1];
    }
    return vresultado;
}


string infixToPostfix(string regex) {
    string postfix;
    stack<char> pilaPostfix;
    bool escapeChar = false;
    cout<<"Se convierte reformatea:"<<endl;
    string formattedRegEx = formatRegEx(regex);
    cout<<"Expresion infix: "<<formattedRegEx<<endl;
    bool completed = false;

    for (char caracter : formattedRegEx) {
        switch (caracter) {
            case '\\':
                escapeChar =true;
                break;
            case ' ':
                escapeChar =true;
                break;
            case '[':
                if(!escapeChar){
                    completed =true;
                    postfix+=caracter;
                }
            case '{':
            case '(':
                if(!escapeChar){
                    pilaPostfix.push(caracter);
                    break;
                }
            case ']':
                if(!escapeChar){
                    completed =false;
                    postfix+=caracter;
                }
            case '}':

            case ')':

                if(!escapeChar){
                    while (pilaPostfix.top() != contraParte(caracter)) {
                        postfix += pilaPostfix.top();
                        pilaPostfix.pop();
                    }
                    pilaPostfix.pop(); // Eliminar el paréntesis izquierdo '(' de la pila
                    break;
                }

            default:
                if((!isOperator(caracter) && !isBinary(caracter)) || escapeChar || completed){
                    escapeChar = false;
                    postfix+=caracter;
                }else {
                    while (!pilaPostfix.empty()) {
                        char caracterTomado = pilaPostfix.top();

                        int precedenciaCaracterTomado = precedence(caracterTomado);
                        int precedenciaCaracterActual = precedence(caracter);
                        if (precedenciaCaracterTomado >= precedenciaCaracterActual) {
                            postfix += pilaPostfix.top();
                            pilaPostfix.pop();
                        } else {
                            break;
                        }
                    }
                    pilaPostfix.push(caracter);
                }


                break;
        }
    }

    while (!pilaPostfix.empty()) {
        postfix += pilaPostfix.top();
        pilaPostfix.pop();
    }

    return postfix;
}


string replaceUnicodeCharacters(const string& input) {
    // Caracteres Unicode a reemplazar
    map<string, string> unicodeMap = {
        {"𝑎", "a"},
        {"𝑏", "b"},
        {"𝑐", "c"},
        {"𝑑", "d"},
        {"𝑒", "e"},
        {"𝑓", "f"},
        {"𝑔", "g"},
        {"𝑎𝑧", "az"},
        {"𝐴𝑍", "AZ"},
        {"∗", "*"}, // Reemplazar el carácter unicode '∗' con '*'
    };

    string output = input;
    for (const auto& entry : unicodeMap) {
        regex pattern(entry.first);
        output = regex_replace(output, pattern, entry.second);
    }

    return output;
}



bool verify(string linea){
    stack<char> pila_regex;
    bool test= true;
    cout<<">>Linea a evaluar: "<<linea<<endl;
            for (char caracter : linea) {
                if(caracter=='('|| caracter == '[' || caracter =='{'){
                    pila_regex.push(caracter);
                }else if (caracter==')'|| caracter == ']' || caracter =='}') {

                    if(!pila_regex.empty()){
                        if(contraParte(pila_regex.top())==caracter){
                            pila_regex.pop();
                        }else {
                            test = false;
                            break;
                        }
                    }else {
                        test =false;
                        break;
                    }
                }

            }
            if(!pila_regex.empty()){
                 cout<<endl;
                 cout<<"*****Lastima, la expresion no esta balanceada. Ya que la pila no esta vacia.****"<< endl;
                 while (!pila_regex.empty()) {
                     pila_regex.pop();
                 }
                 cout<<endl;
                 return false;
            } else if(!test){
                cout<<endl;
                 cout<<"*****Lastima, la expresion no esta balanceada.:(****"<< endl;
                 while (!pila_regex.empty()) {
                     pila_regex.pop();
                 }
                 cout<<endl;
                 return false;
            }else {
                cout<<endl;
                cout<<"////La linea esta correctamente balanceada///"<<endl;
                cout<<endl;
            }
            return true;

}
