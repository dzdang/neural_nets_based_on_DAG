#include "../include/neural_net.h"
#include "../include/constants.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <vector>

void read_bodyfat_file(std::vector<std::vector<double>> &feature_mat,
   std::vector<double> &labels)
{
   //input stream var
   std::ifstream in_file;
   double temp_var1 = 0, temp_var2 = 0, temp_var3 = 0; 
   int line = 0;
   std::vector<double> temp_vec;

   in_file.open("/Users/dzdang/Desktop/neuralNetworks/bodyfat.dat");

   if(!in_file)
   {
      std::cerr << "Unable to open bodyfat.dat" << std::endl;
      exit(1);
   }

   while(in_file >> temp_var1 >> temp_var2 >> temp_var3)
   {
      feature_mat.push_back(temp_vec);
      feature_mat[line].push_back(temp_var1);
      feature_mat[line].push_back(temp_var2);

      labels.push_back(temp_var3);

      line++;
   }

   in_file.close();
}

int main(int argc, char const *argv[])
{
   neural_net net_hw6;
   int num_hidden_layers = 2;
   std::vector<int> nodes_per_hidden_layer = {64,16};
   std::vector<std::vector<double> > feature_mat;
   std::vector<double> labels;
   int num_samples=0, num_training_samples=150, num_test_samples=0;
   int num_features=0;
   int num_output_vars=1;
   std::vector<std::vector<int> > node_connectivity;
   std::vector<int> temp_connectivity(2,0);
   std::vector<double> init_weights;
   std::vector<int> input_node_activation_flag, output_node_activation_flag, hidden_node_activation_flag;

   read_bodyfat_file(feature_mat,labels);

   num_samples = feature_mat.size();
   num_features = feature_mat[0].size();
   num_test_samples = num_samples - num_training_samples;

   std::cout << "num_samples: " << num_samples << std::endl;
   std::cout << "num_training_samples: " << num_training_samples << std::endl;
   std::cout << "num_features: " << num_features << std::endl;

   //generate input node_ids
   std::vector<int> input_node_ids = {0,1};
   int last_node_id = *(std::max_element(input_node_ids.begin(),input_node_ids.end()));

   //set activation flag for input nodes to identity
   for(int i = 0; i < num_features; i++)
   {
      input_node_activation_flag.push_back(identity_flag);
   }

   //generate hidden layer node_ids and also assign activation flags to relu
   std::vector<int> hidden_node_ids;
   for(int j = 0; j < num_hidden_layers; j++)
   {
      for(int i = 0; i < nodes_per_hidden_layer[j]; i++)
      {
         last_node_id++;
         hidden_node_ids.push_back(last_node_id);      
         hidden_node_activation_flag.push_back(relu_flag);   
      }
   }

   //generate output layer node ids
   last_node_id++;
   std::vector<int> output_node_ids = {last_node_id};
   output_node_activation_flag.push_back(identity_flag);

   //generate edges for input layer and first hidden layer; fully connected
   for(int k = 0; k < num_features; k++)
   {
      temp_connectivity[0] = k;
      for(int i = 0; i < nodes_per_hidden_layer[0]; i++)
      {
         temp_connectivity[1] = hidden_node_ids[i];
         node_connectivity.push_back(temp_connectivity);
      }
   }

   //generate edges for first hidden layer and second layer, fully connected
   for(int k = 0; k < nodes_per_hidden_layer[0]; k++)
   {
      temp_connectivity[0] = hidden_node_ids[k];
      for(int i = 0; i < nodes_per_hidden_layer[1]; i++)
      {
         temp_connectivity[1] = hidden_node_ids[i + nodes_per_hidden_layer[0]];
         node_connectivity.push_back(temp_connectivity);
      }
   }

   //generate edges for second/last hidden layer and output, fully connected
   temp_connectivity[1] = output_node_ids[0];
   for(int k = 0; k < nodes_per_hidden_layer[1]; k++)
   {
      temp_connectivity[0] = hidden_node_ids[k + nodes_per_hidden_layer[0]];
      node_connectivity.push_back(temp_connectivity);
   }   
   std::cout << node_connectivity.size() << std::endl;

   //make all weights 1 for testing
   for(int k = 0; k < node_connectivity.size(); k++)
   {
      init_weights.push_back(1);
   }

   net_hw6.add_input_layer(input_node_ids,input_node_activation_flag);
   net_hw6.add_hidden_layers(hidden_node_ids,hidden_node_activation_flag);
   net_hw6.add_output_layer(output_node_ids,output_node_activation_flag);
   net_hw6.add_connectivity(node_connectivity,init_weights);
   net_hw6.forward_pass(feature_mat,input_node_ids);

   return 0;
}