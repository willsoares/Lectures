#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * Retorna um valor contido no intervalo entre (0,1]
 * @return 
 */
double aleatorio()
{
	//resultado sera algo entre [0,0.999999...] proximo a 1.0
	double u = rand() / ((double)RAND_MAX + 1);
	//limitando entre (0,1]
	u = 1.0 - u;
	return u;
}

double minimo(double n1, double n2)
{
	if (n1 < n2)
		return n1;
	return n2;
}

/**
 * Simulador de um caixa onde clientes cheguem em média a cada 10 segundos,
 * e o caixa gaste em média 8 segundos para atender cada cliente.
 * 
 * Utilização ou Ocupação = fração de tempo que o caixa permanecerá ocupado.
 * Utilização = 0.8.
 * 
 * o tempo entre a chegada de clientes, bem como o tempo de atendimento devem 
 * ser gerados de maneira pseudoaleatoria através da v.a. exponencial.
 */
int main()
{
	double soma_atendimentos = 0.0;
	double soma_areas = 0.0;
	double soma_area_entrada = 0.0;
	double soma_area_saida = 0.0;
	double eventos_entrada = 0.0;
	double eventos_saida = 0.0;

	srand(time(NULL));

	double tempo_medio_clientes;
	printf("Informe o tempo medio entre a chegada de clientes (segundos): ");
	scanf("%lF", &tempo_medio_clientes);
	tempo_medio_clientes = 1.0 / tempo_medio_clientes;

	double tempo_medio_atendimento;
	printf("Informe o tempo medio gasto para atender cada cliente (segundos): ");
	scanf("%lF", &tempo_medio_atendimento);
	tempo_medio_atendimento = 1.0 / tempo_medio_atendimento;

	//tempo decorrido tempo da simulacao
	double tempo;

	double tempo_simulacao;
	printf("Informe o tempo total de simulacao (segundos): ");
	scanf("%lF", &tempo_simulacao);

	//armazena o tempo de chegada do proximo cliente
	double chegada_cliente = (-1.0 / tempo_medio_clientes) * log(aleatorio());

	//armazena o tempo em que o cliente que estiver em atendimento saira do comercio
	//saida_atendimento == 0.0 indica caixa ocioso
	double saida_atendimento = 0.0;

	double fila = 0.0;

	//logica da simulacao
	while (tempo <= tempo_simulacao)
	{
		//nao existe cliente sendo atendido no momento atual,
		//de modo que a simulacao pode avancar no tempo para
		//a chegada do proximo cliente
		if (!saida_atendimento)
			tempo = chegada_cliente;
		else
		{
			tempo = minimo(chegada_cliente, saida_atendimento);
		}

		if (tempo == chegada_cliente)
		{
			printf("Chegada de cliente: %lF\n", chegada_cliente);
			//evento de chegada de cliente
			fila++;
			eventos_entrada++;
			printf("fila: %lF\n", fila);
			//indica que o caixa esta ocioso
			//logo, pode-se comecar a atender
			//o cliente que acaba de chegar
			if (saida_atendimento == 0.0)
			{
				saida_atendimento = tempo;
			}

			//gerar o tempo de chegada do proximo cliente
			double tempo_anterior = chegada_cliente;
			chegada_cliente = tempo + (-1.0 / tempo_medio_clientes) * log(aleatorio());
			soma_areas += (chegada_cliente - tempo_anterior) * (fila - 1);
        	soma_area_entrada += (chegada_cliente - tempo_anterior) * eventos_entrada;
		}
		else
		{
			//evento de saida de cliente
			//a cabeca da fila nao consiste no cliente em atendimento.
			//o cliente que comeca a ser atendido portanto, sai da fila,
			//e passa a estar ainda no comercio, mas em atendimento no caixa.

			//verifico se ha cliente na fila
			if (fila)
			{
				fila--;
				eventos_saida++;
				printf("fila: %lF\n", fila);
				double tempo_atendimento = (-1.0 / tempo_medio_atendimento) * log(aleatorio());

				saida_atendimento = tempo + tempo_atendimento;
				soma_atendimentos += tempo_atendimento;
				soma_areas += tempo_atendimento * (fila - 1);
				soma_area_saida += tempo_atendimento * eventos_saida;
				printf("saida de cliente: %lF\n", saida_atendimento);
			}
			else
			{
				double tempo_ocioso = chegada_cliente - saida_atendimento;
				printf("Tempo ocioso: %lF\n", tempo_ocioso);
				saida_atendimento = 0.0;
			}
		}
		printf("==================\n");
	}

	if (saida_atendimento > tempo)
	{
		soma_atendimentos -= saida_atendimento - tempo;
		soma_areas -= saida_atendimento - tempo;
	}

	double taxa_atualizacao = (soma_atendimentos / tempo) * 100;
	double e_n = soma_areas / tempo;
	double e_w = (soma_area_entrada - soma_area_saida) / eventos_entrada;

	printf("Taxa de utilizacao: %lF\n", taxa_atualizacao);
	printf("E[N]: %lF\n", e_n);
	printf("E[W]: %lF\n", e_w);

	getchar();
	getchar();

	return (EXIT_SUCCESS);
}
