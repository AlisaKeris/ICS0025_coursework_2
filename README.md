# ICS0025 - Coursework #2


## Automated tests
- The required test-cases are implemented in the file `ics0025_coursework_2.cpp`. 
- Tests consist of reading commands (connect, ready, stop) from text files, just as a user would type.
- By default, running the application runs the tests automatically, and then exits.

## Manual test
- To execute the application in interactive way (that lets you type the commands yourself and verify non-blocking behavior), just change this in line 12 of the file `ics0025_coursework_2.cpp`, from this:

    ```cpp
    #define AUTOMATED_TESTS true
    ```

    into this:

    ```cpp
    #define AUTOMATED_TESTS false
    ```

- You must not start the server separately. It is started with the `connect` command altogether.

- The client is implemented by the class `PipeClient`, which also takes care of starting the server when requested to connect. 