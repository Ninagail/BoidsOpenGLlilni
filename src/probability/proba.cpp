// #include "proba.hpp"
// #include <cmath>
// #include <cstdlib>
// #include "elementdecor.hpp"

// // Note à moi meme : verifier si tout les points du sujet peuvent etre verifies avec ces lois là

// // Fonction de génération de nombres aléatoires dans l'intervalle [0, 1]
// double RAND()
// {
//     return ((double)rand() / (double)RAND_MAX);
// }

// // Fonction de densité de probabilité de la LOI DE LAPLACE (Probability Density Function) --> pas utile ici car on se base sur la fonction de répartition inversée et non directement sur Laplace
// /*double laplacePDF(double x, double mu, double b)
// {
//     return (1.0 / (2.0 * b)) * exp(-std::abs(x - mu) / b);
// }*/

// // Fonction pour simuler les variations de couleur des coccinelles en fonction du temps
// // Il faudra verifier si cela suit strictement Laplace ou non
// // mu : moyenne de la loi de Laplace
// // b : paramètre de forme de la loi de Laplace
// // time : temps auquel la couleur est évaluée
// double simulateColorVariation(double mu, double b, double time)
// {
//     // Génération d'un échantillon de la loi de Laplace
//     double laplaceSample = mu - b * log(1.0 - RAND());

//     // Ajustement de l'échantillon en fonction du temps (peut être modifié en fonction de votre scénario)
//     return laplaceSample * sin(time); // Exemple simple de variation de couleur en fonction du temps
// }

// /******************************************************************************************************************************/

// // LOI DE PARETO : taille des éléments de décors

// // alpha : paramètre de forme de la loi de Pareto
// // x_min : taille minimale des éléments de décors
// double simulateDecorSize(double alpha, double x_min)
// {
//     // Génération d'un échantillon de la loi de Pareto
//     return x_min / pow(RAND(), 1.0 / alpha) - x_min;
// }

// // Idée de comment l'integrer dans le code

// /*// Paramètres de la loi de Pareto pour la taille des éléments de décors
// double alpha = 2.0; // Paramètre de forme de la loi de Pareto
// double x_min = 1.0; // Taille minimale des éléments de décors

// // Simulation de la taille d'un élément de décor
// double tailleElementDecor = simulateDecorSize(alpha, x_min);

// // Création de l'objet décor avec la taille simulée
// Decor monElementDecor;
// monElementDecor.taille = tailleElementDecor;
// */

// /******************************************************************************************************************************/

// // LOI DE CAUCHY : vent / leger décalement d'un element de décor

// // Fonction pour générer des valeurs selon la loi de Cauchy
// double cauchy_distribution(double location, double scale)
// {
//     double u1 = RAND() * 2.0 - 1.0; // Génère un nombre aléatoire entre -1 et 1
//     double u2 = RAND() * 2.0 - 1.0; // Génère un autre nombre aléatoire entre -1 et 1

//     // Utilise la méthode de Box-Muller pour générer une distribution normale
//     double z = sqrt(-2.0 * log(u1)) * cos(2.0 * 3.1415 * u2);

//     // Applique la localisation et l'échelle pour obtenir la distribution de Cauchy
//     return location + scale * z;
// }

// // Fonction pour appliquer les rafales de vent à un élément de décor
// void appliquer_rafale_vent(ElementDecor& element_decor, double location, double scale)
// {
//     // Générer des déplacements aléatoires pour l'élément de décor
//     double deplacement_x = cauchy_distribution(location, scale);
//     double deplacement_y = cauchy_distribution(location, scale);
//     double deplacement_z = cauchy_distribution(location, scale);

//     // Appliquer les déplacements à l'élément de décor
//     element_decor.move(deplacement_x, deplacement_y, deplacement_z);
// }

// /******************************************************************************************************************************/

// // LOI GEOMETIQUE : apparition de nouveaux éléments de décor

// // Faudra créer une classe element de décor qui contient ces fonctions
// // Pour cette loi je ne suis pas encore sur de comment l'appliquer dans notre code sans que cela fausse la loi de proba géométrique

// // Fonction pour générer des valeurs selon la loi géométrique
// int geometric_distribution(double p)
// {
//     double u = RAND(); // Génère un nombre aléatoire entre 0 et 1
//     return ceil(log(1.0 - u) / log(1.0 - p));
// }

// // Fonction pour déterminer si un nouvel élément de décor apparaît selon la loi géométrique
// bool nouvel_element_decor_apparait(double p)
// {
//     int n = geometric_distribution(p);
//     return (n == 1); // Retourne vrai si n est égal à 1 (nouvel élément de décor apparaît)
// }

// /******************************************************************************************************************************/

// // LOI DE POISSON : probabilite de disparition des éléments de décors
// // Pour cette loi je ne suis pas encore sur de comment l'appliquer dans notre code sans que cela fausse la loi de poisson

// // Fonction pour générer des valeurs selon la loi de Poisson
// int poisson_distribution(double lambda)
// {
//     double L = exp(-lambda);
//     double p = 1.0;
//     int    k = 0;

//     do
//     {
//         k++;
//         p *= RAND(); // Utilisez votre fonction de génération de nombres aléatoires
//     } while (p > L);

//     return k - 1;
// }

// // Fonction pour déterminer si un élément de décor doit disparaître selon la loi de Poisson
// bool element_decor_disparait(double lambda)
// {
//     int n = poisson_distribution(lambda);
//     return (n > 0); // Retourne vrai si n est supérieur à 0 (élément de décor disparaît)
// }

// /******************************************************************************************************************************/

// // LOI BINOMIALE : probabilité qu'une tempete survienne (et donc fasse tomber des elements de décors)

// // Fonction pour générer une variable aléatoire selon une loi binomiale
// // n : nombre d'essais
// // p : probabilité de succès pour chaque essai
// int binomialDistribution(int n, double p)
// {
//     int x = 0;
//     for (int i = 0; i < n; ++i)
//     {
//         if (RAND() < p)
//         { // Utilisation de la fonction RAND() pour générer un nombre aléatoire entre 0 et 1
//             x++;
//         }
//     }
//     return x;
// }

// // Fonction pour simuler l'impact d'une tempête sur les éléments de décors
// // probaTempete : probabilité qu'une tempête survienne
// // seuil : nombre de succès nécessaires pour déclencher la tempête
// // impact : nombre d'éléments de décors affectés par la tempête
// int simulerTempete(double probaTempete, int seuil, int impact)
// {
//     int essais = 1; // Nombre d'essais pour la simulation de la tempête (1 seul essai pour la probabilité) : je ne sais pas trop si 1 est le bon chiffre
//     int succes = binomialDistribution(essais, probaTempete);
//     if (succes >= seuil)
//     {
//         return impact; // La tempête a un impact sur les éléments de décors (à voir comment coder l'impact)
//     }
//     else
//     {
//         return 0; // Pas de tempête, aucun impact sur les éléments de décors
//     }
// }

// /******************************************************************************************************************************/

// // LOI LOGISTIQUE : mortalité des coccinnelles

// // Fonction pour calculer le taux de mortalité des coccinelles en suivant une loi logistique
// // Paramètres :
// // - tauxMortInitial : Taux initial de mortalité
// // - croissance : Taux de croissance de la mortalité
// // - capacite : Capacité maximale de mortalité
// // - temps : Temps auquel évaluer le taux de mortalité
// // Retourne :
// // - Le taux de mortalité calculé en suivant la loi logistique
// double calculerTauxMortaliteLogistique(double tauxMortInitial, double croissance, double capacite, double temps)
// {
//     // Calcul de l'exponentiel de la croissance multipliée par le temps
//     double exponentiel = exp(croissance * temps);

//     // Calcul du taux de mortalité en utilisant la formule de la loi logistique
//     double tauxMortalite = capacite / (1 + exponentiel);

//     // Retourne le taux de mortalité calculé
//     return tauxMortalite;
// }
