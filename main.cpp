/**
 * critterevolution v1.0.0
 * 2014, Gustavo Di Pietro
 * Licensed under the GPL license (http://www.gnu.org/licenses/gpl-2.0.html)
**/

#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <regex>

#include <SFML/Graphics.hpp>

/**
 * Macro function that randomly returns one of the two parents' member.
 * Used in Critter ctor
 */
#define EITHER_PARENT(member) rnd_uniform_1(rng) ? parent1.member : parent2.member

///////////////
// CONSTANTS //
///////////////
const int                 NUM_MOVEMENTS     {3};                     // Number of different movement steps that compose a Critter movement
const sf::Vector2<float>  END_MOVEMENT      {9999.9999, -9999.9999}; // Signal for movement end
const int                 FPS               {60};                    // Fixed frames per second
const int                 DEF_AGE_LIMIT     {2000};                  // Number of cycles that any Critter will live before dying
const int                 DEF_MATE_HEALTH   {300};                   // Minimum health to be allowed to mate
const int                 WIDTH             {800};                   // Window Width
const int                 HEIGHT            {600};                   // Window Height
const int                 DEF_FOODS         {200};                   // Number of foods avaialble to be eaten
const int                 DEF_FOOD_POWER    {300};                   // How much energy each food provides
const int                 DEF_NUM_CRITTERS  {20};                    // Initial number of critters
const std::string         CONFIG_FILE       {"conf"};                // Configuration file name

////////////////////////////
// CONFIGURABLE CONSTANTS //
////////////////////////////
int AGE_LIMIT    {DEF_AGE_LIMIT};
int MATE_HEALTH  {DEF_MATE_HEALTH};
int FOODS        {DEF_FOODS};
int FOOD_POWER   {DEF_FOOD_POWER};
int NUM_CRITTERS {DEF_NUM_CRITTERS};

///////////
// UTILS //
///////////

/**
 * Generic logging function implemented as recursive template function
 */
void LOG() {
    std::cout << std::endl;
}
template<class T, class... OtherArgs>
void LOG(T&& var, OtherArgs&&... args) {
    std::cout << std::forward<T>(var);
    LOG(std::forward<OtherArgs>(args)...);
}

////////////
// CONFIG //
////////////
/**
 * Reads AGE_LIMIT, MATE_HEALTH, FOODS, FOOD_POWER, NUM_CRITTERS values from
 * a file. If a value is not specified, or the file is not found, use the
 * defaults.
 *
 * Config file format:
 * age = NUM
 * health = NUM
 * foods = NUM
 * foodpower = NUM
 * critters = NUM
 */
void load_config() {
    std::ifstream config_file {CONFIG_FILE};
    if (config_file) {
        LOG("Loading from file");

        std::smatch match;
        std::regex rgx_age("age\\s*\\=\\s*(\\d+)");
        std::regex rgx_health("health\\s*\\=\\s*(\\d+)");
        std::regex rgx_foods("foods\\s*\\=\\s*(\\d+)");
        std::regex rgx_food_pwr("foodpower\\s*\\=\\s*(\\d+)");
        std::regex rgx_critters("critters\\s*\\=\\s*(\\d+)");

        std::string line;
        while (std::getline(config_file, line)) {
            if (std::regex_search(line, match, rgx_age)) {
                AGE_LIMIT = std::stoi(match[1]);
            }
            if (std::regex_search(line, match, rgx_health)) {
                MATE_HEALTH = std::stoi(match[1]);
            }
            if (std::regex_search(line, match, rgx_foods)) {
                FOODS = std::stoi(match[1]);
            }
            if (std::regex_search(line, match, rgx_food_pwr)) {
                FOOD_POWER = std::stoi(match[1]);
            }
            if (std::regex_search(line, match, rgx_critters)) {
                NUM_CRITTERS = std::stoi(match[1]);
            }
        }
    }
}

/**
 * At startup, prints the parameters that are going to be used
 */
void print_config() {
    LOG("Using:");
    LOG("   Age limit: ", AGE_LIMIT);
    LOG("   Min health to mate: ", MATE_HEALTH);
    LOG("   Num of foods: ", FOODS);
    LOG("   Food power: ", FOOD_POWER);
    LOG("   Initial num critters: ", NUM_CRITTERS);
}

///////////////////////
// RANDOM GENERATORS //
///////////////////////
std::mt19937 rng;
std::normal_distribution<float>    rnd_normal_02      {0, 0.2};
std::normal_distribution<float>    rnd_normal_1       {};
std::normal_distribution<float>    rnd_normal_3       {0, 3};
std::uniform_int_distribution<int> rnd_uniform_20_100 {20, 200};
std::uniform_int_distribution<int> rnd_uniform_50_255 {50, 255};
std::uniform_int_distribution<int> rnd_uniform_1      {};
std::uniform_int_distribution<int> rnd_uniform_width  {0, WIDTH};
std::uniform_int_distribution<int> rnd_uniform_height {0, HEIGHT};

///////////////////
// MOVEMENT STEP //
///////////////////

/**
 * This struct is the building block of Critter movements. A critter movement
 * is composed of multiple MovementSteps.
 * A MovementStep stores the variables to be fed to a trigonometric function
 * that simulates the Critter movement.
 */
struct MovementStep {
    sf::Vector2<float> speed            {0, 0};  // Steady movement along the axis
    sf::Vector2<float> mov_radius       {0, 0};  // Frequencis for sin and cos
    sf::Vector2<float> mov_tick_speed   {0, 0};  // Steps for sin and cos
    sf::Vector2<float> mov_tick_counter {0, 0};  // Step conter along the axis

    int  length  {0}; // How many cycles the step lasts
    int  counter {0}; // Countdown counter

    /**
     * When constructed, all the parameters are generated randomly
     */
    MovementStep () {
        speed            = {rnd_normal_1(rng),  rnd_normal_1(rng)};
        mov_radius       = {rnd_normal_3(rng),  rnd_normal_3(rng)};
        mov_tick_speed   = {rnd_normal_02(rng), rnd_normal_02(rng)};
        length           = rnd_uniform_20_100(rng);
        mov_tick_counter = {0, 0};
    };

    void start () {
        counter = length;
    }

    void variate() {
        // TODO small variations
    }

    /**
     * Executes a "step" of the movement
     */
    sf::Vector2<float> update_position () {
        if (counter < 0) {
            return END_MOVEMENT;
        }
        counter--;

        mov_tick_counter += mov_tick_speed;

        float movX = speed.x + ((float)cos(mov_tick_counter.x) * mov_radius.x);
        float movY = speed.y + ((float)sin(mov_tick_counter.y) * mov_radius.y);

        return sf::Vector2<float> {movX, movY};
    }

};


//////////
// FOOD //
//////////
/**
 *  Each food to be eaten by the Critters
 */
struct Food {
    /**
     * Randomly placed during construction
     */
    Food () {
        position.x = rnd_uniform_width(rng);
        position.y = rnd_uniform_height(rng);
    }

    sf::Vector2<float> position;
};

/**
 * Food management class
 */
class Foods {
public:
    /**
     * Creates the initial pool of foods
     */
    Foods () {
        foods.resize(FOODS);
        std::generate(begin(foods), end(foods), [](){ return Food{}; });
    };

    /**
     * Draw all the foods in the pool as little green dots
     */
    void draw(sf::RenderWindow& canvas) {
        sf::RectangleShape dot;
        dot.setSize(sf::Vector2f{2, 2});
        dot.setFillColor({0, 255, 0});

        std::for_each(begin(foods), end(foods), [&](const Food& food){
            dot.setPosition(food.position);
            canvas.draw(dot);
        });
    }

    /**
     * Returns true if the passed bouding box (representing a Critter size and
     * position) overlaps a food. Only allows to eat a single food. The eaten
     * food is removed and a new one is randomly placed
     * @param  critterBox
     * @return
     */
    bool try_to_eat(const sf::Rect<float>& critterBox) {
        auto found = std::find_if(begin(foods), end(foods), [&](const Food& food) {
          return critterBox.contains(food.position);
        });

        if (found != foods.end()) {
            foods.erase(found);
            foods.push_back(Food {});
            return true;
        }

        return false;
    }
private:
    std::vector<Food> foods;
};

/////////////
// CRITTER //
/////////////

/**
 * The class representing each creature in the simulation
 */
class Critter {
public:
    /**
     * In the initializer a critter is given a random position and a
     * random sequence of movements is generated
     */
    Critter () {
        initTexture();
        color = {rnd_uniform_50_255(rng), rnd_uniform_50_255(rng), rnd_uniform_50_255(rng)};
        shape.setTexture(Critter::shape_texture);
        shape.setColor(color);

        position.x = rnd_uniform_width(rng);
        position.y = rnd_uniform_height(rng);

        // Movement steps
        movement_steps.resize(NUM_MOVEMENTS);
        std::generate(begin(movement_steps), end(movement_steps), [](){
            return MovementStep{};
        });
        movement_steps[current_step].start();
    }

    /**
     * A Critter can also be constructed by two parent Critters mating.
     * Its properties are either taken from one parent or from the other
     */
    Critter(const Critter& parent1, const Critter& parent2) {
        initTexture();
        shape.setTexture(Critter::shape_texture);

        position = EITHER_PARENT(position);
        movement_steps.resize(NUM_MOVEMENTS);

        movement_steps[0] = EITHER_PARENT(movement_steps[0]);
        movement_steps[1] = EITHER_PARENT(movement_steps[1]);
        movement_steps[2] = EITHER_PARENT(movement_steps[2]);

        color = EITHER_PARENT(color);
        shape.setColor(color);
        movement_steps[current_step].start();
    }

    /**
     * Called at each cycle, this method updates the Critter status. The new
     * position is calculated, the Critter ages, its health is adjusted according
     * to how much it has moved.
     * Also, it eats a food it encounters one and can mate if the appropriate conditions
     * are met.
     * Return true if still alive, false if dead.
     */
    bool update(Foods& foods, std::list<Critter>& critters) {
        // Calculate the new position
        sf::Vector2<float> movement = movement_steps[current_step].update_position();

        // Step to the next movement if the loop for the current one is exhausted
        if (movement == END_MOVEMENT) {
            current_step = (current_step+1 == movement_steps.size()) ? 0 : current_step+1;
            movement_steps[current_step].start();
            movement = movement_steps[current_step].update_position();
        }

        // Age and loose health according to how much it has moved
        age++;
        health -= (abs(movement.x) + abs(movement.y));

        // Die if the age limit is reached
        if (age >= AGE_LIMIT) return false;

        // Eat if a food is encountered
        if (foods.try_to_eat(this->shape.getGlobalBounds())) {
            health += FOOD_POWER;
        }

        // If the mating conditions are met, see if it is touching another matable
        // Critter and eventually spawn a new Critter
        if (canMate()) {
            std::for_each(begin(critters), end(critters), [&](Critter& mateCritter) {
                if (shape.getGlobalBounds().intersects(mateCritter.shape.getGlobalBounds())
                    && mateCritter.canMate()
                    && this != &mateCritter)
                {
                    critters.push_front(Critter {*this, mateCritter});
                    setBred();
                    mateCritter.setBred();
                }
            });
        }

        oldPosition = {position.x, position.y};

        position.x += movement.x;
        position.y += movement.y;

        // Jump to the other side of the window if exiting from the bounds
        if (position.x < 0)   position.x = 800;
        if (position.x > 800) position.x = 0;
        if (position.y < 0)   position.y = 600;
        if (position.y > 600) position.y = 0;

        // Calculate and set the radius for the rotaion according to the angle
        // between the old position and the new one
        rotation = atan2(oldPosition.y-position.y, oldPosition.x-position.x) * (180 / 3.14) - 90;

        return true;
    }

    /**
     * Draw the Critter on the screen
     */
    void draw(sf::RenderWindow& canvas) {
        shape.setPosition(position);
        shape.setRotation(rotation >= 0 ? rotation : rotation + 360);
        canvas.draw(shape);
    }

    /**
     * A Critter can mate if in mating age (25% < age > 75%), has not already bred
     * and if it health is at least the one defined as minimum limit
     * @return [description]
     */
    bool canMate() {
        return (age >= AGE_LIMIT/4 && age <= (AGE_LIMIT/4) * 3)
            && health >= MATE_HEALTH
            && !bred;
    }

    void setBred() {
        bred = true;
    }

private:
    sf::Vector2<float>        position;
    std::vector<MovementStep> movement_steps;
    sf::Color                 color;
    int                       current_step   {0};
    double                    rotation       {0};
    double                    health         {0};
    int                       age            {0};
    bool                      bred           {false};

    static sf::Texture shape_texture;
    static bool        texture_loaded;

    sf::Vector2<float> oldPosition;
    sf::Sprite                shape;

    /**
     * Create the image used to display the Critters. This is single Texture
     * instance shared among all the Critters. It is created only once.
     */
    void initTexture() {
        if (!Critter::texture_loaded) {
            sf::CircleShape circle{5};
            circle.setFillColor({255, 255, 255});
            circle.setPosition(0, 0);

            sf::ConvexShape triangle {3};
            triangle.setFillColor({255, 255, 255});
            triangle.setPoint(0, {0, 5});
            triangle.setPoint(1, {5, 17});
            triangle.setPoint(2, {10, 5});

            sf::RenderTexture canvas;
            canvas.create(10, 17);
            canvas.clear(sf::Color::Transparent);
            canvas.draw(circle);
            canvas.draw(triangle);
            canvas.display();

            Critter::shape_texture  = canvas.getTexture();
            Critter::texture_loaded = true;
        }
    }
};
bool        Critter::texture_loaded = false;
sf::Texture Critter::shape_texture  = sf::Texture{};

///////////////
// MAIN LOOP //
///////////////
int main()
{
    // Load the configuration and print what parameters are used in the simulation
    load_config();
    print_config();

    // Setup the random generator
    rng.seed(time(nullptr));

    // Create the main window
    sf::RenderWindow App(sf::VideoMode(WIDTH, HEIGHT, 32), "Crittevolution");
    App.setFramerateLimit(FPS);
    App.setVerticalSyncEnabled(true);

    // Generate the initial Critters pool
    std::list<Critter> critters;
    critters.resize(NUM_CRITTERS);
    std::generate(begin(critters), end(critters), []() {
        return Critter{};
    });

    // Create the Food manager
    Foods foods;

    // Start the main loop
    while (App.isOpen())
    {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event))
        {
            // Close window or Esc key: exit
            if (Event.type == sf::Event::Closed || (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape)) {
                App.close();
            }
        }

        // Clear screen, and fill it with black
        App.clear({0, 0, 0});

        // Draw the food dots
        foods.draw(App);

        // Loop the Critters and update the status for each. Remove dead Critters
        for (auto critter = begin(critters); critter != end(critters);) {
            bool alive = critter->update(foods, critters);
            if (alive) {
                critter->draw(App);
                 ++critter;
            }
            else {
                critters.erase(critter++);
            }
        }

        // Display the content of the window on screen
        App.display();
    }

    return 0;
}
