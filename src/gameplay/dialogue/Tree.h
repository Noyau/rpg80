#pragma once

struct Edge;
struct DEPRECATED_Node;

// Tree
struct Tree
{
    std::vector<SharedPtr<DEPRECATED_Node>> m_Nodes;
    std::vector<Edge> m_Edges;
};
