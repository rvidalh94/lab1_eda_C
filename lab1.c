#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de estructura NODO, la cual sera utilizad para cada elemento de la lista enlazada.

typedef struct Nodo{
  int dato;
  struct Nodo * sgte;
} nodo;


nodo * crear_nodo(int dato){
  nodo * nuevo_nodo = (nodo *)malloc(sizeof(nodo));
  nuevo_nodo->dato = dato;
  nuevo_nodo->sgte = NULL;
  return nuevo_nodo;
}

nodo * enqueue(nodo * q, int dato){
  nodo * nuevo_nodo = crear_nodo(dato);
  nodo * indice = q;
  if(q == NULL){
    q = nuevo_nodo;
    return q;
  }
  while(indice->sgte != NULL){
    indice = indice->sgte;
  }
  indice->sgte = nuevo_nodo;
  return q;
}

int inicio(nodo * q){
  if(q!=NULL){
    return q->dato;
  }
  return -1;
}

nodo * dequeue(nodo * q){
  if(q!=NULL){
    nodo * aux = q;
    q = q->sgte;
    aux->sgte = NULL;
    return q;
  }
  return q;
}

int vacia(nodo * q){
  if(q==NULL){
    return 1; // esta vacia
  }
  return 0; // no esta vacia
}

void carga_filas(nodo * *fila1, nodo * *fila2){
    /* Función que lee el archivo de texto contenedor de las filas, y lo carga 
    a las filas correspondientes, es decir, fila1 y fila2
    Parámetros: fila1 Nodo, fila2 Nodo.
    Retorno: void
    */

    FILE * fp; // 1
    char * line = NULL; // 1
    size_t len = 0; // 1
    ssize_t read; // 1
    // Ruta del archivo contenedor de los datos.
    char* filename = "entrada.txt"; // 1

    fp = fopen(filename, "r"); // 1
    if (fp == NULL) // 1
        exit(EXIT_FAILURE); // 1

    // Lectura de datos.
    while ((read = getline(&line, &len, fp)) != -1) { // n
        char * token = strtok(line, " "); // n
        int flag_itera = 0; // n
        // Split para separar la línea según su respectiva fila.
        while(token != NULL) { // n²
            if(flag_itera == 0){ // n²
                *fila1 = enqueue(*fila1, atoi(token)); // n²
            }else{
                *fila2 = enqueue(*fila2, atoi(token)); // n²
            }

            flag_itera = flag_itera + 1;  // n²
            token = strtok(NULL, " ");    // n² 
        }
    }
    fclose(fp); // 1
    if (line) // 1
        free(line); // 1
}


void repartir_entradas(nodo * *fila1, nodo * *fila2, nodo * *fila_final, int cantidad_entradas){
    /* Función que recibe las filas cargadas con datos y las modifica realizando
    el flujo de repartir las entradas, quitando datos desde fila1 y fila2 y agregandolos a fila_final
    según corresponda.
    Parámetros: Nodo fila1, Nodo fila2, Nodo fila_final, int cantidad_entradas.
    Retorno: void
    */
    while(cantidad_entradas > 0){ // n
      int inicio_fila = 0; // n
      if(inicio(*fila1) < inicio(*fila2) && vacia(*fila1) == 0){ // n
        inicio_fila = inicio(*fila1); // n
        *fila_final = enqueue(*fila_final, inicio_fila); // n
        *fila1 = dequeue(*fila1); // n
      }else if(inicio(*fila1) > inicio(*fila2) && vacia(*fila2) == 0){ // n
        inicio_fila = inicio(*fila2); // n
        *fila_final = enqueue(*fila_final, inicio_fila); // n
        *fila2 = dequeue(*fila2); // n
      }else if(vacia(*fila2) == 1 && vacia(*fila1) == 0){ // n
        inicio_fila = inicio(*fila1); // n
        *fila_final = enqueue(*fila_final, inicio_fila); // n
        *fila1 = dequeue(*fila1); // n
      }else if(vacia(*fila1) == 1 && vacia(*fila2) == 0){ // n
        inicio_fila = inicio(*fila2); // n
        *fila_final = enqueue(*fila_final, inicio_fila); // n
        *fila2 = dequeue(*fila2); // n
      }else if(inicio(*fila1) == inicio(*fila2) && (vacia(*fila1) == 0 && vacia(*fila2) == 0)){ // n
        inicio_fila = inicio(*fila1); // n
        *fila_final = enqueue(*fila_final, inicio_fila); // n
        *fila1 = dequeue(*fila1); // n

        inicio_fila = inicio(*fila2); // n
        *fila_final = enqueue(*fila_final, inicio_fila); // n
        *fila2 = dequeue(*fila2); // n
      }
      cantidad_entradas = cantidad_entradas - 1; // n
    }
}


void escribe_salida(nodo * *fila1, nodo * *fila2, nodo * *fila_final){
    /* Función que recibe las filas con sus dato finales, para luego
    traspasar su contenido a un archivo de texto, que representa el estado final
    de las filas, aplicada la repartición de las entradas.
    Parámetros: Nodo fila1, Nodo fila2, Nodo fila_final
    Retorno: void
    */
    FILE *fp;  //1
    // Creación de archivo de salida, el cual se creará en la carpeta del código fuente.
    fp = fopen ("salida.txt", "w"); //1

    fputs("Fila 1\n", fp); //1
    // Escritura de las personas que no lograron ingresar, correspondientes a la fila1.
    while(vacia(*fila1) == 0){ // n
      int ini = inicio(*fila1); // n
      fprintf(fp, "%d\n", ini); // n
      *fila1 = dequeue(*fila1); // n
    }

    fputs("\n", fp); // 1
    fputs("Fila 2\n", fp); // 1
    // Escritura de las peronas que no lograron ingresar, correspondientes a la fila 2.
    while(vacia(*fila2) == 0){ // n
      int ini = inicio(*fila2); // n
      fprintf(fp, "%d\n", ini); // n
      *fila2 = dequeue(*fila2); // n
    }

    fputs("\n", fp); // 1
    fputs("Personas con entradas\n", fp); // 1
    // Escritura de las personas que pudieron entrar al cine.
    while(vacia(*fila_final) == 0){ // n
      int ini = inicio(*fila_final); // n
      fprintf(fp, "%d\n", ini); // n
      *fila_final = dequeue(*fila_final); // n
    }

    fclose (fp); // 1
}


int main(int argc, char const *argv[]) { // O(n²)

    // Creación de las 3 filas iniciales.
    nodo * fila1; // 1
    nodo * fila2; // 1
    nodo * fila_final; // 1

    int cantidad_entradas; // 1
    // Solicitud de cantidad de entradas a repartir.
    printf("Por favor, ingrese la cantidad de entradas a repartir: ");  // 1
    scanf("%d", &cantidad_entradas); // 1
    
    // Lectura de archivo de entrada y cargar inicial de valores.
    carga_filas(&fila1, &fila2); // 6n² + 3n + 11
    // Proceso de repartir entradas.
    repartir_entradas(&fila1, &fila2, &fila_final, cantidad_entradas); // 26n
    // Creación de archivo de salida con el estado final de las filas.
    escribe_salida(&fila1, &fila2, &fila_final); // 12n + 8

    return 0; // 1
}