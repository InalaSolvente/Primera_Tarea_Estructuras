## Aplicación Para Gestión Hospitalaria

## Cómo Ejecutar
Para ejecutar el main primero debemos compilar (en la carpeta raíz)
````
gcc tdas/*.c main.c -Wno-unused-result -o main
````

Y luego ejecutar:
````
./main
````

Aconsejado usar este código en replit.com ya que fue el entorno en el cual se creó el código y no hubo ningun problema al ejecutar como anteriormente fue detallado, en el `shell`.

## Carpeta TDAs
En la carpeta `tdas` se encuentra la implementación para utilizar distintos TDAs, en este programa solo utilizamos `list.c` y `list.h`.

## Funcionamiento Y Uso del Código
Al ejecutar correctamente, se mostrará un menú con todas las funcionalidades del código. Para utilizar de manera correcta el programa solamente hay que seguir los pasos que éste mismo nos dará.

Algunas observaciones sobre el funcionamiento del programa, para el nombre como máximo se podrán ingresar 30 caracteres y para los sintomas 250. Si se ingresan más de estos caracteres puede resultar en variadas fallas del programa.

Al registrarse los pacientes todos se ingresarán al sistema con prioridad baja, mas adelante con una de las opciones del menú se podrá cambiar esto y todos los pacientes se ordenarán en su prioridad en orden de llegada (orden de ingreso al sistema).

Lo importante al cambiar prioridad o mostrar prioridad, es escribir bien las palabras o nombres, no importa mayusculas o minusculas pero si la correcta escritura de las palabras.

Tener en cuenta que cada vez que se ejecute o reinicie el programa, el sistema de ingreso hospitalario borrará todo lo anterior y se deberán ingresar todos los pacientes nuevamente. Por favor solamente cerrar o salir del programa cuando se deje de utilizar definitivamente.