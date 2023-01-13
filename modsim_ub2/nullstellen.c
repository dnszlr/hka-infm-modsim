#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

double f1(double x) {
//#  return /* Hier f_1(x) */;
  return ( (x*x*x) - (2 * x) + 2 );
}

double df1(double x) {
  return ( (3 * (x*x)) - 2 );
} 

double f2(double x) {
} 

double df2(double x) { 
}
 
 
double Bisektion(double (*f)(double), double xa, double xb) {
  double m;
  double xc;
  double fxa;
  double fxb;
  double fxc;
  int count = 0;
  //Kopfzeile für die Ausgabe
  printf("  n:  l                 r                 f(l)          f(r)\n"); 
  while (fabs(xa - xb) >= 0.0000000000001 ){ // two times fabs cause the - result might be negative
    xc = 0.5 * (xa + xb);
    fxa = f(xa);
    fxb = f(xb);
    fxc = f(xc);
    printf("  %d:       %.14lf            %.14lf                %.14lf          %.14lf \n", count, xa, xb, fxa, fxb); 
    if((fxc * fxb) <= 0 ){
      xa = xc;
    } else {
      xb = xc;
    }
    count = count + 1;
  }
  return xc;
  //return m; //Rückgabe der gefundenen Nullstelle
}

double Sekanten(double (*f)(double), double x0, double x1) {
  double xnm1 = x0;
  double xn = x1;
  double xnp1;
  int count = 0;
  //Kopfzeile für die Ausgabe
  printf("  n:                   xn                 xn-1                xn+1          \n"); 
  while( 1 ){
    printf("  %d:       %.14lf            %.14lf                %.14lf         \n", count, xn, xnm1, xnp1);
    xnp1 = (xnm1 - (f(xnm1) * ((xn - xnm1)/(f(xn) - f(xnm1)))));
    if(fabs(xnp1 - xn) <= 0.0000000000001){
      break;
    }
    xnm1 = xn;
    xn = xnp1;
    count = count + 1; 
  }
  return xnp1;
}

double Newton(double (*f)(double), double (*dfdx)(double), double x0) {
  double xn = x0;
  double xnp1 = x0;
  double tmp;
  int count = 0;
  //Kopfzeile für die Ausgabe
  printf("  n:                   xn                        xn+1          \n"); 
  while ( 1 ) { // max 100 calculation
    printf("  %d:       %.14lf            %.14lf                      \n", count, xn, xnp1);
    xn = xnp1;
    xnp1 = xn - ((f(xn)) / (dfdx(xn)));
    if(count > 100){
      return NAN;
    } else if (fabs(xnp1 - xn) <= 0.0000000000001) {
      break;
    }
    count = count +1;
  }
  return xnp1;
  
}

int main() {
  double result;

  printf("\n\nBisektion\n–––––––––\n");
  result = Bisektion(f1, -3.0, -0.5);
  printf("Nullstelle Bisektion: %.14lf\n", result);

  printf("\n\nSekanten\n––––––––\n");
  result = Sekanten(f1, -3.0, -0.5);
  printf("Nullstelle Sekanten: %.14lf\n", result);

  printf("\n\nNewton\n––––––\n");
  // -0.5 Ableitung konvegiert nicht
  // result = Newton(f1, df1, -0.5);
  // -1.5 Ableitung konvegiert
  result = Newton(f1, df1, -1.5);
  printf("Nullstelle: %.14lf\n", result);

  return 0;
}
