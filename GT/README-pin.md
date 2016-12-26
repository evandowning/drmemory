# dynamic-binary-instrumentation
Used for performing dynamic binary rewriting on Windows PEs using Pin

## Building (and modifying) this tool
First, install Windows 10 32-bit on a VM.
We will use this VM to construct our tool for modifying a PEs execution.

Install Visual Studio 15
  - Make sure you select options to install C++

Install cygwin (try using the optional of going through Internet Explorer)
  - Include make and gcc-c++ in the installation
  - After installation, start cygwin and move "/bin/link.exe" to "/bin/link\_backup.exe"
    - This is so that it doesn't conflict with Visual Studio's link.exe

Install pin (https://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool)
for Windows (for the msvc compiler).

Move our tool into the ./pin-location/source/tools directory

Compile our tool
```
Run the Visual Studio developer command prompt
Run cygwin.bat (C:\cygwin\Cygwin.bat)

$ cd C:
$ cd ./pin-location/source/tools/ARSA
$ make TARGET-ia32
```

This results in our tool's binary

## Using this tool
First, install Windows xx on another VM.

Then install pin on this machine and transfer our tool's binary.

To test, compile our test program (displays some dialog)
```
$ cd ./pin-locations/source/tools/ARSA
$ cl test.cpp
$ ./test.exe (prints message to screen)
```

First get a trace of the target program's execution.
```
$ cd ./pin-location/source/tools/ARSA
$ make TARGET=ia32
$ ../../../pin -t obj-ia32/arsa-trace.dll -o pin.log test.exe
```

Then, create an input file that modifies this syscall trace (magic part => analysis via ML stuffs).
```
$ ./pin/source/tools/OURTOOL/trace\_original.log
$ ./pin/source/tools/OURTOOL/trace\_new.log
```

Finally, simply rerun the binary using pin and this new input file to change
the binary's execution.
```
$ cd ./pin/source/tools/ARSA
$ ../../../pin -t obj-ia32/arsa-rewrite.dll -- test.exe
```

## Modification

If you would like to add additional API calls during the binary's execution,
simply modify the input file as such:

## Real-world

One can think of the malware author using the first step (getting a trace
of their malware's system calls) to gather recon on what their malware will
look like to a ML algorithm.

Then, using an adversarial ML algorithm, they can compute how to modify the
API's call sequence to evade ML signatures.

Finally, they feed this new sequence back into pin when the malware is finally
run such that it evade detection by the ML algorithm.
