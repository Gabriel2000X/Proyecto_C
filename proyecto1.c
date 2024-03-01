#include <gtk/gtk.h>
#include <stdlib.h>

// Se crean las variables globales necesarias
static GtkWidget *etiqueta_estado;
static GtkWidget *botones[3][3];
static int tablero[3][3] = {{0}};
static int jugador_actual = 1; // Aca se define que inicia el jugador 1

// Se crea una funcion para verificar quien es el ganador o si no hay ganador
int verificar_ganador() {
    // Aca se verifica si hay ganador en fila o columna
    for (int i = 0; i < 3; ++i) {
        if (tablero[i][0] != 0 && tablero[i][0] == tablero[i][1] && tablero[i][1] == tablero[i][2]) 
        return tablero[i][0];
        if (tablero[0][i] != 0 && tablero[0][i] == tablero[1][i] && tablero[1][i] == tablero[2][i]) 
        return tablero[0][i];
    }
    // Verifica si hay ganador en las diagonales
    if (tablero[0][0] != 0 && tablero[0][0] == tablero[1][1] && tablero[1][1] == tablero[2][2]) 
    return tablero[0][0];
    if (tablero[0][2] != 0 && tablero[0][2] == tablero[1][1] && tablero[1][1] == tablero[2][0]) 
    return tablero[0][2];
    return 0;  // Si no se cumple ninguna de las anteriores, devuelve 0, ya que no hay ganador
}

// Esta funcion actualiza la etiqueta, o sea indica si hay ganador y quien y si no indica el empate
void actualizar_etiqueta_estado(int ganador) {
    if (ganador == 0) {
        gtk_label_set_text(GTK_LABEL(etiqueta_estado), "¡Es un empate!");
    } else {
        char mensaje[30];
        sprintf(mensaje, "¡El jugador %d gana!", ganador);
        gtk_label_set_text(GTK_LABEL(etiqueta_estado), mensaje);
    }
}

// Esta funcion se utiliza para manejar los botones
void al_clic_en_boton(GtkButton *boton, gpointer user_data) {
    int fila = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(boton), "fila"));
    int columna = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(boton), "columna"));

    // Aca verifica si el boton ya fue presionado o si ya termino el juego para que no permita presionarlo mas
    if (tablero[fila][columna] != 0 || verificar_ganador() != 0) return;

    // Aca se selecciona si es x o o dependiendo del jugador actual
    const char *simbolo = (jugador_actual == 1) ? "X" : "O"; 
    tablero[fila][columna] = jugador_actual;
    gtk_button_set_label(boton, simbolo);

    // Aca llama la funcion para verificar si ya hay ganador o no
    int ganador = verificar_ganador();
    if (ganador != 0) {
        actualizar_etiqueta_estado(ganador);
    } else {
        // Se revisa si no hay ganador, por lo tanto el empate
        int empate = 1;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (tablero[i][j] == 0) {
                    empate = 0;
                    break;
                }
            }
            if (!empate) break;
        }
        if (empate) actualizar_etiqueta_estado(0);  // Define si es empate
        else {
            // Aca se hace la alternacion de los jugadores, se pasa de 1 a 2
            jugador_actual = (jugador_actual == 1) ? 2 : 1;
            char mensaje[20];
            sprintf(mensaje, "Turno del jugador %d", jugador_actual);
            gtk_label_set_text(GTK_LABEL(etiqueta_estado), mensaje);
        }
    }
}

// Se crea una funcion para poder reiniciar el juego con unboton
void reiniciar_juego(GtkButton *boton, gpointer user_data) {
    // Esta funcion limpia el tablero
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            tablero[i][j] = 0;
            gtk_button_set_label(GTK_BUTTON(botones[i][j]), " ");
        }
    }
    // Aca reinicia el mensaje e imprime que le toca al jugador 1
    jugador_actual = 1;
    gtk_label_set_text(GTK_LABEL(etiqueta_estado), "Turno del jugador 1");
}

// Se crea la funcion main, aca se inicializa GTK
int main(int argc, char *argv[]) {
    // funcion para iniciar GTK
    gtk_init(&argc, &argv);

    // se carga el archivo de glade
    GtkBuilder *builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(builder, "gato.glade", NULL)) {
        g_error("Error al cargar el archivo Glade: %s", "gato.glade");
        return EXIT_FAILURE;
    }

    // Aca se carga la ventada principal de glade, y se pone la funcion para terminarla.
    GtkWidget *ventana = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    if (ventana == NULL) { // Se agrego una parte de verificacion que se cargo bien
        g_error("No se puede obtener la ventana principal del archivo Glade.");
        return EXIT_FAILURE;
    }
    g_signal_connect(ventana, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Aca se carga el titulo que es la etiqueta de estado, y los botones del gato.
    etiqueta_estado = GTK_WIDGET(gtk_builder_get_object(builder, "status_label"));
    if (etiqueta_estado == NULL) { //Se agrego una verificacion para saber si se cargo bien
        g_error("No se puede obtener la etiqueta de estado del archivo Glade.");
        return EXIT_FAILURE;
    }
    botones[0][0] = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    botones[0][1] = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));
    botones[0][2] = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));
    botones[1][0] = GTK_WIDGET(gtk_builder_get_object(builder, "button4"));
    botones[1][1] = GTK_WIDGET(gtk_builder_get_object(builder, "button5"));
    botones[1][2] = GTK_WIDGET(gtk_builder_get_object(builder, "button6"));
    botones[2][0] = GTK_WIDGET(gtk_builder_get_object(builder, "button7"));
    botones[2][1] = GTK_WIDGET(gtk_builder_get_object(builder, "button8"));
    botones[2][2] = GTK_WIDGET(gtk_builder_get_object(builder, "button9"));

    // Se agrego una verificacion para revisar si los botones se cargaron bien
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (botones[i][j] == NULL) {
                g_error("No se puede obtener el botón [%d][%d] del archivo Glade.", i, j);
                return EXIT_FAILURE;
            }
        }
    }

    // Se hacen las conexiones entre las senales del programa y los botones de la interfaz
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            g_signal_connect(botones[i][j], "clicked", G_CALLBACK(al_clic_en_boton), NULL);
            g_object_set_data(G_OBJECT(botones[i][j]), "fila", GINT_TO_POINTER(i));
            g_object_set_data(G_OBJECT(botones[i][j]), "columna", GINT_TO_POINTER(j));
        }
    }

    // Se crea el boton del reinicio, se conecta a la senal y se verifica si se cargo bien
    GtkWidget *boton_reinicio = GTK_WIDGET(gtk_builder_get_object(builder, "restart_button"));
    if (boton_reinicio == NULL) {
        g_error("No se puede obtener el botón de reinicio del archivo Glade.");
        return EXIT_FAILURE;
    }
    g_signal_connect(boton_reinicio, "clicked", G_CALLBACK(reiniciar_juego), NULL);

    // Imprime todos los botones y etiquetas en la pantalla
    gtk_widget_show_all(ventana);

    // Inicia el bucle del juego
    gtk_main();

    return EXIT_SUCCESS;
}
