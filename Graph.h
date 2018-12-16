#ifndef _GRAPH_CLASS_PRACTICE_H_
#define _GRAPH_CLASS_PRACTICE_H_

#include <vector>
#include <utility>
#include <unordered_map>

// typedef struct
struct node_info
{
   int node_id;
   double node_val;
   int activation_type;

   void operator=(const node_info &rhs)
   {
      node_id = rhs.node_id;
      node_val = rhs.node_val;
      activation_type = rhs.activation_type;
   }
   bool operator==(const int val)
   {
      return(node_id == val);
   }
};// node_info;

// typedef struct
struct edge_info
{
   int node_id;  //node_id for v in edge (u,v)
   double weight;

   bool operator==(const int val) const
   {
      return(node_id == val);
   }
};// edge_info;

//A directed graph class
class Graph
{
   public:
      Graph()
      {
         num_nodes = 0;
         num_edges = 0;
      }
      ~Graph() = default;

      //function for adding nodes/vertices to directed graph
      void add_node(int node_id, double node_val, int activation_flag);  

      //function for removing nodes/vertices to directed graph    
      void remove_node(int node_id); 

      //function for adding edge to directed graph
      void add_edge(int u, int v, double weight);   

      //function for removing edge from directed graph
      void remove_edge(int u, int v);

      //function for getting index in nodes that contains node_id
      int get_idx(int node_id);

      //function for getting the node id at idx
      int get_node_id(int idx);

      //function for getting the node weight at idx
      int get_node_weight(int idx);

      //function for getting the number of nodes in graph
      int get_num_nodes();
      
      //function for getting activation type
      int get_activation_type(int idx);
      
      //function for getting a pointer to node_list.node_val
      double* get_ptr_2_node_val(int idx);
      
      //function for setting the nodal value at idx to val
      void set_node_val(int idx, double val);
      
      //initialize all nodal values to 0
      void initialize_node_vals();
      
      //function for getting nodal value at idx
      double get_node_val(int idx);
      
      //function for topological sorting of the directed acyclic graph
      void topological_sort();
      
      //helper function for topological sort
      void topological_sort_helper(int u, std::vector<bool> &visited, 
         std::vector<int> &tp_sorted_list);
      std::vector<int> get_tp_sorted_list();
      
      //function for propagating the node value u_val into all outgoing edges
      //by multiplying u_val with each outgoing edge's weight and assigning the
      //product to the node at the end of the edge
      void weight_vals(double u_val, int u_idx);

   private:
      //variable for storing number of nodes in the graph
      int num_nodes;

      //variable for storing the number of edges in the graph
      int num_edges;

      //variable for storing the topologically sorted list of node_ids in the graph
      std::vector<int> tp_sorted_list;

      //variable for storing the list of nodes and its info
      std::vector<node_info> node_list;

      //adjacency list that stores edge info (weights and the node id for v in
      //E=(u,v)
      std::vector<std::vector<edge_info> > adj_list;

      //hash table that maps key-value pair. key is the node_id and value is the
      //index. The index corresponds to the element in node_list[index] and
      //adj_list[index] that contains the node and adjacency info for the key
      std::unordered_map<int,int> id_map;
};

#endif // _GRAPH_CLASS_PRACTICE_H_