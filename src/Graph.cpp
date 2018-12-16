#include <iostream>
#include "../include/Graph.h"
#include "../include/constants.h"

void Graph::add_node(int node_id, double node_val, int activation_flag)
{
   std::vector<edge_info> temp_vector;
   node_info temp_node;

   temp_node.node_id = node_id;
   temp_node.node_val = node_val;
   temp_node.activation_type = activation_flag;

   node_list.push_back(temp_node);
   num_nodes++;

   //gets the index where the node info is stored
   auto idx = node_list.size() - 1;

   //store key-value (node_id,idx) in hash table
   id_map.insert({node_id, idx}); 

   //allocates space to store adjacency info for this node 
   adj_list.push_back(temp_vector);

   if(VERBOSE)
   {
      std::cout << "Node id: " << node_id << " added. Total number of nodes: " << num_nodes << std::endl;
   }
}

void Graph::remove_node(int node_id)
{
   int idx2remove = 0;

   if(num_nodes == 0)
   {
      std::cout << "Graph is empty" << std::endl;
      return;
   }

   auto it = find(node_list.begin(), node_list.end(), node_id);
   idx2remove = get_idx(node_id);
   node_list.erase(it);

   id_map.erase(node_id);

   num_nodes--;

   if(VERBOSE)
   {
      std::cout << "Node id: " << node_id << " removed. "
      "Total number of nodes: " << num_nodes << std::endl;
   }

   num_edges = num_edges - (adj_list[idx2remove].size());

   //remove row associated with idx2Remove and remove value from node_val
   if(adj_list.size() > idx2remove)
   {
      adj_list.erase(adj_list.begin() + idx2remove);  
      // node_vals.erase(node_vals.begin() + idx2remove);
      node_list.erase(node_list.begin() + idx2remove);
   }

   if(VERBOSE)
   {
      std::cout << "Removed adjacency list corresponding to node id: "
         << node_id << std::endl;
      std::cout << "Total number of edges: " << num_edges << std::endl;
   }   

   //remove edge
   //how to remove dependencies more efficiently?
   //my way is to search each vector, adj_list[i], to see if it contains idx2remove
   //but this seems terribly inefficient
   for(auto adj_listIterator = adj_list.begin(); adj_listIterator != adj_list.end();
      adj_listIterator++) //what if I used < instead of !=
   {
      idx2remove = std::distance(adj_list.begin(),adj_listIterator);
      remove_edge(get_node_id(idx2remove),node_id);
   }
}

//u & v are the node_ids
void Graph::add_edge(int u, int v, double weight) 
{
   int u_idx = 0, v_idx = 0;
   edge_info nw;

   u_idx = get_idx(u);
   v_idx = get_idx(v);

   nw.node_id = v;
   nw.weight = weight;

   //adds edge from u to v in graph
   adj_list[u_idx].push_back(nw);

   num_edges++;

   if(VERBOSE)
   {
      std::cout << "Added edge from node id: " << u << " (idx = " << u_idx << ") to "
      "node id: " << v << " (idx = " << v_idx << ")." << std::endl;
      std::cout << "Total number of edges: " << num_edges << std::endl;
   }   
}

void Graph::remove_edge(int u, int v)  //u & v are node_ids
{
   int u_idx = 0;
   int v_idx = 0;
   std::vector<edge_info>::iterator it;

   u_idx = get_idx(u);
   v_idx = get_idx(v);

   // remove edge from u to v
   it = std::find(adj_list[u_idx].begin(), adj_list[u_idx].end(), v);
   adj_list[u_idx].erase(it); //what if v_idx exceeds size of adj_list[u_idx]

   num_edges--;

   if(VERBOSE)
   {
      std::cout << "Removed edge from node id: " << u << "(idx = " << u_idx << ") to "
      "node id: " << v << "idx = " << v_idx << ")." << std::endl;

      std::cout << "Total number of edges: " << num_edges << std::endl;
   }
}

void Graph::topological_sort()
{
   //mark all nodes as not visited
   std::vector<bool> visited(num_nodes,false);

   tp_sorted_list.reserve(num_nodes);

   //Call recursive helper function to store topological sort starting from
   //all vertices one by one
   for(int i = 0; i < num_nodes; i++)
   {
      if(visited[i] == false)
      {
         topological_sort_helper(i,visited,tp_sorted_list);
      }
   }
}

//recursive function used by topological_sort
void Graph::topological_sort_helper(int u, std::vector<bool> &visited, 
   std::vector<int> &tp_sorted_list)
{
   int u_idx = get_idx(u);
   int v_idx = 0;
   int v = 0;
   //mark current node as visited
   visited[u_idx] = true;

   //recurse for all vertices adjacent to this node
   for(auto it = adj_list[u_idx].begin(); it != adj_list[u_idx].end(); it++)
   {
      v = it->node_id; 
      v_idx = get_idx(v);

      if(!visited[v_idx])
      {
         topological_sort_helper(v,visited,tp_sorted_list);
      }
   }

   tp_sorted_list.push_back(u);
}

void Graph::initialize_node_vals()
{
   for(auto it = node_list.begin(); it != node_list.end(); it++)
   {
      it->node_val = 0;
   }
}

void Graph::weight_vals(double u_val, int u_idx)
{
   //traverse adjacency list for all edges in adjlist[u_idx]
   //and then multiply the weights to u_val and assign the product to
   //node id v in E=(u_v)
   double v_val = 0;
   auto vec = adj_list[0];

   for(auto it = adj_list[u_idx].begin(); it != adj_list[u_idx].end(); it++)
   {
      auto v_idx = get_idx(it->node_id);
      v_val = (it->weight)*u_val + get_node_val(v_idx);

      set_node_val(v_idx,v_val);
   }
}

double Graph::get_node_val(int idx)
{
   return node_list[idx].node_val;
}

void Graph::set_node_val(int idx, double val)
{
   node_list[idx].node_val = val;
}

double* Graph::get_ptr_2_node_val(int idx)
{
   return &(node_list[idx].node_val);
}

int Graph::get_activation_type(int idx)
{
   return node_list[idx].activation_type;
}

int Graph::get_idx(int node_id)
{
   return id_map.at(node_id);
}

int Graph::get_node_id(int idx)
{
   return node_list[idx].node_id;
}

std::vector<int> Graph::get_tp_sorted_list()
{
   return tp_sorted_list;
}

int Graph::get_num_nodes()
{
   return num_nodes;
}