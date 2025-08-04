#ifndef ASW_AUTOPARK_AUTOPARK_H_
#define ASW_AUTOPARK_AUTOPARK_H_

#include "ultrasonic.h"
#include "motor.h"
#include "util.h"
#include <stdbool.h>
#include "level.h"
#include "uart.h"
#include "autopark_teststub.h"
#include "asclin.h"
#include "motor.h"


#define EMPTY_SLOT_DISTANCE_CM 100
#define EMPTY_SLOT_DISTANCE_CM 100
#define EMPTY_SLOT_TIME_MS 3000
#define REAR_STOP_DISTANCE_CM 15

#define ALIGN_CHECK_INTERVAL_MS 200 // 비교 주기
#define ALIGN_DELTA_THRESHOLD 5     // 정렬 민감도 기준 cm



typedef enum
{
    PARKING_IDLE_test,     // 0: 대기 상태 (사용자 입력 대기 중)
    ALIGN_AND_SEARCH_test, // 1: 정렬과 주차 공간 탐색을 동시에 수행
    CONFIRM_SLOT_test,     // 2: 주차 공간이 확보되었음을 확인하고 진입 위치 조정
    PARKING_REVERSE_test,  // 3: 후진 주차 수행 중 (조향 포함)
    PARKING_DONE_test      // 4: 주차 완료, 정지 및 알림

} ParkingState_test;





void parkingAutonomousInit_test(void);

void parkingAutonomousProcess_test(void);


#endif /* ASW_AUTOPARK_LEVEL_H_ */
