from pynput import keyboard
import threading
import time
import serial

# ─── 시리얼 포트 설정 ───
ser = serial.Serial('COM4', 115200)

# ─── 상태 변수 ───
is_braking = False
brake_thread_running = False
direction_pressed = None
lock = threading.Lock()
pressed_keys = set()
engine_unlocked = False  #시동 여부

# ─── NumPad VK → 문자 매핑 ───
NUMPAD_VK = {
    96: '0', 97: '1', 98: '2', 99: '3', 100: '4',
    101: '5', 102: '6', 103: '7', 104: '8', 105: '9'
}

# ─── 키 종류 구분 ───
MOVEMENT_KEYS = {'1','2','3','4','5','6','7','8','9'}
CONFIG_KEYS = {'a', 's', 'd', 'f', 'g','h','j','k','l','r', 'p', 'x','t'}

# ─── 방향 전송 함수 ───
def send_direction_once():
    with lock:
        if direction_pressed:
            ser.write(direction_pressed.encode())
            print(f"[MOV] 전송: {direction_pressed}")

# ─── 브레이크 전송 함수 (에러 해결 포인트) ───
def send_brake_once():
    global brake_thread_running
    with lock:
        ser.write(b'B')
        print("[BRAKE] 전송: B")
        brake_thread_running = False

# ─── 키 누름 핸들러 ───
def on_press(key):
    global is_braking, direction_pressed, engine_unlocked

    if not engine_unlocked:
        return  # 시동 전에는 입력 무시

    if hasattr(key, 'vk') and key.vk in NUMPAD_VK:
        ch = NUMPAD_VK[key.vk]
    elif isinstance(key, keyboard.KeyCode) and key.char in MOVEMENT_KEYS | CONFIG_KEYS:
        ch = key.char
    else:
        return

    if ch in pressed_keys and ch in CONFIG_KEYS:
        return

    pressed_keys.add(ch)
    with lock:
        is_braking = False

    if ch in MOVEMENT_KEYS:
        with lock:
            if direction_pressed != ch:
                direction_pressed = ch
                threading.Thread(target=send_direction_once, daemon=True).start()

    elif ch in CONFIG_KEYS:
        config_map = {
            'a': "속도 설정: 30%",
            's': "속도 설정: 40%",
            'd': "속도 설정: 50%",
            'f': "속도 설정: 60%",
            'g': "속도 설정: 70%",
            'h': "속도 설정: 70%",
            'j': "속도 설정: 80%",
            'k': "속도 설정: 90%",
            'l': "속도 설정: 100%", 
            'r': "비상 정지 해제 (restart)",
            'p': "자율 주차 모드 진입",
            'x': "비상 후진",
            't': "레이저 센서 모드 변경"
        }
        print(config_map[ch])
        ser.write(ch.encode())

# ─── 키 뗌 핸들러 ───
def on_release(key):
    global is_braking, direction_pressed, brake_thread_running

    if key == keyboard.Key.esc:
        print("ESC → 종료")
        ser.close()
        return False

    if hasattr(key, 'vk') and key.vk in NUMPAD_VK:
        ch = NUMPAD_VK[key.vk]
    elif isinstance(key, keyboard.KeyCode) and key.char in pressed_keys:
        ch = key.char
    else:
        return

    pressed_keys.discard(ch)

    if ch in MOVEMENT_KEYS:
        with lock:
            direction_pressed = None

    if not pressed_keys:
        print("키 뗌 → 감속 모드 진입")
        with lock:
            is_braking = True
            direction_pressed = None
        if not brake_thread_running:
            brake_thread_running = True
            threading.Thread(target=send_brake_once, daemon=True).start()

    print(f"released: {ch}, direction_pressed: {direction_pressed}")


# ─── 시동 절차 ───
def authenticate_engine():
    global engine_unlocked

    while not engine_unlocked:
        password = input("암호를 입력하세요: ")

        ser.reset_input_buffer()
        ser.write(password.encode() + b'\n')

        start_time = time.time()
        response = ""

        while time.time() - start_time < 2:
            if ser.in_waiting:
                incoming = ser.read(ser.in_waiting).decode(errors='ignore')
                print(f"📥 받은 응답: {repr(incoming)}")  # ✅ 응답 디버깅용 로그
                response += incoming

                if "OK" in response:
                    print("✅ 시동 성공")
                    engine_unlocked = True
                    break
                elif "FAIL" in response:
                    print("❌ 시동 실패 - 다시 입력하세요\n")
                    break
            time.sleep(0.1)

        if not response:
            print("⚠️ 응답 없음 - 다시 시도하세요\n")





# ─── 메인 실행 ───
if __name__ == "__main__":
    print("RC카 제어 시작 (ESC 종료)")
    authenticate_engine()  # 시동 절차 먼저 실행
    with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
        listener.join()