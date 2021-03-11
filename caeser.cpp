#include <iostream>
#include <array>
#include <string>

std::string shift_string(std::string string, int amount) {
    for(int i = 0; i <= string.size(); i++) {
        string[i] = string[i]+amount;    
    }
    return string;
}

std::string read_cin() {
    std::string line, string;
    while (std::getline(std::cin, line)) {
        string.append(line);
    }
    return string;
}

int main(int argc, char* argv[]) {
    std::string string = read_cin();
    std::string argument(argv[1]);
    argument.erase(0, 1);
    std::cout << shift_string(string, std::stoi(argument)) << std::endl;
}


