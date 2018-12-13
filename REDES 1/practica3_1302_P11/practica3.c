/***************************************************************************
 practica3.c
 Inicio, funciones auxiliares y modulos de transmision implmentados y a implementar de la practica 4.
Compila con warning pues falta usar variables y modificar funciones

 Compila: make
 Autor: Jose Luis Garcia Dorado, Jorge E. Lopez de Vergara Mendez
 2018 EPS-UAM v1
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "interface.h"
#include "practica3.h"

/***************************Variables globales utiles*************************************************/
pcap_t* descr, *descr2; //Descriptores de la interface de red
pcap_dumper_t * pdumper;//y salida a pcap
uint64_t cont=0;	//Contador numero de mensajes enviados
char interface[10];	//Interface donde transmitir por ejemplo "eth0"
uint16_t ID=1;		//Identificador IP


void handleSignal(int nsignal){
	printf("Control C pulsado (%"PRIu64")\n", cont);
	pcap_close(descr);
	exit(OK);
}

int main(int argc, char **argv){

	char errbuf[PCAP_ERRBUF_SIZE];
	char fichero_pcap_destino[CADENAS];
	uint8_t IP_destino_red[IP_ALEN];
	uint16_t MTU;
	uint16_t datalink;
	uint16_t puerto_destino;
	char data[IP_DATAGRAM_MAX];
	uint16_t pila_protocolos[CADENAS];


	int long_index=0;
	char opt;
	char flag_iface = 0, flag_ip = 0, flag_port = 0, flag_file = 0, flag_dontfrag = 0, flag_mostrar = 0;

	static struct option options[] = {
		{"if",required_argument,0,'1'},
		{"ip",required_argument,0,'2'},
		{"pd",required_argument,0,'3'},
		{"f",required_argument,0,'4'},
		{"d",no_argument,0,'5'},
		{"m",no_argument,0,'6'},
		{"h",no_argument,0,'7'},
		{0,0,0,0}
	};

		//Dos opciones: leer de stdin o de fichero, adicionalmente para pruebas si no se introduce argumento se considera que el mensaje es "Payload "
	while ((opt = getopt_long_only(argc, argv,"1:2:3:4:5:6:7", options, &long_index )) != -1) {
		switch (opt) {

			case '1' :

				flag_iface = 1;
				//Por comodidad definimos interface como una variable global
				sprintf(interface,"%s",optarg);
				break;

			case '2' :

				flag_ip = 1;
				//Leemos la IP a donde transmitir y la almacenamos en orden de red
				if (sscanf(optarg,"%"SCNu8".%"SCNu8".%"SCNu8".%"SCNu8"",
				                   &(IP_destino_red[0]),&(IP_destino_red[1]),&(IP_destino_red[2]),&(IP_destino_red[3])) != IP_ALEN){
					printf("Error: Fallo en la lectura IP destino %s\n", optarg);
					exit(ERROR);
				}

				break;

			case '3' :

				flag_port = 1;
				//Leemos el puerto a donde transmitir y la almacenamos en orden de hardware
				puerto_destino=atoi(optarg);
				break;

			case '4' :

				if(strcmp(optarg,"stdin")==0) {
					if (fgets(data, sizeof data, stdin)==NULL) {
						  	printf("Error leyendo desde stdin: %s %s %d.\n",errbuf,__FILE__,__LINE__);
						return ERROR;
					}
					sprintf(fichero_pcap_destino,"%s%s","stdin",".pcap");
				} else {
					sprintf(fichero_pcap_destino,"%s%s",optarg,".pcap");
					//TODO Leer fichero en data [...]
				}
				flag_file = 1;
				break;

			case '5' :
				flag_dontfrag =1; // El usuario solicita que los paquetes se envien con el bit DF=1.
				break;

			case '6' :
				flag_mostrar =1; // El usuario solicita que se muestren en hexadecimal las tramas enviadas.
				break;

			case '7' : printf("Ayuda. Ejecucion: %s -if interface -ip direccion_IP -pd puerto [-f /ruta/fichero_a_transmitir o stdin] [-d] [-m]: %d\n",argv[0],argc); exit(ERROR);
				break;

			case '?' :
			default: printf("Error. Ejecucion: %s -if interface -ip direccion_IP -pd puerto [-f /ruta/fichero_a_transmitir o stdin] [-d] [-m]: %d\n",argv[0],argc); exit(ERROR);
				break;
        }
    }

	if ((flag_iface == 0) || (flag_ip == 0) || (flag_port == 0)){
		printf("Error. Ejecucion: %s -if interface -ip direccion_IP -pd puerto [-f /ruta/fichero_a_transmitir o stdin] [-d] [-m]: %d\n",argv[0],argc);
		exit(ERROR);
	} else {
		printf("Interface:\n\t%s\n",interface);
		printf("IP:\n\t%"PRIu8".%"PRIu8".%"PRIu8".%"PRIu8"\n",IP_destino_red[0],IP_destino_red[1],IP_destino_red[2],IP_destino_red[3]);
		printf("Puerto destino:\n\t%"PRIu16"\n",puerto_destino);
		if (flag_dontfrag) printf("Se solicita enviar paquete con bit DF=1\n");
		if (flag_mostrar) printf("Se solicita mostrar las tramas enviadas en hexadecimal\n");
	}

	if (flag_file == 0) {
		sprintf(data,"%s","Payload "); //Deben ser pares!
		sprintf(fichero_pcap_destino,"%s%s","debugging",".pcap");
	}

	if(signal(SIGINT,handleSignal)==SIG_ERR){
		printf("Error: Fallo al capturar la senal SIGINT.\n");
		return ERROR;
	}
	//Inicializamos las tablas de protocolos
	if(inicializarPilaEnviar()==ERROR){
      	printf("Error leyendo desde stdin: %s %s %d.\n",errbuf,__FILE__,__LINE__);
		return ERROR;
	}


	//Leemos el tamano maximo de transmision del nivel de enlace
	if(obtenerMTUInterface(interface, &MTU)==ERROR)
		return ERROR;

	//Descriptor de la interface de red donde inyectar trafico
	if ((descr = pcap_open_live(interface,MTU+ETH_HLEN,0, 0, errbuf)) == NULL){
		printf("Error: pcap_open_live(): %s %s %d.\n",errbuf,__FILE__,__LINE__);
		return ERROR;
	}

	datalink=(uint16_t)pcap_datalink(descr); //DLT_EN10MB==Ethernet

	//Descriptor del fichero de salida pcap para debugging
	descr2=pcap_open_dead(datalink,MTU+ETH_HLEN);
	pdumper=pcap_dump_open(descr2,fichero_pcap_destino);

	//Formamos y enviamos el trafico, debe enviarse un unico segmento por llamada a enviar() aunque luego se traduzca en mas de un datagrama
	//Primero, un paquete ICMP; en concreto, un ping
	pila_protocolos[0]=ICMP_PROTO; pila_protocolos[1]=IP_PROTO; pila_protocolos[2]=0;
	Parametros parametros_icmp; parametros_icmp.tipo=PING_TIPO; parametros_icmp.codigo=PING_CODE; parametros_icmp.bit_DF=flag_dontfrag; memcpy(parametros_icmp.IP_destino,IP_destino_red,IP_ALEN);
	if(enviar((uint8_t*)ICMP_DATA,strlen(ICMP_DATA),pila_protocolos,&parametros_icmp)==ERROR ){
		printf("Error: enviar(): %s %s %d.\n",errbuf,__FILE__,__LINE__);
		return ERROR;
	}
	else	cont++;
	printf("Enviado mensaje %"PRIu64", ICMP almacenado en %s\n\n", cont,fichero_pcap_destino);
	//Luego, un paquete UDP
	//Definimos la pila de protocolos que queremos seguir
	pila_protocolos[0]=UDP_PROTO; pila_protocolos[1]=IP_PROTO; pila_protocolos[2]=ETH_PROTO;
	//Rellenamos los parametros necesario para enviar el paquete a su destinatario y proceso
	Parametros parametros_udp; memcpy(parametros_udp.IP_destino,IP_destino_red,IP_ALEN); parametros_udp.bit_DF=flag_dontfrag; parametros_udp.puerto_destino=puerto_destino;
	//Enviamos
	if(enviar((uint8_t*)data,strlen(data),pila_protocolos,&parametros_udp)==ERROR ){
		printf("Error: enviar(): %s %s %d.\n",errbuf,__FILE__,__LINE__);
		return ERROR;
	}
	else	cont++;

	printf("Enviado mensaje %"PRIu64", almacenado en %s\n\n\n", cont,fichero_pcap_destino);

		//Cerramos descriptores
	pcap_close(descr);
	pcap_dump_close(pdumper);
	pcap_close(descr2);
	return OK;
}


/****************************************************************************************
 * Nombre: enviar                                                                       *
 * Descripcion: Esta funcion envia un mensaje                                           *
 * Argumentos:                                                                          *
 *  -mensaje: mensaje a enviar                                                          *
 *  -pila_protocolos: conjunto de protocolos a seguir                                   *
 *  -longitud: bytes que componen mensaje                                               *
 *  -parametros: parametros necesario para el envio (struct parametros)                 *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t enviar(uint8_t* mensaje, uint32_t longitud,uint16_t* pila_protocolos,void *parametros){
	uint16_t protocolo=pila_protocolos[0];
printf("Enviar(%"PRIu16") %s %d.\n",protocolo,__FILE__,__LINE__);
	if(protocolos_registrados[protocolo]==NULL){
		printf("Protocolo %"PRIu16" desconocido\n",protocolo);
		return ERROR;
	}
	else {
		return protocolos_registrados[protocolo](mensaje,longitud,pila_protocolos,parametros);
	}
	return ERROR;
}


/***************************TODO Pila de protocolos a implementar************************************/


/****************************************************************************************
 * Nombre: moduloICMP                                                                   *
 * Descripcion: Esta funcion implementa el modulo de envio ICMP                         *
 * Argumentos:                                                                          *
 *  -mensaje: mensaje a anadir a la cabecera ICMP                                       *
 *  -longitud: bytes que componen el mensaje                                            *
 *  -pila_protocolos: conjunto de protocolos a seguir                                   *
 *  -parametros: parametros necesario para el envio este protocolo                      *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t moduloICMP(uint8_t* mensaje, uint32_t longitud, uint16_t* pila_protocolos, void *parametros){
	uint8_t segmento[ICMP_DATAGRAM_MAX]={0};
	uint8_t aux8=0;
	uint16_t aux16=0;
	uint32_t pos=0, pos_aux=0;
	uint8_t protocolo_inferior=pila_protocolos[1];
	uint8_t *arg=NULL;
	printf("modulo ICMP(%"PRIu16") %s %d.\n",protocolo_inferior,__FILE__,__LINE__);

	if(longitud>ICMP_DATAGRAM_MAX-ICMP_HLEN){
		printf("Tamaño demasiado grande");
		return ERROR;
	}

	aux8=PING_TIPO;
	memcpy(segmento+pos,&aux8,sizeof(uint8_t));
	pos+=sizeof(uint8_t);


	aux8=PING_CODE;
	memcpy(segmento+pos,&aux8,sizeof(uint8_t));
	pos+=sizeof(uint8_t);

	pos_aux=pos;

	pos+=sizeof(uint16_t);

	//Identificador
	aux16=htons(getpid());
	memcpy(segmento+pos,&aux16,sizeof(uint16_t));
	pos+=sizeof(uint16_t);

	//Número de secuencia
	aux16=htons(1);
	memcpy(segmento+pos,&aux16,sizeof(uint16_t));
	pos+=sizeof(uint16_t);


	memcpy(segmento+pos, mensaje, longitud);

	arg= (uint8_t*)malloc(sizeof(uint8_t));
	if (calcularChecksum(segmento, longitud+pos, arg)==ERROR){
		printf("Error en checksum ICMP\n");
		return ERROR;

	}
	memcpy(segmento+pos_aux, arg, sizeof(uint8_t));




//TODO rellenar el resto de campos de ICMP, incluyendo el checksum tras haber rellenado todo el segmento, incluyendo el mensaje
// El campo de identificador se puede asociar al pid, y el de secuencia puede ponerse a 1.
//[....]

//Se llama al protocolo definido de nivel inferior a traves de los punteros registrados en la tabla de protocolos registrados
	return protocolos_registrados[protocolo_inferior](segmento,longitud+pos,pila_protocolos,parametros);
}


/****************************************************************************************
 * Nombre: moduloUDP                                                                    *
 * Descripcion: Esta funcion implementa el modulo de envio UDP                          *
 * Argumentos:                                                                          *
 *  -mensaje: mensaje a enviar                                                          *
 *  -longitud: bytes que componen mensaje                                               *
 *  -pila_protocolos: conjunto de protocolos a seguir                                   *
 *  -parametros: parametros necesario para el envio este protocolo                      *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t moduloUDP(uint8_t* mensaje, uint32_t longitud, uint16_t* pila_protocolos, void *parametros){
	uint8_t segmento[UDP_SEG_MAX]={0};
	uint16_t puerto_origen = 0;
	uint16_t aux16=0;
	uint32_t pos=0;
	uint8_t protocolo_inferior=pila_protocolos[1];
	printf("modulo UDP(%"PRIu16") %s %d.\n",protocolo_inferior,__FILE__,__LINE__);

	if (longitud>UDP_SEG_MAX){
		printf("Error: mensaje demasiado grande para UDP (%d).\n",UDP_SEG_MAX);
		return ERROR;
	}

	Parametros udpdatos=*((Parametros*)parametros);
	uint16_t puerto_destino=udpdatos.puerto_destino;


	/*PUERTO DE ORIGEN*/
	obtenerPuertoOrigen(&puerto_origen);
	aux16=htons(puerto_origen);
	memcpy(segmento, &aux16, sizeof(uint16_t));
	pos+=sizeof(uint16_t);

	/*PUERTO DESTINO*/
	aux16=htons(puerto_destino);
	memcpy(segmento+pos,&aux16,sizeof(uint16_t));
	pos+=sizeof(uint16_t);

	/*LONGITUD*/
	aux16= htons(longitud+UDP_HLEN);
	memcpy(segmento+pos,&aux16,sizeof(uint16_t));
	pos+=sizeof(uint16_t);

	/*CHECKSUM*/
	aux16=htons(0);
	memcpy(segmento+pos,&aux16,sizeof(uint16_t));
	pos+=sizeof(uint16_t);

	/*OCTETO DE DATOS*/

	memcpy(segmento+pos, mensaje, longitud);

//TODO Completar el segmento [...]
//[...]
//Se llama al protocolo definido de nivel inferior a traves de los punteros registrados en la tabla de protocolos registrados
	return protocolos_registrados[protocolo_inferior](segmento,longitud+pos,pila_protocolos,parametros);
}


/****************************************************************************************
 * Nombre: moduloIP                                                                     *
 * Descripcion: Esta funcion implementa el modulo de envio IP                           *
 * Argumentos:                                                                          *
 *  -segmento: segmento a enviar                                                        *
 *  -longitud: bytes que componen el segmento                                           *
 *  -pila_protocolos: conjunto de protocolos a seguir                                   *
 *  -parametros: parametros necesario para el envio este protocolo                      *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t moduloIP(uint8_t* segmento, uint32_t longitud, uint16_t* pila_protocolos, void *parametros){
	uint8_t datagrama[IP_DATAGRAM_MAX]={0};
	uint32_t aux32=0;
	uint16_t aux16=0;
	uint8_t aux8=0;
	uint8_t *arg;
	uint32_t pos=0,pos_control=0, pos_flags=0, pos_long=0;
	uint8_t IP_origen[IP_ALEN]={0};
	uint8_t protocolo_superior=pila_protocolos[0];
	uint8_t protocolo_inferior=pila_protocolos[2];
	pila_protocolos++;
	uint8_t mascara[IP_ALEN]={0},IP_rango_origen[IP_ALEN]={0},IP_rango_destino[IP_ALEN]={0}, Gateway[IP_ALEN]={0}, MAC_DST[ETH_ALEN]={0};
	uint16_t MTU=htons(0);
	int cabecera_ip=20, num_paquetes=0;

	printf("modulo IP(%"PRIu16") %s %d.\n",protocolo_inferior,__FILE__,__LINE__);

	Parametros ipdatos=*((Parametros*)parametros);
	uint8_t* IP_destino=ipdatos.IP_destino;


	if(obtenerMTUInterface(interface, &MTU)==ERROR){
		return ERROR;
	}
	if(longitud>MTU && ipdatos.bit_DF==1){
		printf("No se puede fragmentar el archivo ya que por argumentos de entrada se pide que no lo hagamos.\n");
		return ERROR;
	}
	if(longitud>IP_DATAGRAM_MAX-cabecera_ip){
		printf("Tamaño demasiado grande para protocolo IP" );
		return ERROR;
	}

	if(obtenerIPInterface(interface, IP_origen)==ERROR){
		printf("No se pudo obtener IP Interface");
		return ERROR;
	}
	if(obtenerMascaraInterface(interface, mascara)==ERROR){
		printf("No se pudo obtener Mascara Interface");
		return ERROR;
	}
	aplicarMascara(IP_origen, mascara, IP_ALEN, IP_rango_origen);
	aplicarMascara(IP_destino, mascara, IP_ALEN, IP_rango_destino);
	int flag=0, i=0, resultado=0;
	for(i=0;i<IP_ALEN;i++){
		if(IP_rango_origen[i]!=IP_rango_destino[i]){
			flag=1;
		}
	}
	//Flag=1-> El destino no está en la misma subred que el origen; Flag=0->Sí lo está
	if(flag==1){
		if(obtenerGateway(interface, Gateway)==ERROR){
			printf("No se pudo obtener Gateway\n");
			return ERROR;
		}

		if(solicitudARP(interface, Gateway, ipdatos.ETH_destino)==ERROR){
			printf("No se pudo obtener ARP");
			return ERROR;
		}
	}
	else{
		if(solicitudARP(interface, IP_destino, ipdatos.ETH_destino)==ERROR){
			printf("No se pudo obtener ARP");
			return ERROR;
		}
	}



		//Rellenamos los campos de la cabecera ip
		//Version y IHL.
		aux8=0x45;
		memcpy(datagrama, &aux8, sizeof(uint8_t));
		pos+=1;

		//Tipo servicio
		aux16=0;
		memcpy(datagrama+pos, &aux16, sizeof(uint16_t));
		pos+=1;

		//Longitud total
		aux16=MTU+cabecera_ip;

		aux16=htons(aux16);
		memcpy(datagrama+pos, &aux16, sizeof(uint16_t));
		pos+=2;

		//Identificacion
		aux16=htons(ID);
		memcpy(datagrama+pos, &aux16, sizeof(uint16_t));
		pos+=2;

		//flags y posicion
		aux16=(i*(MTU-cabecera_ip))/8;
		aux16= htons(0x4000 | aux16);


		memcpy(datagrama+pos, &aux16, sizeof(uint16_t));
		pos+=2;

		//Tiempo de vida
		aux8=64;
		memcpy(datagrama+pos, &aux8, sizeof(uint8_t));
		pos+=1;

		//Protocolo
		printf("Usamos el protocolo %"PRIu16"", protocolo_superior);
		aux8=(uint8_t)protocolo_superior;
		memcpy(datagrama+pos, &aux8, sizeof(uint8_t));
		pos+=1;
		//EL checksum primero se deja a 0 y mas tarde se cambia.
		aux16=0;
		memcpy(datagrama+pos, &aux16, sizeof(uint16_t));
		pos_control=pos;
		pos+=2;

		//Ip origen e Ip destino.

		memcpy(datagrama+pos, IP_origen, sizeof(uint32_t));
		pos+=4;
		memcpy(datagrama+pos, IP_destino, sizeof(uint32_t));
		pos+=4;


		num_paquetes=((int)longitud/(int)MTU-cabecera_ip)+1;

		for(i=0; i<num_paquetes; i++){

		//Longitud
		aux16=MTU;
		aux16=htons(MTU);
		memcpy(datagrama+2, &aux16, sizeof(uint16_t));

		//flags y OFFSET
		aux16=(i*(MTU-cabecera_ip))/8;
		aux16= htons(0x2000 | aux16);
		memcpy(datagrama+6, &aux16, sizeof(uint16_t));
		pos+=2;

		//Checksum
		if (calcularChecksum(datagrama, cabecera_ip, (uint8_t*)(&aux16))==ERROR){
			printf("Error en checksum ICMP\n");
			return ERROR;
			}
		memcpy(datagrama+10, &aux16, sizeof(uint16_t));


		//Añadimos el mensaje
		memcpy(datagrama+pos, segmento, MTU-cabecera_ip);
		segmento+=(MTU-cabecera_ip);
		protocolos_registrados[protocolo_inferior](datagrama, longitud-resultado+pos, pila_protocolos, &ipdatos);
		}

	//Ultimo fragmento o sin fragmentación.

	//Longitud
	aux16= longitud%(MTU-cabecera_ip)+cabecera_ip;
	aux16=htons(aux16);
	memcpy(datagrama+2, &aux16, sizeof(uint16_t));

	//Flags y offset

	aux16=(i*(MTU-cabecera_ip))/8;
	aux16= htons(0x4000 | aux16);
	memcpy(datagrama+6, &aux16, sizeof(uint16_t));


		//Checksum

		if (calcularChecksum(datagrama, cabecera_ip, (uint8_t*)(&aux16))==ERROR){
			printf("Error en checksum ICMP\n");
			return ERROR;
			}
		memcpy(datagrama+10, &aux16, sizeof(uint16_t));

		//Añadimos el mensaje
		memcpy(datagrama+pos, segmento, MTU-cabecera_ip);

		mostrarHex(datagrama, longitud+cabecera_ip);


	return protocolos_registrados[protocolo_inferior](datagrama, longitud-resultado+pos, pila_protocolos, &ipdatos);
}


/****************************************************************************************
 * Nombre: moduloETH                                                                    *
 * Descripcion: Esta funcion implementa el modulo de envio Ethernet                     *
 * Argumentos:                                                                          *
 *  -datagrama: datagrama a enviar                                                      *
 *  -longitud: bytes que componen el datagrama                                          *
 *  -pila_protocolos: conjunto de protocolos a seguir                                   *
 *  -parametros: Parametros necesario para el envio este protocolo                      *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t moduloETH(uint8_t* datagrama, uint32_t longitud, uint16_t* pila_protocolos,void *parametros){

uint8_t trama[ETH_FRAME_MAX]={0};
uint8_t ETH_or [ETH_ALEN]={0};
uint16_t IP_type =0x0800;
IP_type= htons(IP_type);
uint16_t MTU=htons(0);
struct timeval tiempo;
struct pcap_pkthdr pcap;


printf("modulo ETH(fisica) %s %d.\n",__FILE__,__LINE__);

if(longitud>ETH_FRAME_MAX-ETH_HLEN){
	printf("Tamaño demasiado grande");
	return ERROR;
}

//TODO
//[...] Control de tamano
Parametros ETH= *((Parametros*)parametros);
printf("modulo ETH(fisica) %s %d.\n",__FILE__,__LINE__);

if(obtenerMTUInterface(interface, &MTU)==ERROR){
	printf("ERROR al obtener MTU");
	return ERROR;
}

if(longitud>MTU){
	printf("Longitud: %d mayor que MTU: %d\n", longitud, MTU );
	longitud=MTU;
}

//TODO
//[...] Cabecera del modulo
if(obtenerMACdeInterface( interface, ETH_or)==ERROR){
	printf("Error al obtener la MAC origen\n");
	return ERROR;
}
//DIRECCION DESTINO
memcpy(trama, ETH.ETH_destino , ETH_ALEN);

//DIRECCIÓN ORIGEN
memcpy(trama+6, ETH_or, ETH_ALEN);

//TIPO
memcpy(trama+12, &IP_type, IP_PROTO);
memcpy(trama+14, datagrama, longitud*sizeof(uint8_t));



printf("EL TAMAÑO DEL PAQUETES ES DE %d\n", (int)longitud+ 14);
if (pcap_inject(descr, &trama, longitud + ETH_ALEN +ETH_ALEN + 2 ) ==-1){
	 	printf("\n%s\n", pcap_geterr(descr));
}
//TODO
//Almacenamos la salida por cuestiones de debugging [...]
gettimeofday(&tiempo, NULL);

pcap.ts=tiempo;
pcap.caplen = longitud + ETH_ALEN +ETH_ALEN + 2;
pcap.len= longitud + ETH_ALEN +ETH_ALEN + 2;
pcap_dump((uint8_t *)pdumper, &pcap, trama);
	mostrarHex(trama, longitud+ ETH_ALEN+ ETH_ALEN +2);
return OK;
}



/***************************Funciones auxiliares a implementar***********************************/

/****************************************************************************************
 * Nombre: aplicarMascara                                                               *
 * Descripcion: Esta funcion aplica una mascara a una vector                            *
 * Argumentos:                                                                          *
 *  -IP: IP a la que aplicar la mascara en orden de red                                 *
 *  -mascara: mascara a aplicar en orden de red                                         *
 *  -longitud: bytes que componen la direccion (IPv4 == 4)                              *
 *  -resultado: Resultados de aplicar mascara en IP en orden red                        *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t aplicarMascara(uint8_t* IP, uint8_t* mascara, uint8_t longitud, uint8_t* resultado){
//TODO
//[...]
	if(!IP || !mascara || longitud<0) return ERROR;
	int i=0;
	for(i=0;i<longitud;i++){
		resultado[i]=IP[i]&mascara[i];
	}

	return OK;
}


/***************************Funciones auxiliares implementadas**************************************/

/****************************************************************************************
 * Nombre: mostrarHex                                                                   *
 * Descripcion: Esta funcion imprime por pantalla en hexadecimal un vector              *
 * Argumentos:                                                                          *
 *  -datos: bytes que conforman un mensaje                                              *
 *  -longitud: Bytes que componen el mensaje                                            *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t mostrarHex(uint8_t * datos, uint32_t longitud){
	uint32_t i;
	printf("Datos:\n");
	for (i=0;i<longitud;i++){
		printf("%02"PRIx8" ", datos[i]);
	}
	printf("\n");
	return OK;
}


/****************************************************************************************
 * Nombre: calcularChecksum                                                             *
 * Descripcion: Esta funcion devuelve el ckecksum tal como lo calcula IP/ICMP           *
 * Argumentos:                                                                          *
 *   -datos: datos sobre los que calcular el checksum                                   *
 *   -longitud: numero de bytes de los datos sobre los que calcular el checksum         *
 *   -checksum: checksum de los datos (2 bytes) en orden de red!                        *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t calcularChecksum(uint8_t *datos, uint16_t longitud, uint8_t *checksum) {
    uint16_t word16;
    uint32_t sum=0;
    int i;
    // make 16 bit words out of every two adjacent 8 bit words in the packet
    // and add them up
    for (i=0; i<longitud; i=i+2){
        word16 = (datos[i]<<8) + datos[i+1];
        sum += (uint32_t)word16;
    }
    // take only 16 bits out of the 32 bit sum and add up the carries
    while (sum>>16) {
        sum = (sum & 0xFFFF)+(sum >> 16);
    }
    // one's complement the result
    sum = ~sum;
    checksum[0] = sum >> 8;
    checksum[1] = sum & 0xFF;
    return OK;
}


/***************************Funciones inicializacion implementadas*********************************/

/****************************************************************************************
 * Nombre: inicializarPilaEnviar                                                        *
 * Descripcion: inicializar la pila de red para enviar registrando los distintos modulos*
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t inicializarPilaEnviar() {
	bzero(protocolos_registrados,MAX_PROTOCOL*sizeof(pf_notificacion));
	if(registrarProtocolo(ETH_PROTO, moduloETH, protocolos_registrados)==ERROR)
		return ERROR;
	if(registrarProtocolo(IP_PROTO, moduloIP, protocolos_registrados)==ERROR)
		return ERROR;

//TODO
//A registrar los modulos de ICMP y UDP [...]

	if(registrarProtocolo(ICMP_PROTO, moduloICMP, protocolos_registrados)==ERROR)
		return ERROR;

	if(registrarProtocolo(UDP_PROTO, moduloUDP, protocolos_registrados)==ERROR)
		return ERROR;

	return OK;
}


/****************************************************************************************
 * Nombre: registrarProtocolo                                                           *
 * Descripcion: Registra un protocolo en la tabla de protocolos                         *
 * Argumentos:                                                                          *
 *  -protocolo: Referencia del protocolo (ver RFC 1700)                                 *
 *  -handleModule: Funcion a llamar con los datos a enviar                              *
 *  -protocolos_registrados: vector de funciones registradas                            *
 * Retorno: OK/ERROR                                                                    *
 ****************************************************************************************/

uint8_t registrarProtocolo(uint16_t protocolo, pf_notificacion handleModule, pf_notificacion* protocolos_registrados){
	if(protocolos_registrados==NULL ||  handleModule==NULL){
		printf("Error: registrarProtocolo(): entradas nulas.\n");
		return ERROR;
	}
	else
		protocolos_registrados[protocolo]=handleModule;
	return OK;
}
