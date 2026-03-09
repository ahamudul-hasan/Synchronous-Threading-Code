<div align="center">

# 🔒 Synchronous Threading in C

<p align="center">
  <em>Controlled, ordered execution of concurrent threads using POSIX mutexes and condition variables.</em>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c&logoColor=white" alt="Language: C"/>
  <img src="https://img.shields.io/badge/Library-pthreads-orange?style=for-the-badge&logo=linux&logoColor=white" alt="Library: pthreads"/>
  <img src="https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20WSL-lightgrey?style=for-the-badge&logo=gnu&logoColor=white" alt="Platform"/>
  <img src="https://img.shields.io/badge/Topic-Concurrency-purple?style=for-the-badge" alt="Topic: Concurrency"/>
</p>

</div>

---

## 📖 Overview

This project demonstrates **synchronous thread coordination** in C using the POSIX Threads (`pthreads`) library. Three threads are created and run concurrently, yet they produce output in a **strictly deterministic order** — regardless of how the operating system schedules them.

This is a foundational example of **thread synchronization**, showing how mutexes and condition variables can be used to eliminate race conditions and enforce execution ordering across independent threads.

---

## ⚙️ How It Works

The program spawns three threads — `threadA`, `threadB`, and `threadC` — each responsible for printing one statement. A shared integer `turn` acts as a **sequencing token**, and a condition variable ensures threads wait until it is their turn to proceed.

### 🔄 Execution Flow

```
Main Thread
 ├── Create threadA  →  waits for turn == 1
 ├── Create threadC  →  prints immediately, sets turn = 2, signals
 └── Create threadB  →  waits for turn == 2, then prints, sets turn = 1, signals

Output (always in this order):
  ✅  This is statement 3   ← threadC  (runs first, no waiting)
  ✅  This is statement 2   ← threadB  (wakes when turn == 2)
  ✅  This is statement 1   ← threadA  (wakes when turn == 1)
```

### 🔧 Synchronization Primitives

| Primitive | Role |
|:---:|:---|
| `pthread_mutex_t` | 🔐 Protects shared state (`turn`) from concurrent access |
| `pthread_cond_t` | 💤 Allows threads to sleep and be woken when conditions change |
| `pthread_cond_wait` | ⚛️ Atomically releases the mutex and blocks until signaled |
| `pthread_cond_broadcast` | 📢 Wakes all waiting threads to re-check the condition |

---

## 💡 Key Concepts

<table>
<tr>
<td width="50%">

### 🔐 Mutex (Mutual Exclusion)
Ensures only one thread accesses the shared `turn` variable at a time, preventing data races.

</td>
<td width="50%">

### 💤 Condition Variable
Enables threads to block efficiently instead of spinning in a busy-wait loop.

</td>
</tr>
<tr>
<td>

### 🛡️ Spurious Wakeup Protection
Condition waits are always placed inside `while` loops — not `if` statements — to safely re-check the predicate as required by POSIX.

</td>
<td>

### 📢 Broadcast vs Signal
`pthread_cond_broadcast` wakes **all** waiting threads simultaneously, allowing each to evaluate whether it is its turn.

</td>
</tr>
</table>

---

## 📁 Project Structure

```
📦 Synchronous-Threading-Code/
 └── 📄 synchronous_threading_code.c   ← Single-file C implementation
```

---

## 🚀 Building & Running

### Prerequisites

> ✅ A C compiler — GCC or Clang  
> ✅ A POSIX-compatible system — Linux, macOS, or WSL on Windows

### 1. Compile

```bash
gcc -o sync_threads synchronous_threading_code.c -lpthread
```

### 2. Run

```bash
./sync_threads
```

### 3. Expected Output

```
This is statement 3
This is statement 2
This is statement 1
```

> The output is **always** in this exact order, demonstrating successful synchronization.

---

## 🧠 Why This Matters

Without synchronization, thread execution order is **non-deterministic** — the OS scheduler decides which thread runs and when. This program shows how to:

| Goal | Technique |
|:---|:---|
| 🔢 Enforce execution ordering | Shared `turn` token + condition variable |
| ⚡ Avoid busy-waiting | `pthread_cond_wait` suspends the thread efficiently |
| 🔒 Protect shared state | Mutex guards all reads/writes to `turn` |
| 🛡️ Handle spurious wakeups | `while` loop re-checks the predicate after every wake |

These patterns are directly applicable in real-world scenarios such as **pipeline stages**, **producer-consumer queues**, and **phased parallel computations**.

---