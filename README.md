# ***CSD DVS***

## *Computer Systems Diagnostics - Distributed Voting System*

---

## **Content:**

### 1. [Decription](#description)

### 2. [Installation](#installation)

### 3. [Example Output](#example-output)

### 4. [Documentation](https://github.com/Baro-coder/CSD_DVS/tree/master/docs/)

---

## **Description**

This is a college project to simulate work of **DVS** - *Distributed Voting System*.

**DVS** consists of nodes communicating directly with each other (**P2P** - *point-to-point*), not through a intermediary node (*client-server*).

To simulate this type of communication, there is used UNIX **pipe** mechanism, and to synchronize each node there are used **POSIX semaphores** shared via **shared memory segment**.

Each node has its own pipe and semaphore. Each another node can send a message to this node storing a message in pipe *READ channel*. To perform an operation in *critical section* each node waits for its own semaphore to be up, and after completing the task, it release semaphore of the next node.

---

## **Requirements**

### **Operating System**

Project was developed on **64-bit Ubuntu 22.04 OS**, but every **Debian-based 64-bit OS** can handle this application.

### **Software**

|  **Software**  |  **Version**  |
|----------------|---------------|
|**CMake**       |    `3.21.1`   |
|**Clang**       |    `14.0.0`   |
|**C Standard**  |      `11`     |

---

## **Installation**

### **1. Clone repository**

1.1. **Download the project repository**

``` console
git clone https://github.com/Baro-coder/CSD_DVS.git
```

1.2. **Change current directory to project workdir**

``` console
cd ./CSD_DVS/
```

### **2. Build**

2.1. **Make project `build/` directory**

```console
mkdir ./build/
```

2.2. **Change current directory to `build/`**

```console
cd ./build/
```

2.3. **Build project structure with *CMake***

```console
cmake ..
```

2.4. **Build project with Makefiles**

```console
make
```

### **3. Run**

#### **Usage**

Child components count is set to 6 by default. So default run command will fork 6 of children processes:

*Default usage:*

``` console
./app/app
```

There is opportunity to set components count with positional argument (value must be in range <3, 100>).

*Usage with `21` processes to fork:*

``` console
./app/app 21
```

---

## **Example Output**

``` text
[5-04-2023 01:17:28] : [ INFO  ] : [ App ] : Main init...
[5-04-2023 01:17:28] : [ DEBUG ] : [ App ] : Args parsing...
[5-04-2023 01:17:28] : [ DEBUG ] : [ App ] : Signals handling...
[5-04-2023 01:17:28] : [ DEBUG ] : [ App ] : Pipes init...
[5-04-2023 01:17:28] : [ DEBUG ] : [ App ] : Control mechanisms init...
[5-04-2023 01:17:28] : [ INFO  ] : [ App ] : Components Pre-init...
[5-04-2023 01:17:28] : [ DEBUG ] : [ App ] : Malfunctioned components count: 1
[5-04-2023 01:17:28] : [ INFO  ] : [*C-0 ] : Init...
[5-04-2023 01:17:28] : [ INFO  ] : [*C-0 ] : Ready.
[5-04-2023 01:17:28] : [ INFO  ] : [ C-1 ] : Init...
[5-04-2023 01:17:28] : [ INFO  ] : [ C-1 ] : Ready.
[5-04-2023 01:17:28] : [ INFO  ] : [ C-2 ] : Init...
[5-04-2023 01:17:28] : [ INFO  ] : [ C-2 ] : Ready.
[5-04-2023 01:17:28] : [ INFO  ] : [ C-3 ] : Init...
[5-04-2023 01:17:28] : [ INFO  ] : [ C-3 ] : Ready.
[5-04-2023 01:17:28] : [ INFO  ] : [ C-4 ] : Init...
[5-04-2023 01:17:28] : [ INFO  ] : [ C-4 ] : Ready.
[5-04-2023 01:17:28] : [ INFO  ] : [ C-5 ] : Init...
[5-04-2023 01:17:28] : [ INFO  ] : [ C-5 ] : Ready.
[5-04-2023 01:17:29] : [ INFO  ] : [ App ] : Signal received: 12 | Voting start.
[5-04-2023 01:17:29] : [ INFO  ] : [*C-0 ] : Vote: 0
[5-04-2023 01:17:29] : [ INFO  ] : [ C-1 ] : Vote: 0
[5-04-2023 01:17:29] : [ INFO  ] : [ C-2 ] : Vote: 0
[5-04-2023 01:17:29] : [ INFO  ] : [ C-3 ] : Vote: 1
[5-04-2023 01:17:29] : [ INFO  ] : [ C-4 ] : Vote: 1
[5-04-2023 01:17:29] : [ INFO  ] : [ C-5 ] : Vote: 1
[5-04-2023 01:17:29] : [ INFO  ] : [*C-0 ] : Distributing vote...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-1 ] : Distributing vote...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-2 ] : Distributing vote...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-3 ] : Distributing vote...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-4 ] : Distributing vote...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-5 ] : Distributing vote...
[5-04-2023 01:17:29] : [ INFO  ] : [*C-0 ] : Reading votes...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-1 ] : Reading votes...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-2 ] : Reading votes...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-3 ] : Reading votes...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-4 ] : Reading votes...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-5 ] : Reading votes...
[5-04-2023 01:17:29] : [ INFO  ] : [*C-0 ] : Distributing votes table...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-1 ] : Distributing votes table...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-2 ] : Distributing votes table...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-3 ] : Distributing votes table...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-4 ] : Distributing votes table...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-5 ] : Distributing votes table...
[5-04-2023 01:17:29] : [ INFO  ] : [*C-0 ] : Reading votes tables...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-1 ] : Reading votes tables...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-2 ] : Reading votes tables...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-3 ] : Reading votes tables...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-4 ] : Reading votes tables...
[5-04-2023 01:17:29] : [ INFO  ] : [ C-5 ] : Reading votes tables...
[5-04-2023 01:17:29] : [ INFO  ] : [*C-0 ] : Making decision...
[5-04-2023 01:17:29] : [ DEBUG ] : [*C-0 ] : Checking for malfunctions...
[5-04-2023 01:17:29] : [ DEBUG ] : [*C-0 ] : Votes counting...
[5-04-2023 01:17:29] : [ DEBUG ] : [*C-0 ] : Counted votes: FOR(3), AGAINST(2), IGNORED(1)
[5-04-2023 01:17:29] : [ INFO  ] : [*C-0 ] : Exit: Decision => 0
[5-04-2023 01:17:29] : [ INFO  ] : [ C-1 ] : Making decision...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-1 ] : Checking for malfunctions...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-1 ] : Votes counting...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-1 ] : Counted votes: FOR(3), AGAINST(2), IGNORED(1)
[5-04-2023 01:17:29] : [ INFO  ] : [ C-1 ] : Exit: Decision => 0
[5-04-2023 01:17:29] : [ INFO  ] : [ C-2 ] : Making decision...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-2 ] : Checking for malfunctions...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-2 ] : Votes counting...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-2 ] : Counted votes: FOR(3), AGAINST(2), IGNORED(1)
[5-04-2023 01:17:29] : [ INFO  ] : [ C-2 ] : Exit: Decision => 0
[5-04-2023 01:17:29] : [ INFO  ] : [ C-3 ] : Making decision...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-3 ] : Checking for malfunctions...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-3 ] : Votes counting...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-3 ] : Counted votes: FOR(3), AGAINST(2), IGNORED(1)
[5-04-2023 01:17:29] : [ INFO  ] : [ C-3 ] : Exit: Decision => 0
[5-04-2023 01:17:29] : [ INFO  ] : [ C-4 ] : Making decision...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-4 ] : Checking for malfunctions...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-4 ] : Votes counting...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-4 ] : Counted votes: FOR(3), AGAINST(2), IGNORED(1)
[5-04-2023 01:17:29] : [ INFO  ] : [ C-4 ] : Exit: Decision => 0
[5-04-2023 01:17:29] : [ INFO  ] : [ C-5 ] : Making decision...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-5 ] : Checking for malfunctions...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-5 ] : Votes counting...
[5-04-2023 01:17:29] : [ DEBUG ] : [ C-5 ] : Counted votes: FOR(3), AGAINST(2), IGNORED(1)
[5-04-2023 01:17:29] : [ INFO  ] : [ C-5 ] : Exit: Decision => 0
[5-04-2023 01:17:29] : [ INFO  ] : [ App ] : Cleaning... (3)
[5-04-2023 01:17:29] : [ DEBUG ] : [ App ] : Logging thread-safety clear...
[5-04-2023 01:17:29] : [ DEBUG ] : [ App ] : Pipes closing...
[5-04-2023 01:17:29] : [ DEBUG ] : [ App ] : Control mechanisms closing...
[5-04-2023 01:17:29] : [ INFO  ] : [ App ] : Shutdown.
```

---
