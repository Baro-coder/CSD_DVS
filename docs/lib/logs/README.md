# ***Logs***

## *C logging library*

---

## **Content:**

1. [Overview](#overview)
2. [Installation](#installation)
3. [Example](#example)
4. [License](#license)

---

## **Overview**

### **Description**

Simple logging library developed in C. 

This library is taken from another private project which is available [here]([https://](https://github.com/Baro-coder/C_LibLogs)).

*Example output:*

``` text
[08-04-2023 15:14:22] | [ INFO  ] | [App (19683)] | Initializing...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19685)] | P1 -- Init...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19684)] | P0 -- Init...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19686)] | P2 -- Init...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19689)] | P5 -- Init...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19687)] | P3 -- Init...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19688)] | P4 -- Init...
[08-04-2023 15:14:22] | [ DEBUG ] | [Sub (19690)] | P6 -- Init...
[08-04-2023 15:14:22] | [ TRACE ] | [App (19683)] | Waiting...
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19688)] | P4 -- End of work.
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19687)] | P3 -- End of work.
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19686)] | P2 -- End of work.
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19689)] | P5 -- End of work.
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19685)] | P1 -- End of work.
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19690)] | P6 -- End of work.
[08-04-2023 15:14:25] | [ DEBUG ] | [Sub (19684)] | P0 -- End of work.
[08-04-2023 15:14:25] | [ INFO  ] | [App (19683)] | Done.
```

*Format:*

``` text
[08-04-2023 15:14:22] | [ INFO  ] | [App (19683)] | Initializing...
 ^datetime               ^level      ^owner(^PID)    ^Formatted message
```

### **Interface**

``` c
/* --- LOG WRITERS */
// Write `level` log with `owner` and message as formatted output
void logs_log(log_level_t level, const char* owner, const char* fmt, ...);

// Write TRACE log with `owner` and message as formatted output
void logs_log_trace(const char* owner, const char* fmt, ...);

// Write DEBUG log with `owner` and message as formatted output
void logs_log_debug(const char* owner, const char* fmt, ...);

// Write INFO log with `owner` and message as formatted output
void logs_log_info(const char* owner, const char* fmt, ...);

// Write WARNING log with `owner` and message as formatted output
void logs_log_warn(const char* owner, const char* fmt, ...);

// Write ERROR log with `owner` and message as formatted output
void logs_log_error(const char* owner, const char* fmt, ...);

// Write FATAL log with `owner` and message as formatted output
void logs_log_fatal(const char* owner, const char* fmt, ...);
```

### **Log Levels**

``` c
// Log levels enumeration
enum __log_level_t{
    LOG_LEVEL_TRACE,    // --> 0
    LOG_LEVEL_DEBUG,    // --> 1
    LOG_LEVEL_INFO,     // --> 2
    LOG_LEVEL_WARNING,  // --> 3
    LOG_LEVEL_ERROR,    // --> 4
    LOG_LEVEL_FATAL};   // --> 5

typedef enum __log_level_t log_level_t;
```

### **Other functions**

``` c
/* --- OPTIONS */

// Creating semaphore with name as `sem_name` to sync threads printing.
//  `Attention`: Remember to call `logs_threads_safety_disable()` to remove created semaphore!
//  Returns: `0` on success; `1` - sem_open error
int logs_threads_safety_enable(const char* sem_name);

// Removing semaphore with name as `sem_name` to sync threads printing.
//  Returns: `0` on success; `1` - sem_unlink error; `2` - sem_close error
int logs_threads_safety_disable(const char* sem_name);

// Set minimal log level to `level` (default: LOG_LEVEL_TRACE)
void logs_set_minimal_log_level(log_level_t level);

// Set log output stream to `stream` (default: `stderr`)
void logs_set_output_stream(FILE* stream);

// Set log output stream to `stderr`
void logs_set_output_stream_default();
```

---

## **Installation**

### **Automatic**

### 1. Clone repository

1.1. Download repository:

``` console
git clone https://github.com/Baro-coder/C_LibLogs
```

1.2. Change directory:

``` console
cd ./C_LibLogs/
```

### 2. Run installation script (*`sudo` required*)

```console
sudo ./install.sh
```

---

### **Manual**

### 1. Clone repository

1.1. Download repository:

``` console
git clone https://github.com/Baro-coder/C_LibLogs
```

1.2. Change directory:

``` console
cd ./C_LibLogs/
```

### 2. Build

2.1. Compile source:

``` console
gcc -c ./logs/logs.c -o ./logs/logs.o
```

2.2. Make static library:

```console
ar crs ./logs/liblogs.a ./logs/logs.o
```

### 3. Link

3.1. Copy header file to include path:

```console
sudo ./logs/logs.h /usr/include/
```

3.2. Copy library file to lib path:

``` console
sudo ./logs/liblogs.a /usr/lib/
```

---

## **Example**

### **Source code**

*fork.c :*

``` c
#include <logs.h>
#include <sys/wait.h>

void* thread_body(int id) {
    logs_log_debug("Sub", "P%d -- Init...", id);
    
    sleep(1);

    logs_log(LOG_LEVEL_DEBUG, "Sub", "P%d -- End of work.", id);

    return NULL;
}


int main(void) {
    int p_count = 10;

    logs_log_info("App", "Initializing...");
    logs_threads_safety_enable("/semTest");
    
    for(int i = 0; i < p_count; i++) {
        if(fork() == 0) {
            thread_body(i);
            exit(0);
        }
    }

    logs_log_trace("App", "Waiting...");
    for(int i = 0; i < p_count; i++) {
        wait(NULL);
    }

    logs_threads_safety_disable("/semTest");
    logs_log_info("App", "Done.");

    return 0;
}
```

### **Compilation**

You have to link library in compilation process:

*Example:*

``` console
gcc fork.c -o fork.out -llogs
```

### **Output**

``` console
./fork.out
```

``` text
[08-04-2023 15:29:06] | [  INFO   ] | [App (20756)] | Initializing...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20757)] | P0 -- Init...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20759)] | P2 -- Init...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20758)] | P1 -- Init...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20760)] | P3 -- Init...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20764)] | P7 -- Init...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20761)] | P4 -- Init...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20762)] | P5 -- Init...
[08-04-2023 15:29:06] | [  TRACE  ] | [App (20756)] | Waiting...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20763)] | P6 -- Init...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20765)] | P8 -- Init...
[08-04-2023 15:29:06] | [  DEBUG  ] | [Sub (20766)] | P9 -- Init...
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20757)] | P0 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20759)] | P2 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20758)] | P1 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20760)] | P3 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20764)] | P7 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20761)] | P4 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20762)] | P5 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20763)] | P6 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20765)] | P8 -- End of work.
[08-04-2023 15:29:07] | [  DEBUG  ] | [Sub (20766)] | P9 -- End of work.
[08-04-2023 15:29:07] | [  INFO   ] | [App (20756)] | Done.
```

---

## **License**

Library is full **open-source software**. You can redistribute it and/or modify it under the terms of the **GNU General Public License v3**.

See [*LICENSE*](https://github.com/Baro-coder/C_LibLogs/blob/master/LICENSE) for details.
