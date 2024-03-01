#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_POINTS 10000

double solucion_ecuacion(double a, double b, double c, double x) {
    return a * pow(x, 2) + b * x + c;
}

void encontrar_intervalos(double a, double b, double c, double d, double e, double f, double error, double* intervalo_1, double* intervalo_2) {
    double x = -100000;
    double intervalos[4];
    int i = 0;
    int intervalo_encontrado = 0;
    
    while (x <= 100000) {
        double fx = solucion_ecuacion(a, b, c, x);
        double gx = solucion_ecuacion(d, e, f, x);
        double error1 = fabs(100 * ((fx - gx) / fx));
        
        if (error1 < error && intervalo_encontrado == 0) {
            intervalos[i++] = round(x * 10) / 10;
            intervalo_encontrado = 1;
        } else if (error1 > error && intervalo_encontrado == 1) {
            intervalos[i++] = round((x - 0.1) * 10) / 10;
            intervalo_encontrado = 0;
        }
        x += 0.1;
    }
    if (i == 2 || i == 4) {
        intervalo_1[0] = intervalos[0];
        intervalo_1[1] = intervalos[1];
        intervalo_2[0] = intervalos[2];
        intervalo_2[1] = intervalos[3];
    } else {
        intervalo_1[0] = 0;
        intervalo_1[1] = 0;
        intervalo_2[0] = 0;
        intervalo_2[1] = 0;
    }
}

int main() {
    // Coeficientes de las dos ecuaciones cuadráticas: y = ax^2 + bx + c
    double a, b, c, d, e, f, error;
    printf("Ingrese los coeficientes de la primera ecuación cuadrática (a, b, c): ");
    scanf("%lf %lf %lf", &a, &b, &c);
    printf("Ingrese los coeficientes de la segunda ecuación cuadrática (a, b, c): ");
    scanf("%lf %lf %lf", &d, &e, &f);

    printf("Ingrese el porcentaje de error (0-100): ");
    scanf("%lf", &error);

    if (error < 0 || error > 100) {
        printf("El porcentaje de error debe estar entre 0 y 100.\n");
        return 1;
    }

    double intervalo_1[2], intervalo_2[2];
    encontrar_intervalos(a, b, c, d, e, f, error, intervalo_1, intervalo_2);

    if (intervalo_1[0] == 0 && intervalo_1[1] == 0 && intervalo_2[0] == 0 && intervalo_2[1] == 0) {
        printf("No se encontraron intersecciones.\n");
        return 0;
    }

    printf("El primer subdominio encontrado es:\n");
    printf("[%0.1f, %0.1f]\n", intervalo_1[0], intervalo_1[1]);
    printf("El segundo subdominio encontrado es:\n");
    printf("[%0.1f, %0.1f]\n", intervalo_2[0], intervalo_2[1]);
    printf("Donde el error es menor a %0.1f\n", error);

    // Rango de valores para x
    double x_min = -50;
    double x_max = 50.0;

    // Abrir el proceso de Gnuplot
    FILE *gp = popen("gnuplot -persist", "w");
    if (gp == NULL) {
        printf("Error al abrir Gnuplot.\n");
        return 1;
    }

    // Configurar el título y las etiquetas de los ejes
    fprintf(gp, "set title 'Gráfica de las Funciones Cuadráticas'\n");
    fprintf(gp, "set xlabel 'x'\n");
    fprintf(gp, "set ylabel 'y'\n");

    // Definir las ecuaciones cuadráticas
    fprintf(gp, "f1(x) = %f*x**2 + %f*x + %f\n", a, b, c);
    fprintf(gp, "f2(x) = %f*x**2 + %f*x + %f\n", d, e, f);

    // Graficar las ecuaciones cuadráticas y las líneas verticales
    fprintf(gp, "plot [%f:%f] f1(x) with lines title 'Función 1', f2(x) with lines title 'Función 2', ", x_min, x_max);
    fprintf(gp, "'-' with lines linestyle 2 title 'Subdominio 1', '-' with lines linestyle 3 title 'Subdominio 2'\n");
    fprintf(gp, "%f %f\n%f %f\n", intervalo_1[0], solucion_ecuacion(a, b, c, intervalo_1[0]), intervalo_1[1], solucion_ecuacion(a, b, c, intervalo_1[1]));
    fprintf(gp, "e\n");
    fprintf(gp, "%f %f\n%f %f\n", intervalo_2[0], solucion_ecuacion(d, e, f, intervalo_2[0]), intervalo_2[1], solucion_ecuacion(d, e, f, intervalo_2[1]));
    fprintf(gp, "e\n");

    // Cerrar Gnuplot
    pclose(gp);

    return 0;
}
