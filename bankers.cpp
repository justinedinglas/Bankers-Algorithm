#include <iostream>
#include <vector>
using namespace std;

class BankersAlgorithm {
    int n; // indicates the number of processes
    int m; // indicates the number of resource types

    // system data structures:
    vector<int> available;               // available vector: resources currently available
    vector<vector<int>> maxNeed;         // max matrix: maximum demand of each process
    vector<vector<int>> allocation;      // allocation matrix: resources currently allocated
    vector<vector<int>> need;            // need matrix: remaining resource need (max - allocation)

public:
    // initializes the system with input matrices
    BankersAlgorithm(int n, int m,
      vector<int> available,
      vector<vector<int>> maxNeed,
      vector<vector<int>> allocation)
      : n(n), m(m), available(available),
  maxNeed(maxNeed), allocation(allocation) {
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
          if (finish[p]) continue;
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

    // display all the key matrices and vectors in the banker's algorithm system when the program starts
    void displaySystemState() const {
      cout << "n = " << n << " # Number of processes\n";
      cout << "m = " << m << " # Number of resources types\n\n";

      // display the available vector
      // shows the currently available units of each resource type
      cout << "# Available Vector (initially total resources available)\n[";
      for (size_t i = 0; i < available.size(); i++) {
        cout << available[i];
        if (i != available.size() - 1) cout << ", ";
      }
      cout << "]\n\n";

      // display max matrix
      // maximum demand for each process
      cout << "# Maximum Matrix\n[";
      for (size_t i = 0; i < maxNeed.size(); i++) {
        cout << "[";
        for (size_t j = 0; j < maxNeed[i].size(); j++) {
          cout << maxNeed[i][j];
          if (j != maxNeed[i].size() - 1) cout << ", ";
        }
        cout << "]";
        if (i != maxNeed.size() - 1) cout << ",\n";
      }
      cout << "]\n\n";

      // print allocation matrix
      // resources currently allocated
      cout << "# Allocation Matrix\n[";
      for (size_t i = 0; i < allocation.size(); i++) {
        cout << "[";
        for (size_t j = 0; j < allocation[i].size(); j++) {
          cout << allocation[i][j];
          if (j != allocation[i].size() - 1) cout << ", ";
        }
        cout << "]";
        if (i != allocation.size() - 1) cout << ",\n";
      }
      cout << "]\n\n";

      // print need matrix
      // remaining resources required
      cout << "# Need Matrix (Max - Allocation)\n[";
      for (size_t i = 0; i < need.size(); i++) {
        cout << "[";
        for (size_t j = 0; j < need[i].size(); j++) {
          cout << need[i][j];
          if (j != need[i].size() - 1) cout << ", ";
        }
        cout << "]";
        if (i != need.size() - 1) cout << ",\n";
      }
      cout << "]\n\n";
    }
};
int main() {
  // number of processes and resources
  int n = 5;
  int m = 3;

  // available resources in the system
  vector<int> available = {3, 3, 2};

  // maximum demand of each process
  vector<vector<int>> maxNeed = {
      {7, 5, 3},
      {3, 2, 2},
      {9, 0, 2},
      {2, 2, 2},
      {4, 3, 3}
  };

  // current allocation of resources
  vector<vector<int>> allocation = {
      {0, 1, 0},
      {2, 0, 0},
      {3, 0, 2},
      {2, 1, 1},
      {0, 0, 2}
  };

  // create the banker’s algorithm system
  BankersAlgorithm system(n, m, available, maxNeed, allocation);
  system.displaySystemState();

  // creation of the test menu
  int choice = -1;
  while (choice != 0) {
    cout << "=== BANKER'S ALGORITHM TEST MENU ===\n";
    cout << "1. Check for safe sequence\n";
    cout << "2. User-Defined Resource Request\n";
    cout << "0. Exit\n";
    cout << "Enter your choice (0-2): ";
    cin >> choice;

    if (choice == 1) { // test case 1: run safe test
      cout << "\nTest Case 1: Run safe test\n";

      vector<int> safeSequence;

      if (system.isSafe(safeSequence)) {
        cout << "\nSystem is in a safe state.\nSafe Sequence: [ ";
        for (int p : safeSequence) cout << p << " ";
        cout << "]\n\n";
      } else cout << "System is NOT in a safe state.\n";

    } else if (choice == 2) { // test case 2
      cout << "\nTest Case 2:\n";

      int processID;
      vector<int> request(m);

      cout << "\nEnter process ID (0–" << n - 1 << "): ";
      cin >> processID;
      cout << "Enter " << m << " resource values: " << " (format: r1 r2 r3): ";

      for (int i = 0; i < m; i++) cin >> request[i];
      system.requestResources(processID, request);

    } else if (choice == 0) { // exit code
      cout << "Exiting...\n";
    }
  }
  return 0;
}