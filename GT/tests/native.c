/*
 *  This program creates an empty file using the
 *   NtCreateFile function of Windows NT's Native API.
 *
 *    Note that this function is documented in the
 *     Windows DDK.
 *
 *      Original C++ version at
 *       http://analysis.seclab.tuwien.ac.at/Resources/NativeApiTest.cpp
 *        Ported to C, cygwin/miniddk.h, and wine by Dan Kegel
 *
 *         Halfway to a minimal conformance test for NtCreateFile.
 *          Prints out one of three codes for each check:
 *           FAIL if something's wrong
 *            PASS if something went right
 *             SKIP if some test had to be skipped because the system didn't support it
 *
 *              To compile with cygwin:
 *                 gcc NativeApiTest.c -I.
 *                  To compile with winegcc, copy into top of wine tree, then do:
 *                     winegcc NativeApiTest.c -Iinclude -Llibs/wine
 *                      To compile with Visual C++ Express:
 *                         can't do it from commandline, since Visual C++ Express ignores /GS- flag,
 *                            do: New Project / Win32 Console, named "demo", 
 *                               delete default files from left pane, 
 *                                  Project / Add Existing Object (this file),
 *                                     Project / demo Properties / Precompiled Headers / Not Using Precompiled Headers,
 *                                        Project / Build demo
 *
 *                                         Then just run the executable.  The first time you run it, it should pass,
 *                                          but the 2nd time it should fail unless you delete c:\testfile.txt first.
 *                                          */  


//#ifdef __WINE__
#include "windows.h"
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)  /* FIXME: wine lacks ntdef.h */
#include "winternl.h"      /* wine comes with all needed headers */
//#else
//#include <windows.h>
//#include "miniddk.h"/* installing real DDK is a pain, just use this */
//#endif

#include <stdio.h>

/* Standard headers above declare NtCreateFile, but we want a 
 *  * function pointer, so we gotta declare it ourselves here.
 *   */
typedef NTSTATUS (__stdcall *NtCreateFilePtr)(
        PHANDLE FileHandle, 
        ACCESS_MASK DesiredAccess, 
        OBJECT_ATTRIBUTES *ObjectAttributes, 
        PIO_STATUS_BLOCK IoStatusBlock, 
        PLARGE_INTEGER AllocationSize,
        ULONG FileAttributes, 
        ULONG ShareAccess, 
        ULONG CreateDisposition, 
        ULONG CreateOptions, 
        PVOID EaBuffer, 
        ULONG EaLength );

typedef VOID (__stdcall *RtlInitUnicodeStringPtr) (
        IN OUT PUNICODE_STRING  DestinationString,
        IN PCWSTR  SourceString );

int main(int argc, char *argv[]) 
{
    UNICODE_STRING fn;
    OBJECT_ATTRIBUTES object;
    IO_STATUS_BLOCK ioStatus;
    NtCreateFilePtr pNtCreateFile;
    RtlInitUnicodeStringPtr pRtlInitUnicodeString;
    HANDLE out;
    NTSTATUS status;
    HMODULE hMod;

    /* Get access to ntdll functions.  This will fail on Win9x. */
    hMod = LoadLibraryA("ntdll.dll");
    if (!hMod) {
        printf("SKIP: Could not load ntdll.dll\n");
        exit(0);
    }
    pNtCreateFile = (NtCreateFilePtr) GetProcAddress(hMod, "NtCreateFile");
    if (!pNtCreateFile) {
        printf("FAIL: Could not locate NtCreateFile\n");
        exit(1);
    }
    pRtlInitUnicodeString = (RtlInitUnicodeStringPtr) GetProcAddress(hMod,
            "RtlInitUnicodeString");

    /* Create a file using NtCreateFile */
    memset(&ioStatus, 0, sizeof(ioStatus));
    memset(&object, 0, sizeof(object));
    object.Length = sizeof(object);
    object.Attributes = OBJ_CASE_INSENSITIVE;
    pRtlInitUnicodeString(&fn, L"\\??\\C:\\testfile.txt");
    object.ObjectName = &fn;
    status = pNtCreateFile(&out, GENERIC_WRITE, &object, &ioStatus, NULL,
            FILE_ATTRIBUTE_NORMAL, 0, FILE_CREATE, 0, NULL,
            0);
    if (!NT_SUCCESS(status)) {
        printf("FAIL: Could not create file.\n");
        exit(1);
    }
    printf("PASS: Successfully created the file!\n");
    return 0;
}
