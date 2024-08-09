#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <cctype>

struct Property {
    std::string id = "";
    std::string name = "";
    int completionYear = 0;
    double monthlyRent = 0.0;
    std::string location = "";
    std::string propertyType = "";
    int roomCount = 0;
    int parkingSpaces = 0;
    int bathroomCount = 0;
    std::string areaSize = "";
    std::string furnishingStatus = "";
};

double parseRent(const std::string& rentStr) {
    std::string numericPart;
    for (char ch : rentStr) {
        if (isdigit(ch) || ch == '.') {
            numericPart += ch;
        }
    }
    return numericPart.empty() ? 0.0 : std::stod(numericPart);
}

int safeStringToInt(const std::string& str) {
    try {
        return std::stoi(str);
    }
    catch (const std::invalid_argument&) {
        return 0;
    }
    catch (const std::out_of_range&) {
        return 0;
    }
}

std::vector<Property> readPropertiesFromFile(const std::string& filepath) {
    std::vector<Property> properties;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Unable to open the file" << std::endl;
        return properties;
    }

    std::string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        std::stringstream ss(line);
        Property prop;
        std::string yearStr, rentStr, roomsStr, parkingStr, bathsStr;

        getline(ss, prop.id, ',');
        getline(ss, prop.name, ',');
        getline(ss, yearStr, ',');
        prop.completionYear = safeStringToInt(yearStr);
        getline(ss, rentStr, ',');
        prop.monthlyRent = parseRent(rentStr);
        getline(ss, prop.location, ',');
        getline(ss, prop.propertyType, ',');
        getline(ss, roomsStr, ',');
        prop.roomCount = safeStringToInt(roomsStr);
        getline(ss, parkingStr, ',');
        prop.parkingSpaces = safeStringToInt(parkingStr);
        getline(ss, bathsStr, ',');
        prop.bathroomCount = safeStringToInt(bathsStr);
        getline(ss, prop.areaSize, ',');
        getline(ss, prop.furnishingStatus, ',');

        properties.push_back(prop);
    }
    file.close();
    return properties;
}

int partition(std::vector<Property>& properties, int low, int high) {
    double pivot = properties[high].monthlyRent;
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (properties[j].monthlyRent < pivot) {
            i++;
            std::swap(properties[i], properties[j]);
        }
    }
    std::swap(properties[i + 1], properties[high]);
    return i + 1;
}

void quickSort(std::vector<Property>& properties, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(properties, low, high);
        quickSort(properties, low, pivotIndex - 1);
        quickSort(properties, pivotIndex + 1, high);
    }
}

void merge(std::vector<Property>& properties, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Property> leftArr(n1);
    std::vector<Property> rightArr(n2);

    for (int i = 0; i < n1; i++) {
        leftArr[i] = properties[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArr[j] = properties[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i].monthlyRent <= rightArr[j].monthlyRent) {
            properties[k] = leftArr[i];
            i++;
        }
        else {
            properties[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        properties[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        properties[k] = rightArr[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<Property>& properties, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(properties, left, mid);
    mergeSort(properties, mid + 1, right);
    merge(properties, left, mid, right);
}

int main() {
    std::string filepath = "C:\\Users\\user\\Downloads\\mudah-apartment-kl-selangor.csv";
    std::vector<Property> properties = readPropertiesFromFile(filepath);

    if (properties.empty()) {
        std::cerr << "Error: No properties loaded from the file." << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    quickSort(properties, 0, static_cast<int>(properties.size()) - 1);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> quickSortDuration = end - start;

    std::cout << "Quick Sort Duration: " << quickSortDuration.count() << " seconds" << std::endl;

    properties = readPropertiesFromFile(filepath);

    if (properties.empty()) {
        std::cerr << "Error: No properties loaded from the file." << std::endl;
        return 1;
    }

    start = std::chrono::high_resolution_clock::now();
    mergeSort(properties, 0, static_cast<int>(properties.size()) - 1);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mergeSortDuration = end - start;

    std::cout << "Merge Sort Duration: " << mergeSortDuration.count() << " seconds" << std::endl;

    return 0;
}