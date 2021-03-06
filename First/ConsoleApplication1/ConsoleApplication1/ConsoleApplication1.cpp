#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <math.h>

using namespace std;

//Set for training. Network will calculate x XOR y.
class InputLayer {
	//_trainset[0][2] - result of 0 XOR 0.
	//_trainset[3][2] - result of 1 XOR 0.
public:
	double** _trainset;
	InputLayer() {
		_trainset = new double*[4];
		for (int i = 0; i < 4; ++i) {
			_trainset[i] = new double[3];
		}
		_trainset[0][0] = 0;
		_trainset[0][1] = 0;
		_trainset[0][2] = 0;
		_trainset[1][0] = 0;
		_trainset[1][1] = 1;
		_trainset[1][2] = 1;
		_trainset[2][0] = 1;
		_trainset[2][1] = 0;
		_trainset[2][2] = 1;
		_trainset[3][0] = 1;
		_trainset[3][1] = 1;
		_trainset[3][2] = 0;
	}
	double* get_train_set(int _ts) {
		return _trainset[_ts];
	}
};
InputLayer INPUT_LAYER;

enum Neuron_Type { Input, Hidden, Output, Bias };

class Neuron {
public:
	Neuron_Type neuron_type;
	double input_data[2];
	double input_value;
	double output_value;
	double mistake;
	double weights[2];
	Neuron() {

	}
	Neuron(double* _idata, double* _weights, Neuron_Type TYPE) {
		neuron_type = TYPE;
		for (int i = 0; i < 2; ++i) {
			input_data[i] = _idata[i];
			weights[i] = _weights[i];
		}
	}
	//Calculate input value
	void CIV() {
		input_value = 0;
		for (int i = 0; i < 2; ++i) {
			input_value += input_data[i] * weights[i];
		}
	}
	//Calculate output value, second name - activation function
	void COV() {
		output_value = (1 / (1 + exp(-1 * input_value)));
	}
};

class Layer {
public:	
	Neuron_Type neuron_type;
	Neuron* layer;
	Layer() {

	}
	Layer(Neuron_Type type) {
		if (type == Input) {
			layer = new Neuron[2];
			for (int i = 0; i < 2; ++i) {
				layer[i].output_value = INPUT_LAYER.get_train_set(2)[i];
				layer[i].neuron_type = Input;
			}
		}
		if (type == Hidden) {
			layer = new Neuron[2];
			for (int i = 0; i < 2; ++i) {
				layer[i].neuron_type = Hidden;
				for (int j = 0; j < 2; ++j) {
					layer[i].weights[j] = (rand() % 100) / 100;
				}
			}
		}
		if (type == Output) {
			layer = new Neuron[1];
			layer[0].neuron_type = Output;
			for (int i = 0; i < 2; ++i) {
				layer[0].weights[i] = (rand() % 100) / 100;
			}
		}
	}
};

class Network {
public:
	Layer* i_layer;
	Layer* h_layer;
	Layer* o_layer;
	double ideal;
	double mistake;
	int epoch;

	//Settings of network
	double EPSYLON;
	double ALPHA;
	double* last_deltaw5w6 = new double[2];
	double* last_deltaw2w4 = new double[2];
	double* last_deltaw1w3 = new double[2];
	//
	Network() {
		i_layer = new Layer(Neuron_Type::Input);
		h_layer = new Layer(Neuron_Type::Hidden);
		o_layer = new Layer(Neuron_Type::Output);
		epoch = 0;
		EPSYLON = 0.01;
		ALPHA = 0.01;
		for (int i = 0; i < 2; ++i) {
			last_deltaw1w3[i] = 0;
			last_deltaw2w4[i] = 0;
			last_deltaw5w6[i] = 0;
		}
	}
	void set_action() {
		cout << "---DEMONSTRATION OF x XOR y : " << endl << endl;
		cout << "Choose example:" << endl;
		cout << "1) 0 XOR 0 --- Result must be 0" << endl;
		cout << "2) 0 XOR 1 --- Result must be 1" << endl;
		cout << "3) 1 XOR 0 --- Result must be 1" << endl;
		cout << "4) 1 XOR 1 --- Result must be 0" << endl;
		int act;
		cin >> act;
		for (int i = 0; i < 2; ++i) {
			i_layer->layer[i].output_value = INPUT_LAYER.get_train_set(act - 1)[i];
			ideal = INPUT_LAYER.get_train_set(act - 1)[2];
		}
	}
	void CData() {
		//Set input data and weights
		for (int i = 0; i < 2; ++i) {
			h_layer->layer[0].input_data[i] = i_layer->layer[i].output_value;
		}
		//
		cout << "wi - ";
			for (int i = 0; i < 2; ++i) {
			cout << h_layer->layer[0].weights[i] << " ";
		}
		for (int i = 0; i < 2; ++i) {
			cout << h_layer->layer[1].weights[i] << " ";
		}
		cout << endl;
		//
		h_layer->layer[0].CIV();
		h_layer->layer[0].COV();
		//
		cout << "H1 IV - " << h_layer->layer[0].input_value << endl;
		cout << "H1 OV - " << h_layer->layer[0].output_value << endl;
		//
		for (int i = 0; i < 2; ++i) {
			h_layer->layer[1].input_data[i] = i_layer->layer[i].output_value;
		}
		h_layer->layer[1].CIV();
		h_layer->layer[1].COV();
		//
		cout << "H2 IV - " << h_layer->layer[1].input_value << endl;
		cout << "H2 OV - " << h_layer->layer[1].output_value << endl;
		//
		for (int i = 0; i < 2; ++i) {
			o_layer->layer[0].input_data[i] = h_layer->layer[i].output_value;
		}
		o_layer->layer->CIV();
		o_layer->layer->COV();
		//
		cout << "O1 IV - " << o_layer->layer[0].input_value << endl;
		cout << "O1 OV (Result) - " << o_layer->layer[0].output_value << endl;
		//
		mistake = pow((ideal - o_layer->layer->output_value), 2) / 1;
		//
		cout << "O1 Ideal - " << ideal << endl;
		cout << "O1 Mistake - " << mistake << endl;
		//
		Train();
		cout << "w5 after training: - " << o_layer->layer->weights[0] << endl;
		cout << "w6 after training - " << o_layer->layer->weights[1] << endl;
		cout << "w1, w2, w3, w4 after training - ";
		for (int i = 0; i < 2; ++i) {
			cout << h_layer->layer[0].weights[i] << " ";
		}
		for (int i = 0; i < 2; ++i) {
			cout << h_layer->layer[1].weights[i] << " ";
		}
		cout << endl;
	}
	void Train() {
		double* local_delta_h = new double[2];
		double delta_before = 0;
		for (int i = 0; i < 2; ++i) {
			double output_delta = (ideal - o_layer->layer->output_value)*
				((1 - o_layer->layer->output_value)*o_layer->layer->output_value);
			double h1_delta = ((1 - h_layer->layer[0].output_value)*h_layer->layer[i].output_value)*
				(o_layer->layer->weights[i] * output_delta);
			double GRADw5 = h_layer->layer[i].output_value*output_delta;
			double w5_delta = EPSYLON*GRADw5 + ALPHA*last_deltaw5w6[i];
			last_deltaw5w6[i] = w5_delta;
			double new_weigth = o_layer->layer->weights[i] + w5_delta;
			o_layer->layer->weights[i] = new_weigth;
			//Delta H1 and Delta H2. Using for calculate w1-w4.
			local_delta_h[i] = h1_delta;
		}
		for (int i = 0; i < 1; ++i) {
			double GradWI = i_layer->layer[0].output_value*local_delta_h[0];
			double delta_wi = EPSYLON*GradWI + last_deltaw1w3[0] * ALPHA;
			double new_weigth = h_layer->layer[0].weights[0] + delta_wi;
			h_layer->layer[0].weights[0] = new_weigth;
			last_deltaw1w3[0] = delta_wi;
			GradWI = i_layer->layer[1].output_value*local_delta_h[1];
			double _delta_wi = EPSYLON*GradWI + last_deltaw1w3[1] * ALPHA;
			double _new_weigth = h_layer->layer[0].weights[1] + _delta_wi;
			h_layer->layer[0].weights[1] = _new_weigth;
			last_deltaw1w3[1] = _delta_wi;
		}
		for (int i = 0; i < 1; ++i) {
			double GradWI = i_layer->layer[1].output_value*local_delta_h[0];
			double delta_wi = EPSYLON*GradWI + last_deltaw2w4[0] * ALPHA;
			double new_weigth = h_layer->layer[1].weights[0] + delta_wi;
			h_layer->layer[1].weights[0] = new_weigth;
			last_deltaw2w4[0] = delta_wi;
			double NGradWI = i_layer->layer[1].output_value*local_delta_h[1];
			delta_wi = EPSYLON*NGradWI + last_deltaw2w4[1] * ALPHA;
			new_weigth = h_layer->layer[1].weights[1] + delta_wi;
			h_layer->layer[1].weights[1] = new_weigth;
			last_deltaw2w4[1] = delta_wi;
		}
	}
	void Test() {
		set_action();
		cout << "NETWORK BEFORE LERANING:" << endl;
		h_layer->layer[0].weights[0] = 0.3;
		h_layer->layer[0].weights[1] = 1.3;
		h_layer->layer[1].weights[0] = 0.5;
		h_layer->layer[1].weights[1] = 0.1;
		o_layer->layer[0].weights[0] = 0.5;
		o_layer->layer[0].weights[1] = 0.1;
		CData();
		cout << "GETING 1000000 LESSONS FOR NETWORK:" << endl;
		_getch();
		for (int i = 0; i < 999999; ++i) {
			Train();
		}
		cout << endl << "NETWORK AFTER LEARNING:" << endl;
		CData();
	}
};

int main()
{
	Network network;
	network.Test();
	_getch();
    return 0;
}

