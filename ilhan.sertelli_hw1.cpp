// Ilhan Sertelli

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

// Define a function that prints the possible placements in the matrix
void print(const vector<unsigned long> & row, const vector<unsigned long> & col, const unsigned long & valid, const unsigned long & error, const unsigned long & size) {

    // Printing the valid placements one by one
    if (valid != 0) {
        cout << "Placement number " << valid << ":" << endl;
        for (unsigned long i = 0; i < row.size(); i++) {
            if (i != row.size() - 1) {
                cout << "(" << row[i] << "," << col[i] << ")" << " ";
            }
            else {
                cout << "(" << row[i] << "," << col[i] << ")" << endl;
            }
        }
    }
    // Checking if there is no placements at the end of the file
    if (error == size) {
        cout << "No placements found." << endl;
    }
}

// Defined a function in order to print all possible matches
void strProcess(const vector<vector<char>> & mat, const string & query, const unsigned long & totalLine, const unsigned long & lineLength) {
    unsigned long valid = 0, multiplier, currentLine, currentCol, errorCount = 0, tempError, matSize = totalLine * lineLength;
    vector<unsigned long> placementRow;
    vector<unsigned long> placementCol;

    // Examine every single point in the matrix whether the input string is satisfied or not
    for (unsigned long i = 0; i < totalLine; i++) {
        for (unsigned long j = 0; j < lineLength; j++) {
            tempError = 0;
            currentLine = i;
            currentCol = j;
            placementRow.clear();
            placementCol.clear();

            // Examining the string query by its characters one by one
            for (unsigned long k = 0; k < query.length(); k++) {

                // Check if the character is a digit between 1 and 9
                if (49 <= char(query.at(k)) && char(query.at(k) <= 57)) {
                    multiplier = char(query.at(k)) - 48;
                }

                    // Check if the character is either "s" or "b"
                else if ((query.at(k) == 's') || (query.at(k) == 'b')) {
                    if (query.at(k) == 's') {
                        if (multiplier + currentCol <= lineLength) {
                            for (unsigned long c = 0; c < multiplier; c++) {
                                if (mat[currentLine][currentCol] == '#') {
                                    tempError++;
                                    placementRow.clear();
                                    placementCol.clear();
                                    break;
                                }
                                else if (mat[currentLine][currentCol] == '-') {
                                    placementRow.push_back(currentLine);
                                    placementCol.push_back(currentCol);
                                    currentCol++;
                                }
                            }
                        }
                        else {
                            tempError++;
                            placementRow.clear();
                            placementCol.clear();
                            break;
                        }
                    }
                    else {
                        for (unsigned long b = 0; b < multiplier; b++) {
                            if (currentCol >= lineLength) {
                                tempError++;
                                placementRow.clear();
                                placementCol.clear();
                                break;
                            }
                            currentCol++;
                        }
                    }
                }

                    // Check if there is an error after the character is "/"
                else if (query.at(k) == '/') {
                    currentLine++;
                    currentCol = j;

                    // Check if the current line number exceeds the maximum possible line number in the given file
                    if (currentLine >= totalLine) {
                        tempError++;
                        placementRow.clear();
                        placementCol.clear();
                        break;
                    }
                }
            }
            // Checking if there is an any error that violates the rules of the placement in the matrix
            if (tempError == 0) {
                valid++;
                print(placementRow, placementCol, valid, errorCount, matSize);
            }
            else {
                errorCount++;
                if (errorCount == totalLine * lineLength) {
                    print(placementRow, placementCol, valid, errorCount, matSize);
                    break;
                }
            }
        }
    }
}

int main() {
    ifstream file;
    string name, line, query;
    unsigned long lineLength, count = 0, totalLine = 0;

    cout << "Please enter the file name: " << endl;
    cin >> name;

    file.open(name.c_str());

    // Check if there occurs an error while opening the file
    while (file.fail()) {
        cout << "Could not open the file. Please enter a valid file name: " << endl;
        file.close();
        cin >> name;
        file.open(name.c_str());
    }

    /* Check if there are any conditions that violate the format of the
     given files */
    while (getline(file, line)) {
        for (unsigned long i = 0; i < line.length(); i++) {
            if ((line.at(i) != '#') && (line.at(i) != '-')) { // Checking if there are any other characters rather than "#" and "-"
                cout << "Erroneous file content. Program terminates.";
                return 0;
            }
        }
        if (count == 0) {
            lineLength = line.length();
            count++;
        }
        else {  // Checking if all the given lines are in the same length
            if ((line.length() != lineLength)) {
                cout << "Erroneous file content. Program terminates.";
                return 0;
            }
        }
    }
    // Read the file again
    file.clear();
    file.seekg(0);

    // Start printing the file after checking the format
    cout << "The matrix file contains: " << endl;
    while (getline(file, line)) {
        cout << line << endl;
        totalLine++;
    }
    cout << "Enter the string query for a shape, or \"Quit\" to terminate the program: " << endl;
    cin >> query;

    // Checking the condition whether the user enters "Quit" as an input or not

    while (query != "Quit") {

        file.clear();
        file.seekg(0);

        vector<vector<char>> matrix(totalLine, vector<char>(lineLength));

        // Filling the 2D matrix that we have created with the characters in the given file
        count = 0;
        while (getline(file, line)) {
            for (unsigned long j = 0; j < lineLength; j++) {
                matrix[count][j] = line.at(j);
            }
            count++;
        }

        strProcess(matrix, query, totalLine, lineLength);
        cout << "Enter the string query for a shape, or \"Quit\" to terminate the program: " << endl;
        cin >> query;
    }

    file.close();

    return 0;
}
