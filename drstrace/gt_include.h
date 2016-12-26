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

// Functions to call NTAPI functions
void gt_ntcreatefile(void);
