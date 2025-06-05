#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Création de la fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello SFML");
    window.setFramerateLimit(60);

    std::cout << "Fenêtre lancée" << std::endl;

    // Boucle principale
    while (window.isOpen()) {
        // Gestion des événements
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                std::cout << "Fermeture de la fenêtre" << std::endl;
                window.close();
            }
        }

        // Effacement de l'écran
        window.clear(sf::Color::Black);

        // Ici, vous pouvez ajouter le code de dessin
        // Par exemple, dessiner une forme ou un sprite

        // Affichage
        window.display();
    }

    std::cout << "Programme terminé" << std::endl;
    return 0;
}
