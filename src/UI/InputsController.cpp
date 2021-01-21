#include "UI/InputsController.h"
#include "Config.h"

InputsController::InputsController(HardwareModulesRegistry* hardwareModulesRegistry){
    _leftButton = registerButton(hardwareModulesRegistry, LEFT_BUTTON_PIN);
    _rightButton = registerButton(hardwareModulesRegistry, RIGHT_BUTTON_PIN);
    _upButton = registerButton(hardwareModulesRegistry, UP_BUTTON_PIN);
    _downButton = registerButton(hardwareModulesRegistry, DOWN_BUTTON_PIN);
}

ButtonPressed InputsController::updateInputs(){
    _leftButton->update();
    _rightButton->update();
    _upButton->update();
    _downButton->update();

    bool leftButtonPressed = _leftButton->pressed();
    bool rightButtonPressed = _rightButton->pressed();   
    bool upButtonPressed = _upButton->pressed(); 
    bool downButtonPressed = _downButton->pressed();    

    if (leftButtonPressed && rightButtonPressed) return ButtonPressed::LEFTRIGHT;
    else if (leftButtonPressed) return ButtonPressed::LEFT;
    else if (rightButtonPressed) return ButtonPressed::RIGHT;
    else if (upButtonPressed) return ButtonPressed::UP;
    else if (downButtonPressed) return ButtonPressed::DOWN;

    return ButtonPressed::NONE;
}

Button* InputsController::registerButton(HardwareModulesRegistry* hardwareModulesRegistry, uint8_t buttonPin){
    Button* button = new Button(hardwareModulesRegistry->mcpExtender); 
    button->attach( buttonPin , INPUT_PULLUP );
    button->interval(5);
    button->setPressedState(LOW);

    return button;
}