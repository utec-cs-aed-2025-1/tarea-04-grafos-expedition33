[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/5zgGDtf4)
[![Open in Visual Studio Code](https://classroom.github.com/assets/open-in-vscode-2e0aaae1b6195c2367325f4f02e2d04e9abb55f0b24a779b69b11b9e10269abc.svg)](https://classroom.github.com/online_ide?assignment_repo_id=21834442&assignment_repo_type=AssignmentRepo)
# Tarea de Grafos

## Integrantes: 
- 1 Fabio Dávila Venturo

## Objetivo: 
El objetivo de esta tarea es implementar un **Path Finder** para la ciudad de Lima. 

<p align="center">
    <img src=https://github.com/utec-cs-aed/homework_graph/assets/79115974/b63f69db-17eb-417a-8aa1-8483d8dcdaf0 / >
</p>

## Dependencias

Para esta tarea se solicita utilizar ```C++17``` y la librería ```SFML 2.5```

- Para instalar ```SFML 2.5```:

    - [Windows](https://www.youtube.com/watch?v=HkPRG0vfObc)
    - [MacOS y Linux](https://www.youtube.com/playlist?list=PLvv0ScY6vfd95GMoMe2zc4ZgGxWYj3vua)

Cuando se instale la librería, probar que las siguientes líneas del ```CMakeLists.txt``` encuentren la librería adecuadamente.
```cmake
find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)
if(SFML_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window)
else()
    message("SFML not found")
endif()
```

## Dataset
El dataset consiste de dos csv:

- *nodes.csv*

    ![image](https://github.com/utec-cs-aed/homework_graph/assets/79115974/6a68cf06-196a-4605-83a7-3183e9a3f0ec)


- *edges.csv*

    ![image](https://github.com/utec-cs-aed/homework_graph/assets/79115974/247bbbd7-6203-45f4-8196-fcb0434b0f1d)


## Algoritmos
Se les solicita implementar tres algoritmos para busqueda en grafos

- *Dijkstra*

- *Best First Search*

- *A**

Además:
- Analice la complejidad computacional de los tres algoritmos de acuerdo a su propia implementación.
- Puede considere como heuristica la distancia en linea recta.
- **Debe realizar un pequeño video (2 min) mostrando la funcionalidad visual de cada algoritmo**

## Diagrama de clases UML 

![image](https://github.com/utec-cs-aed/homework_graph/assets/79115974/f5a3d89e-cb48-4715-b172-a17e6e27ee24)

----------
> **Créditos:** Juan Diego Castro Padilla [juan.castro.p@utec.edu.pe](mailto:juan.castro.p@utec.edu.pe)

# Cambios en el sistema de animación

Originalmente, la ejecución de los algoritmos se realizaba mostrando cada paso. Al menos en mi pc, esto hacía que la animación durara varios minutos y consumiera demasiados recursos. Se reemplazó el sistema de animación por uno **sin visualización paso a paso**. El dibujo de la ruta óptima solo se hace al terminar el algoritmo. Además, el usuario puede visualizar las aristas visitadas presionando E.

# Complejidad computacional según la implementación

Sea:

* **N** = número de nodos
* **A** = número de aristas

### ✔ **Dijkstra**

En la implementación:

* Inserciones en heap → $O(log N)$
* Cada arista realiza como máximo una relajación

**Complejidad total:**

$O((N + A)log N)$


Motivo:
Cada arista puede provocar una disminución de distancia → push al heap → $log N$.

### ✔ **A***

A* utiliza:

$f(n) = g(n) + h(n)$

La operación dominante es la misma que en Dijkstra:
relajaciones y pushes al priority_queue.

**Complejidad en el peor caso:**

$O( (N + A) log N )$

### ✔ **Best First Search (Greedy BFS)**

Solo utiliza la heurística:

$f(n) = h(n)$

Entonces siempre expande el nodo “más cercano” al destino.
También usa un min-heap, por lo que su complejidad es:

$O( (N + A) log N )$

# Menú de opciones implementado (GUI)

Para facilitar la interacción, se implementó un **menú** dentro de la ventana principal utilizando la fuente **Arial (arial.ttf)**.
La GUI muestra el estado actual de la aplicación y las acciones disponibles.

### ✔ Elementos del menú mostrados en pantalla

* Estado actual:
  * “Start seleccionado”
  * “End seleccionado”
  * “Algoritmo ejecutado”
  * “Selecciona Start y End primero”
* Lista de comandos disponibles

# Entorno usado (VSCode + MSYS2 + CMake + SFML)

Todo el proyecto se desarrolló utilizando **Visual Studio Code** como editor y **MSYS2** como entorno de compilación para poder usar:

* `gcc` / `g++` del entorno UCRT64
* `ninja` como generador de build
* `cmake` para la configuración del proyecto
* instalación de **SFML** desde MSYS2

# Video demostración:

[Carpeta de Drive con el video](https://drive.google.com/drive/folders/1lkgyVuK_SwPLsIvNF7-ooDv50bL_PoZs?usp=sharing)



