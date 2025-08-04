#include "main0.h"

#include "ultrasonic.h"
#include "tof.h"
#include "level.h"
#include "bluetooth.h"
#include "motor.h"

#define MAX_STEERING 200

int logging = 0;
boolean start = FALSE;

void main0 ()
{
    systemInit();
    myPrintf("SYSTEM START\n");
    bluetoothPrintf("SYSTEM START\n");
    float val;
    while (1)
    {
        char command = bluetoothRecvByteNonBlocked();

        if (command != -1)
        {
            bluetoothPrintf("COMMAND: %c\n", command);
        }

        if (command == 'g')
        {
            int speed, round;
            bluetoothPrintf("Speed(0 ~ 1000): ");
            bluetoothScanf("%d", &speed);
            bluetoothPrintf("Round: ");
            bluetoothScanf("%d", &round);

            while (round--)
            {
                motorMoveForward(speed);
                delayMs(25);
                motorStop();
                delayMs(25);
            }
        }
        if (command == 'z')
        {
            auto_park();
        }

        if (command == 'l')
        {
            ledToggle(LED_RIGHT);

        }
        if (command == 'b')
        {
            ledToggle(EMPTY);

        }

        if (command == 'c')
        {
            printState();
            bluetoothPrintf("left dis: %d\n", getDistanceByUltra(ULT_LEFT));
            bluetoothPrintf("rear dis: %d\n", getDistanceByUltra(ULT_REAR));
        }

        if (command == 'p')
        {
            int time;
            int aDuty;
            int bDuty;

            bluetoothPrintf("aDuty, bDuty, Time: ");
            bluetoothScanf("%d %d %d", &aDuty, &bDuty, &time);
            if (aDuty > bDuty)
            {
                motorMovChAPwm(aDuty, 1);
                motorMovChBPwm(bDuty, 1);
            }
            else
            {
                motorMovChAPwm(aDuty, 0);
                motorMovChBPwm(bDuty, 0);
            }
            delayMs(time);
            motorStop();
        }

        if (command == 'y')
        {
            bluetoothPrintf("Set Kp: ");
            bluetoothScanf("%f", &val);
            setGain(0, val);
        }

        if (command == 'u')
        {
            bluetoothPrintf("Set Ki: ");
            bluetoothScanf("%f", &val);
            setGain(1, val);
        }

        if (command == 'i')
        {
            bluetoothPrintf("Set Kd: ");
            bluetoothScanf("%f", &val);
            setGain(2, val);
        }

        if (command == 'o')
        {
            start = !start;
            if (start)
            {
                motorMovChAPwm(300, 1);
                motorMovChBPwm(300, 1);
                levelInit(LEVEL_LEFT);
                delayMs(50);
            }
            else
            {
                motorStop();
            }
        }

        if (start)
        {
            int mv = getMv(LEVEL_LEFT);
            if (mv < -MAX_STEERING)
                mv = -MAX_STEERING;
            if (mv > MAX_STEERING)
                mv = MAX_STEERING;
            bluetoothPrintf("%d\n", mv);
            motorMovChAPwm(300 + mv, 1);
            motorMovChBPwm(300 - mv, 1);
            delayMs(50);
        }
        if (command == 'x')
        {
            motorStop();
            start = FALSE;
        }

        if (command == 'w')
        {
            motorMoveForward(300);
        }

        if (command == 's')
        {
            motorMoveReverse(300);
        }
    }
}
