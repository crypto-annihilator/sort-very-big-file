#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <climits>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>


void generatedBigFile(std::string& str, int maxElem, long int countFilSize)
{
    srand((unsigned) time(0));
    std::fstream out(str, std::ios::out);

    for(int index = 0; index < countFilSize; index++) {
        out << (rand() % maxElem) << "\n";
    }
    out.close();
}

//return sorted file count
int generatedSortFiles(std::string& fileName, int countFilesSize)
{
    std::vector<int> arr(countFilesSize);
    std::ifstream right(fileName);
    std::string str;
    int countFile = 1;
    bool fileLineWhiteSpace = false;
    while (true) {
        for (int i = 0; !right.eof() && (i < countFilesSize); ++i) {
            std::getline(right, str);
            if (str.size() == 0) {
                fileLineWhiteSpace = true;
                break;
            }
            arr.push_back(std::stoi(str));
        }
        if (fileLineWhiteSpace) break;
        std::sort(arr.begin(), arr.end());
        std::ofstream write(std::to_string(countFile) + ".txt");
        ++countFile;
        for (auto& elem : arr) {
            write << elem << "\n";
        }
        arr.clear();
        write.close();
    }
    right.close();
    return countFile;
}

void mergeFiles(std::string& bigFileName, int countFiles)
{
    std::vector<std::fstream> sortedFiles(countFiles);
    std::unordered_map<int, bool> noteEnodOfFile;
    for (int i = 1; i < countFiles; ++i) {
        sortedFiles[i].open(std::to_string(i) + ".txt", std::ios::in | std::ios::out);
        noteEnodOfFile[i] = true;
    }
    std::fstream bigSoredFile(bigFileName, std::ios::app | std::ios::out);
    int countFinishFile = 0;
    int minElem = INT_MAX;
    std::string str;
    int currentElem;
    int currentElemAddress;
    int previousMinElemAddress;
    int lastMinElemFileIndex = -1;
    bool haveMinElem = false;
    while (true) {
        for(int i = 1; i < countFiles; ++i) {
            if (sortedFiles[i].eof()) {
                if (noteEnodOfFile[i]) {
                    noteEnodOfFile[i] = false;
                    ++countFinishFile;
                }
            }
        }
        if (countFinishFile == (countFiles - 1)) {
            for (int i = 1; i < countFiles; ++i) {
                sortedFiles[i].close();
            }
            bigSoredFile.close();
            return;
        }
        for(int i = 1; i < countFiles; ++i) {
            if (sortedFiles[i].eof()) {
                continue;
            }
            currentElemAddress = sortedFiles[i].tellp();
            std::getline(sortedFiles[i], str);
            if (str.size() == 0) {
                continue;
            }
            currentElem = std::stoi(str);
            if (minElem > currentElem) {
                if (lastMinElemFileIndex != -1) {
                    sortedFiles[lastMinElemFileIndex].seekp(previousMinElemAddress);
                }
                previousMinElemAddress = currentElemAddress;
                lastMinElemFileIndex = i;
                minElem = currentElem;
                haveMinElem = true;
            } else {
                sortedFiles[i].seekp(currentElemAddress);
            }
        }
        if (haveMinElem) {
            bigSoredFile << minElem << "\n";
            minElem = INT_MAX;
            haveMinElem = false;
        }
        lastMinElemFileIndex = -1;
    }
}

int main()
{
    std::string fileName = "big_file.txt";
    int maxElem = INT_MAX;
    int bigFileSize = std::pow(2, 26);
    int sortedFileMaxSIze = std::pow(2, 18);
    std::cout << "Maybe you want to transfer a large file? Y / N" << std::endl;
    char ch;
    std::cin >> ch;
    if (ch == 'Y') {
        std::cout << "enter file name" << std::endl;
        std::cin >> fileName;
    } else {
        generatedBigFile(fileName, maxElem, bigFileSize);
    }
    std::cout << "You want to sort on the spot? Y / N" << std::endl;
    std::string sortfileName;
    std::cin >> ch;
    if (ch == 'Y') {
       sortfileName = fileName;
    } else {
        std::cout << "enter file name" << std::endl;
        std::cin >> sortfileName;
    }
    mergeFiles(sortfileName, generatedSortFiles(fileName, sortedFileMaxSIze));
    return 0;
}
