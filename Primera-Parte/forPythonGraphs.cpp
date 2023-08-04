#include "forPythonGraphs.h"
#include <iostream>
#include <string>
#include <Python.h>

using namespace std;

void draw_expression_tree(const std::string &postfix) {
    std::string python_code = R"(
import turtle

import tkinter as tk

def obtener_dimensiones_pantalla():
    root = tk.Tk()
    root.attributes("-fullscreen", True)
    ancho = root.winfo_screenwidth()
    altura = root.winfo_screenheight()
    root.destroy()
    return ancho, altura

# Estructura de Nodo del árbol
class Nodo:
    def __init__(self, valor):
        self.valor = valor
        self.izquierda = None
        self.derecha = None
expresion_postfix = ''

# Función para construir el árbol a partir de la expresión postfix
def construir_arbol(expresion):
    pila = []
    operadores = set(',*|')
    group = ""
    groped = False
    for simbolo in expresion:
        if simbolo == '[':
            group += simbolo
            groped = True
        elif simbolo == ']':
            group += simbolo
            groped = False
            nodo = Nodo(group)
            pila.append(nodo)
            group = ""
        elif groped:
            group += simbolo
        elif simbolo not in operadores:
            nodo = Nodo(simbolo)
            pila.append(nodo)
        else:
            if simbolo == ',':
                nodo_derecho = pila.pop()
                nodo_izquierdo = pila.pop()
                nodo = Nodo(',')
                nodo.izquierda = nodo_izquierdo
                nodo.derecha = nodo_derecho
                pila.append(nodo)
            elif simbolo == '*':
                nodo_unario = pila.pop()
                nodo = Nodo('*')
                nodo.izquierda = nodo_unario
                pila.append(nodo)
            elif simbolo == '|':
                nodo_derecho = pila.pop()
                nodo_izquierdo = pila.pop()
                nodo = Nodo('|')
                nodo.izquierda = nodo_izquierdo
                nodo.derecha = nodo_derecho
                pila.append(nodo)
    return pila[0]

# Función para dibujar el árbol utilizando Turtle
def dibujar_arbol(arbol, x, y, distancia):
    if arbol:
        t.penup()
        t.goto(x, y)
        t.pendown()
        t.circle(5)
        t.penup()
        t.goto(x-(9*len(arbol.valor)) , y-15)
        t.pensize(5)
        t.pencolor("red")
        t.write(arbol.valor, font=('Arial', 14, 'normal'))
        t.pensize(1)
        t.pencolor("black")
        # Dibujar línea izquierda
        if arbol.izquierda:
            t.goto(x, y)
            t.pendown()
            t.goto(x - distancia, y - 50)
            dibujar_arbol(arbol.izquierda, x - distancia, y - 50, distancia / 2)

        # Dibujar línea derecha
        if arbol.derecha:
            t.goto(x, y)
            t.pendown()
            t.goto(x + distancia, y - 50)
            dibujar_arbol(arbol.derecha, x + distancia, y - 50, distancia / 2)



)";

    string operation = R"(
# Construir el árbol
arbol = construir_arbol(expresion_postfix)

# Configuración de la ventana de dibujo
ventana = turtle.Screen()
ventana.bgcolor("white")
ventana.title("Árbol de Expresión Regular")

# Configuración de la Tortuga
ancho_pantalla, altura_pantalla = obtener_dimensiones_pantalla()
turtle.setup(ancho_pantalla, altura_pantalla)
t = turtle.Turtle()
t.penup()
t.goto(0,200)
t.write(expresion_postfix, font=('Arial', 14, 'normal'))
t.speed(0)
t.width(2)
t.penup()

# Dibujar el árbol
dibujar_arbol(arbol, 0, 100, 400)

# Cerrar la ventana haciendo clic sobre ella
ventana.exitonclick()
)";
    string obtain = "expresion_postfix = '"+postfix+"'";
    Py_Initialize();
    PyRun_SimpleString(python_code.c_str());
    PyRun_SimpleString(obtain.c_str());
    PyRun_SimpleString(operation.c_str());
    Py_Finalize();
}
void calculateSpace(string postfix_expression) {
    draw_expression_tree(postfix_expression);
}
