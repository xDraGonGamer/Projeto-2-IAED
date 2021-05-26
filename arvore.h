typedef struct arvore Arvore;

Arvore *initArvore(int size);

struct caminho;

struct caminho* get_caminho(Arvore* arvore, int index);

void set_caminho(Arvore* arvore, int index, struct caminho* caminho);

struct caminho* get_caminho_ordenado(Arvore* arvore, char* caminho, int ncaminhos);

void add_ordered_caminho(Arvore* arvore, struct caminho* caminho);

void deleteTree(Arvore* arvore, int size);
