#include <string>
#include <vector>
#include <iostream>
#include <regex>

using namespace std;

#define PASSWORD_LENGTH 8

// #------------------------------------------------------------------#
// |                        Argument Verification                     |
// #------------------------------------------------------------------#

bool is_unexpected_uid(string uID_string) {
    if (uID_string.length() != 6)
        return true;

     for (char c : uID_string) {
        if (!isdigit(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_aid(string aID_string) {
    if (aID_string.length() != 3) {
        return true;
    }

    for (char c : aID_string) {
        if (!isdigit(c)) {
            return true;
        }
    }

    int aID_number = atoi(aID_string.c_str());
    if (aID_number < 1) {
        return true;
    }

    return false;
}

bool is_unexpected_password(string password) {
    if (password.length() != 8)
        return true;

    for (char c : password) {
        if (!isalnum(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_value(string value) {
    if (!(value.length() >= 1 && value.length() <= 6))
        return true;

    for (char c : value) {
        if (!isdigit(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_name(string name) {
    if (!(name.length() >= 1 && name.length() <= 10))
        return true;

    for (char c : name) {
        if (!isalnum(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_timeactive(string time) {
    if (!(time.length() >= 1 && time.length() <= 5))
        return true;

    for (char c : time) {
        if (!isdigit(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_fname(string fname) {
    if (!(fname.length() >= 1 && fname.length() <= 24))
        return true;

    for (char c : fname) {
        if (!isalnum(c) && c != '-' && c != '_' && c != '.') {
            return true;
        }
    }

    size_t dotPosition = fname.find_last_of('.');
    if (dotPosition == string::npos) {
        return true;
    }

    if ((fname.length() - dotPosition) != 4) {
        return true;
    }

    return false;
}

bool is_unexpected_fsize(string fsize) {
    if (!(fsize.length() >= 1 && fsize.length() <= 8)) {
        return true;
    }

    for (char c : fsize) {
        if (!isdigit(c)) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_list_auction(vector<string> arguments) {
    if (arguments.size() % 2 != 0 || arguments.size() < 2) {
        return true;
    }

    for (int i = 0; i < arguments.size() - 1; i += 2) {
        if(is_unexpected_aid(arguments[i])) {
            return true;
        }

        if((i + 1) == (arguments.size() - 1)) {
            if (arguments[i + 1] != "0\n" && arguments[i + 1] != "1\n") {
                return true;
            } 

        } else {
            if (arguments[i + 1] != "0" && arguments[i + 1] != "1") {
                return true;
            }
        }
        
    }

    return false;
}

bool is_unexpected_date(string date) {
    regex pattern("\\d{4}-\\d{2}-\\d{2}");

    if (!regex_match(date, pattern)) {
        return true;
    }

    int year, month, day;
    sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
    if (year < 2023 || month < 1 || month > 12 || day < 1 || day > 31) {
        return true;
    }

    return false; 
}

bool is_unexpected_time(string time) {
    regex pattern("\\d{2}:\\d{2}:\\d{2}");

    if (!regex_match(time, pattern)) {
        return true;
    }

    int hour, minute, second;
    sscanf(time.c_str(), "%d:%d:%d", &hour, &minute, &second);
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return true;
    }

    return false; 
}

// #------------------------------------------------------------------#
// |                        Input Verification                        |
// #------------------------------------------------------------------#

bool is_unexpected_login_input(vector<string> arguments, bool is_from_client) {
    if (arguments.size() != 2) {
        return true;
    }

    if (is_unexpected_uid(arguments[0])) {
        return true;
    }

    if (is_unexpected_password(arguments[1])) {
        return true;
    }

    return false;
}

bool is_unexpected_logout_input(vector<string> arguments, bool is_from_client) {
    if(is_from_client) {
        if (arguments.size() != 0)
            return true;
    } else {
        return is_unexpected_login_input(arguments, is_from_client); // Same parameters
    }

    return false;
}

bool is_unexpected_unregister_input(vector<string> arguments, bool is_from_client) {
    if(is_from_client) {
        if (arguments.size() != 0)
            return true;
    } else {
        return is_unexpected_login_input(arguments, is_from_client); // Same parameters
    }

    return false;
}

bool is_unexpected_list_auctions_target_input(vector<string> arguments, bool is_from_client) {
    if(is_from_client) {
        if (arguments.size() != 0)
            return true;
    } else {
        if (arguments.size() != 1) {
            return true;
        }

        if (is_unexpected_uid(arguments[0])) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_list_auctions_input(vector<string> arguments, bool is_from_client) {
    return arguments.size() != 0;
}

bool is_unexpected_list_bids_target_input(vector<string> arguments, bool is_from_client) {
    return is_unexpected_list_auctions_target_input(arguments, is_from_client);
}

bool is_unexpected_show_record_input(vector<string> arguments, bool is_from_client) {
    if (arguments.size() != 1) {
        return true;
    }

    if (is_unexpected_aid(arguments[0])) {
        return true;
    }

    return false;
}

bool is_unexpected_close_input(vector<string> arguments, bool is_from_client) {
    if (is_from_client) {
        if (arguments.size() != 1)  
            return true;

        if (is_unexpected_aid(arguments[0])) {
            return true;
        }

    } else {
        if (arguments.size() != 3) {
            return true;
        }

        if (is_unexpected_uid(arguments[0])) {
            return true;
        }

        if (is_unexpected_password(arguments[1])) {
            return true;
        }

        if (is_unexpected_aid(arguments[2])) {
            return true;
        }
    } 

    return false;
}

bool is_unexpected_show_asset_input(vector<string> arguments, bool is_from_client) {
    if (arguments.size() != 1) {
        return true;
    }

    if (is_unexpected_aid(arguments[0])) {
        return true;
    }

    return false;
}

bool is_unexpected_bid_input(vector<string> arguments, bool is_from_client) {
    if (is_from_client) {
        if (arguments.size() != 2) 
            return true;

        if (is_unexpected_aid(arguments[0])) {
            return true;
        }

        if (is_unexpected_value(arguments[1])) {
            return true;
        }

    } else {
        if (arguments.size() != 4) {
            return true;
        }

        if (is_unexpected_uid(arguments[0])) {
            return true;
        }

        if (is_unexpected_password(arguments[1])) {
            return true;
        }

        if (is_unexpected_aid(arguments[2])) {
            return true;
        }

        if (is_unexpected_value(arguments[3])) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_open_input(vector<string> arguments, bool is_from_client) {
    int arguments_required;
    string name, value, timeactive, fname;
    if (is_from_client) {
        arguments_required = 4;
        name = arguments[0];
        fname = arguments[1];
        value = arguments[2];
        timeactive = arguments[3];

    } else {
        arguments_required = 8;
        if (arguments.size() < arguments_required || is_unexpected_uid(arguments[0]) ||
                 is_unexpected_password(arguments[1]) || is_unexpected_fsize(arguments[6])) {
            return true;
        }

        name = arguments[2];
        value = arguments[3];
        timeactive = arguments[4];
        fname = arguments[5];
    }

    return is_unexpected_name(name) || is_unexpected_value(value) || is_unexpected_timeactive(timeactive) ||
             is_unexpected_fname(fname);
}

// > -------------------- { Always False } -------------------- <

bool is_unexpected_login_input(vector<string> arguments) {
    return is_unexpected_login_input(arguments, false);
}

bool is_unexpected_logout_input(vector<string> arguments) {
    return is_unexpected_logout_input(arguments, false);
}

bool is_unexpected_unregister_input(vector<string> arguments) {
    return is_unexpected_unregister_input(arguments, false);
}

bool is_unexpected_list_auctions_target_input(vector<string> arguments) {
    return is_unexpected_list_auctions_target_input(arguments, false);
}

bool is_unexpected_list_auctions_input(vector<string> arguments) {
    return is_unexpected_list_auctions_input(arguments, false);
}

bool is_unexpected_list_bids_target_input(vector<string> arguments) {
    return is_unexpected_list_bids_target_input(arguments, false);
}

bool is_unexpected_show_record_input(vector<string> arguments) {
    return is_unexpected_show_record_input(arguments, false);
}

bool is_unexpected_close_input(vector<string> arguments) {
    return is_unexpected_close_input(arguments, false);
}

bool is_unexpected_show_asset_input(vector<string> arguments) {
    return is_unexpected_show_asset_input(arguments, false);
}

bool is_unexpected_bid_input(vector<string> arguments) {
    return is_unexpected_bid_input(arguments, false);
}

bool is_unexpected_open_input(vector<string> arguments) {
    return is_unexpected_open_input(arguments, false);
}

// #------------------------------------------------------------------#
// |                        Server Verification                       |
// #------------------------------------------------------------------#

bool is_unexpected_login_output(vector<string> arguments, string status) {
    if (arguments.size() != 0) {
        return true;
    }

    return false;
}

bool is_unexpected_logout_output(vector<string> arguments, string status) {
    if (arguments.size() != 0) {
        return true;
    }

    return false;
}

bool is_unexpected_unregister_output(vector<string> arguments, string status) {
    if (arguments.size() != 0) {
        return true;
    }

    return false;
}

bool is_unexpected_open_output(vector<string> arguments, string status) {
    if(status == "OK") {
        if (arguments.size() != 1) {
            return true;
        } 

        if (!arguments[0].empty() && arguments[0].back() == '\n') {
            arguments[0].pop_back();

        } else {
            return true;
        }

        if (is_unexpected_aid(arguments[0])) {
            return true;
        }

    } else {
        if (arguments.size() != 0) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_close_output(vector<string> arguments, string status) {
    if (arguments.size() != 0) {
        return true;
    }

    return false;
}

bool is_unexpected_myauctions_output(vector<string> arguments, string status) {
    if(status == "OK") {
        if(is_unexpected_list_auction(arguments))
            return true;

    } else {
        if (arguments.size() != 0) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_mybids_output(vector<string> arguments, string status) {
    if(status == "OK") {
        if(is_unexpected_list_auction(arguments))
            return true;

    } else {
        if (arguments.size() != 0) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_list_output(vector<string> arguments, string status) {
    if(status == "OK") {
        if(is_unexpected_list_auction(arguments))
            return true;

    } else {
        if (arguments.size() != 0) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_bid_output(vector<string> arguments, string status) {
    if (arguments.size() != 0) {
        return true;
    }

    return false;
}

bool is_unexpected_show_record_output(vector<string> arguments, string status) {
    if(status == "OK") {
        if (arguments.size() < 7) {
            return true;
        }

        if (is_unexpected_uid(arguments[0])) { 
            return true;
        }

        if (is_unexpected_name(arguments[1])) { 
            return true;
        }

        if (is_unexpected_fname(arguments[2])) { 
            return true;
        }

        if (is_unexpected_value(arguments[3])) {
            return true;
        }

        if (is_unexpected_date(arguments[4])) {
            return true;
        }

        if (is_unexpected_time(arguments[5])) {
            return true;
        }

        if(arguments.size() == 7) {
            if (!arguments[6].empty() && arguments[6].back() == '\n') {
                arguments[6].pop_back();

            } else {
                return true;
            }
        }

        if (is_unexpected_timeactive(arguments[6])) {
            return true;
        }

        int increase = 0;
        for(int i = 7; i < arguments.size(); i = i + increase) {
            if (arguments[i] == "E") {
                if(arguments.size() < (i + 4))
                    return true;
                
                if (is_unexpected_date(arguments[i + 1])) { 
                    return true;
                }

                if (is_unexpected_time(arguments[i + 2])) { 
                    return true;
                }

                if(arguments.size() == (i + 4)) {
                    if (!arguments[i + 3].empty() && arguments[i + 3].back() == '\n') {
                        arguments[i + 3].pop_back();

                    } else {
                        return true;
                    }
                }

                if (is_unexpected_timeactive(arguments[i + 3])) { 
                    return true;
                }

                increase = 4;

            } else if (arguments[i] == "B") {
                if(arguments.size() < (i + 6))
                    return true;
                
                if (is_unexpected_uid(arguments[i + 1])) { 
                    return true;
                }

                if (is_unexpected_value(arguments[i + 2])) { 
                    return true;
                }

                if (is_unexpected_date(arguments[i + 3])) { 
                    return true;
                }

                if (is_unexpected_time(arguments[i + 4])) { 
                    return true;
                }

                if(arguments.size() == (i + 6)) {
                    if (!arguments[i + 5].empty() && arguments[i + 5].back() == '\n') {
                        arguments[i + 5].pop_back();

                    } else {
                        return true;
                    }
                }

                if (is_unexpected_timeactive(arguments[i + 5])) { 
                    return true;
                }

                increase = 6;
            } else{
                return true;
            }

        }

    } else {
        if (arguments.size() != 0) {
            return true;
        }
    }

    return false;
}

bool is_unexpected_show_asset_output(vector<string> arguments, string status) {
    if(status == "OK") {
        if (arguments.size() < 3) {
            return true;
        }

        if (is_unexpected_fname(arguments[0])) { 
            return true;
        }

        if (is_unexpected_fsize(arguments[1])) {
            return true;
        }

    } else {
        if (arguments.size() != 0) {
            return true;
        }
    }

    return false;
}






