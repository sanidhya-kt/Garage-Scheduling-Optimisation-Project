#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

struct Task {
    int id;
    vector<pair<int, double>> children;
    vector<int> deps;
};

struct Assignment {
    int taskId;
    int mechanicId;
};

int N, M, K;
vector<Task> tasks;

// RANDOM 
double randProb() {
    return (double)rand() / RAND_MAX;
}

// FITNESS 
double fitness(const vector<Assignment>& schedule) {
    vector<int> mechanicTime(M, 0);
    vector<int> consecutive(M, 0);
    unordered_map<int, int> finishTime;

    double penalty = 0;

    for (const auto &a : schedule) {
        int t = a.taskId;
        int m = a.mechanicId;

        int start = mechanicTime[m];

        for (int d : tasks[t].deps) {
            if (finishTime.find(d) == finishTime.end()) {
                return 1e9; 
            }
            start = max(start, finishTime[d]);
        }

     
        if (consecutive[m] == K) {
            start += 1;
            consecutive[m] = 0;
        }

        int finish = start + 1;
        finishTime[t] = finish;
        mechanicTime[m] = finish;
        consecutive[m]++;

        penalty += start;
    }

    int makespan = *max_element(mechanicTime.begin(), mechanicTime.end());

    return makespan + penalty;
}
// RANDOM SCHEDULE
vector<Assignment> randomSchedule() {
    vector<int> taskOrder(N);
    for (int i = 0; i < N; i++) taskOrder[i] = i;

    random_device rd;
    mt19937 g(rd());
    shuffle(taskOrder.begin(), taskOrder.end(), g);

    vector<Assignment> schedule;
    for (int i = 0; i < N; i++) {
        schedule.push_back({taskOrder[i], rand() % M});
    }

    return schedule;
}

// REPAIR FUNCTION 
void repair(vector<Assignment>& s) {
    vector<bool> used(N, false);

    // mark used
    for (auto &a : s) {
        if (a.taskId < N)
            used[a.taskId] = true;
    }

    vector<int> missing;
    for (int i = 0; i < N; i++) {
        if (!used[i]) missing.push_back(i);
    }

    vector<bool> seen(N, false);
    int idx = 0;

    for (auto &a : s) {
        if (seen[a.taskId]) {
            a.taskId = missing[idx++];
        }
        seen[a.taskId] = true;
    }
}

//  CROSSOVER
vector<Assignment> crossover(const vector<Assignment>& p1, const vector<Assignment>& p2) {
    int n = p1.size();
    vector<Assignment> child = p1;

    int l = rand() % n;
    int r = rand() % n;
    if (l > r) swap(l, r);

    for (int i = l; i <= r; i++) {
        child[i] = p2[i];
    }

    return child;
}

//MUTATION
void mutate(vector<Assignment>& s) {
    int i = rand() % s.size();
    s[i].mechanicId = rand() % M;

    if (rand() % 2) {
        int j = rand() % s.size();
        swap(s[i], s[j]);
    }
}

// GA 
vector<Assignment> geneticSolve(int generations = 200, int popSize = 50) {
    vector<vector<Assignment>> population;

    for (int i = 0; i < popSize; i++) {
        population.push_back(randomSchedule());
    }

    for (int gen = 0; gen < generations; gen++) {

        sort(population.begin(), population.end(),
             [](const vector<Assignment>& a, const vector<Assignment>& b) {
                 return fitness(a) < fitness(b);
             });

        vector<vector<Assignment>> newPop;

        // Elitism
        for (int i = 0; i < 5; i++) {
            newPop.push_back(population[i]);
        }

        while (newPop.size() < popSize) {
            const auto& p1 = population[rand() % 10];
            const auto& p2 = population[rand() % 10];

            auto child = crossover(p1, p2);
            mutate(child);
            repair(child); 

            newPop.push_back(child);
        }

        population = newPop;
    }

    return population[0];
}

// TASK GENERATION 
void generateNewTasks(vector<int>& activeTasks) {
    vector<int> newTasks;

    for (int t : activeTasks) {
        for (auto &edge : tasks[t].children) {
            double prob = edge.second;
            double r = randProb();

            if (r > prob) {
                newTasks.push_back(edge.first);
            }
        }
    }

    activeTasks.insert(activeTasks.end(), newTasks.begin(), newTasks.end());
}

int main() {
    srand(time(0));

    cin >> N >> M >> K;
    tasks.resize(N);

    for (int i = 0; i < N; i++) {
        int depCount;
        cin >> depCount;

        while (depCount--) {
            int d;
            cin >> d;
            tasks[i].deps.push_back(d);
        }

        int childCount;
        cin >> childCount;

        while (childCount--) {
            int c;
            double p;
            cin >> c >> p;
            tasks[i].children.push_back({c, p});
        }
    }

    vector<int> activeTasks;
    for (int i = 0; i < N; i++) activeTasks.push_back(i);

    auto bestSchedule = geneticSolve();

    cout << "Initial Schedule:\n";
    for (auto &a : bestSchedule) {
        cout << "Task " << a.taskId << " -> Mechanic " << a.mechanicId << "\n";
    }

    for (int t = 0; t < 5; t++) {
        generateNewTasks(activeTasks);

        bestSchedule = geneticSolve();

        cout << "\nUpdated Schedule at step " << t << ":\n";
        for (auto &a : bestSchedule) {
            cout << "Task " << a.taskId << " -> Mechanic " << a.mechanicId << "\n";
        }
    }

    return 0;
}