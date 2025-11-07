#include <iostream>
#include <vector>
using namespace std;

class BankersAlgorithm {
    int n; // indicates the number of processes
    int m; // indactes the number of resource types

    // system data structures:
    vector<int> available;               // available vector: resources currently available
    vector<vector<int>> maxNeed;         // max matrix: maximum demand of each process
    vector<vector<int>> allocation;      // allocation matrix: resources currently allocated
    vector<vector<int>> need;            // need matrix: remaining resource need (max - allocation)

public:
    // initializes the system with input matrices
    BankersAlgorithm(int n, int m, vector<int> available, vector<vector<int>> maxNeed, vector<vector<int>> allocation)
      : n(n), m(m), available(available), maxNeed(maxNeed), allocation(allocation) {
        need.resize(n, vector<int>(m));
        calculateNeed();
    }

    void calculateNeed() {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          need[i][j] = maxNeed[i][j] - allocation[i][j];
        }
      }
    }

    // safety algorithm that ensures that the system remains in a safe state
    bool isSafe(vector<int> &safeSequence) {
      vector<int> work = available; // copy available resources
      vector<bool> finish(n, false); // track the finished processes
      safeSequence.clear(); // keeps track of the order in which processes can safely finish

      int count = 0; // number of processes that can finish safely

      while (count < n) { // keep looping until all processes are finished or no progress is possible
        bool found = false;

        for (int p = 0; p < n; p++) {
          if (!finish[p]) continue;
          bool canRun = true; // check if p can be satisfied
          for (int j = 0; j < m; j++) {
            if (need[p][j] > work[j]) {
              canRun = false;
              break;
            }
          }

          if (canRun) {
            // if process can run, add resources back to available
            for (int k = 0; k < m; k++)
              work[k] += allocation[p][k];

            finish[p] = true;
            safeSequence.push_back(p);
            count++;
            found = true;
          }
        } // if no process could run in this iteration = unsafe
        if (!found)
          return false;
      } // if processes can finish safely
      return true;
    }

    // resource-request algorithm
    void requestResources(int processID, vector<int> request) {
      cout << "# Process " << processID << " requests resources [ ";
      for (int r : request) cout << r << " ";
      cout << "]\n";

      // check if request exceeds need
      for (int i = 0; i < m; i++) {
        if (request[i] > need[processID][i]) {
          cout << "Error: Process exceeded its maximum claim.\n";
          return;
        }
      }

      // check if request exceeds available
      for (int i = 0; i < m; i++) {
        if (request[i] > available[i]) {
          cout << "Error: Not enough resources available.\n";
          return;
        }
      }

      // temporarily allocate requested resources
      for (int i = 0; i < m; i++) {
        available[i] -= request[i];
        allocation[processID][i] += request[i];
        need[processID][i] -= request[i];
      }

      // run safety algorithm
      vector<int> safeSequence;
      if (isSafe(safeSequence)) {
        cout << "System is in a safe state.\nSafe Sequence: [ ";
        for (int p : safeSequence) cout << p << " ";
        cout << "]\n";
        cout << "Resources allocated to process " << processID << ".\n";
      } else {
        // rollback if unsafe
        cout << "System is NOT in a safe state. Rolling back...\n";
        for (int i = 0; i < m; i++) {
          available[i] += request[i];
          allocation[processID][i] -= request[i];
          need[processID][i] += request[i];
        }
      }
      cout << endl;
    }
};