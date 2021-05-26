typedef struct caminho Caminho;

Caminho* newCaminho(char* val, char* caminho);

struct arvore;

struct arvore* get_subcaminhos(Caminho* caminho);

void set_subcaminhos(Caminho* caminho, struct arvore* arvore);

void set_val(Caminho* caminho, char* val);

char* get_val(Caminho* caminho);

char* get_cam(Caminho* caminho);

void deleteCaminho(Caminho* caminho);
