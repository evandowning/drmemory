#include "windows.h"
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)  /* FIXME: wine lacks ntdef.h */
#include "winternl.h"      /* wine comes with all needed headers */
#include <stdio.h>

#include "gt_include.h"

void gt_ntcreatefile(void)
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
//      printf("SKIP: Could not load ntdll.dll\n");
        exit(0);
    }
    pNtCreateFile = (NtCreateFilePtr) GetProcAddress(hMod, "NtCreateFile");
    if (!pNtCreateFile) {
//      printf("FAIL: Could not locate NtCreateFile\n");
        exit(1);
    }
    pRtlInitUnicodeString = (RtlInitUnicodeStringPtr) GetProcAddress(hMod,
            "RtlInitUnicodeString");

    /* Create a file using NtCreateFile */
    memset(&ioStatus, 0, sizeof(ioStatus));
    memset(&object, 0, sizeof(object));
    object.Length = sizeof(object);
    object.Attributes = OBJ_CASE_INSENSITIVE;
    pRtlInitUnicodeString(&fn, L"\\??\\C:\\testfile2.txt");
    object.ObjectName = &fn;
    status = pNtCreateFile(&out, GENERIC_WRITE, &object, &ioStatus, NULL,
            FILE_ATTRIBUTE_NORMAL, 0, FILE_CREATE, 0, NULL,
            0);
    if (!NT_SUCCESS(status)) {
//      printf("FAIL: Could not create file.\n");
        exit(1);
    }
//  printf("PASS: Successfully created the file!\n");

    return;
}
