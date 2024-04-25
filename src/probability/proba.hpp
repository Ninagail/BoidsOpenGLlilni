#include <chrono>
#include <glm/glm.hpp>
#include <thread>
#include <vector>

#ifndef PROBABILITY_H
#define PROBABILITY_H

double RAND();

class MarkovChain {
private:
    std::vector<std::vector<double>> transitionMatrix;
    std::vector<int>                 states;

public:
    MarkovChain(std::vector<std::vector<double>> matrix, std::vector<int> states);
    int nextState(int currentState);
};

enum class PirateState { Present,
                         Absent };

enum class BoatState { Moving,
                       Stationary };

glm::vec3 generateLaplaceColor(double beta);
bool      laplace_color_probability(double mu, double b);
int       poisson(double lambda);
bool      storm_occurs(double lambda, double random_value);
void      raise_ocean_level();
double    generate_island_size(double alpha, double xmin, double xmax);
int       geometric_distribution(double p);
char      random_letter_uniform();
double    taille_arbre_gamma(double k, double theta, double taille_min, double taille_max);
bool      binomial(double p);

#endif
