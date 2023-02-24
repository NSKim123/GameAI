#include <vector>
using namespace std;
#ifndef SBS_NN_H__
#define SBS_NN_H__

#define NONE_ACTIVE -1
#define SIGMOID 0
#define RELU 1
#define SOFTMAX 2

#define INPUT_LAYER 0
#define DENSE_LAYER 1
#define OUTPUT_LAYER 2

int		CPU_MUTI_THREAD_NUM = 0;

void ReLU(float* u, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (u[i] <= 0.0)
			u[i] = 0.0;
		else
			u[i] *= 0.1f;
	}
}

void SoftMax(float* u, int size)
{
	float t = 0;
	float maxu = -9999999999;
	for (int i = 0; i < size; i++)
	{
		if (u[i] >= maxu)
			maxu = u[i];
	}
	for (int i = 0; i < size; i++)
	{
		t += exp(u[i] - maxu);
	}
	if (t == 0.0)
		return;
	for (int i = 0; i < size; i++)
	{
		u[i] = exp(u[i] - maxu) / t;
	}
}


void Sigmoid(float* u, int size)
{
	for (int i = 0; i < size; i++)
	{
		u[i] = 1.0f / (1.0f + exp(-u[i]));
	}
}


class NeuralLayer
{

private:
	int			m_inNodeSize;
	int			m_outNodeSize;
	int			m_Activefunc;
	int			m_Layer_Level;

	float* m_b;
	float* m_x;
	float* m_y;
	float* m_W;
	float* m_delta;
	float* m_grad_b;
	float* m_grad_x;
	float* m_grad_w;

public:
	NeuralLayer(int layer_level = INPUT_LAYER, int inNodeSize = 1, int outNodeSize = 1, int active = NONE_ACTIVE)
	{
		m_Layer_Level = layer_level;
		m_Activefunc = active;

		m_inNodeSize = inNodeSize;
		m_outNodeSize = outNodeSize;

		m_b = NULL;
		m_x = NULL;
		m_y = NULL;
		m_delta = NULL;
		m_grad_b = NULL;
		m_grad_x = NULL;
		m_W = NULL;
		m_grad_w = NULL;

		m_W = (float*)malloc(sizeof(float) * inNodeSize * outNodeSize);
		m_grad_w = (float*)malloc(sizeof(float) * inNodeSize * outNodeSize);

		m_x = (float*)malloc(sizeof(float) * inNodeSize);
		m_grad_x = (float*)malloc(sizeof(float) * inNodeSize);

		m_b = (float*)malloc(sizeof(float) * outNodeSize);
		m_y = (float*)malloc(sizeof(float) * outNodeSize);
		m_delta = (float*)malloc(sizeof(float) * outNodeSize);
		m_grad_b = (float*)malloc(sizeof(float) * outNodeSize);

		memset(m_x, 0, sizeof(float) * inNodeSize);
		memset(m_grad_x, 0, sizeof(float) * inNodeSize);
		memset(m_y, 0, sizeof(float) * outNodeSize);
		memset(m_delta, 0, sizeof(float) * outNodeSize);
		memset(m_grad_b, 0, sizeof(float) * outNodeSize);
		for (int i = 0; i < (inNodeSize * outNodeSize); i++)
		{
			m_W[i] = (((float)(rand() % 9000) - 4500.0f) / 4500.0f);
			m_grad_w[i] = 0.0;
		}


		for (int i = 0; i < outNodeSize; i++)
			m_b[i] = (((float)(rand() % 9000) - 4500.0f) / 4500.0f);
	}

	~NeuralLayer()
	{
		free(m_grad_w);
		m_grad_w = NULL;
		free(m_W);
		m_W = NULL;
		free(m_grad_x);
		m_grad_x = NULL;
		free(m_grad_b);
		m_grad_b = NULL;
		free(m_delta);
		m_delta = NULL;
		free(m_y);
		m_y = NULL;
		free(m_x);
		m_x = NULL;
		free(m_b);
		m_b = NULL;
	}

	float* Get_ydata()
	{
		return m_y;
	}

	float* Get_grad_x()
	{
		return m_grad_x;
	}

	int Get_outNodeSize()
	{
		return m_outNodeSize;
	}
	int Get_inNodeSize()
	{
		return m_inNodeSize;
	}
	void forward(float* ix)
	{
		memcpy(m_x, ix, sizeof(float) * m_inNodeSize);
		memset(m_y, 0, sizeof(float) * m_outNodeSize);

		for (int i = 0; i < m_outNodeSize; i += 1)
		{
			for (int j = 0; j < m_inNodeSize; j++)
			{
				int id = i * m_inNodeSize + j;
				m_y[i] += m_x[j] * m_W[id];
			}
			m_y[i] += m_b[i];
		}

		if (m_Activefunc == SIGMOID)
			Sigmoid(m_y, m_outNodeSize);
		if (m_Activefunc == RELU)
			ReLU(m_y, m_outNodeSize);
		if (m_Activefunc == SOFTMAX)
			SoftMax(m_y, m_outNodeSize);
	}
	void backward(float* igrad_y)
	{
		memset(m_delta, 0, sizeof(float) * m_outNodeSize);
		memset(m_grad_x, 0, sizeof(float) * m_inNodeSize);
		memset(m_grad_w, 0, sizeof(float) * m_inNodeSize * m_outNodeSize);

		if (m_Layer_Level != OUTPUT_LAYER)
		{
			if (m_Activefunc == SIGMOID)
			{
				for (int i = 0; i < m_outNodeSize; i++)
					m_delta[i] = igrad_y[i] * (1.0f - m_y[i]) * m_y[i];
			}

			if (m_Activefunc == RELU)
			{
				for (int i = 0; i < m_outNodeSize; i++)
				{
					if (m_y[i] > 0)
						m_delta[i] = igrad_y[i];
					else
						m_delta[i] = 0.0f;
				}
			}

			if (m_Activefunc == SOFTMAX || m_Activefunc == NONE_ACTIVE)
			{
				for (int i = 0; i < m_outNodeSize; i++)
					m_delta[i] = m_y[i] - igrad_y[i];
			}
		}
		else
		{
			for (int i = 0; i < m_outNodeSize; i++)
				m_delta[i] = m_y[i] - igrad_y[i];
		}
		memcpy(m_grad_b, m_delta, sizeof(float) * m_outNodeSize);

		for (int i = 0; i < m_inNodeSize; i += 1)
		{
			for (int j = 0; j < m_outNodeSize; j++)
			{
				int id = j * m_inNodeSize + i;
				m_grad_w[id] += m_x[i] * m_delta[j];
			}
		}

		for (int i = 0; i < m_inNodeSize; i += 1)
		{

			for (int j = 0; j < m_outNodeSize; j++)
			{
				int id = j * m_inNodeSize + i;
				m_grad_x[i] += m_delta[j] * m_W[id];
			}
		}
	}
	void update(float eta = 0.01)
	{
		for (int i = 0; i < m_inNodeSize; i += 1)
		{
			for (int j = 0; j < m_outNodeSize; j++)
			{
				int id = i * m_outNodeSize + j;
				m_W[id] -= eta * m_grad_w[id];
			}
		}
		for (int i = 0; i < m_outNodeSize; i += 1)
		{
			m_b[i] -= eta * m_grad_b[i];
		}
	}
};

struct NODEINFO
{
	int Layer_Type;
	int Node_count;
	int Activefunc;
};

class SBSNeuralNetwork
{

private:
	vector<NeuralLayer*>		m_Model;
	vector<NODEINFO>			m_listLayerinfo;
public:
	SBSNeuralNetwork()
	{
		m_Model.clear();
		m_listLayerinfo.clear();
	}
	~SBSNeuralNetwork()
	{
	}

	void AddLayer(int Layer_Type = DENSE_LAYER, int Node_count = 1, int Activefunc = NONE_ACTIVE)
	{
		NODEINFO a;
		a.Layer_Type = Layer_Type;
		a.Node_count = Node_count;
		a.Activefunc = Activefunc;
		m_listLayerinfo.push_back(a);
	}

	char* Get_LayerTy(int ty)
	{
		if (ty == INPUT_LAYER)
			return "INPUT";
		else if (ty == DENSE_LAYER)
			return "DENSE";
		else if (ty == OUTPUT_LAYER)
			return "OUTPUT";
		else
			return "NONE";
	}

	char* Get_ActiveTy(int ty)
	{
		if (ty == NONE_ACTIVE)
			return "NONE";
		else if (ty == RELU)
			return "RELU";
		else if (ty == SIGMOID)
			return "SIGMOID";
		else if (ty == SOFTMAX)
			return "SOFTMAX";
		else
			return "NONE";
	}
	void PrintModelInfo()
	{
		printf("Model Information: \n");
		for (int i = 0; i < m_listLayerinfo.size(); i++)
		{
			NODEINFO a = m_listLayerinfo[i];
			printf("==============================================================\n");
			printf("Layer Type: [%s] , Node Count:[%d] , Active_Type:[%s]  \n", Get_LayerTy(a.Layer_Type), a.Node_count, Get_ActiveTy(a.Activefunc));
			printf("==============================================================\n");
		}
		printf("Model Build!! \n\n");
	}

	void Build(bool isprintinfo = true)
	{
		if (m_listLayerinfo.size() == 0)
		{
			printf("Model Layer is  empty! \n");
			return;
		}
		for (int i = 1; i < m_listLayerinfo.size(); i++)
		{
			NODEINFO a = m_listLayerinfo[i - 1];
			NODEINFO a1 = m_listLayerinfo[i];
			m_Model.push_back(new NeuralLayer(a.Layer_Type, a.Node_count, a1.Node_count, a1.Activefunc));
		}
		if (isprintinfo)
			PrintModelInfo();
	}

	void Fit(float* ix, float* iy)
	{
		if (m_Model.size() == 0)
		{
			printf("Model Layer is  empty! \n");
			return;
		}

		m_Model[0]->forward(ix);
		for (int j = 1; j < m_Model.size(); j++)
			m_Model[j]->forward(m_Model[j - 1]->Get_ydata());

		m_Model[m_Model.size() - 1]->backward(iy);
		for (int j = m_Model.size() - 2; j >= 0; j--)
			m_Model[j]->backward(m_Model[j + 1]->Get_grad_x());

		for (int j = 0; j < m_Model.size(); j++)
			m_Model[j]->update();
	}

	float* Predict(float* ix)
	{
		if (m_Model.size() == 0)
		{
			printf("Model Layer is  empty! \n");
			return NULL;
		}
		m_Model[0]->forward(ix);
		for (int j = 1; j < m_Model.size(); j++)
			m_Model[j]->forward(m_Model[j - 1]->Get_ydata());
		return m_Model[m_Model.size() - 1]->Get_ydata();

	}

	int Get_Input_Vector_Size()
	{
		if (m_Model.size() == 0)
		{
			printf("Model Layer is  empty! \n");
			return -1;
		}

		return m_Model[0]->Get_inNodeSize();
	}

	int Get_Output_Vector_Size()
	{
		if (m_Model.size() == 0)
		{
			printf("Model Layer is  empty! \n");
			return -1;
		}

		return m_Model[m_Model.size() - 1]->Get_outNodeSize();
	}

	void Release()
	{
		for (int i = 0; i < m_Model.size(); i++)
			delete m_Model[i];
		m_Model.clear();
		m_listLayerinfo.clear();

	}

};

 

#endif