//
// Created by juan-diego on 3/29/24.
//

#ifndef HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H
#define HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H

#include "window_manager.h"
#include "graph.h"
#include <unordered_map>
#include <set>
#include <queue>
#include <unordered_set>
#include <cmath>
#include <limits>
#include <algorithm>
#include <iostream> // para debugging

// Este enum sirve para identificar el algoritmo que el usuario desea simular
enum Algorithm
{
    None,
    Dijkstra,
    AStar,
    BestFS
};

//* --- PathFindingManager ---
//
// Esta clase sirve para realizar las simulaciones de nuestro grafo.
//
// Variables miembro
//     - path           : Contiene el camino resultante del algoritmo que se desea simular
//     - visited_edges  : Contiene todas las aristas que se visitaron en el algoritmo, notar que 'path'
//                        es un subconjunto de 'visited_edges'.
//     - window_manager : Instancia del manejador de ventana, es utilizado para dibujar cada paso del algoritmo
//     - src            : Nodo incial del que se parte en el algoritmo seleccionado
//     - dest           : Nodo al que se quiere llegar desde 'src'
//*
class PathFindingManager
{
    WindowManager *window_manager;
    std::vector<sfLine> path;

    // Fast mode: La ejecución era extremadamente lenta al dibujar cada arista visitada,
    // simplemente almacenamos las aristas visitadas y las dibujamos todas al final
    std::vector<sfLine> visited_edges;

    struct Entry
    {
        Node *node;
        double dist;

        bool operator<(const Entry &other) const
        {
            return dist < other.dist;
        }
    };

    // Algortmos de búsqueda
    // Dijkstra
    void dijkstra(Graph &graph)
    {
        std::unordered_map<Node *, double> dist;
        std::unordered_map<Node *, Node *> parent;

        auto cmp = [](const std::pair<double, Node *> &a, const std::pair<double, Node *> &b)
        {
            return a.first > b.first; // min-heap
        };

        std::priority_queue<
            std::pair<double, Node *>,
            std::vector<std::pair<double, Node *>>,
            decltype(cmp)>
            pq(cmp);

        const double INF = std::numeric_limits<double>::infinity();

        // init distances
        for (auto &kv : graph.nodes)
        {
            dist[kv.second] = INF;
            parent[kv.second] = nullptr;
        }

        dist[src] = 0.0;
        pq.push({0.0, src});

        while (!pq.empty())
        {
            auto [current_dist, current] = pq.top();
            pq.pop();

            if (current_dist > dist[current])
                continue;

            if (current == dest)
                break; // found shortest path to dest

            for (Edge *edge : current->edges)
            {
                Node *neighbor =
                    (edge->src == current) ? edge->dest : edge->src;

                double w = edge->length;

                if (dist[neighbor] > dist[current] + w)
                {
                    dist[neighbor] = dist[current] + w;
                    parent[neighbor] = current;
                    pq.push({dist[neighbor], neighbor});

                    // registrar arista visitada
                    sfLine visited_line(edge->src->coord, edge->dest->coord,
                                        sf::Color(0, 255, 0),
                                        edge->thickness + 0.4f);
                    visited_edges.push_back(visited_line);
                }
            }
        }

        set_final_path(parent);
    }

    // A*
    void a_star(Graph &graph)
    {
        std::unordered_map<Node *, double> g_score;
        std::unordered_map<Node *, double> f_score;
        std::unordered_map<Node *, Node *> parent;

        auto cmp = [](const std::pair<double, Node *> &a, const std::pair<double, Node *> &b)
        {
            return a.first > b.first; // min-heap by f
        };

        std::priority_queue<
            std::pair<double, Node *>,
            std::vector<std::pair<double, Node *>>,
            decltype(cmp)>
            open_set(cmp);

        const double INF = std::numeric_limits<double>::infinity();

        auto heuristic = [](Node *a, Node *b)
        {
            double dx = a->coord.x - b->coord.x;
            double dy = a->coord.y - b->coord.y;
            return std::sqrt(dx * dx + dy * dy);
        };

        for (auto &kv : graph.nodes)
        {
            g_score[kv.second] = INF;
            f_score[kv.second] = INF;
            parent[kv.second] = nullptr;
        }

        g_score[src] = 0.0;
        f_score[src] = heuristic(src, dest);
        open_set.push({f_score[src], src});

        while (!open_set.empty())
        {
            Node *current = open_set.top().second;
            open_set.pop();

            if (current == dest)
                break;

            for (Edge *edge : current->edges)
            {
                Node *neighbor =
                    (edge->src == current) ? edge->dest : edge->src;

                double tentative_g = g_score[current] + edge->length;

                if (tentative_g < g_score[neighbor])
                {
                    parent[neighbor] = current;
                    g_score[neighbor] = tentative_g;

                    double f = tentative_g + heuristic(neighbor, dest);
                    f_score[neighbor] = f;

                    open_set.push({f, neighbor});

                    // registrar arista visitada
                    sfLine visited_line(edge->src->coord, edge->dest->coord,
                                        sf::Color(0, 255, 0),
                                        edge->thickness + 0.4f);
                    visited_edges.push_back(visited_line);
                }
            }
        }

        set_final_path(parent);
    }

    // Best First Search
    void best_first_search(Graph &graph)
    {
        std::unordered_map<Node *, Node *> parent;

        auto heuristic = [](Node *a, Node *b)
        {
            double dx = a->coord.x - b->coord.x;
            double dy = a->coord.y - b->coord.y;
            return std::sqrt(dx * dx + dy * dy);
        };

        // priority_queue ordenada solo por h(n)
        auto cmp = [](const std::pair<double, Node *> &a,
                      const std::pair<double, Node *> &b)
        {
            return a.first > b.first; // min-heap por heurística
        };

        std::priority_queue<
            std::pair<double, Node *>,
            std::vector<std::pair<double, Node *>>,
            decltype(cmp)>
            pq(cmp);

        // Inicializamos
        double h0 = heuristic(src, dest);
        pq.push({h0, src});
        parent[src] = nullptr;

        std::unordered_set<Node *> visited;

        while (!pq.empty())
        {
            Node *current = pq.top().second;
            pq.pop();

            if (current == dest)
                break;

            if (visited.count(current))
                continue;

            visited.insert(current);

            // expandir vecinos
            for (Edge *edge : current->edges)
            {
                Node *neighbor =
                    (edge->src == current) ? edge->dest : edge->src;

                if (visited.count(neighbor))
                    continue;

                double h = heuristic(neighbor, dest);

                // solo se registra parent la primera vez que lo vemos
                if (!parent.count(neighbor))
                    parent[neighbor] = current;

                pq.push({h, neighbor});

                // guardar aristas visitadas para tecla E
                sfLine visited_line(edge->src->coord, edge->dest->coord,
                                    sf::Color(0, 255, 0),
                                    edge->thickness + 0.4f);
                visited_edges.push_back(visited_line);
            }
        }

        set_final_path(parent);
    }

    //* --- set_final_path ---
    // Esta función se usa para asignarle un valor a 'this->path' al final de la simulación del algoritmo.
    //
    // Ejemplo:
    //     parent(a): b
    //     parent(b): c
    //     parent(c): d
    //     parent(d): NULL
    //
    // Se reconstruye el camino desde dest->src y se colocan líneas sfLine
    //*
    void set_final_path(std::unordered_map<Node *, Node *> &parent)
    {
        path.clear();

        if (parent[dest] == nullptr)
            return;

        Node *current = dest;
        std::vector<sfLine> reversed_path;

        while (current != nullptr && current != src)
        {
            Node *p = parent[current];
            if (p == nullptr)
                break;

            sfLine line(p->coord, current->coord,
                        sf::Color(80, 220, 200),
                        default_thickness + 1.2f);

            reversed_path.push_back(line);

            // colorear nodos del path final
            current->color = sf::Color(80, 220, 200);
            p->color = sf::Color(80, 220, 200);

            current = p;
        }

        // reverse para obtener src → dest
        path.reserve(reversed_path.size());
        for (auto it = reversed_path.rbegin(); it != reversed_path.rend(); ++it)
            path.push_back(*it);
    }

public:
    Node *src = nullptr;
    Node *dest = nullptr;

    explicit PathFindingManager(WindowManager *window_manager)
        : window_manager(window_manager)
    {
    }

    void exec(Graph &graph, Algorithm algorithm)
    {
        if (src == nullptr || dest == nullptr)
            return;

        path.clear();          // vaciar camino previo
        visited_edges.clear(); // vaciar aristas visitadas

        // Reset node visuals
        for (auto &kv : graph.nodes)
            kv.second->reset();

        // Reset edge visuals
        for (Edge *e : graph.edges)
        {
            e->color = default_edge_color;
            e->thickness = default_thickness;
        }

        // marcar src/dest en pantalla
        src->color = sf::Color(50, 200, 50); // verde
        src->radius = default_radius + 0.6f;

        dest->color = sf::Color(200, 50, 50); // rojo
        dest->radius = default_radius + 0.6f;

        // ejecutar algoritmo sin animación
        if (algorithm == Dijkstra)
            dijkstra(graph);
        else if (algorithm == AStar)
            a_star(graph);
        else if (algorithm == BestFS)
            best_first_search(graph);
    }

    void reset()
    {
        path.clear();
        visited_edges.clear();

        if (src)
        {
            src->reset();
            src = nullptr;
        }
        if (dest)
        {
            dest->reset();
            dest = nullptr;
        }
    }

    void draw(bool draw_extra_lines)
    {
        // Dibujar aristas visitadas y camino final
        if (draw_extra_lines)
        {
            for (sfLine &line : visited_edges)
                line.draw(window_manager->get_window(), sf::RenderStates::Default);
        }

        for (sfLine &line : path)
            line.draw(window_manager->get_window(), sf::RenderStates::Default);

        if (src)
            src->draw(window_manager->get_window());

        if (dest)
            dest->draw(window_manager->get_window());
    }
};

#endif // HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H
