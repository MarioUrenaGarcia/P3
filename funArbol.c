#include "defKubo.h"

/**
 * @file funArbol.c
 * @brief Este es el archivo de funciones del árbol binario
 * @date 03/05/2025
 * @author Mario Ureña García, Ricardo Ponce de León Vargas y Emiliano Cisneros Cervantes
 */

// Funciones ----------------------------------------------------------------------------

/**
 *@brief Función que inserta un nuevo nodo en el árbol
 *@param pt puntero al nodo padre
 *@param numCta número de cuenta del cliente
 *@param nombreCliente nombre del cliente
 *@param pizzas cantidad de pizzas
 *@param tacos cantidad de tacos
 *@param total total de la compra
 *@return puntero al nodo insertado
 */
extern tipoHoja *insertarHoja(tipoHoja *pt, int numCta, char nombreCliente[], int pizzas, int tacos, float total)
{
    tipoHoja *aux, *nuevo;
    int salir = 1;

    aux = pt;

    nuevo = (tipoHoja *)malloc(sizeof(tipoHoja));
    if (nuevo == NULL)
    {
        printf("\nNo hay memoria disponible\n");
        exit(1);
    }

    // Como es un cliente recién ingresado, se le asignan sus datos de la primera compra
    nuevo->numCuenta = numCta;
    strcpy(nuevo->nombre, nombreCliente);
    nuevo->numTacos = tacos;
    nuevo->numPizzas = pizzas;
    nuevo->compraAcumulada = total;

    nuevo->izq = NULL;
    nuevo->der = NULL;

    if (aux == NULL) // Árbol vacío
    {
        pt = nuevo;
    }

    while ((aux != NULL) && (salir != 0))
    {
        if (numCta > aux->numCuenta) // mover a la derecha
        {
            if (aux->der != NULL)
            {
                aux = aux->der;
            }
            else
            {
                aux->der = nuevo;
                salir = 0;
            }
        }
        if (numCta <= aux->numCuenta) // mover a la izquierda
        {
            if (aux->izq != NULL)
            {
                aux = aux->izq;
            }
            else
            {
                aux->izq = nuevo;
                salir = 0;
            }
            if (numCta == aux->numCuenta)
            {
                salir = 0;
            }
        }
    }
    return pt;
}

/**
 *@brief Función que imprime el árbol binario
 *@param aux puntero al nodo raiz del árbol
 *@return void
 */
extern void imprimirArb(tipoHoja *aux)
{
    printf("\n");
    if (aux != NULL)
    {
        imprimirArb(aux->izq);
        printf("\n\n");
        printf(YELLOW "Cuenta:" RESET " %d\t" YELLOW "Nombre:" RESET " %s\n", aux->numCuenta, aux->nombre);
        printf(YELLOW "Tacos:" RESET " %d\t" YELLOW "Pizzas:" RESET " %d\n", aux->numTacos, aux->numPizzas);
        printf(YELLOW "Compra acumulada:" RESET " $%.2f\n", aux->compraAcumulada);
        imprimirArb(aux->der);
    }
    return;
}

/**
 *@brief Función que busca un cliente en el árbol
 *@param aux puntero al nodo raiz del árbol
 *@param numCta número de cuenta del cliente
 *@return 1 si el cliente se encuentra en el árbol, 0 si no
 */
extern int buscarCliente(tipoHoja *aux, int numCta)
{
    int encontrado = 0;

    while (aux != NULL && encontrado == 0)
    {
        if (numCta == aux->numCuenta)
        {
            encontrado = 1;
        }
        else if (numCta < aux->numCuenta)
        {
            aux = aux->izq;
        }
        else
        {
            aux = aux->der;
        }
    }

    return encontrado;
}

/**
 *@brief Función que actualiza un cliente en el árbol
 *@param aux puntero al nodo raiz del árbol
 *@param numCta número de cuenta del cliente
 *@param pizzas cantidad de pizzas
 *@param tacos cantidad de tacos
 *@param total total de la compra
 *@return void
 */
extern void actualizarCliente(tipoHoja **aux, int numCta, int pizzas, int tacos, float total)
{
    tipoHoja *temp;

    temp = *aux;

    while (temp != NULL)
    {
        if (numCta == temp->numCuenta)
        {
            temp->numTacos += tacos;
            temp->numPizzas += pizzas;
            temp->compraAcumulada += total;
            break;
        }
        else if (numCta < temp->numCuenta)
        {
            temp = temp->izq;
        }
        else
        {
            temp = temp->der;
        }
    }

    return;
}

/**
 * @brief Función que guarda un nodo del árbol binario en un archivo
 * @param fp puntero al archivo
 * @param aux puntero al nodo a guardar
 * @return void
 */
void guardarNodo(FILE *fp, tipoHoja *aux)
{
    if (aux != NULL)
    {
        fprintf(fp, "%d\t%s\t%d\t%d\t%.2f\n", aux->numCuenta, aux->nombre, aux->numPizzas, aux->numTacos, aux->compraAcumulada);
        guardarNodo(fp, aux->izq);
        guardarNodo(fp, aux->der);
    }
    return;
}

/**
 * @brief Función que guarda el árbol binario en un archivo
 * @param aux puntero al nodo raiz del árbol
 * @return void
 */
extern void guardarArbol(tipoHoja *aux, char nombreArchivo[])
{
    FILE *fp;
    fp = fopen(nombreArchivo, "w");

    if (fp == NULL)
    {
        printf(RED "\nERROR: No se pudo abrir el archivo arbol.txt\n" RESET);
        exit(1);
    }

    guardarNodo(fp, aux);
    fclose(fp);

    return;
}

/**
 * @brief Función que carga el árbol binario desde un archivo
 * @param aux puntero al nodo raiz del árbol
 * @param nombreArchivo nombre del archivo que contiene el árbol
 * @return void
 */
extern void cargarArbol(tipoHoja **aux, char nombreArchivo[])
{
    FILE *fp;
    int numCta, pizzas, tacos;
    float total;
    char nombreCliente[30];

    fp = fopen(nombreArchivo, "r");
    if (fp == NULL)
    {
        // Si no se puede abrir el archivo, se crea uno nuevo
        fp = fopen(nombreArchivo, "w");
        if (fp == NULL)
        {
            printf(RED "\nERROR: No se pudo crear el archivo arbol.txt\n" RESET);
            exit(1);
        }
    }

    while (fscanf(fp, "%d\t%s\t%d\t%d\t%f", &numCta, nombreCliente, &pizzas, &tacos, &total) == 5)
    {
        *aux = insertarHoja(*aux, numCta, nombreCliente, pizzas, tacos, total);
    }
    fclose(fp);

    return;
}