#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*********************************************************************/
/*                                                                   */
/* Es liegen Messdaten "input.dat" in Form von Wertepaaren vor.      */
/* Durch diese Punkte muss eine Ausgleichskurve gelegt werden.       */
/*                                                                   */ 
/*********************************************************************/
/*             Zu Beginn einige Hilfsfunktionen                      */
// --------------------------------------------------------------------


/*
------- AT*A ---------
2.500000e+01 3.000000e+02
3.000000e+02 4.900000e+03
------- AT*logI -----
8.282651e+01
1.223997e+03
------- ATAinv ------
1.507692e-01 -9.230769e-03
-9.230769e-03 7.692308e-04
------- lambda -----
ln_a: 1.189257e+00
b:    1.769836e-01
*/

/** @brief Funktion um die Wertepaare abzuzählen.
 */
int getNumberofPoints(char *name) {
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
  fclose(fp);

  return cnt;
}

/** @brief In dieser Funktion werden die Wertepaare eingelesen und in Form von Arrays t[N] und I[N] übergeben.
 */
void readFile(char *name, int t[], double I[]) {
  FILE   *fp;
  char   *line = NULL;
  size_t  len = 0;

  if ((fp = fopen(name, "r")) == NULL) {
    exit(EXIT_FAILURE);
  }

  int cnt = 0;
  while (getline(&line, &len, fp) != -1) {
    sscanf(line, "%i %lf", &t[cnt], &I[cnt]);
    cnt++;
  }

  free(line);
  fclose(fp);
}

// --------------------------------------------------------------------
// --------------------------------------------------------------------

int main(int argc, char* argv[]){
  // Abzaehlen der Wertepaare
  int N = getNumberofPoints("input.dat");

  int    t[N]; //Vektor für die Kalenderwochen der Messung
  double I[N]; //Vektor für die gemessenen Inzidenzwerte
  // Einlesen der Daten
  readFile("input.dat", t, I);

  // Hilsvariabelen fuer t_0
  int t_0 = t[0];

  double logI[N];
  for(int index = 0; index < N; index++) {
    logI[index] = log(I[index]);
  }

  // Berechnung der Matrix A^T*A und der rechten Seiten A^T*y
  // ATA[0][0] = N -> 1 * 1 + 1 * 1...
  // ATA[0][1] & [1][0] = sum(t) -> 1 * t[0] + 1 * t[1] + ..
  // ATA[1][1] = t[1..n] * t[1..n]
  // Cw = Abkürzung für Kalenderwochen
  double sumCw = 0.0L;
  double prodCw = 0.0L;
  double sumIndices = 0.0L;
  double prodCwAndIndices = 0.0L;
  for(int i = 0; i < N; i++) {
    double iteration = t[i] - t_0;
    sumCw += iteration;
    // printf("sumCw: %12.6e\n", sumCw);
    prodCw += (iteration * iteration);
    // printf("prodCw: %12.6e\n", prodCw);
    sumIndices += logI[i];
    // printf("sumIndices: %12.6e\n", sumIndices);
    prodCwAndIndices += (iteration * logI[i]);
    // printf("prodCwAndIndices: %12.6e\n", prodCwAndIndices);
  }
  double ATA[2][2] = {{N, sumCw},
                      {sumCw, prodCw}};;
  double ATlogI[2] = {sumIndices, prodCwAndIndices};
  
  printf("------- AT*A ---------\n");
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      printf("%12.6e ", ATA[i][j]);
    }
    printf("\n");
  }

  printf("------- AT*logI -----\n");
  for(int i = 0; i < 2; i++) { 
    printf("%12.6e\n", ATlogI[i]);
  }

  // Invertierung von ATA (wegen kleiner Groesse)
  printf("------- ATAinv ------\n");
  double determinant = 1 / ((ATA[0][0] * ATA[1][1]) - (ATA[0][1] * ATA[1][0]));
  double ATAinv[2][2] = {{ATA[1][1], -1 * ATA[0][1]},
                        {-1 * ATA[1][0], ATA[0][0]}};
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      ATAinv[i][j] *= determinant;
      printf("%12.6e ", ATAinv[i][j]);
    }
    printf("\n");
  }

  // Ermittlung von ln(a) und b aus ATAinv und ATy
  double ln_a = ATAinv[0][0] * ATlogI[0] + ATAinv[0][1] * ATlogI[1];
  double b = ATAinv[1][0] * ATlogI[0] + ATAinv[1][1] * ATlogI[1];

  printf("------- lambda -----\n");
  printf("ln_a: %12.6e\n", ln_a);
  printf("b: %12.6e\n", b);

  double a = exp(ln_a);
  double sum = 0.0L;
  for(int i = 0; i < N; i++) {
    sum += (logI[i] - (ln_a + (t[i] - t_0) * b)) * (logI[i] - (ln_a + (t[i] - t_0) * b));
  }
  printf("Fehlerfunktion: %12.6e\n", sum);
  // Plotten wenn plotflag!=0
  long plotflag = 1;

  if (plotflag) {
    FILE *gp = popen("gnuplot -p","w");
    fprintf(gp,"reset; set key left top box; set xlabel \"t - t_0\";\n"
        "set ylabel \"y\";\n"
        "set autoscale fix\n"
//         "set logscale xy\n"
        " f(t) = %le*exp(%le*(t - 23));\n "
        " plot f(x) lt -1 lw 2, \"input.dat\" using 1:2 pt 7 title 'measured data';\n", //lt: LineType, lw: Linewidth, using 1:2: zweite spalte verwenden
        a, b);
    pclose(gp);
  }
  
  return 0;
}
