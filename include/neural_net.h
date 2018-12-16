#ifndef _neural_net_PRACTICE_H
#define _neural_net_PRACTICE_H

#include <vector>
#include "Graph.h"

class neural_net
{
   public:
      neural_net() = default;

      //adds the user provided input node ids and their activation function types
      //to this.neural_graph
      void add_input_layer(const std::vector<int> &node_ids,const std::vector<int> &activation_flag);

      //adds the user provided output node ids and their activation function types
      //to this.neural_graph      
      void add_output_layer(const std::vector<int> &node_ids, 
         const std::vector<int> &activation_flag);

      //adds the user provided hidden node ids and their activation function types
      //to this.neural_graph  
      void add_hidden_layers(const std::vector<int> &node_ids, 
         const std::vector<int> activation_flag);

      //adds the connectivity and weight information provided by user to
      //this.neural_graph
      void add_connectivity(const std::vector<std::vector<int> > &connectivity,
         const std::vector<double> &init_weights);      

      //performs forward pass of neural network
      void forward_pass(const std::vector<std::vector<double> > &feature_mat,
         const std::vector<int> &input_node_ids);

      //sets the input layer's nodal values
      void set_input_node_vals(const std::vector<double> &input_vals,
         const std::vector<int> &input_node_ids);

   private:
      Graph neural_graph;
      
      //activation functions 
      double relu(const double val);
      double sigmoid(const double val);
      double identity(const double val);
};

#endif // _neural_net_PRACTICE_H
