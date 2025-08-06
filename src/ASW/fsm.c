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
        case STATE_IDLE:
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

        case STATE_MANUAL_DRIVE:
            // 주행 중 'p' 키가 눌리면 자동 주차로 전환
            if (motorState->lastKeyInput == 'p')
            {
                currentState = STATE_AUTO_PARK;
            }
            // AEB 발동 시, 단 후진(1,2,3번 키 제외)이 아닐 경우 긴급 정지 상태로 전환
            if (aebFlag && !(motorState->lastKeyInput == '1' || motorState->lastKeyInput == '2' || motorState->lastKeyInput == '3'))
            {
                currentState = STATE_EMERGENCY_STOP;
            }
            else
            {
                // 평상시: 키 입력에 따른 상태 갱신 및 모터 구동
                motorUpdateState(motorState);
                motorRunCommand(motorState);
            }
            break;

        case STATE_EMERGENCY_STOP:
            // 긴급 정지 동작 수행 (예: 모터 정지, 경고 출력 등)
            performEmergencyStop();

            // AEB가 해제되면 IDLE 상태로 복귀
            if (!aebFlag)
            {
                currentState = STATE_IDLE;
            }
            break;

        case STATE_AUTO_PARK:
            // 자동 주차 알고리즘 실행
            auto_park();

            // 주차 완료 후 IDLE 상태로 전환
            currentState = STATE_IDLE;
            break;
    }
}
