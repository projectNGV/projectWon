#include "autopark.h"

#define EMPTY_SLOT_DISTANCE_CM 100
#define EMPTY_SLOT_TIME_MS 3000
#define REAR_STOP_DISTANCE_CM 15

#define ALIGN_CHECK_INTERVAL_MS 200 // 비교 주기
#define ALIGN_DELTA_THRESHOLD 5     // 정렬 민감도 기준 cm

typedef enum
{
    PARKING_IDLE,     // 0: 대기 상태 (사용자 입력 대기 중)
    ALIGN_AND_SEARCH, // 1: 정렬과 주차 공간 탐색을 동시에 수행
    CONFIRM_SLOT,     // 2: 주차 공간이 확보되었음을 확인하고 진입 위치 조정
    PARKING_REVERSE,  // 3: 후진 주차 수행 중 (조향 포함)
    PARKING_DONE      // 4: 주차 완료, 정지 및 알림
} ParkingState;

// 자율 주차 신호가 오기전의 상태
static ParkingState currentState = PARKING_IDLE;

// 주차 공간 감지 시작 시각 저장 변수 (밀리초 기준)
static uint32_t emptySlotStartTime = 0;

// 벽 정렬 완료 여부 → true이면 정렬이 완료된 상태
static bool alignDone = false;

static int prevLeftCm = 0;         // 이전 측정값 저장
static uint32_t lastCheckTime = 0; // 마지막 측정 시각

// 초기화 함수: FSM 초기 상태와 변수들을 초기화
void parkingAutonomousInit(void)
{
    emptySlotStartTime = 0; // 탐색 타이머 초기화
    alignDone = false;      // 정렬 초기화
}

// FSM 메인 프로세스: 주기적으로 호출되어 상태에 따라 행동 수행
void parkingAutonomousProcess()
{
    while (1)
    {
        switch (currentState)
        {
        // [0] 초기 대기 상태: 'p' 키 입력이 들어오면 주차 시작
        case PARKING_IDLE:
        {
            while (asclin0InUart() != 'p')
                // 'p' 키 입력 감지
                parkingAutonomousInit();     // FSM 초기화
            motorStop();                     // 모터 정지
                                             //                LED_set_state(1);          // LED 1로 주차 진행 표시
            currentState = ALIGN_AND_SEARCH; // 정렬과 주차 공간 탐색을 동시에 수행으로 상태 전이

            break;
        }
        // [1] 정렬과 주차 공간 탐색을 동시에 수행
        case ALIGN_AND_SEARCH:
        {
            // 1. 전진
            motorMoveForward(30);
            while (1)
            {
                // 2. 초음파 센서로 왼쪽 벽과 정렬 유지

                
                //level.c 코드
                   

                // 3. 왼쪽 벽 탐색 (주차 공간 판단)
                int leftDist = getDistanceByUltra(ULT_LEFT); // 왼쪼 초음파로 측정

                if (leftDist > EMPTY_SLOT_DISTANCE_CM)
                {
                    if (emptySlotStartTime == 0)
                        emptySlotStartTime = getTime10Ns();
                    else if (getTime10Ns() - emptySlotStartTime > EMPTY_SLOT_TIME_MS * 100000)
                    {
                        motorStop();
                        currentState = CONFIRM_SLOT;
                    }
                }
                else
                {
                    emptySlotStartTime = 0;
                }
            }
            break;
        }

        // [2] 주차 진입 위치 확보: 약간 더 전진해서 후진 준비
        case CONFIRM_SLOT:
            motorMoveForward(25);           // 전진
            delayMs(1000);                  // 1초 정도 이동 (고정값이므로 조정 가능)
            motorStop();                    // 정지
            currentState = PARKING_REVERSE; // 후진 시작
            break;

        // [3] 후진 주차 수행: 초음파 센서를 기반으로 방향 조정
        case PARKING_REVERSE:
        {   
            //후진 시스템 시작
            while(1){

            }
            break;
        }

        // [4] 주차 완료 상태: LED 및 부저로 알림
        case PARKING_DONE:
            //            LED_set_state(2);            // LED2 점등
            //            buzzer_beep_success();               // 성공 알림음
            break;
        }
    }
}
