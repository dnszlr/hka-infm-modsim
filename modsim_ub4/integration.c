#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**
 * @brief fa(x)=sic(x)
 *
 * @param x x
 *
 * @return f(x)
 */

double f1(double x)
{
  double res = 0;
  if (x == 0)
  {
    res = 1;
  }
  else
  {
    res = (sin(x) / x);
  }
  return res;
}

double f2(double x)
{
  return exp(x);
}

/**
 * @brief Integration von f mit Untersummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_unter(double a, double b, long n, double (*f)(double))
{
  double sum = 0;
  double h = abs((b - a) / n); // Schrittweite
  for (int i = 0; i < n; ++i)
  {
    sum += f(a + (i * h)); // a + (i*h) ist das f(x) in abhängigkeit des Intervalls, beginnend ganz links
  }
  return (h * sum); // h* notwendig laut Formel
}

/**
 * @brief Integration von f mit Mittelsummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_mitte(double a, double b, long n, double (*f)(double))
{
  double sum = 0;
  double h = abs((b - a) / n); // Schrittweite
  for (int i = 0; i < n; ++i)
  {
    sum += f(0.5 * ((a + (i * h)) + ((a + ((i + 1) * h))))); // 0.5*((a + (i*h)) + ((a + ((i+1)*h)))) = Mittelwert des Intervalls
  }
  return (h * sum); // notwendig laut Formel
}

/**
 * @brief Integration von f mit Obersummen.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_ober(double a, double b, long n, double (*f)(double))
{
  double sum = 0;
  double h = abs((b - a) / n); // Schrittweite
  for (int i = 0; i < n; ++i)
  {
    sum += f(a + ((i + 1) * h)); // a + ((i+1)*h) f(x9 in abhängigkeit des intervalls, starte direkt beim ersten Intervall nicht beim Nullten)
  }
  return (h * sum);
}

/**
 * @brief Integration von f mit der Trapez-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_trapez(double a, double b, long n, double (*f)(double))
{
  double sum = 0;
  double h = abs((b - a) / n); // Schrittweite
  if (n > 3)
  {
    for (int i = 1; i != n; i++)
    {
      sum += 2 * f(a + (i * h)); // sum up 1 up to n -1, each item twice
    }
    sum += f(a); // dont forget the outer ones
    sum += f(b);
    sum = sum * (h / 2); // dont forget h/2
  }
  else
  {
    sum += f(a); // dont forget the outer ones
    sum += f(b);
    sum = sum * (h / 2); // dont forget h/2
  }

  return sum;
}

/**
 * @brief Integration von f mit der Simpson-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double int_simpson(double a, double b, long n, double (*f)(double))
{
  if ((n % 2) != 0)
  {
    return NAN;
  }
  double h = fabs(b - a) / n; // Schrittweite
  double x = 0;
  double sum = 0;
  for (int i = 1; i < n; i++)
  {
    x = a + i * h; // x in Abhängigkeit des derzeitig betrachteten Intervalls
    if (i % 2 == 0)
    { // Für gerade i's gilt
      sum = sum + 2 * f(x);
    }
    else
    { // Für ungerade i's gilt
      sum = sum + 4 * f(x);
    }
  }
  double result = (h / 3) * (f(a) + f(b) + sum); // Aufsummierung mit den Randwerten und Multiplikation h/3
  return result;
}

/**
 * @brief Integration von f mit der Trapez-Regel.
 *
 * @param a untere Grenze des Integrationsbereiches
 * @param b obere Grenze des Integrationsbereiches
 * @param n Anzahl der Stützstellen
 * @param f zu integrierende Funktion
 */

double bogenlaenge(double a, double b, long n, double (*f)(double))
{
  // double laenge = int_simpson (a, b, n, sqrt(1 + (exp()))); <--- Schoene Formel, wie implementieren
  // Nähere mit Pythagoras Bogenlänge der Teilintervalle an und Summiere das
  double h = abs((b - a) / n); // Schrittweite
  double xN = 0;
  double yN = 0;
  double xO = 0;
  double yO = 0;
  double xDelta = 0;
  double yDelta = 0;
  double sum = 0;
  for (int i = 1; i < n; i++){
    xN = a + (i * h); // xNew
    yN = f(xN);
    xO = a + ((i-1) * h); // xOld
    yO = f(xO);
    xDelta = xN - xO;
    yDelta = yN - yO;
    double tmp = sqrt((xDelta * xDelta) + (yDelta * yDelta));
    sum += sqrt((xDelta * xDelta) + (yDelta * yDelta));
  }

  printf(" %f", sum);
}

/**
 * @brief Sucht die Anzahl der Stützstellen, die benötigt werden um einen Approximationsfehler < err zu haben.
 *
 * @param a        untere Grenze des Integrationsbereiches
 * @param b        obere Grenze des Integrationsbereiches
 * @param f        zu integrierende Funktion
 * @param integral zu verwendendes Integrationsverfahren
 * @param real     analytische Lösung
 * @param err      Fehlertoleranz
 */

void findsteps(double a, double b, double (*f)(double), double integral(double, double, long, double (*)(double)), double real, double err)
{
  int n = 0;        // Default
  double error = 0; // temp Error for calculation
  double tmpIntegriert;
  while (1)
  {
    n = n + 1; // wenn Beginn bei 0 geht das Verfahren nicht, if n = 0 -> return NaN
    tmpIntegriert = integral(a, b, n, f);
    error = abs(tmpIntegriert - real); // abs necessary cause negative values might appear and program stop
    if (error < err)
    {
      break;
    }
  }

  printf("%6d Stuetzstellen, err=%le %f\n", n, error);
}

int main(void)
{
  int n;
  double real;
  double a = 0.0;
  double b = 2.0;
  double (*f)(double);

  double (*fTwo)(double);

  // Sie können f benutzen und an dieser Stelle abändern wenn Sie fb haben wollen
  f = f1;
  fTwo = f2;
  real = 1.6054129768026948485767201; // b=2

  // for f2:
  // f = f2;
  // real = exp(b)- exp(a);

  // b) ------------------------------------------
  // Loop to calculate n = 4, 8, 16, 32, 64, 128

  n = 4;
  for (int i = 0; i < 6; i++)
  {
    printf("\n");
    printf("n: %d", n);
    double unter = int_unter(a, b, n, f);
    double mitte = int_mitte(a, b, n, f);
    double ober = int_ober(a, b, n, f);
    double trapez = int_trapez(a, b, n, f);
    double simpson = int_simpson(a, b, n, f);
    printf("\n");
    printf("Target: %f", real);
    printf("\n");
    printf("Unter: %f", unter);
    printf("\n");
    printf("%6lf (% .4le)\n", unter, real - unter);
    printf("\n");
    printf("Mitte: %f", mitte);
    printf("\n");
    printf("%6lf (% .4le)\n", mitte, real - mitte);
    printf("\n");
    printf("Ober: %f", ober);
    printf("\n");
    printf("%6lf (% .4le)\n", ober, real - ober);
    printf("\n");
    printf("Trapez: %f", trapez);
    printf("\n");
    printf("%6lf (% .4le)\n", trapez, real - trapez);
    printf("\n");
    printf("Simpson: %f", simpson);
    printf("\n");
    printf("%6lf (% .4le)\n", simpson, real - simpson);
    printf("\n");
    n *= 2;
  }
  // #double unter = int_unter(a, b, n, f);
  // #printf("%6lf (% .4le)\n", unten, real - unter);

  double err = 1e-3;

  // c) ------------------------------------------
  printf("Error: %.9f \n", err);
  printf("Untersumme: ");
  findsteps(a, b, f, int_unter, real, err);
  printf("Mittelsumme: ");
  findsteps(a, b, f, int_mitte, real, err);
  printf("Obersumme: ");
  findsteps(a, b, f, int_ober, real, err);
  printf("Trapez: ");
  findsteps(a, b, f, int_trapez, real, err);
  printf("Simpson: ");
  findsteps(a, b, f, int_simpson, real, err);
  printf("\n");

  // d) ------------------------------------------
  for (int i = 0; i < 7; i++)
  {
    printf("Error: %.9f \n", err);
    printf("Mittelsumme: ");
    findsteps(a, b, f, int_mitte, real, err);
    printf("Trapez: ");
    findsteps(a, b, f, int_trapez, real, err);
    printf("Simpson: ");
    findsteps(a, b, f, int_simpson, real, err);
    err *= 0.1;
    printf("\n");
  }

  // Aufgabe 2 -----------------------------------

  n = 5;
  printf("\n");
  printf("Bogenlänge: ");
  bogenlaenge(a, b, n, f2);

  return 0;
}
