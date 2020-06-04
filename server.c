#define _POSIX_C_SOURCE 200809L

#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ADDRESS "0.0.0.0"
#define PORT 8080
#define STEP_COUNT 11
#define INIT_CLIENT_MESSAGE_CLIENT 2000

void freeResources(int sock, int client_sock, FILE * stream, char * client_message);
void gdbme(void);
void quineCheck(void);
void printStream(FILE * stream);
void printLogo(void);

void step0(void);
void step1(void);
void step2(void);
void step3(void);
void step4(void);
void step5(void);
void step6(void);
void step7(void);
void step8(void);
void step9(void);
void step10(void);


int main(int argc , char *argv[])
{
	int sock , client_sock , c;
	struct sockaddr_in server , client;
	char * client_message;
    size_t client_message_size;
    FILE * stream;

    void (*steps[STEP_COUNT])(void) = {step0, step1, step2, step3, step4, step5, step6, step7, step8, step9, step10}; 
    char * stepAns[STEP_COUNT] = {"entendido\n", "0854780\n", "martin luther king\n", "es_go_lo_dro_bo\n", "too_easy\n", ".RUN_ME\n",
            "in_de_ter_mi_na_do\n", "thunder thunder thunder thundercats\n", "this is awesome\n", "chin_chu_lan_cha\n", "gdb_manda\n"};
	
	//Create socket
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
	if (sock == -1){
		perror("Socket");
        exit(1);
    }

    int flag = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(flag)) == -1){
        close(sock);
        perror("setsockopt");
        exit(1);
    }
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ADDRESS);
	server.sin_port = htons(PORT);
	
	//Bind
	if(bind(sock, (struct sockaddr *)&server , sizeof(server)) < 0){
        close(sock);
		perror("bind");
		exit(1);
	}
	
	//Listen
	if(listen(sock , 5) == -1){
        close(sock);
        perror("listen");
        exit(1);
    }

	//Accept and incoming connection
	c = sizeof(struct sockaddr_in);

	client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c);
	if (client_sock < 0){
        close(sock);
		perror("accept");
		exit(1);
	}

    // Open client fd
    stream = fdopen(client_sock, "r");
    if(stream == NULL){
        close(sock);
        close(client_sock);
        perror("fdopen");
        exit(1);
    }

    
    client_message_size = INIT_CLIENT_MESSAGE_CLIENT;
    client_message = (char *)malloc(client_message_size * sizeof(char));
    if(client_message == NULL){
        close(sock);
        close(client_sock);
        fclose(stream);
        perror("malloc");
        exit(1);
    }

    int wrongAnsFlag;
    for(size_t i = 0; i < STEP_COUNT; i++){

        wrongAnsFlag = 0;
        do{

            if(wrongAnsFlag){
                printf("Respuesta incorrecta: %s\n", client_message);
                sleep(2);
            }

            printf("\033[1;1H\033[2J"); //Clear screen
            puts("------------- DESAFIO -------------");

            steps[i]();

            if(getline(&client_message, &client_message_size, stream) == -1){
                freeResources(sock, client_sock, stream, client_message);
                perror("getline");
                exit(1);
            }

            wrongAnsFlag = 1;

        } while(strcmp(client_message, stepAns[i]));
    }

    printf("\033[1;1H\033[2J"); //Clear screen
    printf("Felicitaciones, finalizaron el juego. Ahora deberan implementar el servidor que se comporte como el servidor provisto\n\n");

    freeResources(sock, client_sock, stream, client_message);

    //system("echo QUE CURIOSO> /tmp/hidden" <no return ...> ???

    return 0;
}

void freeResources(int sock, int client_sock, FILE * stream, char * client_message){
    close(sock);
    close(client_sock);
    fclose(stream);
    free(client_message);
}

void quineCheck(void){

    FILE * popenStream;
    if((popenStream = popen("gcc quine.c -o quine", "r")) == NULL){
        perror("popen");
        exit(1);
    }

    // Nos fijamos que gcc haya devuelto 0, es decir, que haya podido compilar quine.c
    int gccReturnValue = pclose(popenStream);
    if(gccReturnValue == -1 || !WIFEXITED(gccReturnValue) || WEXITSTATUS(gccReturnValue)){
        printf("\nENTER para reintentar.\n");
        return;
    }

    puts("\302\241Genial!, ya lograron meter un programa en quine.c, veamos si hace lo que corresponde.");

    if((popenStream = popen("./quine | diff - quine.c", "r")) == NULL){
        perror("popen");
        exit(1);
    }

    // Imprimimos salida de diff
    printStream(popenStream);
    
    // Nos fijamos que diff haya devuelto 0, es decir, que quine.c y su stdout sean iguales
    gccReturnValue = pclose(popenStream);
    if(gccReturnValue == -1 || !WIFEXITED(gccReturnValue) || WEXITSTATUS(gccReturnValue)){
        printf("\ndiff encontr\303\263 diferencias.\n");
        printf("ENTER para reintentar.\n");
        return;
    }
    
    puts("La respuesta es chin_chu_lan_cha"); 
}

void printStream(FILE * stream){
    char buffer[200];
    int len;

    do{
        len = fread(buffer, sizeof(char), 200 - 1, stream);

        if(ferror(stream)){
            perror("fread");
            exit(1);
        }
        buffer[len] = 0;

        printf("%s", buffer);

    } while(!feof(stream));
}

void gdbme(void){
    size_t aux = 0;
    if(aux == 0x12345678)
        printf("La respuesta es gdb_manda \n");
    else
        printf("ENTER para reintentar \n");
}

void step0(void){
    printf("Bienvenidos al TP3 y felicitaciones, ya resolvieron el primer acertijo.\nEn este TP deber\303\241n finalizar el juego que ya comenzaron resolviendo los desaf\303\255os de cada nivel.\nAdem\303\241s tendr\303\241n que investigar otras preguntas para responder durante la defensa.\nEl desaf\303\255o final consiste en crear un programa que se comporte igual que yo, es decir, que provea los mismos desaf\303\255os y que sea necesario hacer lo mismo para resolverlos. No basta con esperar la respuesta.\nAdem\303\241s, deber\303\241n implementar otro programa para comunicarse conmigo.\n\nDeber\303\241n estar atentos a los easter eggs.\n\nPara verificar que sus respuestas tienen el formato correcto respondan a este desaf\303\255o con la palabra 'entendido\\n'\n\n");
    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("\302\277C\303\263mo descubrieron el protocolo, la direcci\303\263n y el puerto para conectarse?\n\n");                                          
}

void step1(void){
    printf("HASHTAG left up up left down right down left ASTERISK\n\n");
    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("\302\277Qu\303\251 diferencias hay entre TCP y UDP y en qu\303\251 casos conviene usar cada uno?\n\n");
}

void step2(void){
    printf("https://voca.ro/hcmH7cqyqTg\n\n");
    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("\302\277El puerto que usaron para conectarse al server es el mismo que usan para mandar las respuestas? \302\277Por qu\303\251?\n\n");
}

// Falta hacer los writes alternantes
void step3(void){
    printf("EBADF...\n\n");

    write(13, "La respuesta es es_go_lo_dro_bo\n", 32);
    perror("write: Bad file descriptor");

    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("\302\277Qu\303\251 \303\272til abstracci\303\263n es utilizada para comunicarse con sockets? \302\277se puede utilizar read(2) y write(2) para operar?\n\n");
}

// Falta que strings tenga el too easy
void step4(void){
    printf("respuesta = strings:377\n\n");
    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("\302\277C\303\263mo garantiza TCP que los paquetes llegan en orden y no se pierden?\n\n");        
}

void step5(void){
    // Falta agregar la seccion .RUN_ME
    printf(".data .bss .comment ? .shstrtab .symtab .strtab\n\n");                                                                     
    puts("\n----- PREGUNTA PARA INVESTIGAR -----");                                                                     
    printf("Un servidor suele crear un nuevo proceso o thread para atender las conexiones entrantes. \302\277Qu\303\251 conviene m\303\241s?\n\n");
}


void step6(void){
    printf("mixed fds\n\n");
    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("\302\277C\303\263mo se puede implementar un servidor que atienda muchas conexiones sin usar procesos ni threads?");        
}

void step7(void){
    printf("Logo\n\n");
    printLogo();
    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("\302\277Qu\303\251 aplicaciones se pueden utilizar para ver el tr\303\241fico por la red?"); 
}

void step8(void){
    printf("Tango Hotel India Sierra India Sierra Alfa Whiskey Echo Sierra Oscar Mike Echo\n\n");
    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("sockets es un mecanismo de IPC. \302\277Qu\303\251 es m\303\241s eficiente entre sockets y pipes?");
    
}

void step9(void){
    printf("quine\n\n");

    quineCheck();

    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("%s\n\n", "\302\277Cu\303\241les son las caracter\303\255sticas del protocolo SCTP?");       
}

// Falta testearlo
void step10(void){
    printf("b gdbme y encontr\303\241 el valor m\303\241gico\n\n");

    gdbme();

    puts("\n----- PREGUNTA PARA INVESTIGAR -----");
    printf("\302\277Qu\303\251 es un RFC?\n\n");
}

void printLogo(void){
    printf("____________________");
    printf("____________________");
    printf("___(###############/");
    printf("____________________");
    puts("_______________________");

    printf("____________________");
    printf("________________###.");
    printf("....................");
    printf("....###_____________");
    puts("_______________________");

    printf("____________________");
    printf("____________##......");
    printf("....................");
    printf(".........##_________");
    puts("_______________________");

    printf("____________________");
    printf("________##..........");
    printf("....................");
    printf(".............#&_____");
    puts("_______________________");

    printf("____________________");
    printf("______#.............");
    printf("....................");
    printf("................#___");
    puts("_______________________");
    
    printf("____________________");
    printf("___&#...............");
    printf("....................");
    printf(".......########...#/");
    puts("_______________________");  
        
    printf("____________________");
    printf("__#.................");
    printf("...............#####");
    printf("############....##..");
    puts("#______________________");

    printf("____________________");
    printf("#...................");
    printf(".......#############");
    printf("##.........#########");
    puts("..#____________________");

    printf("___________________#");
    printf("....................");
    printf("..###..#############");
    printf("####################");
    puts("#&.#___________________");

    printf("__________________#.");
    printf("...................#");
    printf("#....###############");
    printf("####################");
    puts("###.#__________________");

    printf("_________________#..");
    printf(".................##.");
    printf("..##################"); 
    printf("##..&###############");
    puts("###&.#_________________");

    printf("________________#...");
    printf("..............##...#");
    printf("####################");
    printf("#######&..##########");
    puts("####.&#________________");

    printf("________________#...");
    printf("..........&#########");
    printf("####################");
    printf("##########..########");
    puts("#####.#________________");

    printf("_______________*#...");
    printf("......&#############");
    printf("####################");
    printf("############.#######");
    puts("#####.#________________");

    printf("_______________##...");
    printf("...#################");
    printf("####################");
    printf("############..######");
    puts("#####.#(_______________");

    printf("_______________/#...");
    printf(".###################");
    printf("#...........########");
    printf("#############.######");
    puts("#####.#._______________");

    printf("________________#.&#");
    printf("################....");
    printf("..............######");
    printf("############..######");
    puts("#####.#________________");

    printf("________________#...");
    printf(".#############......");
    printf("..............######");
    printf("############.&######");
    puts("#####.#________________");

    printf("_________________##.");
    printf("..#########.........");
    printf("..............######");
    printf("##########..........");
    puts("####.#_________________");

    printf("_________________.#.");
    printf("...#######..........");
    printf("..............######");
    printf("########&...........");
    puts("..#.#__________________");

    printf("___________________#");
    printf("......####&.........");
    printf(".............#######");
    printf("####................");
    puts("...#___________________");

    printf("____________________");
    printf("##......####........");
    printf("............########");
    printf("....................");
    puts("..#____________________");

    printf("____________________");
    printf("_###.......###......");
    printf("...........#######..");
    printf("...................#");
    puts("##_____________________");

    printf("____________________");
    printf("___##...............");
    printf(".........#########..");
    printf("...#..............##");
    puts("_______________________");

    printf("____________________");
    printf("_____##............&");
    printf("##&&################");
    printf("##..............##__");
    puts("_______________________");

    printf("____________________");
    printf("________##..........");
    printf("....################");
    printf(".............##_____");
    puts("_______________________");

    printf("____________________");
    printf("___________##.......");
    printf("....#..##########...");
    printf("..........##________");
    puts("_______________________");

    printf("____________________");
    printf("_______________.##..");
    printf("....................");
    printf(".....##/____________");
    puts("_______________________");

    printf("____________________");
    printf("____________________");
    printf("__######&&...&######");
    printf("#___________________");
    puts("_______________________");
}











