// wit == true zwart == false
// bij verandering van meting rijd door en meet nogmaals
// algorithm priority: right > straight > left > turn around

// prev straight, cur straight = go straight (rechte weg)
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
    // false means road, true means wall
    if (!prevLine[2]) {  // last measurement had road in the center (2)
        // prev straight, cur straight
        if (prevLine[0] && prevLine[1] && prevLine[3] && prevLine[4] && curLine[0] && curLine[1] && !curLine[2] && curLine[3] && curLine[4]) {
            action = moveStraight;
        } else if (prevLine[0] && prevLine[1] && prevLine[3] && prevLine[4] && curLine[0] && curLine[1] && curLine[2] && curLine[3] && curLine[4]) {
            // prev straight, cur none
            action = turnAround;
        } else if (!prevLine[0] && !prevLine[1] && !prevLine[3] && !prevLine[4] && curLine[0] && curLine[1] && !curLine[2] && curLine[3] && curLine[4]) {
            // prev all, cur straight
            action = moveRight;
        } else if (!prevLine[0] && !prevLine[1] && !prevLine[3] && !prevLine[4] && curLine[0] && curLine[1] && curLine[2] && curLine[3] && curLine[4]) {
            // prev all, cur none
            action = moveRight;
        } else if (!prevLine[0] && !prevLine[1] && !prevLine[3] && !prevLine[4] && !curLine[0] && !curLine[1] && !curLine[2] && !curLine[3] && !curLine[4]) {
            // prev all, cur all
            action = finish;
        } else if (!prevLine[0] && !prevLine[1] && prevLine[3] && prevLine[4] && curLine[0] && curLine[1] && !curLine[2] && curLine[3] && curLine[4]) {
            // prev left, cur straight
            action = moveStraight;
        } else if (prevLine[0] && prevLine[1] && !prevLine[3] && !prevLine[4] && curLine[0] && curLine[1] && !curLine[2] && curLine[3] && curLine[4]) {
            // prev right, cur straight
            action = moveRight;
        } else if (!prevLine[0] && !prevLine[1] && prevLine[3] && prevLine[4] && curLine[0] && curLine[1] && curLine[2] && curLine[3] && curLine[4]) {
            // prev left, cur none
            action = moveLeft;
        } else if (prevLine[0] && prevLine[1] && !prevLine[3] && !prevLine[4] && curLine[0] && curLine[1] && curLine[2] && curLine[3] && curLine[4]) {
            // prev right, cur none
            action = moveRight;
        } else if (prevLine[0] && prevLine[1] && prevLine[3] && prevLine[4] && curLine[0] && curLine[1] && curLine[2] && curLine[3] && curLine[4]) {
            // prev none, cur none
            action = error;
        } else if (prevLine[0] && prevLine[1] && prevLine[3] && prevLine[4] && curLine[0] && curLine[1] && !curLine[2] && curLine[3] && curLine[4]) {
            // prev none, cur straight
            action = moveStraight;
        } else {
            action = error;
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