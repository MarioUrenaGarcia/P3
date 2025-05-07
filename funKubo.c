#include "defKubo.h"

/**
 * @file kubo.c
 * @brief En este archivo se almacenan las funciones de kubo.c
 * @date 04/04/2025
 * @author Mario Ureña García, Ricardo Ponce de León Vargas y Emiliano Cisneros Cervantes
 */

// Funciones del árbol binario ----------------------------------------------------------
tipoHoja *insertarHoja(tipoHoja *pt, int numCta, char nombreCliente[], int pizzas, int tacos, float total);
int buscarCliente(tipoHoja *aux, int numCta);

// Funciones ----------------------------------------------------------------------------
/**
 * @brief Función que busca un nodo en la lista doble usando el nombre del nodo.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @param nombreCola[] Nombre de la cola a buscar.
 * @return *aux Puntero al nodo encontrado o NULL si no se encuentra.
 */
extern nodoD *buscarNodo(nodoD *aux, char nombreCola[])
{
    // Buscar el nodo con el nombre de la cola
    while (aux != NULL)
    {
        if (strcmp(aux->terminal, nombreCola) == 0)
        {
            break; // Se encontró el nodo
        }
        aux = aux->next;
    }
    return aux; // Si no se encuentra el nodo, se devuelve NULL
}

/**
 * @brief Función que inserta un cliente en la cola de comida.
 * @param **terminal Puntero a la lista doble que contiene los puestos de comida.
 * @param *cliente Puntero a la cola que contiene los datos del cliente.
 * @return void
 */
void insertarClienteComida(nodoD **terminal, nodoCola *cliente)
{
    nodoCola *nuevo;

    nuevo = (nodoCola *)malloc(sizeof(nodoCola));
    if (nuevo == NULL)
    {
        printf(RED "\nERROR: No hay memoria disponible\n" RESET);
        exit(1);
    }
    nuevo->numCuenta = cliente->numCuenta;
    strcpy(nuevo->nombre, cliente->nombre);
    nuevo->monedero = cliente->monedero;
    nuevo->next = NULL;

    // Caso: Cola vacía
    if (((*terminal)->primero == NULL) && ((*terminal)->ultimo == NULL))
    {
        (*terminal)->primero = nuevo;
        (*terminal)->ultimo = nuevo;
    }
    else // Caso: Cola no vacía
    {
        (*terminal)->ultimo->next = nuevo;
        (*terminal)->ultimo = nuevo;
    }
    // Actualizar el número de clientes y el monto acumulado
    (*terminal)->clientes++;
    (*terminal)->montoAcumulado += nuevo->monedero;
    return;
}

/**
 * @brief Función que crea una factura y la inserta en la pila de facturas.
 * @param **facturacion Puntero a la lista doble que contiene la terminal de facturacion.
 * @param nombreCliente[] Nombre del cliente.
 * @param nombreTerminal[] Nombre de la terminal donde se realizó la compra.
 * @param total Total de la compra.
 * @return void
 */
void crearFactura(nodoD **facturacion, char nombreCliente[], char nombreTerminal[], float total)
{
    static int numeroFactura = 1;
    nodoFactura *nuevo;

    nuevo = (nodoFactura *)malloc(sizeof(nodoFactura));
    if (nuevo == NULL)
    {
        printf(RED "\nERROR: No hay memoria disponible\n" RESET);
        exit(1);
    }

    // Crear la factura
    nuevo->numFactura = numeroFactura;
    strcpy(nuevo->nombre, nombreCliente);
    strcpy(nuevo->compra, nombreTerminal);
    nuevo->totalFacturado = total;
    nuevo->next = (*facturacion)->top;
    (*facturacion)->top = nuevo;
    numeroFactura++;

    // Actualizar el número de clientes y el monto acumulado
    (*facturacion)->clientes++;
    (*facturacion)->montoAcumulado += total;
    return;
}

/**
 * @brief Función que crea un archivo de texto con los datos de los clientes en la cola de caja.
 * @param nombreArchivo[] Nombre del archivo a crear.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void crearArchivoClientes(char nombreArchivo[], nodoD *aux)
{
    FILE *fp;
    nodoCola *clientesInicio, *clientesFin;

    // Buscar la terminal de caja
    while (aux != NULL && (strcmp(aux->terminal, "Caja") != 0))
    {
        aux = aux->next;
    }
    // Cuando se encuentra la terminal de caja, se asigna a clientes
    clientesInicio = aux->primero;
    clientesFin = aux->ultimo;
    // Crear el archivo de clientes
    fp = fopen(nombreArchivo, "w");
    if (fp == NULL)
    {
        printf(RED "\nERROR: No se pudo crear el archivo de clientes\n" RESET);
        exit(1);
    }
    // Imprimir la lista de clientes
    // Verificar que la cola no esté vacía
    if (clientesInicio == NULL && clientesFin == NULL)
    {
        fprintf(fp, "0\n");
    }
    else
    {
        do
        {
            fprintf(fp, "%d\t%s\t%.2f\n", clientesInicio->numCuenta, clientesInicio->nombre, clientesInicio->monedero);
            clientesInicio = clientesInicio->next;
        } while (clientesInicio != clientesFin->next);
    }

    return;
}

/**
 * @brief Función que verifica si hay clientes en las colas de comida.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return clientes 1 si hay clientes en las colas de comida, 0 si no hay clientes.
 */
extern int verificarColas(nodoD *aux)
{
    int clientes = 0;
    // Verificar que no haya clientes en las colas de comida
    while (aux != NULL)
    {
        if (strcmp(aux->terminal, "Caja") != 0 && strcmp(aux->terminal, "Facturacion") != 0)
        {
            if (aux->primero != NULL)
            {
                printf(RED "\n\n\tError: Aún hay clientes en la cola de %s, no puedes cerrar el programa!\n\n" RESET, aux->terminal);
                clientes = 1;
            }
        }
        aux = aux->next;
    }

    return clientes;
}

/**
 * @brief Función que crea un archivo de texto con los datos de las facturas.
 * @param nombreArchivo[] Nombre del archivo a crear.
 * @param aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void crearArchivoFacturas(char nombreArchivo[], nodoD *aux)
{
    FILE *fp;
    nodoFactura *facturas = NULL;

    fp = fopen(nombreArchivo, "w");
    if (fp == NULL)
    {
        printf(RED "\nERROR: No se pudo crear el archivo de facturas\n" RESET);
        exit(1);
    }
    // Buscar la terminal de facturacion
    while (aux != NULL && (strcmp(aux->terminal, "Facturacion") != 0))
    {
        aux = aux->next;
    }

    // Imprimir la pila de facturas
    facturas = aux->top;

    // Si no hay facturas, se almacena como 0
    if (facturas == NULL)
    {
        fprintf(fp, "0\n");
    }
    // Imprimir la lista de facturas
    while (facturas != NULL)
    {
        fprintf(fp, "%d\t%s\t%s\t%.2f\n", facturas->numFactura, facturas->nombre, facturas->compra, facturas->totalFacturado);
        facturas = facturas->next;
    }
    fclose(fp);

    return;
}

/**
 * @brief Función que elimina un cliente de la cola.
 * @param **terminal Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void borrarCliente(nodoD **terminal)
{
    nodoCola *borra;

    // Verificar si la cola está vacía
    if ((*terminal)->primero == NULL && (*terminal)->ultimo == NULL)
    {
        printf(RED "\nLa cola %s está vacía" RESET, (*terminal)->terminal);
    }
    else
    {
        borra = (*terminal)->primero;

        // Caso: Un solo cliente en la cola
        if ((*terminal)->primero == (*terminal)->ultimo)
        {
            (*terminal)->primero = NULL;
            (*terminal)->ultimo = NULL;
        }
        else // Caso: Más de un cliente en la cola
        {
            (*terminal)->primero = (*terminal)->primero->next;
            (*terminal)->ultimo->next = (*terminal)->primero; // Mantener la circularidad
        }

        free(borra); // Liberar la memoria del cliente eliminado

        // Actualizar el número de clientes y el monto acumulado
        (*terminal)->clientes--;
        (*terminal)->montoAcumulado -= borra->monedero;

        printf(GREEN "\nCliente eliminado de la cola %s" RESET, (*terminal)->terminal);
    }
    return;
}

/**
 * @brief Función que imprime el menú de opciones y devuelve la opción seleccionada por el usuario.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @param **seleccion Puntero a la lista doble que contiene el puesto que se va a atender.
 * @return Opción seleccionada por el usuario.
 */
extern int menuOpciones(nodoD *aux, nodoD **seleccion)
{
    int opcion, validez = 2;
    nodoD *first = aux;

    printf("\n");

    // Para que sea funcional no importando cuantos puestos de comida existan, se imprimiran tantas
    // opciones como puestos de comida existan. Por cada puesto de comida existirá la opción de
    // imprimir la cola de ese puesto de comida o atender esa cola.

    do
    {

        if (validez == 1)
        {
            printf(RED "\nOpción inválida. Intente de nuevo.\n" RESET);
        }
        printf(GREEN "1." RESET " Imprimir estado de todas las colas\n");
        printf(CYAN "2." RESET " Imprimir cola de Caja\n");
        printf(YELLOW "3." RESET " Atender cola de Caja\n");
        printf(CYAN "4." RESET " Imprimir cola de Pizzas\n");
        printf(YELLOW "5." RESET " Atender cola de Pizzas\n");
        printf(CYAN "6." RESET " Imprimir cola de Tacos\n");
        printf(YELLOW "7." RESET " Atender cola de Tacos\n");
        printf(CYAN "8." RESET " Imprimir cola de Facturacion\n");
        printf(CYAN "9." RESET " Imprimir árbol binario\n");

        printf(BG_WHITE BLACK "0. Salir\n" RESET);
        printf("\nElija una opción: ");
        scanf(" %d", &opcion);

        // Validación de la opción
        if (opcion < 0 || opcion > 9) // Opción inválida
        {
            validez = 1;
        }
        else // Opción válida
        {
            validez = 0;
        }

    } while (validez == 1);

    return opcion;
}

/**
 * @brief Función que inserta un nodoD en la lista doble.
 * @param first Puntero a la cabeza de la lista doble.
 * @param last Puntero a la cola de la lista doble.
 * @param nombreCola Nombre de la cola a insertar.
 * @param tickets Número de tickets en la cola.
 * @param total Monto acumulado en la cola.
 */
extern void insertarColaD(nodoD **first, nodoD **last, char nombreCola[], int tickets, float total)
{
    nodoD *nuevo;

    nuevo = (nodoD *)malloc(sizeof(nodoD));
    if (nuevo == NULL)
    {
        printf(RED "\nERROR: No hay memoria disponible\n" RESET);
        exit(1);
    }
    strcpy(nuevo->terminal, nombreCola);
    nuevo->clientes = tickets;
    nuevo->montoAcumulado = total;

    // Inicializar subcolas
    nuevo->primero = NULL;
    nuevo->ultimo = NULL;
    nuevo->top = NULL;

    // Caso: Lista vacía
    if ((*first == NULL) && (*last == NULL))
    {
        nuevo->back = NULL;
        nuevo->next = NULL;
        *first = nuevo;
        *last = nuevo;
    }
    else // Caso: Lista no vacía
    {
        nuevo->back = *last;
        nuevo->next = NULL;
        (*last)->next = nuevo;
        *last = nuevo;
    }

    return;
}

/**
 * @brief Función que inserta un cliente en la cola de caja.
 * @param **first Puntero a la cabeza de la lista doble.
 * @param numCuenta Número de cuenta del cliente.
 * @param nombreCliente[] Nombre del cliente.
 * @param monedero Monto del monedero del cliente.
 * @return void
 */
extern void insertarCaja(nodoD **first, int numCuenta, char nombreCliente[], float monedero)
{
    nodoCola *nuevo;
    nodoD *aux;

    // Puede que la caja no esté en la primera posición del archivo negocios.txt
    // Por lo que se debe buscar en toda la lista doble y almacenarla en una variable caja
    nodoD **caja = NULL;

    // Buscar la cola con nombre Caja
    aux = *first;
    while (aux != NULL)
    {
        if (strcmp(aux->terminal, "Caja") == 0)
        {
            caja = &aux;
            break;
        }
        aux = aux->next;
    }

    nuevo = (nodoCola *)malloc(sizeof(nodoCola));
    if (nuevo == NULL)
    {
        printf(RED "\nERROR: No hay memoria disponible\n" RESET);
        exit(1);
    }
    nuevo->numCuenta = numCuenta;
    strcpy(nuevo->nombre, nombreCliente);
    nuevo->monedero = monedero;
    nuevo->next = NULL;

    // Caso: Cola vacía
    if (((*caja)->primero == NULL) && ((*caja)->ultimo == NULL))
    {
        (*caja)->primero = nuevo;
        (*caja)->ultimo = nuevo;
    }
    else // Caso: Cola no vacía
    {
        (*caja)->ultimo->next = nuevo;
        (*caja)->ultimo = nuevo;
    }

    // Actualizar el número de clientes y el monto acumulado
    (*caja)->clientes++;
    (*caja)->montoAcumulado += nuevo->monedero;

    return;
}

/**
 * @brief Función que imprime el estado de todas las colas.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void imprimirListaD(nodoD *aux)
{
    printf(YELLOW "\nEstado de Cajas\n" RESET);

    if (aux == NULL)
    {
        printf(RED "\nLa lista está vacía\n" RESET);
        return;
    }
    else
    {
        while (aux != NULL)
        {
            printf(YELLOW "\nNombre de la cola:" RESET " %s\n", aux->terminal);
            printf(GREEN "Número de clientes:" RESET " %d\n", aux->clientes);
            printf(GREEN "Monto acumulado:" RESET " $%.2f\n", aux->montoAcumulado);
            aux = aux->next;
        }
    }

    return;
}

/**
 * @brief Función que imprime el estado de una terminal.
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void imprimirTerminal(nodoD *aux)
{
    printf(YELLOW "Estado de la cola de %s\n" RESET, aux->terminal);
    printf(GREEN "Número de clientes: %d\n" RESET, aux->clientes);
    printf(GREEN "Monto acumulado: $%.2f\n\n" RESET, aux->montoAcumulado);

    return;
}

/**
 * @brief Función que imprime la cola de clientes.
 * @param *primeroFila Puntero al primer nodo de la cola.
 * @param *ultimoFila Puntero al último nodo de la cola.
 * @return void
 */
extern void imprimirCola(nodoCola *primeroFila, nodoCola *ultimoFila)
{

    // Verificar que la cola no esté vacía
    if (primeroFila == NULL && ultimoFila == NULL)
    {
        printf(GREEN "\nLa cola está vacía\n" RESET);
    }
    else
    {
        // Imprimir la cola
        printf(GREEN "Cola de clientes:\n" RESET);
        do
        {
            printf(BG_YELLOW BLACK "%d %s %.2f" RESET "-> ", primeroFila->numCuenta, primeroFila->nombre, primeroFila->monedero);
            primeroFila = primeroFila->next;
        } while (primeroFila != ultimoFila->next);
    }
    // Imprimir FIN
    printf(BG_YELLOW BLACK "FIN" RESET "\n");

    printf("\n");
    return;
}

/**
 * @brief Función que imprime la pila de facturas.
 * @param *aux Puntero a la lista de facturas.
 * @return void
 */
extern void imprimirFacturas(nodoFactura *aux)
{
    // Lista vacía
    if (aux == NULL)
    {
        printf(RED "\nLa lista de facturas está vacía\n" RESET);
        return;
    }
    // Imprimir la lista de facturas
    while (aux != NULL)
    {
        printf(BG_YELLOW BLACK "[ %d %s %s %.2f ]" RESET "\n", aux->numFactura, aux->nombre, aux->compra, aux->totalFacturado);
        aux = aux->next;
    }
    printf("\n");

    return;
}

/**
 * @brief Función que atiende la cola de caja.
 * @param **caja Puntero a la lista doble que contiene la terminal de caja.
 * @return void
 */
extern void atenderCaja(nodoD **caja, tipoHoja **arbol)
{
    int cantidad, compraValida = 0;
    float pagar;
    nodoD *nodoCompra;      // Para ubicar la terminal de la compra
    nodoD *nodoFacturacion; // Para ubicar la terminal de facturacion
    nodoCola *clienteActual;

    // Verificar que la cola no esté vacía
    if (((*caja)->primero == NULL) && ((*caja)->ultimo == NULL))
    {
        printf(GREEN "\nLa caja está vacía\n" RESET);
    }
    else // Cola con clientes
    {
        clienteActual = (*caja)->primero;

        // Atender a la caja eliminara al ultimo cliente de la cola FIFO y se copiaran sus datos a la terminal donde haya comprado
        printf("Atendiendo a %s " GREEN "$%.2f" RESET, clienteActual->nombre, clienteActual->monedero);

        // Pizzas
        printf("\nPizzas ($89.00) cantidad:  \n");
        scanf("%d", &cantidad);
        if (cantidad > 0)
        {
            pagar = cantidad * 25;
            if (pagar <= clienteActual->monedero)
            {
                clienteActual->monedero -= pagar;
                printf(GREEN "\nCompra exitosa\n" RESET);
                compraValida = 1;
                nodoCompra = buscarNodo(*caja, "Pizzas");
            }
            else
            {
                printf(RED "\nNo tiene suficiente dinero\n" RESET);
                compraValida = -1;
            }
        }
        else // Tacos
        {
            printf("\nTacos ($25.00) cantidad:  \n");
            scanf("%d", &cantidad);
            if (cantidad > 0)
            {
                pagar = cantidad * 25;
                if (pagar <= clienteActual->monedero)
                {
                    clienteActual->monedero -= pagar;
                    printf(GREEN "\nCompra exitosa" RESET);
                    compraValida = 1;
                    nodoCompra = buscarNodo(*caja, "Tacos");
                }
                else
                {
                    printf(RED "\nNo tiene suficiente dinero\n" RESET);
                    compraValida = -1;
                }
            }
        }

        // Si la compraValida == 1, se elimina el cliente de la cola y se enviará a la colaActual
        // Si la compraValida == -1, solamente se eliminará al cliente de la cola
        if (compraValida == 1)
        {
            insertarClienteComida(&nodoCompra, clienteActual);

            // Buscar la terminal de facturacion
            nodoFacturacion = buscarNodo(*caja, "Facturacion");

            printf(GREEN "\nNodo de facturacion creado" RESET);
            crearFactura(&nodoFacturacion, clienteActual->nombre, nodoCompra->terminal, pagar);

            // Agregar al cliente al árbol binario
            // Si el cliente no existe en el árbol
            if (buscarCliente(*arbol, clienteActual->numCuenta) == 0)
            {
                // Insertar el cliente en el árbol
                printf("\nInsertando cliente en árbol");
                *arbol = insertarHoja(*arbol, clienteActual->numCuenta, clienteActual->nombre, cantidad, cantidad, pagar);
                printf(GREEN "\nCliente insertado en árbol" RESET);
            }
            else // Si el cliente ya existe en el árbol
            {
            }
        }
        // Actualizar valores de terminal
        (*caja)->clientes--;
        (*caja)->montoAcumulado -= clienteActual->monedero;
    }

    return;
}

/**
 * @brief Función que verifica si existen las terminales fundamentales (Caja y Facturacion).
 * @param *aux Puntero a la lista doble que contiene los puestos de comida.
 * @return void
 */
extern void existenTerminalesFundamentales(nodoD *aux) // Esta función inicia la pila de facturas también
{
    // Debe de existir la terminal Caja y la terminal Facturacion
    int caja = 0, facturacion = 0;
    while (aux != NULL)
    {
        if (strcmp(aux->terminal, "Caja") == 0)
        {
            caja = 1;
        }
        if (strcmp(aux->terminal, "Facturacion") == 0)
        {
            facturacion = 1;
            aux->top = NULL; // Inicializar la pila de facturas
        }
        aux = aux->next;
    }
    if (caja == 0)
    {
        printf(RED "\n\n\tError: No existe la terminal Caja\n\n" RESET);
        exit(1);
    }
    if (facturacion == 0)
    {
        printf(RED "\n\n\tError: No existe la terminal Facturacion\n\n" RESET);
        exit(1);
    }
    return;
}

/**
 * @brief Función que espera a que el usuario presione Enter para continuar.
 * @param void
 */
extern void presioneEnter()
{
    printf("\n\nPresione " BG_GREEN "ENTER" RESET " para continuar\n");
    getchar();
    system("clear");

    return;
}