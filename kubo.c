/**
 *
 * @file kubo.c
 * @brief En una universidad muy famosa existe la cafetería El Kubo. Cuenta con servicio
 *        de alimentos de comida: Pizzas y Tacos.
 *        El Kubo requiere de un sistema que mejore el servicio y facturación de los alimentos.
 * @details A) Los clientes llegan a formar una sola cola en caja (FIFO). En la caja, los clientes
 *             una vez que pagan su pizza o sus tacos (son atendidos) salen de la cola.
 *          B) De ahí sus pedidos son enviados a una cola de tacos (FIFO) o a una cola de pizzas (FIFO circular).
 *          C) Los clientes llegan a la cola de pizzas o tacos y una vez que les entregan su comida, salen de la
 *             cola sus solicitudes (de pizzas o tacos).
 *          D) El negocio lleva los tickets de facturación en un histórico que es una pila (stack) dfacturas (LIFO).
 *          E) Todas las colas del sistema están integradas a una Lista Doble Lineal.
 *          F) El sistema inicia con una secuencia de clientes que está contenida en un archivo de texto y otro archivo
 *             la configuración del negocio: clientes.txt y negocio.txt.
 *             Dentro de clientes.txt la información se encuentra en el formato: # de Cuenta\tNombre\tMonedero
 *             Dentro de negocio.txt la información se encuentra en el formato: Terminal\tClientes\tMonto Acumulado
 *          G) El programa debe imprimir el estado de todas las colas.
 *          H) Para salir del sistema no debe haber clientes en las colas de tacos y pizzas.
 *          I) Al salir se debe de guardar un archivo con las facturas del día en el formato:
 *             # de factura\tnombre\tcompra\total facturado
 *             Y en la parte inferior del archivo se debe de guardar el monto total facturado.
 *          J) El programa debe almacenar los clientes restantes de la cola de la caja en un archivo de texto
 *             en el formato: # de Cuenta\tNombre\tMonedero
 * @date 05/04/2025
 * @author Mario Ureña García, Ricardo Ponce de León Vargas y Emiliano Cisneros Cervantes
 *
 */

#include "defKubo.h"

// Declaración de funciones --------------------------------------------------------------
// Funciones de la lista doble -----------------------------------------------------------
void insertarColaD(nodoD **first, nodoD **last, char nombreCola[], int tickets, float total);
void imprimirListaD(nodoD *aux);
int menuOpciones(nodoD *aux);
void imprimirTerminal(nodoD *aux);
void presioneEnter();
void insertarCaja(nodoD **first, int numCuenta, char nombreCliente[], float monedero);
void existenTerminalesFundamentales(nodoD *aux);
void imprimirFacturas(nodoFactura *aux);
void atenderCaja(nodoD **caja, tipoHoja **arbol);
void borrarCliente(nodoD **terminal);
void imprimirCola(nodoCola *primeroFila, nodoCola *ultimoFila);
void crearArchivoFacturas(char nombreArchivo[], nodoD *aux);
int verificarColas(nodoD *aux);
void crearArchivoClientes(char nombreArchivo[], nodoD *aux);
nodoD *buscarNodo(nodoD *aux, char nombreCola[]);
// Funciones del árbol binario ----------------------------------------------------------
void imprimirArb(tipoHoja *aux);
// Main ---------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    // argv[1] debe ser el archivo negocio.txt
    // argv[2] debe ser el archivo clientes.txt

    // Variables ------------------------------------------------------------------------
    nodoD *inicio, *fin;

    // Variable auxiliar para selección dinámica de atención, es decir, la caja que se atenderá o imprimirá
    nodoD *seleccion;
    int opcion;

    // Variables para la lectura de datos de negocio.txt
    char nombreCola[20];
    int tickets;
    float total;

    // Variables para la lectura de datos de clientes.txt
    int numCuenta;
    char nombreCliente[20];
    float monedero;

    // Para el nombre del archivo de facturas y el comando para moverlo
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char nombreArchivo[50];
    char comando[120];
    FILE *fp;

    // Variables para el árbol binario
    tipoHoja *raiz;

    // Inicio Personalizado-----------------------------------------------------------
    printf("\n\n\tEste código fue creado por Mario Ureña García, Ricardo Ponce de León Vargas y Emiliano Cisneros Cervantes\n\n");
    printf("\n\tBienvenido al sistema de gestión de colas del Kubo!" RESET "\n\n");
    presioneEnter();
    //-------------------------------------------------------------------------------------

    // Procesos---------------------------------------------------------------------------

    // Inicialización de la lista doble
    inicio = NULL;
    fin = NULL;

    // Inicialización del árbol binario
    raiz = NULL;

    // Carga de datos desde el archivo de negocio
    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf(RED "\n\n\tError al abrir el archivo de negocio\n\n" RESET);
        return 1;
    }

    // Lectura de datos del archivo negocio.txt
    printf(YELLOW "Cargando datos de negocio.txt...\n\n" RESET);
    while (fscanf(fp, "%s\t%d\t%f", nombreCola, &tickets, &total) == 3)
    {
        // Se inserta en la lista doble
        printf(CYAN "Creando nodo de %s\n" RESET, nombreCola);
        insertarColaD(&inicio, &fin, nombreCola, tickets, total);
        printf(GREEN "Nodo de %s creado con éxito\n" RESET, nombreCola);
    }
    fclose(fp);
    presioneEnter();

    printf(GREEN "Lista doble creada con éxito\n" RESET);
    imprimirListaD(inicio); // Imprimir lista doble para verificar que se cargaron los datos correctamente
    presioneEnter();

    // Verificar que existan las terminales fundamentales
    printf(YELLOW "Verificando que existan las terminales fundamentales\n\n" RESET);
    existenTerminalesFundamentales(inicio);
    printf(GREEN "Terminales fundamentales verificadas con éxito\n" RESET);
    printf(GREEN "Pila de facturas inicializada con éxito\n" RESET);
    presioneEnter();

    // Carga de datos desde el archivo de clientes a la terminal de la caja
    fp = fopen(argv[2], "r");
    if (fp == NULL)
    {
        printf(RED "\n\n\tError al abrir el archivo de clientes\n\n" RESET);
        return 1;
    }
    // Lectura de datos del archivo clientes.txt
    printf(YELLOW "Cargando datos de clientes.txt...\n\n" RESET);
    while (fscanf(fp, "%d\t%s\t%f", &numCuenta, nombreCliente, &monedero) == 3)
    {
        printf(CYAN "Insertando cliente %s con cuenta %d y monedero %.2f\n" RESET, nombreCliente, numCuenta, monedero);
        insertarCaja(&inicio, numCuenta, nombreCliente, monedero);
        printf(GREEN "Cliente %s insertado con éxito\n" RESET, nombreCliente);
    }
    printf(GREEN "\n\nClientes cargados con éxito\n" RESET);
    fclose(fp);
    presioneEnter();

    // Imprimir menú de opciones de impresión/atención
    do
    {

        opcion = menuOpciones(inicio);
        getchar(); // Limpiar el buffer de entrada

        // Si la opción es 1, se imprimen todas las colas
        if (opcion == 1)
        {
            system("clear");
            printf("\n\n\tImprimiendo estado de todas las colas\n\n");
            imprimirListaD(inicio);
        }
        // Si la opción es 2, se imprime la cola de caja
        else if (opcion == 2)
        {
            system("clear");
            printf("\n\n\tImprimiendo cola de Caja\n\n");

            // Buscar el nodo de la caja
            seleccion = buscarNodo(inicio, "Caja");

            imprimirCola(seleccion->primero, seleccion->ultimo);
        }
        // Si la opción es 3, se atiende la cola de caja
        else if (opcion == 3)
        {
            system("clear");
            printf("\n\n\tAtendiendo cola de Caja\n\n");

            // Buscar el nodo de la caja
            seleccion = buscarNodo(inicio, "Caja");

            // Atender la cola de caja
            atenderCaja(&seleccion, &raiz);
            borrarCliente(&seleccion);
            getchar(); // Limpiar el buffer de entrada
        }
        // Si la opción es 4, se imprime la cola de pizzas
        else if (opcion == 4)
        {
            system("clear");
            printf("\n\n\tImprimiendo cola de Pizzas\n\n");

            // Buscar el nodo de las pizzas
            seleccion = buscarNodo(inicio, "Pizzas");

            imprimirCola(seleccion->primero, seleccion->ultimo);
        }
        // Si la opción es 5, se atiende la cola de pizzas
        else if (opcion == 5)
        {
            system("clear");
            printf("\n\n\tAtendiendo cola de Pizzas\n\n");

            // Buscar el nodo de las pizzas
            seleccion = buscarNodo(inicio, "Pizzas");

            // Atender la cola de pizzas
            borrarCliente(&seleccion);
            printf(GREEN "\n\nCliente atendido con éxito" RESET);
        }
        // Si la opción es 6, se imprime la cola de tacos
        else if (opcion == 6)
        {
            system("clear");
            printf("\n\n\tImprimiendo cola de Tacos\n\n");

            // Buscar el nodo de los tacos
            seleccion = buscarNodo(inicio, "Tacos");

            imprimirCola(seleccion->primero, seleccion->ultimo);
        }
        // Si la opción es 7, se atiende la cola de tacos
        else if (opcion == 7)
        {
            system("clear");
            printf("\n\n\tAtendiendo cola de Tacos\n\n");

            // Buscar el nodo de los tacos
            seleccion = buscarNodo(inicio, "Tacos");

            // Atender la cola de tacos
            borrarCliente(&seleccion);
            printf(GREEN "\n\nCliente atendido con éxito" RESET);
        }
        // Si la opción es 8, se imprime la cola de facturación
        else if (opcion == 8)
        {
            system("clear");
            printf("\n\n\tImprimiendo pila de Facturacion\n\n");

            // Buscar el nodo de la facturación
            seleccion = buscarNodo(inicio, "Facturacion");

            imprimirFacturas(seleccion->top);
        }
        // Si la opción es 9, se imprime el árbol binario
        else if (opcion == 9)
        {
            system("clear");
            printf("\n\n\tImprimiendo árbol binario\n\n");
            if (raiz == NULL)
            {
                printf(GREEN "\n\n\tEl árbol binario está vacío" RESET);
            }
            else
            {
                imprimirArb(raiz);
            }
        }

        // Si la opción es 0, se debe verificar que no haya clientes en las colas de comida
        else if (opcion == 0)
        {
            opcion = verificarColas(inicio);
        }
        presioneEnter();
    } while (opcion != 0); // La opción 0 es para salir del programa

    // Cuando se sale del programa, se almacenan los datos de las facturas en un archivo
    // Se crea el archivo de facturas, el nombre debe tener la fecha y hora actual
    sprintf(nombreArchivo, "facturas_%04d-%02d-%02d_%02d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    crearArchivoFacturas(nombreArchivo, inicio);

    // Crear carpeta para almacenar el archivo de facturas si no existe
    if (system("mkdir -p facturas") == -1)
    {
        printf(RED "\n\n\tError al crear la carpeta de facturas\n\n" RESET);
        return 1;
    }
    // Mover el archivo de facturas a la carpeta de facturas
    sprintf(comando, "mv %s facturas/%s", nombreArchivo, nombreArchivo);
    if (system(comando) == -1)
    {
        printf(RED "\n\n\tError al mover el archivo de facturas\n\n" RESET);
        return 1;
    }
    else
    {
        printf(GREEN "\n\nArchivo de facturas creado con éxito\n\n" RESET);
        printf(GREEN "El archivo de facturas se ha creado con el nombre " YELLOW "%s" RESET, nombreArchivo);
    }

    // Se crea el archivo de clientes restantes
    crearArchivoClientes("clientes.txt", inicio);
    printf(GREEN "\n\nLos clientes restantes se han almacenado en \"clientes.txt\"\n\n" RESET);

    // FINALIZACIÓN DEL PROGRAMA ---------------------------------------------------------
    printf("\n\n\tPrograma Finalizado con " GREEN "ÉXITO !!!" RESET "\n\n");
    return 0;
}