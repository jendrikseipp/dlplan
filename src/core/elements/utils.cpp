#include "utils.h"

#include <deque>
#include <iostream>

#include "../types.h"
#include "../../../include/dlplan/core.h"


namespace dlplan::core::element::utils {

extern int path_addition(int a, int b) {
    if (a == INF || b == INF) {
        return INF;
    } else {
        if (a > 0 && b > INF - a) {
            // handle overflow
            return INF;
        }
        return a + b;
    }
}

AdjList compute_adjacency_list(const RoleDenotation& r, bool inverse) {
    int num_objects = r.get_num_objects();
    const auto& r_data = r.get_const_data();
    AdjList adjacency_list(num_objects);
    for (int i = 0; i < num_objects; ++i) {
        for (int j = 0; j < num_objects; ++j) {
            if (r_data.test(i * num_objects + j)) {
                if (inverse) adjacency_list[j].push_back(i);
                else adjacency_list[i].push_back(j);
            }
        }
    }
    return adjacency_list;
}


Distances compute_distances_from_state(const AdjList& adj_list, int source) {
    Distances distances(adj_list.size(), INF);
    std::vector<bool> visited(adj_list.size(), false);
    distances[source] = 0;
    visited[source] = true;
    std::deque<int> queue;
    queue.push_back(source);
    while (!queue.empty()) {
        int source = queue.front();
        queue.pop_front();
        for (int target : adj_list[source]) {
            if (!visited[target]) {
                int alt = path_addition(distances[source], 1);
                if (distances[target] > alt) {
                    distances[target] = alt;
                }
                visited[target] = true;
                queue.push_back(target);
            }
        }
    }
    return distances;
}

PairwiseDistances compute_floyd_warshall(const AdjList& adj_list, bool reflexive) {
    int num_nodes = adj_list.size();
    PairwiseDistances dist(num_nodes, Distances(num_nodes, INF));
    // initialize edge costs
    for (int source = 0; source < num_nodes; ++source) {
        for (int target : adj_list[source]) {
            dist[source][target] = 1;
        }
    }
    // reflexive?
    if (reflexive) {
        for (int source = 0; source < num_nodes; ++source) {
            dist[source][source] = 0;
        }
    }
    // main loop
    for (int k = 0; k < num_nodes; ++k) {
        for (int i = 0; i < num_nodes; ++i) {
            for (int j = 0; j < num_nodes; ++j) {
                if (dist[i][j] > path_addition(dist[i][k], dist[k][j])) {
                    dist[i][j] = path_addition(dist[i][k], dist[k][j]);
                }
            }
        }
    }
    return dist;
}


RoleDenotation compute_transitive_closure(const PairwiseDistances& distances, int num_objects) {
    RoleDenotation result(num_objects * num_objects);
    auto& result_data = result.get_data();
    for (int i = 0; i < num_objects; ++i) {
        for (int j = 0; j < num_objects; ++j) {
            if (distances[i][j] < INF) {
                result_data.set(i * num_objects + j);
            }
        }
    }
    return result;
}


}
