#include "proba.hpp"
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>
#include <iostream>

// Fonction de génération de nombres aléatoires dans l'intervalle [0, 1]
double RAND()
{
    return ((double)rand() / (double)RAND_MAX);
}

/******************************************************************************************************************************/

// LOI DE CAUCHY : vent qui provoque un leger décalement de la barque

bool cauchy_move_probability(double location, double scale)
{
    // Générer un nombre aléatoire entre 0 et 1
    double random_value = rand() / double(RAND_MAX);

    // Utiliser la loi de Cauchy pour calculer la probabilité de mouvement
    double cauchy_probability =
        1.0 / (1.0 + pow((random_value - 0.5) / scale, 2));

    // Comparer la probabilité générée avec la probabilité de mouvement
    return random_value < cauchy_probability;
}

/******************************************************************************************************************************/
// LOI DE LAPLACE : changement de couleur du soleil

bool laplace_color_probability(double mu, double b)
{
    // Générer un échantillon aléatoire selon la loi de Laplace
    double beta          = 1.0 / b;                                                    // Calculer le paramètre beta
    double u1            = rand() / (RAND_MAX + 1.0);                                  // Générer un nombre aléatoire uniforme dans [0, 1]
    double laplaceSample = beta * (u1 < 0.5 ? log(2.0 * u1) : -log(2.0 * (1.0 - u1))); // Appliquer la transformation inverse de la distribution de Laplace

    // Déterminer si la sphère doit changer de couleur en fonction de la probabilité
    bool change_color = (laplaceSample < mu);

    // Afficher un message de débogage indiquant la couleur de la sphère
    if (change_color)
    {
        std::cout << "La sphere devient rouge." << std::endl;
    }
    else
    {
        std::cout << "La sphere reste jaune." << std::endl;
    }

    return change_color;
}

/******************************************************************************************************************************/

// LOI DE PARETO : taille des iles

double generate_island_size(double alpha, double xmin, double xmax)
{
    double u = RAND(); // Générer un nombre aléatoire uniforme dans l'intervalle [0, 1]

    // Appliquer la transformation inverse de Pareto
    double island_size = xmin * pow(1.0 / u - 1.0, 1.0 / alpha);

    // Assurer que la taille de l'île est dans la plage [xmin, xmax]
    island_size = std::max(xmin, std::min(xmax, island_size));

    std::cout << "Taille de l'ile : " << island_size << std::endl;

    return island_size;
}

// /******************************************************************************************************************************/

// LOI GEOMETIQUE : apparition de nouvelles îles

int geometric_distribution(double p)
{
    double u          = RAND(); // Génère un nombre aléatoire entre 0 et 1
    int    n          = 0;
    double cumulative = 0.0;
    while (cumulative < 1.0)
    {
        cumulative += p * pow((1.0 - p), n);
        if (u <= cumulative)
        {
            // Retourner le nombre d'îles (au moins une île), limité entre 1 et 5 îles maximum
            return std::min(n + 1, 5);
        }
        n++;
    }
    // Si la boucle se termine sans retourner, cela signifie qu'aucune île n'a été générée
    return 0;
}

/******************************************************************************************************************************/

// LOI DE POISSON : probabilite qu'une têmpete survienne ce qui entraine la montée de l'océan

int poisson(double lambda)
{
    if (lambda == 0.0)
        return 0;

    double L = std::exp(-lambda);
    double p = 1.0;
    int    k = 0;

    while (p >= L)
    {
        p *= RAND();
        k++;
    }

    return k - 1;
}

bool storm_occurs(double lambda, double random_value)
{
    // Générer un nombre aléatoire selon la loi de Poisson avec le paramètre lambda
    int poisson_value = poisson(lambda);

    // Déterminer un seuil en fonction de lambda
    double threshold = lambda * random_value;

    // Si la valeur de la distribution de Poisson dépasse le seuil, une tempête se produit
    return poisson_value > threshold;
}

/******************************************************************************************************************************/
// LOI UNIFORME : bateau pirate présent ou non dans la scene

//  Fonction pour simuler une loi uniforme (50% de chance que le bateau pirate soit là)

char random_letter_uniform()
{
    int         index    = rand() % 26;
    const char* alphabet = "abcdefghijklmnopqrstuvwxyz";
    char        letter   = alphabet[index];

    if (index < 13)
    {
        std::cout << "Bateau pirate : Absent" << std::endl;
        return 'a';
    }
    else
    {
        std::cout << "Bateau pirate : Present" << std::endl;
        return 'p';
    }
}

/******************************************************************************************************************************/
// LOI GAMMA : définie la taille de l'arbre

double taille_arbre_gamma(double k, double theta, double taille_min, double taille_max)
{
    double d, c, x, v, u;

    if (k < 1.0)
    {
        do
        {
            u = RAND();
            v = RAND();
            x = pow(u, 1.0 / k);
            c = theta * (exp(1.0) / k);
            d = v * pow(x, k - 1.0);
        } while (d > exp(-x) || x * theta < taille_min || x * theta > taille_max);
    }
    else
    {
        do
        {
            u = RAND();
            v = RAND();
            x = -log(u);
            v = v * pow(x, k - 1.0);
            d = v - exp(-x);
        } while (d > 0.0 || x * theta < taille_min || x * theta > taille_max);
    }

    std::cout << "Taille arbre : " << theta * x << std::endl;

    return theta * x;
}

/******************************************************************************************************************************/
// LOI LOGISTIQUE :

int nombre_boids_initial(double L, double q, double x0)
{
    double x = RAND(); // Générer un nombre aléatoire entre 0 et 1
    // Calculer la fonction logistique
    double proba_logistique = L / (1 + exp(-q * (x - x0)));
    // Utiliser la probabilité pour déterminer le nombre de boids
    int nombre_boids = (int)(proba_logistique * 100); // Multiplier par 100 pour obtenir un nombre entier
    return nombre_boids;
}
