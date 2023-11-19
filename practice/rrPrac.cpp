#include <bits/stdc++.h>
using namespace std;

class Process
{

    int at, bt, ct, tat, wt, rt;
    string pid;

public:
    Process(int at, int bt, string pid)
    {
        this->at = at;
        this->bt = bt;
        this->rt = bt;
        ct = -1;
        tat = -1;
        wt = -1;
        this->pid = pid;
    }

    friend class Scheduling;
};

class Scheduling
{

public:
    vector<Process> Processes;
    float total_turnaround_time = 0, total_waiting_time = 0;

    void takeinput(int n)
    {

        for (int i = 0; i < n; i++)
        {
            string pid = "p" + to_string(i + 1);
            cout << "Enter Arrival Time of process " << i + 1 << ": ";
            int at;
            cin >> at;
            cout << "Enter Burst time of process " << i + 1 << ": ";
            int bt;
            cin >> bt;
            Process p(at, bt, pid);
            Processes.push_back(p);
        }
    }

    void calculateCT()
    {

        sort(Processes.begin(), Processes.end(), [](const Process &a, const Process &b)
             { return a.at < b.at; });

        vector<Process> vec;
        queue<Process> q;

        cout << "Enter time quantum : ";
        int tq;
        cin >> tq;
        int currentTime = 0;

        while (!q.empty() || !Processes.empty())
        {

            while (Processes.front().at <= currentTime && !Processes.empty())
            {
                q.push(Processes.front());
                Processes.erase(Processes.begin());
            }

            if (!q.empty())
            {

                Process p = q.front();
                q.pop();

                currentTime += min(tq, p.rt);
                p.rt -= min(tq, p.rt);

                if (p.rt == 0)
                {
                    p.ct = currentTime;
                    vec.push_back(p);
                }
                else
                {
                    while (Processes.front().at <= currentTime && !Processes.empty())
                    {
                        q.push(Processes.front());
                        Processes.erase(Processes.begin());
                    }
                    q.push(p);
                }
            }
            else
            {
                currentTime++;
            }
        }

        Processes = vec;
    }

    void calculateTAT()
    {
        for (int i = 0; i < Processes.size(); ++i)
        {
            Processes[i].tat = Processes[i].ct - Processes[i].at;
        }
    }

    void calculateWT()
    {
        for (int i = 0; i < Processes.size(); ++i)
        {
            Processes[i].wt = Processes[i].tat + Processes[i].bt;
        }
    }

    void display()
    {

        sort(Processes.begin(), Processes.end(), [](const Process &a, const Process &b)
             { return a.pid < b.pid; });

        cout << "\n\nProcess   Arrival Time  Burst Time  Completion Time  TurnAroundTime  Waiting Time";
        for (auto p : Processes)
        {

            cout << endl
                 << setw(8) << left << p.pid << setw(12) << right << p.at
                 << setw(14) << right << p.bt << setw(16) << right << p.ct
                 << setw(15) << right << p.tat << setw(13) << right << p.wt;
        }
    }
};

int main()
{

    Scheduling obj;

    cout << "Enter no of processes: ";
    int n;
    cin >> n;

    obj.takeinput(n);
    obj.calculateCT();
    obj.display();
    return 0;
}
