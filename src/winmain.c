#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <kfs.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR szCmdLine, int iCmdShow )
{
  wchar_t buffer[36];
  GetPrivateProfileString(
    "DEFAULTS", "ARC", "main.kfs", buffer, sizeof(buffer), ".\\config.ini"
  );
  MessageBox(NULL, buffer, buffer, 0x00);
}