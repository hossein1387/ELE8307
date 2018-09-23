// /*
//  * NNLayer.cpp
//  *
//  *  Created on: Jul 11, 2013
//  *      Author: jpdavid
//  */

// #include "NNLayer.h"
// #include "math.h"
// #include "stdio.h"
// #include "stdlib.h"

// #define MAKE_TERNARY_THRESHOLD 0.3

// NNLayer::NNLayer() {
//     // TODO Auto-generated constructor stub
//     n_input = 0;
//     n_neuron = 0;
//     positive_threshold = 0;
//     negative_threshold = 0;
//     bias = 0;
//     weight = 0;
//     value = 0;
// }

// NNLayer::NNLayer(int new_n_input, int new_n_neuron) {
//     // TODO Auto-generated constructor stub
//     init(new_n_input, new_n_neuron);
// }

// void NNLayer::init(int new_n_input, int new_n_neuron) {
//     // TODO Auto-generated constructor stub
//     n_input = new_n_input;
//     n_neuron = new_n_neuron;

//     positive_threshold = 0;
//     negative_threshold = 0;
//     bias = new float[n_neuron];

//     weight = new float[n_neuron*n_input];
//     value = new float[n_neuron];
// }

// float NNLayer::rand_FloatRange(float a, float b) {
//     return ((b-a)*((float)rand()/RAND_MAX))+a;
// }

// void NNLayer::random_init(int new_n_input, int new_n_neuron) {
//     // TODO Auto-generated constructor stub
//     init(new_n_input, new_n_neuron);

//     positive_threshold = rand_FloatRange(0, log2(new_n_input));
//     negative_threshold = -positive_threshold;
//     bias = new float[n_neuron];

//     float * cur_weight = weight;
//     for (int i=0; i<n_neuron; i++) {
//         bias[i] = rand_FloatRange(-log2(new_n_input),log2(new_n_input));
//         for (int j=0; j<n_input; j++) {
//             *(cur_weight++) = rand_FloatRange(-1,1);
//         }
//     }
// }

// float NNLayer::ternary_value(float x) {
//     float result = 0;
//     if (x>positive_threshold) result = 1;
//     if (x<negative_threshold) result = -1;
//     return result;
// }

// void NNLayer::make_ternary() {
//     float * cur_weight = weight;
//     for (int i=0; i<n_neuron; i++) {
//         bias[i] = trunc(bias[i]);
//         for (int j=0; j<n_input; j++) {
//             if (*cur_weight>MAKE_TERNARY_THRESHOLD) *cur_weight = 1;
//             else if (*cur_weight<-MAKE_TERNARY_THRESHOLD) *cur_weight = -1;
//             else *cur_weight = 0;
//             cur_weight++;
//         }
//     }
// }

// NNLayer::~NNLayer() {
//     // TODO Auto-generated destructor stub
// }

// //Edit this function for ternary logic
// float NNLayer::fct(float x) {
//     float f = 1.0/(1.0+exp(-x));
//     return f;
// }

// float * NNLayer::propagate(float * source) {
//     // TODO Auto-generated constructor stub
//     float * cur_weight = weight;

//     for (int i=0; i<n_neuron; i++) {
//         float acc = bias[i];

//         for (int j=0; j<n_input; j++) {
//             acc += *(cur_weight++) * source[j];
//         }
// //      value[i] = fct(acc);
//         value[i] = ternary_value(acc);
//     }
//     return value;
// }

// void NNLayer::print_activation() {
//     printf("---------------\n");
//     for (int i=0; i<n_neuron; i++) {
//         printf("%i, %i\n", i, (int)value[i]);
//     }
// }

// void NNLayer::print() {
//     // TODO Auto-generated constructor stub

//     float * cur_weight = weight;

//     for (int i=0; i<n_neuron; i++) {
//         printf("Neuron %i: %f, {", i+1, bias[i]);
//         for (int j=0; j<n_input; j++) {
//             if (j!=0) printf(", %2.2f", *(cur_weight++));
//             else printf("%2.2f", *(cur_weight++));
//         }
//         printf("}, %f\r\n",value[i]);
//     }
// }

