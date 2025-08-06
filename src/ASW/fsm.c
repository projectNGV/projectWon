#include "fsm.h"

extern volatile bool aebFlag;  // 긴급 제동 여부를 나타내는 전역 변수 (true면 장애물 감지됨)

VehicleState currentState = STATE_IDLE;  // 현재 차량 상태 (초기 상태는 IDLE)

// 상태 기계 메인 함수
void handleStateMachine (MotorState *motorState)
{
    // TOF 센서로부터 거리 측정값을 가져옴
    unsigned int distance = tofGetValue();

    // 측정된 거리 기반으로 AEB 플래그 업데이트 (10cm 이하일 때 true)
    updateAebFlagByTof(distance);

    // 현재 상태에 따라 동작 분기
    switch (currentState)
    {
        case STATE_IDLE :
            // 1~9 방향키가 눌리면 수동 주행 상태로 전환
            if (motorState->lastKeyInput >= '1' && motorState->lastKeyInput <= '9')
            {
                currentState = STATE_MANUAL_DRIVE;
            }
            // 'p' 키가 눌리면 자동 주차 상태로 전환
            else if (motorState->lastKeyInput == 'p')
            {
                currentState = STATE_AUTO_PARK;
            }
            break;

        case STATE_MANUAL_DRIVE :
            // AEB가 발동되었고, 후진 키가 아닌 경우 긴급정지
            if (aebFlag
                    && !(motorState->lastKeyInput == '1' || motorState->lastKeyInput == '2'
                            || motorState->lastKeyInput == '3'))
            {
                currentState = STATE_EMERGENCY_STOP;
            }
            else
            {
                // 후진 키거나, AEB가 꺼진 상태라면 정상 주행
                motorUpdateState(motorState);
                motorRunCommand(motorState);
            }
            break;

        case STATE_EMERGENCY_STOP :
            performEmergencyStop();
            emergencyBuzzer();
            // 후진 키 입력 + 후방 거리 확보 → 다시 수동 주행
            if ((motorState->lastKeyInput == '1' || motorState->lastKeyInput == '2' || motorState->lastKeyInput == '3'))
            {
                currentState = STATE_MANUAL_DRIVE;
            }
            // 또는 AEB 해제되었을 경우 → IDLE
            else if (!aebFlag)
            {
                currentState = STATE_IDLE;
            }
            break;

        case STATE_AUTO_PARK :
            // 자동 주차 알고리즘 실행
            autoPark();

            // 주차 완료 후 IDLE 상태로 전환
            currentState = STATE_IDLE;
            break;
    }
}
