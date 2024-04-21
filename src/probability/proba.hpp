#include <glm/glm.hpp>

#ifndef PROBABILITY_H
#define PROBABILITY_H

double    RAND();
glm::vec3 generateLaplaceColor(double beta);
bool      laplace_color_probability(double mu, double b);
double    simulateDecorSize(double alpha, double x_min);
bool      cauchy_move_probability(double location, double scale);
bool      nouvelElementDecorApparait(double p);
int       poisson(double lambda);
bool      storm_occurs(double lambda, double random_value);
void      raise_ocean_level();
bool      elementDecorDisparait(double lambda);
int       binomialDistribution(int n, double p);
int       simulerTempete(double probaTempete, int seuil, int impact);
double    generate_island_size(double alpha, double xmin, double xmax);
int       geometric_distribution(double p);
bool      pirate_presence_uniform();
char      random_letter_uniform();
double    taille_arbre_gamma(double k, double theta, double taille_min, double taille_max);
int       nombre_boids_initial(double L, double q, double x0);
void      raise_ocean_level();
double    generate_laplace_sample(double beta);

#endif
