#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

// Muster Ausgabe für input.dat
// 1.0  1.1
// 2.5 -0.5
// 3.4  2.0
// 4.0 -1.0
// 4.5  1.1
// (py-venv3.5.6) codem@thinkpad:~/work/ModSim/2020SS/ub1/src$ ./interpolationspolynom 
// 
//   Ausgabe der dividierten Differenzen D: 
//  1.10000 
//  -0.50000  -1.06667 
//  2.00000  2.77778  1.60185 
//  -1.00000  -5.00000  -5.18519  -2.26235 
//  1.10000  4.20000  8.36364  6.77441  2.58193 
// 
// f(x) = +a_0+a_1*(x-x0)+a_2*(x-x0)*(x-x1)+a_3*(x-x0)*(x-x1)*(x-x2)+a_4*(x-x0)*(x-x1)*(x-x2)*(x-x3)
// f(x) = +1.100000+-1.066667*(x-1.000000)+1.601852*(x-1.000000)*(x-2.500000)+-2.262346*(x-1.000000)*(x-2.500000)*(x-3.400000)+2.581930*(x-1.000000)*(x-2.500000)*(x-3.400000)*(x-4.000000)


//
// In dieser Funktion werden die Wertepaare abgezählt, 
// indem die entsprechenden Zeilen gezählt werden.
//
// Die Ausgabe dieser Funktion ist gleich der Anzahl der Wertepaare.
int getNumberOfPoints(char *name) {
  FILE   *fp;
  char   *line = NULL;
  size_t  len = 0;

  if ((fp = fopen(name, "r")) == NULL) {
    exit(EXIT_FAILURE);
  }

  int cnt = 0;
  while (getline(&line, &len, fp) != -1) {
    cnt++;
  }

  free(line);

  return cnt;
}

// In dieser Funktion werden die Wertepaare eingelesen und 
// in Form von Arrays x[N] und y[N] übergeben.
void readFile(char *name, double x[], double y[]) {
  FILE   *fp;
  char   *line = NULL;
  size_t  len = 0;

  if ((fp = fopen(name, "r")) == NULL) {
    exit(EXIT_FAILURE);
  }

  int cnt = 0;
  while (getline(&line, &len, fp) != -1) {
    sscanf(line, "%lf %lf", &x[cnt], &y[cnt]);
    cnt++;
  }

  free(line);
}

double dividedDifference(double xHigh, double xLow, double yHigh, double yLow) {

  return (yHigh - yLow) / (xHigh - xLow);
}

int main(int argc,char* argv[]) {
  // Abzählen der Wertepaare.
  int N = getNumberOfPoints("input.dat");

  double x[N]; // Vektor für die Abstände der Messungen
  double y[N]; // Vektor für die gemessenen Werte

  // Einlesen der Daten.
  readFile("input.dat", x, y);

  // Laufvariablen
  int i, j;
  double D[N][N]; // für eine Matrix der Dimension NxN

  // Berechnen Sie die dividierten Differenzen:
  // [0][0] = a0
  // [0][1]     [1][1] = a1
  // [0][2]     [1][2] [2][2] = a2
  // [0][3]     [1][3] [2][3] [3][3] = a3
  // [0][4]     [1][4] [2][4] [3][4] [4][4] = a4
  // Im ersten Schritt werden die bekannten y-Werte in die erste Spalte der Matrix geschrieben.
  for(int i = 0; i < N; i++) {
    D[0][i] = y[i];
  }
  // Im zweiten Schritt werden die Berechnungen des Newton-Algorithmus durchgeführt.
  for(int i = 1; i < N; i++) {
    for(int j = i; j < N; j++) {
      D[i][j] = dividedDifference(x[j], x[j - i], D[i - 1][j], D[i - 1][j - 1]);
    }
  }

  // Geben Sie hier die dividierten Differenzen in Form einer Matrix aus:
  printf("\n  Ausgabe der dividierten Differenzen D: \n");
  /*********************************************************************/
  /* Zu Erinnerung:                                                    */
  /* %3d     - Ganzzahl mit 3 Stellen                                  */
  /* %.14lf  - Fließkommazahl mit 14 Nachkommastellen                  */
  /* \n      - Neue Zeile                                              */
  /* Beispiel: printf("%3d: % .2lf % .2lf\n", Variablen);              */
  /*          000: -0.01  0.01                                         */
  /*********************************************************************/
  for(int i = 0; i < N; i++) {
    for(int j = 0; j <= i; j++) {
      double value = D[j][i];
      if(value > 0) printf(" ");
      printf("%.5f\t", value);
    }
    printf("\n");
  }

  // Weisen Sie dem Vektor a die Polynomkoeffizienten aus den entsprechenden Einträgen der Matrix D zu:
  double a[N]; // Vektor der Länge N
    for(int i = 0; i < N; i++) {
    a[i] = D[i][i];
  }

  // Geben Sie das Polynom aus:
  printf("\n  Geben Sie das Polynom aus:\n");
  printf("Ursprung:\n");
  printf("f(x) = ");
  for(int i = 0; i < N; i++) {
    printf("a_%i", i);
    if(i != 0) printf("*");
    for(int j = 0; j < i; j++) {
      printf("(x-x%i)", j);
      if(j != i - 1) printf("*");
    }
    if(i != N - 1) printf("+");
  }
  printf("\nAusgefüllt:\n");
  printf("f(x) = ");
  for(int i = 0; i < N; i++) {
    printf("%.6f", a[i]);
    if(i != 0) printf("*");
    for(int j = 0; j < i; j++) {
      printf("(x-%.5f)", x[j]);
      if(j != i - 1) printf("*");
    }
    if(i != N - 1) printf("+");
  }
  printf("\n");

  int useplotter = 1;

  if (useplotter) {
    // Plotten des Polynoms
    FILE *gp = popen("gnuplot -p", "w");
    fprintf(gp, "set key right bottom box; set xrange [0:5]; set yrange [-9:3.5]; set xlabel \"x\"; set ylabel \"y\";\n");
    fprintf(gp, "f(x) = ");
    for (i = 0; i < N; i++) {
      fprintf(gp, "+%lf", a[i]);
      for (j = 0; j < i; j++) {
        fprintf(gp, "*(x-%lf)", x[j]);
      }
    }
    fprintf(gp, ";\n");
    fprintf(gp, "plot f(x) lc 3 ti \"Interpolation\", \"input.dat\" lc 4 ps 3 lw 2 ti \"data\";\n");

    pclose(gp);
  }

  return 0;
}
