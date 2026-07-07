#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    int n;
    double grade, credit;
    double totalGradePoints = 0;
    double totalCredits = 0;

    cout << "Enter the number of courses: ";
    cin >> n;

    cout << "\nEnter Grade Point and Credit Hour for each course:\n";

    for (int i = 1; i <= n; i++) {
        cout << "\nCourse " << i << endl;

        cout << "Grade Point: ";
        cin >> grade;

        cout << "Credit Hour: ";
        cin >> credit;

        totalGradePoints += grade * credit;
        totalCredits += credit;
    }

    double cgpa = totalGradePoints / totalCredits;

    cout << fixed << setprecision(2);

    cout << "\n========== RESULT ==========\n";
    cout << "Total Credit Hours : " << totalCredits << endl;
    cout << "Total Grade Points : " << totalGradePoints << endl;
    cout << "Final CGPA         : " << cgpa << endl;

    return 0;
}
