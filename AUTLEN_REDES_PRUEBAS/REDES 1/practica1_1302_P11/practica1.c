/***************************************************************************
EjemploPcapP1.c
Muestra el tiempo de llegada de los primeros 50 paquetes a la interface eth0
y los vuelca a traza nueva (�correctamente?) con tiempo actual

 Compila: gcc -Wall -o EjemploPcapP1 EjemploPcapP1.c -lpcap
 Autor: Jose Luis Garcia Dorado
 2018 EPS-UAM
***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "practica1.h"


#define ETH_FRAME_MAX 1514	// Tamano maximo trama ethernet

pcap_t *descr=NULL,*descr2=NULL;
pcap_dumper_t *pdumper=NULL;
int N;

void handle(int nsignal){
	printf("Control C pulsado\n");
	if(descr)
		pcap_close(descr);
	if(descr2)
		pcap_close(descr2);
	if(pdumper)
		pcap_dump_close(pdumper);
	exit(OK);
 }
  
void fa_nuevo_paquete(uint8_t *usuario, const struct pcap_pkthdr* cabecera, const uint8_t* paquete){
	int* num_paquete=(int *)usuario;
	(*num_paquete)++;
	struct pcap_pkthdr* aux=(struct pcap_pkthdr*) cabecera;
	int i, status, status_aux;


	if(N>cabecera->len){
		status=1;
		status_aux=N;
		N=cabecera->len;
	}

	//Avanzar 30 minutos la fecha
	aux->ts.tv_sec=cabecera->ts.tv_sec;
 	aux->ts.tv_sec+=60*30;
	aux->ts.tv_usec=cabecera->ts.tv_usec;
	aux->len=cabecera->len;
	aux->caplen=cabecera->caplen;
	printf("Nuevo paquete capturado a las %s",ctime((const time_t*)&(cabecera->ts.tv_sec)));
	if(pdumper){
		pcap_dump((uint8_t *)pdumper, aux, paquete);
	}
		printf("Primeros %d bits del paquete \n", N);
		for ( i=0; i<N; i++){
			printf("%2.2x ", ((uint8_t*)paquete)[i]);
		}
	printf("\n\n");
	if(status==1){
	N=status_aux;
	}

}

int main(int argc, char **argv)
{
	int retorno=0, contador=0;
	char errbuf[PCAP_ERRBUF_SIZE];
	char file_name[256]="";
	struct timeval time;

	if(signal(SIGINT,handle)==SIG_ERR){
		printf("Error: Fallo al capturar la senal SIGINT.\n");
		exit(ERROR);
	}

		//ejecución sin argumentos
	if(argc==1){

		printf("Si se ejecuta con un argumento, consideramos que queremos capturar de interfaz:\n"
    		"El programa debe mostrar el número de paquetes recibidos por la interfaz de red eth0 tras pulsar Control-C.\n"
   		"El programa debe almacenar los paquetes capturados enteros en una traza con nombre eth0.FECHA.pcap(donde FECHA será" 			" el tiempo actual UNIX en segundos).\n"
    		"Al almacenar la traza queremos modificar la fecha de cada paquete capturado. La modificación consistirá en sumar 30" 			" minutos a la fecha de captura. Ejemplo: si capturamos el día 20 de octubre a las 10:23, deberíamos observar en la" 			" traza almacenada los paquetes con fecha del 20 de octubre a las 10:53.\n");

		printf("\nSi se ejecuta con dos argumentos (el segundo será la traza a analizar), consideramos que queremos analizar" 			" una traza pcap. El programa debe mostrar el número de paquetes de la traza al finalizar su ejecución.\n");
		
		exit(ERROR);
	}

	N=atoi(argv[1]); 
		//ejecución con 1 argumento
	if(argc==2){

			//Apertura de interface
   		if ((descr = pcap_open_live("eth0",N,0,100, errbuf)) == NULL){
			printf("Error: pcap_open_live(): %s, %s %d.\n",errbuf,__FILE__,__LINE__);
			exit(ERROR);
		}
			//Para volcado de traza
		descr2=pcap_open_dead(DLT_EN10MB,ETH_FRAME_MAX);
		if (!descr2){
			printf("Error al abrir el dump.\n");
			pcap_close(descr);
			exit(ERROR);
		}
		gettimeofday(&time,NULL);

		sprintf(file_name,"captura.eth0.%lld.pcap", (long long)time.tv_sec);
		printf("Nombre de la traza: %s\n", file_name);
		pdumper=pcap_dump_open(descr2,file_name);
		if(!pdumper){
			printf("Error al abrir el dumper: %s, %s %d.\n",pcap_geterr(descr2),__FILE__,__LINE__);
			pcap_close(descr);
			pcap_close(descr2);
			exit(ERROR);
		}



	}
		//ejecucion con 2 argumentos
	if(argc==3){

		if ((descr= pcap_open_offline(argv[2], errbuf)) == NULL){
			printf("Error: pcap_open_live(): %s, %s %d.\n",errbuf,__FILE__,__LINE__);
			exit(ERROR);
		}
	}



		//Se pasa el contador como argumento, pero sera mas comodo y mucho mas habitual usar variables globales
		retorno = pcap_loop (descr, -1, fa_nuevo_paquete, (uint8_t*)&contador);

		if(retorno == -1){ 		//En caso de error
			printf("Error al capturar un paquete %s, %s %d.\n",pcap_geterr(descr),__FILE__,__LINE__);
			pcap_close(descr);
			if(argc==2){
				pcap_close(descr2);
				pcap_dump_close(pdumper);
			}
			exit(ERROR);
		}
		else if(retorno==-2){ //pcap_breakloop() no asegura la no llamada a la funcion de atencion para paquetes ya en el 						buffer
			printf("Llamada a %s %s %d.\n","pcap_breakloop()",__FILE__,__LINE__);
		}
		else if(retorno == 0){ //Se leyo la traza entera o se supero el limite
			printf("No mas paquetes o limite superado %s %d.\n",__FILE__,__LINE__);

		}

	

	printf("\nNumero de paquetes leidos: %d \n", contador);
	pcap_close(descr);
	if(argc==2){
		pcap_close(descr2);
		pcap_dump_close(pdumper);
	}

	return OK;
}
