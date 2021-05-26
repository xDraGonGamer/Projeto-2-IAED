#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "caminho.h"

typedef struct arvore{ /*lista de caminhos*/
	Caminho **caminhos;
}Arvore;

Arvore *initArvore(int size){ /*cria uma nova arvore de tamanho size*/
	int i;
	Arvore *arvore = (Arvore*) malloc(sizeof(Arvore));
	arvore->caminhos = (Caminho**) malloc(size * sizeof(Caminho*));
	for(i = 0; i < size; i++){ /*inicia todos os caminho a NULL*/
		arvore->caminhos[i] = NULL;
	}
	return arvore;
}

void deleteTree(Arvore* arvore, int size){
	int i;
	for(i = 0; i < size; i++){
		free(arvore->caminhos[i]);
	}
	free(arvore);
}

Caminho* get_caminho(Arvore* arvore, int index){ /*retorna o ponteiro para o caminho 
						na posicao index de arvore*/
	return arvore->caminhos[index];
}

void set_caminho(Arvore* arvore, int index, Caminho* caminho){
	arvore->caminhos[index] = caminho; /*coloca o ponteiro caminho na posicao index
					     da arvore*/
}

void add_ordered_caminho(Arvore* arvore, Caminho* caminho){ /*adiciona caminho no final 
								da arvore*/
	int i = 0;
	while(arvore->caminhos[i] != NULL){
		i++;
	}
	arvore->caminhos[i] = caminho;
}

Caminho* get_caminho_ordenado(Arvore* arvore, char* caminho, int ncaminhos){
	int i = 0;
	if(arvore == NULL){ 
		return NULL;
	}
	while(i < ncaminhos){
		if(arvore->caminhos[i] == NULL){
			return NULL;
		}
		if(strcmp(get_cam(arvore->caminhos[i]), caminho) == 0){
			break;
		}
		i++;
	}
	return arvore->caminhos[i];
}
