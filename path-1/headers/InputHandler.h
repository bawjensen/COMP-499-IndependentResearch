class InputHandler {
private:
public:
    static enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    InputHandler();

    Direction convert(char input);
};