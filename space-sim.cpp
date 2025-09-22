#include <SFML/Graphics.hpp>
#include <list>
#include <vector>
//#include <iostream>
#include <cmath>
#include <random>


const int WIDTH = 1920;
const int HEIGHT = 1080;
const int PARTICLES = 10000;
int MAX_FRAMES = 60;
float zoomOutFactor = 1.2f;
//int frame = 0;




class GravitySource {

    float radius;
    float strength;
    float mass;
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::CircleShape obj;

    public:
        GravitySource(float _radius, float _strength, float _mass, sf::Vector2f _pos) : radius(_radius), strength(_strength), mass(_mass), pos(_pos) {
            obj.setPosition(pos);
            obj.setRadius(radius);
            obj.setFillColor(sf::Color::Blue);
        }

        sf::Vector2f getPos() {
            return pos;
        };
        sf::Vector2f getVelocity() {
            return velocity;
        };
        void setPos(sf::Vector2f position) {
            pos = position;
        }
        void translatePos(sf::Vector2f position) {
            pos += position;
        }
        float getStrength() {
            return strength;
        };
        float getRadius() {
            return radius;
        };

        void render(sf::RenderWindow& window) {
            window.draw(obj);
        };
        void move() {
            obj.setPosition(pos);
        };

        
};

class Debris {

    float mass;
    float radius;
    sf::Vector2f pos;
    sf::Vector2f velocity;
    sf::CircleShape obj;
    bool hitSource;

    public:
        Debris(float _mass, float _radius, sf::Vector2f _pos, sf::Vector2f _velocity) : mass(_mass), radius(_radius), pos(_pos), velocity(_velocity) {
            obj.setFillColor(sf::Color::White);
            obj.setRadius(radius);
            obj.setPosition(pos);
            hitSource = false;
            float distance = 0;
        };

        sf::Vector2f getPos() {
            return pos;
        };
        float getMass() {
            return mass;
        };
        float getRadius() {
            return radius;
        };
        sf::Vector2f getVelocity() {
            return velocity;
        };
        void setPosition(sf::Vector2f position) {
            pos = position;
        };
        void translatePosition(sf::Vector2f position) {
            pos += position;
        };
        void setVelocity(sf::Vector2f vel) {
            velocity = vel;
        };
        void render(sf::RenderWindow& window) {
            window.draw(obj);
        };
        //For calculating vector for gravitationell pull
        void updateVelocity(sf::Vector2f source_pos, float strength, float source_radius) {
            

            if (!hitSource) {
                source_pos.x = source_pos.x + source_radius;
                source_pos.y = source_pos.y + source_radius;
                //Gets the distance to the source as a float

                float accelScriptXPos = source_pos.x - pos.x;
                float accelScriptYPos = source_pos.y - pos.y;


                float distance = sqrt(pow(accelScriptXPos, 2) + pow(accelScriptYPos, 2));
                float inverse_distance = 1.0f / distance;
                float inverse_square = inverse_distance * inverse_distance;
                sf::Vector2f gravAccelNorm((accelScriptXPos)/distance, (accelScriptYPos)/distance);
                sf::Vector2f gravAccel(gravAccelNorm.x*strength*inverse_square, gravAccelNorm.y*strength*inverse_square);

                if (distance <= source_radius + radius) {
                    //if (distance > source_radius*0.1f) {
                    //    translatePosition(sf::Vector2f(gravAccelNorm));
                    //} else {
                    //    setPosition(sf::Vector2f(source_pos.x - radius/0.75f, source_pos.y - radius/0.75f));
                    //}
                    hitSource = true;
                    velocity = sf::Vector2f(0, 0);
                    obj.setRadius(0);
                } else {
                    velocity += gravAccel;
                }
            }
            
            

        }
        void move() {
            pos += velocity;
            obj.setPosition(pos);
        }
};
   

int main() {


    //Main Windows settings
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Titel", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(MAX_FRAMES);
    sf::Event ev;
    sf::ContextSettings settings;
    //settings.antialiasingLevel = 2;
    sf::View view(sf::FloatRect(0, 0, WIDTH, HEIGHT));
    view.zoom(zoomOutFactor);
    window.setView(view);



    //Space for creating shapes:
    //GravitySource source(40.0, 10000.0, 10.0, sf::Vector2f(WIDTH/2, HEIGHT/2));
    //Debris debris1(1.0, 10.0, sf::Vector2f(500, HEIGHT/2), sf::Vector2f(0, 3));
    
    

    //Loops:
    std::vector<Debris> particles;
    std::vector<GravitySource> sources;

    for (int i = 0; i <= PARTICLES/2; i++) {
        particles.emplace_back(Debris(1.0, 5.0, sf::Vector2f(WIDTH/5 + 20, HEIGHT/2), sf::Vector2f(2+(float(rand() % 200)-99)/100, 3+(float(rand() % 200)-99)/100)));
    };
    for (int i = 0; i <= PARTICLES/2; i++) {
        particles.emplace_back(Debris(1.0, 5.0, sf::Vector2f(WIDTH/1.2f - 20, HEIGHT/2), sf::Vector2f(-2-(float(rand() % 200)-99)/100, -3-(float(rand() % 200)-99)/100)));
    };
    
    
    sources.emplace_back(GravitySource(100.0, 10000.0, 10.0, sf::Vector2f(WIDTH/2, HEIGHT/2)));
    sources.emplace_back(GravitySource(40.0, 1000.0, 10.0, sf::Vector2f(WIDTH/2, HEIGHT/3)));
    sources.emplace_back(GravitySource(25.0, 700.0, 10.0, sf::Vector2f(500, 300)));
    




    //Main Loop/Game Loop
    while (window.isOpen()) {
        //Window Management:
        while (window.pollEvent(ev))
        {
           switch(ev.type) 
           {
            case sf::Event::Closed:
                window.close();
                break;
           }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        //------------------------------------------------


        

        for (int i = 0; i <= particles.size()-1; i++) {
            for (int ii = 0; ii <= sources.size()-1; ii++) {
                particles[i].updateVelocity(sources[ii].getPos(), sources[ii].getStrength(), sources[ii].getRadius());
            };
            
            particles[i].move();
        }

        //sources[0].setPos(sf::Vector2f(WIDTH/2 + sin(float(frame)/50)*100, HEIGHT/2));
        //sources[0].move();




        //Rendering:
        window.clear();
        for (int i = 0; i <= sources.size()-1; i++) {sources[i].render(window);}
        for (int i = 0; i <= particles.size()-1; i++) {particles[i].render(window);}
        window.display();
        
        
        //if (frame % MAX_FRAMES == 0) {
        //    std::cout << frame << ": " << frame/MAX_FRAMES <<"\n";
        //};

        

        
        //frame += 1;

    };

    return 0;
};