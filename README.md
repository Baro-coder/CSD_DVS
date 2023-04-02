# ***CSD DVS***

## *Computer Systems Diagnostics - Distributed Voting System*

---

## **Content**:

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
|**GCC**         |    `11.3.0`   |
|**C Standard**  |      `99`     |

---

## **Installation**

### **1. Clone repository**

**1.1. Download the project repository**
``` console
git clone https://github.com/Baro-coder/CSD_DVS.git
```

**1.2. Change current directory to project workdir**
``` console
cd ./CSD_DVS/
```

### **2. Build**

**2.1. Make project `build/` directory**
```console
mkdir ./build/
```

**2.2. Change current directory to `build/`**
```console
cd ./build/
```

**2.3. Build project structure with *CMake***
```console
cmake ..
```

**2.4. Build project with Makefiles**
```console
make
```

### **3. Run**

**Usage**

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

*Output for deafult set 6 processes*

``` bash
$USER@$HOSTNAME:~/CSD_DVS/build$ ./app/app

-- App: Init: Done.
-- App: Children init:

-- * P[0]: Init.
-- * P[0]: Ready.
-- * P[1]: Init.
-- * P[1]: Ready.
-- * P[2]: Init.
-- * P[2]: Ready.
-- * P[3]: Init.
-- * P[3]: Ready.
-- * P[4]: Init.
-- * P[4]: Ready.
-- * P[5]: Init.
-- * P[5]: Ready.

-- App: Start Voting.

-- * P[0]: Vote: [0]
-- * P[1]: Vote: [1]
-- * P[2]: Vote: [1]
-- * P[3]: Vote: [1]
-- * P[4]: Vote: [1]
-- * P[5]: Vote: [0]
-- * P[0]: Distributing vote...
-- * P[1]: Distributing vote...
-- * P[2]: Distributing vote...
-- * P[3]: Distributing vote...
-- * P[4]: Distributing vote...
-- * P[5]: Distributing vote...
-- * P[0]: Reading votes... [ 0 1 1 1 1 0 ]
-- * P[1]: Reading votes... [ 0 1 1 1 1 0 ]
-- * P[2]: Reading votes... [ 0 1 1 1 1 0 ]
-- * P[3]: Reading votes... [ 0 1 1 1 1 0 ]
-- * P[4]: Reading votes... [ 0 1 1 1 1 0 ]
-- * P[5]: Reading votes... [ 0 1 1 1 1 0 ]
-- * P[0]: Making decision... [ For: 4 | Against: 2 ]
-- * P[0]: Exit: Decision: [1]
-- * P[1]: Making decision... [ For: 4 | Against: 2 ]
-- * P[1]: Exit: Decision: [1]
-- * P[2]: Making decision... [ For: 4 | Against: 2 ]
-- * P[2]: Exit: Decision: [1]
-- * P[3]: Making decision... [ For: 4 | Against: 2 ]
-- * P[3]: Exit: Decision: [1]
-- * P[4]: Making decision... [ For: 4 | Against: 2 ]
-- * P[4]: Exit: Decision: [1]
-- * P[5]: Making decision... [ For: 4 | Against: 2 ]
-- * P[5]: Exit: Decision: [1]

-- App: Cleaning: Done.
-- App: Exit.
```

---
