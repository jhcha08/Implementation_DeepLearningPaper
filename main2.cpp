#include<iostream>
using namespace std;

class scheduling {
public:
    int n, burst_time[10], arr_time[10], turnarround_time[10], waiting_time[10], rt[10], finish[10], total_wt, total_tat, total;
    void readData();
    void Init();
    void FCFS();
    void SRTF();
    void RR();
    void dispTime();
    int getNextProcess(int);

};

void scheduling::readData()
{
    cout << "Enter number of processes(maximum 10)" << endl;
    cin >> n;
    cout << "Enter the burst times in order: " << endl;
    for (int i = 0; i < n; i++)
        cin >> burst_time[i];
    cout << "Enter the arrival times in order: " << endl;
    for (int i = 0; i < n; i++)
        cin >> arr_time[i];
}

void scheduling::Init() {
    total = 0;
    total_wt = 0;
    total_tat = 0;
    for (int i = 0; i < n; i++) {
        rt[i] = burst_time[i];
        finish[i] = 0;
        waiting_time[i] = 0;
        turnarround_time[i] = 0;
        total += burst_time[i];
    }
}
void scheduling::FCFS()
{
    readData();
    Init();
    int i, j, time, q;
    for (i = 1; i < n; i++)
    {
        waiting_time[i] = 0;
        for (j = 0; j < i; j++) {
            waiting_time[i] += burst_time[j];
        }
        waiting_time[i] = waiting_time[i] - arr_time[i];
    }

    for (time = 0; time < total;)
    {
        for (i = 0; i < n; i++)
        {
            if (arr_time[i] <= time && finish[i] == 0)
            {

                cout << "(" << time << ")|==P" << (i + 1) << "==|";
                time = time + rt[i];

            }

        }

    }

    cout << "(" << total << ")" << endl;
    dispTime();

}
void scheduling::SRTF() {
    readData();
    Init();
    int time, next = 0, old, i;
    cout << "Gantt Chart " << endl;
    for (time = 0; time < total; time++)
    {
        old = next;
        next = getNextProcess(time);
        if (old != next || time == 0) {
            cout << "(" << time << ")|==P" << next + 1 << "==|";

        }
        rt[next] = rt[next] - 1;
        if (rt[next] == 0) finish[next] = 1;

        for (i = 0; i < n; i++)
            if (i != next && finish[i] == 0 && arr_time[i] <= time)
                waiting_time[i]++;

    }
    cout << "(" << total << ")" << endl;

    for (i = 0; i < n; i++)
        if (!finish[i]) { cout << "Scheduling failed, cannot continue" << endl;; return; }

    dispTime();

}

void scheduling::RR() {

    readData();
    Init();
    int time, j, q, i, dec = 0;
    cout << "Enter the time quantum: " << endl;
    cin >> q;
    cout << "Gantt Chart " << endl;
    for (time = 0; time < total;)
    {
        for (i = 0; i < n; i++)
        {
            if (arr_time[i] <= time && finish[i] == 0)
            {

                cout << "(" << time << ")|==P" << (i + 1) << "==|";
                if (rt[i] < q) {
                    dec = rt[i];
                }
                else { dec = q; }

                rt[i] = rt[i] - dec;
                if (rt[i] == 0) finish[i] = 1;
                for (j = 0; j < n; j++)
                    if (j != i && finish[j] == 0 && arr_time[j] <= time)
                        waiting_time[j] += dec;
                time = time + dec;

            }

        }

    }

    cout << "(" << total << ")" << endl;
    dispTime();

}

void scheduling::dispTime()
{
    for (int i = 0; i < n; i++)
    {

        total_wt += waiting_time[i];
        turnarround_time[i] = waiting_time[i] + burst_time[i];
        total_tat += turnarround_time[i];
        cout << "Waiting time for P" << (i + 1) << " = " << waiting_time[i] << ", Turnaround time = " << turnarround_time[i] << endl;

    }
    cout << "==================================================================================================" << endl;
    cout << "Avg Waiting time = " << (double)total_wt / n << " and Avg Turnaround time = " << (double)total_tat / n << endl;
    cout << "==================================================================================================" << endl;
    cout << "Scheduling complete" << endl << endl;
}

int scheduling::getNextProcess(int time) {

    int i, low = 0;
    for (i = 0; i < n; i++)
        if (finish[i] == 0) { low = i; break; }
    for (i = 0; i < n; i++)
        if (finish[i] != 1)
            if (rt[i] < rt[low] && arr_time[i] <= time)
                low = i;
    return low;

}



int main()
{
    scheduling s;
    int ch = 0;

    while (ch != 4)
    {
        cout << "1.FCFS(First Come - First Served)" << endl << "2.SRTF(Preemptive SJF)" << endl << "3.RR(Round Robin)" << endl << "4.Exit" << endl;
        cout << "What scheduling do you want to do? (Enter a number)" << endl << "=>";
        cin >> ch;
        switch (ch)
        {
        case 1: s.FCFS(); break;
        case 2: s.SRTF(); break;
        case 3: s.RR(); break;
        case 4: break;
        default: cout << "Invalid Option" << endl;

        }
    }

}