#include <iostream>
#include <Windows.h>
#include <thread>

/*
    ** -- KLASA KEYBOARD SENDER -- **

    -> ZBIERANIE DANYCH Z KLAWIATURY     <-
    -> WYSYŁANIE DANYCH Z KLAWIATURY     <-
    -> AKTYWACJA KLAWIATUYU              <-
    -> DEAZAKTYWACJA KLAWIATURY          <-

*/

static HHOOK keyboardHook;

class KEYBOARD {
public:

    static LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode >= 0) {
            if (wParam == WM_KEYDOWN) {
                KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
                std::cout << "Key down: " << kbStruct->vkCode << std::endl;
                // Wykonaj odpowiednie działania dla klawisza wciśniętego
                return 1;
            }
            else if (wParam == WM_KEYUP) {
                KBDLLHOOKSTRUCT* kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
                std::cout << "Key up: " << kbStruct->vkCode << std::endl;
                return 1;
                // Wykonaj odpowiednie działania dla klawisza zwolnionego
            }
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
        
    }

    static void _deactivateKeyboard() {
        // Wyłączenie obsługi klawiatury
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, &KeyboardHookProc, NULL, 0);
        
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            // Obsługa innych komunikatów, jeśli wymagane
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        UnhookWindowsHookEx(keyboardHook);
    }

    static void _activateKeyboard(){
        UnhookWindowsHookEx(keyboardHook);
    }

};

int main() {

    std::thread thread(&KEYBOARD::_deactivateKeyboard);

    thread.detach();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    KEYBOARD::_activateKeyboard();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;

}