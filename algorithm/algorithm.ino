// wit == true zwart == false
// bij verandering van meting rijd door en meet nogmaals
// algorithm priority: right > straight > left > turn around

// cur straight, prev straight = go straight (rechte weg)
// prev straight, cur none = turn around (doodlopende weg)
// prev all, cur straight = go right (drie weg kruispunt)
// prev all, cur none = go right (T kruispunt)
// prev all, cur all = finish (eindpunt)
// prev left, cur straight = go straight (kruispunt rechtdoor, linksaf)
// prev right, cur straight = go right (kruispunt rechtdoor, rechtsaf)
// prev left, cur none = go left (bocht linksaf)
// prev right, cur none = go right (bocht rechtsaf)
// prev none, cur none = error
// prev none, cur straight = go straight (beginpunt)

bool curLine[5] = {true, true, false, true, true};
bool prevLine[5] = {true, true, true, true, true};

enum Action {
    moveStraight,
    moveRight,
    moveLeft,
    turnAround,
    finish,
    measureAgain,
    error
};
Action action = moveStraight;

void setup() {
}

void loop() {
    // decide action based on current and previous line
    for (int i = 0; i < 5; i++) {
        prevLine[i] = curLine[i];
    }
    Action action = measureAgain;
    // rijden
    drive();
    // actie bepalen
    solveMaze();
}

void solveMaze() {
    if (!prevLine[2]) {                                                                 // vorige meting had tenminste het midden (anders gaat het fout)
        if (prevLine[0] && prevLine[1] && prevLine[2] && prevLine[3] && prevLine[4]) {  // vorige meting was een rechte weg
            if (curLine[0] && curLine[1] && curLine[2] && curLine[3] && curLine[4]) {   // huidige meting is een rechte weg
                action = moveStraight;
            } else if (!curLine[0] && !curLine[1] && !curLine[2] && !curLine[3] && !curLine[4]) {  // huidige meting is een doodlopende weg
                action = turnAround;
            } else if (!curLine[0] && !curLine[1] && curLine[2] && !curLine[3] && !curLine[4]) {  // huidige meting is een T kruispunt
                action = moveRight;
            } else if (!curLine[0] && !curLine[1] && curLine[2] && curLine[3] && curLine[4]) {  // huidige meting is een drie weg kruispunt
                action = moveRight;
            } else if (!curLine[0] && !curLine[1] && curLine[2] && !curLine[3] && curLine[4]) {  // huidige meting is een kruispunt rechtdoor, linksaf
                action = moveStraight;
            } else if (!curLine[0] && curLine[1] && curLine[2] && !curLine[3] && !curLine[4]) {  // huidige meting is een kruispunt rechtdoor, rechtsaf
                action = moveRight;
            } else if (!curLine[0] && curLine[1] && curLine[2] && curLine[3] && curLine[4]) {  // huidige meting is een eindpunt
                action = finish;
            } else if (curLine[0] && curLine[1] && !curLine[2] && curLine[3] && curLine[4]) {  // huidige meting is een bocht linksaf
                action = moveLeft;
            }
        }
    }
}

void doMoveStraight() {
    // ga rechtdoor
}

void doMoveRight() {
    // ga rechtsaf
}

void doMoveLeft() {
    // ga linksaf
}

void drive() {
}