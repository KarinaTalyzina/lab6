#include <thread> // ��� ������ � ��������
#include <iostream>
#include <vector>
#include <mutex> //��� ������ � ������������ (��� �������������� ��������� �����)

using namespace std;

mutex coutMutex; // ���������� ������ ����������

// ��� ���������, ������������ � ����������� ����������
void print_hello(int id, double &sum)
{
    // ������� ����������
    coutMutex.lock();
    // ������� ID ������
    sum += id;
    cout << "Thread: " << id << " sum: " << sum << endl;
    coutMutex.unlock();
}

// ������ ���� � ������������
int main()
{
    double sum = 0;
    cout << "void main" << endl;
    vector<thread> threads;
    for (int i = 0; i < 8; i++)
    {
        // ������� �� ������ ��������
        threads.push_back(thread(print_hello, i, ref(sum)));
    }
    // ������� ������������� ������
    for (auto &th : threads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }

    return 0;
}

// ������� ������ �������
//  ������ ����� � ��������� �����, ������� ����� ����� ���������� �� ���������, �������� ���� ����� ���: Hello from thread 2Hello from thread 2
//  int main()
//  {
//      vector<thread> threads;
//      for (int i=0; i<8; i++)
//      {
//          threads.push_back(thread(print_hello));
//      }
//      cout <<"void main"<<endl;
//      //������� ������������� ������
//      for (auto& th: threads)
//      {
//          if (th.joinable())
//          {
//              th.join();
//          }
//      }

//     return 0;
// }

// �������� ������

// int main()
// {
//     cout <<"void main"<<endl;
//     thread thread2(print_hello);
//     thread2.join();
//     return 0;
// }