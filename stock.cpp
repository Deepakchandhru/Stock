#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

const int numStates = 3;
const int numActions = 2;
const double discountFactor = 0.9;
const double threshold = 0.0001;

vector<vector<vector<double>>> transitionMatrix = {
    { {0.7, 0.2, 0.1}, {0.1, 0.8, 0.1}, {0.2, 0.3, 0.5} },
    { {0.1, 0.8, 0.1}, {0.1, 0.8, 0.1}, {0.2, 0.3, 0.5} }
};

vector<vector<double>> rewardMatrix = {
    {10, -4, 3},
    {5, -3, 2}
};

vector<double> valueIteration() {
    vector<double> valueFunction(numStates, 0.0);
    vector<double> newValueFunction(numStates, 0.0);

    cout << "DP Table (Value Function Table)" << endl;
    cout << "--------------------------------" << endl;
    cout << "| Day | Bull Market | Bear Market | Stagnant Market |" << endl;
    cout << "--------------------------------" << endl;
    int day ;
    for(day=1;day<10;day++) {
        double delta = 0.0;

        for (int state = 0; state < numStates; ++state) {
            double maxActionValue = -INFINITY;

            for (int action = 0; action < numActions; ++action) {
                double actionValue = 0.0;

                for (int nextState = 0; nextState < numStates; ++nextState) {
                    actionValue += transitionMatrix[action][state][nextState] * (rewardMatrix[action][nextState] + discountFactor * valueFunction[nextState]);
                }

                if (actionValue > maxActionValue) {
                    maxActionValue = actionValue;
                }
            }

            newValueFunction[state] = maxActionValue;
            delta = max(delta, fabs(newValueFunction[state] - valueFunction[state]));
        }

        valueFunction = newValueFunction;

        cout << "| " << day << "   ";
        for (int state = 0; state < numStates; ++state) {
            cout << "| " << valueFunction[state] << "   \t";
        }
        cout << "|" << endl;

    }

    cout << "--------------------------------" << endl;

    return valueFunction;
}

vector<int> extractPolicy(const vector<double>& valueFunction) {
    vector<int> policy(numStates, 0);

    for (int state = 0; state < numStates; ++state) {
        double maxActionValue = -INFINITY;
        int bestAction = 0;

        for (int action = 0; action < numActions; ++action) {
            double actionValue = 0.0;

            for (int nextState = 0; nextState < numStates; ++nextState) {
                actionValue += transitionMatrix[action][state][nextState] * (rewardMatrix[action][nextState] + discountFactor * valueFunction[nextState]);
            }

            if (actionValue > maxActionValue) {
                maxActionValue = actionValue;
                bestAction = action;
            }
        }

        policy[state] = bestAction;
    }

    return policy;
}

int main() {
    vector<double> optimalValueFunction = valueIteration();
    vector<int> optimalPolicy = extractPolicy(optimalValueFunction);
    string s[3]={"Bull market", "Bear market","Stagnant Market"};

    cout << "Optimal Policy:" << endl;
    for (int state = 0; state < numStates; ++state) {
        cout  << s[state] << ": " << (optimalPolicy[state] == 0 ? "Invest in Stocks" : "Invest in Bonds") << endl;
    }

    return 0;
}
