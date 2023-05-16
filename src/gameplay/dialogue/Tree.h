#pragma once

struct Edge;
struct Node;

// Tree
struct Tree
{
    std::vector<SharedPtr<Node>> m_Nodes;
    std::vector<Edge> m_Edges;
};
