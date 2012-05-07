#include <windows.h>
#include <process.h>
#include <QApplication>
#include "extio_tinysdr.h"

// Device initialisation settings
#define EXTIO_DEVICE_NAME "SimpleTest"
#define EXTIO_DEVICE_TYPE "v0.1"
#define EXTIO_TYPE_DAC16 3
#define EXTIO_TYPE_NODAC 4
#define EXTIO_TYPE_DAC24 5
#define EXTIO_TYPE_DAC32 6
#define EXTIO_TYPE_DAC32FLOAT 7

// Winrad callback definitions
#define WINRAD_STATUS_NONE 0
#define WINRAD_STATUS_SR_CHANGED 100
#define WINRAD_STATUS_LO_CHANGED 101

// API References
#define LIBRARY_API extern "C" __stdcall __declspec(dllexport)

// Global declarations
ExtIO *extio;
HANDLE guiThread;
void (* WinradCallBack)(int, int, float, void *);


// QT GUI run in a thread
void GuiThreadProc(void * param) {
    int argc = 0;
    char **argv = NULL;
    QApplication app(argc, argv);
    extio = new ExtIO();
    extio->show();
    app.exec();
    return;
}


// Win32 DLL Main entry
BOOL WINAPI DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpvReserved*/ ) {
    return TRUE;
}


LIBRARY_API
bool InitHW(char *name, char *model, int& type) {
    strcpy(name, EXTIO_DEVICE_NAME);
    strcpy(model, EXTIO_DEVICE_TYPE);
    type = EXTIO_TYPE_DAC16; // SoundCard input 16 bits

    // Start GUI (QT) thread
    guiThread = (HANDLE) _beginthread( GuiThreadProc, 0, NULL );

    return true;
}


LIBRARY_API
bool OpenHW() {
    // Callback entry for the QT object
    extio->WinradCallBack = WinradCallBack;
    return true;
}


LIBRARY_API
void CloseHW() {
    //WaitForSingleObject(guiThread,INFINITE);
    delete extio;
}


LIBRARY_API
int StartHW(long freq) {
    // Winrad start button pressed, IO Buffer request
    return extio->GetPS(); // number of complex elements returned each invocation of the callback routine
}



LIBRARY_API
void StopHW() {
    // Winrad stop button pressed, stop sending IO
}


LIBRARY_API
int GetStatus() {
    return 0; // No status report for now
}


LIBRARY_API
long GetHWLO() {
    return extio->GetLO();
}


LIBRARY_API
long GetHWSR() {
    return extio->GetSR();
}


LIBRARY_API
int SetHWLO(long freq) {
    extio->SetLO(freq);
    return 0; // return 0 if the frequency is within the limits the HW can generate
}


LIBRARY_API
void ShowGUI() {
    extio->show();
}


LIBRARY_API
void HideGUI() {
    extio->hide();
}


LIBRARY_API
void SetCallback(void (* getCallBack)(int, int, float, void *)) {
    WinradCallBack = getCallBack;
}


LIBRARY_API
void TuneChanged(long freq) {
}


LIBRARY_API
void IFLimitsChanged(long low, long high) {
}


LIBRARY_API
void RawDataReady(long samprate, int *Ldata, int *Rdata, int numsamples) {
}
