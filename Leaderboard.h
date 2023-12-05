#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include "Text.h"
#include <SFML/Graphics.hpp>
#ifndef MINESWEEP_LEADERBOARD_H
#define MINESWEEP_LEADERBOARD_H

using namespace std;

struct Leaderboard {
    string user_name;
    string formatted_string;
    bool recent;
    int index_line;
    vector<float> time_values;

    Leaderboard() {}

    Leaderboard(string& user_name) {
        this->user_name = user_name;
        recent = false;
        index_line = 99999999999;
    }


    void formatString(string& file_content, bool& recent) {
        formatted_string = "";
        stringstream ss(file_content);
        string display_string;
        int line_number = 1;
        int max_lines = 5;

        /* Breaks up the one string that contains each records name and time in the txt file */
        while (getline(ss, display_string, '\n')) {
            if (line_number > max_lines) {
                break;
            }
            if(line_number == index_line + 1) {
                display_string = to_string(line_number++) + "." + "\t" + display_string + "*";
            } else {
                display_string = to_string(line_number++) + "." + "\t" + display_string;
            }
            int pos = display_string.find(',');
            while (pos != string::npos) {
                display_string.replace(pos, 1, "\t");
                pos = display_string.find(',', pos + 1);
            }

            formatted_string += display_string + "\n\n";
        }
        index_line = 99999999999;
    }

    void readTextFile() {
        fstream stream("files/leaderboard.txt", ios_base::in);

        if (!stream.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }

        string value;
        /* reads in the entire file */
        stringstream buffer;
        buffer << stream.rdbuf();
        string file_content = buffer.str();


        stringstream ss(file_content);
        string new_line;
        time_values.clear();

        while (getline(ss, new_line)) {
            stringstream line_stream(new_line);
            string time_string, name;
            getline(line_stream, time_string, ',');
            replace(time_string.begin(), time_string.end(), ':', '.');

            float time = stof(time_string);
            time_values.push_back(time);
        }

        formatString(file_content, recent);

        stream.close();
    }

    void writeTextFile(string& info, float& time) {
        index_line = 9999999999999;
        fstream stream("files/leaderboard.txt", ios_base::in);
        stringstream buffer;
        buffer << stream.rdbuf();
        string new_content = buffer.str();
        stringstream ss( new_content);
        string temp_line;
        time_values.clear();
        while (getline(ss, temp_line)) {
            stringstream line_stream(temp_line);
            string time_string, name;
            getline(line_stream, time_string, ',');
            replace(time_string.begin(), time_string.end(), ':', '.');

            float time = stof(time_string);
            time_values.push_back(time);
        }
        stream.close();

        int replace_index = 0;
        for (int i = 0; i < time_values.size(); i++) {
            if (time_values.at(i) >= time) {
                replace_index = i;
                break;
            }
        }
        time_values.insert(time_values.begin() + replace_index, time);
        index_line = replace_index;

        stream.open("files/leaderboard.txt", ios_base::in);
        vector<string> file_content;
        string new_line;
        while (getline(stream, new_line)) {
            file_content.push_back(new_line);
        }
        stream.close();


        stream.open("files/leaderboard.txt", ios_base::out | ios_base::trunc);
        for (int i = 0; i < file_content.size(); i++) {
            if (i == replace_index) {
                stream << info << '\n';
            }
            stream << file_content.at(i) << '\n';
        }
        stream.close();

    }

    void displayLeaderWindow(int& num_cols, int& num_rows, bool& leaderboard_check, int& game_state) {
        readTextFile();
        sf::RenderWindow leaderboardWindow(sf::VideoMode((num_cols * 16), ((num_rows * 16) + 50)), "Leaderboard Window",sf::Style::Close);
        Text leaderboard_text(20,  "files/font.ttf", "LEADERBOARD");
        Text records(20, "files/font.ttf", formatted_string);
        leaderboard_text.new_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
        leaderboard_text.setText(leaderboard_text.new_text, ((num_cols*16)/2.0f), ((((num_rows*16) + 100))/2.0f) - 120);
        records.new_text.setStyle(sf::Text::Bold);
        records.setText(records.new_text, ((num_cols*16)/2.0f), ((((num_rows*16) + 100))/2.0f) + 20);

        while (leaderboardWindow.isOpen()) {
            sf::Event event;

            while (leaderboardWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    leaderboardWindow.close();
                    leaderboard_check = false;
                }
            }
            leaderboardWindow.clear(sf::Color::Blue);
            leaderboardWindow.draw(leaderboard_text.new_text);
            leaderboardWindow.draw(records.new_text);
            leaderboardWindow.display();
        }
    }

};



#endif //MINESWEEP_LEADERBOARD_H
