/*
Se incluyen las librerías necesarias para el funcionamiento
del código, por ejemplo para operaciones matemáticas se tiene
math.h y stdio para entrada de datos del usuario.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Se define la función solucion_ecuacion que recibe los coeficientes
introducidos por el usuario y retorna el valor de la ecuación evaluada
en x

*/
double solucion_ecuacion(double a, double b, double c, double x) {
    return a * pow(x, 2) + b * x + c;
}

/*
Se define la función encontrar_intervalos, que encuentra el intervalo
o subdominio donde la diferencia entre las cuadráticas que se construyeron no 
supere el porcentaje de error, el subdominio principal va de -100000 a 100000
*/
void encontrar_intervalos(double a, double b, double c, double d, double e, double f, double error, double* intervalo_1, double* intervalo_2) {
      
    double x = -100000;
    double intervalos[4];
    int i = 0;
    int intervalo_encontrado = 0;
    
    /*
    Se inicia un bucle que itera desde -100000 hasta 100000, 
    en este bucle se calculan las soluciones de cada ecuación
    para el valor de x actual y se calcula el error entre estas
    ecuaciones, hasta que el error calculado sea mayor que el 
    introducido por el usuario no se encontrará un intervalo
    válido.

    */
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
        /*
        para cada iteración del ciclo se suma 0.1 a la x
        */
        x += 0.1;
    
    /*
    Se verifica si se encontraron dos límites válidos 
    o cuatro límites válidos y se pasan los límites encontrados
    de la matriz intervalos a las matrices para cada intervalo
    si no se tienen la cantidad de limites necesaria las matrices
    se dejan con entradas cero.
    */
    
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
    
/*
Se le pide al usuario las variables necesarias para construir las ecucacione
cuadráticas y el error.
*/
    double a, b, c, d, e, f, error;
    printf("Ingrese los coeficientes de la primera ecuación cuadrática (a, b, c): ");
    scanf("%lf %lf %lf", &a, &b, &c);
    printf("Ingrese los coeficientes de la segunda ecuación cuadrática (d, e, f): ");
    scanf("%lf %lf %lf", &d, &e, &f);

    printf("Ingrese el porcentaje de error (0-100): ");
    scanf("%lf", &error);
/*
Si el usuario introduce un valor para el porcentaje de error que sea menor que cero
o mayor que 100 el programa devuelve un mensaje de erorr y termina.
*/
    if (error < 0 || error > 100) {
        printf("El porcentaje de error debe estar entre 0 y 100.\n");
        return 1;
    }
/*
Se crean las variables para contener los limites de los intervalos o subdominios
y se utiliza la función encontrar intervalos con los datos recibidos del usuario.
*/
    double intervalo_1[2], intervalo_2[2];
    encontrar_intervalos(a, b, c, d, e, f, error, intervalo_1, intervalo_2);
    /*
    si los limites de ambos intervalos son cero, entonces se imprime que no se encontraron
    intervalos
    */
    if (intervalo_1[0] == 0 && intervalo_1[1] == 0 && intervalo_2[0] == 0 && intervalo_2[1] == 0) {
        printf("No se encontraron intersecciones.\n");
        return 0;
    }

    /*
    Si se encontraron los subdominios entonces estos se imprimen. 
    */
    printf("El primer subdominio encontrado es:\n");
    printf("[%0.1f, %0.1f]\n", intervalo_1[0], intervalo_1[1]);
    printf("El segundo subdominio encontrado es:\n");
    printf("[%0.1f, %0.1f]\n", intervalo_2[0], intervalo_2[1]);
    printf("Donde el error es menor a %0.1f\n", error);


    /*
    Se declara el rango de valores de x para la gráfica de las funciones cuadráticas
    */
    double x_min = -50.0;
    double x_max = 50.0;

    /*
    Se abre un proceso de gnuplot en modo de escritura para poder gráficar y 
    se verifica si se tuvo un error abriendo el proceso, si se tuviera un 
    error con gnuplot se termina el programa
    */
    FILE *gp = popen("gnuplot -persist", "w");
    if (gp == NULL) {
        printf("Error al abrir Gnuplot.\n");
        return 1;
    }

    /*
    Se asigna el título de las grafica y de los ejes 
    */
    fprintf(gp, "set title 'Gráfica subdominios'\n");
    fprintf(gp, "set xlabel 'x'\n");
    fprintf(gp, "set ylabel 'y'\n");

    /*
    Se definen las dos ecuaciones cuadráticas en gnuplo
    */
    fprintf(gp, "f1(x) = %f*x**2 + %f*x + %f\n", a, b, c);
    fprintf(gp, "f2(x) = %f*x**2 + %f*x + %f\n", d, e, f);

    /*
    Se configura la gráfica para poder mostrar las funciones cuadráticas en el rango
    desde la x miníma a la x máxima y se configura su leyenda, además de configurar las 
    líneas verticales que marcan los bordes de los subdominios, además de poner a los
    subdominios en la leyenda.
    */    
    fprintf(gp, "plot [%f:%f] f1(x) with lines title 'Función 1', f2(x) with lines title 'Función 2', ", x_min, x_max);
    fprintf(gp, "'-' with lines linestyle 2 title 'Límites Subdominio 1 [%0.1f, %0.1f]', '-' with lines linestyle 3 title 'Límites Subdominio 2 [%0.1f, %0.1f]'\n", intervalo_1[0], intervalo_1[1], intervalo_2[0], intervalo_2[1]);
    /*
    Se definen las coordenadas para dibujar las líneas que limitan a los subdominios
    estas coordenadas son los límites de los subdominios, se ponen los límites del eje x
    para asegurar que los lìmites en y se extiendan todo lo posible. 
    */
    
    fprintf(gp, "%f %f\n%f %f\n", intervalo_1[0], -100000.0, intervalo_1[0], 100000.0);
    fprintf(gp, "%f %f\n%f %f\n", intervalo_1[1], -100000.0, intervalo_1[1], 100000.0);
    fprintf(gp, "e\n");
    fprintf(gp, "%f %f\n%f %f\n", intervalo_2[0], -100000.0, intervalo_2[0], 100000.0);
    fprintf(gp, "%f %f\n%f %f\n", intervalo_2[1], -100000.0, intervalo_2[1], 100000.0);
    fprintf(gp, "e\n");

    /*
    Se cierra gnuplot
    */
    pclose(gp);
    /*
    Se termina el programa
    */
    return 0;
}