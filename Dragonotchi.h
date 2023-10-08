bool dragonExists = false;
unsigned int boredomLevel = 5;
unsigned int cleanlinessLevel = 5;
unsigned int hungerLevel = 5;
unsigned int health = 5;
bool threwFood = false;
bool threwBall = false;
bool usedMop = false;
int age = 0;

const int MAX_HEALTH = 5;
const int MAX_BOREDOM = 5;
const int MAX_HUNGER = 5;
const int MAX_CLEANLINESS = 5;

const int dropAmount = (MAX_CLEANLINESS + MAX_HUNGER + MAX_BOREDOM) / MAX_HEALTH;
