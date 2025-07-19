#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <fstream>

HWND desired_hwnd = NULL;
std::wstring window_title = L"";
std::wstring sub = L"- Медиапроигрыватель VLC";
std::wstring sub2 = L"- VLC media player";
size_t gPosition = 0;
bool is_found = false;

size_t getProcId(const wchar_t* name);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

char* Rus(const wchar_t* str)
{
    char s[1024];
    CharToOem(str, s);
    return s;
}

void log(char* str) {
    std::ofstream myfile;
    myfile.open("song.txt");
    myfile << str;
    myfile.close();
}

void log(const char* str) {
    std::ofstream myfile;
    myfile.open("song.txt");
    myfile << str;
    myfile.close();
}
void log(const wchar_t* str) {
    std::ofstream myfile;
    myfile.open("song.txt");
    myfile << str;
    myfile.close();
}


bool subCheck(std::wstring str, std::wstring sub_str) {
    size_t found = str.find(sub_str);
    if (found != std::wstring::basic_string::npos) {
        return true;
    }
    return false;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    size_t position;
    std::wstring modified;

    std::cout << Rus(L"Программа работает.\nФайл вывода: song.txt\n@fluxdeken") << std::endl;

    while (true) {
        wchar_t title[1024];

        EnumWindows(EnumWindowsProc, NULL);

        if (is_found)
        {
            modified = window_title.substr(0, gPosition - 1);

            is_found = false;
            log(Rus(modified.c_str()));
        }

        Sleep(1500);
    }
    return 0;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    wchar_t buff[1024];
    GetWindowText(hwnd, buff, 1024);
    std::wstring str(buff);

    size_t pos1 = str.find(sub);
    size_t pos2 = str.find(sub2);

    if (pos1 != std::wstring::basic_string::npos) {
        gPosition = pos1;
        desired_hwnd = hwnd;
        window_title = str;
        is_found = true;
        return FALSE;
    }
    else if(pos2 != std::wstring::basic_string::npos)
    {
        gPosition = pos2;
        desired_hwnd = hwnd;
        window_title = str;
        is_found = true;
        return FALSE;
    }

    return TRUE;
}

size_t getProcId(const wchar_t* name) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 procEntry;
    procEntry.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(snap, &procEntry)) {
        do {
            if (wcscmp(procEntry.szExeFile, name) == 0) {
                return procEntry.th32ProcessID;
            }
        } while (Process32Next(snap, &procEntry));
    }
    return 0;
}