#include "arvore.h"
#include <stdlib.h>
#include <string.h>

typedef struct caminho{
	char *val;
	char *caminho;
	Arvore *subcaminhos;
}Caminho;

Caminho* newCaminho(char* val, char* cam){ /*cria um novo caminho de valor val e caminho cam*/
	Caminho* caminho;
	char *valor, *camStr;
	camStr = (char*) malloc(strlen(cam));
	caminho = (Caminho*) malloc(sizeof(Caminho));
	if(val == NULL){
		caminho->val = NULL;
	}
	else{
		valor = (char*) malloc(strlen(val));
		strcpy(valor, val);
		caminho->val = valor;
	}
	strcpy(camStr, cam);
	caminho->caminho = camStr;
	caminho->subcaminhos = NULL;
	return caminho;
}

Arvore* get_subcaminhos(Caminho* caminho){ /*retorna os subcaminhos do caminho caminho*/
	return caminho->subcaminhos;
}

void deleteCaminho(Caminho* caminho){
	free(caminho->val);
	free(caminho->caminho);
	if(caminho->subcaminhos == NULL){
		free(caminho->subcaminhos);
	}
}

void set_subcaminhos(Caminho* caminho, struct arvore* arvore){
	caminho->subcaminhos = arvore; /*altera os subcaminhos de caminho para arvore*/
}

void set_val(Caminho* caminho, char* val){ /*altera o valor do caminho*/
	char* aux = (char*) malloc(strlen(val));
	strcpy(aux, val);
	caminho->val = aux;
}

char* get_val(Caminho* caminho){ /*retorna o valor do caminho*/
	return caminho->val;
}

char* get_cam(Caminho* caminho){ /*retorna o caminho do caminho*/
        if(caminho == NULL){
		return NULL;
	}
	return caminho->caminho;
}
