# Garage-Scheduling-Optimisation-Project

# 🚗 Garage Scheduling Optimization (Genetic Algorithm)

## 📌 Overview

This project implements an intelligent scheduling system for a garage using a **Genetic Algorithm (GA)**. The goal is to optimize task allocation among mechanics while considering task dependencies, fatigue constraints, and dynamic task generation.

---

## 🎯 Problem Statement

A garage has:

* **N tasks (cars/services)** represented as a **DAG (Directed Acyclic Graph)**
* **M mechanics**
* Each task takes **1 time unit**
* Tasks may generate **new probabilistic sub-tasks**

### Constraints:

* 🔗 **Task Dependencies:** A task can only start after its dependencies are completed
* 😴 **Fatigue Constraint:** A mechanic must take a break after **K consecutive tasks**
* 🎲 **Dynamic Tasks:** Tasks may generate new tasks based on probability

---

## ⚙️ Approach

### 🧠 Metaheuristic Used:

* **Genetic Algorithm (GA)**

### 🧬 Chromosome Representation:

* A schedule = list of `(taskId, mechanicId)` assignments

### 🔁 GA Operations:

* **Initialization:** Random valid schedules
* **Selection:** Based on fitness (minimization)
* **Crossover:** Combine two parent schedules
* **Mutation:** Random swap or mechanic change
* **Repair:** Ensures each task appears exactly once

---

## 📊 Fitness Function

The fitness function evaluates:

* ⏱ **Makespan (total completion time)**
* ⏳ **Waiting time penalties**
* ❌ **Invalid schedules (dependency violation)** → rejected using high cost (`1e9`)

---

## 🔄 Dynamic Scheduling

After each iteration:

* New tasks may be generated probabilistically
* The schedule is **recomputed dynamically**

---

## 🏗️ Code Structure

| Component            | Description                                      |
| -------------------- | ------------------------------------------------ |
| `Task`               | Represents a task with dependencies and children |
| `Assignment`         | Maps task to mechanic                            |
| `fitness()`          | Evaluates schedule quality                       |
| `geneticSolve()`     | Runs Genetic Algorithm                           |
| `repair()`           | Fixes duplicate/missing tasks                    |
| `generateNewTasks()` | Adds dynamic tasks                               |

---

## ▶️ How to Run

### 🔧 Compile:

```bash
g++ -std=c++17 project.cpp -o output
```

### ▶️ Run:

```bash
./output
```

### 📥 Input Format:

```
N M K
For each task:
  depCount dep1 dep2 ...
  childCount child prob ...
```

---

## 📈 Sample Output

```
Initial Schedule:
Task 0 -> Mechanic 1
Task 2 -> Mechanic 0
Task 1 -> Mechanic 0

Updated Schedule at step 1:
Task 0 -> Mechanic 1
Task 2 -> Mechanic 0
Task 1 -> Mechanic 0
```

---

## ✅ Features

* ✔ Uses **Genetic Algorithm (Metaheuristic)**
* ✔ Handles **DAG dependencies**
* ✔ Supports **dynamic task generation**
* ✔ Enforces **fatigue constraints**
* ✔ Ensures **valid scheduling (no duplicates)**

---

## 🚀 Future Improvements

* 🔥 Topological sorting for better initialization
* ⚡ Hybrid GA + Local Search
* 📊 Gantt chart visualization
* 🧠 Smarter mutation strategies

---

## 🎯 Conclusion

This project demonstrates how metaheuristic techniques like Genetic Algorithms can efficiently solve complex real-world scheduling problems with constraints and uncertainty.

---

## 👨‍💻 Author

Sanidhya Kumar Tiwari
