
struct _Transiciones{
  Estados *estados;
  char **nombre_estado_i;
  char **simbolo_entrada;
  char **nombre_estado_f;
  int next;
};

Transiciones *crearTransiciones(Estados *est){

Transiciones *trans= (Transiciones*)malloc(sizeof(Transiciones));
trans->est=est;
trans->next=0;
trans->nombre_estado_f=(char**)malloc(sizeof(char*));
trans->nombre_estado_i=(char**)malloc(sizeof(char*));
trans->simbolo_entrada=(char**)malloc(sizeof(char*));

return trans;
}


int insertaTransicion(Transiciones *trans, char *ini, char *fin, char *simbolo){

if(!char || !fin || !simbolo) return ERROR;

if(trans->next>0){
  trans->nombre_estado_f=(char**)realloc(trans->nombre_estado_f, next*sizeof(char*));
  trans->nombre_estado_i=(char**)realloc(trans->nombre_estado_i, next*sizeof(char*));
  trans->simbolo_entrada=(char**)realloc(trans->simbolo_entrada, next*sizeof(char*));
}


trans->nombre_estado_f[trans->next]=(char*)malloc((strlen(fin)+1)*sizeof(char*));
if(!trans->nombre_estado_f[trans->next]) return ERROR;
strcpy(trans->nombre_estado_f[trans->next], fin);

trans->nombre_estado_i[trans->next]=(char*)malloc((strlen(ini)+1)*sizeof(char*));
if(!trans->nombre_estado_i[trans->next]) return ERROR;
strcpy(trans->nombre_estado_i[trans->next], ini);

trans->simbolo_entrada[trans->next]=(char*)malloc((strlen(simbolo)+1)*sizeof(char*));
if(!trans->simbolo_entrada[trans->next]) return ERROR;
strcpy(trans->simbolo_entrada[trans->next], simbolo);

trans->next++;
return OK;
}

char *getEstadoFinal(Transiciones *trans, char *incial, char *simbolo){
int i, j;

for(i=0; i<trans->next; i++){
  for(j=0; j<trans->next; j++){
    if(strcmp(trans->nombre_estado_i[i], inicial)==0){
      if(strcmp(trans->simbolo_entrada[j], simbolo)==0)
      return trans->nombre_estado_f[trans->next];
    }
  }
}
return NULL;
}
