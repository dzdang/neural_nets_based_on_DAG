#include <vector>
#include <iostream>
#include <algorithm>
#include "neural_net.h"
#include "constants.h"

void neural_net::add_input_layer(const std::vector<int> &node_ids, 
   const std::vector<int> &activation_flag)
{
   int init_node_val = 0;
   int num_input_neurons = node_ids.size();
   
   for(int i = 0; i < num_input_neurons; i++)
   {
      neural_graph.add_node(node_ids[i], init_node_val, activation_flag[i]);

      if(VERBOSE) std::cout << "Input layer neuron added to graph." << std::endl;
   }
}

void neural_net::add_output_layer(const std::vector<int> &node_ids, 
   const std::vector<int> &activation_flag)
{
   int init_node_val = 0;
   int num_output_neurons = node_ids.size();

   for(int i = 0; i < num_output_neurons; i++)
   {
      neural_graph.add_node(node_ids[i], init_node_val, activation_flag[i]);

      if(VERBOSE) std::cout << "Output layer neuron added to graph." << std::endl;
   }
}

void neural_net::add_hidden_layers(const std::vector<int> &node_ids, 
   const std::vector<int> activation_flag)
{
   int init_node_val = 0;
   int num_hidden_neurons = node_ids.size();

   for(int j = 0; j < num_hidden_neurons; j++)
   {
      neural_graph.add_node(node_ids[j], init_node_val, activation_flag[j]);

      if(VERBOSE == true) 
      {
         std::cout << "Hidden layer neuron added to graph." << std::endl;
      }
   }
}

void neural_net::add_connectivity(const std::vector<std::vector<int> > &connectivity,
   const std::vector<double> &init_weights)
{
   if(init_weights.size() != connectivity.size())
   {
      std::cerr << "Mismatch in size of user-spec'ed initial weights vector and connectivity " << std::endl;
      std::cerr << "Initial weights size: " << init_weights.size() << std::endl;
      std::cerr << "connectivity size: " << connectivity.size() << std::endl;
      exit(0);
   }

   int num_edges = init_weights.size();
   for(int i = 0; i < num_edges; i++)
   {
      neural_graph.add_edge(connectivity[i].at(0),connectivity[i].at(1),init_weights.at(i));
   }
   neural_graph.topological_sort();
}

void neural_net::forward_pass(const std::vector<std::vector<double> > &feature_mat,
   const std::vector<int> &input_node_ids)
{
   int idx = 0;
   double activated_val = 0;

   auto tp_sorted_list = neural_graph.get_tp_sorted_list();

   //loop through each sample stored in feature_mat
   for(auto it_feat_mat = feature_mat.begin(); it_feat_mat != feature_mat.end(); it_feat_mat++)
   {
      //initializes all node values to zero
      neural_graph.initialize_node_vals();

      //sets input node values to those given in feature_mat
      set_input_node_vals(*it_feat_mat,input_node_ids);

      //loops through topological sorted list
      for(auto it = tp_sorted_list.rbegin(); it != tp_sorted_list.rend(); it++)
      {
         //*it stores the node_id
         idx = neural_graph.get_idx(*it);
         //gets a pointer to the node value at idx in neural_graph
         auto ptr_2_node_val = neural_graph.get_ptr_2_node_val(idx);

         switch(neural_graph.get_activation_type(idx))
         {
            //apply activation function
            case relu_flag:
               activated_val = relu(*ptr_2_node_val);
               break;
            case sigmoid_flag:
               activated_val = sigmoid(*ptr_2_node_val);
               break;
            case identity_flag:
               activated_val = identity(*ptr_2_node_val);
               break;
            default:
               std::cout << "Invalid activation flag for node_id: " << *it << std::endl;
               exit(0);
         }

         //assigns the node value at idx to activated value
         *ptr_2_node_val = activated_val;
         ptr_2_node_val = nullptr;

         //multiplies the activated nodal value and the edge weight and assigns
         //it to all nodes connected to the node at *it
         neural_graph.weight_vals(activated_val,idx);
      }
   }
}

void neural_net::set_input_node_vals(const std::vector<double> &input_vals,
   const std::vector<int> &input_node_ids)
{
   if(input_node_ids.size() != input_vals.size())
   {
      std::cerr << "Mismatch in input_node_ids and input_vals size()" << std::endl;
      exit(0);
   }

   for(int i = 0; i < input_node_ids.size(); i++)
   {
      neural_graph.set_node_val(neural_graph.get_idx(input_node_ids[i]),
         input_vals[i]);
   }
}

double neural_net::relu(const double val)
{
   return std::max(double(0),val);
}

double neural_net::sigmoid(const double val)
{
   return (1/(1+exp(-val)));
}

double neural_net::identity(const double val)
{
   return val;
}