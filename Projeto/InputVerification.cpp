#include <string>
#include <vector>
#include <iostream>

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