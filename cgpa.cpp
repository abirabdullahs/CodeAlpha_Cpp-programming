#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

int main() {
    int n;
    cout << "Enter number of courses: ";
    cin >> n;

    double grade;
    int credit;

    double totalGradePoint = 0;
    int totalCredit = 0;

    vector<double> gradePoints;

  
    for (int i = 0; i < n; i++) {
        cout << "\nCourse " << i + 1 << endl;
        cout << "Grade Point: ";
        cin >> grade;

        cout << "Credit Hour: ";
        cin >> credit;

        double gradePoint = grade * credit;

        totalGradePoint += gradePoint;
        totalCredit += credit;

        gradePoints.push_back(gradePoint);
    }

  
    cout << "\n===== Course Grade Points =====\n";
    for (size_t i = 0; i < gradePoints.size(); i++) {
        cout << "Course " << i + 1 << " : " << gradePoints[i] << endl;
    }


    double gpa = totalGradePoint / totalCredit;

    cout << "\nSemester GPA = " << gpa << endl;

  
    ofstream outfile("data.txt", ios::app);

    if (outfile.is_open()) {
        outfile << totalGradePoint << " " << totalCredit << endl;
        outfile.close();
    }

    
    ifstream infile("data.txt");

    double semesterGradePoint;
    int semesterCredit;

    double grandTotalGradePoint = 0;
    int grandTotalCredit = 0;

    while (infile >> semesterGradePoint >> semesterCredit) {
        grandTotalGradePoint += semesterGradePoint;
        grandTotalCredit += semesterCredit;
    }

    infile.close();

   
    double cgpa = grandTotalGradePoint / grandTotalCredit;

    cout << "\n===== Overall CGPA =====" << endl;
    cout << cgpa << endl;

    return 0;
}