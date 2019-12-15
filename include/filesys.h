#pragma once

#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <map>

using namespace std;
using namespace boost::filesystem;

string getLatestDate(const string &date1, const string &date2) {
    unsigned int d1 = stoi(date1);
    unsigned int d2 = stoi(date2);

    if (d1 >= d2)
        return date1;
    else
        return date2;
}

void printFinFile(const path &p,
    map<string, pair <unsigned int, string>>& accounts) {

    string name = p.stem().string();
    string balance = name.substr(0, 8);
    string number = name.substr(8, 8);
    string separator = name.substr(16, 1);
    string date = name.substr(17, 8);
    string type = p.extension().string();

    if(balance == "balance_" && stoi(number) && separator == "_" && stoi(date)
    && type == ".txt") {

        cout << p.parent_path().string() + " " + p.filename().string() + '\n';

        if (accounts.find(number) == accounts.end())
            accounts[number] = pair<int, string> (1, date);
        else {
            accounts[number].first++;
            accounts[number].second =
            getLatestDate(date, accounts[number].second);
        }
    }
}

void printAccountsInfo(const path &path_to_dir,
    map<string, pair <unsigned int, string>>& accounts) {
    for (const directory_entry& obj : recursive_directory_iterator(path_to_dir)) {
        if (is_regular_file(obj.path())) {
            string name = obj.path().stem().string();
            string balance = name.substr(0, 8);
            string number = name.substr(8, 8);
            string separator = name.substr(16, 1);
            string date = name.substr(17, 8);
            string type = obj.path().extension().string();

            if(balance == "balance_" && stoi(number) && separator == "_" &&
              stoi(date) && type == ".txt" && date == accounts[number].second) {

                cout << "broker:" << obj.path().string().substr(0, obj.path()
                .string().length() - obj.path().filename().string().length())
                << " account:" + number + " files:" << accounts[number].first <<
                " lastdate:" + date + '\n';
            }
        }
    }
}

void analyse(const path& p,
     map<string ,pair <unsigned int, string>>& accounts) {
    try {
        if (exists(p)) {
            if (is_regular_file(p))
                printFinFile(p, accounts);
            else if (is_directory(p))
                cout << p << " is a directory containing:\n";
            else if (is_symlink(p)) {
                path symlinkPath = read_symlink(p);
                analyse(symlinkPath, accounts);
            }
            else
                cout << p << "exists, but is not a regular file or directory\n";
        }
        else
            cout << p << "does not exist\n";
    }
    catch (const filesystem_error& ex) {
        cout << ex.what() << '\n';
    }
}

void iterate(const path &pathToDir) {
    map<string ,pair <unsigned int, string>> accounts;
    for (const directory_entry& obj : recursive_directory_iterator(pathToDir)) {
        analyse(obj.path(), accounts);
    }
    printAccountsInfo(pathToDir, accounts);
}
