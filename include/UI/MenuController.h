#include <map>
#include "HardwareModules/HardwareModulesRegistry.h"
#include "Healthchecks/HealthcheckController.h"
#include "InputsController.h"
#include "MenuConfiguration.h"
#include "OLED.h"

class MenuController
{
public:
    MenuController(HardwareModulesRegistry* hardwareModulesRegistry, HealthcheckController* healthcheckController);
    void showMenu(MenuMode menuMode = MAIN_MENU_MODE);
    void buttonPressed(ButtonPressed button);

    bool isMenuActive() { return _menuMode != OFF_MENU_MODE; }

private:
    OLED* _screen;
    volatile MenuMode _menuMode = OFF_MENU_MODE;
    volatile int _menuOptionSelectedIndex = 0;
    volatile int _scrollerPosition = 0;
    MenuConfiguration _menuConfiguration;
    MenuScreen getCurrentScreen() { return _menuConfiguration.getMenuScreen(_menuMode); }
    MenuOption getSelectedOption() { return _menuConfiguration.getMenuScreen(_menuMode).options[_menuOptionSelectedIndex]; }
    int getCurrentScreenOptionsCount() { return _menuConfiguration.getMenuScreen(_menuMode).options.size(); }

    void updateMenu();
    void printMenu();

    void showDevicesInfo();
    void showCurrentState();
    void showErrorsLog();
    void showDebugScreen();

    void selectNextOption();
    void selectPreviousOption();
    void clickOption();
    void returnBack();

    HardwareModulesRegistry* _hardwareModulesRegistry;
    HealthcheckController* _healthcheckController;    
};