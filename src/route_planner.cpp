#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();

    // loop through the list of pointers.
    for(RouteModel::Node* neighbor_node : current_node->neighbors) {
        neighbor_node->parent = current_node;
        neighbor_node->g_value = current_node->g_value + neighbor_node->distance(*current_node);
        neighbor_node->h_value = CalculateHValue(neighbor_node);
        neighbor_node->visited = true;
        open_list.emplace_back(neighbor_node);
    }
}

bool CompareNode(const RouteModel::Node *n1, const RouteModel::Node *n2) {
    return (n1->g_value + n1->h_value) > (n2->g_value + n2->h_value);
}

RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end(), CompareNode);
    RouteModel::Node *lowest = open_list.back();
    open_list.pop_back();

    return lowest;
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    while (current_node != start_node) {
        path_found.emplace_back(*current_node);
        distance += current_node->distance(*current_node->parent);
        current_node = current_node->parent;
    }

    path_found.emplace_back(*current_node);
    std::reverse(path_found.begin(), path_found.end());
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.

    return path_found;
}

void RoutePlanner::AStarSearch() {
    // Process starting node
    RouteModel::Node *current_node = start_node;
    start_node->visited = true;
    start_node->g_value = 0;
    start_node->h_value = CalculateHValue(start_node);
    open_list.emplace_back(start_node);

    while(current_node != end_node) {
        current_node = NextNode();
        AddNeighbors(current_node);
    }

    m_Model.path = ConstructFinalPath(current_node);
}
