#include "app.h"

#include <cstdio>
#include <cstdlib>

/* --Platform include. */
#include "cmsis_os.h"
#include "digitalinout.h"

namespace Joystick = Platform::BSP;
enum Taste {UP,DOWN,LEFT,RIGHT,CENTER, NOTHING};
enum Test {FAILURE, SUCCESS};

//Vielleicht muss das Argument mit "const" nicht deklariert werden
//ich vermute es wird kein Attribut geaendert, deswegen habe ich das gemacht
Taste IsPressed( const Joystick::DigitalInOut *&Buttons);
//------------------------------------------------------------------------
Test Debounce (const Joystick::DigitalInOut *&Buttons, const Taste &Direction);
void set_counter(const Taste &Direction, int &counter);

void simpleLoop(void const* p) {

    //Die Parameter der Objekte sind zufällig!!!
    Joystick::DigitalInOut Buttons[5]= {    Joystick::DigitalInOut UpButton(3,5),
                                            Joystick::DigitalInOut DownButton(6,7),
                                            Joystick::DigitalInOut LeftButton(5,8),
                                            Joystick::DigitalInOut RightButton(6,9),
                                            Joystick::DigitalInOut CenterButton(3,8)
                                        }
    uint32_t counter = 0;
    Taste Direction = NOTHING;
	/* --Print how to use. */
	printf("Usage of SW2 (Joystick): \r\n");
	printf(" right - increment\r\n");
	printf("  left - decrement\r\n");
	printf("    up - clear\r\n");
	printf("  down - randomize\r\n");
	printf(" press - double\r\n");

	/* --Main loop. */
	while (true) {
    
    Direction = IsPressed(Buttons);

        if(Direction != NOTHING)
        {
            if(Debounce(Buttons,Direction) == SUCCESS)
                set_counter(Direction, counter);
        }
		/* --Wait a moment... */
		osDelay(200);
	}
}

Taste IsPressed(const Joystick::DigitalInOut *&Buttons)
{
    int cnt;
    for(cnt = 0; cnt < 4; cnt++)
    {
        if(Buttons[cnt].get())
        {
            switch (cnt)
            {
            case 0:
                return UP;
                break;
            case 1:
                return DOWN;
                break;
            case 2:
                return LEFT;
                break;
            case 3:
                return RIGHT;
            default:
                break;
            }
        }         
    }
    //war keine Taste auf High, dann müssen wir pruefen ob die Taste von der
    //Mitte gedrueckt ist, cnt ist schon 4

    if(!(Buttons[cnt].get()))
        return CENTER;
    
    //wurde keine Taste gedrueckt, return NOTHING
    return NOTHING;
}

//Es wird 10ms gewartet, wenn danach noch derselben Wert liegt,
//sind wir sicher dass die Taste gedrueckt wurde
Taste Debounce (const Joystick::DigitalInOut *&Buttons, const Taste &Direction)
{
    //100ms warten,damit sich die Spannung stabilisiert.
    //Ist nach dieser Zeit die Taste nicht mehr auf High
    //dann war die Taste nicht gedrueckt
    osDelay(100);

    switch (Direction)
    {
    case UP:
        if(Buttons[0].get() != true)
            return FAILURE;
        break;
    case DOWN:
        if(Buttons[1].get() != true)
            return FAILURE;
        break;
     case LEFT:
            if(Buttons[2].get() != true)
            return FAILURE;
        break;
    case RIGHT:
        if(Buttons[3].get() != true)
            return FAILURE;
        break;
    case CENTER:
        if(Buttons[4].get() != false)
            return FAILURE;
        break;    
    default:
        return FAILURE;
        break;
    }
    return SUCCESS;
}

//Die Implementierung bei srand() (Taste Left) fehlt noch

void set_counter(const Taste &Direction, int &counter)
{
     switch (Direction)
    {
    case UP:
        counter = 0;
        printf("%u\n",counter);
        break;
    case DOWN:
        printf("%u\n",counter); //Die srand() Implementierung kommt hier noch
        break;
     case LEFT:
        printf("%u\n",--counter);
        break;
    case RIGHT:
        printf("%u\n",++counter);
        break;
    case CENTER:
        printf("%u\n",counter*2);
        break;    
    default:
        break;
    }
}