void mostrar_lista_pacientes(List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  // Mostrar pacientes en la cola de espera
  printf("Pacientes en espera: \n");
  unsigned int contTotal = 0, cont;
  Paciente *pacienteActual = list_first(prioridadAlta);
  if (pacienteActual == NULL) printf("No hay pacientes en prioridad Alta.\n");
  while (pacienteActual != NULL) {
    printf("Siguiente paciente: %s\n", pacienteActual->nombre);
    printf("Sintomas: %s\n",pacienteActual->sintoma);
    pacienteActual = list_next(prioridadAlta);
    cont++;
  }
  pacienteActual = list_first(prioridadMedia);
  if (pacienteActual == NULL) printf("No hay pacientes en prioridad Media.\n");
  while (pacienteActual != NULL) {
    printf("Siguiente paciente: %s\n", pacienteActual->nombre);
    printf("Sintomas: %s\n",pacienteActual->sintoma);
    pacienteActual = list_next(prioridadMedia);
    cont++;
  }
  pacienteActual = list_first(prioridadBaja);
  if (pacienteActual == NULL) printf("No hay pacientes en prioridad Baja.\n");
  while (pacienteActual != NULL) {
    printf("Siguiente paciente: %s\n", pacienteActual->nombre);
    printf("Sintomas: %s\n",pacienteActual->sintoma);
    pacienteActual = list_next(prioridadBaja);
    cont++;
  }
  if (cont == 0) printf("No hay pacientes en la lista de espera");
  // Aquí implementarías la lógica para recorrer y mostrar los pacientes
}

void pacientesPorPrioridad(List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  printf("Ingrese la prioridad que desea mostrar: ");
  char prioridad[5];
  scanf(" %4s", prioridad);
  convertirMayusculas(prioridad);
  if (strcmp("ALTA", prioridad) == 0) {
    Paciente *pacienteActual = list_first(prioridadAlta);
    while (pacienteActual != NULL) {
      printf("Siguiente paciente: %s\n", pacienteActual->nombre);
      printf("Sintomas: %s\n",pacienteActual->sintoma);
      pacienteActual = list_next(prioridadAlta);
    }
  } else if (strcmp("MEDIA", prioridad) == 0) {
    Paciente *pacienteActual = list_first(prioridadMedia);
    while (pacienteActual != NULL) {
      printf("Siguiente paciente: %s\n", pacienteActual->nombre);
      printf("Sintomas: %s\n",pacienteActual->sintoma);
      pacienteActual = list_next(prioridadMedia);
    }
  } else if (strcmp("BAJA", prioridad) == 0) {
    Paciente *pacienteActual = list_first(prioridadBaja);
    while (pacienteActual != NULL) {
      printf("Siguiente paciente: %s\n", pacienteActual->nombre);
      printf("Sintomas: %s\n",pacienteActual->sintoma);
      pacienteActual = list_next(prioridadBaja);
    }
  }
}

void atenderSiguiente(List *prioridadAlta, List *prioridadMedia, List *prioridadBaja) {
  Paciente *pacienteActual = list_first(prioridadAlta);
  if (pacienteActual != NULL) {
    printf("Siguiente paciente: %s\n", pacienteActual->nombre);
    printf("Sintomas: %s\n",pacienteActual->sintoma);
    free(pacienteActual);
    list_popFront(prioridadAlta);
  } else {
    pacienteActual = list_first(prioridadMedia);
    if (pacienteActual != NULL) {
      printf("Siguiente paciente: %s\n", pacienteActual->nombre);
      printf("Sintomas: %s\n",pacienteActual->sintoma);
      free(pacienteActual);
      list_popFront(prioridadMedia);
    } else {
      pacienteActual = list_first(prioridadBaja);
      if (pacienteActual != NULL) {
        printf("Siguiente paciente: %s\n", pacienteActual->nombre);
        printf("Sintomas: %s\n",pacienteActual->sintoma);
        free(pacienteActual);
        list_popFront(prioridadBaja);
      } else
        printf("No hay pacientes en espera\n");
    }
  }
}