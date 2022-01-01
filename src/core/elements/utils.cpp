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

AdjList compute_adjacency_list(const RoleDenotation& r_vec, int num_objects) {
    AdjList adjacency_list(num_objects);
    for (const auto& r : r_vec) {
        adjacency_list[r.first].push_back(r.second);
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

RoleDenotation compute_transitive_closure(const PairwiseDistances& distances) {
    int num_objects = distances.size();
    RoleDenotation result;
    result.reserve(num_objects * num_objects);
    for (int source = 0; source < num_objects; ++source) {
        for (int target = 0; target < num_objects; ++target) {
            if (distances[source][target] < INF) {
                result.emplace(source, target);
            }
        }
    }
    return result;
}

}
