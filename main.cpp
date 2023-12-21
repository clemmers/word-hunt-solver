#include <iostream>
#include <fstream>
#include <unordered_set>
#include <set>

const int BOARD_SIZE_X = 4;
const int BOARD_SIZE_Y = 4;



char board[BOARD_SIZE_Y][BOARD_SIZE_X] = {
        {'H','R','R','I'},
        {'B','G','C','S'},
        {'H','E','I','K'},
        {'O','E','T','S'}
};







struct CompareStringLength {
    bool operator()(const std::string& str1, const std::string& str2) const {
        if (str1.length() == str2.length()) {
            return str1 > str2;
        }
        return str1.length() > str2.length();
    }
};


std::unordered_set<std::string> dictionary;
std::set<std::string, CompareStringLength> found_words;

bool are_available_words (std::string word_segment) {
    for (const auto& cur_word : dictionary) {
        if (cur_word.length() > word_segment.length() && cur_word.compare(0, word_segment.length(), word_segment) == 0) {
            return true;
        }
    }
    return false;
}

void search ( int x, int y, std::string previous = "" ) {
    char current_letter = board[y][x];
    if( dictionary.count(previous + current_letter) ) {
        found_words.insert(previous + current_letter);
    }
    if( !are_available_words(previous + current_letter) ) {
        return;
    }

    board[y][x] = '!';
    if( x != 0 ) {
        if(board[y][x - 1] != '!') { search( x-1, y, previous + current_letter); }
        if( y != 0) {
            if(board[y-1][x-1] != '!') { search( x-1, y-1, previous + current_letter); }
        }
        if( y != BOARD_SIZE_Y - 1) {
            if(board[y+1][x-1] != '!') { search( x-1, y+1, previous + current_letter); }
        }
    }

    if( x != BOARD_SIZE_X - 1 ) {
        if(board[y][x + 1] != '!') { search( x+1, y, previous + current_letter); }
        if( y != 0) {
            if(board[y-1][x+1] != '!') { search( x+1, y-1, previous + current_letter); }
        }
        if( y != BOARD_SIZE_Y - 1) {
            if(board[y+1][x+1] != '!') { search( x+1, y+1, previous + current_letter); }
        }
    }

    if ( y != 0 ) {
        if(board[y-1][x] != '!') { search( x, y-1, previous + current_letter); }
    }

    if ( y != BOARD_SIZE_Y - 1 ) {
        if(board[y+1][x] != '!') { search( x, y+1, previous + current_letter); }
    }

    board[y][x] = current_letter;
}

int main() {

    std::unordered_set<char> letters;
    for ( int i = 0; i < BOARD_SIZE_Y; i++ ) {
        for ( int j = 0; j < BOARD_SIZE_X; j++ ) {
            letters.insert(board[j][i]);
        }
    }
    std::ifstream file("dictionary.txt");
    if (!file.is_open()) {
        std::cerr << "error opening dictionary" << std::endl;
        return 1;
    }
    std::string word;
    bool flag = false;
    while (std::getline(file, word)) {
        flag = false;
        if(word.length() > 2) {
            for(int i = 0; i < word.length(); i++) {
                if(!letters.count(word[i])) {
                    flag = true;
                    break;
                }
            }
            if(flag == false) {
                dictionary.insert(word);
            }
        }
    }
    file.close();
    std::cout << "finished loading dictionary" << std::endl;

    // initiate search with each character in board
    // starting from top left
    for ( int i = 0; i < BOARD_SIZE_Y; i++ ) {
        for ( int j = 0; j < BOARD_SIZE_X; j++ ) {
            search(j, i);
        }
    }

    std::string final_words;
    for (const auto& cur_word : found_words) {
        final_words += cur_word + "\n";
    }

    std::cout << final_words << std::endl;

    std::ofstream stuff("test.txt");
    stuff << final_words;

    return 0;
}