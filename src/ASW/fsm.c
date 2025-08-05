#include "fsm.h"

extern volatile bool aebFlag;

VehicleState currentState = STATE_IDLE;

void handleStateMachine(MotorState* motorState)
{
    unsigned int distance = tofGetValue();
    updateAebFlagByTof(distance);  // AEB 상태 갱신

    switch (currentState) {
        case STATE_IDLE:
            if (motorState->lastKeyInput >= '1' && motorState->lastKeyInput <= '9') {
                currentState = STATE_MANUAL_DRIVE;
            } else if (motorState->lastKeyInput == 'p') {
                currentState = STATE_AUTO_PARK;
            }
            break;

        case STATE_MANUAL_DRIVE:
            if (aebFlag && !(motorState->lastKeyInput == '1' || motorState->lastKeyInput == '2' || motorState->lastKeyInput == '3')) {
                currentState = STATE_EMERGENCY_STOP;
            } else {
                motorUpdateState(motorState);
                motorRunCommand(motorState);
            }
            break;

        case STATE_EMERGENCY_STOP:
            performEmergencyStop();
            if (!aebFlag) {
                currentState = STATE_IDLE;
            }
            break;

        case STATE_AUTO_PARK:
            // 자동 주차 알고리즘 시작 (TODO)
            // 현재는 미구현 상태, 주차 종료되면 IDLE 복귀
            currentState = STATE_IDLE;
            break;
    }
}
