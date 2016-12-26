# dynamic-binary-instrumentation
Used for performing dynamic binary rewriting on Windows PEs using DrMemory (based on DynamoRIO)

## Installing
1. Install Windows 10 32-bit on a VM.
   We will use this VM to construct our tool for modifying a PEs execution.

2. Install Visual Studio 2013 Professional
  - Make sure you select options to install C++

3. Install cygwin (try using the optional of going through Internet Explorer)
  - Include perl and git in the installation
  - After installation, start cygwin and move "/bin/link.exe" to "/bin/link\_backup.exe"
    This is so that it doesn't conflict with Visual Studio's link.exe

4. Install cmake (at least 2.8.10.1 but not 3.x) for Windows (not Cygwin):
  - https://cmake.org/download/
    - We downloaded and installed 2.8.12.2 (https://cmake.org/files/)
  - Put its bin directory on the path for your shell (an option during installation)

5. Start Visual Studio 2013's developer console

6. Start cygwin within this console (type "C:\cygwin\Cygwin.bat")

7. Download this repository
```
$ cd C:
$ cd Users/*username*/Desktop

We are based on DrMemory release 1.11.0
  - https://github.com/DynamoRIO/drmemory
  - https://github.com/DynamoRIO/drmemory/wiki/How-To-Build

$ git clone *url*.git
$ cd drmemory
$ make/git/devsetup.sh
```

8. Compile DrMemory
```
$ mkdir build
$ cd build
$ CC=cl CXX=cl cmake-gui ..

Press Configure
Select "Visual Studio 12"

$ CC=cl CXX=cl cmake -DCMAKE\_MAKE\_PROGRAM:FILEPATH=MSBuild ..
```

9. Compile DrStrace (and other DrMemory extensions)
```
$ cmake --build . --config RelWithDebInfo -- /m

NOTE: It's okay if it fails to build the docs. Compiling was successful as long
as the configuration says "Configuring done".
```

10. Test that the installation was successful:
```
$ cd ./drmemory/build
$ ./bin/drstrace.exe -- calc
$ vim drstrace.calc.exe.\*.log
```

## Usage (Work in progress...)
1. Install Windows xx on another VM.

2. Start Visual Studio 2013's developer console.

3. Start cygwin within this console (type "C:\cygwin\Cygwin.bat")

4. Run the following:
```
$ cd ./drmemory/build
$ ./bin/drstrace -- target.exe
```

This creates drstrace\_target.exe.\*.log
Magic happens and you create another .log file (new system calls)

*TODO*
Then the plugin takes this as input and ensures this system call sequence is
followed.

## Testing
1. Start Visual Studio 2013's developer console

2. Start cygwin within this console (type "C:\cygwin\Cygwin.bat")

3. cl ./drmemory/GT-tests/simple.cpp

4. ./drmemory/build/bin/drstrace -gttrace simple.exe

5. ./drmemory/build/bin/drstrace -gtmod infile simple.exe

## Modifying this tool
1. Start Visual Studio 2013's developer console

2. Start cygwin within this console (type "C:\cygwin\Cygwin.bat")

3. Modify whatever you'd like in drstrace

4. Recompile DrStrace:
```
$ cd ./drmemory/build
$ cmake --build . --config RelWithDebInfo -- /m
```

5. Rerun our tool:
```
$ cd ./drmemory/build
$ ./bin/drstrace -- target.exe
```

## Real-world Application
One can think of the malware author using the first step (getting a trace
of their malware's system calls) to gather recon on what their malware will
look like to a ML algorithm.

Then, using an adversarial ML algorithm, they can compute how to modify the
API's call sequence to evade ML signatures.

Finally, they feed this new sequence back into pin when the malware is finally
run such that it evade detection by the ML algorithm.

## Implementation details
We modified DrStrace to do our work.
In particular, we can modify (in drstrace.c) the function "event_post_syscall"
such that we can add an additional system call.

We can call Windows internal functions via (https://msdn.microsoft.com/en-us/library/bb432200.aspx)
As an example: http://www.kegel.com/wine/NativeApiTest.c
