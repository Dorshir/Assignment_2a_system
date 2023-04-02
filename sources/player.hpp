

#ifndef CARDWAR_A_MAIN_PLAYER_HPP
#define CARDWAR_A_MAIN_PLAYER_HPP
class Player {



public:

    explicit Player(const char *name="") {

    }
    ~Player()= default;

    int stacksize(){
        return 1;
    }

    int cardesTaken(){
        return 0;
    }

    void drawCard() {

    }

    const char *getName() {
        return "";
    }
};
#endif //CARDWAR_A_MAIN_PLAYER_HPP
