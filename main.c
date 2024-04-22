#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
  char nombre[30];
  int edad;
  char sintoma[250];
  char prioridad[10];
  unsigned short ordenLlegada;
} Paciente;

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}

//Función para convertir a mayúscula las palabras, esto facilitará la comparación entre palabaras sin importar cómo las ingrese el usuario
void convertirMayusculas(char *nombre){
  while (*nombre){
    *nombre = toupper(*nombre);
    nombre++;
  }
}

//Función para mostrar solamente una prioridad
unsigned int mostrarPrioridad(List *prioridad, char *prioridadAmostrar){
  unsigned int cont = 0;
  Paciente *pacienteAmostrar = list_first(prioridad);
  while (pacienteAmostrar != NULL){
    if (cont == 0) printf("\nPRIORIDAD %s\n\n", prioridadAmostrar);
    printf("Siguiente paciente: %s\n", pacienteAmostrar->nombre);
    printf("Edad del paciente: %d\n", pacienteAmostrar->edad);
    printf("Sintomas: %s\n",pacienteAmostrar->sintoma);
    printf("Numero de llegada del paciente: %d\n", pacienteAmostrar->ordenLlegada);
    pacienteAmostrar = list_next(prioridad);
    cont++;
  }
  //La función retorna cont para asi conocer la cantidad de pacientes que hay en la lista
  return cont;
}


//Función para registrar a los pacientes
void registrar_paciente(List *listaPacientes, unsigned short cont, List *listaBaja) {

  //Se imprime lo requerido por el programa para registrar correctamente al paciente
  printf("Registrar nuevo paciente\n");
  Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
  printf("Ingrese el nombre del paciente: ");
  scanf(" %29[^\n]", paciente->nombre);
  printf("Ingrese la edad del paciente: ");
  scanf("%d", &paciente->edad);
  printf("Ingrese los sintomas del paciente: ");
  scanf(" %249[^\n]", paciente->sintoma);

  //Se convierten las palabras a mayusculas para facilitar el manejo
  convertirMayusculas(paciente->nombre);
  convertirMayusculas(paciente->sintoma);

  //Se le asigna un orden de llegada a cada paciente para mas adelante poder ordenarlos correctamente
  paciente->ordenLlegada = cont;
  //Al momento del registro, a cada paciente se le asigna prioridad baja
  strcpy(paciente->prioridad, "BAJA");
  if (cont == 0) {
    list_pushFront(listaPacientes, paciente);
    list_pushFront(listaBaja, paciente);
  } else {
    list_pushBack(listaPacientes, paciente);
    list_pushBack(listaBaja, paciente);
  }
}

//Esta función eliminará al paciente de su antigua prioridad al momento de cambiarlo de esta última
void eliminarPaciente(List *lista, Paciente pacienteEliminar) {
  Paciente *paciente = list_first(lista);
  while (paciente != NULL) {
    if (strcmp(paciente->nombre, pacienteEliminar.nombre) == 0) {
      //Se libera memoria y se elimina de la lista
      free(paciente);
      list_popCurrent(lista);
      return;
    }
    paciente = list_next(lista);
  }
}

//Función que comparará el orden de llegada de los pacientes para ordenar correctamente la lista, esta función es necesaria para list_sortedInsert
int comparar (void *data1, void *data2){
  Paciente *paciente1 = (Paciente*)data1;
  Paciente *paciente2 = (Paciente*)data2;
  if(paciente1->ordenLlegada < paciente2->ordenLlegada) return 1;
  return 0;
}

//Función para reconocer el paciente al cual se le cambiará la prioridad
Paciente *identificar(List *listaPacientes, char *nombre){
  Paciente *paciente = list_first(listaPacientes);
  while(strcmp(paciente->nombre, nombre) != 0) {
    paciente = list_next(listaPacientes);
  }
  return paciente;
}

//Funcion para cambiar la prioridad de un paciente
void cambiarPrioridad(List *listaPacientes, List *prioridadAlta,
List *prioridadMedia, List *prioridadBaja){
  char nombreACambiar[30], prioridad[10],prioridadPaciente[10];
  //Se imprimen mensajes para facilitarle el uso al usuario
  printf("Ingrese el paciente al cual se le cambiará la prioridad\n");
  scanf(" %29[^\n]",nombreACambiar);
  printf("Ingrese la nueva prioridad del paciente\n");
  scanf(" %9[^\n]",prioridad);
  //Se convierten a mayusculas las palabras ingresadas por el usuario para facilitar el manejo
  convertirMayusculas(nombreACambiar);
  convertirMayusculas(prioridad);
  //Se identifica el paciente al cual se le cambiará la prioridad
  Paciente *identificarPaciente = identificar(listaPacientes, nombreACambiar);
  //Se le cambia la prioridad al paciente
  strcpy(prioridadPaciente,identificarPaciente->prioridad);
  //Si el paciente ya tiene la prioridad a la que se quiere cambiar, se imprime un mensaje de aviso y se regresa al menú principal
  if (strcmp(prioridad,prioridadPaciente) == 0) {
    printf("El paciente ya tiene esa prioridad\n");
    return;
  }
  //Se comprueba a que prioridad se cambiará el paciente y dependiendo de ello, se utiliza la función list_sortedInsert para insertar el paciente en la lista correspondiente
  if (strcmp(prioridad,"ALTA") == 0){
    strcpy(identificarPaciente->prioridad,"ALTA");
    list_sortedInsert(prioridadAlta,(Paciente * )identificarPaciente, comparar);
      }
  else if (strcmp(prioridad,"MEDIA") == 0){
    strcpy(identificarPaciente->prioridad,"MEDIA");
    list_sortedInsert(prioridadMedia,(Paciente * )identificarPaciente, comparar);
  }
  else if (strcmp(prioridad,"BAJA") == 0){
    strcpy(identificarPaciente->prioridad,"BAJA");
    list_sortedInsert(prioridadBaja,(Paciente * )identificarPaciente, comparar);
      }
  //Si no ingresa en ninguna de las opciones, se imprime un mensaje de error debido a que se ingresó mal la prioridad y se regresa al menú principal
  else {
    printf("Prioridad ingresada no es válida\n");
    return;
  }

  //Si todo se realizó correctamente, se elimina el paciente de su antigua prioridad.
  if (strcmp(prioridadPaciente, "ALTA") == 0) eliminarPaciente(prioridadAlta, *identificarPaciente);
  else if (strcmp(prioridadPaciente, "MEDIA") == 0) eliminarPaciente(prioridadMedia, *identificarPaciente);
  else eliminarPaciente(prioridadBaja, *identificarPaciente);
}

//Función para mostrar la lista de espera en su totalidad
void mostrar_lista_pacientes(List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  printf("Pacientes en espera: \n");
  unsigned int contTotal = 0;
  contTotal += mostrarPrioridad(prioridadAlta,"ALTA");
  contTotal += mostrarPrioridad(prioridadMedia,"MEDIA");
  contTotal += mostrarPrioridad(prioridadBaja,"BAJA");
  //Si el contador de pacientes queda en 0 se imprime un aviso sobre que no hay pacientes en espera
  if (contTotal == 0) printf("No hay pacientes en la lista de espera\n");
}

//Función para atención al siguiente paciente según su prioridad y orden de llegada, es booleana debido a que puede ser que no haya pacientes en espera y si es así pasará a la siguiente prioridad en la siguiente función
bool atenderSiguientePrioridad(List *listaPrincipal, List *prioridad){
  Paciente *paciente = list_first(prioridad);
  if (paciente != NULL){
    printf("Siguiente paciente: %s\n", paciente->nombre);
    printf("Edad del paciente: %d\n", paciente->edad);
    printf("Sintomas: %s\n",paciente->sintoma);
    printf("Orden de llegada del paciente: %d\n", paciente->ordenLlegada);
    //Se elimina el paciente de la lista principal para optimizar memoria
    eliminarPaciente(listaPrincipal,*paciente);
    free(paciente);
    list_popFront(prioridad);
    return true;
  }
  return false;
}

//Función principal para atender al siguiente paciente
void atenderSiguiente(List *listaPrincipal, List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  //Se pregunta por cada prioridad si hay algún paciente a atender, si no hay pasa a la siguiente prioridad, y si no hay en ninguna lista se imprime un mensaje de aviso sobre que no hay pacientes en espera
  if (atenderSiguientePrioridad(listaPrincipal,prioridadAlta));
  else if (atenderSiguientePrioridad(listaPrincipal,prioridadMedia));
  else if (atenderSiguientePrioridad(listaPrincipal,prioridadBaja));
  else printf("No hay pacientes en espera\n");
}

//Función para mostrar los pacientes por prioridad
void pacientesPorPrioridad(List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  //Se inicializa un contador, que dependerá de lo retornado por la función para mostrar
  unsigned int cont;
  //Se imprimen mensajes para facilitarle el uso al usuario
  printf("Ingrese la prioridad que desea mostrar: ");
  char prioridad[10];
  scanf(" %9[^\n]", prioridad);
  //Se convierten las palabras a mayusculas para facilitar el manejo
  convertirMayusculas(prioridad);
  //Se comprueba a que prioridad se quiere mostrar y dependiendo de ello se utiliza la función mostrarPrioridad para mostrar los pacientes de esa prioridad
  if (strcmp("ALTA", prioridad) == 0){
    cont = mostrarPrioridad(prioridadAlta,"ALTA");
    //Si el contador queda en 0 se imprime un aviso sobre que no hay pacientes en esa prioridad
    if (cont == 0) printf("No hay pacientes en prioridad Alta.\n");
  }
  else if (strcmp("MEDIA", prioridad) == 0) {
    cont = mostrarPrioridad(prioridadMedia,"MEDIA");
    if (cont == 0) printf("No hay pacientes en prioridad Media.\n");
    }
  else if (strcmp("BAJA", prioridad) == 0) {
    cont = mostrarPrioridad(prioridadBaja,"BAJA");
    if (cont == 0) printf("No hay pacientes en prioridad Baja.\n");
    }
  //Si no ingresa en ninguna de las opciones, se imprime un mensaje de error debido a que la prioridad que se ingresó no fue válida
  else printf("Prioridad ingresada no es válida\n");
}

int main() {
  char opcion;
  //Se inicia un contador para saber el orden de llegada de los pacientes
  unsigned short ordenLlegada = 0;
  //Se crea una lista principal, para facilitar la busqueda de pacientes
  List *listaPacientes = list_create(); 
  //Se inicializan listas según prioridad para optimizar el programa y manejar de manera correcta cada prioridad
  List *prioridadAlta = list_create();
  List *prioridadMedia = list_create();
  List *prioridadBaja = list_create();

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); 
    switch (opcion) {
    case '1':
      registrar_paciente(listaPacientes, ordenLlegada, prioridadBaja);
      ordenLlegada++;
      break;
    case '2':
      cambiarPrioridad(listaPacientes, prioridadAlta, prioridadMedia, prioridadBaja);
      break;
    case '3':
      mostrar_lista_pacientes(prioridadAlta, prioridadMedia, prioridadBaja);
      break;
    case '4':
      atenderSiguiente(listaPacientes,prioridadAlta, prioridadMedia, prioridadBaja);
      break;
    case '5':
      pacientesPorPrioridad(prioridadAlta, prioridadMedia, prioridadBaja);
      break;
    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos
  list_clean(listaPacientes);

  return 0;
}
