#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <malloc.h>


int calculaAleatorios(int min, int max);

void manejador(int sig);

void manejador2(int sig);

int main(int argc, char *argv[])
{
    struct sigaction sign;
    sign.sa_handler = manejador2;
    //recogemos el numero de pacientes pasados por parametro
    int numHijos = atoi(argv);
    //reservamos espacio en un puntero para almacenar a los pacientes y un int para los vacunados
    int *pacientes;
    int vacunados = 0;
    int num = 0;
    pacientes = malloc(sizeof(int)*numHijos);
    //creamos los procesos hijos
    pid_t medico, farmaceutico;

    farmaceutico = fork();

    if(farmaceutico == -1){
        perror("\nError en la llamada a fork()\n");

    }
    //se esta ejecutando el farmaceutico
    if(farmaceutico == 0){
        //espera la señal del epidemiologo
        printf("\nSoy el farmaceutico con PID = %d",getpid());
        sigaction(SIGUSR1,&sign,NULL);
        pause();

        //comprobamos si hay dosis suficientes
        num = aleatorio(0,1);
        kill(farmaceutico,SIGUSR1);
    }
        //si el pid es > 0 entonces se esta ejecutando epidemiologo
    else{
        //el epidemiologo duerme 2 segundos y manda una señal al farmaceutico
        printf("\nSoy el epidemiologo con PID = %d",getpid());
        sleep(2);
        kill(farmaceutico,SIGUSR1);
        //el epidemiologo espera la respuesta sobre si hay o no dosis
        sigaction(SIGUSR1,&sign,NULL);
        pause();
        //el epidemiologo manda una señal al medico para empezar a vacunar o para la ejecucion del programa
        if(num == 0){
            //no hay dosis suficiente
            printf("\nNo hay dosis suficientes para vacunar a los pacientes\n");
            exit(-1);
        }else{
            //hay dosis
            printf("\nHay dosis suficientes para vacunar\n");
            kill(medico,SIGUSR2);
        }
    }

    medico = fork();
    sign.sa_handler = manejador;
    if(medico == -1){
        perror("Error en la llamada a fork()");

    }else if(medico == 0){
        //esperamos a la señal del padre
        printf("Soy el medico con PID = %d",getpid());
        sigaction(SIGUSR2,&sign,NULL);
        pause();

        //creamos los pacientes a partir del medico
        for(int i=0;i<numHijos;i++) {
            pid_t paciente = fork();
            if (paciente == -1) {
                perror("Error en la llamada a fork()");

            } else if (paciente == 0) {
                //se esta ejecutando el proceso hijo(el paciente correspondiente)
                //esperamos a que llegue una señal del medico
                printf("Soy el paciente numero %d con PID = %d",i,getpid());
                sigaction(SIGUSR1, &sign, NULL);
                pause();

                sleep(2);
                int reaccion = aleatorio(1, 10);
                if ((reaccion % 2) == 0) {
                    vacunados += 1;
                    kill(medico,SIGUSR2);
                } else {
                    continue;
                }
                *(pacientes + i) = paciente;

            } else {
                //se esta ejecutando el proceso padre(el medico)
                //enviamos una señal a los pacientes para comprobar su reaccion
                kill(0,SIGUSR2);

                sigaction(SIGUSR1, &sign, NULL);
                pause();
            }

        }
    }


    printf("\nSe han vacunado %d pacientes, y en %d la vacuna ha sido efectiva\n",numHijos,vacunados);

    return 0;
}

//funcion que calcula un numero aleatorio en un intervalo
int aleatorio(int min, int max) {
    srand(time(NULL));
    return rand() % (max-min+1) + min;

}
//funcion que controla el tipo de señal que se usa y su accion

void manejador(int sig)
{
    switch(sig){
        case 16:
            printf("\nHe recibido la señal SIGUSR1\n");
            break;
        case 17:
            printf("\nHe recibido la señal SIGUSR2\n");
            break;
        default:
            break;
    }
}
void manejador2(int sig)
{

    switch(sig){
        case 16:
            printf("\nHe recibido la señal SIGUSR1\n");
            break;
        default:
            break;
    }
}
