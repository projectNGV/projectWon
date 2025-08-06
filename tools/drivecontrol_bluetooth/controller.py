from pynput import keyboard
import serial
import time

# ─── 시리얼 포트 설정 ───
ser = serial.Serial('COM10', 115200, timeout=2)

# ─── NumPad VK → 문자 매핑 ───
NUMPAD_VK = {
    96: '0', 97: '1', 98: '2', 99: '3', 100: '4',
    101: '5', 102: '6', 103: '7', 104: '8', 105: '9'
}

# ─── 키 종류 구분 ───
MOVEMENT_KEYS = {'1','2','3','4','5','6','7','8','9','t'}
CONFIG_KEYS = {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'p'}

# ─── 상태 변수 ───
current_direction = None

# ─── 방향 전송 함수 ───
def send(char):
    ser.write(char.encode())
    print(f"[SEND] {char}")
# ─── 키 누름 핸들러 ───
def on_press(key):
    global current_direction

    ch = None
    if hasattr(key, 'vk') and key.vk in NUMPAD_VK:
        ch = NUMPAD_VK[key.vk]
    elif isinstance(key, keyboard.KeyCode):
        ch = key.char

    if not ch:
        return

    if ch in CONFIG_KEYS:
        send(ch)
        config_map = {
            'a': "듀티: 30%", 
            's': "듀티: 40%", 
            'd': "듀티: 50%", 
            'f': "듀티: 60%",
            'g': "듀티: 70%", 
            'h': "듀티: 80%", 
            'j': "듀티: 90%", 
            'k': "듀티: 100%",
            'p': "자율 주차 모드"
        }
        print(config_map.get(ch, "설정 키"))
        return

    if ch in MOVEMENT_KEYS and ch != current_direction:
        current_direction = ch
        send(ch)

# ─── 키 뗌 핸들러 ───
def on_release(key):
    global current_direction

    if key == keyboard.Key.esc:
        print("ESC → 종료")
        ser.close()
        return False

    ch = None
    if hasattr(key, 'vk') and key.vk in NUMPAD_VK:
        ch = NUMPAD_VK[key.vk]
    elif isinstance(key, keyboard.KeyCode):
        ch = key.char

    if not ch:
        return

    if ch == current_direction:
        current_direction = None
        send('B')  # 감속 명령
        print("[BRAKE] 감속 모드 진입")

# ─── 메인 실행 ───
if __name__ == "__main__":
    while(1):
        print("비밀번호 입력: ")
        send_data = input()
        
        for char in send_data:
            ser.write(char.encode('utf-8'))
            time.sleep(0.01)
        ser.write(b'\n') 

        res = ser.readline().decode().strip()
        if(res == "OK"):
            print("비밀번호 일치")
            break
        elif(res == "FAILED"):
            print("비밀번호가 일치하지 않습니다.")
        else:
            if(res == ''):
                print(f">> 응답 없음 또는 알 수 없는 응답: '{res}'")

    print("RC카 키보드 제어 시작 (ESC 종료)")
    with keyboard.Listener(on_press=on_press, on_release=on_release) as listener:
        listener.join()