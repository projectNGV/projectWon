//#include "parking_Autonomous.h"
//#include "Ultrasonic.h"
//#include "Motor.h"
//#include "Timer.h"
//#include "LED.h"
//#include "Buzzer.h"
//#include <stdbool.h>
//
//#define EMPTY_SLOT_DISTANCE_CM 100
//#define EMPTY_SLOT_TIME_MS     3000
//#define REAR_STOP_DISTANCE_CM  15
//
//#define ALIGN_CHECK_INTERVAL_MS 200       // 비교 주기
//#define ALIGN_DELTA_THRESHOLD 5           // 정렬 민감도 기준 cm
//
//typedef enum {
//    PARKING_IDLE,          // 0: 대기 상태 (사용자 입력 대기 중)
//    ALIGN_AND_SEARCH,      // 1: 정렬과 주차 공간 탐색을 동시에 수행
//    CONFIRM_SLOT,          // 2: 주차 공간이 확보되었음을 확인하고 진입 위치 조정
//    PARKING_REVERSE,       // 3: 후진 주차 수행 중 (조향 포함)
//    PARKING_DONE           // 4: 주차 완료, 정지 및 알림
//} ParkingState;
//
//// 자율 주차 신호가 오기전의 상태
//static ParkingState current_state = PARKING_IDLE;
//
//// 주차 공간 감지 시작 시각 저장 변수 (밀리초 기준)
//static uint32_t empty_slot_start_time = 0;
//
//// 벽 정렬 완료 여부 → true이면 정렬이 완료된 상태
//static bool align_done = false;
//
//static int prev_left_cm = 0;              // 이전 측정값 저장
//static uint32_t last_check_time = 0;      // 마지막 측정 시각
//
//
//// 초기화 함수: FSM 초기 상태와 변수들을 초기화
//void parking_autonomous_init(void)
//{
//    current_state = PARKING_IDLE;      // 초기 상태는 대기 상태
//    empty_slot_start_time = 0;         // 탐색 타이머 초기화
//    align_done = false;                // 정렬 초기화
//}
//
//// FSM 메인 프로세스: 주기적으로 호출되어 상태에 따라 행동 수행
//void parking_autonomous_process(void)
//{
//    switch (current_state)
//    {
//        // [0] 초기 대기 상태: 'p' 키 입력이 들어오면 주차 시작
//        case PARKING_IDLE:
//            if (user_pressed_key('p')) {
//                LED_set_state(1);          // LED 1로 주차 진행 표시
//                current_state = ALIGN_AND_SEARCH; // 정렬과 주차 공간 탐색을 동시에 수행으로 상태 전이
//            }
//            break;
//
//        // [1] 정렬과 주차 공간 탐색을 동시에 수행
//        case ALIGN_AND_SEARCH:
//        {
//            // 1. 전진
//            motor_forward_pwm(30); // 전지 수행하는 모터 함수
//
//            // 2. 왼쪽 벽 탐색 (주차 공간 판단)
//            int left_dist = ultrasonic_get(LEFT); // 왼쪼 초음파로 측정
//
//            if (left_dist > EMPTY_SLOT_DISTANCE_CM) {
//                if (empty_slot_start_time == 0)
//                    empty_slot_start_time = timer_get_ms();
//                else if (timer_get_ms() - empty_slot_start_time > EMPTY_SLOT_TIME_MS) {
//                    motor_stop();
//                    current_state = CONFIRM_SLOT;
//                }
//            } else {
//                empty_slot_start_time = 0;
//            }
//
//            // 3. 일정 시간마다 정렬 보정
//            uint32_t now = timer_get_ms();
//            if (now - last_check_time > ALIGN_CHECK_INTERVAL_MS)
//            {
//                int curr_left_cm = left_dist;  // 방금 측정한 거리
//                int delta = prev_left_cm - curr_left_cm;
//
//                if (delta > ALIGN_DELTA_THRESHOLD) {
//                    motor_turn_left();    // 점점 가까워짐 → 벽 쪽으로 기울었음 → 왼쪽 조향
//                }
//                else if (delta < -ALIGN_DELTA_THRESHOLD) {
//                    motor_turn_right();   // 점점 멀어짐 → 오른쪽 조향
//                }
//                else {
//                    motor_straight();     // 거의 변화 없음 → 평행
//                }
//
//                prev_left_cm = curr_left_cm;         // 이전 값 갱신
//                last_check_time = now;               // 시간 갱신
//            }
//
//            break;
//        }
//
//        // [2] 주차 진입 위치 확보: 약간 더 전진해서 후진 준비
//        case CONFIRM_SLOT:
//
//                motor_forward_pwm(25);         // 전진
//                delay_ms(1000);                // 1초 정도 이동 (고정값이므로 조정 가능)
//                motor_stop();                  // 정지
//                current_state = PARKING_REVERSE; // 후진 시작
//
//            break;
//
//        // [3] 후진 주차 수행: 초음파 센서를 기반으로 방향 조정
//        case PARKING_REVERSE:
//        {
//            int rear = ultrasonic_get_rear_cm();
//            int curr_right = ultrasonic_get_right_cm();
//            static int prev_right = 0;
//
//            int delta = prev_right - curr_right;
//            prev_right = curr_right;
//
//            // 조향 유지
//            if (curr_right < 20)
//                motor_reverse_turn_left();
//            else if (curr_right > 60)
//                motor_reverse_turn_right();
//            else
//                motor_reverse_pwm(30);
//
//            // 정렬 + 거리 둘 다 만족해야 멈춤
//            if (rear < REAR_STOP_DISTANCE_CM && abs(delta) < 5) {
//                motor_stop();
//                current_state = PARKING_DONE;
//            }
//
//            break;
//        }
//
//        // [4] 주차 완료 상태: LED 및 부저로 알림
//        case PARKING_DONE:
//            LED_set_state(2);            // LED2 점등
//            buzzer_beep_success();               // 성공 알림음
//            break;
//    }
//}
