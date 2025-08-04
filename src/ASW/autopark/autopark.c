//#include "autopark.h"
//
//// 주차 공간 감지 시작 시각 저장 변수 (밀리초 기준)
//static uint64 emptySlotStartTime = 0;
//
//// 벽 정렬 완료 여부 → true 정렬이 완료된 상태
//static bool alignDone = false;
//
//// 자율 주차 신호가 오기전의 상태
//static ParkingState currentState = PARKING_IDLE;
//
//#define MAX_STEERING 200
//
//// 초기화 함수: FSM 초기 상태와 변수들을 초기화
//void parkingAutonomousInit (void)
//{
//    emptySlotStartTime = 0; // 탐색 타이머 초기화
//    alignDone = false;      // 정렬 초기화
//    motorStop();       // 모터 정지
//}
//
//// FSM 메인 프로세스: 주기적으로 호출되어 상태에 따라 행동 수행
//void parkingAutonomousProcess(void)
//{
//    while (1)
//    {
//        switch (currentState)
//        {
//            // [0] 초기 대기 상태: 'p' 키 입력이 들어오면 주차 시작
//            case PARKING_IDLE :
//            {
//                while (bluetoothRecvByteNonBlocked() != 'p') // 'p' 키 입력 감지
//                    parkingAutonomousInit();     // FSM 초기화
//                bluetoothPrintf("RED LED ON\n"); //ledOn(RED);// LED 1로 주차 진행 표시
//                currentState = ALIGN_AND_SEARCH; // 정렬과 주차 공간 탐색을 동시에 수행으로 상태 전이
//                break;
//            }
//                // [1] 정렬과 주차 공간 탐색을 동시에 수행
//            case ALIGN_AND_SEARCH :
//            {
//
//                bluetoothPrintf("자동 주차 시작!\n");
//                // 1. 전진
//                motorMoveForward(300);
//                while (1)
//                {
//                    int mv = getMv(LEVEL_LEFT);
//                    if (mv < -MAX_STEERING)
//                        mv = -MAX_STEERING;
//                    if (mv > MAX_STEERING)
//                        mv = MAX_STEERING;
//                    bluetoothPrintf("%d\n", mv);
//                    motorMovChAPwm(300 + mv, 1);
//                    motorMovChBPwm(300 - mv, 1);
//                    delayMs(50);
//
//                }
//
//                break;
//            }
//
////                // [2] 주차 진입 위치 확보: 약간 더 전진해서 후진 준비
////            case CONFIRM_SLOT :
////                myPrintf("주차 진입 위치 학보! \n");
////                motorMoveForward(25);           // 전진
////                delayMs(1000);                  // 1초 정도 이동 (고정값이므로 조정 가능)
////                motorStop();                    // 정지
////                currentState = PARKING_REVERSE; // 후진 시작
////                break;
////
////                // [3] 후진 주차 수행: 초음파 센서를 기반으로 방향 조정
////            case PARKING_REVERSE :
////            {
////                myPrintf("후진 주차 수행! \n");
////                //후진 시스템 시작
////
////                parkReverse();
////                currentState = PARKING_DONE;
////
////                break;
////            }
////
////                // [4] 주차 완료 상태: LED 및 부저로 알림
////            case PARKING_DONE :
////                myPrintf("자동주차 완료! \n");
////                //            LED_set_state(2);            // LED2 점등
////                //            buzzer_beep_success();               // 성공 알림음
////                return;
//        }
//    }
//}
//
////
////while (1)
////            {
////
////                // 2. 초음파 센서로 왼쪽 벽과 정렬 유지
////                steer(ULT_LEFT);
////
////                // 3. 왼쪽 벽 탐색 (주차 공간 판단)
////                uint32 leftDist = getDistanceByUltra(ULT_LEFT); // 왼쪼 초음파로 측정
////
////                if (leftDist > EMPTY_SLOT_DISTANCE_CM)
////                {
////                    myPrintf("주차공간 확보되려나??\n");
////                    if (emptySlotStartTime == 0)
////                        emptySlotStartTime = getTime10Ns();
////                    else if (getTime10Ns() - emptySlotStartTime > EMPTY_SLOT_TIME_MS * 100000)
////                    {
////                        motorStop();
////                        myPrintf("주차공간 확보성공\n");
////                        currentState = CONFIRM_SLOT;
////                        break;
////                    }
////                }
////                else
////                {
////                    if(emptySlotStartTime != 0)
////                        myPrintf("주차공간 확보실패랑 초기화!!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
////
////                    myPrintf("주차공간 확보실패\n");
////                    emptySlotStartTime = 0;
////                }
////            }
