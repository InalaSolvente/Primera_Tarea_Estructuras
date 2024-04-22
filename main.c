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

void convertirMayusculas(char *nombre){
  while (*nombre){
    *nombre = toupper(*nombre);
    nombre++;
  }
}

unsigned int mostrarPrioridad(List *prioridad, char *prioridadAmostrar){
  Paciente *pacienteAmostrar = (Paciente *) malloc(sizeof(Paciente));
  unsigned int cont = 0;
  pacienteAmostrar = list_first(prioridad);
  while (pacienteAmostrar != NULL){
    if (cont == 0) printf("\nPRIORIDAD %s\n\n", prioridadAmostrar);
    printf("Siguiente paciente: %s\n", pacienteAmostrar->nombre);
    printf("Sintomas: %s\n",pacienteAmostrar->sintoma);
    pacienteAmostrar = list_next(prioridad);
    cont++;
  }
  return cont;
}

void registrar_paciente(List *listaPacientes, unsigned short cont, List *listaBaja) {
  printf("Registrar nuevo paciente\n");
  Paciente *paciente = (Paciente *)malloc(sizeof(Paciente));
  printf("Ingrese el nombre del paciente: ");
  scanf(" %29[^\n]", paciente->nombre);
  printf("Ingrese la edad del paciente: ");
  scanf("%d", &paciente->edad);
  printf("Ingrese los sintomas del paciente: ");
  scanf(" %249[^\n]", paciente->sintoma);
  convertirMayusculas(paciente->nombre);
  convertirMayusculas(paciente->sintoma);
  paciente->ordenLlegada = cont;
  strcpy(paciente->prioridad, "BAJA");
  if (cont == 0) {
    list_pushFront(listaPacientes, paciente);
    list_pushFront(listaBaja, paciente);
  } else {
    list_pushBack(listaPacientes, paciente);
    list_pushBack(listaBaja, paciente);
  }
  // Aquí implementarías la lógica para registrar un nuevo paciente
}

void eliminarPaciente(List *lista, Paciente pacienteEliminar) {
  Paciente *paciente = list_first(lista);
  while (paciente != NULL) {
    if (strcmp(paciente->nombre, pacienteEliminar.nombre) == 0) {
      list_popCurrent(lista);
      return;
    }
    paciente = list_next(lista);
  }
}


int comparar (void *data1, void *data2){
  Paciente *paciente1 = (Paciente*)data1;
  Paciente *paciente2 = (Paciente*)data2;
  if(paciente1->ordenLlegada < paciente2->ordenLlegada) return 1;
  return 0;
}

Paciente *identificar(List *listaPacientes, char *nombre){
  Paciente *paciente = list_first(listaPacientes);
  while(strcmp(paciente->nombre, nombre) != 0) {
    paciente = list_next(listaPacientes);
  }
  return paciente;
}

void cambiarPrioridad(List *listaPacientes, List *prioridadAlta,
List *prioridadMedia, List *prioridadBaja){
  printf("Ingrese el paciente al cual se le cambiará la prioridad\n");
  char nombreACambiar[30], prioridad[10],prioridadPaciente[10];
  scanf(" %29[^\n]",nombreACambiar);
  printf("Ingrese la nueva prioridad del paciente\n");
  scanf(" %9[^\n]",prioridad);
  convertirMayusculas(nombreACambiar);
  convertirMayusculas(prioridad);
  printf("Prioridad actual: %s\n", prioridad);
  Paciente *identificarPaciente = identificar(listaPacientes, nombreACambiar);
  strcpy(prioridadPaciente,identificarPaciente->prioridad);
  if (strcmp(prioridad,prioridadPaciente) == 0) {
    printf("El paciente ya tiene esa prioridad\n");
    return;
  }
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
  else {
    printf("Prioridad ingresada no es válida\n");
    return;
  }

  if (strcmp(prioridadPaciente, "ALTA") == 0) eliminarPaciente(prioridadAlta, *identificarPaciente);
  else if (strcmp(prioridadPaciente, "MEDIA") == 0) eliminarPaciente(prioridadMedia, *identificarPaciente);
  else eliminarPaciente(prioridadBaja, *identificarPaciente);
}

void mostrar_lista_pacientes(List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  // Mostrar pacientes en la cola de espera
  printf("Pacientes en espera: \n");
  unsigned int contTotal = 0, cont;
  cont = mostrarPrioridad(prioridadAlta,"ALTA");
  contTotal += cont;
  cont = mostrarPrioridad(prioridadMedia,"MEDIA");
  contTotal += cont;
  cont = mostrarPrioridad(prioridadBaja,"BAJA");
  contTotal += cont;
  if (contTotal == 0) printf("No hay pacientes en la lista de espera\n");
  // Aquí implementarías la lógica para recorrer y mostrar los pacientes
}

bool atenderSiguientePrioridad(List *prioridad){
  Paciente *paciente = list_first(prioridad);
  if (paciente != NULL){
    printf("Siguiente paciente: %s\n", paciente->nombre);
    printf("Sintomas: %s\n",paciente->sintoma);
    free(paciente);
    list_popFront(prioridad);
    return true;
  }
  return false;
}

void atenderSiguiente(List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  if (atenderSiguientePrioridad(prioridadAlta));
  else if (atenderSiguientePrioridad(prioridadMedia));
  else if (atenderSiguientePrioridad(prioridadBaja));
  else printf("No hay pacientes en espera\n");
}

void pacientesPorPrioridad(List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  unsigned int cont;
  printf("Ingrese la prioridad que desea mostrar: ");
  char prioridad[10];
  scanf(" %9[^\n]", prioridad);
  convertirMayusculas(prioridad);
  if (strcmp("ALTA", prioridad) == 0){
    cont = mostrarPrioridad(prioridadAlta,"ALTA");
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
  else printf("Prioridad ingresada no es válida\n");
}

int main() {
  char opcion;
  unsigned short ordenLlegada = 0;
  List *listaPacientes = list_create(); // puedes usar una lista para gestionar los pacientes
  List *prioridadAlta = list_create();
  List *prioridadMedia = list_create();
  List *prioridadBaja = list_create();

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el newline anterior
    switch (opcion) {
    case '1':
      registrar_paciente(listaPacientes, ordenLlegada, prioridadBaja);
      ordenLlegada++;
      break;
    case '2':
      cambiarPrioridad(listaPacientes, prioridadAlta, prioridadMedia,
                       prioridadBaja);
      // Lógica para asignar prioridad
      break;
    case '3':
      mostrar_lista_pacientes(prioridadAlta, prioridadMedia, prioridadBaja);
      break;
    case '4':
      atenderSiguiente(prioridadAlta, prioridadMedia, prioridadBaja);
      // Lógica para atender al siguiente paciente
      break;
    case '5':
      pacientesPorPrioridad(prioridadAlta, prioridadMedia, prioridadBaja);
      // Lógica para mostrar pacientes por prioridad
      break;
    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos, si es necesario
  list_clean(listaPacientes);

  return 0;
}
